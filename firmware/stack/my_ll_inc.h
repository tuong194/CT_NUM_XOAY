/********************************************************************************************************
 * @file	my_ll_inc.h
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
#ifndef MY_LL_INC_H
#define MY_LL_INC_H

void my_asrc_44to48_init ();
int my_asrc_44to48_data (int *ps, int n, int *pd);
void my_asrc_48to44_init();
int my_asrc_48to44_data(int* ps, int n, int* pd);


int  my_asrc_init(unsigned char *buffer, unsigned int buffer_len, int ppm_data_len_max, int chn);

void my_asrc_set_ppm (int handle, int ppm);

void my_asrc_switch(int handle, int x);

int  my_asrc_data_mono (int handle, int chn, short *ps, int n, short *pd);

int  my_asrc_data_stereo (int handle, int *ps, int n, int *pd);

int my_asrc_get_output_data_mono (int handle, int ilen);


int my_llenc_init (void * p, int insize, int channels, int bitrate);

int my_lldec_init (void * p, int insize, int channels);

int my_llenc (int chn, int16_t *ps, int ns, uint8_t *pd);

int my_lldec (int chn, uint8_t *ps, int ns, int16_t *pd, int plc);


#ifndef	MY_LIB_ID
#define	MY_LIB_ID		0
#endif

#ifndef	MY_MIC_CODEC
#define	MY_MIC_CODEC	(MY_LIB_ID == 2 || MY_LIB_ID == 3)
#endif

#ifndef	MY_LL_CODEC
#define	MY_LL_CODEC		(MY_LIB_ID == 2 || MY_LIB_ID == 3 || MY_LIB_ID == 5)
#endif

#ifndef	MY_SRS_LIB
#define	MY_SRS_LIB		(MY_LIB_ID == 1)
#endif

#ifndef	MY_ASRC_LIB
#define	MY_ASRC_LIB		(MY_LIB_ID == 2 || MY_LIB_ID == 5)
#endif

#ifndef	MY_AFH_LIB
#define	MY_AFH_LIB	    1 //(MY_LIB_ID == 1 || MY_LIB_ID == 2 || MY_LIB_ID == 3 || MY_LIB_ID == 5)
#endif

#ifndef MY_MIC_CODEC_ENABLE
#define	MY_MIC_CODEC_ENABLE	0
#endif

#endif

