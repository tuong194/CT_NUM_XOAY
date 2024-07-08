/********************************************************************************************************
 * @file	my_mic_api.h
 *
 * @brief	This is the header file for B91m
 *
 * @author	Driver Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

#include <stdint.h>

typedef	struct
{
    int16_t X[2][328];
    uint32_t scale_factor[2][8];
    int32_t sb_sample_f[16][2][8];
    int32_t position;
    uint32_t sbc_analyze_even;

    uint8_t blocks;
    uint8_t bitpool;
    uint8_t allocation;
}my_mic_enc_para_t;


typedef	struct
{
#ifdef F_FLOAT
    float V[2][160];  
#else
    int16_t V[2][160];
#endif
    uint8_t blocks;
    uint8_t bitpool;
    uint8_t allocation;
}my_mic_dec_para_t;



void my_mic_enc_init(my_mic_enc_para_t* encoder, u8 blocks, u8 bitpool);
int  my_mic_enc(my_mic_enc_para_t* encoder, s16 *ps, int samples, u8 *pd);
int  my_mic_get_enc_len(my_mic_enc_para_t* encoder);


void my_mic_dec_init(my_mic_dec_para_t* decoder, u8 blocks, u8 bitpool);
int  my_mic_dec(my_mic_dec_para_t* decoder, u8 *ps, int samples, s16 *pd);
int  my_mic_get_enc_len_by_decoder(my_mic_dec_para_t* decoder);

