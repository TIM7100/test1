/*
  ******************************************************************************
  * @file    fxx_it.c
  * @brief   fxx_it source File.
  ******************************************************************************
*/

#include "fxx_it.h"
#include "app.h"

extern volatile uint32_t gu32_GPIOIRQ_Flag; 

void GPIO_IT_IRQHandler(void)
{
    if(GPIO_GetITStatus(GPIO_IT_Port, GPIO_IT_Pin))
    {
        gu32_GPIOIRQ_Flag = true; 
        GPIO_ITClear(GPIO_IT_Port, GPIO_IT_Pin);
    }
    
}

void GPIOCD_IRQHandler(void)
{
    if(GPIO_GetITStatus(GPIOC, GPIO_PIN_13))
	{
		gu32_GPIOIRQ_Flag = true; 
        GPIO_ITClear(GPIOC, GPIO_PIN_13);
	}                
}



