/********************************************************************************************************
 * @file	app_config_B91.h
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
#pragma once

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

#include "vendor/common/version.h"    // include mesh_config.h inside.

#define BLE_OTA_ENABLE						1	// telink GATT OTA protocol
#define TELIK_SPP_SERVICE_ENABLE			0
#define BLT_APP_LED_ENABLE					0
#define DUMP_STR_EN                         0	// usb output debug log
#define	VCD_EN								0	// usb output debug log


//firmware check
#define FIRMWARES_SIGNATURE_ENABLE          0


//////////////// SMP SETTING  //////////////////////////////
#define BLE_SECURITY_ENABLE 			   	0
//////////////////board sel/////////////////////////////////////
#define PCBA_ADK80D_C1T213A20_V13          	1	// development board
#define PCBA_C1T213A16_V13_PA_DONGLE 		2	// PA dongle
#define PCBA_C1T213A3_V11_DONGLE			3 	// dongle without PA
#define PCBA_C1T216A20_V1_2                 4	//  without SWS for this PCBA.

#ifndef PCBA_B91_SEL // user can define in user_app_config.h
#define PCBA_B91_SEL			PCBA_ADK80D_C1T213A20_V13
#endif


#define _USER_CONFIG_DEFINED_	1	// must define this macro to make others known
#define	__LOG_RT_ENABLE__		0
//#define	__DEBUG_PRINT__			0

#if (DUAL_MESH_ZB_BL_EN || DUAL_MESH_SIG_PVT_EN)
#define FLASH_PLUS_ENABLE     	0
#elif DUAL_VENDOR_EN
#define FLASH_PLUS_ENABLE    	0
#else
	#if MI_API_ENABLE
#define FLASH_PLUS_ENABLE     	0
	#else
#define FLASH_PLUS_ENABLE     	0	// means 2M flash for B91, 1M flash for B85m
	#endif
#endif

#if FLASH_PLUS_ENABLE
#if (DUAL_MESH_ZB_BL_EN || DUAL_MESH_SIG_PVT_EN)
#define PINGPONG_OTA_DISABLE    1 // it can disable only when flash plus.
#else
#define PINGPONG_OTA_DISABLE    0 // it can disable only when flash plus.
#endif
#if	PINGPONG_OTA_DISABLE
#define SWITCH_FW_ENABLE		0 // set to 0, just for particular customer 
#endif
#endif

#define MESH_FLASH_PROTECTION_EN		0

//////////// product  Information  //////////////////////////////
#define ID_VENDOR				0x248a			// for report
#define ID_PRODUCT_BASE			0x880C
#define STRING_VENDOR			L"Telink"
#define STRING_PRODUCT			L"BLE Mesh"
#define STRING_SERIAL			L"TLSR825X"

#define DEV_NAME                "SigMesh"

#define APPLICATION_DONGLE		0					// or else APPLICATION_DEVICE
#define	USB_PRINTER				1
#define	FLOW_NO_OS				1

/////////////////////HCI ACCESS OPTIONS///////////////////////////////////////
#define HCI_USE_NONE	0
#define HCI_USE_UART	1
#define HCI_USE_USB		2

#ifndef HCI_ACCESS
#if (WIN32 || PTS_TEST_EN)
#define HCI_ACCESS		HCI_USE_USB
#elif MESH_MONITOR_EN
#define HCI_ACCESS		HCI_USE_UART
#else
	#if GATT_RP_EN
#define HCI_ACCESS		HCI_USE_UART
	#else
#define HCI_ACCESS		HCI_USE_NONE
	#endif
#endif 

#if (HCI_ACCESS==HCI_USE_UART)
#define UART_TX_PIN				UART0_TX_PB2
#define UART_RX_PIN				UART0_RX_PB3
#define UART_DMA_BAUDRATE		115200
#endif
#endif

#ifndef HCI_LOG_FW_EN
#define HCI_LOG_FW_EN   (0 || DEBUG_LOG_SETTING_DEVELOP_MODE_EN)
#if HCI_LOG_FW_EN
#define DEBUG_INFO_TX_PIN           		GPIO_PC4
#define PRINT_DEBUG_INFO                    1
#endif
#endif

#define BATT_CHECK_ENABLE       			0   // need to connect GPIO_BAT_DETECT pin to VCC on PCBA when VBAT_CHANNEL_EN is 0.
#if (BATT_CHECK_ENABLE)
#define VBAT_CHANNEL_EN						0

#if VBAT_CHANNEL_EN
	/**		The battery voltage sample range is 1.8~3.5V    **/
#else
	/** 	if the battery voltage > 3.6V, should take some external voltage divider	**/
	#define GPIO_BAT_DETECT					GPIO_PB0
	#define PB0_FUNC						AS_GPIO
	#define PB0_INPUT_ENABLE				0
	#define PB0_OUTPUT_ENABLE				0
	#define PB0_DATA_OUT					0
	#define ADC_INPUT_PIN_CHN				ADC_GPIO_PB0
#endif
#endif

#define JTAG_DEBUG_ENABLE					0
#if (JTAG_DEBUG_ENABLE)//2-wire jtag mode

#define PE4_FUNC	AS_TDI  //JTAG 4-WIRE FUNCTION
#define PE5_FUNC	AS_TDO  //JTAG 4-WIRE FUNCTION
#define PE6_FUNC	AS_TMS  //JTAG 4-WIRE FUNCTION
#define PE7_FUNC	AS_TCK  //JTAG 4-WIRE FUNCTION

#define PE4_INPUT_ENABLE	1 //JTAG 4-WIRE FUNCTION
#define PE5_INPUT_ENABLE	1 //JTAG 4-WIRE FUNCTION
#define PE6_INPUT_ENABLE	1 //JTAG 4-WIRE FUNCTION
#define PE7_INPUT_ENABLE	1 //JTAG 4-WIRE FUNCTION

#define PULL_WAKEUP_SRC_PE4	GPIO_PIN_PULLUP_10K //JTAG 4-WIRE FUNCTION
#define PULL_WAKEUP_SRC_PE5	GPIO_PIN_PULLUP_10K //JTAG 4-WIRE FUNCTION
#define PULL_WAKEUP_SRC_PE6	GPIO_PIN_PULLUP_10K //JTAG 4-WIRE FUNCTION
#define PULL_WAKEUP_SRC_PE7	GPIO_PIN_PULLDOWN_100K //JTAG 4-WIRE FUNCTION
#endif


#define ADC_ENABLE		0
#if ADC_ENABLE
#define ADC_MODE				ADC_GPIO_MODE
#define ADC_CHNM_ANA_INPUT 		ADC_GPIO_PB3 // one of adc_input_pin_def_e
#define ADC_PRESCALER			ADC_PRESCALE_1F4
#endif

#define ONLINE_STATUS_EN        0

#if (DUAL_MESH_ZB_BL_EN || DUAL_MESH_SIG_PVT_EN)
#define DUAL_MODE_ADAPT_EN 			1   // dual mode as master with Zigbee
#else
#define DUAL_MODE_ADAPT_EN 			0   // dual mode as master with Zigbee
#endif
#if (0 == DUAL_MODE_ADAPT_EN)
#define DUAL_MODE_WITH_TLK_MESH_EN  0   // dual mode as slave with Telink mesh
#endif

#define SPEECH_ENABLE				0

/////////////////// mesh project config /////////////////////////////////
#ifndef TRANSITION_TIME_DEFAULT_VAL
#if (MESH_RX_TEST || (!MD_DEF_TRANSIT_TIME_EN) || SPEECH_ENABLE)
#define TRANSITION_TIME_DEFAULT_VAL (0)
#else
	#if MI_API_ENABLE
#define TRANSITION_TIME_DEFAULT_VAL	0
	#elif LPN_CONTROL_EN
#define TRANSITION_TIME_DEFAULT_VAL	0
	#else
#define TRANSITION_TIME_DEFAULT_VAL (GET_TRANSITION_TIME_WITH_STEP(1, TRANSITION_STEP_RES_1S)) // (0x41)  // 0x41: 1 second // 0x00: means no default transition time
	#endif
#endif
#endif

#if MI_API_ENABLE
#define MESH_DLE_MODE               MESH_DLE_MODE_GATT
#define DLE_LEN_MAX_RX              (136)
#define DLE_LEN_MAX_TX              (40)
#elif GATT_LPN_EN
#define MESH_DLE_MODE               MESH_DLE_MODE_EXTEND_BEAR
#define DLE_LEN_MAX_RX              (MAX_OCTETS_DATA_LEN_EXTENSION)
#define DLE_LEN_MAX_TX              (80)
#elif DEBUG_CFG_CMD_GROUP_AK_EN
#define MESH_DLE_MODE               0   // because not enough RAM
#elif EXTENDED_ADV_ENABLE
#define MESH_DLE_MODE               MESH_DLE_MODE_EXTEND_BEAR
#define DLE_LEN_MAX_RX              (MAX_OCTETS_DATA_LEN_EXTENSION) // must MAX_OCTETS_DATA_LEN_EXTENSION
#define DLE_LEN_MAX_TX              (40)
#else
#define MESH_DLE_MODE               MESH_DLE_MODE_GATT
#define DLE_LEN_MAX_RX              (56)
#define DLE_LEN_MAX_TX              (40)
#endif

/////////////////// MODULE /////////////////////////////////
#if (MI_SWITCH_LPN_EN || GATT_LPN_EN||DU_LPN_EN)
#define BLE_REMOTE_PM_ENABLE			1
#else
#define BLE_REMOTE_PM_ENABLE			0
#endif
#if BLE_REMOTE_PM_ENABLE
#define PM_DEEPSLEEP_RETENTION_ENABLE   1   // must
#else
#define PM_DEEPSLEEP_RETENTION_ENABLE   0
#endif
#define BLE_REMOTE_SECURITY_ENABLE      0
#define BLE_IR_ENABLE					0
#define BLE_SIG_MESH_CERTIFY_ENABLE 	0

#if GATT_LPN_EN
#define BLT_SOFTWARE_TIMER_ENABLE		1
#endif

#ifndef BLT_SOFTWARE_TIMER_ENABLE
#define BLT_SOFTWARE_TIMER_ENABLE		0
#endif

//////////////////////////// KEYSCAN/MIC  GPIO //////////////////////////////////
#define	MATRIX_ROW_PULL					PM_PIN_PULLDOWN_100K
#define	MATRIX_COL_PULL					PM_PIN_PULLUP_10K

#define	KB_LINE_HIGH_VALID				0   //dirve pin output 0 when keyscan, scanpin read 0 is valid
#define DEEPBACK_FAST_KEYSCAN_ENABLE	1   //proc fast scan when deepsleep back trigged by key press, in case key loss
#define KEYSCAN_IRQ_TRIGGER_MODE		0
#define LONG_PRESS_KEY_POWER_OPTIMIZE	1   //lower power when pressing key without release

//stuck key
#define STUCK_KEY_PROCESS_ENABLE		0
#define STUCK_KEY_ENTERDEEP_TIME		60  //in s

//repeat key
#define KB_REPEAT_KEY_ENABLE			0
#define	KB_REPEAT_KEY_INTERVAL_MS		200
#define KB_REPEAT_KEY_NUM				1
//

//----------------------- GPIO for UI --------------------------------
#if ((0 == PM_DEEPSLEEP_RETENTION_ENABLE) && DUAL_VENDOR_EN)
#define SLEEP_FUNCTION_DISABLE          1
#endif

//---------------  Button 
#if (PCBA_B91_SEL == PCBA_ADK80D_C1T213A20_V13)
// button
#define PULL_WAKEUP_SRC_PC2     PM_PIN_PULLUP_1M	//btn
#define PULL_WAKEUP_SRC_PC0     PM_PIN_PULLUP_1M	//btn
#define PC2_INPUT_ENABLE		1
#define PC0_INPUT_ENABLE		1
#define	SW1_GPIO				GPIO_PC0			// PCB mark SW4 // TL_Key2 
#define	SW2_GPIO				GPIO_PC2			// PCB mark SW2 // TL_Key1

#define PULL_WAKEUP_SRC_PC3     PM_PIN_PULLDOWN_100K	//btn	// TL_Key3 // change key matrix to button
#define PULL_WAKEUP_SRC_PC1     PM_PIN_UP_DOWN_FLOAT	//btn	// TL_Key4 // change key matrix to button

#if	IRQ_GPIO_ENABLE
#define IRQ_GPIO_SELECT			GPIO_PD2
#endif

#elif (PCBA_B91_SEL == PCBA_C1T213A16_V13_PA_DONGLE)
#define PULL_WAKEUP_SRC_PB2     PM_PIN_PULLUP_1M	//btn
#define PULL_WAKEUP_SRC_PB1     PM_PIN_PULLUP_1M	//btn
#define PB2_INPUT_ENABLE		1
#define PB1_INPUT_ENABLE		1
#define	SW1_GPIO				GPIO_PB1			// PCB mark SW1
#define	SW2_GPIO				GPIO_PB2			// PCB mark SW2
#elif (PCBA_B91_SEL == PCBA_C1T213A3_V11_DONGLE)
#define PULL_WAKEUP_SRC_PB2     PM_PIN_PULLUP_1M	//btn
#define PULL_WAKEUP_SRC_PB3     PM_PIN_PULLUP_1M	//btn
#define PB2_INPUT_ENABLE		1
#define PB3_INPUT_ENABLE		1
#define	SW1_GPIO				GPIO_PB3			// PCB mark SW7
#define	SW2_GPIO				GPIO_PB2			// PCB mark SW2
#elif (PCBA_B91_SEL == PCBA_C1T216A20_V1_2)
#define PULL_WAKEUP_SRC_PC2     PM_PIN_PULLUP_1M	//btn
#define PULL_WAKEUP_SRC_PC0     PM_PIN_PULLUP_1M	//btn
#define PC2_INPUT_ENABLE		1
#define PC0_INPUT_ENABLE		1
#define	SW1_GPIO				GPIO_PC0			// PCB mark SW4 // TL_Key2 
#define	SW2_GPIO				GPIO_PC2			// PCB mark SW2 // TL_Key1
#define PULL_WAKEUP_SRC_PC3     PM_PIN_PULLDOWN_100K	//btn	// TL_Key3 // change key matrix to button
#define PULL_WAKEUP_SRC_PC1     PM_PIN_UP_DOWN_FLOAT	//btn	// TL_Key4 // change key matrix to button
#endif

#define XIAOMI_MODULE_ENABLE	MI_API_ENABLE
#define XIAOMI_TEST_CODE_ENABLE 	0

//---------------  LED / PWM
#if(PCBA_B91_SEL == PCBA_ADK80D_C1T213A20_V13)
#define PWM_R       PWM_PWM2_PB7	//red
#define PWM_G       PWM_PWM1_PB5	//green
#define PWM_B       PWM_PWM0_PB4	//blue
#define PWM_W       PWM_PWM5_PB0	//white // GPIO_PB6 is not PWM IO // please connect TL_PB0 to LED_W for J34 from C1T213A20_V1_3.pdf.
#elif(PCBA_B91_SEL == PCBA_C1T213A16_V13_PA_DONGLE)
#define PWM_R       PWM_PWM0_PB4	//red
#define PWM_G       PWM_PWM5_PB0	//green
#define PWM_B       PWM_PWM2_PB7	//blue
#define PWM_W       PWM_PWM1_PB5	//white
#elif(PCBA_B91_SEL == PCBA_C1T213A3_V11_DONGLE)
#define PWM_R       PWM_PWM0_PB4	//red
#define PWM_G       PWM_PWM5_PB0	//green
#define PWM_B       PWM_PWM2_PB7	//blue
#define PWM_W       PWM_PWM1_PB5	//white
#elif(PCBA_B91_SEL == PCBA_C1T216A20_V1_2)
#define PWM_R       PWM_PWM3_PB1	//red
#define PWM_G       PWM_PWM1_PB5	//green
#define PWM_B       PWM_PWM5_PB0	//blue
#define PWM_W       GPIO_PB6		//white // TODO: PB6 is not PWM.
#endif

#define PWM_FUNC_R  0  // no use, just for compile
#define PWM_FUNC_G  0  // no use, just for compile
#define PWM_FUNC_B  0  // no use, just for compile
#define PWM_FUNC_W  0  // no use, just for compile

#define PWMID_R     get_pwmid(PWM_R)
#define PWMID_G     get_pwmid(PWM_G)
#define PWMID_B     get_pwmid(PWM_B)
#define PWMID_W     get_pwmid(PWM_W)
                    
#define PWM_INV_R   get_pwm_invert_val(PWM_R)
#define PWM_INV_G   get_pwm_invert_val(PWM_G)
#define PWM_INV_B   get_pwm_invert_val(PWM_B)
#define PWM_INV_W   get_pwm_invert_val(PWM_W)

#ifndef GPIO_LED
#define GPIO_LED	PWM_R
#endif

/////////////open SWS digital pullup to prevent MCU err, this is must ////////////
//#define PA7_DATA_OUT			1 // TODO, no need for B91 ?


/////////////////// Clock  /////////////////////////////////

#if AUDIO_MESH_EN
#define CLOCK_SYS_CLOCK_HZ  	96000000
#elif DUAL_MESH_ZB_BL_EN // keep same with zb
#define CLOCK_SYS_CLOCK_HZ  	32000000
#elif (MI_API_ENABLE)
#define CLOCK_SYS_CLOCK_HZ  	48000000
#elif SPEECH_ENABLE
#define CLOCK_SYS_CLOCK_HZ  	48000000
#else
#define CLOCK_SYS_CLOCK_HZ  	48000000
#endif
#define     PWM_PCLK_SPEED				12000000 //pwm clock 12M.
enum{
	CLOCK_PWM_CLOCK_1S = PWM_PCLK_SPEED,
	CLOCK_PWM_CLOCK_1MS = (CLOCK_PWM_CLOCK_1S / 1000),
	CLOCK_PWM_CLOCK_1US = (CLOCK_PWM_CLOCK_1S / 1000000),
};


/////////////////// watchdog  //////////////////////////////
#define MODULE_WATCHDOG_ENABLE		1
#if (MI_API_ENABLE)
#define WATCHDOG_INIT_TIMEOUT		20000  //in mi mode the watchdog timeout is 20s
#else
#define WATCHDOG_INIT_TIMEOUT		2000  //in mi mode the watchdog timeout is 20s
#endif

#if MI_API_ENABLE && (MSC_TYPE != MSC_NONE)
/////////////////// MSC //////////////////////////////////
#define PULL_WAKEUP_SRC_PB4             PM_PIN_PULLUP_10K
#define GPIO_MSC_RESET                  GPIO_PB4
#define PB4_FUNC                        AS_GPIO

#define PULL_WAKEUP_SRC_PC2             PM_PIN_PULLUP_10K
#define GPIO_MSC_SDA                    GPIO_PC2

#define PULL_WAKEUP_SRC_PC3             PM_PIN_PULLUP_10K
#define GPIO_MSC_SCL                    GPIO_PC3    

//i2c clock = system_clock/(4*DivClock)
#define I2C_CLOCK_100K                  (CLOCK_SYS_CLOCK_HZ/(4*100000))
#define I2C_CLOCK_400K                  (CLOCK_SYS_CLOCK_HZ/(4*400000))
#define I2C_MSC_FREQ                    I2C_CLOCK_400K 
#endif

#if(DEBUG_GPIO_ENABLE)


	#define GPIO_CHN0							GPIO_PE1
	#define GPIO_CHN1							GPIO_PE2
//	#define GPIO_CHN2							GPIO_PA0
    #define GPIO_CHN3							GPIO_PA4
	#define GPIO_CHN4							GPIO_PA3
	#define GPIO_CHN5							GPIO_PB0
//	#define GPIO_CHN6							GPIO_PB2
	#define GPIO_CHN7							GPIO_PE0

	#define GPIO_CHN8							GPIO_PA2
	#define GPIO_CHN9							GPIO_PA1
	#define GPIO_CHN10							GPIO_PB1
//	#define GPIO_CHN11							GPIO_PB3



	#define PE1_OUTPUT_ENABLE					1
	#define PE2_OUTPUT_ENABLE					1
//	#define PA0_OUTPUT_ENABLE					1
	#define PA4_OUTPUT_ENABLE					1
	#define PA3_OUTPUT_ENABLE					1
	#define PB0_OUTPUT_ENABLE					1
//	#define PB2_OUTPUT_ENABLE					1
	#define PE0_OUTPUT_ENABLE					1

	#define PA2_OUTPUT_ENABLE					1
	#define PA1_OUTPUT_ENABLE					1
	#define PB1_OUTPUT_ENABLE					1
//	#define PB3_OUTPUT_ENABLE					1


	#define DBG_CHN0_LOW		gpio_write(GPIO_CHN0, 0)
	#define DBG_CHN0_HIGH		gpio_write(GPIO_CHN0, 1)
	#define DBG_CHN0_TOGGLE		gpio_toggle(GPIO_CHN0)
	#define DBG_CHN1_LOW		gpio_write(GPIO_CHN1, 0)
	#define DBG_CHN1_HIGH		gpio_write(GPIO_CHN1, 1)
	#define DBG_CHN1_TOGGLE		gpio_toggle(GPIO_CHN1)
	#define DBG_CHN2_LOW		//gpio_write(GPIO_CHN2, 0)
	#define DBG_CHN2_HIGH		//gpio_write(GPIO_CHN2, 1)
	#define DBG_CHN2_TOGGLE		//gpio_toggle(GPIO_CHN2)
	#define DBG_CHN3_LOW		gpio_write(GPIO_CHN3, 0)
	#define DBG_CHN3_HIGH		gpio_write(GPIO_CHN3, 1)
	#define DBG_CHN3_TOGGLE		gpio_toggle(GPIO_CHN3)
	#define DBG_CHN4_LOW		gpio_write(GPIO_CHN4, 0)
	#define DBG_CHN4_HIGH		gpio_write(GPIO_CHN4, 1)
	#define DBG_CHN4_TOGGLE		gpio_toggle(GPIO_CHN4)
	#define DBG_CHN5_LOW		gpio_write(GPIO_CHN5, 0)
	#define DBG_CHN5_HIGH		gpio_write(GPIO_CHN5, 1)
	#define DBG_CHN5_TOGGLE		gpio_toggle(GPIO_CHN5)
	#define DBG_CHN6_LOW		//gpio_write(GPIO_CHN6, 0)
	#define DBG_CHN6_HIGH		//gpio_write(GPIO_CHN6, 1)
	#define DBG_CHN6_TOGGLE		//gpio_toggle(GPIO_CHN6)
	#define DBG_CHN7_LOW		gpio_write(GPIO_CHN7, 0)
	#define DBG_CHN7_HIGH		gpio_write(GPIO_CHN7, 1)
	#define DBG_CHN7_TOGGLE		gpio_toggle(GPIO_CHN7)
	#define DBG_CHN8_LOW		gpio_write(GPIO_CHN8, 0)
	#define DBG_CHN8_HIGH		gpio_write(GPIO_CHN8, 1)
	#define DBG_CHN8_TOGGLE		gpio_toggle(GPIO_CHN8)
	#define DBG_CHN9_LOW		gpio_write(GPIO_CHN9, 0)
	#define DBG_CHN9_HIGH		gpio_write(GPIO_CHN9, 1)
	#define DBG_CHN9_TOGGLE		gpio_toggle(GPIO_CHN9)
	#define DBG_CHN10_LOW		gpio_write(GPIO_CHN10, 0)
	#define DBG_CHN10_HIGH		gpio_write(GPIO_CHN10, 1)
	#define DBG_CHN10_TOGGLE	gpio_toggle(GPIO_CHN10)
	#define DBG_CHN11_LOW		//gpio_write(GPIO_CHN11, 0)
	#define DBG_CHN11_HIGH		//gpio_write(GPIO_CHN11, 1)
	#define DBG_CHN11_TOGGLE	//gpio_toggle(GPIO_CHN11)
#else
	#define DBG_CHN0_LOW
	#define DBG_CHN0_HIGH
	#define DBG_CHN0_TOGGLE
	#define DBG_CHN1_LOW
	#define DBG_CHN1_HIGH
	#define DBG_CHN1_TOGGLE
	#define DBG_CHN2_LOW
	#define DBG_CHN2_HIGH
	#define DBG_CHN2_TOGGLE
	#define DBG_CHN3_LOW
	#define DBG_CHN3_HIGH
	#define DBG_CHN3_TOGGLE
	#define DBG_CHN4_LOW
	#define DBG_CHN4_HIGH
	#define DBG_CHN4_TOGGLE
	#define DBG_CHN5_LOW
	#define DBG_CHN5_HIGH
	#define DBG_CHN5_TOGGLE
	#define DBG_CHN6_LOW
	#define DBG_CHN6_HIGH
	#define DBG_CHN6_TOGGLE
	#define DBG_CHN7_LOW
	#define DBG_CHN7_HIGH
	#define DBG_CHN7_TOGGLE
	#define DBG_CHN8_LOW
	#define DBG_CHN8_HIGH
	#define DBG_CHN8_TOGGLE
	#define DBG_CHN9_LOW
	#define DBG_CHN9_HIGH
	#define DBG_CHN9_TOGGLE
	#define DBG_CHN10_LOW
	#define DBG_CHN10_HIGH
	#define DBG_CHN10_TOGGLE
	#define DBG_CHN11_LOW
	#define DBG_CHN11_HIGH
	#define DBG_CHN11_TOGGLE
#endif  //end of DEBUG_GPIO_ENABLE

#if MI_API_ENABLE
#include "mi_config.h"
#endif
/////////////////// set default   ////////////////

#include "../common/default_config.h"

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
