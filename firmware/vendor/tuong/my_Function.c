/*
 * my_Function.c
 *
 *  Created on: May 23, 2024
 *      Author: DELL
 */


#include "my_Function.h"
#include "switch.h"
#include "LED_LC8823.h"

#include "../application/print/u_printf.h"
extern u8 value_start;
extern u8 ui_ota_is_working;

extern u8 buff_test[8];
extern int encoder_value1;

u8 flag_display;



extern u8 get_ota_check_type();
extern int access_cmd_set_light_hsl(u16 adr, u8 rsp_max, u16 lightness, u16 hue, u16 sat, int ack, transition_par_t *trs_par);
extern  _Bool ec1;
extern  _Bool ec2;
extern u8 check_encoder_r_l;

int cnt_print_value;


u8 kick_out_flag; u32 time_out_kick_out;
u8 check_press;

u8 mode=0;
u8 state_led2_blink = 0;
u8 check_reset;

u8 hsvH1,hsvH2;
char swx;
u8 checkSuccess;u8 checkPro;
unsigned char buff[8];





void Setting_stt_led(void){
	transition_par_t trs_par = {0};
	trs_par.transit_t = 10;

	access_cmd_set_lightness(LED_ADDR,2,lum2_lightness(dim_set),0, &trs_par);

	/*if(value_start == 0){
	access_cmd_set_lightness(LED_ADDR,2,0,0, &trs_par);

	}else if(value_start == 1){


		access_cmd_set_lightness(LED_ADDR,2,lum2_lightness(100),0, &trs_par);
	}else if(value_start == 2){

		if(stt_sw4 == 0){
			access_cmd_set_lightness(LED_ADDR,2,lum2_lightness(dim_set),0, &trs_par);
		}else {
			access_cmd_set_lightness(LED_ADDR,2,0,0, &trs_par);
		}
	}*/
}



void getValue(void){
	Read_Data_Flash();


}

void setValue(void){
	nowtime = 0;
	checkScene = 0;
	checkKickout = 0;
	checkOTA = 1;
	countBlinkLed2 = 0;
	checkPro = 1;      // Proved
	checkSuccess = 1;  // provision success
	check_reset = 1;
	kick_out_flag = 0;
}

void Reset_main(void){
	if(check_reset == 1){
		if(checkProvision ==0){
//			Blink_20_Led_Blue();
//			Blink_20_Led_Blue();
//			Blink_20_Led_Blue();
			check_reset = 0;
		}else if(checkProvision == 1){
			check_reset =0;

		}
	}
}

void check_OTA(void){
	if(get_ota_check_type() == OTA_PACKET_INVALID && checkOTA == 1){
		checkScene = 3;

		ui_Screen1_screen_init();
		lv_disp_load_scr(ui_Screen1);

		lv_label_set_text(ui_Label1, "ota is working");
		checkOTA = 0;
	}
}

/*
 * 0: Unprov
 * 1: Provision success
 * 2: Provision unsuccess  -> kick out  5s later
 *    Proved, not messeage -> kick out 30s later
 */
void check_provision(void){
	if(get_provision_state() == STATE_DEV_UNPROV ){
		checkProvision = 0;
		for(u8 i=0; i<8; i++){
			para[i] = 0;
		}
		checkPro = 1;

	}
	if(!checkProvision){
		if(get_provision_state() == STATE_DEV_PROVING){
			//Blink_20_Led_Green();
		}else if(get_provision_state() == STATE_DEV_PROVED){
			if(checkPro == 1){
				//checkPro = 0;
				timeOut = pm_get_32k_tick();
			}
			/**********check Secure************/
					if(pm_get_32k_tick() - timeOut >= 32000*TIMEOUT_SECURE){
						if(para[0] == 0 && para[1] == 0 && para[2] == 0 && para[3] == 0 && para[4] == 0 && para[5] == 0 && para[6] == 0 && para[7] == 0){
							uart_send_byte(UART0, 'N');
							kick_out(0);
						}
					}
					if(checkProvision == 2){ // secure error
						if(pm_get_32k_tick() - timeOut >= 32000*5){
							uart_send_byte(UART0,'2');
							kick_out(0);
						}
					}
		}
	}else if(checkProvision == 1){

		if(checkSuccess == 1){
			uart_send_byte(UART0, '1');
			Write_Data_Flash();
			//uart_send_byte(UART0,'1');
			checkSuccess = 0;
		}
	}
}


void displayClock(void){
	if(pm_get_32k_tick() - nowtime >=32000){

		if(flag_flash){
			Write_Data_Flash();
			flag_flash = 0;
		}

		giay++;
		if(giay == 60){
			phut++;
			giay=0;
			if(phut == 60){
				phut=0;
			}
			if(phut%12 == 0){
				gio++;
			}
			if(gio==60){
				gio=0;
			}
		}
		 nowtime = pm_get_32k_tick();

		 if(!checkKickout){
			 timeKickout=nowtime;
		 }
	}
}

void setRotation(u8 gio, u8 phut, u8 giay){
	lv_img_set_angle(ui_gio, gio*60);
	lv_img_set_angle(ui_phut, phut*60);
	lv_img_set_angle(ui_giay, giay*60);
}

void check_Scene(void){
		if(checkScene == 0){
			//timeScene = pm_get_32k_tick();
			setRotation(gio,phut,giay);
		}
		else if(checkScene == 3){
			if(get_ota_check_type() == OTA_SUCCESS){
				checkOTA = 1;
				checkScene=0;
				ui_Screen2_screen_init();
				lv_disp_load_scr(ui_Screen2);
			}
		}
}
void set_RGB_panel(u16 hsvH){
	lv_img_set_angle(ui_Image3, hsvH*72);  // 7.2 degree = 1
	if(hsvH<=25){
		color = lv_color_hsv_to_rgb(180-hsvH*7.2,100,100); //H,S,V
	}else{
		color = lv_color_hsv_to_rgb(540-hsvH*7.2,100,100);
	}
	lv_obj_set_style_bg_color(ui_Panel2, color, LV_PART_MAIN | LV_STATE_DEFAULT);
}
void Set_RGB(void){
	transition_par_t trs_par = {0};
	trs_par.transit_t = 10;
	u16 hue;
//	if(hsvH<=180){
//		hue = (180-hsvH)*65535/360;
//	}else{
//		hue = (540-hsvH)*65535/360;
//	}
	if(hsvH<=25){
		hue = (25-hsvH)*65535/50;
	}else{
		hue = (75-hsvH)*65535/50;
	}
	u16 lightness = dim_set*32767/100; // 50%
	u16 sat = 0xffff;
	access_cmd_set_light_hsl(LED_ADDR,2,lightness,hue,sat,0,&trs_par);
}

void RD_Model_OPCODE_SET_DIMCCT(void)
{
	Struct_OPCODE_SET_DIMCCT_t *mess_buff;

	//mess_buff = (Struct_OPCODE_SET_DIMCCT_t*) vrts_GWIF_IncomeMessage;

	u8 dim_set	= mess_buff->Dim_Stt;
	u8 cct_set = mess_buff->Cct_Stt;

	//rdPrintf("OPCODE SET dim: %d% cct:%d %", dim_set, cct_set);
	access_cmd_set_light_ctl_100(LED_ADDR, 2 , dim_set, cct_set, 0);
	//access_cmd_set_light_ctl(0xffff, 2 , lum2_lightness(dim_set), cct_set, 0, &TTS_CTRL_DF);
}

void RD_Send_Relay_Stt(uint8_t Relay_ID, uint8_t Relay_Stt)
{
	uint8_t Mess_Buff[8] = {0};
	uint16_t Element_Add = 0x0000;

	Mess_Buff[0]		= Relay_Stt;
	Element_Add 		= ele_adr_primary + (Relay_ID-1);

	mesh_tx_cmd2normal(G_ONOFF_STATUS, Mess_Buff, 1, Element_Add, GATEWAY_ADDRESS, 2);

}

//void Relay_On_Off(u8 stateLed){
//	transition_par_t trs_par = {0};
//	trs_par.transit_t = 10;
//	if(stateLed2){
//		access_cmd_set_lightness(LED_ADDR,2,lum2_lightness(dim_set),0, &trs_par);
//	}else{
//		access_cmd_set_lightness(LED_ADDR,2,0,0, &trs_par);
//	}
//}

void press_sw_kick_out(void){

	if(kick_out_flag == 3){
		kick_out(0);
	}
	if(pm_get_32k_tick() - time_out_kick_out >= 32000*10){
		kick_out_flag = 0;
	}
}

void read_sw(void){
	swx = ReadSW();
	if(swx == '5' ){
		checkKickout = 1;
		// press 1 & 2 in 3 sec -> kickout
		if( (pm_get_32k_tick() - timeKickout >= (32000*3))){
			if(!check_press){

				kick_out_flag++;
				check_press = 1;

				//uart_send_byte(UART0, kick_out_flag);
			}
			time_out_kick_out = pm_get_32k_tick();
			timeKickout = pm_get_32k_tick();
		}
	}else if(swx == '4'){
		stt_sw4 = !stt_sw4;

	}else if(swx == '3'){
		stt_sw3 = !stt_sw3;

	}
	else if(swx == '2'){
		stt_sw2 = !stt_sw2;

		checkScene = 0;
		ui_Screen2_screen_init();
		lv_disp_load_scr(ui_Screen2);
		//lv_obj_set_style_bg_color(ui_Screen2, color, LV_PART_MAIN | LV_STATE_DEFAULT);
		//Set_RGB();
	}
	else if(swx == '1'){
		stt_sw1 = !stt_sw1;

		checkScene = 1;
		mode++;
		mode %= 3;
		if(mode == MODE_DIM_SET){
			encoder_value1 = dim_set;
			ui_Screen1_screen_init();
			lv_disp_load_scr(ui_Screen1);
			lv_label_set_text(ui_Label1, "dim");
			lv_arc_set_value(ui_Arc1, dim_set);
			//access_set_lum(LED_ADDR,2,dim_set,0);

		}if(mode == MODE_CTT_SET){
			encoder_value1 = ctt_set;
			ui_Screen1_screen_init();
			lv_disp_load_scr(ui_Screen1);
			lv_label_set_text(ui_Label1, "ctt");
			lv_arc_set_value(ui_Arc1, ctt_set);
			//access_cmd_set_light_ctl_temp_100(LED_ADDR,2,ctt_set,0);

		}if(mode == MODE_RGB_SET){

			encoder_value1 = hsvH;
			ui_Screen3_screen_init();
			lv_disp_load_scr(ui_Screen3);
			set_RGB_panel(hsvH);
			//Set_RGB();
		}
	}
	else if(swx == SW_NOT_PRESS){

		checkKickout = 0;

		check_press = 0;
	}
	if(swx != SW_NOT_PRESS ){
		On_Off_Led_SW(stt_sw1, stt_sw2, stt_sw3, stt_sw4);
		Write_Data_Flash();
	}
	press_sw_kick_out(); // kick out
}

void Write_Data_Flash(void){
	flash_erase_sector(ADDR_START);
	buf[0] = stt_sw3;
	buf[1] = stt_sw4;
	buf[2] = checkProvision;
	buf[3] = dim_set;
	buf[4] = ctt_set;
	buf[5] = hsvH;
	buf[6] = value_start;
	flash_write_page(ADDR_START,7,buf);

}

void Read_Data_Flash(void){
	flash_read_page(ADDR_START,7,buf);
	stt_sw3 = buf[0];
	stt_sw4 = buf[1];
	checkProvision = buf[2];
	dim_set = buf[3];
	ctt_set = buf[4];
	hsvH = buf[5];
	value_start = buf[6];

}




void Encoder_Control(void){
	if(checkScene == 1 ){
		if(check_encoder_r_l !=0){


				if(mode == MODE_DIM_SET){

						//while(dim_set != encoder_value1){
							dim_set = encoder_value1;
							lv_arc_set_value(ui_Arc1, dim_set);
							sprintf(buff,"%d",dim_set);
							lv_label_set_text(ui_Label1, buff);
							access_set_lum(LED_ADDR,2,dim_set,0);
					//	}
						cnt_print_value ++ ;
						if(cnt_print_value == 30){
							flag_display = 1;

						}



				}else if(mode == MODE_CTT_SET){
					//while(ctt_set != encoder_value1){
						ctt_set = encoder_value1;
						lv_arc_set_value(ui_Arc1, ctt_set);
						sprintf(buff,"%d",ctt_set);
						lv_label_set_text(ui_Label1, buff);
						access_cmd_set_light_ctl_temp_100(LED_ADDR,2,ctt_set,0);
					//};
					cnt_print_value ++ ;
					if(cnt_print_value == 30){
						flag_display = 1;
					}


				}else if(mode == MODE_RGB_SET){
					if(check_encoder_r_l == 1 && (hsvH==0 || hsvH >= 50)){   // unsigned >=0
						encoder_value1 = 49;
					}else if(check_encoder_r_l == 2 && hsvH >= 49){
						encoder_value1 = 0;
					}
					hsvH = encoder_value1;
					set_RGB_panel(hsvH);
					Set_RGB();
				}
				if(check_encoder_r_l == 1 && flag_display == 1){
					//sleep_ms(30);
					Write_Data_Flash();
					func_led_l();
					flag_display=0;
					check_encoder_r_l = 0;
					cnt_print_value = 0;


				}else if(check_encoder_r_l == 2 && flag_display == 1){
					//sleep_ms(30);
					Write_Data_Flash();
					func_led_r();
					flag_display=0;
					check_encoder_r_l = 0;
					cnt_print_value = 0;
				}
			//	Write_Data_Flash();

		}
		ec1 = gpio_read(ENCODER1);
		ec2 = gpio_read(ENCODER2);

	}
}








