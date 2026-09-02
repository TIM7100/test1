#ifndef __TIMER7_H__
#define __TIMER7_H__
#include "f4_f3.h"
#include "fxx_tim.h"
extern uint8_t Tim7_Update_Flag;
void Timer7_Init(void);
void Timer7_SetUs(uint32_t us);
void Timer7_Start(void);
void Timer7_Stop(void);
#endif

