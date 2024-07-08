/*
 * LED_LC8823.h
 *
 *  Created on: Jun 18, 2024
 *      Author: DELL
 */

#ifndef VENDOR_TUONG_LED_LC8823_H_
#define VENDOR_TUONG_LED_LC8823_H_

#include "tl_common.h"
#include "proj/mcu/watchdog_i.h"
#include "vendor/common/user_config.h"
#include "drivers.h"
#include "proj_lib/ble/blt_config.h"
#include "proj_lib/ble/ll/ll.h"
#include "proj_lib/sig_mesh/app_mesh.h"

#define FREQ_LED 25000

#define LED_DI GPIO_PA5
#define LED_CI GPIO_PA6

typedef enum{
	BLUE_LC8823,
	GREEN_LC8823,
	RED_LC8823,
	YELLOW_LC8823,
	COLORx_LC8823,
}color_led_rgb;

typedef enum{
	ON_LED_SW1 = 0,
	OFF_LED_SW1,
}on_off_led_sw1;

typedef enum{
	ON_LED_SW2 = 0,
	OFF_LED_SW2,
}on_off_led_sw2;

typedef enum{
	ON_LED_SW3 = 0,
	OFF_LED_SW3,
}on_off_led_sw3;

typedef enum{
	ON_LED_SW4 = 0,
	OFF_LED_SW4,
}on_off_led_sw4;

void Config_Pin_Led_Lc8823(void);
void On_Led(u8 led_num, color_led_rgb color);
void On_20_Led_Blue(void);
void On_20_Led_Green(void);
void On_20_Led_Red(void);
void Blink_20_Led_Red(void);
void Blink_20_Led_Green(void);
void Blink_20_Led_Blue(void);
void Off_20_Led(void);

void func_led_r(void);
void func_led_l(void);
void On_Led_4SW(void);
void On_Off_Led_SW(on_off_led_sw1 stt_sw1, on_off_led_sw2 stt_sw2, on_off_led_sw3 stt_sw3, on_off_led_sw4 stt_sw4);

#endif /* VENDOR_TUONG_LED_LC8823_H_ */
