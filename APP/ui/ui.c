#include "ui.h"
#include "flash.h"

#include "bsp_log.h"
uint8_t adc_en;

char buff[32] = {0};
uint16_t VLotage_buff[ADC_SAMPLE_SIZE] = {0};
uint16_t Current_buff[ADC_SAMPLE_SIZE] = {0};

float32_t Vol_Real, Cur_Real;
uint32_t Vol_ADC, Cur_ADC;

// 5V与15V 校准
unsigned int X05 = 0;
unsigned int X15 = 0;

unsigned int Y15 = 12; // 由于作者没有15V电源，故以12V代替
unsigned int Y05 = 5;
float32_t K; // 斜率

// 0.5A与1.5A 校准
unsigned int IX05 = 0;
unsigned int IX15 = 0;

unsigned int IY15 = 150; // 1.5A
unsigned int IY05 = 50;  // 0.5A
float32_t KI;            // 斜率

Calibrate_List Vol_List, Cur_List;

void get_allADC()
{
    if (adc_en)
    {
        adc_en = 0;
        GetVoltagContinue(VLotage_buff);
        GetCurrentContinue(Current_buff);
    }
}

void UI_main(u8g2_t *u8g2) // 主界面，显示电压电流信息
{
    u8g2_SetFont(u8g2, u8g2_font_fub30_tf);

    Vol_ADC = mean_value_filter(VLotage_buff, ADC_SAMPLE_SIZE);
    Cur_ADC = mean_value_filter(Current_buff, ADC_SAMPLE_SIZE);
    if (Vol_ADC > X05)
    {
        Vol_Real = (float32_t)((Vol_ADC - X05) * K + Y05);
    }
    else
    {
        Vol_Real = (float32_t)(Vol_ADC * K);
    }

    if (Cur_ADC > IX05)
    {
        Cur_Real = (float32_t)(((Cur_ADC - IX05) * KI + IY05) / 100);
    }
    else
    {
        Cur_Real = (float32_t)(Cur_ADC * KI / 100);
    }
    u8g2_FirstPage(u8g2);
    do
    {
        sprintf(buff, "%.2fv", Vol_Real);
        u8g2_DrawStr(u8g2, 0, 31, buff);
        sprintf(buff, "%.3fa", Cur_Real);
        u8g2_DrawStr(u8g2, 0, 63, buff);
    } while (u8g2_NextPage(u8g2));
}

void UI_SetVOl(u8g2_t *u8g2) // 电压标定界面
{
    u8g2_SetFont(u8g2, u8g2_font_t0_16_tf);

    Vol_ADC = mean_value_filter(VLotage_buff, ADC_SAMPLE_SIZE);

    if (Vol_ADC > X05)
    {
        Vol_Real = (float32_t)((Vol_ADC - X05) * K + Y05);
    }
    else
    {
        Vol_Real = (float32_t)(Vol_ADC * K);
    }
    u8g2_FirstPage(u8g2);
    do
    {
        sprintf(buff, "Vol Setup");
        u8g2_DrawStr(u8g2, 0, 15, buff);
        sprintf(buff, "Vol->ADC:%d", Vol_ADC);
        u8g2_DrawStr(u8g2, 0, 31, buff);
        switch (Vol_List)
        {
        case CA_Y05:
            sprintf(buff, "Vol->Set:%.2fv", (float32_t)Y05);
            break;
        case CA_Y15:
            sprintf(buff, "Vol->Set:%.2fv", (float32_t)Y15);
            break;
        default:
            break;
        }
        u8g2_DrawStr(u8g2, 0, 47, buff);
        sprintf(buff, "Vol->Now:%.2fv", Vol_Real);
        u8g2_DrawStr(u8g2, 0, 63, buff);

    } while (u8g2_NextPage(u8g2));
}

void UI_SetCurr(u8g2_t *u8g2) // 电流标定界面
{
    u8g2_SetFont(u8g2, u8g2_font_t0_16_tf);
    Cur_ADC = mean_value_filter(Current_buff, ADC_SAMPLE_SIZE);
    if (Cur_ADC > IX05)
    {
        Cur_Real = (float32_t)(((Cur_ADC - IX05) * KI + IY05) / 100);
    }
    else
    {
        Cur_Real = (float32_t)(Cur_ADC * KI / 100);
    }
    u8g2_FirstPage(u8g2);
    do
    {
        sprintf(buff, "Cur Setup");
        u8g2_DrawStr(u8g2, 0, 15, buff);
        sprintf(buff, "Cur->ADC:%d", Cur_ADC);
        u8g2_DrawStr(u8g2, 0, 31, buff);
        switch (Cur_List)
        {
        case CA_Y05:
            sprintf(buff, "Cur->Set:%.3fa", (float32_t)IY05 / 100);
            break;
        case CA_Y15:
            sprintf(buff, "Cur->Set:%.3fa", (float32_t)IY15 / 100);
            break;
        default:
            break;
        }
        u8g2_DrawStr(u8g2, 0, 47, buff);
        sprintf(buff, "Cur->Now:%.3fa", Cur_Real);
        u8g2_DrawStr(u8g2, 0, 63, buff);

    } while (u8g2_NextPage(u8g2));
}

void ComputeK(void)
{
    K = (Y15 - Y05);
    K = K / (X15 - X05); // 电压斜率

    KI = (IY15 - IY05);
    KI = KI / (IX15 - IX05); // 电流斜率
}

void save_calibration(void)
{
    uint16_t da[5];
    da[0] = 0xaa;
    da[1] = X05;
    da[2] = X15;
    da[3] = IX05;
    da[4] = IX15;
    flash_erase();
    flash_write(0, da, 5);
}
/**
 * @brief
 *
 */
void read_vol_cur_calibration(void)
{
    uint16_t da[5];
    flash_read(0, da, 5);
    if (da[0] != 0xaa) // 还没校准过时，计算理论值，并存储
    {
        X15 = 15.0 / 23 / 1.5 * 4096;
        X05 = 5.0 / 23 / 1.5 * 4096;
        IX05 = 0.5 * 0.1 / 1.5 * 4096;
        IX15 = 1.5 * 0.1 / 1.5 * 4096;
        save_calibration();
    }
    else
    {
        X05 = da[1];
        X15 = da[2];
        IX05 = da[3];
        IX15 = da[4];
    }
}