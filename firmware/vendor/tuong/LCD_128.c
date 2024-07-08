

#include "LCD_128.h"
#include "font.h"
#include "image.h"



void PWM_Confing(void){
//	pwm_set_pin(PWM_PWM5_PE5);
//	pwm_set_clk((unsigned char) (sys_clk.pclk*1000*1000/24000000-1));//24M
//	pwm_set_pwm0_mode(PWM_NORMAL_MODE);
//	pwm_set_tcmp(PWM0_ID,0);
//	pwm_set_tmax(PWM0_ID,1000*CLOCK_PWM_CLOCK_1US);
//	pwm_start(PWM0_ID);

	gpio_set_func(GPIO_PE5,AS_GPIO);
	gpio_output_en(GPIO_PE5);
	gpio_write(GPIO_PE5,1);
}


void SPI_Config(void)
{
	// config Pin
	gpio_set_func(RST_Pin,func);
	gpio_output_en(RST_Pin);
	gpio_write(RST_Pin,1);
	gpio_set_func(DC_Pin,func);
	gpio_output_en(DC_Pin);

	//config spi
	hspi_pin_config_t configPin;
	configPin.hspi_clk_pin = HSPI_CLK_PB4;
	configPin.hspi_csn_pin = HSPI_CSN_PB6;
	configPin.hspi_miso_io1_pin = HSPI_MISO_IO1_PB2;
	configPin.hspi_mosi_io0_pin = HSPI_MOSI_IO0_PB3;
	configPin.hspi_hold_io3_pin = HSPI_HOLD_IO3_PB0;
	configPin.hspi_wp_io2_pin = HSPI_WP_IO2_PB1;
	hspi_set_pin(&configPin);

	spi_master_init(HSPI_MODULE,sys_clk.hclk*1000000/(2*SPI_CLOCK)-1,SPI_MODE0);//f=12 MHz
	spi_master_config(HSPI_MODULE,SPI_3LINE);
}

void SPI2_Config(void){
	// config Pin
	gpio_set_func(RST_Pin,func);
	gpio_output_en(RST_Pin);
	gpio_write(RST_Pin,1);
	gpio_set_func(DC_Pin,func);
	gpio_output_en(DC_Pin);
	//config spi
	pspi_pin_config_t config_pin;
	config_pin.pspi_clk_pin = PSPI_CLK_PC5;
	config_pin.pspi_csn_pin = PSPI_CSN_PC4;
	config_pin.pspi_miso_io1_pin = PSPI_MISO_IO1_PC6;
	config_pin.pspi_mosi_io0_pin = PSPI_MOSI_IO0_PC7;
	pspi_set_pin(&config_pin);
	spi_master_init(PSPI_MODULE,sys_clk.hclk*1000000/(2*SPI_CLOCK)-1,SPI_MODE0);//f=12 MHz
	spi_master_config(PSPI_MODULE,SPI_3LINE);
}

void LCD_SetBacklight(int value){
	pwm_set_pwm0_pulse_num(value);
}

void LCD_RST(void){
//	gpio_write(RST_Pin,1);
//	sleep_ms(5);

	gpio_write(RST_Pin,0);
	sleep_ms(10);

	gpio_write(RST_Pin,1);
	sleep_ms(120);
}

void LCD_SendCmd(u8 cmd){
	u8 cmdx[5];
	cmdx[0]=cmd;
	gpio_write(DC_Pin,0);
	spi_master_write(HSPI_MODULE,cmdx,1);
}
void LCD_SendData8bit(u8 data){
	u8 datax[5];
	datax[0] = data;
	gpio_write(DC_Pin,1);
	spi_master_write(HSPI_MODULE,datax,1);
}
void LCD_SendData16bit(u16 data){
	u8 datax[5];
	datax[0]=data>>8;
	datax[1]=data;
	gpio_write(DC_Pin,1);
	spi_master_write(HSPI_MODULE,datax,2);

}

void LCD_InitReg(void){
	LCD_SendCmd(0xEF);

	LCD_SendCmd(0xEB);
	LCD_SendData8bit(0x14);

	LCD_SendCmd(0xFE);
	LCD_SendCmd(0xEF);

	LCD_SendCmd(0xEB);
	LCD_SendData8bit(0x14);

	LCD_SendCmd(0x84);
	LCD_SendData8bit(0x40);

	LCD_SendCmd(0x85);
	LCD_SendData8bit(0xFF);

	LCD_SendCmd(0x86);
	LCD_SendData8bit(0xFF);

	LCD_SendCmd(0x87);
	LCD_SendData8bit(0xFF);

	LCD_SendCmd(0x88);
	LCD_SendData8bit(0x0A);

	LCD_SendCmd(0x89);
	LCD_SendData8bit(0x21);

	LCD_SendCmd(0x8A);
	LCD_SendData8bit(0x00);

	LCD_SendCmd(0x8B);
	LCD_SendData8bit(0x80);

	LCD_SendCmd(0x8C);
	LCD_SendData8bit(0x01);

	LCD_SendCmd(0x8D);
	LCD_SendData8bit(0x01);

	LCD_SendCmd(0x8E);
	LCD_SendData8bit(0xFF);

	LCD_SendCmd(0x8F);
	LCD_SendData8bit(0xFF);

	LCD_SendCmd(0xB6);    // display func control
	LCD_SendData8bit(0x00);
	LCD_SendData8bit(0x20); //20->00

	LCD_SendCmd(0x36); // memory access control
	LCD_SendData8bit(0x08); //08->18 28 48 88

	LCD_SendCmd(0x3A);  // set pixel format
	LCD_SendData8bit(0x05);

	LCD_SendCmd(0x90);
	LCD_SendData8bit(0x08);
	LCD_SendData8bit(0x08);
	LCD_SendData8bit(0x08);
	LCD_SendData8bit(0x08);


	LCD_SendCmd(0xBD);
	LCD_SendData8bit(0x06);

	LCD_SendCmd(0xBC);
	LCD_SendData8bit(0x00);

	LCD_SendCmd(0xFF);
	LCD_SendData8bit(0x60);
	LCD_SendData8bit(0x01);
	LCD_SendData8bit(0x04);

	LCD_SendCmd(0xC3);
	LCD_SendData8bit(0x13);
	LCD_SendCmd(0xC4);
	LCD_SendData8bit(0x13);

	LCD_SendCmd(0xC9);
	LCD_SendData8bit(0x22);

	LCD_SendCmd(0xBE);
	LCD_SendData8bit(0x11);

	LCD_SendCmd(0xE1);
	LCD_SendData8bit(0x10);
	LCD_SendData8bit(0x0E);

	LCD_SendCmd(0xDF);
	LCD_SendData8bit(0x21);
	LCD_SendData8bit(0x0C);
	LCD_SendData8bit(0x02);

	LCD_SendCmd(0xF0);
	LCD_SendData8bit(0x45);
	LCD_SendData8bit(0x09);
	LCD_SendData8bit(0x08);
	LCD_SendData8bit(0x08);
	LCD_SendData8bit(0x26);
	LCD_SendData8bit(0x2A);

	LCD_SendCmd(0xF1);
	LCD_SendData8bit(0x43);
	LCD_SendData8bit(0x70);
	LCD_SendData8bit(0x72);
	LCD_SendData8bit(0x36);
	LCD_SendData8bit(0x37);
	LCD_SendData8bit(0x6F);

	LCD_SendCmd(0xF2);
	LCD_SendData8bit(0x45);
	LCD_SendData8bit(0x09);
	LCD_SendData8bit(0x08);
	LCD_SendData8bit(0x08);
	LCD_SendData8bit(0x26);
	LCD_SendData8bit(0x2A);;

	LCD_SendCmd(0xF3);
	LCD_SendData8bit(0x43);
	LCD_SendData8bit(0x70);
	LCD_SendData8bit(0x72);
	LCD_SendData8bit(0x36);
	LCD_SendData8bit(0x37);
	LCD_SendData8bit(0x6F);

	LCD_SendCmd(0xED);
	LCD_SendData8bit(0x1B);
	LCD_SendData8bit(0x0B);

	LCD_SendCmd(0xAE);
	LCD_SendData8bit(0x77);

	LCD_SendCmd(0xCD);
	LCD_SendData8bit(0x63);

	LCD_SendCmd(0x70);
	LCD_SendData8bit(0x07);
	LCD_SendData8bit(0x07);
	LCD_SendData8bit(0x04);
	LCD_SendData8bit(0x0E);
	LCD_SendData8bit(0x0F);
	LCD_SendData8bit(0x09);
	LCD_SendData8bit(0x07);
	LCD_SendData8bit(0x08);
	LCD_SendData8bit(0x03);

	LCD_SendCmd(0xE8);
	LCD_SendData8bit(0x34);

	LCD_SendCmd(0x62);
	LCD_SendData8bit(0x18);
	LCD_SendData8bit(0x0D);
	LCD_SendData8bit(0x71);
	LCD_SendData8bit(0xED);
	LCD_SendData8bit(0x70);
	LCD_SendData8bit(0x70);
	LCD_SendData8bit(0x18);
	LCD_SendData8bit(0x0F);
	LCD_SendData8bit(0x71);
	LCD_SendData8bit(0xEF);
	LCD_SendData8bit(0x70);
	LCD_SendData8bit(0x70);

	LCD_SendCmd(0x63);
	LCD_SendData8bit(0x18);
	LCD_SendData8bit(0x11);
	LCD_SendData8bit(0x71);
	LCD_SendData8bit(0xF1);
	LCD_SendData8bit(0x70);
	LCD_SendData8bit(0x70);
	LCD_SendData8bit(0x18);
	LCD_SendData8bit(0x13);
	LCD_SendData8bit(0x71);
	LCD_SendData8bit(0xF3);
	LCD_SendData8bit(0x70);
	LCD_SendData8bit(0x70);

	LCD_SendCmd(0x64);
	LCD_SendData8bit(0x28);
	LCD_SendData8bit(0x29);
	LCD_SendData8bit(0xF1);
	LCD_SendData8bit(0x01);
	LCD_SendData8bit(0xF1);
	LCD_SendData8bit(0x00);
	LCD_SendData8bit(0x07);

	LCD_SendCmd(0x66);
	LCD_SendData8bit(0x3C);
	LCD_SendData8bit(0x00);
	LCD_SendData8bit(0xCD);
	LCD_SendData8bit(0x67);
	LCD_SendData8bit(0x45);
	LCD_SendData8bit(0x45);
	LCD_SendData8bit(0x10);
	LCD_SendData8bit(0x00);
	LCD_SendData8bit(0x00);
	LCD_SendData8bit(0x00);

	LCD_SendCmd(0x67);
	LCD_SendData8bit(0x00);
	LCD_SendData8bit(0x3C);
	LCD_SendData8bit(0x00);
	LCD_SendData8bit(0x00);
	LCD_SendData8bit(0x00);
	LCD_SendData8bit(0x01);
	LCD_SendData8bit(0x54);
	LCD_SendData8bit(0x10);
	LCD_SendData8bit(0x32);
	LCD_SendData8bit(0x98);

	LCD_SendCmd(0x74);
	LCD_SendData8bit(0x10);
	LCD_SendData8bit(0x85);
	LCD_SendData8bit(0x80);
	LCD_SendData8bit(0x00);
	LCD_SendData8bit(0x00);
	LCD_SendData8bit(0x4E);
	LCD_SendData8bit(0x00);

	LCD_SendCmd(0x98);
	LCD_SendData8bit(0x3e);
	LCD_SendData8bit(0x07);

	LCD_SendCmd(0x35);
	LCD_SendCmd(0x21);

	LCD_SendCmd(0x11);
	sleep_ms(120);
	wd_clear();
	LCD_SendCmd(0x29);
	sleep_ms(20);
}




LCD_1IN28_ATTRIBUTES LCD_1INCH28;
void LCD_setAttribute(u8 scan_dir){

	LCD_1INCH28.SCAN_DIR = scan_dir;
	u8 memoryaccess = 0x08;

	if(scan_dir == HORIZONTAL){
		LCD_1INCH28.HEIGHT = LCD_HEIGHT;
		LCD_1INCH28.WIDTH = LCD_WIDTH;
		memoryaccess = 0xC8;
	}else{
		LCD_1INCH28.WIDTH = LCD_HEIGHT;
		LCD_1INCH28.HEIGHT = LCD_WIDTH;
		memoryaccess = 0x68;
	}
	LCD_SendCmd(0x36); //truy cap memory
	LCD_SendData8bit(memoryaccess);
}
void LCD_Init(u8 scan_dir){
	LCD_RST();
	LCD_InitReg();
	LCD_setAttribute(scan_dir);
}

void LCD_setWindow(u16 Xstart, u16 Ystart, u16 Xend, u16 Yend){  //u8,16
	LCD_SendCmd(0x2A); // set dia chi cot
	LCD_SendData8bit(0x00);
	LCD_SendData8bit(Xstart);
	LCD_SendData8bit(0x00);
	LCD_SendData8bit(Xend);

	LCD_SendCmd(0x2B); // set dc hang
	LCD_SendData8bit(0x00);
	LCD_SendData8bit(Ystart);
	LCD_SendData8bit(0x00);
	LCD_SendData8bit(Yend);

	LCD_SendCmd(0x2C); // write to memory
}

void LCD_setpixel(u16 x, u16 y, u16 color){
	LCD_setWindow(x,y,x,y);
	LCD_SendData16bit(color);
}
//show img
void LCD_showImg(void){
	u8 i,j;
	LCD_setWindow(0,0,239,239);
	for(j=0;j<240;j++){
		for(i=0;i<240;i++){
			//LCD_setpixel(i,j,img[240*j+i]);
			LCD_SendData16bit(img[240*j+i]);
			wd_clear();
		}
	}
}
//show img
void LCD_showImg2(void){
	u8 i,j;
	LCD_setWindow(0,0,239,239);

	for(j=0;j<240;j++){
		for(i=0;i<240;i++){
			LCD_SendData16bit(duck_gif[240*j+i]);
			wd_clear();
		}
	}
}


void LCD_putChar(u8 x,u8 y, u8 value){
	u8 i,j,k;
	u8 *p = charr24;
	LCD_setWindow(x,y,x+23,y+23);
	p+= (value-32)*24*3;
	for(j=0;j<24;j++){  // nen trang chu do   //duyet hang
		k=0;
		for(i=0;i<24;i++){
			if((*p & (1<<(7-k))) != 0){
				LCD_SendData16bit(RED);
			}else{
				LCD_SendData16bit(WHITE);
			}
			k++;
			if(k == 8){
				k=0;
				p++;
			}
		}
	}
}

// show string
void LCD_putStr(u8 x,u8 y, u8 *str){
	while(*str != '\0'){
		LCD_putChar(x,y,*str);
		x+=7;
		str++;
	}
}

void LCD_Clear(u16 color){
	u8 i,j;
	LCD_setWindow(0,0,239,239); //200,120
	u8 data[5];
	data[0]= color>>8;
	data[1] = color;
	gpio_write(DC_Pin,1);
	for( i=0;i<240;i++){
		for( j=0;j<240;j++){
			spi_master_write(HSPI_MODULE,data,2);
			wd_clear();
		}
	}
}














