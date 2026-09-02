#ifndef _DELAY_H
#define _DELAY_H

#include "ach512.h"

/*函数功能: 初始化Timer0作为Delay延时的定时器使用  */
void Timer0DelayInit(void);

/*函数功能: Ms(毫秒)延时函数                       */
void Timer0DelayMs(UINT32 DelayMs);

/*函数功能: Us(微秒)延时函数                       */
void Timer0DelayUs(UINT32 DelayUs);

void Timer0Delay2_5Us(void);

void Timer0Delay5Us(void);

#endif
