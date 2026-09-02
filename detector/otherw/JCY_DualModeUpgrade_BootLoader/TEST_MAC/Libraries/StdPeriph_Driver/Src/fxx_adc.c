/*
  ******************************************************************************
  * @file    fxx_adc.c
  * @version V1.0.0
  * @date    2020
  * @brief   ADC module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Analog to Digital Converter (ADC) peripheral:
  *           @ Initialization  functions
  *           @ IO operation functions
  ******************************************************************************
*/
#include "fxx_std.h"

/**
  * @brief  Deinitializes the ADCx peripheral registers to their default reset values.
  * @param  ADCx: where x can be ADC to select the ADC peripheral.
  * @retval None
  */
void ADC_DeInit(ADC_TypeDef* ADCx)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));

    if (ADCx == ADC)
    {
        /* Enable ADC reset state */
        System_Module_Reset (RST_ADC);
    }
}

/**
  * @brief  Initializes the ADCx peripheral according to the specified parameters
  *         in the ADC_InitStruct.
  * @param  ADCx: where x can be ADC to select the ADC peripheral.
  * @param  ADC_InitStruct: pointer to an ADC_InitTypeDef structure that contains
  *         the configuration information for the specified ADC peripheral.
  * @retval None
  */
void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct)
{
	uint32_t tmpreg1 = 0;
	uint8_t tmpreg2 = 0;
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_ALL_CLOCKDIV(ADC_InitStruct->ADC_ClockDiv));
	assert_param(IS_FUNCTIONAL_STATE(ADC_InitStruct->ADC_ContinuousConvMode));
	assert_param(IS_ADC_ALL_TRIG(ADC_InitStruct->ADC_ExternalTrigConv));   
	assert_param(IS_ADC_REGULAR_LENGTH(ADC_InitStruct->ADC_NbrOfChannel));

   //Reset AFE.
    SET_BIT(ADCx->CR2,ADC_CR2_AFE_RSTN);
   //Set Clock DIV.
    MODIFY_REG(ADCx->CR2,ADC_CR2_DIV_MASK, (ADC_InitStruct->ADC_ClockDiv<<ADC_CR2_DIV_POS));

    if(ADC_InitStruct->ADC_ContinuousConvMode == ENABLE)
        SET_BIT(ADCx->CR1,ADC_CR1_CONT);
    else
        CLEAR_BIT(ADCx->CR1,ADC_CR1_CONT);
    
    //ExTrigSel set
    MODIFY_REG(ADCx->CR1, ADC_CR1_EXTSEL_MASK, (ADC_InitStruct->ADC_ExternalTrigConv<<ADC_CR1_EXTSEL_POS));

	/*---------------------------- ADCx SQR1 Configuration -----------------*/
	MODIFY_REG(ADCx->SQR1,ADC_SQR1_L,(ADC_InitStruct->ADC_NbrOfChannel-1));
}

/**
  * @brief  Fills each ADC_InitStruct member with its default value.
  * @param  ADC_InitStruct : pointer to an ADC_InitTypeDef structure which will be initialized.
  * @retval None
  */
void ADC_StructInit(ADC_InitTypeDef* ADC_InitStruct)
{
	/* Reset ADC init structure parameters values */
	/* Initialize the ADC_ContinuousConvMode member */
	ADC_InitStruct->ADC_ContinuousConvMode = DISABLE;
	/* Initialize the ADC_ExternalTrigConv member */
	ADC_InitStruct->ADC_ExternalTrigConv = ADC_SOFTWARE_START;
	/* Initialize the ADC_NbrOfChannel member */
	ADC_InitStruct->ADC_NbrOfChannel = 1;
}

/**
  * @brief  Enables or disables the specified ADC peripheral.
  * @param  ADCx: where x can be ADC to select the ADC peripheral.
  * @param  NewState: new state of the ADCx peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_Cmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if (NewState != DISABLE)
	{
		/* Set the ADON bit to wake up the ADC from power down mode */
		ADCx->CR2 |= ADC_CR2_ADC_EN;
	}
	else
	{
		/* Disable the selected ADC peripheral */
		ADCx->CR2 &= ~ADC_CR2_ADC_EN;
	}
}

/**
  * @brief  Enables or disables the specified ADC DMA request.
  * @param  ADCx: where x can be ADC to select the ADC peripheral.
  * @param  NewState: new state of the selected ADC DMA transfer.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_DMACmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if (NewState != DISABLE)
	{
		/* Enable the selected ADC DMA request */
		ADCx->CR1 |= ADC_CR1_DMA;
	}
	else
	{
		/* Disable the selected ADC DMA request */
		ADCx->CR1 &= ~ADC_CR1_DMA;
	}
}

/**
  * @brief  Enables or disables the specified ADC interrupts.
  * @param  ADCx: where x can be ADC to select the ADC peripheral.
  * @param  ADC_IT: specifies the ADC interrupt sources to be enabled or disabled. 
  *   This parameter can be any combination of the following values:
  *     @arg ADC_IE_AWDIE: Analog watchdog interrupt mask
  *     @arg ADC_IE_OVERFIE: Over flow interrupt mask
  *     @arg ADC_IE_EOGIE: End of conversion of regular group interrupt mask
  *     @arg ADC_IE_JEOCIE: End of injected conversion interrupt mask
  *     @arg ADC_IE_EOCIE: End of conversion interrupt mask
  * @param  NewState: new state of the specified ADC interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_ITConfig(ADC_TypeDef* ADCx, uint16_t ADC_IT, FunctionalState NewState)
{
    uint8_t itmask = 0;
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    assert_param(IS_ADC_IT(ADC_IT));
    /* Get the ADC IT index */
    itmask = (uint8_t)ADC_IT;
    if (NewState != DISABLE)
    {
        /* Enable the selected ADC interrupts */
        ADCx->IE |= itmask;
    }
    else
    {
        /* Disable the selected ADC interrupts */
        ADCx->IE &= (~(uint32_t)itmask);
    }
}

/**
  * @brief  Enables or disables the selected ADC software start conversion .
  * @param  ADCx: where x can be ADC to select the ADC peripheral.
  * @param  NewState: new state of the selected ADC software start conversion.
  *   This parameter can be: Only ENABLE.
  * @retval None
  */
void ADC_SoftwareStartConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE)
    {
        /* Enable the selected ADC conversion on external event and start the selected
        ADC conversion */
        /* Start covertion */
        SET_BIT(ADCx->CR1,ADC_CR1_SWSTART);
    }
}

/**
  * @brief  Gets the selected ADC Software start conversion Status.
  * @param  ADCx: where x can be ADC to select the ADC peripheral.
  * @retval The new state of ADC software start conversion (SET or RESET).
  */
FlagStatus ADC_GetSoftwareStartConvStatus(ADC_TypeDef* ADCx)
{
    FlagStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    /* Check the status of SWSTART bit */
    if ((ADCx->CR1 & ADC_CR1_SWSTART) != (uint32_t)RESET)
    {
        /* SWSTART bit is set */
        bitstatus = SET;
    }
    else
    {
        /* SWSTART bit is reset */
        bitstatus = RESET;
    }
    /* Return the SWSTART bit status */
    return  bitstatus;
}

/**
  * @brief  Stop the selected ADC continue mode conversion .
  * @param  ADCx: where x can be ADC to select the ADC peripheral.
  * @retval None
  */
void ADC_ContinueModeStopCmd(ADC_TypeDef* ADCx)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    /* Set stop flag */
    SET_BIT(ADCx->CR2, ADC_CR2_ADC_STP);        
    /* Waitting stop flag be cleared */    
    while(READ_BIT(ADCx->CR2, ADC_CR2_ADC_STP));
}

/**
  * @brief  Configures the discontinuous mode for the selected ADC regular
  *         group channel.
  * @param  ADCx: where x can be ADC to select the ADC peripheral.
  * @param  Number: specifies the discontinuous mode regular channel
  *         count value. This number must be between 1 and 16.
  * @retval None
  */
void ADC_DiscModeChannelCountConfig(ADC_TypeDef* ADCx, uint8_t Number)
{
    uint32_t tmpreg1 = 0;
    uint32_t tmpreg2 = 0;
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_REGULAR_DISC_NUMBER(Number));
    
    /* Modify the discontinuous mode regular channel count value */    
    MODIFY_REG(ADC->CR1, ADC_CR1_DISCNUM_MASK, (Number << ADC_CR1_DISCNUM_POS));
}

/**
  * @brief  Enables or disables the discontinuous mode on regular group
  *         channel for the specified ADC
  * @param  ADCx: where x can be ADC to select the ADC peripheral.
  * @param  NewState: new state of the selected ADC discontinuous mode
  *         on regular group channel.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_DiscModeCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE)
    {
        /* Enable the selected ADC regular discontinuous mode */
        SET_BIT(ADCx->CR1, ADC_CR1_DISCEN);
    }
    else
    {
        /* Disable the selected ADC regular discontinuous mode */
        CLEAR_BIT(ADCx->CR1, ADC_CR1_DISCEN);
    }
}

/**
  * @brief  Configures for the selected ADC regular channel its corresponding
  *         rank in the sequencer and its sample time.
  * @param  ADCx: where x can be ADC to select the ADC peripheral.
  * @param  ADC_Channel: the ADC channel to configure. 
  *   This parameter can be one of the following values:
  *     @arg ADC_CHANNEL_0: ADC Channel0 selected
  *     ...to ...
  *     @arg ADC_CHANNEL_15: ADC Channel15 selected
  *     @arg ADC_CHANNEL_TEMP: ADC Channel16 selected
  *     @arg ADC_CHANNEL_VBAT: ADC Channel17 selected
  *     @arg ADC_CHANNEL_VBGR: ADC Channel18 selected
  *     @arg ADC_CHANNEL_EXT2: ADC Channel19 selected
  *     @arg ADC_CHANNEL_EXT2: ADC Channel20 selected
  * @param  Rank: The rank in the regular group sequencer. This parameter must be between 1 to 16.
  * @param  ADC_SampleTime: The sample time value to be set for the selected channel. 
  *   This parameter can be one of the following values:
  *     @arg ADC_SMP_CLOCK_3: Sample time equal to 3 cycles
  *     ...to ...
  *     @arg ADC_SMP_CLOCK_640: Sample time equal to 640 cycles	
  * @param  ADC_DiffMode: Specify the differential parameters. 
  *     @arg DISABLE:single end mode
  *     @arg ENABLE:differential end mode
  * @retval None
  */
void ADC_RegularChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime, FunctionalState ADC_DiffMode)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_ALL_CHANNEL(ADC_Channel));
    assert_param(IS_ADC_REGULAR_RANK(Rank));
    assert_param(IS_ADC_ALL_SMPCLOCK(ADC_SampleTime));
    assert_param(IS_FUNCTIONAL_STATE(ADC_DiffMode));
    /* Differential mode set*/
    if(ADC_DiffMode == ENABLE)
    {
        SET_BIT(ADCx->DIFF,1<<ADC_Channel);
        SET_BIT(ADCx->SIGN,1<<ADC_Channel);     //If define differential mode ,set as sign resault
    }
    else
    {
        CLEAR_BIT(ADCx->DIFF,1<<ADC_Channel);
        CLEAR_BIT(ADCx->SIGN,1<<ADC_Channel);   //If define differential mode ,set as unsign resault
    }

    if((Rank >= 1)&&(Rank <= 5)) 
        MODIFY_REG(ADCx->SQR1,(ADC_CH_MASK << (5*Rank )),(ADC_Channel << (5*Rank )));
    else if((Rank >= 6)&&(Rank <= 11))
        MODIFY_REG(ADCx->SQR2,(ADC_CH_MASK << (5*(Rank-6))),(ADC_Channel << (5*(Rank-6))));
    else if((Rank >= 12)&&(Rank <= 16))
        MODIFY_REG(ADCx->SQR3,(ADC_CH_MASK << (5*(Rank-12))),(ADC_Channel << (5*(Rank-12))));    

    /* Set the SMPR to every register*/
    if(ADC_Channel <= ADC_CHANNEL_7) 
        MODIFY_REG(ADCx->SMPR1,(ADC_SMPR_CH_MASK << (4*ADC_Channel )),(ADC_SampleTime << (4*ADC_Channel )));
    else if((ADC_Channel >= ADC_CHANNEL_8)&&(ADC_Channel <= ADC_CHANNEL_15))
        MODIFY_REG(ADCx->SMPR2,(ADC_SMPR_CH_MASK << (4*(ADC_Channel-8))),(ADC_SampleTime << (4*(ADC_Channel-8))));
    else if((ADC_Channel >= ADC_CHANNEL_TEMP)&&(ADC_Channel <= ADC_CHANNEL_EXT3))
        MODIFY_REG(ADCx->SMPR3,(ADC_SMPR_CH_MASK << (4*(ADC_Channel-16))),(ADC_SampleTime << (4*(ADC_Channel-16))));
    
    if(ADC_Channel == ADC_CHANNEL_TEMP)
    {
        SET_BIT(ADCx->TSREF,ADC_TSREF_EN_TS);//Enable the temperature sensor
        System_Delay(1000);
    }    
    
    if( (ADC_Channel == ADC_CHANNEL_VBGR) || 
        (ADC_Channel == ADC_CHANNEL_EXT2) ||
        (ADC_Channel == ADC_CHANNEL_EXT3) )
    {
        SET_BIT(ADCx->CR2,ADC_CR2_EN_BUF);//Enable the buffer
        System_Delay(1000);
    }
}

/**
  * @brief  Returns the last ADCx conversion result data for regular channel.
  * @param  ADCx: where x can be ADC to select the ADC peripheral.
  * @retval The Data conversion value.
  */
uint32_t ADC_GetConversionValue(ADC_TypeDef* ADCx)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    /* Return the selected ADC conversion value */
    return (uint32_t) ADCx->DR;
}

/**
  * @brief  Enables or disables the selected ADC injected channel
  *         conversion after regular one.
  * @param  ADCx: where x can be ADC to select the ADC peripheral.
  * @param  NewState: new state of the selected ADC auto injected conversion
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_InjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE)
    {
        /* Enable the selected ADC injected channel conversion */
        SET_BIT(ADCx->CR1, ADC_CR1_JEN);
    }
    else
    {
        /* Disable the selected ADC injected channel conversion */
        CLEAR_BIT(ADCx->CR1, ADC_CR1_JEN);
    }
}

/**
  * @brief  Configures the ADCx external trigger for injected channel conversion.
  * @param  ADCx: where x can be ADC to select the ADC peripheral.
  * @param  ADC_ExternalTrigInjecConv: specifies the ADC trigger to start injected conversion. 
  *   This parameter can be one of the following values:
  *     @arg ADC_SOFTWARE_START: Injected conversion started by software and not by external trigger
  *     @arg ADC_EXTERNAL_TIG1: External trig source 1 selected
  *     ...to ...
  *     @arg ADC_EXTERNAL_TIG7: External trig source 7 selected
  * @retval None
  */
void ADC_ExternalTrigInjectedConvConfig(ADC_TypeDef* ADCx, uint32_t ADC_ExternalTrigInjecConv)
{
    uint32_t tmpreg = 0;
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_ALL_TRIG(ADC_ExternalTrigInjecConv));

    //JExTrigSel set
    MODIFY_REG(ADCx->CR1, ADC_CR1_JEXTSEL_MASK, (ADC_ExternalTrigInjecConv<<ADC_CR1_JEXTSEL_POS));        
}

/**
  * @brief  Enables or disables the selected ADC start of the injected 
  *         channels conversion.
  * @param  ADCx: where x can be ADC to select the ADC peripheral.
  * @param  NewState: new state of the selected ADC software start injected conversion.
  *   This parameter can be: Only ENABLE.
  * @retval None
  */
void ADC_SoftwareStartInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE)
    {
        /* Enable the selected ADC conversion for injected group on external event and start the selected
        ADC injected conversion */
        /* Start covertion */
        SET_BIT(ADCx->CR1,ADC_CR1_JSWSTART);
    }
}

/**
  * @brief  Gets the selected ADC Software start injected conversion Status.
  * @param  ADCx: where x can be ADC to select the ADC peripheral.
  * @retval The new state of ADC software start injected conversion (SET or RESET).
  */
FlagStatus ADC_GetSoftwareStartInjectedConvCmdStatus(ADC_TypeDef* ADCx)
{
    FlagStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    /* Check the status of JSWSTART bit */
    if ((ADCx->CR1 & ADC_CR1_JSWSTART) != (uint32_t)RESET)
    {
        /* JSWSTART bit is set */
        bitstatus = SET;
    }
    else
    {
        /* JSWSTART bit is reset */
        bitstatus = RESET;
    }
    /* Return the JSWSTART bit status */
    return  bitstatus;
}

/**
  * @brief  Configures for the selected ADC Injected channel and its sample time.
  * @param  ADCx: where x can be ADC to select the ADC peripheral.
  * @param  ADC_Channel: the ADC channel to configure. 
  *   This parameter can be one of the following values:
  *     @arg ADC_CHANNEL_0: ADC Channel0 selected
  *     ...to ...
  *     @arg ADC_CHANNEL_15: ADC Channel15 selected
  *     @arg ADC_CHANNEL_TEMP: ADC Channel16 selected
  *     @arg ADC_CHANNEL_VBAT: ADC Channel17 selected
  *     @arg ADC_CHANNEL_VBGR: ADC Channel18 selected
  *     @arg ADC_CHANNEL_EXT2: ADC Channel19 selected
  *     @arg ADC_CHANNEL_EXT2: ADC Channel20 selected
  * @param  ADC_SampleTime: The sample time value to be set for the selected channel. 
  *   This parameter can be one of the following values:
  *     @arg ADC_SMP_CLOCK_3: Sample time equal to 3 cycles
  *     ...to ...
  *     @arg ADC_SMP_CLOCK_640: Sample time equal to 640 cycles	
  * @param  ADC_DiffMode: Specify the differential parameters. 
  *     @arg DISABLE:single end mode
  *     @arg ENABLE:differential end mode
  * @retval None
  */
void ADC_InjectedChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t ADC_SampleTime, FunctionalState ADC_DiffMode)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_ALL_CHANNEL(ADC_Channel));
    assert_param(IS_ADC_ALL_SMPCLOCK(ADC_SampleTime));
    assert_param(IS_FUNCTIONAL_STATE(ADC_DiffMode));
    /* Differential mode set*/
    if(ADC_DiffMode == ENABLE)
    {
        SET_BIT(ADCx->DIFF,1<<ADC_Channel);
        SET_BIT(ADCx->SIGN,1<<ADC_Channel);     //If define differential mode ,set as sign resault
    }
    else
    {
        CLEAR_BIT(ADCx->DIFF,1<<ADC_Channel);
        CLEAR_BIT(ADCx->SIGN,1<<ADC_Channel);   //If define differential mode ,set as unsign resault
    }

    /* Inject channel */
    MODIFY_REG(ADCx->JSQR,ADC_CH_MASK,ADC_Channel);

    /* Set the SMPR to every register*/
    if(ADC_Channel <= ADC_CHANNEL_7) 
        MODIFY_REG(ADCx->SMPR1,(ADC_SMPR_CH_MASK << (4*ADC_Channel )),(ADC_SampleTime << (4*ADC_Channel )));
    else if((ADC_Channel >= ADC_CHANNEL_8)&&(ADC_Channel <= ADC_CHANNEL_15))
        MODIFY_REG(ADCx->SMPR2,(ADC_SMPR_CH_MASK << (4*(ADC_Channel-8))),(ADC_SampleTime << (4*(ADC_Channel-8))));
    else if((ADC_Channel >= ADC_CHANNEL_TEMP)&&(ADC_Channel <= ADC_CHANNEL_EXT3))
        MODIFY_REG(ADCx->SMPR3,(ADC_SMPR_CH_MASK << (4*(ADC_Channel-16))),(ADC_SampleTime << (4*(ADC_Channel-16))));
    
    if(ADC_Channel == ADC_CHANNEL_TEMP)
    {
        SET_BIT(ADCx->TSREF,ADC_TSREF_EN_TS);//Enable the temperature sensor
        System_Delay(1000);
    }    
    
    if( (ADC_Channel == ADC_CHANNEL_VBGR) || 
        (ADC_Channel == ADC_CHANNEL_EXT2) ||
        (ADC_Channel == ADC_CHANNEL_EXT3) )
    {
        SET_BIT(ADCx->CR2,ADC_CR2_EN_BUF);//Enable the buffer
        System_Delay(1000);
    }
}

/**
  * @brief  Returns the ADC injected channel conversion result
  * @param  ADCx: where x can be ADC to select the ADC peripheral.
  * @retval The Data conversion value.
  */
uint32_t ADC_GetInjectedConversionValue(ADC_TypeDef* ADCx)
{  
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    /* Return the selected ADC conversion value */
    return ADCx->JDR;
}

/**
  * @brief  Enables or disables the analog watchdog on single/all regular
  *         or injected channels
  * @param  ADCx: where x can be ADC to select the ADC peripheral.
  * @param  ADC_AnalogWatchdog: the ADC analog watchdog configuration.
  *   This parameter can be one of the following values:
  *     @arg ADC_ANALOGWATCHDOG_RCH_SINGLE: Analog watchdog on a single regular channel
  *     @arg ADC_ANALOGWATCHDOG_JCH_SINGLE: Analog watchdog on a single injected channel
  *     @arg ADC_ANALOGWATCHDOG_RCH_OR_JCH_SINGLE: Analog watchdog on a single regular or injected channel
  *     @arg ADC_ANALOGWATCHDOG_RCH_ALL: Analog watchdog on  all regular channel
  *     @arg ADC_ANALOGWATCHDOG_JCH_ALL: Analog watchdog on  all injected channel
  *     @arg ADC_ANALOGWATCHDOG_RCH_AND_JCH_ALL: Analog watchdog on all regular and injected channels
  *     @arg ADC_AnalogWatchdog_None: No channel guarded by the analog watchdog
  * @retval None	  
  */
void ADC_AnalogWatchdogCmd(ADC_TypeDef* ADCx, uint32_t ADC_AnalogWatchdog)
{
    uint32_t tmpreg = 0;
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_ANALOG_WATCHDOG(ADC_AnalogWatchdog));
    
    switch(ADC_AnalogWatchdog)
    {
            /* AWDSGL:0; AWDEN:1; JAWDEN:0 */
        case ADC_ANALOGWATCHDOG_RCH_ALL:
            CLEAR_BIT(ADCx->CR1,ADC_CR1_AWDSGL);
            SET_BIT(ADCx->CR1,ADC_CR1_AWDEN);
            CLEAR_BIT(ADCx->CR1,ADC_CR1_JAWDEN);
            break;
            /* AWDSGL:0; AWDEN:0; JAWDEN:1 */
        case ADC_ANALOGWATCHDOG_JCH_ALL:
            CLEAR_BIT(ADCx->CR1,ADC_CR1_AWDSGL);
            CLEAR_BIT(ADCx->CR1,ADC_CR1_AWDEN);
            SET_BIT(ADCx->CR1,ADC_CR1_JAWDEN);
            break;
            /* AWDSGL:0; AWDEN:1; JAWDEN:1 */
        case ADC_ANALOGWATCHDOG_RCH_AND_JCH_ALL:
            CLEAR_BIT(ADCx->CR1,ADC_CR1_AWDSGL);
            SET_BIT(ADCx->CR1,ADC_CR1_AWDEN);
            SET_BIT(ADCx->CR1,ADC_CR1_JAWDEN);
            break;
            /* AWDSGL:1; AWDEN:1; JAWDEN:0 */
        case ADC_ANALOGWATCHDOG_RCH_SINGLE:
            SET_BIT(ADCx->CR1,ADC_CR1_AWDSGL);
            SET_BIT(ADCx->CR1,ADC_CR1_AWDEN);
            CLEAR_BIT(ADCx->CR1,ADC_CR1_JAWDEN);
            break;
            /* AWDSGL:1; AWDEN:0; JAWDEN:1 */
        case ADC_ANALOGWATCHDOG_JCH_SINGLE:
            SET_BIT(ADCx->CR1,ADC_CR1_AWDSGL);
            CLEAR_BIT(ADCx->CR1,ADC_CR1_AWDEN);
            SET_BIT(ADCx->CR1,ADC_CR1_JAWDEN);   
            break;
            /* AWDSGL:1; AWDEN:1; JAWDEN:1 */
        case ADC_ANALOGWATCHDOG_RCH_OR_JCH_SINGLE:
            SET_BIT(ADCx->CR1,ADC_CR1_AWDSGL);
            SET_BIT(ADCx->CR1,ADC_CR1_AWDEN);
            SET_BIT(ADCx->CR1,ADC_CR1_JAWDEN);
            break;

            /* AWDSGL:x; AWDEN:0; JAWDEN:0 */
        default: /* ADC_ANALOGWATCHDOG_NONE */
            CLEAR_BIT(ADCx->CR1,ADC_CR1_AWDEN);
            CLEAR_BIT(ADCx->CR1,ADC_CR1_JAWDEN);
            break;
    }
}

/**
  * @brief  Configures the high and low thresholds of the analog watchdog.
  * @param  ADCx: where x can be ADC to select the ADC peripheral.
  * @param  HighThreshold: the ADC analog watchdog High threshold value.
  *   This parameter must be a 12bit value.
  * @param  LowThreshold: the ADC analog watchdog Low threshold value.
  *   This parameter must be a 12bit value.
  * @retval None
  */
void ADC_AnalogWatchdogThresholdsConfig(ADC_TypeDef* ADCx, uint16_t HighThreshold,
                                        uint16_t LowThreshold)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_THRESHOLD(HighThreshold));
    assert_param(IS_ADC_THRESHOLD(LowThreshold));
    /* Set the ADCx high threshold */
    ADCx->HTR = HighThreshold;
    /* Set the ADCx low threshold */
    ADCx->LTR = LowThreshold;
}

/**
  * @brief  Configures the high and low thresholds of the differential mode analog watchdog.
  * @param  ADCx: where x can be ADC to select the ADC peripheral.
  * @param  HighThreshold: the ADC analog watchdog High threshold value.
  *   This parameter must be a 12bit value.
  * @param  LowThreshold: the ADC analog watchdog Low threshold value.
  *   This parameter must be a 12bit value.
  * @retval None
  */
void ADC_AnalogWatchdogDiffThresholdsConfig(ADC_TypeDef* ADCx, int16_t HighThreshold,
                                        int16_t LowThreshold)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_DIFF_THRESHOLD(HighThreshold));
    assert_param(IS_ADC_DIFF_THRESHOLD(LowThreshold));
    /* Set the ADCx high threshold */
    ADCx->HTR = HighThreshold<<16;
    /* Set the ADCx low threshold */
    ADCx->LTR = LowThreshold<<16;
}

/**
  * @brief  Configures the analog watchdog guarded Regular channel
  * @param  ADCx: where x can be ADC to select the ADC peripheral.
  * @param  ADC_Channel: the ADC channel to configure for the analog watchdog. 
  *   This parameter can be one of the following values:
  *     @arg ADC_CHANNEL_0: ADC Channel0 selected
  *     ...to ...
  *     @arg ADC_CHANNEL_15: ADC Channel15 selected
  *     @arg ADC_CHANNEL_TEMP: ADC Channel16 selected
  *     @arg ADC_CHANNEL_VBAT: ADC Channel17 selected
  *     @arg ADC_CHANNEL_VBGR: ADC Channel18 selected
  *     @arg ADC_CHANNEL_EXT2: ADC Channel19 selected
  *     @arg ADC_CHANNEL_EXT2: ADC Channel20 selected
  * @retval None
  */
void ADC_AnalogWatchdogRegularChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_ALL_CHANNEL(ADC_Channel));
    //The regular watchdog channel set
    MODIFY_REG(ADCx->CR1,ADC_CH_MASK,ADC_Channel);  
}

/**
  * @brief  Configures the analog watchdog guarded Injected channel
  * @param  ADCx: where x can be ADC to select the ADC peripheral.
  * @param  ADC_Channel: the ADC channel to configure for the analog watchdog. 
  *   This parameter can be one of the following values:
  *     @arg ADC_CHANNEL_0: ADC Channel0 selected
  *     ...to ...
  *     @arg ADC_CHANNEL_15: ADC Channel15 selected
  *     @arg ADC_CHANNEL_TEMP: ADC Channel16 selected
  *     @arg ADC_CHANNEL_VBAT: ADC Channel17 selected
  *     @arg ADC_CHANNEL_VBGR: ADC Channel18 selected
  *     @arg ADC_CHANNEL_EXT2: ADC Channel19 selected
  *     @arg ADC_CHANNEL_EXT2: ADC Channel20 selected
  * @retval None
  */
void ADC_AnalogWatchdogInjectedChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_ALL_CHANNEL(ADC_Channel));
    //The injected watchdog channel set
    MODIFY_REG(ADCx->CR1,(ADC_CH_MASK<<27),ADC_Channel<<27);  //The inject watchdog channel set
}

/**
  * @brief  Configures the over sample ratio and right bit shift.
  * @param  ADCx: where x can be ADC to select the ADC peripheral.
  * @param  Ratio: the over sample ratio. 
  *   This parameter can be one of the following values:
  *     @arg ADC_CR2_OVSR_2X: 2X over sample ratio
  *     ...to ...
  *     @arg ADC_CR2_OVSR_256X: 256X over sample ratio
  * @param  RightBitShift: the right bit shift. 
  *   This parameter can be one of the following values:
  *     @arg ADC_CR2_OVSS_0: 0 bit right shift
  *     ...to ...
  *     @arg ADC_CR2_OVSS_8: 8 bits right shift.
  * @retval None
  */
void ADC_OverSampleConfig(ADC_TypeDef* ADCx, uint8_t Ratio, uint8_t RightBitShift)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_ALL_OVSR(Ratio));
    assert_param(IS_ADC_ALL_OVSS(RightBitShift));

    MODIFY_REG(ADCx->CR2, ADC_CR2_OVSR_MASK, Ratio<<ADC_CR2_OVSR_POS);   //over sample rate
    MODIFY_REG(ADCx->CR2, ADC_CR2_OVSS_MASK, RightBitShift<<ADC_CR2_OVSS_POS); //over sample right shift.
}

/**
  * @brief  Enables or disables the ADC over sample mode for Regular channel. 
  * @param  ADCx: where x can be ADC to select the ADC peripheral.
  * @param  NewState: new state of the selected ADC over sample mode.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_OverSampleRegularCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    
    if (NewState != DISABLE)
    {
        /* Regular channel over sample enable */
        SET_BIT(ADCx->CR2,ADC_CR2_OVSE);
    }
    else
    {
        /* Regular channel over sample disable */
        CLEAR_BIT(ADCx->CR2,ADC_CR2_OVSE);
    }
}

/**
  * @brief  Enables or disables the ADC over sample mode for Injected channel. 
  * @param  ADCx: where x can be ADC to select the ADC peripheral.
  * @param  NewState: new state of the selected ADC over sample mode.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_OverSampleInjectedCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    
    if (NewState != DISABLE)
    {
        /* Injected channel over sample enable */
        SET_BIT(ADCx->CR2, ADC_CR2_JOVSE);
    }
    else
    {
        /* Injected channel over sample disable */
        CLEAR_BIT(ADCx->CR2, ADC_CR2_JOVSE);
    }
}

/**
  * @brief  Enables or disables the ADC over sample mode N times covert for Injected channel. 
  * @param  ADCx: where x can be ADC to select the ADC peripheral.
  * @param  NewState: new state of the selected ADC over sample mode.
  *   This parameter can be: ENABLE (N times covert) or DISABLE (1 times covert).
  * @retval None
  */
void ADC_OverSampleInjectedNTimesConfig(ADC_TypeDef* ADCx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    
    if (NewState != DISABLE)
    {
        /* Injected channel over sample N times covert enable */
        SET_BIT(ADCx->CR2, ADC_CR2_JTOVS);
    }
    else
    {
        /* Injected channel over sample N times covert disable */
        CLEAR_BIT(ADCx->CR2, ADC_CR2_JTOVS);
    }
}
/**
  * @brief  Enables or disables the Vref.
  * @param  ADCx: where x can be ADC to select the ADC peripheral.
  * @param  NewState: new state of the Vref.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_VrefCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE)
    {
        /* Enable the Vref*/
        SET_BIT(ADCx->TSREF,ADC_TSREF_VREFBI_EN);
    }
    else
    {
        /* Disable the Vref*/
        CLEAR_BIT(ADCx->TSREF,ADC_TSREF_VREFBI_EN);
    }
}

/**
  * @brief  Vref configuration.
  * @param  ADCx: where x can be ADC to select the ADC peripheral.
  * @param  NewState: new state of the Vref.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_VrefConfig(ADC_TypeDef* ADCx, uint8_t Vref)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_ALL_VREF(Vref));
    /*Set The Vref value */
    MODIFY_REG(ADCx->CR1, ADC_TSREF_VREFBI_SEL_MASK, Vref<<ADC_TSREF_VREFBI_SEL_POS);
}

/**
  * @brief  Get the specified ADC all flags.
  * @param  ADCx: where x can be ADC to select the ADC peripheral.
  * @retval The SR value.
  */
uint32_t ADC_GetStatus(ADC_TypeDef* ADCx)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    /* Return the SR status */
    return  ADCx->SR;
}

/**
  * @brief  Checks whether the specified ADC flag is set or not.
  * @param  ADCx: where x can be ADC to select the ADC peripheral.
  * @param  ADC_FLAG: specifies the flag to check. 
  *   This parameter can be one of the following values:
  *     @arg ADC_SR_AWD: Analog watchdog
  *     @arg ADC_SR_OVERF: Data over flow flag
  *     @arg ADC_SR_EOG: End of conversion group flag
  *     @arg ADC_SR_EOC: End of conversion flag
  *     @arg ADC_SR_JEOC: End of injected conversion flag
  *     @arg ADC_SR_ADRDY: The adc is ready flag
  * @retval The new state of ADC_FLAG (SET or RESET).
  */
FlagStatus ADC_GetFlagStatus(ADC_TypeDef* ADCx, uint8_t ADC_FLAG)
{
    FlagStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_SR_FLAG(ADC_FLAG));
    /* Check the status of the specified ADC flag */
    if ((ADCx->SR & ADC_FLAG) != (uint8_t)RESET)
    {
        /* ADC_FLAG is set */
        bitstatus = SET;
    }
    else
    {
        /* ADC_FLAG is reset */
        bitstatus = RESET;
    }
    /* Return the ADC_FLAG status */
    return  bitstatus;
}

/**
  * @brief  Clears the ADCx's pending flags.
  * @param  ADCx: where x can be ADC to select the ADC peripheral.
  * @param  ADC_FLAG: specifies the flag to check. 
  *   This parameter can be one of the following values:
  *     @arg ADC_SR_AWD: Analog watchdog
  *     @arg ADC_SR_OVERF: Data over flow flag
  *     @arg ADC_SR_EOG: End of conversion group flag
  *     @arg ADC_SR_EOC: End of conversion flag
  *     @arg ADC_SR_JEOC: End of injected conversion flag
  *     @arg ADC_SR_ADRDY: The adc is ready flag
  * @retval None
  */
void ADC_ClearFlag(ADC_TypeDef* ADCx, uint8_t ADC_FLAG)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_SR_FLAG(ADC_FLAG));
    /* Clear the selected ADC flags */
    ADCx->SR = ADC_FLAG;
}

FlagStatus ADC_GetITSource(ADC_TypeDef* ADCx, uint16_t ADC_IT)
{
    FlagStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_IT(ADC_IT));
    
    if ((ADCx->IE & ADC_IT) != (uint8_t)RESET)
    {
        /* ADC_IT is set */
        bitstatus = SET;
    }
    else
    {
        /* ADC_IT is reset */
        bitstatus = RESET;
    }
    /* Return the ADC_FLAG status */
    return  bitstatus;
}

