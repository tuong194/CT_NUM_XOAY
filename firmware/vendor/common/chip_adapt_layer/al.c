/********************************************************************************************************
 * @file	al.c
 *
 * @brief	for TLSR chips
 *
 * @author	BLE GROUP
 * @date	2020.06
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"
#include "proj_lib/mesh_crypto/aes_cbc.h"
#include "../mesh_common.h"

extern  u8 ble_devName[];

STATIC_ASSERT(BIT_IS_POW2(ACL_RX_FIFO_NUM));
STATIC_ASSERT(BIT_IS_POW2(ACL_TX_FIFO_NUM - 1));
#if (MCU_CORE_TYPE == MCU_CORE_9518)  //only Eagle have this limitation
STATIC_ASSERT(ACL_TX_FIFO_SIZE * (ACL_TX_FIFO_NUM - 1) <= 4096); // return error with LL_ACL_TX_BUF_SIZE_MUL_NUM_EXCEED_4K for blc_controller_check_appBufferInitialization_()
STATIC_ASSERT(ACL_TX_FIFO_NUM <= 33);	// for lib. // no number limitation for RX buffer which is manual mode.
#endif

STATIC_ASSERT(BIT_IS_POW2(SCAN_PRICHN_RXFIFO_NUM));
STATIC_ASSERT(SCAN_PRICHN_RXFIFO_SIZE % 16 == 0);
STATIC_ASSERT(!(AUDIO_MESH_EN && SPEECH_ENABLE));	// can not enable at the same time now.


// just for the compile part 
_attribute_ram_code_ void gpio_set_func(u32 pin, u32 func)
{
	if(func == AS_GPIO){
		gpio_function_en(pin);
	}else{
		gpio_function_dis(pin);
	}
}


void aes_ecb_encryption(u8 *key, u8 len, u8 *plaintext, u8 *encrypted_data)
{
	tn_aes_128(key, plaintext, encrypted_data);
}

void aes_ecb_decryption(u8 *key, u8 *encrypted_data, u8 *decrypted_data)
{
	#if 1
	aes_decrypt(key, encrypted_data, decrypted_data);
	#else
	u8 rencrypted_data[16], rkey[16];
	swapX(key, rkey, 16);
	swapX(encrypted_data, rencrypted_data, 16);
	aes_ll_decryption(rkey, rencrypted_data, decrypted_data); // aes_ll_decryption has been changed to the same as aes_decrypt_.
	#endif
}

#if !SUPPORT_PFT_ARCH	// add by qifa
void flash_en_support_arch_flash(unsigned char en)
{
	g_plic_preempt_en = en;
	flash_plic_preempt_config(en, 1);
}
#else
void flash_en_support_arch_flash(unsigned char en){}
#endif


int otaRead(void * p)
{
	return 0;
}

ble_sts_t bls_att_setDeviceName(u8* pName,u8 len)
{
	memset(ble_devName, 0, MAX_DEV_NAME_LEN );
	memcpy(ble_devName, pName, len < MAX_DEV_NAME_LEN ? len : MAX_DEV_NAME_LEN);

	return BLE_SUCCESS;
}

u8		bls_ll_isConnectState (void)
{
	return	 (get_blt_state() == BLS_LINK_STATE_CONN);
}

ble_sts_t	bls_att_pushNotifyData (u16 attHandle, u8 *p, int len)
{
	return blc_gatt_pushHandleValueNotify(BLS_CONN_HANDLE, attHandle, p, len);
}

void	mz_mul2 (unsigned int * r, unsigned int * a, int na, unsigned int b)
{
#if 0
	int i;
	u64 m;
	u64 c = 0;
	for (i=0; i<na; i++)
	{
		c += r[i];
		m = a[i] * (u64)b + c;
		r[i] = (unsigned int )m;
		c = m >> 32;
	}
	r[i]= (unsigned int )c;
#else
	int i;
	unsigned long long m;
	unsigned long long c = 0;
	for (i=0; i<na; i++)
	{
		c += r[i];
		m = (unsigned long long)(a[i] * b) + c;
		r[i] = m;
		c = m >> 32;
	}
	r[i]= c;
#endif
}

int blt_ota_writeBootMark(void);
int ota_set_flag()
{
	return blt_ota_writeBootMark();
}

u8 ota_reboot_flag = 1;// initial it will reboot 
u8 fw_ota_value = 0;
#if (GATEWAY_ENABLE)
void set_ota_reboot_flag(u8 flag)
{
	ota_reboot_flag = flag;
	return ;
}

u8 get_fw_ota_value()
{
	return fw_ota_value;
}
#endif

_attribute_data_retention_ 	l2cap_handler_t blc_l2cap_handler_pre_cb =0;	// add by weixiong
#if BLE_SRC_TELINK_MESH_EN // by haiwen mi api
void blc_l2cap_register_pre_handler(void *p)	// add by weixiong
{
	blc_l2cap_handler_pre_cb = p;
}
#endif

// ----------- EXTENDED ADV -----------
mesh_adv_extend_module_t *mesh_adv_extend_module = NULL;
u32 irq_rx_aux_adv_tick;

#if EXTENDED_ADV_ENABLE
#define reg_rf_chn_current	REG_ADDR8(0x14080d)	// refer to rf_set_ble_channel()
#define RAW_PKT_INDEX_AUX_AD_TYPE			(3)	// 

aux_payload_t g_aux_payload = {0};

/*extern */u8				blm_scan_type;
extern ll_ext_adv_t		*blt_pextadv;  	//global adv_set data pointer
extern ll_ext_adv_t		*p_ext_adv;     //for immediate use

STATIC_ASSERT(sizeof(rf_pkt_adv_ext_ind_2) <= sizeof(rf_pkt_pri_adv_t));

static inline int  blt_packet_crc24 (unsigned char *p, int n, int crc)
{
	//crc16¡êo G(X) = X16 + X15 + X2 + 1

	//crc16:  x24 + x10 + x9 + x6 + x4 + x3 + x + 1 ==> 0x00065b
	//               13   14   17   19   20  22  23 ==> 0xda6000
	// static unsigned short poly[2]={0, 0xa001};              //0x8005 <==> 0xa001

	//int  poly[2]={0, 0xda6000};              //0x8005 <==> 0xa001
	for (int j=0; j<n; j++) {
		u8 ds = p[j];
		for (int i=0; i<8; i++) {
			int cond = (crc ^ ds ) & 1;
			crc >>= 1;
			if (cond) {
			 	crc ^= 0xda6000;
			}
			//crc = (crc >> 1) ^ poly[(crc ^ ds ) & 1];
			ds = ds >> 1;
		}
	}
	return crc;
}

_attribute_ram_code_ int is_crc_ok(u8 *raw_pkt)
{
	int crc_ok = 0;
	int crc_seed = 0xaaaaaa;
	int crc = blt_packet_crc24(raw_pkt+DMA_RFRX_OFFSET_HEADER, raw_pkt[DMA_RFRX_OFFSET_RFLEN]+2, crc_seed);
	u8 *p_crc_cal = (u8 *)&crc;
	u8 *p_crc_pkt = raw_pkt + DMA_RFRX_OFFSET_CRC24(raw_pkt);
	if((p_crc_cal[0] == p_crc_pkt[0])&&(p_crc_cal[1] == p_crc_pkt[1])&&(p_crc_cal[2] == p_crc_pkt[2])){
		crc_ok = 1;
	}

	return crc_ok;
}

_attribute_ram_code_ u8 mesh_blc_rx_aux_adv(u8 * raw_pkt, u8 * new_pkt)
{
	rf_packet_adv_t * pAdv = (rf_packet_adv_t *) (raw_pkt + DMA_RFRX_LEN_HW_INFO);
	
	u8 adv_type = pAdv->header.type;
	
	if (adv_type != LL_TYPE_ADV_EXT_IND) {
		return 1;
	}
	
	if(((pAdv->rf_len != 7) && (pAdv->rf_len != 7 + sizeof(aux_payload_t))) || !is_crc_ok(raw_pkt)){  // add by weixiong, receive abnormal adv, confirm later
		return 0;
	}

    rf_pkt_adv_ext_ind_2 *adv_ext_ind = (rf_pkt_adv_ext_ind_2 *)raw_pkt;
	int flag_aux_payload_with_src_sno = 0;
	if(adv_ext_ind->rf_len == adv_ext_ind->ext_hdr_len + 1 + sizeof(aux_payload_t)){
		aux_payload_t *p_aux = (aux_payload_t *)adv_ext_ind->data;
		bear_head_src_sno_t *p_head = &p_aux->head_src_sno;
		if((DATA_TYPE_MANUFACTURER_SPECIFIC_DATA == p_aux->ad_type) && 
			(BEAR_TX_PAR_TYPE_SRC_SNO == p_head->bear_par_type) && 
			(g_vendor_id == p_aux->vnd_id)){
			extend_adv_aux_src_sno_xor(p_aux); // get original source address.
			foreach(i, g_cache_buf_max){
				if(is_own_ele(p_head->src_addr)){
					return 0;
				}
				
				if(cache_buf[i].src == p_head->src_addr){
					u8 sno_cache = (cache_buf[i].sno & 0xff);
					u8 delta = p_head->sno_low - sno_cache;
				   	if((0 == delta) || delta > 0x80){
				   		/* when >3 nodes send MIC messages, relay messages will be too much, and will be later than a new message. 
				   		so if only check by "=", filter will be fail.*/
		    			return  0; // filter old  payload
		    		}
		  		}
			}
			flag_aux_payload_with_src_sno = 1;
		}
	}
	
	u32 timeStamp = reg_rf_timestamp;
    u8	next_buffer = 0;
    u8 channel = adv_ext_ind->chn_index;
    u32 time_offset = (adv_ext_ind->offset_unit?(sys_tick_per_us*300):(sys_tick_per_us*30))
        * adv_ext_ind->aux_offset;

    u8 channel_backup = reg_rf_chn_current;
	u8 rf_rxtmax_backup = reg_rf_rxtmaxlen;
    rf_set_tx_rx_off ();
	rf_set_rx_maxlen(255);

    //blt_rxfifo.wptr--;
	ble_rf_set_rx_dma((u8*)raw_pkt, bltParam.acl_rx_dma_size); // reg_dma_rf_rx_addr = (u16)(u32)raw_pkt;
    volatile u32 *ph  = (u32 *) (raw_pkt + DMA_RFRX_OFFSET_HEADER);
    *ph = 0;

    rf_set_ble_channel (channel);

    CLEAR_ALL_RFIRQ_STATUS;
	u32 rx_begin_tick = clock_time();
	while((timeStamp + time_offset - 200*sys_tick_per_us) - clock_time() < BIT(31));
    rf_set_rxmode ();
    //rf_start_srx(tick_now + time_offset - 50*sys_tick_per_us);

    
    reg_rf_irq_status = FLD_RF_IRQ_RX;  //clear

    // my_printf_uart("start\r\n");

    while (!(*ph) && (u32)(clock_time() - rx_begin_tick) < time_offset + ((1+4+2)*8+50) * sys_tick_per_us); //150 + pkt(22*8) + 150 + margin(50)

    //my_printf_uart("start receive chn=%2d, backup=%2d\r\n", channel, channel_backup);
    if (*ph)
    {
        u32 rx_begin_tick = clock_time ();                                          // (31+10)*8 + margin(50)
        while (!(reg_rf_irq_status & FLD_RF_IRQ_RX) && (clock_time() - rx_begin_tick) < (8*((1+4+2+256)+50) * sys_tick_per_us));

        STOP_RF_STATE_MACHINE;
        reg_rf_irq_status = FLD_RF_IRQ_RX;

        if(RF_BLE_PACKET_VALIDITY_CHECK(raw_pkt)){ //CRC OK
            //blt_rxfifo.wptr++;
			ble_rf_set_rx_dma((u8*)new_pkt, bltParam.acl_rx_dma_size); // reg_dma_rf_rx_addr = (u16)(u32)new_pkt;

            // AUX
            next_buffer = 1;
            raw_pkt[2] = 0xAA;
            raw_pkt[RAW_PKT_INDEX_AUX_AD_TYPE] = flag_aux_payload_with_src_sno ? BEAR_TX_PAR_TYPE_SRC_SNO : 0; // 0 means no payload for primary adv of aux.
            //pAdv = (rf_packet_adv_t *) (raw_pkt + DMA_RFRX_LEN_HW_INFO);
            // my_printf_uart("received=%2d backup=%2d\r\n", pAdv->rf_len, channel_backup);
        }else{
            // my_printf_uart("received error=%2d\r\n", pAdv->rf_len);
        }
    }

	irq_rx_aux_adv_tick = clock_time()|1;
    rf_set_tx_rx_off ();
	rf_set_rx_maxlen(rf_rxtmax_backup);
    rf_set_ble_channel(channel_backup);
    CLEAR_ALL_RFIRQ_STATUS;
    rf_set_rxmode ();

    return next_buffer;
}


int mesh_blc_rx_aux_handler(u8 *raw_pkt, s8 rssi_pkt)
{
	u8 t = raw_pkt[DMA_RFRX_OFFSET_HEADER] & 15;
	if (t == LL_TYPE_ADV_EXT_IND) {
		u8 tmp[320];
		event_adv_report_t * pa = (event_adv_report_t *)tmp;
		u32 timeStamp = 0;
		#define ADV_LEN_MAX     (255)
		u8 len = raw_pkt[DMA_RFRX_OFFSET_RFLEN];
		if((len >= 6+2) && (len <= ADV_LEN_MAX)&&(raw_pkt[DMA_RFRX_OFFSET_RFLEN+1+10]<=(ADV_LEN_MAX - 7))){// first len in the payload
			
			memcpy(&timeStamp, raw_pkt+DMA_RFRX_OFFSET_TIME_STAMP(raw_pkt),sizeof(timeStamp)); // back up
			pa->subcode = HCI_SUB_EVT_LE_ADVERTISING_REPORT;
			pa->nreport = (raw_pkt[RAW_PKT_INDEX_AUX_AD_TYPE] >= BEAR_TX_PAR_TYPE_SRC_SNO) ? raw_pkt[3] : 1; // have checked (raw_pkt[2] == 0xAA) before.
			pa->event_type = LL_TYPE_ADV_NONCONN_IND;
			pa->adr_type = raw_pkt[DMA_RFRX_OFFSET_HEADER] & BIT(6) ? 1 : 0;

			pa->len = len - 10;
			memcpy (pa->mac, raw_pkt+DMA_RFRX_OFFSET_DATA+2, 6);
			memcpy(pa->data, raw_pkt+DMA_RFRX_OFFSET_DATA+10, pa->len);
			pa->data[pa->len] = (u8)rssi_pkt;
			
			#if (0) // not use here, refer to blc_ll_procScanData_
			//-- get frequency offset --
			//cordic_freq = 1000000 //250k
			//cordic_freq = 2000000 //500k
			//cordic_freq = 4000000 //ble1m, blelr
			//cordic_freq = 8000000 //ble2m, zb
			//fOff = dc * cordic_freq / 256 / 2 / 3.14159 / 1000 (KHz)
			s16  freq_offset = raw_pkt[DMA_RFRX_OFFSET_FREQ_OFFSET(raw_pkt)] | (raw_pkt[DMA_RFRX_OFFSET_FREQ_OFFSET(raw_pkt)+1] << 8);
			extern RF_ModeTypeDef g_RFMode;

			if(g_RFMode == RF_MODE_BLE_1M || g_RFMode == RF_MODE_LR_S2_500K || \
			   g_RFMode == RF_MODE_BLE_1M_NO_PN || g_RFMode == RF_MODE_LR_S8_125K){
				freq_offset = (s32)freq_offset * 400000000/(256*2*314159);
			}
			else if(g_RFMode == RF_MODE_BLE_2M || g_RFMode == RF_MODE_BLE_2M_NO_PN){
				freq_offset = (s32)freq_offset * 800000000/(256*2*314159);
			}
			else{

			}
			pa->data[pa->len + 1] = freq_offset;
			pa->data[pa->len + 2] = freq_offset >> 8;
			#endif
			memcpy(&pa->data[pa->len + 3], &timeStamp, 4);
			
			if(hci_le_eventMask & HCI_LE_EVT_MASK_ADVERTISING_REPORT)
			{
				//my_printf_uart_hexdump(pa, len + (blm_scan_type & 0x80 ? 8 : 6));
				blc_hci_send_event (HCI_FLAG_EVENT_BT_STD | HCI_EVT_LE_META, (u8 *)pa, len + (blm_scan_type & 0x80 ? 8 : 6));
			}
		}
	}
	return 0;
}

void mesh_blt_send_adv2()
{
    void mesh_blc_ll_setExtAdvData(u8 adv_pdu_len, u8 *data);
    mesh_blc_ll_setExtAdvData(pkt_adv.rf_len - 6, pkt_adv.data);
    p_ext_adv = blt_pextadv;
    blt_send_adv2();
}

static mesh_adv_extend_module_t adv_extend_module = {
	.rx_aux_adv = mesh_blc_rx_aux_adv,
	.send_extend_adv = mesh_blt_send_adv2,
	.rx_adv_handler = mesh_blc_rx_aux_handler,
};
#endif

void extend_adv_aux_src_sno_xor(aux_payload_t *p_aux)
{
	u8 *p_src_u8 = (u8 *)&p_aux->head_src_sno.src_addr;
	u8 *p_ad_type_u8 = &p_aux->ad_type;
	int len = sizeof(bear_head_src_sno_t) - OFFSETOF(bear_head_src_sno_t, src_addr);
	foreach(i, len){
		p_src_u8[i] ^= p_ad_type_u8[i];
	}
}

void relay_check_and_add_extend_adv_src_sno(mesh_cmd_bear_t *p_bear_enc, mesh_cmd_nw_t *p_nw_dec, u8 *p_payload, int src_type)
{
#if EXTENDED_ADV_ENABLE
	if(MESH_BEAR_ADV == src_type){
		if(p_bear_enc->len > (ADV_MAX_DATA_LEN - 1)){
			event_adv_report_t *pa = CONTAINER_OF(p_payload,event_adv_report_t,data[0]); // packet is from adv channel, so there is event_adv_report_t here.
			//LOG_MSG_LIB(TL_LOG_NODE_SDK, (u8 *)&p_bear_enc->len, 16,"test relay audio, nreport:0x%0x, src_type:0x%0x: ", pa->nreport, src_type);
			if(BEAR_TX_PAR_TYPE_SRC_SNO == pa->nreport){
				bear_head_src_sno_t *p = (bear_head_src_sno_t *)&p_bear_enc->tx_head;
				p->bear_par_type = BEAR_TX_PAR_TYPE_SRC_SNO;
				p->src_addr = p_nw_dec->src;
				p->sno_low = p_nw_dec->sno[0];
			}
		}
	}
#endif
}

ble_sts_t blc_ll_setScanEnable (scan_en_t scan_enable, dupFilter_en_t filter_duplicate)
{
	if(NULL == ll_enter_pri_chn_scan_cb){
		//return LL_ACL_RX_BUF_NO_INIT;	// can not return, because app_enable_scan_all_device() is before scan init.
	}

	if(scan_enable){
		blts.scan_extension_mask |= BLS_FLAG_SCAN_IN_ADV_MODE;
		blts.scan_extension_mask |= BLS_FLAG_SCAN_IN_SLAVE_MODE;
	}else{
		u32 r = irq_disable();
		blts.scan_extension_mask &= ~BLS_FLAG_SCAN_IN_ADV_MODE;
		blts.scan_extension_mask &= ~BLS_FLAG_SCAN_IN_SLAVE_MODE;

		if((BLS_LINK_STATE_CONN == get_blt_state()) && (BLE_STATE_PRICHN_SCAN_S == get_ble_state())){
			bltParam.ble_state = BLE_STATE_BRX_E;	// must restore, if not, irq_slave_system_timer() will not handle gatt interval irq tick.
		}
		irq_restore(r);
	}
	return BLE_SUCCESS;
}

void set_scan_interval(u32 ms)
{
	 bltScn.scnInterval_tick = ms * 1000 * SYSTEM_TIMER_TICK_1US; // blts_scan_interval
}

_attribute_ram_code_ int  blc_ll_procScanPkt_mesh(u8 *raw_pkt, u8 *new_pkt, u32 tick_now)
{
	return adv_filter_proc(raw_pkt, get_blt_state());
}

/**
 * @brief      callback function of LinkLayer Event "BLT_EV_FLAG_SUSPEND_EXIT"
 * @param[in]  e - LinkLayer Event type
 * @param[in]  p - data pointer of event
 * @param[in]  n - data length of event
 * @return     none
 */
_attribute_ram_code_ void	task_suspend_exit (u8 e, u8 *p, int n)
{
	rf_set_power_level_index (my_rf_power_index);
}

ll_procScanPkt_callback_t  blc_ll_procScanPktCb = NULL;
u16 g_SCAN_PRICHN_RXFIFO_SIZE = SCAN_PRICHN_RXFIFO_SIZE; // move to scan_fifo_t later
u16 g_SCAN_PRICHN_RXFIFO_NUM = SCAN_PRICHN_RXFIFO_NUM;

void blc_ll_initScanning_module_mesh(void)	// do not use blc_ll_initScanning_module, because scan request is not required.
{
	blt_ll_initScanState();
	blc_ll_procScanPktCb = blc_ll_procScanPkt_mesh;
#if BLT_ADV_IN_CONN_SLAVE_EN
	//blc_ll_addAdvertisingInConnSlaveRole(); // no need this mode.
#endif

	//smemcpy(pkt_scan_req.scanA, bltMac.macAddress_public, BLE_ADDR_LEN);
	set_scan_interval(EXTENDED_ADV_ENABLE ? 50 : 9);

#if EXTENDED_ADV_ENABLE
	mesh_adv_extend_module = &adv_extend_module;
#endif
	
	bls_app_registerEventCallback (BLT_EV_FLAG_SUSPEND_EXIT, &task_suspend_exit);	// b85m has added in lib.
}

// ------ USB ------
#define			USB_ENDPOINT_BULK_IN			8
#define			USB_ENDPOINT_BULK_OUT			5
#define			USB_ENDPOINT_BULK_OUT_FLAG		(1 << (USB_ENDPOINT_BULK_OUT & 7))
//0x800110(reg_usb_ep8_ptr),0x800118(reg_usb_ep8_dat),0x800120(reg_usb_ep8_ctrl),0x800139(reg_usb_irq),0x80013d(reg_usb_ep8_fifo_mode),

static inline u32 usb_endpoint_busy(u32 ep) {
	write_reg8 (0x10083d, 0);
	return read_reg8 (0x100820 + (ep&7)) & 1;
}

void usb_bulk_drv_init (void *p)
{
	// usb_p_bulkout_func = (usb_bulk_out_callback_t) p;

	write_reg8 (0x100820 + (USB_ENDPOINT_BULK_OUT & 7), 1);		//ACK
}

// 0: last packet; 72: max size packet; -1: busy
int usb_bulk_in_packet (u8 *p, int n)
{
	static u32 offset = 0;

	if( usb_endpoint_busy(USB_ENDPOINT_BULK_IN))
		return -1;

	int data_unfinish = 0;

	if (n <= 64)
	{
		offset = 0;
	}
	else
	{
		n -= offset;
		if (n > 64)
		{
			n = 64;
			data_unfinish = 1;  //data not over
		}
	}

	//reset pointer to 0
	write_reg8 (0x100810 + (USB_ENDPOINT_BULK_IN & 7), 0);
	for (int i=0; i<n; i++)
	{
		write_reg8 (0x100818 + (USB_ENDPOINT_BULK_IN & 7), p[offset + i]);
	}
	write_reg8 (0x100820 + (USB_ENDPOINT_BULK_IN & 7), 1);		//ACK

	offset = data_unfinish ? offset + 64 : 0;

	return offset;
}

int usb_bulk_out_get_data (u8 *p)
{
	if (read_reg8 (0x100839) & USB_ENDPOINT_BULK_OUT_FLAG)
	{
		//clear interrupt flag
		write_reg8 (0x100839, USB_ENDPOINT_BULK_OUT_FLAG);

		// read data
		int n = read_reg8 (0x100810 + (USB_ENDPOINT_BULK_OUT & 7));
		write_reg8 (0x100810 + (USB_ENDPOINT_BULK_OUT & 7), 0);
		for (int i=0; i<n; i++)
		{
			p[i] = read_reg8 (0x100818 + (USB_ENDPOINT_BULK_OUT & 7));
		}
		write_reg8 (0x100820 + (USB_ENDPOINT_BULK_OUT & 7), 1);		//ACK

		return n;
	}
	return 0;
}

int blc_hci_tx_to_usb ()
{
	u8 *p = my_fifo_get (&hci_tx_fifo);
	if (p)
	{
		//extern int usb_bulk_in_packet (u8 *p, int n);
		if (usb_bulk_in_packet (p + 2, p[0] + p[1] * 256) == 0)
		{
		my_fifo_pop (&hci_tx_fifo);
		}
	}
	return 0;
}

#if (DUMP_STR_EN)
#define	USB_LOG_TX_FIFO_SIZE		272
#define	USB_LOG_TX_FIFO_NUM			32
_attribute_no_retention_bss_ u8 		usb_log_txfifo_b[USB_LOG_TX_FIFO_SIZE * USB_LOG_TX_FIFO_NUM];
_attribute_no_retention_data_ my_fifo_t	usb_log_txfifo = {
													USB_LOG_TX_FIFO_SIZE,
													USB_LOG_TX_FIFO_NUM,
													0,
													0,
													usb_log_txfifo_b,};
#endif

#if BLE_SRC_TELINK_MESH_EN
u32 adv_in_conn_tick = 0;
u8 adv_in_conn_tick_init_flag = 0;

int rf_link_time_allow (u32 us)	// add by weixiong
{
	u32 t = reg_system_tick_irq - clock_time () - us * sys_tick_per_us;
	return t < BIT(30);
}

void ble_loop_send_adv_in_gatt()
{
	if((BLS_LINK_STATE_CONN == get_blt_state()) && (BLE_STATE_BRX_S != get_ble_state()) && rf_link_time_allow((mesh_adv_extend_module==NULL) ? 3000 : 5000)){ // generic off 1.6ms, max payload is 1.8ms
	    if(clock_time_exceed(adv_in_conn_tick, blta.adv_interval/sys_tick_per_us)){
	        if(adv_in_conn_tick_init_flag || clock_time_exceed(adv_in_conn_tick, ((blta.adv_interval/sys_tick_per_us) + 8*1000))){ // 8*1000: because need 13ms to  handle onoff - rsp when 16MHz 
                adv_in_conn_tick = clock_time(); // fix the case which duration is too long.
                adv_in_conn_tick_init_flag = 0;
	        }else{
    		    adv_in_conn_tick += blta.adv_interval;
    		}
    		blt_send_adv2scan_mode(1);
		}
    }
}

_attribute_data_retention_	bls_phy_scan_mode_t		bls_phy_scan_mode_cb = 0;// add by weixiong // for sig mesh + telink mesh dual mode

blt_update_err_callback_t update_err_cb =NULL; // add by weixiong
void bls_set_update_chn_cb(void *p)
{
	update_err_cb = p;
}

void bls_register_phy_scan_mode (void* phy_scan_func) // add by weixiong
{
	bls_phy_scan_mode_cb = phy_scan_func;
}
#endif


#if 1//(DEBUG_LOG_SETTING_DEVELOP_MODE_EN || HCI_LOG_FW_EN)
volatile long trap0_cnt;
volatile long trap1_entry_ra;
volatile long trap2_entry_sp;
volatile long trap3_entry_gp;
volatile long trap4_mcause;
volatile long trap5_mepc;
volatile long trap6_mtvl;
volatile long trap7_mdcause;
volatile long trap8_mscratch;

_attribute_ram_code_ void except_handler()
{
	register long ra asm("x1"), sp asm("x2"),gp asm("x3");
	trap1_entry_ra = ra;
	trap2_entry_sp = sp;
	trap3_entry_gp = gp;
	trap4_mcause = read_csr(NDS_MCAUSE);//4148
	trap5_mepc = read_csr(NDS_MEPC);//4147
	trap6_mtvl = read_csr(NDS_MTVAL);//4147
	trap7_mdcause = read_csr(NDS_MDCAUSE);
	trap8_mscratch = read_csr(NDS_MSCRATCH);

	while(1){
		/* Unhandled Trap */
		wd_clear();
		trap0_cnt++;
	}
}
#endif

void gpio_set_wakeup(u32 pin, u32 level, int en)
{
	// no need to do anything for B91.
}

void main_loop_risv_sdk()
{
#if (DUMP_STR_EN)
	udb_usb_handle_irq();
#endif
#if AUDIO_MESH_EN
	app_audio_task();
#endif
}

void user_init_risv_sdk()
{
	bls_ll_continue_adv_after_scan_req(1);
#if AUDIO_MESH_EN
	app_audio_init ();
#endif
}

/**
 * @brief This function serves to adc module setting init.
 * @param[in]  none.
 * @return none.
 */
#if ADC_ENABLE
void adc_drv_init(){	
	adc_gpio_sample_init(ADC_GPIO_PB3, ADC_VREF_1P2V, ADC_PRESCALE_1F4, ADC_SAMPLE_FREQ_96K);
	adc_power_on();
	#if (BATT_CHECK_ENABLE)
	adc_hw_initialized = 0;	// inform battery check to re-init.
	#endif
}
#endif

#if (HCI_ACCESS==HCI_USE_UART)
const uart_num_e UART_RX_NUM = UART_NUM_GET(UART_RX_PIN);
const uart_num_e UART_TX_NUM = UART_NUM_GET(UART_TX_PIN);
const irq_source_e UART_IRQ_NUM = UART_IRQ_GET(UART_RX_PIN);
static volatile u32 uart_dma_sending_tick = 0;

STATIC_ASSERT(UART_NUM_GET(UART_RX_PIN) == UART_NUM_GET(UART_TX_PIN)); // must to be same now.

void uart_tx_busy_timeout_poll()
{
	 if(uart_dma_sending_tick){
		 if(clock_time_exceed(uart_dma_sending_tick, 500*1000)){
			 uart_dma_sending_tick = 0;
		 }
	 }
}

unsigned char uart_tx_is_busy_dma_tick()
{
	return (uart_dma_sending_tick != 0);
}

unsigned char uart_Send_dma_with_busy_hadle(unsigned char* data, unsigned int len)
{
	int success = uart_send_dma(UART_TX_NUM, data, len);
	if(success){
		uart_dma_sending_tick = clock_time() | 1;
	}else{
		uart_dma_sending_tick = 0;
	}

	return success;
}

unsigned char uart_ErrorCLR(void){
	if((uart_get_irq_status(UART_RX_NUM, UART_RX_ERR)))
    {
    	uart_clr_irq_status(UART_RX_NUM, UART_CLR_RX);
		return 1;
    }
	return 0;
}

static u16 uart_tx_irq=0, uart_rx_irq=0;

_attribute_ram_code_ void irq_uart_handle_fifo()
{
    if(uart_get_irq_status(UART_RX_NUM, UART_RXDONE)) //A0-SOC can't use RX-DONE status,so this interrupt can noly used in A1-SOC.
    {
    	uart_rx_irq++;
		/************************get the length of receive data****************************/
        u32 rev_data_len = uart_get_dma_rev_data_len(UART_RX_NUM, UART_DMA_CHANNEL_RX);
    	/************************cll rx_irq****************************/
    	uart_clr_irq_status(UART_RX_NUM, UART_CLR_RX);
    	u8* w = hci_rx_fifo.p + (hci_rx_fifo.wptr & (hci_rx_fifo.num-1)) * hci_rx_fifo.size;
		memcpy(w, &rev_data_len, sizeof(rev_data_len));
		my_fifo_next(&hci_rx_fifo);
		u8* p = hci_rx_fifo.p + (hci_rx_fifo.wptr & (hci_rx_fifo.num-1)) * hci_rx_fifo.size;
		uart_receive_dma(UART_RX_NUM, (unsigned char *)(p+OFFSETOF(uart_data_t, data)), (unsigned int)hci_rx_fifo.size);
    }

	if(uart_get_irq_status(UART_TX_NUM, UART_TXDONE))
	{
		uart_tx_irq++;
    	uart_dma_sending_tick=0;
	    uart_clr_tx_done(UART_TX_NUM);
	}
}

void uart_drv_init_B91m()
{
	unsigned short div;
	unsigned char bwpc;
	uart_reset(UART_RX_NUM);
	uart_set_pin(UART_TX_PIN ,UART_RX_PIN );// uart tx/rx pin set
	uart_cal_div_and_bwpc(UART_DMA_BAUDRATE, sys_clk.pclk*1000*1000, &div, &bwpc);
	uart_set_rx_timeout(UART_RX_NUM, bwpc, 12, UART_BW_MUL1);
	uart_init(UART_RX_NUM, div, bwpc, UART_PARITY_NONE, UART_STOP_BIT_ONE);

	uart_clr_irq_mask(UART_RX_NUM, UART_RX_IRQ_MASK | UART_TX_IRQ_MASK | UART_TXDONE_MASK|UART_RXDONE_MASK);
	core_interrupt_enable();

	uart_set_tx_dma_config(UART_TX_NUM, UART_DMA_CHANNEL_TX);
	uart_set_rx_dma_config(UART_RX_NUM, UART_DMA_CHANNEL_RX);

	uart_clr_tx_done(UART_TX_NUM);
	uart_set_irq_mask(UART_RX_NUM, UART_RXDONE_MASK);

	uart_set_irq_mask(UART_TX_NUM, UART_TXDONE_MASK);
	plic_interrupt_enable(UART_IRQ_NUM);

	u8 *uart_rx_addr = hci_rx_fifo.p + (hci_rx_fifo.wptr & (hci_rx_fifo.num-1)) * hci_rx_fifo.size;
	uart_receive_dma(UART_RX_NUM, (unsigned char *)(uart_rx_addr+OFFSETOF(uart_data_t, data)), (unsigned int)hci_rx_fifo.size);
}
#endif

#if 0 // debug
void debug_set_irq_interval_pin(u8 level)
{
    u32 pin = GPIO_PA0;
    gpio_set_func(pin, AS_GPIO);
    gpio_set_output_en(pin, 1);
    gpio_write(pin, level);
}

void debug_set_scan_rx_pin(u8 level)
{
    u32 pin = GPIO_PA2;
    gpio_set_func(pin, AS_GPIO);
    gpio_set_output_en(pin, 1);
    gpio_write(pin, level);
}

void debug_set_scan_crc_ok_pin(u8 level)
{
    u32 pin = GPIO_PA4;
    gpio_set_func(pin, AS_GPIO);
    gpio_set_output_en(pin, 1);
    gpio_write(pin, level);
}

void debug_set_scan_rx_ok_pin(u8 level)
{
    u32 pin = GPIO_PA1;
    gpio_set_func(pin, AS_GPIO);
    gpio_set_output_en(pin, 1);
    gpio_write(pin, level);
}
#endif

#if (__TLSR_RISCV_EN__)
#define reg_swire_ctrl1			REG_ADDR8(0x100c01)
#define AS_USB      			(AS_USB_DP)
#define FLD_USB_BDT_EN      	(BIT(3)|BIT(4))
#endif

/*_attribute_ahead_ram_code_ */void bootloader_unlock_flash()
{
    #if 1 // ((MCU_CORE_TYPE == MCU_CORE_8258)||(MCU_CORE_TYPE == MCU_CORE_8278)) // no need for eagle
    gpio_set_func(GPIO_DP, AS_USB);   	// input enable inside for B85m, but no input enable inside for B91m. // for B85 GPIO as default, for B87 USB as default.
    usb_dp_pullup_en(1);
   
    	#if (__TLSR_RISCV_EN__)
    gpio_input_en(GPIO_DP);				// must // input enable as default for chip, but may be disable by gpio_init_() in product image.
    gpio_input_en(GPIO_DM);				// must // input enable as default for chip, but may be disable by gpio_init_() in product image.
    	#else
    gpio_set_func(GPIO_DM, AS_USB);   	// input enable inside.
    // analog_write(0x34, 0x80);   		// USB clock // disable as default. enable in cpu wakeup init(),
    	#endif
    gpio_setup_up_down_resistor(GPIO_DM, PM_PIN_PULLUP_1M);
    #endif
   
    reg_swire_ctrl1 |= FLD_SWIRE_USB_EN;
    reg_usb_mdev |= FLD_USB_BDT_EN;   	// also enable
}


#if MESH_FLASH_PROTECTION_EN
/**
 * @brief     flash mid definition
 */
typedef enum{
	MID146085   =   0x146085,//P25Q80U // 1M byte
	MID156085   =   0x156085,//P25Q16SU
	MID1560c8   =   0x1560c8,//GD25LQ16E
	MID166085   =   0x166085,//P25Q32SU
	MID186085   =   0x186085,//P25Q128L
}flash_mid_e;

#define FLASH_LOCK_SECTION		FLASH_LOCK_LOW_896K_MID146085
#define FLASH_UNLOCK_VAL		FLASH_LOCK_NONE_MID146085

static u32 g_flash_mid = 0;

STATIC_ASSERT(FLASH_ADR_AREA_1_START >= 0xE0000);		// because lock 896k
STATIC_ASSERT(FLASH_PLUS_ENABLE == 0);					// only support 1M now. 2M need to change flash map and FLASH_LOCK_SECTION.

/**
 * @brief      this function is used to lock flash.
 * @param[in]  flash_lock_block - flash lock block, different value for different flash type
 * @return     none
 */
void mesh_flash_lock() // only for MID146085 now. TODO: others.
{
	if(0 == g_flash_mid){
		g_flash_mid = flash_read_mid();
	}

	if(MID146085 != g_flash_mid){
		return ;
	}

	u16 cur_lock_status = flash_read_status_mid146085();
	u16 target_lock_status = FLASH_LOCK_SECTION;

	if(cur_lock_status == target_lock_status){ //lock status is want we want, no need lock again

	}else{ //unlocked or locked block size is not what we want
		#if 0 // no need unlock first
		if(cur_lock_status != flash_unlock_status){ //locked block size is not what we want, need unlock first
			for(int i = 0; i < 3; i++){ //Unlock flash up to 3 times to prevent failure.
				flash_unlock_mid();
				cur_lock_status = flash_get_lock_status_mid();

				if(cur_lock_status == flash_unlock_status){ //unlock success
					break;
				}
			}
		}
		#endif


		for(int i = 0; i < 3; i++) //Lock flash up to 3 times to prevent failure.
		{
			flash_lock_mid146085(target_lock_status);
			cur_lock_status = flash_read_status_mid146085();
			if(cur_lock_status == target_lock_status){  //lock OK
				break;
			}
		}
	}
}

/**
 * @brief      this function is used to unlock flash.
 * @param[in]  none
 * @return     none
 */
void mesh_flash_unlock(void)
{
	if(0 == g_flash_mid){
		g_flash_mid = flash_read_mid();
	}
	
	if(MID146085 != g_flash_mid){
		return ;
	}
	
	u16 cur_lock_status = flash_read_status_mid146085();
	u16 target_lock_status = FLASH_UNLOCK_VAL;

	if(cur_lock_status != target_lock_status){ //not in lock status
		for(int i = 0; i < 3; i++){ //Unlock flash up to 3 times to prevent failure.
			flash_unlock_mid146085();
			cur_lock_status = flash_read_status_mid146085();

			if(cur_lock_status == target_lock_status){ //unlock success
				break;
			}
		}
	}
}
#else
void mesh_flash_unlock(void){ }
#endif


