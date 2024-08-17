/*
 * 立创开发板软硬件资料与相关扩展板软硬件资料官网全部开源
 * 开发板官网：www.lckfb.com
 * 技术支持常驻论坛，任何技术问题欢迎随时交流学习
 * 立创论坛：https://oshwhub.com/forum
 * 关注bilibili账号：【立创开发板】，掌握我们的最新动态！
 * 不靠卖板赚钱，以培养中国工程师为己任
 * Change Logs:
 * Date           Author       Notes
 * 2024-06-12     LCKFB-LP    first version
 */
#include "board.h"

#include "bsp_log.h"

#include "oled.h"

#include "ui.h"

#include "module_adc.h"
#include "module_time.h"
#include "bsp_key.h"

UI_t *uip = NULL;
void KeyLeft_CB(KEY_EVEN EVEN, void *args)
{
	switch (EVEN)
	{
	case KEY_EVEN_PRESS:
		break;
	case KEY_EVEN_LONGPRESS_STAR:
		break;
	case KEY_EVEN_LONGPRESS_HOLD:
		break;
	case KEY_EVEN_RELEASE:
		break;
	case KEY_EVEN_CLICK:
		switch (uip->page_now)
		{
		case UI_MAIN:
			break;
		case UI_SETVOL:
			uip->page_now = UI_MAIN;
			break;
		case UI_SETCUR:
			uip->page_now = UI_SETVOL;
			break;
		default:
			break;
		}
		break;
	case KEY_EVEN_REPEAT:
		break;
	default:
		break;
	}
}
void KeyOK_CB(KEY_EVEN EVEN, void *args)
{
	switch (EVEN)
	{
	case KEY_EVEN_PRESS:
		break;
	case KEY_EVEN_LONGPRESS_STAR:
		break;
	case KEY_EVEN_LONGPRESS_HOLD:
		break;
	case KEY_EVEN_RELEASE:
		break;
	case KEY_EVEN_CLICK:
		switch (uip->page_now)
		{
		case UI_SETVOL:
			switch (Vol_List)
			{
			case CA_Y05:
				X05 = Vol_ADC;
				Vol_List = CA_Y15;
				break;
			case CA_Y15:
				X15 = Vol_ADC;
				Vol_List = CA_Y05;
				ComputeK();
				save_calibration();
				break;
			default:
				break;
			}
			break;
		case UI_SETCUR:
			switch (Cur_List)
			{
			case CA_Y05:
				IX05 = Cur_ADC;
				Cur_List = CA_Y15;
				break;
			case CA_Y15:
				IX15 = Cur_ADC;
				Cur_List = CA_Y05;
				ComputeK();
				save_calibration();
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		break;
	case KEY_EVEN_REPEAT:
		break;
	default:
		break;
	}
}
void KeyRight_CB(KEY_EVEN EVEN, void *args)
{
	switch (EVEN)
	{
	case KEY_EVEN_PRESS:
		break;
	case KEY_EVEN_LONGPRESS_STAR:
		break;
	case KEY_EVEN_LONGPRESS_HOLD:
		break;
	case KEY_EVEN_RELEASE:
		break;
	case KEY_EVEN_CLICK:
		switch (uip->page_now)
		{
		case UI_MAIN:
			uip->page_now = UI_SETVOL;
			break;
		case UI_SETVOL:
			uip->page_now = UI_SETCUR;
			break;
		case UI_SETCUR:

			break;
		default:
			break;
		}
		break;
		break;
	case KEY_EVEN_REPEAT:
		break;
	default:
		break;
	}
}

int32_t main(void)
{
	board_init(); // 开发板初始化

	uart1_init(115200); // 串口1波特率115200

#if USE_U8G2
	u8g2_t u8g2;

	u8g2_Setup_sh1106_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_4wire_hw_spi, u8x8_stm32_gpio_and_delay);

	u8g2_InitDisplay(&u8g2);	 // 根据所选的芯片进行初始化工作，初始化完成后，显示器处于关闭状态
	u8g2_SetPowerSave(&u8g2, 0); // 打开显示器
#else
	OLED_Init();
	OLED_DisPlay_On();
#endif

	Module_ADC_init();
	BTIME1_InitFor1ms();

	KEY_Init();
	KEY_Attach(KEY_LEFT, KeyLeft_CB);
	KEY_Attach(KEY_OK, KeyOK_CB);
	KEY_Attach(KEY_RIGHT, KeyRight_CB);

	UI_t ui = {
		.page_now = UI_MAIN,
		.u8g2 = &u8g2};
	uip = &ui;

	read_vol_cur_calibration();
	ComputeK();
	while (1)
	{
		FSM_KeyScanServer(); // 按键扫描
		get_allADC();		 // 获取电压电流信息
		switch (ui.page_now)
		{
		case UI_MAIN:
			UI_main(ui.u8g2);
			break;
		case UI_SETVOL:
			UI_SetVOl(ui.u8g2);
			break;
		case UI_SETCUR:
			UI_SetCurr(ui.u8g2);
			break;
		default:
			break;
		}
	}
}
