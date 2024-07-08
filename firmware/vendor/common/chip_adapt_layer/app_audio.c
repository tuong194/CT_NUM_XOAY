/********************************************************************************************************
 * @file	app_audio.c
 *
 * @brief	This is the source file for BLE SDK
 *
 * @author	Mesh Group
 * @date	2022
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
/*
 * app_audio.c
 *
 *  Created on: Jun 20, 2022
 *      Author: telink
 */
#include "proj_lib/sig_mesh/app_mesh.h"
#include "vendor/common/pair_provision.h"
#include "app_audio.h"
#include <math.h>
#include "drivers/B91/audio.h"


#if AUDIO_MESH_EN
#define AUDIO_DUMP_EN                	1
#define AUDIO_LED_INDICATION_TEST_EN	1	// use PWM to indicate audio level.
#if AUDIO_LED_INDICATION_TEST_EN
#define AUDIO_LED_INDICATION_IO			PWM_G // for PCBA_ADK80D_C1T213A20_V13, if use PWM_W Please connect PWM to TL_PB0 manually, because PB6(PWM_W) is not a PWM IO.
	#if AUDIO_DATA_NO_TX_WHEN_SILENCE_EN
#define AUDIO_SENDING_LED_IO			GPIO_PB6 // white led
	#endif
#define AUDIO_LED_DURATION				(MIC_SAMPLES_PER_PACKET * 1000 / 8)
#define AUDIO_LED_HIGH_DURATION_US		(AUDIO_LED_DURATION/2)
#define AUDIO_LED_LOW_DURATION_US		(AUDIO_LED_DURATION - AUDIO_LED_HIGH_DURATION_US)
_attribute_bss_dlm_ audio_led_indication_t audio_led_indication;

#define LOG_LED_INDICATION_DEBUG(pbuf,len,format,...)    //LOG_MSG_LIB(TL_LOG_NODE_BASIC,pbuf,len,format,__VA_ARGS__)
STATIC_ASSERT(AUDIO_LED_HIGH_DURATION_US <= (MIC_SAMPLES_PER_PACKET * 1000 / 8)/2);
#endif

#if(CODEC_ALGORITHM_SEL == CODEC_ALGORITHM_SBC)
	#if (RF_SEND_AUDIO_SAMPLE_RATE_SEL == RF_SEND_AUDIO_SAMPLE_RATE_8K)
#define AUDIO_SAMPLE_RATE				AUDIO_16K // don't change // means audio ADC sample rate and audio speaker sample rate.
	#else
#error not support this sample rate, because RF audio message will be too much.
	#endif
#elif(CODEC_ALGORITHM_SEL == CODEC_ALGORITHM_LC3)
	#if (RF_SEND_AUDIO_SAMPLE_RATE_SEL == RF_SEND_AUDIO_SAMPLE_RATE_8K)
#define AUDIO_SAMPLE_RATE				AUDIO_8K
	#elif (RF_SEND_AUDIO_SAMPLE_RATE_SEL == RF_SEND_AUDIO_SAMPLE_RATE_16K)
#define AUDIO_SAMPLE_RATE				AUDIO_16K
	#else
#error TO be defined
	#endif
#endif
		
#define MIC_SAMPLES_TIME_US				(MIC_SAMPLES_PER_PACKET*1000 / ((AUDIO_SAMPLE_RATE==AUDIO_8K)?8:((AUDIO_SAMPLE_RATE==AUDIO_16K)?16:24)))
#define MIC_TOLLERANCE_THRES 			(MIC_SAMPLES_PER_PACKET*MIC_NUM_MESH_TX/2)
#define PLAY_TOLLERANCE_THRES 			(((TRANSMIT_CNT_DEF+1)*(TRANSMIT_INVL_STEPS_DEF+1+1)*10) * ((AUDIO_SAMPLE_RATE==AUDIO_8K)?8:((AUDIO_SAMPLE_RATE==AUDIO_16K)?16:24)))


static _attribute_bss_dlm_ tcodec_cfg_t	tcodec;

_attribute_bss_dlm_ tadc_int	buff_mic[AUDIO_FIFO_SIZE_MAX]; 		 // some space for echo cancel
_attribute_bss_dlm_ tcodec_int	buff_playback[PLAY_FIFO_SIZE + 256]; // some space for echo cancel

_attribute_bss_dlm_ u8	audio_mesh_enc_buff[1024 * 8 * 1];		// 1 channel: 0x0f00
_attribute_bss_dlm_ u8	audio_mesh_dec_buff[1024 * 12];		    // 1 channel: 0x29f0

_attribute_bss_dlm_ u32 audio_mesh_tx_tick;
_attribute_bss_dlm_ audio_mesh_rx_par_t  audio_mesh_rx_par[AUDIO_RX_NODES_MAX]={{0}};
u8 audio_active_nodes_cnt_rx = 0; // Records the number of nodes that are currently playing(not include self)


#if AUDIO_MESH_MULTY_NODES_TX_EN
STATIC_ASSERT((CODEC_ALGORITHM_SEL == CODEC_ALGORITHM_LC3) && (RF_SEND_AUDIO_SAMPLE_RATE_SEL == RF_SEND_AUDIO_SAMPLE_RATE_8K));
#else
STATIC_ASSERT(AUDIO_RX_NODES_MAX == 1);
#endif

#if APP_NS_ENABLE
STATIC_ASSERT(MIC_SAMPLES_PER_PACKET % NS_SAMPLE_NUM == 0); // refer to codes where call app_ns_process_frame().
STATIC_ASSERT(SRC_DATA_LEN_MAX == NS_SAMPLE_NUM);
#endif


#if (CODEC_ALGORITHM_SEL == CODEC_ALGORITHM_SBC)
#include "stack/my_mic_api.h"
#include "my_resample.h"

#define MIC_TX_NUM						1

_attribute_bss_dlm_ my_mic_enc_para_t mic_encoder;
_attribute_bss_dlm_ my_mic_dec_para_t mic_decoder[MIC_TX_NUM];

STATIC_ASSERT(RF_SEND_AUDIO_SAMPLE_RATE_SEL == RF_SEND_AUDIO_SAMPLE_RATE_8K);

void mic_enc_dec_init()
{
    my_mic_enc_init(&mic_encoder, SBC_BLOCK_NUM, SBC_BIT_POOL);

    for(int i=0; i<MIC_TX_NUM; i++) {
        my_mic_dec_init(&mic_decoder[i], SBC_BLOCK_NUM, SBC_BIT_POOL);
    }
}

_attribute_ram_code_
int mic_audio_encode(s16 *ps, int samples, u8 *pd)
{
    return my_mic_enc(&mic_encoder, ps, samples, pd);
}

_attribute_ram_code_
int mic_audio_decode(int chn, u8 *ps, int samples, s16 *pd)
{
    return my_mic_dec(mic_decoder+chn, ps, samples, pd);
}
#endif

void audio_codec_config (audio_channel_wl_mode_e channel_wl,int sample_rate, u32 * speaker_buff, int speaker_size, u32 *mic_buff, int mic_size)
{
//	analog_write_reg8 (0x02, 0xc4);		//flash 2.8V trim   codec ?

	audio_i2s_invert_config_t audio_mesh_audio_i2s_invert_config={
		.i2s_lr_clk_invert_select=I2S_LR_CLK_INVERT_DIS,
		.i2s_data_invert_select=I2S_DATA_INVERT_DIS,
	};

	#if (AUDIO_I2S_EN == 0)
	audio_set_codec_supply(CODEC_2P8V);

	audio_set_codec_clk(1, 16);////from ppl 192/16=12M

	audio_mux_config(CODEC_I2S, BIT_16_MONO, BIT_16_MONO, BIT_16_MONO);

	audio_i2s_config(I2S_I2S_MODE, I2S_BIT_16_DATA, I2S_M_CODEC_S,&audio_mesh_audio_i2s_invert_config);

	audio_set_i2s_clock(sample_rate, AUDIO_RATE_LT_L1, 0);

	audio_clk_en(1,1);

	reg_audio_codec_vic_ctr = FLD_AUDIO_CODEC_SLEEP_ANALOG;//active analog sleep mode

	while(!(reg_audio_codec_stat_ctr&FLD_AUDIO_CODEC_PON_ACK));//wait codec can be configed

	audio_codec_dac_config(I2S_M_CODEC_S, sample_rate, CODEC_BIT_16_DATA, MCU_WREG);

	audio_codec_adc_config(I2S_M_CODEC_S, AMIC_IN_TO_BUF, sample_rate, CODEC_BIT_16_DATA,MCU_WREG);
	#else
	audio_i2s_set_pin();
	audio_set_chn_wl(channel_wl);
	audio_mux_config(IO_I2S,BIT_16_MONO,BIT_16_MONO,BIT_16_MONO_FIFO0); // To be consistent with the audio group(broadcast_mic_sdk)
	audio_i2s_config(I2S_I2S_MODE, I2S_BIT_16_DATA, I2S_M_CODEC_S,&audio_mesh_audio_i2s_invert_config);
	audio_set_i2s_clock(AUDIO_SAMPLE_RATE, AUDIO_RATE_EQUAL, 0); // To be consistent with the audio group(broadcast_mic_sdk)
	audio_clk_en(1,1);
	#endif

	audio_data_fifo0_path_sel(I2S_DATA_IN_FIFO,I2S_OUT);

	////DMA init
	if (speaker_buff && speaker_size)
	{
		audio_tx_dma_chain_init (DMA3, (u16*)speaker_buff, speaker_size);
	}

	if (mic_buff && mic_size)
	{
		audio_rx_dma_chain_init(DMA2,(u16*)mic_buff, mic_size);
	}
}


void app_audio_init ()
{
	audio_set_codec_in_path_a_d_gain(CODEC_IN_D_GAIN_0_DB, CODEC_IN_A_GAIN_20_DB); //mic sco
	audio_set_codec_out_path_a_d_gain(CODEC_OUT_D_GAIN_0_DB, CODEC_OUT_A_GAIN_12_DB); //mic sco

	tcodec.sample_rate = AUDIO_SAMPLE_RATE;
	tcodec.play_wptr = tcodec.play_rptr = 0;

	audio_codec_config (MONO_BIT_16, tcodec.sample_rate, (u32 *)buff_playback, PLAY_FIFO_SIZE_BYTE , (u32 *)buff_mic, MIC_FIFO_SIZE_BYTE);

	#if AUDIO_I2S_EN
	gpio_input_en(I2S_BCK_PC3|I2S_DAC_LR_PC6|I2S_DAC_DAT_PC7|I2S_ADC_LR_PC4|I2S_ADC_DAT_PC5);		//IIS¨°y??
	#else
	audio_codec_adc_power_on ();
	#endif
	
	//audio_set_i2s_clock(AUDIO_SAMPLE_RATE, AUDIO_RATE_EQUAL, 0); // audio set i2s clock() has been called in audio_codec_config() for both analog and I2S mode.

#if (CODEC_ALGORITHM_SEL == CODEC_ALGORITHM_LC3)
	audio_mesh_enc_init (audio_mesh_enc_buff, sizeof(audio_mesh_enc_buff), 48000, 1, LC3_BIT_RATE, 0);		//0: 10ms; 1: 7.5ms
	audio_mesh_tns_enable (0, 0);
	audio_mesh_dec_init (audio_mesh_dec_buff, sizeof(audio_mesh_dec_buff), 48000, 1, LC3_BIT_RATE, 0);		//0: 10ms; 1: 7.5ms
#elif (CODEC_ALGORITHM_SEL == CODEC_ALGORITHM_SBC)
	mic_enc_dec_init();
#endif
	app_ns_init();
#if MESH_AUDIO_RESAMPLE_EN
	my_resample8to16_init();
	my_resample16to8_init();
#endif

	/*
	for(int i=0;i<PLAY_FIFO_SIZE;i++) {
		buff_playback[i] = 1024.0f*5.0f*sinf(2 * M_PI * 1000.0f * (float)i / 8000.0f);
	}
	*/

#if AUDIO_LED_INDICATION_TEST_EN
	// PWM indication for audio.
	//pwm_set_clk((unsigned char) (sys_clk.pclk*1000*1000/PWM_PCLK_SPEED-1));
	u32 pwm_io = AUDIO_LED_INDICATION_IO;
	pwm_set_tcmp(get_pwmid(pwm_io),(get_pwm_invert_val(pwm_io) ? (PWM_MAX_TICK - 0) : 0));
	pwm_set_tmax(get_pwmid(pwm_io),PWM_MAX_TICK);
	pwm_start(get_pwmid(pwm_io));
	gpio_function_en(pwm_io);
	pwm_set_pin(pwm_io);
	
	#if AUDIO_DATA_NO_TX_WHEN_SILENCE_EN
	gpio_set_func(AUDIO_SENDING_LED_IO, AS_GPIO);
    gpio_set_output_en(AUDIO_SENDING_LED_IO, 1);
	gpio_write(AUDIO_SENDING_LED_IO, 0);
	#endif
	
#endif
}

static inline u32 tcodec_get_speaker_rptr (void) {
	return ((audio_get_tx_dma_rptr(DMA3) - (u32)buff_playback) / sizeof (tcodec_int));
}

static inline u32 tcodec_get_mic_wptr (void) {
	return ((audio_get_rx_dma_wptr(DMA2) - (u32)buff_mic) / sizeof (tadc_int));
}

void app_sync_mic_sample (int n, int tollerance)
{
    int wptr = tcodec_get_mic_wptr ();
    int num = (wptr - tcodec.mic_rptr) & MIC_FIFO_MAX;
//    my_dump_str_u32s (1, "app_sync_mic_sample", tcodec.mic_rptr, num, wptr, n);
    if (num < n - tollerance || num > n + tollerance)
    {
        tcodec.mic_rptr = (wptr - n) & MIC_FIFO_MAX;
        my_dump_str_u32s (1, "Reset MIC Samples", 0, num, n, tollerance);
    }
}

void app_check_playback_buffer (int ref, int tollerance)
{
	int rptr0 = tcodec_get_speaker_rptr ();
	int samples_in_fifo0 = (tcodec.play_wptr - rptr0) & PLAY_FIFO_MAX;
	if (samples_in_fifo0 < ref - tollerance || samples_in_fifo0 > ref + tollerance)
	{
//		LOG_MSG_LIB(TL_LOG_NODE_SDK,0, 0,"***************Reset Play back buffer samples_in_fifo:%d ref:%d tollerance:%d", samples_in_fifo0, ref, tollerance);
		my_dump_str_u32s (AUDIO_DUMP_EN, "MIC - Reset Play back buffer", 0, samples_in_fifo0, ref, tollerance);
		tcodec.play_wptr = (rptr0 + ref) & PLAY_FIFO_MAX;
	}
}

int tcodec_get_mic_data_number()
{
	int wptr = tcodec_get_mic_wptr ();
	return (wptr - tcodec.mic_rptr) & MIC_FIFO_MAX;
}

int tcodec_get_mic_data (tadc_int *pd, int len)
{
    int num = tcodec_get_mic_data_number();
	if (num >= len)
	{
		for (int i = 0; i < len; i++)
		{
			*pd++ = buff_mic[tcodec.mic_rptr++];
			tcodec.mic_rptr = tcodec.mic_rptr & MIC_FIFO_MAX;
		}
		return 1;
	}

	return 0;
}


void pcm_2_playback (tcodec_int *pcm, int n, u16 *p_avg, u16 *p_max)
{
	u32 total_abs = 0;
	u32 max_abs = 0;
	int n_org = n;
    while (n--)
    {
    	#if AUDIO_LED_INDICATION_TEST_EN
		u16 val_abs = abs(*pcm);
    	total_abs += val_abs;
    	max_abs = max2(max_abs, val_abs);
    	#endif
        buff_playback[tcodec.play_wptr++ & PLAY_FIFO_MAX] = *pcm++;
    }
	
	if(p_avg){
		*p_avg = total_abs / n_org;
		//LOG_LED_INDICATION_DEBUG(0, 0,"total :%d, n:%d, avg:%d", total_abs, n_org, *p_avg);
	}
	
	if(p_max){
		*p_max = max_abs;
	}
}

void app_audio_mic_onoff(u8 on)
{
	if(on){
		vd_cmd_mic_tx_req(ADR_ALL_NODES);
	}
	else{
		#if AUDIO_DATA_NO_TX_WHEN_SILENCE_EN
		gpio_write(AUDIO_SENDING_LED_IO, 0);
		#endif
		audio_mesh_tx_tick = 0;
	}

#if (BATT_CHECK_ENABLE)
	battery_set_detect_enable(!on);
#endif
}

void app_audio_rx_st_clear(u8 node_adr_idx)
{
	if((0 == audio_active_nodes_cnt_rx) || (node_adr_idx >= AUDIO_RX_NODES_MAX)){
		return ;
	}

	audio_active_nodes_cnt_rx--;
	if(0 == audio_active_nodes_cnt_rx){
		audio_led_indication.tick_start = 0;
		memset(buff_playback, 0x00, sizeof(buff_playback));
	}
	memset(&audio_mesh_rx_par[node_adr_idx], 0x00, sizeof(audio_mesh_rx_par[node_adr_idx]));

#if AUDIO_LED_INDICATION_TEST_EN
	pwm_set_lum(get_pwmid(AUDIO_LED_INDICATION_IO), 0, get_pwm_invert_val(AUDIO_LED_INDICATION_IO));
#endif
}

void app_audio_rx_st_fresh(audio_mesh_rx_par_t *p_rx, u16 adr_src)
{
	p_rx->tick_last = clock_time()|1;
	
	#if (AUDIO_MESH_MULTY_NODES_TX_EN == 0)	// can not move to cb_vd_group_g_mic_tx_req(), because mic_tx_req message may be lost, and just receive audio data message.
	if(audio_active_nodes_cnt_rx == 0){
		audio_active_nodes_cnt_rx = 1; 		// In single-node mode, the value must be set to 1; otherwise, the audio player cannot play
	}
	p_rx->adr_src = adr_src; //  Single-node mode requires that src be updated, otherwise 0 is returned at cb_vd_async_audio_data() of app_audio.c
	#endif
		
}

void proc_ui_audio()
{
	static u8 st_sw1_last;	
	u8 st_sw1 = !gpio_read(SW1_GPIO);
	static u32 st_sw1_long_press_tick;
	if(!(st_sw1_last)&&st_sw1){
		// was just pressed
		st_sw1_long_press_tick = clock_time() | 1;
		#if RELAY_ROUTE_FILTE_TEST_EN
		extern u32 mac_filter_by_button;
		if(0 == mac_filter_by_button){
			mac_filter_by_button = 0x2022070a; // test_mac
		}else{
			mac_filter_by_button = 0;
		}
		#endif
	}else if(st_sw1){
		// keep pressed
		if(st_sw1_long_press_tick && clock_time_exceed(st_sw1_long_press_tick, 5000*1000)){
			// long pressed trigger.
			st_sw1_long_press_tick = 0;
			#if PAIR_PROVISION_ENABLE
			pair_prov_kick_out_all_nodes();
			#endif
		}
	}else if(!st_sw1){
		// release
		if(st_sw1_long_press_tick){
			// short pressed
			st_sw1_long_press_tick = 0;
			#if PAIR_PROVISION_ENABLE
			pair_proc_start();
			#endif
		}else{
			// long pressed trigger before.
		}
	}
	
	st_sw1_last = st_sw1;
	
	static u8 st_sw2_last;	
	u8 st_sw2 = !gpio_read(SW2_GPIO);	
	if(!(st_sw2_last)&&st_sw2){			
		#if 0
		u8 mac_play[] = {0x00,0x00,0x20,0x00,0xff,0xff};
		if(0 == memcmp(tbl_mac, mac_play, 6)){
			extern u32 mac_filter_by_button;
			if(0 == mac_filter_by_button){
				mac_filter_by_button = 0x2022070a; // test_mac
			}else{
				mac_filter_by_button = 0;
			}
		}else
		#endif
		{
			#if AUDIO_MESH_MULTY_NODES_TX_EN
			if(app_audio_is_valid_key_pressed() == 0){
				return ;
			}
			#endif
			
			app_audio_mic_onoff(!audio_mesh_tx_tick);
		}
	}
	st_sw2_last = st_sw2;
}

STATIC_ASSERT((ACCESS_NO_MIC_LEN_MAX_UNSEG+CONST_DELTA_EXTEND_AND_NORMAL) >= (sizeof(vd_audio_t)+OP_TYPE_VENDOR)); // (225 >= x +3)
_attribute_bss_dlm_ u8 audio_mic_index = 0;
_attribute_bss_dlm_ vd_audio_t vd_audio_data;

_attribute_bss_dlm_ int audio_play_tick = 0;

s16 audio_level_tx_threadhold = 50;
int  app_audio_is_voice_active(tadc_int *pcm, int count_pcm)
{
	s16 pcm_value = 0;
	int pcm_sum = 0;
	foreach(i, count_pcm){
		pcm_sum += abs(pcm[i]);
	}
	
	pcm_value = (pcm_sum / count_pcm);	
	return (pcm_value >= audio_level_tx_threadhold);
}

void app_audio_task()
{
	tadc_int pcm[MIC_SAMPLES_PER_PACKET];
	static u8 enc_offset = 0;
	if(audio_active_nodes_cnt_rx > 0){
		foreach(i, AUDIO_RX_NODES_MAX){
			if(audio_mesh_rx_par[i].tick_last && clock_time_exceed(audio_mesh_rx_par[i].tick_last, AUDIO_RX_TIMEOUT*1000)){	
				app_audio_rx_st_clear(i);
			}
		}
	}

	if(clock_time_exceed(audio_play_tick, MIC_SAMPLES_TIME_US)){
		audio_play_tick += MIC_SAMPLES_TIME_US * sys_tick_per_us;
		app_check_playback_buffer(MIC_SAMPLES_PER_PACKET + PLAY_TOLLERANCE_THRES, PLAY_TOLLERANCE_THRES);
		audio_rx_proc();
	}

	if(!is_audio_mesh_tx_working()){
		#if AUDIO_LED_INDICATION_TEST_EN
		int led_on_flag = (audio_led_indication.index & 1);
		u32 led_duration_us = led_on_flag ? AUDIO_LED_LOW_DURATION_US : AUDIO_LED_HIGH_DURATION_US;
		if (audio_led_indication.tick_start && clock_time_exceed(audio_led_indication.tick_start, led_duration_us)) {
			audio_led_indication.tick_start = clock_time();
			audio_led_par_t *par = &audio_led_indication.unit;
			u16 led_val = (audio_led_indication.index & 1) ? par->avg : par->max;
			pwm_set_lum(get_pwmid(AUDIO_LED_INDICATION_IO), led_val, get_pwm_invert_val(AUDIO_LED_INDICATION_IO));
			LOG_LED_INDICATION_DEBUG(0, 0,"pwm output index:%d, led val:%d", audio_led_indication.index, led_val);
			audio_led_indication.index++;
		}
		#endif
		
		return;
	}
	
	else if(!is_led_busy()){
        cfg_led_event (LED_EVENT_FLASH_1HZ_1T);
	}
	
	if (clock_time_exceed(audio_mesh_tx_tick, MIC_SAMPLES_TIME_US)) {
		audio_mesh_tx_tick += MIC_SAMPLES_TIME_US * sys_tick_per_us;
		app_sync_mic_sample(MIC_SAMPLES_PER_PACKET + MIC_TOLLERANCE_THRES, MIC_TOLLERANCE_THRES);	// allow 30ms tollerance
		
		#if 0 // test reading sample data 
		static int audio_addr_read=0;
		flash_read_page(0xf6000+audio_addr_read, sizeof(pcm), (u8 *)pcm);
		audio_addr_read += sizeof(pcm);
		if(audio_addr_read > MIC_SAMPLES_PER_PACKET*150){
			audio_addr_read = 0;
		}
		#else
		tcodec_get_mic_data(pcm, MIC_SAMPLES_PER_PACKET);
		#endif

		#if AUDIO_DATA_NO_TX_WHEN_SILENCE_EN
		static int sound_sent_flag = 0;
		#endif
		
#if (CODEC_ALGORITHM_SEL == CODEC_ALGORITHM_LC3)
		#if MESH_AUDIO_RESAMPLE_EN
		tadc_int pcm16k[MIC_SAMPLES_PER_PACKET*2]; // 2 = 16k/8k;
		my_resample8to16_data((int *)pcm, MIC_SAMPLES_PER_PACKET, (int *)pcm16k, 0);
		app_ns_process_frame(pcm16k, MIC_SAMPLES_PER_PACKET*2);
    	my_resample16to8_data((int *)pcm16k, ARRAY_SIZE(pcm16k), (int *)pcm, 0);
		#else
		app_ns_process_frame(pcm, MIC_SAMPLES_PER_PACKET);
		#endif
			
		#if AUDIO_DATA_NO_TX_WHEN_SILENCE_EN
		sound_sent_flag |= app_audio_is_voice_active(pcm, ARRAY_SIZE(pcm));
		#endif
		int leni = audio_mesh_enc (0, pcm, MIC_SAMPLES_PER_PACKET, vd_audio_data.data+enc_offset);	// cost about 3ms in 96M clock
#elif(CODEC_ALGORITHM_SEL == CODEC_ALGORITHM_SBC)
		tadc_int pcm8k[MIC_SAMPLES_PER_PACKET/2];
		app_ns_process_frame(pcm, MIC_SAMPLES_PER_PACKET);
		my_resample16to8_data((int *)pcm, MIC_SAMPLES_PER_PACKET, (int *)pcm8k, 0);
		
		#if AUDIO_DATA_NO_TX_WHEN_SILENCE_EN
		sound_sent_flag |= app_audio_is_voice_active(pcm, ARRAY_SIZE(pcm));
		#endif
		int leni = mic_audio_encode(pcm8k, MIC_SAMPLES_PER_PACKET/2, vd_audio_data.data+enc_offset);
#endif

		enc_offset += leni;
		if(enc_offset >= (MIC_NUM_MESH_TX*leni)){
			#if AUDIO_DATA_NO_TX_WHEN_SILENCE_EN
			if(sound_sent_flag)
			#endif
			{
				vd_cmd_audio_data(ADR_ALL_NODES, 0, (u8 *)&vd_audio_data, OFFSETOF(vd_audio_t, data) + enc_offset); // cost about 16.5ms in 96M clock
			}

			#if AUDIO_DATA_NO_TX_WHEN_SILENCE_EN
			gpio_write(AUDIO_SENDING_LED_IO, sound_sent_flag);
			sound_sent_flag = 0;
			#endif
			vd_audio_data.index++;
			enc_offset = 0;			
		}
	}
}

int vd_cmd_mic_tx_req(u16 adr_dst)
{
	audio_mesh_tx_tick = clock_time()|1;
	
	#if AUDIO_MESH_MULTY_NODES_TX_EN
	return 0;
	#else
	u8 sub_op = VD_GROUP_G_MIC_TX_REQ;
	SendOpParaDebug(adr_dst, 0, VD_GROUP_G_SET_NOACK, &sub_op, sizeof(sub_op));
	return SendOpParaDebug(adr_dst, 0, VD_GROUP_G_SET_NOACK, &sub_op, sizeof(sub_op));
	#endif
}

int vd_cmd_audio_data(u16 adr_dst, u8 rsp_max, u8 *par, int len)
{
#if 1
	u16 src_addr = ele_adr_primary;
	bear_head_t tx_head = {0};
	bear_head_src_sno_t *p = (bear_head_src_sno_t *)&tx_head;
	p->bear_par_type = BEAR_TX_PAR_TYPE_SRC_SNO;
	p->src_addr = src_addr;
	p->sno_low = (u8)mesh_adv_tx_cmd_sno;
	
	return mesh_tx_cmd2normal_with_tx_head(VD_ASYNC_AUDIO_DATA, par, len, src_addr, adr_dst, rsp_max, &tx_head);
#else
	return SendOpParaDebug(adr_dst, rsp_max, VD_ASYNC_AUDIO_DATA, par, len);
#endif
}

void app_audio_set_aux_payload(u8 *p_bear)
{
    mesh_cmd_bear_t *p_br = (mesh_cmd_bear_t *)p_bear;
	aux_payload_t *p_aux = &g_aux_payload;  /* global buffer can be used, because it is called in prepare handle which will copy this global buffer adv at once.
											   it is similar to pkt_adv which is also global.*/
	if(BEAR_TX_PAR_TYPE_SRC_SNO == p_br->tx_head.par_type){
		p_aux->len = sizeof(aux_payload_t) - OFFSETOF(aux_payload_t, ad_type);
		p_aux->ad_type = DATA_TYPE_MANUFACTURER_SPECIFIC_DATA;
		p_aux->vnd_id = g_vendor_id;
		memcpy(&p_aux->head_src_sno, &p_br->tx_head, sizeof(p_aux->head_src_sno));
		extend_adv_aux_src_sno_xor(p_aux);
	}else{
		memset(p_aux, 0, sizeof(aux_payload_t)); // clear
	}
}

#if AUDIO_LED_INDICATION_TEST_EN
void audio_led_value_get(u16 *p_avg, u16 *p_max)
{
	//#define NOISE_LEVEL_INIT	50
	static volatile u32 AUDIO_NOISE_TRIM_LEVEL = 60;
	if(*p_avg <= AUDIO_NOISE_TRIM_LEVEL){
		*p_avg = 0;	// skip noise.
	}else{
		*p_avg -= AUDIO_NOISE_TRIM_LEVEL;
	}

	if(*p_avg){
		static volatile u32 AUDIO_LEVEL_MUL = 30;
		*p_avg = min((*p_avg) * AUDIO_LEVEL_MUL, 0xffff);
		*p_max = min((*p_max) * AUDIO_LEVEL_MUL, 0xffff);
	}else{
		*p_max = 0;
	}
}
#endif

int cb_vd_async_audio_data(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par)
{
	vd_audio_t *p_audio = (vd_audio_t *)par;
	if(is_own_ele(cb_par->adr_src)){
		return 0;
	}
	#if (AUDIO_MESH_MULTY_NODES_TX_EN == 0)
	if(audio_mesh_rx_par[0].tick_last && (cb_par->adr_src != audio_mesh_rx_par[0].adr_src)){
		return 0;
	}
	#endif
	
	audio_mesh_rx_par_t *p_rx = NULL;
	foreach(i, AUDIO_RX_NODES_MAX){
		if(audio_mesh_rx_par[i].adr_src == cb_par->adr_src){
			p_rx = &audio_mesh_rx_par[i];
			break;
		}
	}
		
	if(NULL == p_rx){
		foreach(i, AUDIO_RX_NODES_MAX){
			if(audio_mesh_rx_par[i].adr_src == 0){
				audio_mesh_rx_par[i].adr_src = cb_par->adr_src;
				p_rx = &audio_mesh_rx_par[i];
				audio_active_nodes_cnt_rx++;
				break;
			}
		}
	}

	if(NULL == p_rx){
		return 0;
	}
	
	app_audio_rx_st_fresh(p_rx, cb_par->adr_src);
	
	u8 delta = p_audio->index - p_rx->index_last;
	if(delta>1){
		p_rx->miss_cnt += (delta-1);
//		LOG_MSG_LIB(TL_LOG_NODE_SDK,0, 0,"***************audio data missing cnt:%d",  audio_mesh_rx_par.miss_cnt);
	}
	
	p_rx->index_last = p_audio->index;
//	LOG_MSG_LIB(TL_LOG_NODE_SDK,0, 0,"audio sno:0x%x idx:%d", cb_par->p_nw->sno[0]+(cb_par->p_nw->sno[1]<<8)+(cb_par->p_nw->sno[2]<<16), p_audio->index);
	tadc_int pcm[MIC_SAMPLES_PER_PACKET];
	for(int num=0; num<MIC_NUM_MESH_TX; num++){	
#if (CODEC_ALGORITHM_SEL == CODEC_ALGORITHM_LC3)
		audio_mesh_dec (0, p_audio->data+num*MIC_ENC_SIZE, MIC_ENC_SIZE, pcm, 0);
#elif(CODEC_ALGORITHM_SEL == CODEC_ALGORITHM_SBC)
		tadc_int data[MIC_SAMPLES_PER_PACKET/2];
		mic_audio_decode (0, p_audio->data+num*MIC_ENC_SIZE, SBC_FRAME_SAMPLES, data);
		my_resample8to16_data((int *)data, MIC_SAMPLES_PER_PACKET/2, (int *)pcm, 0);
#endif
		
		foreach(i, MIC_SAMPLES_PER_PACKET){
			p_rx->audio_rx[p_rx->wptr++] = pcm[i];
			if(p_rx->wptr >= (AUDIO_FIFO_SIZE_MAX)){
				p_rx->wptr = 0;
			}
		}
	}
	
#if AUDIO_LED_INDICATION_TEST_EN
	// initial
	audio_led_indication.tick_start = clock_time()|1;
	//audio_led_indication.index = 0;
#endif
    return 0;
}

void audio_rx_proc(void)
{
	tadc_int pcm[MIC_SAMPLES_PER_PACKET]={0};
	if(audio_active_nodes_cnt_rx > 0){
		foreach(i, MIC_SAMPLES_PER_PACKET){ // feacharray(i, pcm)
			s32 sum = 0;
			foreach(j, AUDIO_RX_NODES_MAX){
				audio_mesh_rx_par_t *p_rx = &audio_mesh_rx_par[j];
				if((p_rx->adr_src != 0) && (p_rx->rptr != p_rx->wptr)){
					sum += p_rx->audio_rx[p_rx->rptr];
					p_rx->rptr++;
					if(p_rx->rptr >= (AUDIO_FIFO_SIZE_MAX)){
						p_rx->rptr = 0;
					}		
				}
			}
			
			if(sum >32767){
				pcm[i] = 32767;
			}else if(sum<-32768){
				pcm[i] = -32768;
			}else{
				pcm[i] = (tadc_int)sum;
			}
		}
		audio_led_par_t *par = &audio_led_indication.unit;
		pcm_2_playback(pcm, MIC_SAMPLES_PER_PACKET, &par->avg, &par->max);
		__UNUSED u16 avg_org = par->avg;
		__UNUSED u16 max_org = par->max;
		audio_led_value_get(&par->avg, &par->max);
		LOG_LED_INDICATION_DEBUG(0, 0,"record org:%5d, led:%5d",	avg_org, max_org);
	}
}


int cb_vd_group_g_mic_tx_req(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par)
{
#if (AUDIO_MESH_MULTY_NODES_TX_EN == 0)
	u8 audio_adr_idx = 0;
	#if 0
	foreach(i, AUDIO_RX_NODES_MAX){
		if(cb_par->adr_src == audio_mesh_rx_par[i].adr_src){
			audio_adr_idx = i;
			break;
		}
	}
	#endif
	app_audio_rx_st_clear(audio_adr_idx);
	if(is_own_ele(cb_par->adr_src)){
		return 0;
	}
	app_audio_mic_onoff(0);
	app_audio_rx_st_fresh(&audio_mesh_rx_par[audio_adr_idx], cb_par->adr_src);
#endif

	return 0;
}
#endif

#if AUDIO_MESH_MULTY_NODES_TX_EN
#define AUDIO_MESH_RELAY_STEP_MAX					(8)
_attribute_bss_dlm_ u8 volatile g_audio_mesh_current_relay_cnt;

u8 audio_mesh_get_all_tx_nodes_cnt()	// include self.
{
	return (audio_active_nodes_cnt_rx + is_audio_mesh_tx_working());
}

int app_audio_is_valid_key_pressed(void)
{
	if(!is_audio_mesh_tx_working()){
		if(audio_active_nodes_cnt_rx == AUDIO_RX_NODES_MAX){
			return 0;
		}
	}
	return 1;
}

int audio_mesh_is_reduce_relay_random()
{
	// relay a little faster should be better, or it will occupy 100% tx window.
	return (audio_mesh_get_all_tx_nodes_cnt() >= MIC_NUM_MESH_TX);
}

const u8 AUDIO_MESH_RELAY_CNT_TX_WORKING[AUDIO_MESH_RELAY_STEP_MAX] 		= {3,3,3,2,1,0,0,0};
const u8 AUDIO_MESH_RELAY_CNT_TX_NOT_WORKING[AUDIO_MESH_RELAY_STEP_MAX] 	= {3,3,3,2,1,1,1,0};

u8 audio_mesh_get_tx_retransmit_cnt()
{
	mesh_transmit_t transmit_relay;
	transmit_relay.val = model_sig_cfg_s.relay_retransmit.val;
	if(AUDIO_SAMPLE_RATE == AUDIO_8K){ // not support 16k now
		u8 all_tx_nodes_cnt = audio_mesh_get_all_tx_nodes_cnt();
		if(all_tx_nodes_cnt <= MIC_NUM_MESH_TX){ // MIC_NUM_MESH_TX is 3.
			// keep default value.
		}else if(all_tx_nodes_cnt <= ARRAY_SIZE(AUDIO_MESH_RELAY_CNT_TX_WORKING)){
			if(is_audio_mesh_tx_working()){
				transmit_relay.count = AUDIO_MESH_RELAY_CNT_TX_WORKING[all_tx_nodes_cnt - 1];
			}else{
				transmit_relay.count = AUDIO_MESH_RELAY_CNT_TX_NOT_WORKING[all_tx_nodes_cnt - 1];
			}
		}else{
			transmit_relay.count = 0;
		}
	}
	g_audio_mesh_current_relay_cnt = transmit_relay.count; // for monitor
	
	return transmit_relay.val;
}
#endif


#if APP_NS_ENABLE
_attribute_bss_dlm_ static NS_CFG_PARAS nsParas;
_attribute_bss_dlm_ static SpeexPreprocessState* den;
#define         MIC_SAMPLING_RATE_NS                 16000

void app_ns_init(void)
{
    nsParas.noise_suppress_default = -15;
    nsParas.echo_suppress_default = -55;
    nsParas.echo_suppress_active_default = -45;
    nsParas.low_shelf_enable = 1;
    nsParas.ns_smoothness = 27853;      // QCONST16(0.85f,15)
    nsParas.ns_threshold_low = 100000.0f;

#if 0
    int nsSize = ns_get_size();
    my_dump_str_data(1, "NS Buffer Size", &nsSize, 4);   // 0x234E
    den = (SpeexPreprocessState*)malloc(nsSize);
#else
    static _attribute_bss_dlm_ u32 ns_buffer[0x234E/4+256];
    den = (SpeexPreprocessState*)ns_buffer;
#endif
    ns_init(den, &nsParas, 16000 * NS_SAMPLE_NUM / MIC_SAMPLING_RATE_NS, SPEEX_SAMPLERATE);
}

_attribute_ram_code_
int app_ns_process_frame(s16 *pcm, int size) // "size" must be aligned with NS_SAMPLE_NUM.
{
	int voice_activity_flag = 0;
	foreach(i, size/NS_SAMPLE_NUM){
		voice_activity_flag |= ns_process_frame(den, pcm+i*NS_SAMPLE_NUM);
	}
	return voice_activity_flag;
}
#endif



