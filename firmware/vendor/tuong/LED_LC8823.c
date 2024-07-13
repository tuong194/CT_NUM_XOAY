/*
 * LED_LC8823.c
 *
 *  Created on: Jun 18, 2024
 *      Author: DELL
 */

#include"LED_LC8823.h"

extern u8 stt_sw1;
extern u8 stt_sw2;
extern u8 stt_sw3;
extern u8 stt_sw4;


void On_Off_Led_SWxx(on_off_led_sw1 stt_sw1, on_off_led_sw2 stt_sw2, on_off_led_sw3 stt_sw3, on_off_led_sw4 stt_sw4);

extern u8 flag_display;

void Config_Pin_Led_Lc8823(void){
	gpio_set_func(LED_DI, AS_GPIO);
	gpio_output_en(LED_DI);
	gpio_set_func(LED_CI, AS_GPIO);
	gpio_output_en(LED_CI);
}


/****************Frame****************/
void start_frame_led(void){
	gpio_write(LED_DI,0);
	for(int i=0;i<32;i++){
		sleep_us(500000/FREQ_LED);
		gpio_write(LED_CI,0);
		sleep_us(500000/FREQ_LED);
		gpio_write(LED_CI,1);
	}
	wd_clear();
}

void end_frame_led(void){
	for(int i=0;i<32;i++){
		sleep_us(500000/FREQ_LED);
		gpio_write(LED_CI,0);
		gpio_write(LED_DI,1);
		sleep_us(500000/FREQ_LED);
		gpio_write(LED_CI,1);
	}
}

void EN_Frame_Color(void){
	for(int i=0;i<8;i++){
		sleep_us(500000/FREQ_LED);
		gpio_write(LED_CI,0);
		gpio_write(LED_DI,1);
		sleep_us(500000/FREQ_LED);
		gpio_write(LED_CI,1);
	}
	wd_clear();
}

void DIS_Frame_Color(void){
	for(int i=0;i<8;i++){
		sleep_us(500000/FREQ_LED);
		gpio_write(LED_CI,0);
		gpio_write(LED_DI,0);
		sleep_us(500000/FREQ_LED);
		gpio_write(LED_CI,1);
	}
	wd_clear();
}

void Frame_On_Led(void){    // dim 50%
	for(int i=0;i<4;i++){
		sleep_us(500000/FREQ_LED);
		gpio_write(LED_CI,0);
		gpio_write(LED_DI,1);
		sleep_us(500000/FREQ_LED);
		gpio_write(LED_CI,1);
	}
	for(int i=0;i<4;i++){
		sleep_us(500000/FREQ_LED);
		gpio_write(LED_CI,0);
		gpio_write(LED_DI,0);
		sleep_us(500000/FREQ_LED);
		gpio_write(LED_CI,1);
	}
	wd_clear();
}

void Frame_On_Led_20pt(void){    // dim 2/32

	for(int i=0;i<8;i++){
		if(i == 3 || i == 4 || i == 5 || i==7){
			sleep_us(500000/FREQ_LED);
			gpio_write(LED_CI,0);
			gpio_write(LED_DI,0);
			sleep_us(500000/FREQ_LED);
			gpio_write(LED_CI,1);
		}else{
			sleep_us(500000/FREQ_LED);
			gpio_write(LED_CI,0);
			gpio_write(LED_DI,1);
			sleep_us(500000/FREQ_LED);
			gpio_write(LED_CI,1);
		}
	}
	wd_clear();
}
void Frame_Off_Led(void){
	for(int i=0;i<3;i++){
		sleep_us(500000/FREQ_LED);
		gpio_write(LED_CI,0);
		gpio_write(LED_DI,1);
		sleep_us(500000/FREQ_LED);
		gpio_write(LED_CI,1);
	}
	for(int i=0;i<5;i++){
		sleep_us(500000/FREQ_LED);
		gpio_write(LED_CI,0);
		gpio_write(LED_DI,0);
		sleep_us(500000/FREQ_LED);
		gpio_write(LED_CI,1);
	}
	wd_clear();
}



// CONTROL LED
void Off_Led(void){
	Frame_Off_Led();
	DIS_Frame_Color();
	DIS_Frame_Color();
	DIS_Frame_Color();
}


void On_Led(u8 led_num, color_led_rgb color){  // on 1 led (9-20)
	for(u8 i =9; i<led_num;i++){
		Off_Led();
	}
	Frame_On_Led();

	switch(color){
	case COLORx_LC8823:
		EN_Frame_Color();
		EN_Frame_Color();
		DIS_Frame_Color();
		break;
	case YELLOW_LC8823:
		DIS_Frame_Color();
		EN_Frame_Color();
		EN_Frame_Color();
		break;
	default:
		break;
	}
	for(u8 j = led_num+1;j<=20;j++){
		Off_Led();
	}
}

void Off_20_Led(void){
	start_frame_led();
	for(u8 i = 0; i<20;i++){
		Off_Led();
	}
	end_frame_led();
}

void On_20_Led_Blue(void){
	start_frame_led();
	for(u8 i=0; i<20;i++){
		Frame_On_Led();
		EN_Frame_Color();
		DIS_Frame_Color();
		DIS_Frame_Color();
	}
	end_frame_led();
}
void On_20_Led_Green(void){
	start_frame_led();
	for(u8 i=0; i<20;i++){
		Frame_On_Led();
		DIS_Frame_Color();
		EN_Frame_Color();
		DIS_Frame_Color();
	}
	end_frame_led();
}
void On_20_Led_Red(void){
	start_frame_led();
	for(u8 i=0; i<20;i++){
		Frame_On_Led();
		DIS_Frame_Color();
		DIS_Frame_Color();
		EN_Frame_Color();
	}
	end_frame_led();
}

void Blink_20_Led_Red(void){
	On_20_Led_Red();
	sleep_ms(100);
	Off_20_Led();
	sleep_ms(100);
}
void Blink_20_Led_Green(void){
	On_20_Led_Green();
	sleep_ms(100);
	Off_20_Led();
	sleep_ms(100);
}
void Blink_20_Led_Blue(void){
	On_20_Led_Blue();
	sleep_ms(100);
	Off_20_Led();
	sleep_ms(100);
}



void func_led_l(void){
	for(u8 i = 9; i <=20; i++){
		if(!flag_display)	break;
		start_frame_led();
		On_Off_Led_SWxx(stt_sw1, stt_sw2, stt_sw3, stt_sw4);
		//On_Off_Led_SWxx(OFF_LED_SW1, OFF_LED_SW2, OFF_LED_SW3, OFF_LED_SW4);
		On_Led(i,COLORx_LC8823);
		end_frame_led();
	}

	start_frame_led();
	On_Off_Led_SWxx(stt_sw1, stt_sw2, stt_sw3, stt_sw4);
	//On_Off_Led_SWxx(OFF_LED_SW1, OFF_LED_SW2, OFF_LED_SW3, OFF_LED_SW4);
	for(u8 i = 0;i<12;i++){
		Off_Led();
	}
	end_frame_led();

}

void func_led_r(void){
	for(u8 i = 20; i >= 9 ; i--){
		if(!flag_display)	break;

		start_frame_led();
		On_Off_Led_SWxx(stt_sw1, stt_sw2, stt_sw3, stt_sw4);
//		On_Off_Led_SWxx(OFF_LED_SW1, OFF_LED_SW2, OFF_LED_SW3, OFF_LED_SW4);
		On_Led(i,YELLOW_LC8823);
		end_frame_led();
	}

	start_frame_led();
	On_Off_Led_SWxx(stt_sw1, stt_sw2, stt_sw3, stt_sw4);
//	On_Off_Led_SWxx(OFF_LED_SW1, OFF_LED_SW2, OFF_LED_SW3, OFF_LED_SW4);
	for(u8 i = 0;i<12;i++){
		Off_Led();
	}
	end_frame_led();

}




void On_Led_4SW(void){  // green
	start_frame_led();
	for(u8 i =0;i<8;i++){
		Frame_On_Led();
		DIS_Frame_Color();
		EN_Frame_Color();
		DIS_Frame_Color();
	}
	end_frame_led();
}



/***********************************************************/
void on_led_sw(void){
	Frame_On_Led();
	DIS_Frame_Color();
	EN_Frame_Color();
	DIS_Frame_Color();
}

void off_led_sw(void){
	Frame_On_Led_20pt();
	DIS_Frame_Color();
	EN_Frame_Color();
	DIS_Frame_Color();
}

void On_Off_Led_SW(on_off_led_sw1 stt_sw1, on_off_led_sw2 stt_sw2, on_off_led_sw3 stt_sw3, on_off_led_sw4 stt_sw4){
	start_frame_led();
	if(stt_sw1 == ON_LED_SW1){
		on_led_sw();
		on_led_sw();
	}else if(stt_sw1 == OFF_LED_SW1){
		off_led_sw();
		off_led_sw();
	}

	if(stt_sw2 == ON_LED_SW2){
		on_led_sw();
		on_led_sw();
	}else if(stt_sw2 == OFF_LED_SW2){
		off_led_sw();
		off_led_sw();
	}


	if(stt_sw4 == ON_LED_SW4){
		on_led_sw();
		on_led_sw();
	}else if(stt_sw4 == OFF_LED_SW4){
		off_led_sw();
		off_led_sw();
	}

	if(stt_sw3 == ON_LED_SW3){
		on_led_sw();
		on_led_sw();
	}else if(stt_sw3 == OFF_LED_SW3){
		off_led_sw();
		off_led_sw();
	}

	for(u8 i = 9; i<=20; i++){
		Off_Led();
	}
	end_frame_led();
}

void On_Off_Led_SWxx(on_off_led_sw1 stt_sw1, on_off_led_sw2 stt_sw2, on_off_led_sw3 stt_sw3, on_off_led_sw4 stt_sw4){
	if(stt_sw1 == ON_LED_SW1){
		on_led_sw();
		on_led_sw();
	}else if(stt_sw1 == OFF_LED_SW1){
		off_led_sw();
		off_led_sw();
	}

	if(stt_sw2 == ON_LED_SW2){
		on_led_sw();
		on_led_sw();
	}else if(stt_sw2 == OFF_LED_SW2){
		off_led_sw();
		off_led_sw();
	}


	if(stt_sw4 == ON_LED_SW4){
		on_led_sw();
		on_led_sw();
	}else if(stt_sw4 == OFF_LED_SW4){
		off_led_sw();
		off_led_sw();
	}

	if(stt_sw3 == ON_LED_SW3){
		on_led_sw();
		on_led_sw();
	}else if(stt_sw3 == OFF_LED_SW3){
		off_led_sw();
		off_led_sw();
	}
}
