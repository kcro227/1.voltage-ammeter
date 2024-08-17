#include "module_adc.h"

void Module_ADC_init()
{
    __RCC_GPIOA_CLK_ENABLE();
    __RCC_ADC_CLK_ENABLE();

    PB10_ANALOG_ENABLE();
    PB01_ANALOG_ENABLE();
    PB11_ANALOG_ENABLE();
    PB00_ANALOG_ENABLE();

    ADC_InitTypeDef ADC_InitStructure;
    ADC_StructInit(&ADC_InitStructure);
    ADC_InitStructure.ADC_ClkDiv = ADC_Clk_Div4;
    ADC_InitStructure.ADC_VrefSel = ADC_Vref_BGR1p5;
    ADC_InitStructure.ADC_SampleTime = ADC_SampTime10Clk;

    ADC_SerialChTypeDef ADC_SerialChStructure;
    // ADC_SerialChStructure.ADC_SqrEns = ADC_SqrEns0;
    ADC_SerialChStructure.ADC_Sqr0Chmux = ADC_SqrCh11;
    ADC_SerialChStructure.ADC_Sqr1Chmux = ADC_SqrCh12;
    ADC_SerialChStructure.ADC_SqrEns = ADC_SqrEns01;
    ADC_SerialChStructure.ADC_InitStruct = ADC_InitStructure;

    ADC_SerialChContinuousModeCfg(&ADC_SerialChStructure);
    ADC_ClearITPendingAll();
    ADC_Enable();
    ADC_SoftwareStartConvCmd(ENABLE);
}

// 连续获取电压值
void GetVoltagContinue(uint16_t *buff)
{
    for (uint8_t i = 0; i < ADC_SAMPLE_SIZE; i++)
    {
        ADC_GetSqr0Result(buff + i);
    }
}
// 连续获取电流
void GetCurrentContinue(uint16_t *buff)
{
    for (uint8_t i = 0; i < ADC_SAMPLE_SIZE; i++)
    {
        ADC_GetSqr1Result(buff + i);
    }
}

// 均值滤波
uint32_t mean_value_filter(uint16_t *value, uint16_t size)
{
    uint32_t sum;
    uint16_t max;
    uint16_t min = 0xffff;
    for (int i = 0; i < size; i++)
    {
        sum += value[i];
        if (value[i] > max)
            max = value[i];
        else if (value[i] < min)
            min = value[i];
        else
            ;
    }
    sum -= (max + min);
    sum = sum / (size - 2);
    return sum;
}
