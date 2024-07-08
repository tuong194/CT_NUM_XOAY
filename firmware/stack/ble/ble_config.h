/********************************************************************************************************
 * @file	ble_config.h
 *
 * @brief	This is the header file for BLE SDK
 *
 * @author	BLE GROUP
 * @date	06,2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************************************/
#pragma once


#include "drivers.h"
#include "tl_common.h"
#include "proj_lib/ble/blt_config.h"	// add by qifa

// Version : CERTIFICATION_MARK.SOFT_STRUCTURE.Major.Minor Patch
#define	CERTIFICATION_MARK			3
#define	SOFT_STRUCTURE				3
#define	MAJOR_VERSION				0
#define	MINOR_VERSION				0
#define	PATCH						0
#define	RSVD						0xff


#if (MCU_CORE_TYPE == MCU_CORE_825x)
	#define	FIX_HW_CRC24_EN									1
	#define HW_ECDH_EN                                      0
#elif (MCU_CORE_TYPE == MCU_CORE_827x)
	#define	FIX_HW_CRC24_EN									0
	#define HW_ECDH_EN                                      1
#elif (MCU_CORE_TYPE == MCU_CORE_9518)
	#define	FIX_HW_CRC24_EN									0
	#define HW_ECDH_EN                                      1
	#define BLT_CONN_MASTER_EN								0
	#define BLT_SCAN_EN										0	// be replaced by BLT_SCAN_IN_ADV_EN in B91
	#define BLT_ADV_IN_CONN_SLAVE_EN						0	// mesh need to send in loop, not only right after BRX_E.
	#define BLT_SCAN_IN_ADV_EN								1
	#define BLT_SCAN_IN_CONN_SLAVE_EN						1
	#define STACK_IRQ_CODE_IN_SRAM_DUE_TO_FLASH_OPERATION			1	//Eagle use high IRQ priority for BLE task
#else
	#error "unsupported mcu type !"
#endif



#ifndef			DATA_NO_INIT_EN
#define			DATA_NO_INIT_EN									0
#endif

///////////////////  Feature ////////////////////////////
#ifndef			BLT_CONN_MASTER_EN
#define			BLT_CONN_MASTER_EN								1
#endif

#ifndef			BLT_SCAN_EN
#define			BLT_SCAN_EN										1
#endif

#ifndef			BLT_ADV_IN_CONN_SLAVE_EN
#define			BLT_ADV_IN_CONN_SLAVE_EN						1
#endif

#ifndef			BLT_SCAN_IN_CONN_SLAVE_EN
#define			BLT_SCAN_IN_CONN_SLAVE_EN						1
#endif



///////////////////  Feature ////////////////////////////

/*	This code in RF irq and system irq put in RAM by force
 * Because of the flash resource contention problem, when the
 * flash access is interrupted by a higher priority interrupt,
 * the interrupt processing function cannot operate the flash(For Eagle)
*/
#ifndef STACK_IRQ_CODE_IN_SRAM_DUE_TO_FLASH_OPERATION
#define STACK_IRQ_CODE_IN_SRAM_DUE_TO_FLASH_OPERATION				0
#endif



#ifndef SECURE_CONNECTION_ENABLE
#define SECURE_CONNECTION_ENABLE							1
#endif






//conn param update/map update
#ifndef	BLS_PROC_MASTER_UPDATE_REQ_IN_IRQ_ENABLE
#define BLS_PROC_MASTER_UPDATE_REQ_IN_IRQ_ENABLE			1
#endif


#ifndef LE_AUTHENTICATED_PAYLOAD_TIMEOUT_SUPPORT_EN
#define LE_AUTHENTICATED_PAYLOAD_TIMEOUT_SUPPORT_EN			0
#endif





#ifndef DEEP_RET_ENTRY_CONDITIONS_OPTIMIZATION_EN
#define	DEEP_RET_ENTRY_CONDITIONS_OPTIMIZATION_EN			0
#endif


#ifndef HOST_CONTROLLER_DATA_FLOW_IMPROVE_EN
#define	HOST_CONTROLLER_DATA_FLOW_IMPROVE_EN				1
#endif



//Link layer feature enable flag default setting
#ifndef LL_FEATURE_SUPPORT_LE_DATA_LENGTH_EXTENSION
#define LL_FEATURE_SUPPORT_LE_DATA_LENGTH_EXTENSION					1
#endif

#ifndef LL_FEATURE_SUPPORT_LE_2M_PHY
#define LL_FEATURE_SUPPORT_LE_2M_PHY								1
#endif

#ifndef LL_FEATURE_SUPPORT_LE_CODED_PHY
#define LL_FEATURE_SUPPORT_LE_CODED_PHY								1
#endif

#ifndef LL_FEATURE_SUPPORT_LE_EXTENDED_ADVERTISING
#define LL_FEATURE_SUPPORT_LE_EXTENDED_ADVERTISING					1
#endif

#ifndef LL_FEATURE_SUPPORT_LE_PERIODIC_ADVERTISING
#define LL_FEATURE_SUPPORT_LE_PERIODIC_ADVERTISING					0
#endif

#ifndef LL_FEATURE_SUPPORT_CHANNEL_SELECTION_ALGORITHM2
#define LL_FEATURE_SUPPORT_CHANNEL_SELECTION_ALGORITHM2				1
#endif




//core_5.2 feature begin
#ifndef LL_FEATURE_SUPPORT_CONNECTED_ISOCHRONOUS_STREAM_MASTER
#define LL_FEATURE_SUPPORT_CONNECTED_ISOCHRONOUS_STREAM_MASTER		1
#endif

#ifndef LL_FEATURE_SUPPORT_CONNECTED_ISOCHRONOUS_STREAM_SLAVE
#define LL_FEATURE_SUPPORT_CONNECTED_ISOCHRONOUS_STREAM_SLAVE		1
#endif

#ifndef LL_FEATURE_SUPPORT_ISOCHRONOUS_BROADCASTER
#define LL_FEATURE_SUPPORT_ISOCHRONOUS_BROADCASTER					1
#endif

#ifndef LL_FEATURE_SUPPORT_SYNCHRONIZED_RECEIVER
#define LL_FEATURE_SUPPORT_SYNCHRONIZED_RECEIVER					1
#endif

#ifndef LL_FEATURE_SUPPORT_ISOCHRONOUS_CHANNELS
#define LL_FEATURE_SUPPORT_ISOCHRONOUS_CHANNELS						1
#endif
//core_5.2 feature end


#ifndef ZBIT_FLASH_WRITE_TIME_LONG_WORKAROUND_EN
#define ZBIT_FLASH_WRITE_TIME_LONG_WORKAROUND_EN					0
#endif



/*
 * for B85m IC, not support break nesting, generally use GD flash,  add second flash vendor which
 * provide Sonos architecture flash(such as PUYA),
 *
 * feature:
 * write 1B, 2B, ... 255B timing cost equal to write a page(256B),
 * about 1~2 mS. In flash_write_page function, disable IRQ may lead to RX packet loss for BTX/BRX
 * so we should take some action to workaround this problem
 */
#ifndef SONOS_FLASH_WRITE_TIME_LONG_WORKAROUND_EN
#define SONOS_FLASH_WRITE_TIME_LONG_WORKAROUND_EN					0
#endif


#ifndef		MULTIPLE_LOCAL_DEVICE_ENABLE
#define		MULTIPLE_LOCAL_DEVICE_ENABLE				    		1
#endif

#ifndef		LOCAL_DEVICE_NUM_MAX
#define		LOCAL_DEVICE_NUM_MAX				    				4
#endif


/* 1: multiple connection SDK; 0 : single connection SDK */
#define	BLE_MULTIPLE_CONNECTION_ENABLE								0




#ifndef	BLE_IRQ_DBG_EN
#define BLE_IRQ_DBG_EN					0
#endif

#ifndef	BLE_ADV_DBG_EN
#define BLE_ADV_DBG_EN					0
#endif

#ifndef	TX_FIFO_DBG_EN
#define TX_FIFO_DBG_EN					0
#endif


#ifndef		DEBUG_PAIRING_ENCRYPTION
#define 	DEBUG_PAIRING_ENCRYPTION		0
#endif





#ifndef LL_FEATURE_LOCAL_RPA_ENABLE
#define LL_FEATURE_LOCAL_RPA_ENABLE									0
#endif


//////////// for temp debug, will remove later after tested OK ///////////////////
#define NEW_LEGACY_ADV_CODE											0

#define LL_PRIVACY_NEW_CODE											0


#if(LL_PRIVACY_NEW_CODE)
	#define LEGADV_PRIVACY_NEW_CODE									1
	#define CONN_PRIVACY_NEW_CODE									1
#else
	#define LEGADV_PRIVACY_NEW_CODE									0
	#define CONN_PRIVACY_NEW_CODE									0
#endif
