/*
 * RD_Secure.c
 *
 *  Created on: Jul 5, 2022
 *      Author: Hao PC
 */

#include "RD_Secure.h"

Secure_Stt_Str Secure_Stt_Val;  
unsigned char RD_key[16] = "Digital@28112804"; //6B 65 79 52 61 6E 67 44 6F 6E 67 5F 52 6E 44 44
unsigned char param_RD[8] = {0x24, 0x02, 0x28, 0x04, 0x28, 0x11, 0x20, 0x20}; // 8 byte selfgen by RD
unsigned char RD_mac[6] = {0};
extern u8 checkProvision;

extern uint8_t para[8];
extern u32 timeOut ;
/**
 * @brief Assembly param_RD - mac - Unicast to 16byte Result key
 * 
 * @param param_RD : 8 btye const by RD define 
 * @param mac 		: 6 byte mac ID of device 
 * @param unicast_ID : 2 byte unicast ID proved after provision   
 * @param result_Key : 16 byte result_Key
 */
static void RD_Secure_Assembly_For_KeyIn(uint8_t param_RD[8], uint8_t mac[6], uint16_t unicast_ID, unsigned char *result_Key)
{
	for(unsigned char i=0;i<8;i++){
		result_Key[i] = param_RD[i];
	}

	for(unsigned char i=0;i<6;i++){
		result_Key[i+8] = mac[i];
	}

	result_Key[14] = unicast_ID & 0xFF;
	result_Key[15] = (unicast_ID >> 8) & 0xff;
}

//Secure_return = RD_Secure_AesreCheck(&RD_key[0], &tbl_mac[0], ele_adr_primary, &par[2]);

unsigned char RD_Secure_AesreCheck(uint8_t key[16], uint8_t mac[6], uint16_t unicast, uint8_t compare_key[6])
{
	unsigned char compareBuff[16] = {0};
	unsigned char aesEncrypt[16] = {0};

	RD_Secure_Assembly_For_KeyIn(param_RD, &mac[0], unicast, &compareBuff[0]);


	if(aes_encrypt(&key[0],compareBuff,aesEncrypt)!= 0){
		//uart_send_byte(UART0,'F');
		checkProvision=3;
	}

	for(u8 i=0;i< 6;i++){
		if(aesEncrypt[i+10] != compare_key[i]){
			return 0;
		}
	}
	timeOut = pm_get_32k_tick();
	return 1;

	#if 0
	uart_CSend("\n Key in: ");
	for(int i=0; i<16; i++)
	{
		char UART_TempSend[128];
		sprintf(UART_TempSend,"0x%x -",compareBuff[i]);
		uart_CSend(UART_TempSend);
	}
	uart_CSend("\n key out: ");
	for(int i=0; i<16; i++)
	{
		char UART_TempSend[128];
		sprintf(UART_TempSend,"0x%x -",aesEncrypt[i]);
		uart_CSend(UART_TempSend);
	}
	#endif
}
void RD_Handle_MessType(uint8_t par[8], uint16_t Gw_Add_Buff)
{
	for(u8 i =0; i<8; i++){
		para[i] = par[i];
	}
	uart_send(UART0, para,8);

	uint8_t Secure_return = 0;
	Secure_return = RD_Secure_AesreCheck(&RD_key[0], &tbl_mac[0], ele_adr_primary, &par[2]);
	if(Secure_return == 1){
		checkProvision=1;

	}else{
		checkProvision=2;
	}
}
int RD_Messenger_ProcessCommingProcess_Type(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par){

	uint16_t Gw_Add_Buff =0x00;
	Gw_Add_Buff = cb_par->adr_src;

	uint16_t header_Buff = 0x0000;
	header_Buff = (par[1] << 8 | par[0]);

	RD_Handle_MessType(par, Gw_Add_Buff);

	/*switch (header_Buff)
	{
		case RD_HEADER_RSP_TYPE_DEVICE:
			RD_Handle_MessType(par, Gw_Add_Buff);
			break;
		case RD_HEADER_SAVE_GW_ADD:
			//RD_Handle_SaveGw(par, Gw_Add_Buff);
			break;

		default:
			break;
	}*/
	return 0;
}



void RD_Secure_CheckLoop(void)
{
//	if((Secure_Stt_Val.Secure_Pass != 1) && (get_provision_state() == STATE_DEV_PROVED)) 	// check secure if device in state provided and secure faile
//	{
//		if(Secure_Stt_Val.Check_Secure_Ready == 0)
//		{
//			uart_CSend("Provisioned Start waiting for secure key \n");
//			Secure_Stt_Val.Check_Secure_Ready 	= 1;
//			Secure_Stt_Val.Time_Start_check 	= clock_time_ms();			// set time Start check secure
//		}
//
//		if( ((clock_time_ms() - Secure_Stt_Val.Time_Start_check) >=  TIME_UOT_SECURE) && (Remote_Working_Stt_Val.OTA_Flag == 0x00))
//		{
//			uart_CSend("Secure Time out . Factory reset \n");
//			RD_Remote_FacReset_ByUser();
//		}
//
//		if(((clock_time_ms() - Secure_Stt_Val.Time_Check_Key_Fail) >=  TIME_UOT_KEY_FAIL) && (Secure_Stt_Val.Check_Key_Fail == RD_EN))
//		{
//			uart_CSend("Secure Key fail . Factory reset \n");
//			RD_Remote_FacReset_ByUser();
//		}
//	}
}

static void RD_Secure_Check(void)
{
//	uint8_t mac[6] = {0xe0, 0x6a, 0x80, 0x02, 0x03, 0x02};
//	uint16_t Add = 0x0012;
//	uint8_t Kq[6] = {0x84, 0xd1, 0x82, 0xb1, 0x88, 0x48};
//	uint8_t result = RD_Secure_AesreCheck(&RD_key[0], &mac[0], Add, &Kq[0]);
//	if(result == 1) uart_CSend("test key pass \n");
//	else 			uart_CSend("test key fail \n");

}

void RD_Secure_CheckInit(void)
{
//	Secure_Stt_Val.Secure_Pass = Remote_Flash_Data_Val.Secure_Pass;
//
//	if((Secure_Stt_Val.Secure_Pass == FALSE) && (get_provision_state() == STATE_DEV_PROVED))
//	{
//		uart_CSend("Secure Init fail. reset factory");
//
//		RD_Remote_FacReset_ByUser();
//	}

	//ARD_Secure_Check();
}

void RD_Secure_Set(uint8_t Secure_Pass_Stt)
{
	Secure_Stt_Val.Secure_Pass = Secure_Pass_Stt;
}

void RD_Secure_Check_Key_Fail(void)
{
//	Secure_Stt_Val.Check_Key_Fail = RD_EN;
//	Secure_Stt_Val.Time_Check_Key_Fail = clock_time_ms();
}
