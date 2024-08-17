#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "board.h"

#define KEY_PORT CW_GPIOB
#define KEY1_PIN GPIO_PIN_12
#define KEY2_PIN GPIO_PIN_13
#define KEY3_PIN GPIO_PIN_14

#define TICKS_INTERVAL 10                  // ms            // 心跳时钟
#define SHORT_TICKS (200 / TICKS_INTERVAL) // 短按时间
#define LONG_TICKS (600 / TICKS_INTERVAL)  // 长按时间

/// @brief 按键相关
typedef enum
{
    KEY_STATE_RELEASE = 0,
    KEY_STATE_PRESS,
} KEY_STATE;

typedef enum
{
    KEY_LEVEL_LOW = 0,
    KEY_LEVEL_HIGH
} KEY_LEVEL;

typedef enum
{
    KEY_EVEN_NULL = 0,
    KEY_EVEN_PRESS,
    KEY_EVEN_RELEASE,
    KEY_EVEN_CLICK,
    KEY_EVEN_REPEAT,
    KEY_EVEN_LONGPRESS_STAR,
    KEY_EVEN_LONGPRESS_HOLD,

} KEY_EVEN;

typedef enum KEY_LIST_t
{
    KEY_LEFT = 0,
    KEY_OK,
    KEY_RIGHT,

    KEY_COUNT // 按键数量，需固定保留，第一个按键枚举需定为0
} KEY_LIST;

/// @brief FSM相关

typedef enum
{
    FSM_KEY_Up = 0,    // 按键释放
    FSM_KEY_DownShake, // 按键按压抖动
    FSM_KEY_Down,      // 按键按压状态
    FSM_KEY_UpShake,   // 按键释放抖动
} FSM_State_t;

typedef enum
{
    FSM_EVEN_INIT = 0,
    FSM_EVEN_PRESS,
    FSM_EVEN_CLICK,
    FSM_EVEN_REPEATDOWN,
    FSM_EVEN_LONGPRESS

} FSM_EVEN_State_t;

typedef struct
{
    FSM_State_t state; // 按键状态
    FSM_EVEN_State_t even_state;
    uint8_t volatile cnt;         // 定时器
    uint8_t volatile click_times; // 连按次数
} FSM_value_t;

typedef struct
{
    KEY_LEVEL key_pressLevel;
    FSM_value_t FSM_value;
    KEY_STATE key_state;
    void (*EvenCallBack)(KEY_EVEN, void *);
    uint32_t (*ReadKeyValue)(void);
} KEY_t;

extern KEY_t KEY[KEY_COUNT];
void KEY_Init();

void FSM_KeyScanServer();
// void FSM_KeyActionServer();
void KEY_Attach(KEY_LIST keyid, void (*EVEN)(KEY_EVEN, void *));
void FSM_KeyScanHeadler(uint8_t ticks);

#endif // !__BSP_KEY_H
