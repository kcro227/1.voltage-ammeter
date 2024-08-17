#ifndef __BSP_LOG_H
#define __BSP_LOG_H

#include "bsp_uart.h"


// LOG Font color
#define COLOR_START "\033["
#define COLOR_STOP "\033[0m"
#define COLOR_TYPE_BOLD "1m"
#define COLOR_TYPE_UNDERLINE "4m"
#define COLOR_TYPE_BLINK "5m"
#define COLOR_TYPE_INVERSE "7m"
#define COLOR_TYPE_INVISIBLE "8m"
#define COLOR_TYPE_NONE "2m"
 
#define COLOR_BG_BLACK    "40;"
#define COLOR_BG_RED    "41;"
#define COLOR_BG_GREEN    "42;"
#define COLOR_BG_BROWN    "43;"
#define COLOR_BG_BLUE    "44;"
#define COLOR_BG_PURPLE    "45;"
#define COLOR_BG_BLUE2    "46;"
#define COLOR_BG_GREY    "47;"
 
#define COLOR_FG_BLACK    "30;"
#define COLOR_FG_RED    "31;"
#define COLOR_FG_GREEN    "32;"
#define COLOR_FG_YELLOW    "33;"
#define COLOR_FG_BLUE    "34;"
#define COLOR_FG_PURPLE    "35;"
#define COLOR_FG_BLUE2    "36;"
#define COLOR_FG_WHITE    "37;"

// LOG LEVEL 0:only error 1: error and warn  2: error warn info 3:all
#define LOG_LEVEL 3

#define LOG_ERROR(format, ...) printf(COLOR_START COLOR_FG_RED COLOR_TYPE_BOLD "[%s:%d->%s] " format "\r\n" COLOR_STOP, __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#if (LOG_LEVEL > 0)
#define LOG_WARN(format, ...) printf(COLOR_START COLOR_FG_YELLOW COLOR_TYPE_BOLD "[%s:%d->%s] " format "\r\n" COLOR_STOP, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#else
#define LOG_WARN(format, ...)
#endif

#if (LOG_LEVEL > 1)
#define LOG_INFO(format, ...) printf(COLOR_START COLOR_FG_WHITE COLOR_TYPE_BOLD "[%s:%d->%s] " format "\r\n" COLOR_STOP, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#else
#define LOG_INFO(format, ...)
#endif

#if (LOG_LEVEL > 2)
#define LOG_DEBUG(format, ...)  printf(COLOR_START COLOR_FG_GREEN COLOR_TYPE_BOLD format "\r\n" COLOR_STOP ,##__VA_ARGS__)
#else 
#define LOG_DEBUG(format, ...)
#endif 



#endif // !__BSP_LOG_H
