/********************************************************************************************************
 * @file	ble_ll_ota.h
 *
 * @brief	for TLSR chips
 *
 * @author	telink
 * @date	Sep. 30, 2010
 *
 * @par     Copyright (c) 2010, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#pragma once 

#ifndef	BLE_OTA_ENABLE
#define BLE_OTA_ENABLE		0
#endif

#define START_UP_FLAG		(0x544c4e4b)

#if __TLSR_RISCV_EN__
#include "drivers/B91/ext_driver/mcu_boot.h"
#else
#define	BOOT_MARK_ADDR					0x00008  //Kite/vulture: 0x08
#define	FW_SIZE_ADDR					0x00018
#endif


enum{
    FW_CHECK_NONE       = 0x00,  //
    FW_CHECK_AGTHM1     = 0x01,  //crc16, then add
    FW_CHECK_AGTHM2     = 0x02,  //crc 32
    FW_CHECK_MAX,
};


#if __TLSR_RISCV_EN__
#include "stack/ble/service/ota/ota_stack.h"
#include "stack/ble/service/ota/ota.h"
#include "stack/ble/service/ota/ota_server.h"

typedef struct{
	u8  ota_start_flag;
}ota_service_t;

ota_service_t blcOta;


#else

#ifndef	BLE_OTA_ENABLE
#define BLE_OTA_ENABLE		0
#endif


#define CMD_OTA_FW_VERSION					0xff00
#define CMD_OTA_START						0xff01
#define CMD_OTA_END	

typedef struct{
	u8  ota_start_flag;
}ota_service_t;

extern ota_service_t blcOta;


extern int blt_ota_start_flag;
extern u32 blt_ota_start_tick;
extern u32 blt_ota_timeout_us;

extern u32	ota_program_offset;
extern u32 	ota_firmware_size_k;
extern u32	bls_ota_bootFlagAddr;
extern int 	ota_program_bootAddr ;


typedef void (*ota_startCb_t)(void);
_attribute_data_retention_	ota_startCb_t		otaStartCb = NULL;

typedef void (*ota_versionCb_t)(void);

typedef void (*ota_resIndicateCb_t)(int result);

extern ota_resIndicateCb_t otaResIndicateCb;


enum{
	OTA_SUCCESS = 0,		//success
	OTA_PACKET_LOSS,		//lost one or more OTA PDU
	OTA_DATA_CRC_ERR,		//data CRC err
	OTA_WRITE_FLASH_ERR,	//write OTA data to flash ERR
 	OTA_DATA_UNCOMPLETE,	//lost last one or more OTA PDU
 	OTA_TIMEOUT,			//total timeout
 	OTA_FW_CHECK_ERR,		//firmware CRC check error
	OTA_FW_TYPE_ERR,		//firmware type error
	OTA_VERSION_NUMBER_ERR,	//the version number to be update is lower than the current version
	OTA_PDU_LARGER_DLE,		//OTA PDU is larger than DLE supported
	OTA_PDU_ERR,			//OTA PDU is not equal to 64 or 128
 	OTA_RF_PACKET_TIMEOUT,	//timeout between RF packet
 	OTA_OVERFLOW,		 // the ota adr overflow to 0x30000
 	OTA_ERR_STS,
	OTA_SUCCESS_DEBUG,     //success
 	OTA_FW_CHECK_ERR,
	OTA_REBOOT_NO_LED,	// no LED indication, for quickly reboot.
};

#endif /* BLE_LL_OTA_H_ */

#if 1
typedef struct{
	u8 device_type;
	u32 fw_version;
	u32 fw_size;
	u16 fw_crc16;
	u8 ota_flag;
}ais_ota_req_t;

typedef struct{
	u8 allow_ota;
	u32 trans_size_last;
	u8 one_round_pkts;
}ais_ota_rsp_t;

typedef struct{
	u8 seg_index;
	u32 trans_size_last;
}ais_ota_receive_t;

#define AIS_MAX_DATA_SIZE	32// please makesure have enough for aes128 padding
typedef struct{
	union{
		u8 header;
		struct{
			u8 msg_id:4;
			u8 enc_flag:1;
			u8 fw_ver:3;
		};
	};
	u8 msg_type;
	union{
		u16 frame_desc;
		struct{
			u8 frame_seq:4;
			u8 frame_total:4;
			u8 length;
		};
	};
	union{
		u8 data[AIS_MAX_DATA_SIZE];  
		u8 device_type;
		u8 ota_result;
		ais_ota_req_t ais_ota_req;
		ais_ota_rsp_t ais_ota_rsp;
		ais_ota_receive_t ais_ota_rcv;
	};
}ais_msg_t;

typedef struct{
	u8 device_type;
	u32 fw_version;
}ais_fw_info_t;

#define AIS_INDICATE_HANDLE	0x32
#define AIS_NOTIFY_HANDLE	0x36

#define AIS_AUTH_RANDOM			0x10
#define AIS_AES_CIPHER			0x11
#define AIS_AUTH_CHECK			0x12
#define AIS_AUTH_RESULT			0x13
#define AIS_FW_VERSION_GET		0x20
#define AIS_FW_VERSION_RSP		0x21
#define	AIS_OTA_REQ				0x22
#define	AIS_OTA_RSP				0x23
#define	AIS_OTA_RECEVIED		0x24
#define	AIS_OTA_END				0x25
#define	AIS_OTA_RESULT			0x26
#define	AIS_OTA_DATA			0x2f

void bls_ota_procTimeout(void);

//user interface
void bls_ota_registerStartCmdCb(ota_startCb_t cb);
void bls_ota_registerVersionReqCb(ota_versionCb_t cb);
void bls_ota_registerResultIndicateCb(ota_resIndicateCb_t cb);
#endif

void bls_ota_setTimeout(u32 timeout_us);
void blt_slave_ota_finish_handle();
void blt_ota_finished_flag_set(u8 reset_flag);
void rf_link_slave_ota_finish_led_and_reboot(u8 st);

extern int otaWrite(void * p);
extern int otaRead(void * p);
int ais_otaWrite(void * p);

extern void start_reboot(void);	// irq_disable() inside

//firmware_size_k  must be 4k aligned, ota_offset will be ignored in 8267/8269, valid in 8261/8266
void bls_ota_setFirmwareSizeAndOffset(int firmware_size_k, u32 ota_offset);
void bls_ota_set_fwSize_and_fwBootAddr(int firmware_size_k, int boot_addr);

//only valid for 8261/8266
void bls_ota_setBootFlagAddr(u32 bootFlag_addr);

int bls_ota_clearNewFwDataArea(int check_only);

unsigned short crc16(const unsigned char *pD, int len);
u8 ota_condition_enable();
void set_ota_reboot_flag(u8 flag);
u8 get_fw_ota_value();


