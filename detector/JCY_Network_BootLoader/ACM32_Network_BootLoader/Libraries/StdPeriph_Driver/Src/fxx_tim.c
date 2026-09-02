/*
  ******************************************************************************
  * @file    fxx_tim.c
  * @version V1.0.0
  * @date    2021
  * @brief   timer module driver.  
  ******************************************************************************
*/
#include "fxx_std.h"


/*********************************************************************************
  * @brief  Timer base initiation, like period, count mode.
  * @param  TIMx: TIM1/TIM2/TIM3... 
  * @param  TIM_TimeBaseInitStruct: point to timer initiation strcture.    
  * @retval None                     
**********************************************************************************/  	
void TIM_TimeBase_Init(TIM_TypeDef *TIMx, TIM_Base_InitTypeDef * TIM_TimeBaseInitStruct)
{
	TIMx->CR1 = BIT2;  // CEN=0, URS=1, OPM = 0   
			
	TIMx->ARR = TIM_TimeBaseInitStruct->Period; 
	TIMx->PSC = TIM_TimeBaseInitStruct->Prescaler;   
    
	if (IS_TIM_REPETITION_COUNTER_INSTANCE(TIMx))
	{
		TIMx->RCR = TIM_TimeBaseInitStruct->RepetitionCounter;   
	}

	if (IS_TIM_CLOCK_DIVISION_INSTANCE(TIMx))  
	{
		TIMx->CR1 = (TIMx->CR1 & (~(BIT8|BIT9))) | ((TIM_TimeBaseInitStruct->ClockDivision) & (BIT8|BIT9));     
	}
	//up/down/center mode 
	TIMx->CR1 = (TIMx->CR1 & (~(BIT4|BIT5|BIT6))) | ((TIM_TimeBaseInitStruct->CounterMode) & (BIT4|BIT5|BIT6));    
	
    TIMx->EGR = TIM_ReloadMode_Immediate;  
    
	TIMx->CR1 &= (~BIT2); //URS = 0     
	
	return;  
} 


/*============================================================================================
  * @brief  Enables or disables the specified TIM interrupts.
  * @param  TIMx: TIM1/TIM2/TIM3... 
  * @param  TIM_IT: specifies the TIM interrupts sources to be enabled or disabled.
  *   This parameter can be any combination of the following values:
  *     @arg TIM_IT_Update: TIM update Interrupt source
  *     @arg TIM_IT_CC1: TIM Capture Compare 1 Interrupt source
  *     @arg TIM_IT_CC2: TIM Capture Compare 2 Interrupt source
  *     @arg TIM_IT_CC3: TIM Capture Compare 3 Interrupt source
  *     @arg TIM_IT_CC4: TIM Capture Compare 4 Interrupt source
  *     @arg TIM_IT_COM: TIM Commutation Interrupt source
  *     @arg TIM_IT_Trigger: TIM Trigger Interrupt source
  *     @arg TIM_IT_Break: TIM Break Interrupt source    
  * @param  NewState: new state of the TIM interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
==============================================================================================*/
void TIM_ITConfig(TIM_TypeDef* TIMx, uint16_t TIM_IT, FunctionalState NewState)
{  
  
  if (NewState != DISABLE)
  {
    /* Enable the Interrupt sources */
    TIMx->DIER |= TIM_IT;
  }
  else 
  {
    /* Disable the Interrupt sources */
    TIMx->DIER &= (uint16_t)~TIM_IT;
  }
}  


/*==============================================================================
  * @brief  Enables or disables the specified TIM peripheral.
  * @param  TIMx: where x can be 1 to 17 to select the TIMx peripheral.
  * @param  NewState: new state of the TIMx peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
=====================================================================================*/
void TIM_Cmd(TIM_TypeDef* TIMx, FunctionalState NewState)
{ 
  if (NewState != DISABLE)
  {
    /* Enable the TIM Counter */
    if (0 == IS_TIM_SLAVE_INSTANCE(TIMx) )     
	{
		TIMx->CR1 |= BIT0;    
		return;    
	}  
    
    if (TIM_SLAVE_MODE_TRIG != (TIMx->SMCR & (BIT0|BIT1|BIT2) ) )  
	{
		TIMx->CR1 |= BIT0;      
		return;        
	}
    
  }
  else
  {
    /* Disable the TIM Counter */
    TIMx->CR1 &= (uint16_t)(~((uint16_t)BIT0));   
  }
}  

/*==================================================================================================
  * @brief  Initializes the TIMx Channel1 according to the specified
  *         parameters in the TIM_OCInitStruct.
  * @param  TIMx: where x can be 1, 2, 3, 4, 14, 15, 16, 17 to select 
  *         the TIM peripheral.
  * @param  TIM_OCInitStruct: pointer to a TIM_OCInitTypeDef structure
  *         that contains the configuration information for the specified TIM peripheral.
  * @retval None
=======================================================================================================*/
void TIM_OC1Init(TIM_TypeDef* TIMx, TIM_OC_InitTypeDef* TIM_OCInitStruct)
{

	TIMx->CCER &= (~BIT0);  //disable OC1 
	if (OUTPUT_POL_ACTIVE_HIGH == TIM_OCInitStruct->OCPolarity)
	{
		TIMx->CCER &= (~BIT1);  
	}
	else
	{
		TIMx->CCER |= (BIT1);   
	}
		
	if (IS_TIM_CCXN_INSTANCE(TIMx, TIM_CHANNEL_1) )  
	{
		TIMx->CCER &= (~BIT2);  //disable OC1N  
			
		if (OUTPUT_POL_ACTIVE_HIGH == TIM_OCInitStruct->OCNPolarity)   
		{
			TIMx->CCER &= (~BIT3);  
		}
		else
		{
			TIMx->CCER |= (BIT3);     
		}  
	}
		
	TIMx->CCMR1 &= (~0x00FFU); // reset low 8 bits   
	TIMx->CCR1 = TIM_OCInitStruct->Pulse;   
	if (OUTPUT_FAST_MODE_ENABLE == TIM_OCInitStruct->OCFastMode)  
	{
		TIMx->CCMR1 |= (BIT2);  
	}
	TIMx->CCMR1 |= (BIT3);  // preload enable 	
		
	if (IS_TIM_BREAK_INSTANCE(TIMx)) 
	{
		if (OUTPUT_IDLE_STATE_0 == TIM_OCInitStruct->OCIdleState)
		{
			TIMx->CR2 &= (~BIT8); 
		}
		else
		{
			TIMx->CR2 |= BIT8; 
		}
			
		if (OUTPUT_IDLE_STATE_0 == TIM_OCInitStruct->OCNIdleState)  
		{
			TIMx->CR2 &= (~BIT9); 
		}
		else
		{
			TIMx->CR2 |= BIT9;   
		}  
			
	}
	TIMx->CCMR1 = (TIMx->CCMR1 & (~(BIT4|BIT5|BIT6))) | (TIM_OCInitStruct->OCMode << 4);   
	
}   

/*==================================================================================================
  * @brief  Initializes the TIMx Channel2 according to the specified
  *         parameters in the TIM_OCInitStruct.
  * @param  TIMx: where x can be 1, 2, 3, 4, 14, 15, 16, 17 to select 
  *         the TIM peripheral.
  * @param  TIM_OCInitStruct: pointer to a TIM_OCInitTypeDef structure
  *         that contains the configuration information for the specified TIM peripheral.
  * @retval None
=======================================================================================================*/
void TIM_OC2Init(TIM_TypeDef* TIMx, TIM_OC_InitTypeDef* TIM_OCInitStruct)
{
	TIMx->CCER &= (~BIT4);  //disable OC2
	if (OUTPUT_POL_ACTIVE_HIGH == TIM_OCInitStruct->OCPolarity)
	{
		TIMx->CCER &= (~BIT5);  
	}
	else
	{
		TIMx->CCER |= (BIT5);   
	}
		
	if (IS_TIM_CCXN_INSTANCE(TIMx, TIM_CHANNEL_2) )  
	{
		TIMx->CCER &= (~BIT6);  //disable OC2N  
			
		if (OUTPUT_POL_ACTIVE_HIGH == TIM_OCInitStruct->OCNPolarity)   
		{
			TIMx->CCER &= (~BIT7);  
		}
		else
		{
			TIMx->CCER |= (BIT7);     
		}  
	}
		
	TIMx->CCMR1 &= (~0xFF00U); // reset high 8 bits 
	TIMx->CCR2 = TIM_OCInitStruct->Pulse;   // write value to ccr before preload enable 
	if (OUTPUT_FAST_MODE_ENABLE == TIM_OCInitStruct->OCFastMode)  
	{
		TIMx->CCMR1 |= (BIT10);  
	}
	TIMx->CCMR1 |= (BIT11);  // preload enable 
		
	if (IS_TIM_BREAK_INSTANCE(TIMx)) 
	{
		if (OUTPUT_IDLE_STATE_0 == TIM_OCInitStruct->OCIdleState)
		{
			TIMx->CR2 &= (~BIT10); 
		}
		else
		{
			TIMx->CR2 |= BIT10; 
		}
			
		if (OUTPUT_IDLE_STATE_0 == TIM_OCInitStruct->OCNIdleState)  
		{
			TIMx->CR2 &= (~BIT11); 
		}
		else
		{
			TIMx->CR2 |= BIT11;   
		}
			
	} 
    
	TIMx->CCMR1 = (TIMx->CCMR1 & (~(BIT12|BIT13|BIT14))) | (TIM_OCInitStruct->OCMode << 12);   
}  

/*==================================================================================================
  * @brief  Initializes the TIMx Channel3 according to the specified
  *         parameters in the TIM_OCInitStruct.
  * @param  TIMx: where x can be 1, 2, 3, 4, 14, 15, 16, 17 to select 
  *         the TIM peripheral.
  * @param  TIM_OCInitStruct: pointer to a TIM_OCInitTypeDef structure
  *         that contains the configuration information for the specified TIM peripheral.
  * @retval None
=======================================================================================================*/
void TIM_OC3Init(TIM_TypeDef* TIMx, TIM_OC_InitTypeDef* TIM_OCInitStruct)
{
	TIMx->CCER &= (~BIT8);  //disable OC3  
	if (OUTPUT_POL_ACTIVE_HIGH == TIM_OCInitStruct->OCPolarity)
	{
		TIMx->CCER &= (~BIT9);  
	}
	else
	{
		TIMx->CCER |= (BIT9);   
	}
		
	if (IS_TIM_CCXN_INSTANCE(TIMx, TIM_CHANNEL_3) )     
	{
		TIMx->CCER &= (~BIT10);  //disable OC3N   
			
		if (OUTPUT_POL_ACTIVE_HIGH == TIM_OCInitStruct->OCNPolarity)   
		{
			TIMx->CCER &= (~BIT11);  
		}
		else
		{
			TIMx->CCER |= (BIT11);     
		}  
	}
		
	TIMx->CCMR2 &= (~0x00FF); // reset low 8 bits 
	TIMx->CCMR2 |= (BIT3);  // preload enable   
	if (OUTPUT_FAST_MODE_ENABLE == TIM_OCInitStruct->OCFastMode)  
	{
		TIMx->CCMR2 |= (BIT2);  
	}
		 
	TIMx->CCR3 = TIM_OCInitStruct->Pulse;  	
	if (IS_TIM_BREAK_INSTANCE(TIMx)) 
	{
		if (OUTPUT_IDLE_STATE_0 == TIM_OCInitStruct->OCIdleState)
		{
			TIMx->CR2 &= (~BIT12); 
		}
		else
		{
			TIMx->CR2 |= BIT12; 
		}
			
		if (OUTPUT_IDLE_STATE_0 == TIM_OCInitStruct->OCNIdleState)  
		{
			TIMx->CR2 &= (~BIT13); 
		}
		else
		{
			TIMx->CR2 |= BIT13;     
		} 
			
	}
    
	TIMx->CCMR2 = (TIMx->CCMR2 & (~(BIT4|BIT5|BIT6))) | (TIM_OCInitStruct->OCMode << 4);    
}  

/*==================================================================================================
  * @brief  Initializes the TIMx Channel4 according to the specified
  *         parameters in the TIM_OCInitStruct.
  * @param  TIMx: where x can be 1, 2, 3, 4, 14, 15, 16, 17 to select 
  *         the TIM peripheral.
  * @param  TIM_OCInitStruct: pointer to a TIM_OCInitTypeDef structure
  *         that contains the configuration information for the specified TIM peripheral.
  * @retval None
=======================================================================================================*/
void TIM_OC4Init(TIM_TypeDef* TIMx, TIM_OC_InitTypeDef* TIM_OCInitStruct)
{
	TIMx->CCER &= (~BIT12);  //disable OC4  
	if (OUTPUT_POL_ACTIVE_HIGH == TIM_OCInitStruct->OCPolarity)
	{
		TIMx->CCER &= (~BIT13);  
	}
	else
	{
		TIMx->CCER |= (BIT13);     
	}
		
	TIMx->CCMR2 &= (~0xFF00); // reset high 8 bits    
	TIMx->CCR4 = TIM_OCInitStruct->Pulse;   
	if (OUTPUT_FAST_MODE_ENABLE == TIM_OCInitStruct->OCFastMode)  
	{
		TIMx->CCMR2 |= (BIT10);  // fast mode  
	}
	TIMx->CCMR2 |= (BIT11);  // preload enable 
	
	if (IS_TIM_BREAK_INSTANCE(TIMx)) 
	{
		if (OUTPUT_IDLE_STATE_0 == TIM_OCInitStruct->OCIdleState)
		{
			TIMx->CR2 &= (~BIT14); 
		}
		else
		{
			TIMx->CR2 |= BIT14;    
		}
			
	}
    
	TIMx->CCMR2 = (TIMx->CCMR2 & (~(BIT12|BIT13|BIT14))) | (TIM_OCInitStruct->OCMode << 12);   
}     

	
/*=========================================================================================
  * @brief  Enables or disables the TIM Capture Compare Channel x.
  * @param  TIMx: TIM1/TIM2/TIM3...
  * @param  TIM_Channel: specifies the TIM Channel
  *   This parameter can be one of the following values:
  *     @arg TIM_Channel_1: TIM Channel 1
  *     @arg TIM_Channel_2: TIM Channel 2
  *     @arg TIM_Channel_3: TIM Channel 3
  *     @arg TIM_Channel_4: TIM Channel 4
  * @param  TIM_CCx: specifies the TIM Channel CCxE bit new state.
  *   This parameter can be: TIM_CCx_Enable or TIM_CCx_Disable. 
  * @retval None
================================================================================================*/
void TIM_CCxCmd(TIM_TypeDef* TIMx, uint16_t TIM_Channel, uint16_t TIM_CCx)
{
    uint16_t tmp = 0, channel;  
    
    switch(TIM_Channel)  
    {
        case TIM_CHANNEL_1:  
        channel = 0; 
        break; 
        
        case TIM_CHANNEL_2:  
        channel = 4;  
        break;
        
        case TIM_CHANNEL_3:  
        channel = 8;  
        break;  
        
        case TIM_CHANNEL_4: 
        channel = 12;  
        break;  
        
        default:
        break;      
    }

    tmp = CCER_CCE_Set << channel;  

    /* Reset the CCxE Bit */
    TIMx->CCER &= (uint16_t)~ tmp;

    /* Set or reset the CCxE Bit */ 
    TIMx->CCER |=  (uint16_t)(TIM_CCx << channel);     
    
}  


/*=============================================================================================
  * @brief  Enables or disables the TIM Capture Compare Channel xN.
  * @param  TIMx: TIM1/TIM2/TIM3... 
  * @param  TIM_Channel: specifies the TIM Channel
  *   This parameter can be one of the following values:
  *     @arg TIM_Channel_1: TIM Channel 1
  *     @arg TIM_Channel_2: TIM Channel 2
  *     @arg TIM_Channel_3: TIM Channel 3
  * @param  TIM_CCxN: specifies the TIM Channel CCxNE bit new state.
  *   This parameter can be: TIM_CCxN_Enable or TIM_CCxN_Disable. 
  * @retval None
====================================================================================================*/   
void TIM_CCxNCmd(TIM_TypeDef* TIMx, uint16_t TIM_Channel, uint16_t TIM_CCxN)
{
    uint16_t tmp = 0, channel;

    switch(TIM_Channel)  
    {
        case TIM_CHANNEL_1:  
        channel = 0; 
        break; 
        
        case TIM_CHANNEL_2:  
        channel = 4;  
        break;
        
        case TIM_CHANNEL_3:  
        channel = 8;  
        break;  
        
        default:  
        return;   
    }

    tmp = CCER_CCNE_Set << channel;

    /* Reset the CCxNE Bit */
    TIMx->CCER &= (uint16_t) ~tmp;

    /* Set or reset the CCxNE Bit */ 
    TIMx->CCER |=  (uint16_t)(TIM_CCxN << channel);    
    
}  


/*===========================================================================================
  * @brief  Enables or disables the TIM peripheral Main Outputs.
  * @param  TIMx: where x can be 1, 14, 15, 16 or 17 to select the TIMx peripheral.
  * @param  NewState: new state of the TIM peripheral Main Outputs.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
=================================================================================================*/
void TIM_CtrlPWMOutputs(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    /* Enable the TIM Main Output */
    TIMx->BDTR |= BIT15;
  }
  else
  {
    /* Disable the TIM Main Output */
    TIMx->BDTR &= (uint16_t)(~((uint16_t)BIT15));   
  }  
}  


/*********************************************************************************
* Function    : TIMER_IC1_SetConfig  
* Description : configure TI1FP1 or TI2FP1 channel polarity and filter 
* Input:
  TIMx : timer instance    
  TIM_ICPolarity:  TIM_SLAVE_CAPTURE_ACTIVE_RISING, TIM_SLAVE_CAPTURE_ACTIVE_FALLING...   
  Filter:  TIM_TI1_FILTER_LVL(x), x=0-15  
* Output      : none  
* Author      : xwl                       
**********************************************************************************/ 
void TIMER_IC1_SetConfig(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection, uint32_t Filter)
{
    /* Disable the Channel 1: Reset the CC1E Bit */
    TIMx->CCER &= (~BIT0);  

    if(TIM_SLAVE_CAPTURE_ACTIVE_RISING == TIM_ICPolarity)  
    {
        TIMx->CCER |= TIM_CC1_SLAVE_CAPTURE_POL_RISING;   
    }
    else if (TIM_SLAVE_CAPTURE_ACTIVE_FALLING == TIM_ICPolarity)  
    {
		TIMx->CCER |= TIM_CC1_SLAVE_CAPTURE_POL_FALLING;      
    }
	else if (TIM_SLAVE_CAPTURE_ACTIVE_RISING_FALLING == TIM_ICPolarity)  
	{
		TIMx->CCER |= TIM_CC1_SLAVE_CAPTURE_POL_BOTH;     
	}
		
    if(TIM_ICSELECTION_DIRECTTI == TIM_ICSelection)     
    {
        TIMx->CCMR1 = (TIMx->CCMR1 & (~(BIT0|BIT1))) | BIT0;   
        TIMx->CCMR1 &= (~0xF0U);  
    }
    else
    {
        TIMx->CCMR1 = (TIMx->CCMR1 & (~(BIT0|BIT1))) | BIT1;     
        TIMx->CCMR1 &= (~0xF000U);    
    }
    
    TIMx->CCMR1 |= Filter;  
       
}

/*********************************************************************************
* Function    : TIMER_IC2_SetConfig  
* Description : configure TI1FP2 or TI2FP2 channel polarity and filter 
* Input:
  TIMx : timer instance    
  TIM_ICPolarity:  TIM_SLAVE_CAPTURE_ACTIVE_RISING, TIM_SLAVE_CAPTURE_ACTIVE_FALLING...   
  Filter:  TIM_TI2_FILTER_LVL(x), x=0-15  
* Output      : none  
* Author      : xwl                       
**********************************************************************************/ 
static void TIMER_IC2_SetConfig(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection, uint32_t Filter)
{
    /* Disable the Channel 2, Reset the CC2E Bit */
    TIMx->CCER &= (~BIT4); 
    if(TIM_SLAVE_CAPTURE_ACTIVE_RISING == TIM_ICPolarity)  
    {
		TIMx->CCER |= TIM_CC2_SLAVE_CAPTURE_POL_RISING;    
    }
	else if (TIM_SLAVE_CAPTURE_ACTIVE_FALLING == TIM_ICPolarity)  
	{
		TIMx->CCER |= TIM_CC2_SLAVE_CAPTURE_POL_FALLING;         
	}
	else if (TIM_SLAVE_CAPTURE_ACTIVE_RISING_FALLING == TIM_ICPolarity)  
	{
		TIMx->CCER |= TIM_CC2_SLAVE_CAPTURE_POL_BOTH;       
	}
		
	if(TIM_ICSELECTION_DIRECTTI == TIM_ICSelection)     
	{
		TIMx->CCMR1 = (TIMx->CCMR1 & (~(BIT8|BIT9))) | BIT8; 
        TIMx->CCMR1 &= (~0xF000U);   
	}
	else
	{
		TIMx->CCMR1 = (TIMx->CCMR1 & (~(BIT8|BIT9))) | BIT9; 
        TIMx->CCMR1 &= (~0xF0U);     
	}
    
    TIMx->CCMR1 |= Filter;     
      
}  

/*********************************************************************************
* Function    : TIMER_IC3_SetConfig  
* Description : configure TI3FP3 or TI4FP3 channel polarity and filter 
* Input:
  TIMx : timer instance    
  TIM_ICPolarity:  TIM_SLAVE_CAPTURE_ACTIVE_RISING, TIM_SLAVE_CAPTURE_ACTIVE_FALLING...   
  Filter:  TIM_TI3_FILTER_LVL(x), x=0-15  
* Output      : none  
* Author      : xwl                       
**********************************************************************************/ 
static void TIMER_IC3_SetConfig(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection, uint32_t Filter)
{
    /* Disable the Channel 3, Reset the CC3E Bit */
    TIMx->CCER &= (~BIT8); 
    
    if(TIM_SLAVE_CAPTURE_ACTIVE_RISING == TIM_ICPolarity)  
	{
        TIMx->CCER |= TIM_CC3_SLAVE_CAPTURE_POL_RISING;    
	}
    else if (TIM_SLAVE_CAPTURE_ACTIVE_FALLING == TIM_ICPolarity)  
    {
        TIMx->CCER |= TIM_CC3_SLAVE_CAPTURE_POL_FALLING;         
    }
    else if (TIM_SLAVE_CAPTURE_ACTIVE_RISING_FALLING == TIM_ICPolarity)  
    {
        TIMx->CCER |= TIM_CC3_SLAVE_CAPTURE_POL_BOTH;       
    }
		
    if(TIM_ICSELECTION_DIRECTTI == TIM_ICSelection)        
    {
        TIMx->CCMR2 = (TIMx->CCMR2 & (~(BIT0|BIT1))) | BIT0;    
        TIMx->CCMR2 &= (~0xF0U);    
    }
    else
    {
        TIMx->CCMR2 = (TIMx->CCMR2 & (~(BIT0|BIT1))) | BIT1;       
        TIMx->CCMR2 &= (~0xF000U);     
    }
     

    TIMx->CCMR2 |= Filter;  
}


/*********************************************************************************
* Function    : TIMER_IC4_SetConfig  
* Description : configure TI3FP4 or TI4FP4 channel polarity and filter 
* Input:
  TIMx : timer instance    
  TIM_ICPolarity:  TIM_SLAVE_CAPTURE_ACTIVE_RISING, TIM_SLAVE_CAPTURE_ACTIVE_FALLING...   
  Filter:  TIM_TI4_FILTER_LVL(x), x=0-15  
* Output      : none  
* Author      : xwl                             
**********************************************************************************/ 
static void TIMER_IC4_SetConfig(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection, uint32_t Filter)
{
     /* Disable the Channel 3, Reset the CC3E Bit */
    TIMx->CCER &= (~BIT12);   
    
    if(TIM_SLAVE_CAPTURE_ACTIVE_RISING == TIM_ICPolarity)  
    {
        TIMx->CCER |= TIM_CC4_SLAVE_CAPTURE_POL_RISING;    
    }
    else if (TIM_SLAVE_CAPTURE_ACTIVE_FALLING == TIM_ICPolarity)  
    {
        TIMx->CCER |= TIM_CC4_SLAVE_CAPTURE_POL_FALLING;         
    }
    else if (TIM_SLAVE_CAPTURE_ACTIVE_RISING_FALLING == TIM_ICPolarity)    
    {
        TIMx->CCER |= TIM_CC4_SLAVE_CAPTURE_POL_BOTH;         
    }

    if(TIM_ICSELECTION_DIRECTTI == TIM_ICSelection)     
    {
        TIMx->CCMR2 = (TIMx->CCMR2 & (~(BIT8|BIT9))) | BIT8;    
        TIMx->CCMR2 &= (~0xF000U);    
    }
    else
    {
        TIMx->CCMR2 = (TIMx->CCMR2 & (~(BIT8|BIT9))) | BIT9;    
        TIMx->CCMR2 &= (~0xF0U);           
    }

    TIMx->CCMR2 |= Filter;     
       
}

/*********************************************************************************
* Function    : TIM_ICInit  
* Description : configure capture parameters   
* Input       : 
              TIMx: timer instance 
              Capture_Config: capture configuration parameter strcture 
                            ICPolarity: TIM_SLAVE_CAPTURE_ACTIVE_RISING, TIM_SLAVE_CAPTURE_ACTIVE_FALLING... 
                            ICSelection: TIM_ICSELECTION_DIRECTTI, TIM_ICSELECTION_INDIRECTTI
                            ICFilter: TIM_IC1_FILTER_LVL(x), TIM_IC2_FILTER_LVL(x), x:0-15 
                            ICPrescaler: TIM_IC1_PRESCALER_1, TIM_IC2_PRESCALER_1... 
              Channel: channel id, TIM_CHANNEL_1, TIM_CHANNEL_2...   
* Output      : 0: success; else:error 
* Author      : xwl                       
**********************************************************************************/  	
uint32_t TIM_ICInit(TIM_TypeDef *TIMx, TIM_IC_InitTypeDef * Capture_Config, uint32_t Channel)  
{
	switch(Channel) 
	{
		case TIM_CHANNEL_1:   
		TIMER_IC1_SetConfig(TIMx, Capture_Config->ICPolarity, Capture_Config->ICSelection, Capture_Config->TIFilter);  
         
        /* Reset the IC1PSC Bits */
        TIMx->CCMR1 &= (~BIT2|BIT3);   
        /* Set the IC1PSC value */
        TIMx->CCMR1 |= Capture_Config->ICPrescaler;   
		break;  
		
		case TIM_CHANNEL_2:  
        TIMER_IC2_SetConfig(TIMx, Capture_Config->ICPolarity, Capture_Config->ICSelection, Capture_Config->TIFilter); 

        
        /* Reset the IC2PSC Bits */
        TIMx->CCMR1 &= (~BIT10|BIT11);
        /* Set the IC2PSC value */
        TIMx->CCMR1 |= Capture_Config->ICPrescaler;  
		break; 
		
		case TIM_CHANNEL_3:    
        TIMER_IC3_SetConfig(TIMx, Capture_Config->ICPolarity, Capture_Config->ICSelection, Capture_Config->TIFilter);    

        /* Reset the IC3PSC Bits */
        TIMx->CCMR2 &= (~BIT2|BIT3);
        /* Set the IC3PSC value */
        TIMx->CCMR2 |= Capture_Config->ICPrescaler;      
                
		break; 
		
		case TIM_CHANNEL_4:   
        TIMER_IC4_SetConfig(TIMx, Capture_Config->ICPolarity, Capture_Config->ICSelection, Capture_Config->TIFilter);         
        
        /* Reset the IC4PSC Bits */
        TIMx->CCMR2 &= (~BIT10|BIT11);   
        /* Set the IC4PSC value */
        TIMx->CCMR2 |= Capture_Config->ICPrescaler;  
		break;   
		
		default:  
		return 1;     
	}
		
	return 0;  
}    


/*===================================================================================
  * @brief  Checks whether the specified TIM flag is set or not.
  * @param  TIMx: where x can be 1 to 17 to select the TIM peripheral.
  * @param  TIM_FLAG: specifies the flag to check.
  *   This parameter can be one of the following values:
  *     @arg TIMER_SR_UIF: TIM update Flag
  *     @arg TIMER_SR_CC1IF: TIM Capture Compare 1 Flag
  *     @arg TIMER_SR_CC2IF: TIM Capture Compare 2 Flag
  *     @arg TIMER_SR_CC3IF: TIM Capture Compare 3 Flag
  *     @arg TIMER_SR_CC4IF: TIM Capture Compare 4 Flag
  *     @arg TIMER_SR_COMIF: TIM Commutation Flag
  *     @arg TIMER_SR_TIF:   TIM Trigger Flag
  *     @arg TIMER_SR_BIF: TIM Break Flag
  *     @arg TIMER_SR_CC1OF: TIM Capture Compare 1 overcapture Flag
  *     @arg TIMER_SR_CC2OF: TIM Capture Compare 2 overcapture Flag
  *     @arg TIMER_SR_CC3OF: TIM Capture Compare 3 overcapture Flag
  *     @arg TIMER_SR_CC4OF: TIM Capture Compare 4 overcapture Flag    
  * @retval The new state of TIM_FLAG (SET or RESET).
==========================================================================================*/
FlagStatus TIM_GetFlagStatus(TIM_TypeDef* TIMx, uint16_t TIM_FLAG)
{ 
  ITStatus bitstatus = RESET;  
  
  if ((TIMx->SR & TIM_FLAG) != (uint16_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

/*===============================================================================================
  * @brief  Clears the TIMx's pending flags.
  * @param  TIMx: where x can be 1 to 17 to select the TIM peripheral.
  * @param  TIM_FLAG: specifies the flag bit to clear.
  *   This parameter can be any combination of the following values:
  *     @arg TIMER_SR_UIF: TIM update Flag
  *     @arg TIMER_SR_CC1IF: TIM Capture Compare 1 Flag
  *     @arg TIMER_SR_CC2IF: TIM Capture Compare 2 Flag
  *     @arg TIMER_SR_CC3IF: TIM Capture Compare 3 Flag
  *     @arg TIMER_SR_CC4IF: TIM Capture Compare 4 Flag
  *     @arg TIMER_SR_COMIF: TIM Commutation Flag
  *     @arg TIMER_SR_TIF:   TIM Trigger Flag
  *     @arg TIMER_SR_BIF: TIM Break Flag
  *     @arg TIMER_SR_CC1OF: TIM Capture Compare 1 overcapture Flag
  *     @arg TIMER_SR_CC2OF: TIM Capture Compare 2 overcapture Flag
  *     @arg TIMER_SR_CC3OF: TIM Capture Compare 3 overcapture Flag
  *     @arg TIMER_SR_CC4OF: TIM Capture Compare 4 overcapture Flag 
  * @retval None
================================================================================*/
void TIM_ClearFlag(TIM_TypeDef* TIMx, uint16_t TIM_FLAG)
{    
  /* Clear the flags */
  TIMx->SR = (uint16_t)~TIM_FLAG;  
} 

/*===================================================================================================
  * @brief  Sets or Resets the TIMx Master/Slave Mode.
  * @param  TIMx: where x can be 1, 2, 3, ... 
  * @param  TIM_MasterSlaveMode: specifies the Timer Master Slave Mode.
  *   This parameter can be one of the following values:
  *     @arg TIM_MASTERSLAVEMODE_ENABLE: synchronization between the current timer
  *                                      and its slaves (through TRGO).
  *     @arg TIM_MASTERSLAVEMODE_DISABLE: No action  
  * @retval None
=====================================================================================================*/
void TIM_SelectMasterSlaveMode(TIM_TypeDef* TIMx, uint16_t TIM_MasterSlaveMode)
{
  /* Reset the MSM Bit */
  TIMx->SMCR &= (~BIT7);     
  
  /* Set or Reset the MSM Bit */
  TIMx->SMCR |= TIM_MasterSlaveMode;
}   

/*================================================================================================================
  * @brief  Selects the TIMx Trigger Output Mode.
  * @param  TIMx: where x can be 1, 2, 3,... 
  * @param  TIM_TRGOSource: specifies the Trigger Output source.
  *   This paramter can be one of the following values:
  *
  *  - For all TIMx
  *     @arg TIM_TRGO_RESET:  The UG bit in the TIM_EGR register is used as the trigger output (TRGO).
  *     @arg TIM_TRGO_ENABLE: The Counter Enable CEN is used as the trigger output (TRGO).
  *     @arg TIM_TRGO_UPDATE: The update event is selected as the trigger output (TRGO).
  *
  *  - For all TIMx except TIM6 and TIM7
  *     @arg #define TIM_TRGO_CMP_PULSE: compare pulse 
  *     @arg TIM_TRGO_OC1REF: OC1REF signal is used as the trigger output (TRGO).
  *     @arg TIM_TRGO_OC2REF: OC2REF signal is used as the trigger output (TRGO).
  *     @arg TIM_TRGO_OC3REF: OC3REF signal is used as the trigger output (TRGO).
  *     @arg TIM_TRGO_OC4REF: OC4REF signal is used as the trigger output (TRGO).
  *
  * @retval None
====================================================================================================================*/
void TIM_SelectOutputTrigger(TIM_TypeDef* TIMx, uint16_t TIM_TRGOSource)
{
  /* Reset the MMS Bits */
  TIMx->CR2  &= (~(BIT4|BIT5|BIT6));   
  /* Select the TRGO source */
  TIMx->CR2 |=  TIM_TRGOSource;
}      


/*==========================================================================================================
  * @brief  Selects the Input Trigger source
  * @param  TIMx: where x can be  1, 2, 3, 4, 5, 8, 9, 12 or 15 to select the TIM peripheral.
  * @param  TIM_InputTriggerSource: The Input Trigger source.
  *   This parameter can be one of the following values:
  *     @arg TIM_TRIGGER_SOURCE_ITR0: Internal Trigger 0
  *     @arg TIM_TRIGGER_SOURCE_ITR1: Internal Trigger 1
  *     @arg TIM_TRIGGER_SOURCE_ITR2: Internal Trigger 2
  *     @arg TIM_TRIGGER_SOURCE_ITR3: Internal Trigger 3
  *     @arg TIM_TRIGGER_SOURCE_TI1F_ED: TI1 Edge Detector
  *     @arg TIM_TRIGGER_SOURCE_TI1FP1: Filtered Timer Input 1
  *     @arg TIM_TRIGGER_SOURCE_TI2FP2: Filtered Timer Input 2
  *     @arg TIM_TRIGGER_SOURCE_ETRF: External Trigger input
  * @retval None
============================================================================================================*/
void TIM_SelectInputTrigger(TIM_TypeDef* TIMx, uint16_t TIM_InputTriggerSource)
{
  uint16_t tmpsmcr = 0; 
  /* Get the TIMx SMCR register value */
  tmpsmcr = TIMx->SMCR;
  /* Reset the TS Bits */
  tmpsmcr &= (~(BIT4|BIT5|BIT6));  
  /* Set the Input Trigger source */
  tmpsmcr |= TIM_InputTriggerSource;
  /* Write to TIMx SMCR */   
  TIMx->SMCR = tmpsmcr;
}  

/*=========================================================================================================
  * @brief  Selects the TIMx Slave Mode.
  * @param  TIMx: where x can be 1, 2, 3,...  
  * @param  TIM_SlaveMode: specifies the Timer Slave Mode.
  *   This parameter can be one of the following values:
  *     @arg TIM_SLAVE_MODE_DIS: slave mode disabled 
  *     @arg TIM_SLAVE_MODE_ENC1:  encoder mode 1 
  *     @arg TIM_SLAVE_MODE_ENC2:  encoder mode 2
  *     @arg TIM_SLAVE_MODE_ENC3:  encoder mode 3  
  *     @arg TIM_SLAVE_MODE_RST: reset mode 
  *     @arg TIM_SLAVE_MODE_GATE: gate mode 
  *     @arg TIM_SLAVE_MODE_TRIG: trigger mode   
  *     @arg TIM_SLAVE_MODE_EXT1: external clock mode  
  * @retval None
================================================================================================================*/
void TIM_SelectSlaveMode(TIM_TypeDef* TIMx, uint16_t TIM_SlaveMode)  
{
 /* Reset the SMS Bits */
  TIMx->SMCR &= (~(BIT0|BIT1|BIT2)); 
  /* Select the Slave Mode */
  TIMx->SMCR |= TIM_SlaveMode;     
}


/*********************************************************************************
* Function    : TIMER_TI1FP1_ConfigInputStage  
* Description : configure TI1FP1 channel polarity and filter 
* Input:
  TIMx : timer instance    
  TIM_ICPolarity:  TIM_SLAVE_CAPTURE_ACTIVE_RISING, TIM_SLAVE_CAPTURE_ACTIVE_FALLING...   
  Filter:  TIM_TI1_FILTER_LVL(x), x=0-15  
* Output      : none  
* Author      : xwl                       
**********************************************************************************/ 
void TIM_TI1FP1_ConfigInputStage(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t Filter)
{    
    TIMx->CCER &= (~BIT0);  //Disable the Channel 1: Reset the CC1E Bit   
    TIMx->CCMR1 = (TIMx->CCMR1 & (~(BIT0|BIT1)) | BIT0);    // CH1 as input 
  
    TIMx->CCMR1 &= (~0xF0U); // reset TI1 filter   
    TIMx->CCMR1 |= Filter; 
    
    if(TIM_SLAVE_CAPTURE_ACTIVE_RISING == TIM_ICPolarity)  
	{
		TIMx->CCER |= TIM_CC1_SLAVE_CAPTURE_POL_RISING;   
	}
	else if (TIM_SLAVE_CAPTURE_ACTIVE_FALLING == TIM_ICPolarity)  
	{
		TIMx->CCER |= TIM_CC1_SLAVE_CAPTURE_POL_FALLING;      
	}
	else
	{
		TIMx->CCER |= TIM_CC1_SLAVE_CAPTURE_POL_BOTH;     
	}
}


/*********************************************************************************
* Function    : TIMER_TI2FP2_ConfigInputStage  
* Description : configure TI2FP2 channel polarity and filter 
* Input:
  TIMx : timer instance    
  TIM_ICPolarity:  TIM_SLAVE_CAPTURE_ACTIVE_RISING, TIM_SLAVE_CAPTURE_ACTIVE_FALLING...   
  Filter:  TIM_TI2_FILTER_LVL(x), x=0-15  
* Output      : none  
* Author      : xwl                       
**********************************************************************************/ 
void TIMER_TI2FP2_ConfigInputStage(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t Filter)
{    
    TIMx->CCER &= (~BIT4);  //Disable the Channel 2: Reset the CC2E Bit   
    TIMx->CCMR1 = (TIMx->CCMR1 & (~(BIT8|BIT9)) | BIT8);    // CH2 as input 
  
    TIMx->CCMR1 &= (~0xF000U); // reset TI2 filter    
    TIMx->CCMR1 |= Filter;   
    
    if(TIM_SLAVE_CAPTURE_ACTIVE_RISING == TIM_ICPolarity)  
	{
		TIMx->CCER |= TIM_CC2_SLAVE_CAPTURE_POL_RISING;   
	}
	else if (TIM_SLAVE_CAPTURE_ACTIVE_FALLING == TIM_ICPolarity)  
	{
		TIMx->CCER |= TIM_CC2_SLAVE_CAPTURE_POL_FALLING;      
	}
	else
	{
		TIMx->CCER |= TIM_CC2_SLAVE_CAPTURE_POL_BOTH;     
	}
}         

/*********************************************************************************
* Function    : TIMER_ETR_SetConfig  
* Description : configure ETR channel polarity, prescaler and filter 
* Input:
  TIMx : timer instance  
  TIM_ExtTRGPrescaler: TIM_ETR_PRESCALER_1, TIM_ETR_PRESCALER_2...  
  TIM_ExtTRGPolarity:  TIM_ETR_POLAIRTY_HIGH, TIM_ETR_POLAIRTY_LOW
  ExtTRGFilter:  TIM_ETR_FILTER_LVL(x), x=0-15  
* Output      : none  
* Author      : xwl                       
**********************************************************************************/ 
void TIM_ETR_SetConfig(TIM_TypeDef* TIMx, uint32_t TIM_ExtTRGPrescaler, uint32_t TIM_ExtTRGPolarity, uint32_t ExtTRGFilter)
{
  /* Reset the ETR Bits */
  TIMx->SMCR &= (~0xFF00U);     

  /* Set the Prescaler, the Filter value and the Polarity */
  TIMx->SMCR |= (TIM_ExtTRGPrescaler | TIM_ExtTRGPolarity | ExtTRGFilter);     
}   


