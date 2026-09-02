/*
  ******************************************************************************
  * @file    APP.h
  * @author  Chris_Kyle
  * @version V1.0.0
  * @date    2020
  * @brief   GPIO demo Header file.
  ******************************************************************************
*/
#ifndef __APP_H__
#define __APP_H__

#include "fxx_std.h"



#define  GPIO_LED_Port                      GPIOF
#define  GPIO_LED_Pin                       GPIO_PIN_3
#define  GPIO_LED_CLK                       EN_GPIOEF


#define  GPIO_IT_Port                      GPIOB
#define  GPIO_IT_Pin                       GPIO_PIN_9
#define  GPIO_IT_CLK                       EN_GPIOAB

#define  GPIO_IT_IRQ                       GPIOAB_IRQn
#define  GPIO_IT_IRQHandler                GPIOAB_IRQHandler

typedef enum
{
    GPIO_INT,
    LED_BLINK,
	GPIO_PC13
}enum_Mode_t;

void APP_GPIO_Test(enum_Mode_t fe_Mode);

#endif
