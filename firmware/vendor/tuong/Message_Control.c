/*
 * Message_Control.c
 *
 *  Created on: May 30, 2024
 *      Author: DELL
 */


#include "Message_Control.h"
#include "my_Function.h"

u8 value_start;

void Assembly_Message_Control(u8 para[8], u8 addr_device[2], u8 opcode[3], u8 Header[2], u8 stt, unsigned char *Result){
	for(u8 i = 0; i<8;i++){
		Result[i] = para[i];
	}
	Result[8] = addr_device[0];
	Result[9] = addr_device[1];

	for(u8 i=0;i<3;i++){
		Result[i+10] = opcode[i];
	}
	Result[13] = Result[14] = 0;

	Result[15] = Header[0];
	Result[16] = Header[1];
	Result[17] = stt;
}

int a(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par){
	uint16_t header_Buff = 0x0000;
	header_Buff = (par[0] << 8 | par[1]);

	u8 stt = par[2];
	if(header_Buff == HEADER_SET_START_LED){

		if(stt == OFF_ALL_LED){
			uart_send_byte(UART0, '0');
			value_start = 0;

		}else if(stt == ON_ALL_LED){
			uart_send_byte(UART0, '1');
			value_start = 1;
		}else if(stt == SAVE_STT_LED){
			uart_send_byte(UART0, '2');
			value_start = 2;
		}
		Write_Data_Flash();
	}else{
		uart_send_byte(UART0, 'N');
	}
}

