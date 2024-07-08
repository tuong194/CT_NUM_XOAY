/********************************************************************************************************
 * @file	app_audio.h
 *
 * @brief	This is the header file for BLE SDK
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
 * app_audio.h
 *
 *  Created on: Jun 20, 2022
 *      Author: telink
 */

#pragma once

#include "tl_common.h"
#include "drivers.h"
#include "my_resample.h"


#define CODEC_DAC_MONO_MODE         1
#define CODEC_ADC_MONO_MODE         1


#if		CODEC_DAC_MONO_MODE
	typedef	signed short 	tcodec_int;
#else
	typedef	int 	tcodec_int;
#endif

#if		CODEC_ADC_MONO_MODE
	typedef	signed short 	tadc_int;
#else
	typedef	int 	tadc_int;
#endif

#ifndef	MIC_FIFO_SIZE
#define	MIC_FIFO_SIZE			2048
#define AUDIO_FIFO_SIZE_MAX		(MIC_FIFO_SIZE + 256)

#endif

#ifndef PLAY_FIFO_SIZE
#define	PLAY_FIFO_SIZE			8192  // for sample rate 16k or above
#endif

#ifndef PLAY_BLOCK_SIZE
#define	PLAY_BLOCK_SIZE			128
#endif

#define PLAY_FIFO_MAX			(PLAY_FIFO_SIZE - 1)
#define PLAY_FIFO_SIZE_BYTE		(PLAY_FIFO_SIZE * sizeof(tcodec_int))

#define MIC_FIFO_MAX			(MIC_FIFO_SIZE - 1)
#define MIC_FIFO_SIZE_BYTE		(MIC_FIFO_SIZE * sizeof(tadc_int))


typedef	struct	tcodec_cfg
{
	int			sample_rate;	 //= 44100;		//
	int			samples_played;
	u16			mute_samples;
	u16			latency;
	u8			mode;			// mono:0; stereo 1
	u8			sel;
	u8			status;				// idle, play, play_mute, powerdown
	u8			rsvd;

	short		mic_rptr;
	short		mic_wptr;
	short		play_wptr;
	short		play_rptr;
	int			play_dec_samples;
} tcodec_cfg_t;

typedef struct{
	u32 tick_last;
	u32 miss_cnt;
	u16 adr_src;
	u8 index_last;
	u8 rsv;  // for 4 bytes align
	u16 wptr;
	u16 rptr;
	tcodec_int audio_rx[AUDIO_FIFO_SIZE_MAX];
}audio_mesh_rx_par_t;

extern u32 audio_mesh_tx_tick;
#if AUDIO_MESH_EN
typedef struct{
	u8 index;
	u8 rsv[2];
	u8 data[MIC_NUM_MESH_TX*MIC_ENC_SIZE];
}vd_audio_t;

typedef struct{
	u16 avg;
	u16 max;
}audio_led_par_t;

typedef struct{
	u32 tick_start;
	audio_led_par_t unit;
	u32 index;
}audio_led_indication_t;
#endif

#if AUDIO_MESH_EN
#define APP_NS_ENABLE			1
#endif

#if APP_NS_ENABLE // about noise cancel
#define USE_DC_NOTCH
#define SPEEX_SAMPLERATE 		16000
#define NS_SAMPLE_NUM			120

typedef struct {
	int   low_shelf_enable;          /* 1: enable lowshelf filter, 0: disable lowshelf filter */
	int   noise_suppress_default;    /* noise suppression ratio, set to -15db by default */
	int   echo_suppress_default;  
	int   echo_suppress_active_default;
	short ns_smoothness;             /* gain smoothing factor in Q15 format */
	_align_4_ float ns_threshold_low;/* must align 4, because library has removed fpack struct setting when compile. */
	int   reserved;
} NS_CFG_PARAS;


/** Speex pre-processor state. */
struct SpeexPreprocessState_;

typedef struct SpeexPreprocessState_ SpeexPreprocessState;

typedef signed short spx_int16_t;

void app_ns_init(void);
int app_ns_process_frame(s16 *pcm, int size);

/** Creates a new preprocessing state. You MUST create one state per channel processed.
 * @param frame_size Number of samples to process at one time (should correspond to 10-20 ms). Must be
 * the same value as that used for the echo canceller for residual echo cancellation to work.
 * @param sampling_rate Sampling rate used for the input.
 * @return Newly created preprocessor state
*/
void ns_init(SpeexPreprocessState* st, NS_CFG_PARAS* param, int frame_size, int sampling_rate);

/** Preprocess a frame
 * @param st Preprocessor state
 * @param x Audio sample vector (in and out). Must be same size as specified in ns_init().
 * @return Bool value for voice activity (1 for speech, 0 for noise/silence), ONLY if VAD turned on.
*/
int ns_process_frame(SpeexPreprocessState* st, spx_int16_t* x);
#endif


static inline int is_audio_mesh_tx_working()
{
	return (0 != audio_mesh_tx_tick);
}

extern audio_i2s_codec_config_t audio_i2s_codec_config;

void app_audio_init ();
void app_audio_task();
void app_audio_mic_onoff(u8 on);
int audio_mesh_enc_init (unsigned char *buff, int buffsize, int sample_rate, int channels, int bitrate, int mode);
int audio_mesh_dec_init (unsigned char *buff, int buffsize, int sample_rate, int channels, int bitrate, int mode);
void audio_mesh_tns_enable (int chn, int32_t en);
int audio_mesh_enc (int chn, int16_t* ps, int ns, uint8_t* pd);
int audio_mesh_dec (int chn, uint8_t* ps, int ns, int16_t* pd, int32_t plc);
int cb_vd_async_audio_data(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
void audio_rx_proc(void);
int cb_vd_group_g_mic_tx_req(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int vd_cmd_audio_data(u16 adr_dst, u8 rsp_max, u8 *par, int len);
int vd_cmd_mic_tx_req(u16 adr_dst);
void proc_ui_audio();
void app_audio_set_aux_payload(u8 *p_bear);
u8 audio_mesh_get_tx_nodes_cnt();
int app_audio_is_valid_key_pressed(void);
int audio_mesh_is_reduce_relay_random();
u8 audio_mesh_get_tx_retransmit_cnt();
void audio_i2s_set_pin(void);
void audio_set_chn_wl(audio_channel_wl_mode_e chn_wl);


