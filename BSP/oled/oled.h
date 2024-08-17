#ifndef __OLED_H
#define __OLED_H 

#include "board.h"


//-----------------OLED端口定义---------------- 

#define BSP_SPI1        	CW_SPI1

//GPIO AF
#define SPI1_AF_SCK()   	PA05_AFx_SPI1SCK()
#define SPI1_AF_MOSI()  	PA07_AFx_SPI1MOSI()

#define OLED_GPIO_PORT		CW_GPIOA

#define OLED_SCL_PIN		GPIO_PIN_5
#define OLED_MOSI_PIN		GPIO_PIN_7
#define OLED_RES_PIN		GPIO_PIN_3
#define OLED_DC_PIN			GPIO_PIN_2
#define OLED_CS_PIN			GPIO_PIN_4


#define OLED_RES_Clr() GPIO_WritePin(OLED_GPIO_PORT, OLED_RES_PIN, GPIO_Pin_RESET)//RES
#define OLED_RES_Set() GPIO_WritePin(OLED_GPIO_PORT, OLED_RES_PIN, GPIO_Pin_SET)

#define OLED_DC_Clr()  GPIO_WritePin(OLED_GPIO_PORT, OLED_DC_PIN, GPIO_Pin_RESET)//DC
#define OLED_DC_Set()  GPIO_WritePin(OLED_GPIO_PORT, OLED_DC_PIN, GPIO_Pin_SET)
 		     
#define OLED_CS_Clr()  GPIO_WritePin(OLED_GPIO_PORT, OLED_CS_PIN, GPIO_Pin_RESET)//CS
#define OLED_CS_Set()  GPIO_WritePin(OLED_GPIO_PORT, OLED_CS_PIN, GPIO_Pin_SET)

// 使用U8G2

#define USE_U8G2 1
// u8g2
#if USE_U8G2

#include "u8g2.h"

uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8x8_stm32_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr);
#else

#define OLED_CMD 0  // 写命令
#define OLED_DATA 1 // 写数据

#define u8 unsigned char
#define u16 unsigned int
#define u32 unsigned long

void OLED_ClearPoint(u8 x, u8 y);
void OLED_ColorTurn(u8 i);
void OLED_DisplayTurn(u8 i);
void OLED_WR_Byte(u8 dat, u8 mode);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x, u8 y, u8 t);
void OLED_DrawLine(u8 x1, u8 y1, u8 x2, u8 y2, u8 mode);
void OLED_DrawCircle(u8 x, u8 y, u8 r);
void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 size1, u8 mode);
void OLED_ShowChar6x8(u8 x, u8 y, u8 chr, u8 mode);
void OLED_ShowString(u8 x, u8 y, u8 *chr, u8 size1, u8 mode);
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size1, u8 mode);
void OLED_ShowChinese(u8 x, u8 y, u8 num, u8 size1, u8 mode);
void OLED_ScrollDisplay(u8 num, u8 space, u8 mode);
void OLED_ShowPicture(u8 x, u8 y, u8 sizex, u8 sizey, u8 BMP[], u8 mode);
#endif
void OLED_Init(void);
#endif

