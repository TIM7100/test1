#include "delay.h"

TIM_Base_InitTypeDef TIM_TimeBaseStructure;

void TIM6_Init(void)
{
    uint32_t timer_clock;

    timer_clock = System_Get_APBClock();

    if (System_Get_SystemClock() != System_Get_APBClock())  // if hclk/pclk != 1, then timer clk = pclk * 2
    {
        timer_clock =  System_Get_APBClock() << 1;    //系统频率为180mhz
    }
    TIM6->CR1 &= ~(0X03);
    TIM_TimeBaseStructure.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    TIM_TimeBaseStructure.CounterMode   = TIM_COUNTERMODE_DOWN ;
    TIM_TimeBaseStructure.Prescaler = (timer_clock / TIM_CLOCK_FREQ) - 1;
    TIM_TimeBaseStructure.Period = (TIM_CLOCK_FREQ / 1000000) - 1; // 1us
    TIM_TimeBase_Init(TIM6, &TIM_TimeBaseStructure);


}
void NVIC_Configuration(void)
{
    NVIC_ClearPendingIRQ(TIM6_IRQn);
    NVIC_EnableIRQ(TIM6_IRQn);
}

void Clock_Configuration(void)
{
    System_Module_Enable(EN_TIM6);
}

void Delay_ms(uint16_t count)
{
    TIM_TimeBaseStructure.Prescaler = 18000 - 1;
    TIM_TimeBaseStructure.Period =  (10 * count) - 1;
    TIM_TimeBase_Init(TIM6, &TIM_TimeBaseStructure);

    TIM6->CR1 |= BIT0;
    while ((TIM6->SR & 0x01) != SET);
    TIM6->SR = (uint16_t)~0x01;
    TIM6->CR1 &= (uint16_t)(~((uint16_t)BIT0));
    TIM6->CNT = 0X00;
}

void Delay_s(void)
{
    TIM_TimeBaseStructure.Prescaler = 18000-1;
    TIM_TimeBaseStructure.Period = 10000 - 1;
    TIM_TimeBase_Init(TIM6, &TIM_TimeBaseStructure);

    TIM6->CR1 |= BIT0;
    while ((TIM6->SR & 0x01) != SET);
    TIM6->SR = (uint16_t)~0x01;
    TIM6->CR1 &= (uint16_t)(~((uint16_t)BIT0));
    TIM6->CNT = 0X00;
}



void Delay_us(uint16_t count)
{
    TIM_TimeBaseStructure.Prescaler = (180000000 / TIM_CLOCK_FREQ) - 1;
    TIM_TimeBaseStructure.Period = (TIM_CLOCK_FREQ / 1000000) * count - 1;
    TIM_TimeBase_Init(TIM6, &TIM_TimeBaseStructure);

    TIM6->CR1 |= BIT0;
    while ((TIM6->SR & 0x01) != SET);
    TIM6->SR = (uint16_t)~0x01;
    TIM6->CR1 &= (uint16_t)(~((uint16_t)BIT0));
    TIM6->CNT = 0X00;
}

void TimeDelay(uint16_t Time)
{
    while (Time--);
}

/************************************************************************
 * function   : 延时函数
 * Description: us延时
 ************************************************************************/
void Fiic_delay_ms(uint32_t count)
{		  
	
	TIM_TimeBaseStructure.Period =(TIM_CLOCK_FREQ/1000)*count - 1; 
	TIM_TimeBase_Init(TIM1, &TIM_TimeBaseStructure);

	TIM1->CR1 |= BIT0;   	
//	printf("%d",TIM1->CNT);
	while((TIM1->SR & 0x01)!=SET);
	TIM1->SR = (uint16_t)~0x01;	
	
	TIM1->CR1 &= (uint16_t)(~((uint16_t)BIT0));  	
	TIM1->CNT=0X00;

}


/************************************************************************
 * function   : 延时函数
 * Description: us延时
 ************************************************************************/
void Fiic_delay_us(uint32_t count)
{		  
	
	TIM_TimeBaseStructure.Period =(TIM_CLOCK_FREQ/1000000)*count - 1; 
	TIM_TimeBase_Init(TIM1, &TIM_TimeBaseStructure);

	TIM1->CR1 |= BIT0;   	
//	printf("%d",TIM1->CNT);
	while((TIM1->SR & 0x01)!=SET);
	TIM1->SR = (uint16_t)~0x01;	
	
	TIM1->CR1 &= (uint16_t)(~((uint16_t)BIT0));  	
	TIM1->CNT=0X00;

}