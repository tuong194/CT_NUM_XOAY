/********************************************************************************************************
 * @file	main.c
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
#include "tl_common.h"
#include "proj/mcu/watchdog_i.h"
#include "vendor/common/user_config.h"
#include "drivers.h"
#include "proj_lib/ble/blt_config.h"
#include "proj_lib/ble/ll/ll.h"
#include "proj_lib/sig_mesh/app_mesh.h"

#include "../lvgl/lvgl.h"
#include "../tuong/LCD_128.h"
#include "../tuong/LCD_lvgl.h"
#include "../lvgl/demos/benchmark/lv_demo_benchmark.h"
#include "../lvgl/examples/lv_examples.h"
#include "../UI/ui.h"

#include "../tuong/my_Function.h"
#include "../tuong/switch.h"
#include "../tuong/LED_LC8823.h"


unsigned short div; unsigned char bwpc;

 _Bool ec1;
 _Bool ec2;
static _Bool ec1_temp = 0, ec2_temp = 0;

int lastEncoded;
int encoder_value1;
int encoder_value2;
int sub_encoder = 0;
u8 check_encoder_r_l;

u8 buff_test[8];

extern void user_init();
extern void main_loop ();
void blc_pm_select_none();


#if (HCI_ACCESS==HCI_USE_UART)
#include "drivers.h"
extern my_fifo_t hci_rx_fifo;

#if __TLSR_RISCV_EN__
_attribute_ram_code_ void irq_uart_handle()
{
    irq_uart_handle_fifo();
}
#else // b85m
u16 uart_tx_irq=0, uart_rx_irq=0;

_attribute_ram_code_ void irq_uart_handle()
{
	unsigned char irqS = reg_dma_rx_rdy0;
	if(irqS & FLD_DMA_CHN_UART_RX)	//rx
	{
		uart_rx_irq++;
		reg_dma_rx_rdy0 = FLD_DMA_CHN_UART_RX;
		u8* w = hci_rx_fifo.p + (hci_rx_fifo.wptr & (hci_rx_fifo.num-1)) * hci_rx_fifo.size;
		if(w[0]!=0)
		{
			my_fifo_next(&hci_rx_fifo);
			u8* p = hci_rx_fifo.p + (hci_rx_fifo.wptr & (hci_rx_fifo.num-1)) * hci_rx_fifo.size;
			reg_dma0_addr = (u16)((u32)p);
		}
	}

	if(irqS & FLD_DMA_CHN_UART_TX)	//tx
	{
		uart_tx_irq++;
		reg_dma_rx_rdy0 = FLD_DMA_CHN_UART_TX;
	}
}
#endif
#endif

#if IRQ_TIMER1_ENABLE
	#if __TLSR_RISCV_EN__
int timer1_irq_cnt = 0;
_attribute_ram_code_sec_ void timer1_irq_handler(void)
{
	if(timer_get_irq_status(TMR_STA_TMR1))
	{
		lv_tick_inc(1);
		timer_clr_irq_status(TMR_STA_TMR1);
		timer1_irq_cnt++;
		//gpio_write(GPIO_PA1, timer1_irq_cnt%2);
	}
}
	#else
_attribute_ram_code_ void irq_timer_handle()
{
    u32 src = reg_irq_src;
    static u32 A_debug_irq_cnt =0;
    if(src & FLD_IRQ_TMR1_EN){
       A_debug_irq_cnt++;
       reg_tmr_sta = FLD_TMR_STA_TMR1;
       gpio_write(GPIO_PA1,A_debug_irq_cnt%2);
    }
}
	#endif
#endif

#if	IRQ_GPIO_ENABLE
#if __TLSR_RISCV_EN__
volatile int gpio_irq_cnt = 0, gpio_irq_risc0_cnt = 0, gpio_irq_risc1_cnt = 0;
_attribute_ram_code_sec_noinline_ void gpio_irq_handler(void)
{
	gpio_irq_cnt++;
	gpio_clr_irq_status(FLD_GPIO_IRQ_CLR);
}

_attribute_ram_code_sec_noinline_ void gpio_risc0_irq_handler(void)
{
	if(ec1 != gpio_read(ENCODER1)){
		if(gpio_read(ENCODER2) == 0){
			encoder_value1 --;
			if(encoder_value1 <= 0 || encoder_value1 >= 100){
				encoder_value1 = 0;
			}
			check_encoder_r_l = 1;

		}else if(gpio_read(ENCODER2) == 1){
			encoder_value1 ++;
			if(encoder_value1 >= 100) encoder_value1 = 100;
			check_encoder_r_l = 2;
		}
	}


	gpio_irq_risc0_cnt++;
	gpio_clr_irq_status(FLD_GPIO_IRQ_GPIO2RISC0_CLR);
}

_attribute_ram_code_sec_noinline_ void gpio_risc1_irq_handler(void)
{

	if(ec2 != gpio_read(ENCODER2)){
		if(gpio_read(ENCODER1) == 0){
			encoder_value1 --;
			if(encoder_value1 <= 0 || encoder_value1 >= 100){
				encoder_value1 = 0;
			}
			check_encoder_r_l = 1;

		}else if(gpio_read(ENCODER1) == 1){
			encoder_value1 ++;
			if(encoder_value1 >= 100) encoder_value1 = 100;
			check_encoder_r_l = 2;
		}
	}


	gpio_irq_risc1_cnt++;
	gpio_clr_irq_status(FLD_GPIO_IRQ_GPIO2RISC1_CLR);
}
#else
void irq_gpio_handle()
{
	u32 src = reg_irq_src;
	if(src & FLD_IRQ_GPIO_EN){
		gpio_irq_user_handle();
		reg_irq_src = FLD_IRQ_GPIO_EN;        // clear irq_gpio irq flag		
	}

	/************* gpio irq risc0 *************/
	if(src & FLD_IRQ_GPIO_RISC0_EN){
		gpio_risc0_user_handle();
		reg_irq_src = FLD_IRQ_GPIO_RISC0_EN;        // clear irq_gpio irq flag				
	}

	/************* gpio irq risc1 *************/
	if(src & FLD_IRQ_GPIO_RISC1_EN){
		gpio_risc1_user_handle();
		reg_irq_src = FLD_IRQ_GPIO_RISC1_EN;        // clear irq_gpio irq flag		
	}
	#if (!(__TL_LIB_8258__ || (MCU_CORE_TYPE && MCU_CORE_TYPE == MCU_CORE_8258) || (MCU_CORE_TYPE == MCU_CORE_8278)))
	if(src & FLD_IRQ_GPIO_RISC2_EN){
		gpio_risc2_user_handle();
		reg_irq_src = FLD_IRQ_GPIO_RISC2_EN;
	}
	#endif
}
#endif
#endif

_attribute_ram_code_ void irq_handler(void)
{
	#if ACTIVE_SCAN_ENABLE
	rp_active_scan_req_proc();
	#endif
	#if DUAL_MESH_ZB_BL_EN
	if(rf_mode == RF_MODE_ZIGBEE){
		irq_zigbee_sdk_handler();
	}else
	#endif
	{
		irq_blt_sdk_handler ();  //ble irq proc
	}

#if (IRQ_TIMER1_ENABLE && !__TLSR_RISCV_EN__)
	irq_timer_handle();
#endif

}

/**
 * @brief		BLE SDK RF interrupt handler.
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_
void rf_irq_handler(void)
{

	DBG_CHN10_HIGH;

	irq_handler();
	DBG_CHN10_LOW;

}

_attribute_ram_code_
void stimer_irq_handler(void)
{
	DBG_CHN9_HIGH;

	irq_handler();

	DBG_CHN9_LOW;
}

_attribute_ram_code_
void uart0_irq_handler(void)
{
#if (HCI_ACCESS==HCI_USE_UART)
	if(IRQ19_UART0 == UART_IRQ_NUM){
		irq_uart_handle();
	}
#endif
}

_attribute_ram_code_
void uart1_irq_handler(void)
{
#if (HCI_ACCESS==HCI_USE_UART)
	if(IRQ18_UART1 == UART_IRQ_NUM){
		irq_uart_handle();
	}
#endif
}


FLASH_ADDRESS_DEFINE;
#if(MCU_CORE_TYPE == MCU_CORE_8269)
int main (void) {
	FLASH_ADDRESS_CONFIG;
	cpu_wakeup_init();

	clock_init();
	set_tick_per_us(CLOCK_SYS_CLOCK_HZ/1000000);

	gpio_init();

	rf_drv_init(CRYSTAL_TYPE);

	user_init ();

    irq_enable();

	while (1) {
#if (MODULE_WATCHDOG_ENABLE)
		wd_clear(); //clear watch dog
#endif
		main_loop ();
	}
}
#elif((MCU_CORE_TYPE == MCU_CORE_8258) || (MCU_CORE_TYPE == MCU_CORE_8278)||(MCU_CORE_TYPE == MCU_CORE_9518))
_attribute_ram_code_ int main (void)    //must run in ramcode
{
	FLASH_ADDRESS_CONFIG;
#if (PINGPONG_OTA_DISABLE && (0 == FW_START_BY_BOOTLOADER_EN))
    ota_fw_check_over_write();  // must at first for main_
#endif

#if SLEEP_FUNCTION_DISABLE
    blc_pm_select_none();
#else
	blc_pm_select_internal_32k_crystal();
#endif
#if(MCU_CORE_TYPE == MCU_CORE_8258)
	cpu_wakeup_init();
#elif(MCU_CORE_TYPE == MCU_CORE_8278)
	cpu_wakeup_init(LDO_MODE,EXTERNAL_XTAL_24M);
#elif(MCU_CORE_TYPE == MCU_CORE_9518)
	sys_init(LDO_1P4_LDO_1P8,VBAT_MAX_VALUE_GREATER_THAN_3V6); // need to confirm if want to use DCDC.
#endif

	/* detect if MCU is wake_up from deep retention mode */
	int deepRetWakeUp = pm_is_MCU_deepRetentionWakeup();  //MCU deep retention wakeUp

#if (MCU_CORE_TYPE == MCU_CORE_9518)
	clock_init_B91();
	#if (MODULE_WATCHDOG_ENABLE)
	wd_set_interval_ms(WATCHDOG_INIT_TIMEOUT);
	wd_start();
	#endif
	rf_drv_ble_init();
	gpio_init(!deepRetWakeUp);
	if(!deepRetWakeUp){//read flash size

		//blc_readFlashSize_autoConfigCustomFlashSector(); called in global init.

		#if (FLASH_FIRMWARE_CHECK_ENABLE)
			blt_firmware_completeness_check();
		#endif

		#if FIRMWARES_SIGNATURE_ENABLE
			blt_firmware_signature_check();
		#endif
	}

	/* load customized freq_offset cap value. */
	blc_app_loadCustomizedParameters();
#else
	rf_drv_init(RF_MODE_BLE_1M);
	gpio_init( !deepRetWakeUp );  //analog resistance will keep available in deepSleep mode, so no need initialize again
    clock_init(SYS_CLK_CRYSTAL);
#endif


#if	(PM_DEEPSLEEP_RETENTION_ENABLE)
	if( deepRetWakeUp ){ // MCU wake_up from deepSleep retention mode
		user_init_deepRetn ();
	}
	else // MCU power_on or wake_up from deepSleep mode
#endif
	{
		user_init();

		SPI_Config();
		PWM_Confing();


		uart_cal_div_and_bwpc(115200,sys_clk.pclk*1000000, &div, &bwpc);
		uart_init(UART0,div,bwpc,UART_PARITY_NONE,UART_PARITY_NONE);
		uart_set_pin(UART0_TX_PD2,UART0_RX_PD3);
//		uart_init(UART0,12,15,UART_PARITY_NONE,UART_STOP_BIT_ONE); // sysclock = 24M
//		uart_set_pin(UART0_TX_PD2,UART0_RX_PD3);



		gpio_set_func(ENCODER1,AS_GPIO);
		gpio_input_en(ENCODER1);
//		gpio_set_up_down_res(ENCODER1,GPIO_PIN_UP_DOWN_FLOAT);
		gpio_set_interrupt_init(ENCODER1,GPIO_PIN_UP_DOWN_FLOAT, INTR_RISING_EDGE, IRQ26_GPIO2RISC0 );


		gpio_set_func(ENCODER2,AS_GPIO);
		gpio_input_en(ENCODER2);
//		gpio_set_up_down_res(ENCODER2,GPIO_PIN_UP_DOWN_FLOAT);
		gpio_set_interrupt_init(ENCODER2,GPIO_PIN_UP_DOWN_FLOAT, INTR_FALLING_EDGE, IRQ27_GPIO2RISC1 );


		lv_init();
		lv_port_disp_init();
		ui_init();

		blc_pm_select_external_32k_crystal();

		Config_Pin_Led_Lc8823();
		Pin_SW_Conf();
		setValue();
		getValue();
		Setting_stt_led();

		Blink_20_Led_Green();
		Blink_20_Led_Blue();
		Blink_20_Led_Red();
		On_Off_Led_SW(ON_LED_SW1,ON_LED_SW2,ON_LED_SW3,ON_LED_SW4);

		ec1 = gpio_read(ENCODER1);
		ec2 = gpio_read(ENCODER2);

		encoder_value1=encoder_value2=10;

	}

    irq_enable();
	#if (DEBUG_LOG_SETTING_DEVELOP_MODE_EN || (MESH_USER_DEFINE_MODE == MESH_IRONMAN_MENLO_ENABLE))
	LOG_USER_MSG_INFO(0, 0,"[mesh] Start from SIG Mesh", 0);
	#endif

	while (1) {
#if (MODULE_WATCHDOG_ENABLE)
		wd_clear(); //clear watch dog
#endif
		main_loop ();

		lv_timer_handler();

//		ec1_temp = gpio_read(ENCODER1);
//		ec2_temp = gpio_read(ENCODER2);
//		if(ec1 != ec1_temp)
//		{
//			if(ec1_temp ^ ec2_temp)
//			{
//				//func_led_l();
//				encoder_value1 -- ;
//				if(encoder_value1 <= 0 || encoder_value1 >= 100){
//					encoder_value1 = 0;
//				}
//				sprintf(buff_test, "%d\n",encoder_value1);
//				uart_send(UART0,buff_test,6);
//				check_encoder_r_l = 1;
//
//			} else {
//				//func_led_r();
//				encoder_value1 ++ ;
//				if(encoder_value1 >= 100) encoder_value1 = 100;
//				sprintf(buff_test, "%d\n",encoder_value1);
//				uart_send(UART0,buff_test,6);
//				check_encoder_r_l = 2;
//			}
//			//ec1 = gpio_read(ENCODER1);
//		}


		check_OTA();
		check_provision();
		displayClock();
		check_Scene();
		read_sw();
		Encoder_Control();


	}
	return 0;
}
#endif
