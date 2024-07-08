/********************************************************************************************************
 * @file	sbc_api.h
 *
 * @brief	This is the header file for B91m
 *
 * @author	Driver Group
 * @date	2019
 *
 * @par		Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd.
 *			All rights reserved.
 *
 *          The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or
 *          alter) any information contained herein in whole or in part except as expressly authorized
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#ifndef SBC_API_H
#define SBC_API_H

//#define F_FLOAT

#include <stdint.h>

/*! Construct version number from major/minor/micro values. */
#define SBC_VERSION_INT(major, minor, micro) (((major) << 16) | ((minor) << 8) | (micro))

/*! Version number to ensure header and binary are matching. */
#define SBC_VERSION SBC_VERSION_INT(0, 3, 0)

#define SBC_MAX_CHANNELS    2
#define SBC_X_BUFFER_SIZE   328
#define SBC_MAX_SUBBANDS    8
#define SBC_MAX_BLOCKS      16

typedef unsigned char u8;

/*! Return library version number. It should match SBC_VERSION. */
int sbc_get_version(void);

typedef	struct sbc_enc_para
{
    int16_t X[SBC_MAX_CHANNELS][SBC_X_BUFFER_SIZE];
    uint32_t scale_factor[SBC_MAX_CHANNELS][SBC_MAX_SUBBANDS];
    int32_t sb_sample_f[SBC_MAX_BLOCKS][SBC_MAX_CHANNELS][SBC_MAX_SUBBANDS];
    int32_t position;
    uint32_t sbc_analyze_even;

    u8 sbc_blocks;
    u8 sbc_bitpool;
    u8 sbc_allocation;

}sbc_enc_para_t;

typedef	struct sbc_dec_para
{

#ifdef F_FLOAT
    float V[2][160];  
#else
    int16_t V[2][160];
#endif

    u8 sbc_blocks;
    u8 sbc_bitpool;
    u8 sbc_allocation;

}sbc_dec_para_t;


int sbc_enc_get_size(void);
int sbc_dec_get_size(void);

int sbc_enc_init(sbc_enc_para_t* encoder_p);
int sbc_dec_init(sbc_dec_para_t* decoder_p);

/*
* name:		sbc_enc / sbc_dec
* param:	msbc: 1->msbc   0->sbc
*/
uint32_t sbc_dec_process(sbc_dec_para_t* decoder, const uint8_t* buf, uint32_t len, uint8_t* outbuf, uint32_t outbuf_len, uint32_t* out_len, int msbc, uint8_t sbc_out_chn_mask);
uint32_t sbc_enc_process(sbc_enc_para_t* encoder, const uint8_t* buf, uint16_t len, uint8_t* outbuf, uint32_t outbuf_len, uint32_t* out_len, int msbc);


/*
* allocation: 1->SNR   0->LOUDNESS
*/
void sbc_enc_set_params(sbc_enc_para_t* encoder, u8 blocks, u8 bitpool,u8 allocation);
void sbc_dec_set_params(sbc_dec_para_t* deocder,u8 blocks, u8 bitpool,u8 allocation);


#endif
