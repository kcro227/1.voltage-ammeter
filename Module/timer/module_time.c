#include "module_time.h"

#include "bsp_key.h"

void BTIME1_InitFor1ms()
{
    __RCC_BTIM_CLK_ENABLE();

    __disable_irq();
    NVIC_EnableIRQ(BTIM1_IRQn);
    __enable_irq();

    BTIM_TimeBaseInitTypeDef BTIM_TimeBaseInitStruct = {
        .BTIM_Mode = BTIM_Mode_TIMER,
        .BTIM_OPMode = BTIM_OPMode_Repetitive,
        .BTIM_Period = 8192,
        .BTIM_Prescaler = BTIM_PRS_DIV8};

    BTIM_TimeBaseInit(CW_BTIM1, &BTIM_TimeBaseInitStruct);
    // 使能BTIM1的溢出中断
    BTIM_ITConfig(CW_BTIM1, BTIM_IT_OV, ENABLE);

    // 启动定时器BTIM1
    BTIM_Cmd(CW_BTIM1, ENABLE);
}
uint16_t t;
extern uint8_t adc_en;
void BTIM1_IRQHandler(void)
{
    // 判断是否为溢出中断
    if (BTIM_GetITStatus(CW_BTIM1, BTIM_IT_OV))
    {
        // 清除溢出中断标志
        BTIM_ClearITPendingBit(CW_BTIM1, BTIM_IT_OV);

        // 打印信息到串口，用于调试观察中断触发
        FSM_KeyScanHeadler(1);
        if(++t == 500)
        {
            t = 0;
            adc_en = 1;
        }
    }
}