#include "bsp_key.h"

uint8_t FSM_Scan_Count;

KEY_t KEY[KEY_COUNT];

/**
 * @brief 按键状态扫描服务
 *
 */
void FSM_KeyScanServer()
{
    if (FSM_Scan_Count >= TICKS_INTERVAL) // 默认每次进入为10ms
    {
        FSM_Scan_Count = 0;
        for (uint8_t i = 0; i < KEY_COUNT; i++)
        {
            /**
             * @brief 此部分为按键状态的扫描
             *
             */
            if (KEY[i].ReadKeyValue == NULL)
                continue;
            switch (KEY[i].FSM_value.state)
            {
            case FSM_KEY_Up: // 如果按键按下，则进入按键按下抖动状态，否则为按键释放状态
                if (KEY[i].ReadKeyValue() == KEY[i].key_pressLevel)
                    KEY[i].FSM_value.state = FSM_KEY_DownShake;
                else
                {
                    KEY[i].key_state = KEY_STATE_RELEASE;
                }
                break;
            case FSM_KEY_DownShake: // 按键按下抖动状态，此时为经过一次时间延迟，如果按键为
                if (KEY[i].ReadKeyValue() == KEY[i].key_pressLevel)
                    KEY[i].FSM_value.state = FSM_KEY_Down;
                else
                    KEY[i].FSM_value.state = FSM_KEY_Up;
                break;
            case FSM_KEY_Down:
                if (KEY[i].ReadKeyValue() == KEY[i].key_pressLevel)
                {
                    KEY[i].key_state = KEY_STATE_PRESS;
                }
                else
                    KEY[i].FSM_value.state = FSM_KEY_UpShake;
                break;
            case FSM_KEY_UpShake:
                if (KEY[i].ReadKeyValue() == KEY[i].key_pressLevel)
                    KEY[i].FSM_value.state = FSM_KEY_Down;
                else
                    KEY[i].FSM_value.state = FSM_KEY_Up;
                break;
            default:
                break;
            }
            /**
             * @brief 此部分为按键事件的检测，如单击，双击等，并调用相应的回调函数
             *
             */
            switch (KEY[i].FSM_value.even_state)
            {
            case FSM_EVEN_INIT: // 初始状态
                if (KEY[i].key_state == KEY_STATE_PRESS)
                {
                    KEY[i].FSM_value.even_state = FSM_EVEN_PRESS;
                    KEY[i].EvenCallBack(KEY_EVEN_PRESS, NULL); // 出发按键按下事件
                }
                else
                    KEY[i].FSM_value.even_state = FSM_EVEN_INIT;
                break;

            case FSM_EVEN_PRESS:
                if (KEY[i].key_state == KEY_STATE_PRESS)
                {
                    if (KEY[i].FSM_value.cnt++ > (200 / TICKS_INTERVAL)) // 如果按下，进入长按状态
                    {
                        KEY[i].FSM_value.cnt = 0;
                        KEY[i].FSM_value.even_state = FSM_EVEN_LONGPRESS;
                        KEY[i].EvenCallBack(KEY_EVEN_LONGPRESS_STAR, NULL); // 出发长按开始事件
                    }
                }
                else
                {
                    KEY[i].FSM_value.click_times++;
                    KEY[i].FSM_value.even_state = FSM_EVEN_CLICK; // 否则进入点击状态
                    KEY[i].EvenCallBack(KEY_EVEN_RELEASE, NULL);
                }

                break;

            case FSM_EVEN_CLICK:

                if (KEY[i].key_state == KEY_STATE_PRESS)
                {
                    KEY[i].FSM_value.cnt = 0;
                    KEY[i].FSM_value.even_state = FSM_EVEN_REPEATDOWN; // 如果按下，进入连击状态
                }
                else // 否则根据条件，触发连击事件或者点击事件
                {
                    if (KEY[i].FSM_value.cnt++ >= SHORT_TICKS)
                    {
                        KEY[i].FSM_value.cnt = 0;
                        KEY[i].FSM_value.even_state = FSM_EVEN_INIT;

                        if (KEY[i].FSM_value.click_times > 1)
                            KEY[i].EvenCallBack(KEY_EVEN_REPEAT, (void *)&KEY[i].FSM_value.click_times);
                        else
                            KEY[i].EvenCallBack(KEY_EVEN_CLICK, NULL);
                        KEY[i].FSM_value.click_times = 0;
                    }
                }

                break;
            case FSM_EVEN_REPEATDOWN:
                if (KEY[i].key_state == KEY_STATE_PRESS)
                {

                    if (KEY[i].FSM_value.cnt++ >= LONG_TICKS / 2)
                    {
                        KEY[i].FSM_value.click_times++;
                        KEY[i].EvenCallBack(KEY_EVEN_REPEAT, (void *)&KEY[i].FSM_value.click_times);
                        KEY[i].FSM_value.click_times = 0;
                        KEY[i].FSM_value.even_state = FSM_EVEN_LONGPRESS;
                        KEY[i].FSM_value.cnt = 0;
                    }
                }
                else
                {
                    KEY[i].FSM_value.even_state = FSM_EVEN_CLICK;
                    KEY[i].FSM_value.click_times++;
                }

                break;
            case FSM_EVEN_LONGPRESS:
                if (KEY[i].key_state == KEY_STATE_PRESS)
                {
                    if (KEY[i].FSM_value.cnt++ >= LONG_TICKS)
                    {
                        KEY[i].FSM_value.cnt = 0;
                        KEY[i].EvenCallBack(KEY_EVEN_LONGPRESS_HOLD, NULL);
                    }
                }
                else
                {
                    KEY[i].FSM_value.even_state = FSM_EVEN_INIT;
                    KEY[i].FSM_value.cnt = 0;
                    KEY[i].EvenCallBack(KEY_EVEN_RELEASE, NULL);
                }

                break;

            default:
                KEY[i].FSM_value.even_state = FSM_EVEN_INIT;
                break;
            }
        }
    }
}

/**
 * @brief 按键扫描tick，需在定时器或其他任务中运行，为按键扫描提供心跳
 *
 * @param ticks 每次执行改函数所经历的时间,单位ms
 */
void FSM_KeyScanHeadler(uint8_t ticks)
{
    FSM_Scan_Count += ticks;
}

void KEY_defconfig(KEY_t *keys)
{
    for (int i = 0; i < KEY_COUNT; i++)
    {
        keys[i].FSM_value.state = FSM_KEY_Up;
        keys[i].key_pressLevel = KEY_LEVEL_LOW;
        keys[i].key_state = KEY_STATE_RELEASE;
    }
}
void KEY_Attach(KEY_LIST keyid, void (*EVEN)(KEY_EVEN, void *))
{
    KEY[keyid].EvenCallBack = EVEN;
}

/// @brief 需要完成每个按键的读取点评函数
/// @return none

uint32_t KEY_LeftRead()
{
    return GPIO_ReadPin(KEY_PORT, KEY1_PIN);
}

uint32_t KEY_OkRead()
{
    return GPIO_ReadPin(KEY_PORT, KEY2_PIN);
}
uint32_t KEY_RightRead()
{
    return GPIO_ReadPin(KEY_PORT, KEY3_PIN);
}

void KEY_Init()
{
    RCC_AHBPeriphClk_Enable(RCC_AHB_PERIPH_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure = {
        .Pins = KEY1_PIN | KEY2_PIN | KEY3_PIN,
        .Mode = GPIO_MODE_INPUT,
        .Speed = GPIO_SPEED_HIGH};
    GPIO_Init(KEY_PORT, &GPIO_InitStructure);
    KEY_defconfig(KEY);
    KEY[KEY_LEFT].ReadKeyValue = KEY_LeftRead;
    KEY[KEY_RIGHT].ReadKeyValue = KEY_RightRead;
    KEY[KEY_OK].ReadKeyValue = KEY_OkRead;
}