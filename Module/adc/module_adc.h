#ifndef __MODULE_ADC_H
#define __MODULE_ADC_H

#include "board.h"

#define ADC_SAMPLE_SIZE (20)

void Module_ADC_init();
void GetVoltagContinue(uint16_t *buff);
void GetCurrentContinue(uint16_t *buff);
uint32_t mean_value_filter(uint16_t *value, uint16_t size);

#endif // !__ADC_H
