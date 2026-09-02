#ifndef __DELAY_H__
#define __DELAY_H__

#include "fxx_std.h"
#include "crc.h"

#define TIM_CLOCK_FREQ            (10000000)

void TIM6_Init(void);

void NVIC_Configuration(void);

void Clock_Configuration(void);


void Delay_us(uint16_t count);

void Delay_ms(uint16_t count);

void Delay_s(void);

void Fiic_delay_ms(uint32_t count);

void Fiic_delay_us(uint32_t count);

void TimeDelay(uint16_t Time);

#endif