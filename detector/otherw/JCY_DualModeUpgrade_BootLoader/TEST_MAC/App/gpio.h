#ifndef __APPGPIO_H__
#define __APPGPIO_H__

#include "crc.h"
#include "fxx_std.h"
#include "delay.h"
#include "lcd.h"



#define KEY 0x1E000000
#define READ_Enter   GPIO_ReadInputDataBit(GPIOD,GPIO_PIN_12)
#define READ_Esc     GPIO_ReadInputDataBit(GPIOD,GPIO_PIN_11)
#define READ_Up      GPIO_ReadInputDataBit(GPIOD,GPIO_PIN_10)
#define READ_Down    GPIO_ReadInputDataBit(GPIOD,GPIO_PIN_9)

#define Key_Enter   ((u32)KEY & (~0x10000000))
#define Key_Esc     ((u32)KEY & (~0x08000000))
#define Key_Up      ((u32)KEY & (~0x04000000))
#define Key_Down    ((u32)KEY & (~0x02000000))

enum PowerValue
{
    V3_3	,
    V5		,
};


void KeyInit(void);

u32 WaitKey(void);




//#define SPEEKER_UP

#define KEY1_IRQHandler                     EXTI_IRQHandler

#define ENTER_READ()      GPIO_ReadInputDataBit(GPIOD,GPIO_PIN_12)



void LedInit(void);

void BuzzerInit(void);

void BuzzerSound(u8 Times);

void IOPowerSelect(enum PowerValue w);

#endif
