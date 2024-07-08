/********************************************************************************************************
 * @file	gw_node_info.h
 *
 * @brief	for TLSR chips
 *
 * @author	telink
 * @date	Sep. 30, 2010
 *
 * @par     Copyright (c) 2017, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef __GW_NODE_INFO_H_
#define __GW_NODE_INFO_H_
#include "vendor/common/mesh_node.h"

#define ELE_LIGHT_MODEL_SIZE  (380-12)	

typedef struct{
	u8 nums;
	u8 numv;
	u8 id[ELE_LIGHT_MODEL_SIZE];
}mesh_element_model_id;

typedef struct{
    u16 len_cps;
    #if 1   // must same with page0_local_t from start to numv
    mesh_page0_head_t page0_head;
    u16 local;
    mesh_element_model_id model_id;
    #endif
}VC_node_cps_t;

#if DONGLE_PROVISION_EN&&!WIN32
typedef struct{
    u16 node_adr;    // primary address
    u8 element_cnt;
    u8 rsv;
    u8 dev_key[16];
	#if (MD_REMOTE_PROV && DEVICE_KEY_REFRESH_ENABLE)
	u8 dev_key_candi[16];
	#endif
}VC_node_info_t;

typedef struct{
	u16 node_adr;
	u8 ele_cnt;
	u16 index;
}gw_node_info_t;

#define SINGLE_NODE_INFO_SIZE	sizeof(VC_node_info_t)
extern _align_4_ gw_node_info_t gw_node_info[];
#else
typedef struct{
    u16 node_adr;    // primary address
    u8 element_cnt;
    u8 rsv;
    u8 dev_key[16];
    VC_node_cps_t cps;
	#if WIN32
	u8 dev_key_candi[16];
	#endif
}VC_node_info_t;    // size is 404(0x194)
#endif

extern _align_4_ VC_node_info_t VC_node_info[];
extern VC_node_cps_t VC_node_cps;

void clear_gw_node_info(u16 addr, int is_must_primary);
#endif

