/*
 * my_Function.h
 *
 *  Created on: May 23, 2024
 *      Author: DELL
 */

#ifndef VENDOR_TUONG_MY_FUNCTION_H_
#define VENDOR_TUONG_MY_FUNCTION_H_

#include "tl_common.h"
#include "proj/mcu/watchdog_i.h"
#include "vendor/common/user_config.h"
#include "drivers.h"
#include "proj_lib/ble/blt_config.h"
#include "proj_lib/ble/ll/ll.h"
#include "proj_lib/sig_mesh/app_mesh.h"

#include "../UI/ui.h"

//#define ADDR_START  0x201F7000  //sector1: 4kB
#define ADDR_START	0xF7000

#define GATEWAY_ADDRESS 0x0001
#define LED_ADDR 0xFFFF

#define TIMEOUT_SECURE 30   // 30s

#define MODE_DIM_SET 0
#define MODE_CTT_SET 1
#define MODE_RGB_SET 2

#define ENCODER1 GPIO_PE7
#define ENCODER2 GPIO_PE6

u8 flag_flash;

u8 dim_set;
u8 ctt_set;

u8 buf[10];
uint8_t para[8];

u8 stateLed1;
u8 stateLed2;
u8 gio,phut,giay;


lv_color_t color;
u16 hsvH;

u8 countBlinkLed2;
u32 nowtime;  u32 timeKickout; u32 timeProvision;
u32 timeOut;  // time out provision

u8 checkOTA ;
u8 checkScene;
u8 checkKickout;
u8 checkProvision;


typedef struct {
	uint8_t Header[2];
	uint8_t Length;
	uint8_t OpCode[2];
	uint8_t Dim_Stt;
	uint8_t Cct_Stt;
	uint8_t CRC;
} Struct_OPCODE_SET_DIMCCT_t;

void Reset_main(void);
void BlinkLed1(u8 number);
void Blink2Led(u8 number);
void Setting_stt_led(void);
void getValue(void);
void setValue(void);
void check_OTA(void);
void check_provision(void);
void displayClock(void);
void check_Scene(void);
void read_sw(void);
void Set_RGB(void);
void Relay_On_Off(u8 stateLed);
void RD_Send_Relay_Stt(uint8_t Relay_ID, uint8_t Relay_Stt);
void Write_Data_Flash(void);
void Write_Data_Flash2(uint8_t led1_stt);
void Read_Data_Flash(void);
void Encoder_Control(void);

#endif /* VENDOR_TUONG_MY_FUNCTION_H_ */
