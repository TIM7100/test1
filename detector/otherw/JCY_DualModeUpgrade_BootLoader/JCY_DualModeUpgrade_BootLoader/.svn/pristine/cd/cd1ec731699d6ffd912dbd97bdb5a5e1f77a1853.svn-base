#include "fxx_tim.h"
#include "delay.h"


void delay_us(uint32_t Times)
{
    TIM6->PSC = 20 - 1;             //9MHz
    TIM6->ARR = 9 * Times - 1;

    TIM6->EGR |= 1;     //更新计数器值
    TIM6->SR &= ~0x01;  //清除标志位

    TIM6->CR1 |= 1;     //使能计数器
    while ((TIM6->SR & 0x01) != 1);
    TIM6->CR1 &= ~0x01;         //失能计数器
}



void delay_ms(uint32_t Times)
{
    TIM6->PSC = 20000 - 1;              //9KHz
    TIM6->ARR = 9 * Times - 1;

    TIM6->EGR |= 1;     //更新计数器值
    TIM6->SR &= ~0x01;  //清除标志位

    TIM6->CR1 |= 1;     //使能计数器
    while ((TIM6->SR & 0x01) != 1);
    TIM6->CR1 &= ~0x01;         //失能计数器
}




