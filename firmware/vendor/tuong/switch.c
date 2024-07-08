/*
 * switch.c
 *
 *  Created on: May 7, 2024
 *      Author: DELL
 */

#include "switch.h"

u8 checkPress = 0 ;

void Pin_SW_Conf(void){
	gpio_set_func(SW1, AS_GPIO);
	gpio_input_en(SW1);
	gpio_set_up_down_res(SW1,GPIO_PIN_PULLUP_10K);

	gpio_set_func(SW2, AS_GPIO);
	gpio_input_en(SW2);
	gpio_set_up_down_res(SW2,GPIO_PIN_PULLUP_10K);

	gpio_set_func(SW3, AS_GPIO);
	gpio_input_en(SW3);
	gpio_set_up_down_res(SW3,GPIO_PIN_PULLUP_10K);

	gpio_set_func(SW4, AS_GPIO);
	gpio_input_en(SW4);
	gpio_set_up_down_res(SW4,GPIO_PIN_PULLUP_10K);
}

char ReadSW(void){

	if(gpio_read(SW1)==0 && gpio_read(SW2) == 0){
		checkPress = 5;
		//uart_send_byte(UART0, '5');
		return '5';
	}

	if(!gpio_read(SW1)){
		checkPress = 1;

	}else if(!gpio_read(SW2)){
		checkPress = 2;

	}else if(!gpio_read(SW3)){
		checkPress = 3;

	}else if(!gpio_read(SW4)){
		checkPress = 4;

	}

	//wd_clear();

	if(gpio_read(SW1) == 1 && checkPress == 1){
		checkPress = 0;
		return '1';
	}else if(gpio_read(SW2) == 1 && checkPress == 2){
		checkPress = 0;
		return '2';
	}else if(gpio_read(SW3) == 1 && checkPress == 3){
		checkPress = 0;
		return '3';
	}else if(gpio_read(SW4) == 1 && checkPress == 4){
		checkPress = 0;
		return '4';
	}

	return SW_NOT_PRESS;
}


