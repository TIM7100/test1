/*
  ******************************************************************************
  * @file    APP.c
  * @author  Chris_Kyle
  * @version V1.0.0
  * @date    2020
  * @brief   GPIO demo source code.
  ******************************************************************************
*/

#include "app.h"

GPIO_InitTypeDef GPIO_LED_Handle;

GPIO_InitTypeDef GPIO_IT_Handle;  
GPIO_InitTypeDef GPIO_PC13_Handle;  

volatile uint32_t gu32_GPIOIRQ_Flag = false; 
/*********************************************************************************
* Function    : GPIO_LED_Test
* Description : GPIO LED Test
* Input       : 
* Output       : 
**********************************************************************************/
void GPIO_LED_Test()
{
    printfS("This is LED Blinking TEST \r\n");
    System_Module_Enable(GPIO_LED_CLK);
    
    GPIO_LED_Handle.Pin       = GPIO_LED_Pin;
    GPIO_LED_Handle.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_LED_Handle.Pull      = GPIO_PULLUP;
    GPIO_LED_Handle.Alternate = GPIO_FUNCTION_0;
    GPIO_Init(GPIO_LED_Port, &GPIO_LED_Handle);
    
    while (1) 
    {
        GPIO_WriteBit(GPIOF, GPIO_PIN_3,Bit_RESET);
        System_Delay_MS(500);
        GPIO_WriteBit(GPIOF, GPIO_PIN_3,Bit_SET);
        System_Delay_MS(500);
    }
}

/*********************************************************************************
* Function    : GPIO_IT_Test
* Description : GPIO IT TEST
* Input       : 
* Output       : 
**********************************************************************************/
void GPIO_IT_Test()
{
    printfS("This is GPIO interrupt TEST. Please press user button to generate interrupt! \r\n");   
    System_Module_Enable(GPIO_IT_CLK);

    GPIO_IT_Handle.Pin       = GPIO_IT_Pin;
    GPIO_IT_Handle.Mode      = GPIO_MODE_IT_FALLING;  
    GPIO_IT_Handle.Pull      = GPIO_PULLUP;
    GPIO_IT_Handle.Alternate = GPIO_FUNCTION_0;

    GPIO_Init(GPIO_IT_Port, &GPIO_IT_Handle);
    
    GPIO_ITConfig(GPIO_IT_Port, GPIO_IT_Pin, ENABLE);
    
    /* Clear Pending Interrupt */
    NVIC_ClearPendingIRQ(GPIO_IT_IRQ);

    /* Enable External Interrupt */
    NVIC_EnableIRQ(GPIO_IT_IRQ);
    while (1) 
    {
        if (gu32_GPIOIRQ_Flag) 
        {
            gu32_GPIOIRQ_Flag = false;

            printfS("Get interrupt flag!!! \r\n");    
        }
    }
}

/*********************************************************************************
* Function    : GPIO_PC13_Test
* Description : GPIO_PC13_Test
* Input       : 
* Output      : 
**********************************************************************************/
void GPIO_PC13_Test(void)
{
	   /* 注意：配置 GPIO PC13、PC14、PC15 管脚的数模、上下拉、驱动能力 使用PMU 域寄存器配置 */
		printfS("This is GPIO PC13 interrupt TEST \r\n");
	    System_Module_Enable(EN_GPIOCD);

		GPIO_PC13_Handle.Pin       = GPIO_PIN_13;
		GPIO_PC13_Handle.Mode      = GPIO_MODE_IT_FALLING;	
		GPIO_PC13_Handle.Alternate = GPIO_FUNCTION_0;

		GPIO_Init(GPIOC, &GPIO_PC13_Handle);
	
	    GPIO_ITConfig(GPIOC, GPIO_PIN_13, ENABLE);

		/* RTC access enable */
		System_Enable_Disable_RTC_Domain_Access(FUNC_ENABLE);  
		
		__HAL_RTC_PC13_SEL(0);  // GPIO function   
		__HAL_RTC_PC13_PULL_UP_ENABLE();
		__HAL_RTC_PC13_DIGIT();  

		/* Clear Pending Interrupt */
		NVIC_ClearPendingIRQ(GPIOCD_IRQn);
		
		/* Enable External Interrupt */
		NVIC_EnableIRQ(GPIOCD_IRQn);
		
		while (1) 
		{
			if (gu32_GPIOIRQ_Flag) 
			{
				gu32_GPIOIRQ_Flag = false;
				
				printfS("Get PC13 interrupt flag!!! \r\n");
			}
		}


}

/*********************************************************************************
* Function    : APP_GPIO_Test
* Description : APP GPIO Test
* Input       : 
* Output       : 
**********************************************************************************/
void APP_GPIO_Test(enum_Mode_t fe_Mode)
{
    switch (fe_Mode)
    {
        case GPIO_INT: 
        {   
            
            GPIO_IT_Test();
            
        }break;
        
        case LED_BLINK: 
        {    
            
            GPIO_LED_Test();
            
        }break;
        case GPIO_PC13:
		{		
			GPIO_PC13_Test();
		
		}break;
			
        default: break; 
    }
}

