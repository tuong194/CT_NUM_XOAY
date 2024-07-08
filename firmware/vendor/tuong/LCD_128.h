#ifndef _LCD_128_H_

#define _LCD_128_H_

#include "tl_common.h"
#include "proj/mcu/watchdog_i.h"
#include "vendor/common/user_config.h"
#include "drivers.h"
#include "proj_lib/ble/blt_config.h"
#include "proj_lib/ble/ll/ll.h"
#include "proj_lib/sig_mesh/app_mesh.h"




#define RST_Pin GPIO_PE1 //GPIO_PA1
#define func  AS_GPIO
#define DC_Pin  GPIO_PE3 //GPIO_PA3

#define HORIZONTAL 0 //ngang
#define VERTICAL   1 //doc

#define LCD_HEIGHT 240
#define LCD_WIDTH 240

#define SPI_CLOCK 12000000

typedef struct{
	u16 WIDTH;
	u16 HEIGHT;
	u16 SCAN_DIR;
}LCD_1IN28_ATTRIBUTES;
//extern LCD_1IN28_ATTRIBUTES LCD_1INCH28;

#define WHITE 0xFFFF
#define BLACK 0x0000
#define RED   0xF800  // F800
#define GREEN 0x07E0
#define BLUE  0x001F

void PWM_Confing(void);
void SPI_Config(void);
void LCD_SetBacklight(int value);
void LCD_RST(void);
void LCD_InitReg(void);
//void LCD_Init(u8 scan_dir);
void LCD_setWindow(u16 Xstart, u16 Ystart, u16 Xend, u16 Yend);
void LCD_showImg(void);
void LCD_showImg2(void);
void LCD_showImg3(void);
void LCD_putChar(u8 x,u8 y, u8 value);
void LCD_putStr(u8 x,u8 y, u8 *str);
void LCD_Clear(u16 color);



#endif
