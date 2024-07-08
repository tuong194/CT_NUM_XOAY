/********************************************************************************************************
 * @file	app_buffer.h
 *
 * @brief	This is the header file for BLE SDK
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
#ifndef VENDOR_B91_BLE_SAMPLE_APP_BUFFER_H_
#define VENDOR_B91_BLE_SAMPLE_APP_BUFFER_H_

#include "vendor/common/user_config.h"


/**
 * @brief	connMaxRxOctets
 * refer to BLE SPEC "4.5.10 Data PDU length management" & "2.4.2.21 LL_LENGTH_REQ and LL_LENGTH_RSP"
 * usage limitation:
 * 1. should be in range of 27 ~ 251
 */
#define ACL_CONN_MAX_RX_OCTETS			DLE_LEN_MAX_RX


/**
 * @brief	connMaxTxOctets
 * refer to BLE SPEC "4.5.10 Data PDU length management" & "2.4.2.21 LL_LENGTH_REQ and LL_LENGTH_RSP"
 * usage limitation:
 * 1. connMaxTxOctets should be in range of 27 ~ 251
 */
#define ACL_CONN_MAX_TX_OCTETS			(((39==DLE_LEN_MAX_TX)||(40==DLE_LEN_MAX_TX)) ? 38 : DLE_LEN_MAX_TX) // set 38 to align 16 after adding 10. and 38 can still report one ADV packet in one single GATT RF packet.


#define BLT_RX_FIFO_CNT			8	//adv_filter_proc() reserve (BLE_RCV_FIFO_MAX_CNT+2) buf while gatt connecting.
#define BLT_TX_FIFO_CNT			16	// set to 16, because there is no blt_notify_fifo_

/********************* ACL connection LinkLayer TX & RX data FIFO allocation, Begin ************************************************/
/**
 * @brief	ACL RX buffer size & number
 *  		ACL RX buffer is used to hold LinkLayer RF RX data.
 * usage limitation for ACL_RX_FIFO_SIZE:
 * 1. should be greater than or equal to (connMaxRxOctets + 21)
 * 2. should be be an integer multiple of 16 (16 Byte align). user can use formula:  size = CAL_LL_ACL_RX_FIFO_SIZE(connMaxRxOctets)
 * usage limitation for ACL_RX_FIFO_NUM:
 * 1. must be: 2^n, (power of 2)
 * 2. at least 4; recommended value: 8, 16
 */
#define ACL_RX_FIFO_SIZE				CAL_LL_ACL_RX_FIFO_SIZE(ACL_CONN_MAX_RX_OCTETS)  // ACL_CONN_MAX_RX_OCTETS + 21, then 16 Byte align
#define ACL_RX_FIFO_NUM					BLT_RX_FIFO_CNT	// must be: 2^n


/**
 * @brief	ACL TX buffer size & number
 *  		ACL TX buffer is used to hold LinkLayer RF TX data.
 * usage limitation for ACL_TX_FIFO_SIZE:
 * 1. should be greater than or equal to (connMaxTxOctets + 10)
 * 2. should be be an integer multiple of 16 (16 Byte align). user can use formula:  size = CAL_LL_ACL_TX_FIFO_SIZE(connMaxTxOctets)
 * usage limitation for ACL_TX_FIFO_NUM:
 * 1. must be: (2^n) + 1, (power of 2, then add 1)
 * 2. at least 9; recommended value: 9, 17, 33; other value not allowed.
 * usage limitation for size * (number - 1)
 * 1. (ACL_TX_FIFO_SIZE * (ACL_TX_FIFO_NUM - 1)) must be less than 4096 (4K)
 *    so when ACL TX FIFO size equal to or bigger than 256, ACL TX FIFO number can only be 9(can not use 17 or 33), cause 256*(17-1)=4096
 */
#define ACL_TX_FIFO_SIZE				CAL_LL_ACL_TX_FIFO_SIZE(ACL_CONN_MAX_TX_OCTETS)	// ACL_CONN_MAX_TX_OCTETS + 10, then 16 Byte align
#define ACL_TX_FIFO_NUM					(BLT_TX_FIFO_CNT + 1)	// must be: (2^n) + 1




extern	u8	app_acl_rxfifo[];
extern	u8	app_acl_txfifo[];
/******************** ACL connection LinkLayer TX & RX data FIFO allocation, End ***************************************************/

/********************* ACL scan RX data FIFO allocation, Begin ********************************/
#if EXTENDED_ADV_ENABLE
#define SCAN_PRICHN_RXFIFO_SIZE			(ACL_RX_FIFO_SIZE)  //37+24(maybe 17, 24 is come from 826x,) Eagle: 37+21,16B align(dma rx size)
#else
#define SCAN_PRICHN_RXFIFO_SIZE			64  //37+24(maybe 17, 24 is come from 826x,) Eagle: 37+21,16B align(dma rx size)
#endif

#define SCAN_PRICHN_RXFIFO_NUM			8   //must be 2^n
/********************* ACL scan RX data FIFO allocation, End **********************************/



#endif /* VENDOR_B91_BLE_SAMPLE_APP_BUFFER_H_ */
