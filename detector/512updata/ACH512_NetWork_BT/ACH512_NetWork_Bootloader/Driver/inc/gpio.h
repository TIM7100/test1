/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : gpio.h
 * Description : gpio driver header file
 * Author(s)   : Eric
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#ifndef __GPIO_H__
#define __GPIO_H__

#include  "common.h"


#define DS_H        (REG_GPIO_SET(GPIOA) |= (1U << 31))  //GPIO31
#define DS_L        (REG_GPIO_CLR(GPIOA) |= (1U << 31))  //GPIO31
#define STCP_H      (REG_GPIO_SET(GPIOB) |= (1U << 1))   //GPIO33
#define STCP_L      (REG_GPIO_CLR(GPIOB) |= (1U << 1))   //GPIO33
#define SHCP_H      (REG_GPIO_SET(GPIOB) |= (1U << 2))   //GPIO34
#define SHCP_L      (REG_GPIO_CLR(GPIOB) |= (1U << 2))   //GPIO34


#define BEEP_ON      (REG_GPIO_SET(GPIOA) |= (1U << 29)); //GPIO29
#define BEEP_OFF     (REG_GPIO_CLR(GPIOA) |= (1U << 29)); //GPIO29

void Multi_LEDInit(void);
void LED_SendBytes(uint8_t dat_first,uint8_t dat_second);
void LED_DisPlay(u8 location,u8 sta);
void LED_Update(void);
void Beep_Init(void);
void Beeptest(void);

#endif

