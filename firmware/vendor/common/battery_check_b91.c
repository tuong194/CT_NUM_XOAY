/********************************************************************************************************
 * @file     battery_check.c
 *
 * @brief    This is the source file for BLE SDK
 *
 * @author	 BLE GROUP
 * @date         06,2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
 *******************************************************************************************************/

#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"
#include "proj_lib/sig_mesh/app_mesh.h"

#include "battery_check.h"


#if __TLSR_RISCV_EN__

_attribute_data_retention_	u8 		lowBattDet_enable = 1;
_attribute_no_retention_bss_ u8      adc_hw_initialized = 0;   //note: can not be retention variable
_attribute_data_retention_  u16     batt_vol_mv;

#define ADC_SAMPLE_NUM		8

unsigned short adc_dat_buf[ADC_SAMPLE_NUM];  //size must 16 byte aligned(16/32/64...)

#if BATT_CHECK_ENABLE
void battery_set_detect_enable (int en)
{
	lowBattDet_enable = en;

	if(!en){
		adc_hw_initialized = 0;   //need initialized again
	}

}

/**
 * @brief     This function serves to get battery detect status.
 * @param	none
 * @return    other: battery detect enable
 * 			  0: battery detect disable
 */
int battery_get_detect_enable (void)
{
	return lowBattDet_enable;
}

extern unsigned short g_adc_vref;
extern unsigned short g_adc_gpio_calib_vref;
extern unsigned char g_adc_pre_scale;
extern unsigned char g_adc_vbat_divider;
extern signed char g_adc_vref_offset;
extern signed char g_adc_gpio_calib_vref_offset;
/**
 * @brief      this function is used for user to initialize battery detect.
 * @param	   none
 * @return     none
 */
_attribute_ram_code_ void adc_bat_detect_init(void)
{
	g_adc_vref = g_adc_gpio_calib_vref;//set gpio sample calib vref
#if VBAT_CHANNEL_EN//vbat mode, vbat channel
	g_adc_vref_offset = 0;//Vbat has no two-point calibration, offset must be set to 0.
#else
	g_adc_vref_offset = g_adc_gpio_calib_vref_offset;//set adc_vref_offset as adc_gpio_calib_vref_offset
#endif
	/******power off sar adc********/
	adc_power_off();

	//reset whole digital adc module
	adc_reset();

	/******set adc sample clk as 4MHz******/
	adc_clk_en();//enable signal of 24M clock to sar adc
	adc_set_clk(5);//default adc_clk 4M = 24M/(1+div),

	//set misc channel vref 1.2V
	analog_write_reg8(areg_adc_vref, ADC_VREF_1P2V);
	analog_write_reg8(areg_ain_scale  , (analog_read_reg8( areg_ain_scale  )&(0xC0)) | 0x3d );
	g_adc_vref = 1175;

	//set Analog input pre-scaling,ADC_PRESCALE_1F4
	analog_write_reg8( areg_ain_scale  , (analog_read_reg8( areg_ain_scale  )&(~FLD_SEL_AIN_SCALE)) | (ADC_PRESCALE_1F4<<6) );
	g_adc_pre_scale = 1<<(unsigned char)ADC_PRESCALE_1F4;

	//set sample frequency.96k
	adc_set_state_length(240, 10);

	//set misc t_sample 6 cycle of adc clock:  6 * 1/4M
	adc_set_tsample_cycle(ADC_SAMPLE_CYC_6);

	//default adc_resolution set as 14bit ,BIT(13) is sign bit
	adc_set_resolution(ADC_RES14);

	//enable adc channel.
	adc_set_m_chn_en();

	//set vbat divider : ADC_VBAT_DIV_OFF
	analog_write_reg8(areg_adc_vref_vbat_div, (analog_read_reg8(areg_adc_vref_vbat_div)&(~FLD_ADC_VREF_VBAT_DIV)) | (ADC_VBAT_DIV_OFF<<2) );
	g_adc_vbat_divider = 1;

#if VBAT_CHANNEL_EN//vbat mode, vbat channel
	adc_set_diff_input(ADC_VBAT, GND);
#else//base mode, gpio channel
	adc_set_diff_input(ADC_INPUT_PIN_CHN>>12, GND);
#endif
	/******power on sar adc********/
	//note: this setting must be set after all other settings
	adc_power_on();

	//wait at least 2 sample cycle(f = 96K, T = 10.4us)
	sleep_us(25);
}

_attribute_ram_code_ int app_battery_power_check(u16 alram_vol_mv, int loop_flag)
{
	int ret_slept_flag = 0;

	//when MCU powered up or wakeup from deep/deep with retention, adc need be initialized
	if(!adc_hw_initialized){
		adc_hw_initialized = 1;
		adc_bat_detect_init();
	}

	batt_vol_mv = adc_get_voltage();

	if(batt_vol_mv < alram_vol_mv){
		#if 0
		analog_write_reg8(USED_DEEP_ANA_REG,  analog_read_reg8(USED_DEEP_ANA_REG) | LOW_BATT_FLG);  //mark

		u32 pin[] = KB_DRIVE_PINS;
		for (int i=0; i<(sizeof (pin)/sizeof(*pin)); i++)
		{
			cpu_set_gpio_wakeup (pin[i], Level_High, 1);  //drive pin pad high wakeup deepsleep
		}

		cpu_sleep_wakeup(DEEPSLEEP_MODE, PM_WAKEUP_PAD, 0);  //deepsleep
		#else
		LOG_BATTERY_CHECK_DEBUG(0,0,"battery low, loop:%d, current:%d, thres:%d", loop_flag, batt_vol_mv, alram_vol_mv);
        battery_power_low_handle(loop_flag);
        ret_slept_flag = 1;
		#endif
	}

	return ret_slept_flag;
}
#endif
/**
 * @brief This function serves to sort adc sample code and get average value.
 * @return 		adc_code_average 	- the average value of adc sample code.
 */
unsigned short adc_sort_and_get_average_code(void)
{

	unsigned short adc_code_average = 0;
	int i, j;
	unsigned short temp;
	/**** insert Sort and get average value ******/
	for(i = 1 ;i < ADC_SAMPLE_NUM; i++)
	{
		if(adc_dat_buf[i] < adc_dat_buf[i-1])
		{
			temp = adc_dat_buf[i];
			adc_dat_buf[i] = adc_dat_buf[i-1];
	/**
		 * add judgment condition "j>=0" in for loop,
		 * otherwise may have array out of bounds.
		 * changed by chaofan.20201230.
	 */
			for(j=i-1; j>=0 && adc_dat_buf[j] > temp;j--)
			{
				adc_dat_buf[j+1] = adc_dat_buf[j];
			}
			adc_dat_buf[j+1] = temp;
		}
	}

	//get average value from raw data(abandon 1/4 small and 1/4 big data)
	for (i = ADC_SAMPLE_NUM>>2; i < (ADC_SAMPLE_NUM - (ADC_SAMPLE_NUM>>2)); i++)
	{
		adc_code_average += adc_dat_buf[i]/(ADC_SAMPLE_NUM>>1);
	}
	return adc_code_average;
}

/**
 * @brief This function serves to get adc sample code by manual and convert to voltage value.
 * @return 		adc_vol_mv_average 	- the average value of adc voltage value.
 */
unsigned short adc_get_voltage(void)
{
	unsigned short adc_vol_mv_average = 0;
	unsigned short adc_code_average = 0;
	for (int i = 0; i < ADC_SAMPLE_NUM; i++)
	{
	/**
	 * move the "2 sample cycle" wait operation before adc_get_code(),
	 * otherwise may have data lose due to no waiting when adc_power_on.
	 * changed by chaofan.20201230.
	 */
		delay_us(25);//wait at least 2 sample cycle(f = 96K, T = 10.4us)
		adc_dat_buf[i] = adc_get_code();
	}
	adc_code_average = adc_sort_and_get_average_code();
	adc_vol_mv_average = adc_calculate_voltage(adc_code_average);
	return adc_vol_mv_average;
}

#endif
