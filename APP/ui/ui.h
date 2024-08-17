#ifndef __UI_H
#define __UI_H

#include "board.h"
#include "u8g2.h"
#include "module_adc.h"

typedef enum
{
    UI_MAIN = 0,
    UI_SETVOL,
    UI_SETCUR,
} UI_List;

typedef enum
{
    CA_Y05,
    CA_Y15
} Calibrate_List;

extern Calibrate_List Vol_List, Cur_List;
extern uint32_t Vol_ADC, Cur_ADC;

extern unsigned int X05, X15, IX05, IX15; // 需校准值

typedef struct
{
    /* data */
    u8g2_t *u8g2;
    UI_List page_now;
} UI_t;

void UI_main(u8g2_t *u8g2);
void UI_SetVOl(u8g2_t *u8g2);
void UI_SetCurr(u8g2_t *u8g2);

void ComputeK(void);
void save_calibration(void);
void read_vol_cur_calibration(void);
void get_allADC();
#endif // !__UI_H
