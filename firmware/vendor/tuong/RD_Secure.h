/*
 * RD_Secure.h
 *
 *  Created on: Jul 5, 2022
 *      Author: Hao PC
 */

#ifndef RD_SECURE_H_
#define RD_SECURE_H_

//#include "proj/tl_common.h"
//#include "proj/mcu/watchdog_i.h"
//#include "vendor/common/user_config.h"
//#include "proj_lib/pm.h"
//#include "proj_lib/ble/blt_config.h"
//#include "proj_lib/ble/ll/ll.h"
//#include "proj_lib/sig_mesh/app_mesh.h"
//#include "../../common/light.h"
#include "tl_common.h"
#include "proj/mcu/watchdog_i.h"
#include "vendor/common/user_config.h"
#include "drivers.h"
#include "proj_lib/ble/blt_config.h"
#include "proj_lib/ble/ll/ll.h"
#include "proj_lib/sig_mesh/app_mesh.h"

extern unsigned char RD_key[16];



#define RD_OPCODE_TYPE_SEND				(0xE0)
#define RD_OPCODE_TYPE_RSP				(0xE1)

#define RD_HEADER_SET_TYPE_DEVICE		(0x0001)
#define RD_HEADER_SAVE_GW_ADD			(0x0002)
#define RD_HEADER_RSP_TYPE_DEVICE		(0x0003)
#define RD_HEADER_FACTORY_TEST_END		(0x0004)
#define RD_HEADER_KICK_ALL				(0xFFFF)


#define RD_OPCODE_SCENE_SEND			(0xE2)
#define RD_OPCODE_SCENE_RSP				(0xE3)

#define TIME_UOT_SECURE     120000    // 120s factory reset 
#define TIME_UOT_KEY_FAIL 	5000		// 5s factory reset if key fail
typedef struct
{
    uint8_t Check_Secure_Ready;    // call after Provisioned
    uint8_t Secure_Pass;
    uint32_t Time_Start_check;     // time ms in provisiones
    uint8_t  Check_Key_Fail;
    uint32_t Time_Check_Key_Fail;
} Secure_Stt_Str;


extern int RD_Messenger_ProcessCommingProcess_Type(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
/**
 * @brief Check Secure key by RD
 * 
 * @param key : const 16byte define company. Ralli Smart use "RD_key[16]" defined 
 * @param mac : 6 byte mac ID of device. telink sdk named: tbl_mac[].
 * @param unicast : primary unicast add of device . telink sdk named: ele_adr_primary
 * @param compare_key : key gate way send to compare with hey device gen.
 * @return unsigned char   1: Secure passed 
 *                         0: secure fail
 */
unsigned char RD_Secure_AesreCheck(uint8_t key[16], uint8_t mac[6], uint16_t unicast, uint8_t compare_key[6]);
/**
 * @brief call in loop.  Check secure after provisioned end factory reset when time out 120s
 * config by TIME_UOT_SECURE
 * 
 */
void RD_Secure_CheckLoop(void);
/**
 * @brief check when powerup. factory reset if secure fail and provisioned
 * Note: call functions after init Flash Data
 * 
 */
void RD_Secure_CheckInit(void);

/**
 * @brief Set Secure stt.  when pass secure device don't check reset end keep working 
 *                         when pail
 * 
 * @param Secure_Pass_Stt 	RD_EN: save Secure_Stt Pass
 * 							RD_DIS: save Secure_Stt fail 
 */
void RD_Secure_Set(uint8_t Secure_Pass_Stt);

/**
 * @brief Call when check secure fail. device will reset after TIME_UOT_KEY_FAIL millisecond
 * 
 */
void RD_Secure_Check_Key_Fail(void);
#endif /* RD_SECURE_H_ */
