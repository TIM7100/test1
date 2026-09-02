/******************************************************************************
* @file      : fxx_dac.c
* @version   : 1.0
* @date      : 2023.6.1
* @brief     : DAC SPL module driver
******************************************************************************/
#include "fxx_dac.h"

/******************************************************************************
* @brief : Configures the selected DAC channel.
* @param : hdac : pointer to a DAC_HandleTypeDef structure that contains
*                      the configuration information for DAC module.
* @param : sConfig : DAC configuration structure
* @param : Channel : This parameter can be one of the following values:  @arg  DAC_CHANNEL_1   @argDAC_CHANNEL_2
* @return: HAL_StatusTypeDef
******************************************************************************/
void DAC_Init(DAC_TypeDef* DACx, uint32_t Channel, DAC_InitTypeDef* DAC_InitStruct)
{
    uint32_t tmpreg1, tmpreg2;
    uint32_t tickstart = 0U;
    uint32_t ConnectOnChipPeripheral = 0U;
    uint32_t connectOnChip;
    
    /* Check the DAC parameters */
    assert_param(IS_DAC_ALL_PERIPH(DACx));
    assert_param(IS_DAC_TRIGGER(DAC_InitStruct->DAC_Trigger));
    assert_param(IS_DAC_OUTPUT_BUFFER_STATE(DAC_InitStruct->DAC_OutputBuffer));
    assert_param(IS_DAC_TRIMMING(DAC_InitStruct->DAC_UserTrimming));
    if ((DAC_InitStruct->DAC_UserTrimming) == DAC_TRIMMING_USER)
    {
        assert_param(IS_DAC_TRIMMINGVALUE(DAC_InitStruct->DAC_TrimmingValue));
    }
    assert_param(IS_DAC_SAMPLEANDHOLD(DAC_InitStruct->DAC_SampleAndHold));

    if(DAC_InitStruct->DAC_OutputBuffer == DAC_OUTPUTBUFFER_ENABLE)
    {
         /* output buffer on */
        assert_param(IS_DAC_CHIP_CONNECTION_BUFON(DAC_InitStruct->DAC_ConnectOnChipPeripheral));
    }
    else
    {
        /* output buffer off */
        if((DAC_InitStruct->DAC_SampleAndHold) == DAC_SAMPLEANDHOLD_ENABLE)
        {
            /* Sample and hold mode */
            assert_param(IS_DAC_CHIP_CONNECTION_SHMODE_BUFOFF(DAC_InitStruct->DAC_ConnectOnChipPeripheral));
            
            assert_param(IS_DAC_SAMPLETIME(DAC_InitStruct->DAC_SampleAndHoldConfig.DAC_SampleTime));
            assert_param(IS_DAC_HOLDTIME(DAC_InitStruct->DAC_SampleAndHoldConfig.DAC_HoldTime));
            assert_param(IS_DAC_REFRESHTIME(DAC_InitStruct->DAC_SampleAndHoldConfig.DAC_RefreshTime));
        }
        else
        {
            /* Normal mode */
             assert_param(IS_DAC_CHIP_CONNECTION_NORMALMODE_BUFOFF(DAC_InitStruct->DAC_ConnectOnChipPeripheral));
        }
    }
        
    assert_param(IS_DAC_CHANNEL(Channel));

    /* ## sample, hold, refresh ---------------------------------------------------------- */
    if (DAC_InitStruct->DAC_SampleAndHold == DAC_SAMPLEANDHOLD_ENABLE)
    /* Sample on old configuration */
    {
        /* SampleTime */
        if (Channel == DAC_CHANNEL_1)
        {
            DACx->SHSR1 = DAC_InitStruct->DAC_SampleAndHoldConfig.DAC_SampleTime;
        }
        else /* Channel 2 */

        DACx->SHSR2 = DAC_InitStruct->DAC_SampleAndHoldConfig.DAC_SampleTime;

        /* HoldTime */
        MODIFY_REG(DACx->SHHR, DAC_SHHR_THOLD1 << (Channel & 0x10UL), (DAC_InitStruct->DAC_SampleAndHoldConfig.DAC_HoldTime) << (Channel & 0x10UL));
        /* RefreshTime */
        MODIFY_REG(DACx->SHRR, DAC_SHRR_TREFRESH1 << (Channel & 0x10UL), (DAC_InitStruct->DAC_SampleAndHoldConfig.DAC_RefreshTime) << (Channel & 0x10UL));
    }

    /* ## CCR---------------------------------------------------------------------------- */
    if (DAC_InitStruct->DAC_UserTrimming == DAC_TRIMMING_USER)
    /* USER TRIMMING */
    {
        /* Get the DAC CCR value */
        tmpreg1 = DACx->CCR;
        /* Clear trimming value */
        tmpreg1 &= ~(((uint32_t)(DAC_CCR_OTRIM1)) << (Channel & 0x10UL));
        /* Configure for the selected trimming offset */
        tmpreg2 = DAC_InitStruct->DAC_TrimmingValue;
        /* Calculate CCR register value depending on DAC_Channel */
        tmpreg1 |= tmpreg2 << (Channel & 0x10UL);
        /* Write to DAC CCR */
        DACx->CCR = tmpreg1;
    }
    else
    {
        /*  factory trimming in NVR,read to DAC_CCR */
        uint32_t OTRIM=*(uint32_t *)(0x80248);
        uint32_t OTRIM_high=(OTRIM&0xffff0000)>>16;
        uint32_t OTRIM_low=(OTRIM&0xffff);
        if (OTRIM_low==((~OTRIM_high)&0xffff))
        {
            tmpreg1=(OTRIM_low&0x1f)|(((OTRIM_low&0x3E0)>>5)<<16);
            DACx->CCR = tmpreg1;  
        }
    }

    /* ## MCR---------------------------------------------------------------------------- */
    /* Get the DAC MCR value */
    tmpreg1 = DACx->MCR;
    /* Clear DAC_MCR_MODEx bits */
    tmpreg1 &= ~(((uint32_t)(DAC_MCR_MODE1)) << (Channel & 0x10UL));
    /* Configure for the selected DAC channel: mode, buffer output & on chip peripheral connect */

    if (DAC_InitStruct->DAC_ConnectOnChipPeripheral == DAC_CHIPCONNECT_EXTERNAL)
    {
        connectOnChip = 0x00000000UL;
    }
    else if (DAC_InitStruct->DAC_ConnectOnChipPeripheral == DAC_CHIPCONNECT_INTERNAL)
    {
        connectOnChip = DAC_MCR_MODE1_0;
    }
    else /* (DAC_InitStruct->DAC_ConnectOnChipPeripheral == DAC_CHIPCONNECT_BOTH) */
    {
        if(DAC_InitStruct->DAC_SampleAndHold == DAC_SAMPLEANDHOLD_ENABLE)
        {
            if (DAC_InitStruct->DAC_OutputBuffer == DAC_OUTPUTBUFFER_ENABLE)
            {
                connectOnChip = DAC_MCR_MODE1_0;
            }
            else
            {
                connectOnChip = 0x00000000UL;
            }
        }
        else
        {
            connectOnChip = DAC_MCR_MODE1_0;
        }
    }

    /* ## CR ---------------------------------------------------------------------------- */
    /* DAC in normal operating mode hence clear DAC Calibraion bit DAC_CR_CENx */
    CLEAR_BIT(DACx->CR, DAC_CR_CEN1 << (Channel & 0x10UL));

    /* Get the DAC CR value */
    tmpreg1 = DACx->CR;
    /* Clear TENx, TSELx, WAVEx and MAMPx bits */
    tmpreg1 &= ~(((uint32_t)(DAC_CR_MAMP1 | DAC_CR_WAVE1 | DAC_CR_TSEL1 | DAC_CR_TEN1)) << (Channel & 0x10UL));
    /* Configure for the selected DAC channel: trigger */
    /* Set TSELx and TENx bits according to DAC_Trigger value */
    tmpreg2 = DAC_InitStruct->DAC_Trigger;
    /* Calculate CR register value depending on DAC_Channel */
    tmpreg1 |= tmpreg2 << (Channel & 0x10UL);
    /* Write to DAC CR */
    DACx->CR = tmpreg1;

    /* Disable wave generation */
    DACx->CR &= ~(DAC_CR_WAVE1 << (Channel & 0x10UL));

}

/**
  * @brief  Enables or disables the selected DAC channel software trigger.
  * @param  DACx: where x can be 1 or 2 to select the DAC peripheral.  
  * @param  DAC_Channel: the selected DAC channel. 
  *          This parameter can be one of the following values:
  *            @arg DAC_Channel_1: DAC Channel1 selected
  *            @arg DAC_Channel_2: DAC Channel2 selected
  * @param  NewState: new state of the selected DAC channel software trigger.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void DAC_SoftwareTriggerCmd(DAC_TypeDef* DACx, uint32_t DAC_Channel, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_DAC_ALL_PERIPH(DACx));
  assert_param(IS_DAC_CHANNEL(DAC_Channel));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable software trigger for the selected DAC channel */
    DACx->SWTRIGR |= (uint32_t)DAC_SWTRIGR_SWTRIG1 << (DAC_Channel >> 4);
  }
  else
  {
    /* Disable software trigger for the selected DAC channel */
    DACx->SWTRIGR &= ~((uint32_t)DAC_SWTRIGR_SWTRIG1 << (DAC_Channel >> 4));
  }
}
/**
  * @brief  Enables or disables the specified DAC channel DMA request.
  *         When enabled DMA1 is generated when an external trigger (EXTI Line9,
  *         TIM2, TIM4, TIM6, TIM7 or TIM9  but not a software trigger) occurs
  * @param  DACx: where x can be 1 or 2 to select the DAC peripheral.
  * @param  DAC_Channel: the selected DAC channel.
  *          This parameter can be one of the following values:
  *            @arg DAC_Channel_1: DAC Channel1 selected
  *            @arg DAC_Channel_2: DAC Channel2 selected
  * @param  NewState: new state of the selected DAC channel DMA request.
  *          This parameter can be: ENABLE or DISABLE.
  * @note The DAC channel1 (channel2) is mapped on DMA1 channel3 (channel4) which 
  *       must be already configured. 
  * @retval None
  */
void DAC_DMACmd(DAC_TypeDef* DACx, uint32_t DAC_Channel, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_DAC_ALL_PERIPH(DACx));
  assert_param(IS_DAC_CHANNEL(DAC_Channel));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected DAC channel DMA request */
    DACx->CR |= (DAC_CR_DMAEN1 << DAC_Channel);
  }
  else
  {
    /* Disable the selected DAC channel DMA request */
    DACx->CR &= (~(DAC_CR_DMAEN1 << DAC_Channel));
  }
}
/**
  * @brief  Enables or disables the specified DAC channel.
  * @param  DACx: where x can be 1 or 2 to select the DAC peripheral.  
  * @param  DAC_Channel: The selected DAC channel. 
  *          This parameter can be one of the following values:
  *            @arg DAC_Channel_1: DAC Channel1 selected
  *            @arg DAC_Channel_2: DAC Channel2 selected
  * @param  NewState: new state of the DAC channel. 
  *          This parameter can be: ENABLE or DISABLE.
  * @note   When the DAC channel is enabled the trigger source can no more
  *         be modified.
  * @retval None
  */
void DAC_Cmd(DAC_TypeDef* DACx, uint32_t DAC_Channel, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_DAC_ALL_PERIPH(DACx));
  assert_param(IS_DAC_CHANNEL(DAC_Channel));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected DAC channel */
    DACx->CR |= (DAC_CR_EN1 << DAC_Channel);
  }
  else
  {
    /* Disable the selected DAC channel */
    DACx->CR &= (~(DAC_CR_EN1 << DAC_Channel));
  }
}
/**
  * @brief  Enables or disables simultaneously the two DAC channels software
  *         triggers.
  * @param  DACx: where x can be 1 to select the DAC1 peripheral.
  * @note   Dual trigger is not applicable for DAC2 (DAC2 integrates one channel).
  * @param  NewState: new state of the DAC channels software triggers.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void DAC_DualSoftwareTriggerCmd(DAC_TypeDef* DACx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_DAC_ALL_PERIPH(DACx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable software trigger for both DAC channels */
    DACx->SWTRIGR |= DAC_SWTRIGR_SWTRIG1|DAC_SWTRIGR_SWTRIG2;
  }
  else
  {
    /* Disable software trigger for both DAC channels */
    DACx->SWTRIGR &= ~(DAC_SWTRIGR_SWTRIG1|DAC_SWTRIGR_SWTRIG2);
  }
}
/**
  * @brief  Clears the DAC channel's pending flags.
  * @param  DACx: where x can be 1 or 2 to select the DAC peripheral.  
  * @param  DAC_Channel: the selected DAC channel. 
  *          This parameter can be one of the following values:
  *            @arg DAC_Channel_1: DAC Channel1 selected
  *            @arg DAC_Channel_2: DAC Channel2 selected
  * @param  DAC_FLAG: specifies the flag to clear. 
  *          This parameter can be:
  *            @arg DAC_FLAG_DMAUDR: DMA underrun flag                          
  * @retval None
  */
void DAC_ClearFlag(DAC_TypeDef* DACx, uint32_t DAC_Channel, uint32_t DAC_FLAG)
{
  /* Check the parameters */
  assert_param(IS_DAC_ALL_PERIPH(DACx));
  assert_param(IS_DAC_CHANNEL(DAC_Channel));
  assert_param(IS_DAC_FLAG(DAC_FLAG));

  /* Clear the selected DAC flags */
  DACx->SR = (DAC_FLAG << DAC_Channel);
}
/**
  * @brief  Clears the DAC channel's interrupt pending bits.
  * @param  DACx: where x can be 1 or 2 to select the DAC peripheral.
  * @param  DAC_Channel: the selected DAC channel. 
  *          This parameter can be one of the following values:
  *            @arg DAC_Channel_1: DAC Channel1 selected
  *            @arg DAC_Channel_2: DAC Channel2 selected
  * @param  DAC_IT: specifies the DAC interrupt pending bit to clear.
  *          This parameter can be the following values:
  *            @arg DAC_IT_DMAUDR: DMA underrun interrupt mask
  * @retval None
  */
void DAC_ClearITPendingBit(DAC_TypeDef* DACx, uint32_t DAC_Channel, uint32_t DAC_IT)
{
  /* Check the parameters */
  assert_param(IS_DAC_ALL_PERIPH(DACx));
  assert_param(IS_DAC_CHANNEL(DAC_Channel));
  assert_param(IS_DAC_IT(DAC_IT)); 

  /* Clear the selected DAC interrupt pending bits */
  DACx->SR = (DAC_IT << DAC_Channel);
}
/**
  * @brief  Checks whether the specified DAC flag is set or not.
  * @param  DACx: where x can be 1 or 2 to select the DAC peripheral.  
  * @param  DAC_Channel: thee selected DAC channel. 
  *          This parameter can be one of the following values:
  *            @arg DAC_Channel_1: DAC Channel1 selected
  *            @arg DAC_Channel_2: DAC Channel2 selected
  * @param  DAC_FLAG: specifies the flag to check. 
  *          This parameter can be:
  *            @arg DAC_FLAG_DMAUDR: DMA underrun flag
  * @note   The DMA underrun occurs when a second external trigger arrives before
  *         the acknowledgement for the first external trigger is received (first request).
  * @retval The new state of DAC_FLAG (SET or RESET).
  */
FlagStatus DAC_GetFlagStatus(DAC_TypeDef* DACx, uint32_t DAC_Channel, uint32_t DAC_FLAG)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters */
  assert_param(IS_DAC_ALL_PERIPH(DACx));
  assert_param(IS_DAC_CHANNEL(DAC_Channel));
  assert_param(IS_DAC_FLAG(DAC_FLAG));

  /* Check the status of the specified DAC flag */
  if ((DACx->SR & (DAC_FLAG << DAC_Channel)) != (uint8_t)RESET)
  {
    /* DAC_FLAG is set */
    bitstatus = SET;
  }
  else
  {
    /* DAC_FLAG is reset */
    bitstatus = RESET;
  }
  /* Return the DAC_FLAG status */
  return  bitstatus;
}
/**
  * @brief  Checks whether the specified DAC interrupt has occurred or not.
  * @param  DACx: where x can be 1 or 2 to select the DAC peripheral.  
  * @param  DAC_Channel: the selected DAC channel. 
  *          This parameter can be one of the following values:
  *            @arg DAC_Channel_1: DAC Channel1 selected
  *            @arg DAC_Channel_2: DAC Channel2 selected
  * @param  DAC_IT: specifies the DAC interrupt source to check. 
  *          This parameter can be:
  *            @arg DAC_IT_DMAUDR: DMA underrun interrupt mask
  * @note   The DMA underrun occurs when a second external trigger arrives before
  *         the acknowledgement for the first external trigger is received (first request).
  * @retval The new state of DAC_IT (SET or RESET).
  */
ITStatus DAC_GetITStatus(DAC_TypeDef* DACx, uint32_t DAC_Channel, uint32_t DAC_IT)
{
  ITStatus bitstatus = RESET;
  uint32_t enablestatus = 0;
  
  /* Check the parameters */
  assert_param(IS_DAC_ALL_PERIPH(DACx));
  assert_param(IS_DAC_CHANNEL(DAC_Channel));
  assert_param(IS_DAC_IT(DAC_IT));

  /* Get the DAC_IT enable bit status */
  enablestatus = (DACx->CR & (DAC_IT << DAC_Channel)) ;
  
  /* Check the status of the specified DAC interrupt */
  if (((DACx->SR & (DAC_IT << DAC_Channel)) != (uint32_t)RESET) && enablestatus)
  {
    /* DAC_IT is set */
    bitstatus = SET;
  }
  else
  {
    /* DAC_IT is reset */
    bitstatus = RESET;
  }
  /* Return the DAC_IT status */
  return  bitstatus;
}

/******************************************************************************
* @brief : Set the specified data holding register value for DAC channel.
* @param : hdac : pointer to a DAC_HandleTypeDef structure that contains
*                      the configuration information for DAC module.
* @param : Channel : This parameter can be one of the following values:  @arg  DAC_CHANNEL_1   @argDAC_CHANNEL_2
* @param : Alignment: Specifies the data alignment for DAC channel.This parameter can be one of the following values:
*          @arg DAC_ALIGN_8B_R   @arg DAC_ALIGN_12B_L   @arg DAC_ALIGN_12B_R 
* @param : Data: The destination peripheral data. 
* @return: HAL_StatusTypeDef
******************************************************************************/
void DAC_SetChannel1Data(DAC_TypeDef* DACx, uint32_t DAC_Align, uint32_t Data)
{
    __IO uint32_t tmp = 0;

    /* Check the parameters */
    assert_param(IS_DAC_ALL_PERIPH(DACx));
    assert_param(IS_DAC_ALIGN(DAC_Align)); 

    tmp = (uint32_t)DACx;

    tmp += DAC_DHR12R1_ALIGNMENT(DAC_Align);

    /* Calculate and set dual DAC data holding register value */
    if (DAC_Align == DAC_ALIGN_12B_L)
    {
        Data = (uint32_t)Data << 4;
    }
    /* Set the DAC channel selected data holding register */
    *(__IO uint32_t *) tmp = Data;
}

/******************************************************************************
* @brief : Set the specified data holding register value for DAC channel.
* @param : hdac : pointer to a DAC_HandleTypeDef structure that contains
*                      the configuration information for DAC module.
* @param : Channel : This parameter can be one of the following values:  @arg  DAC_CHANNEL_1   @argDAC_CHANNEL_2
* @param : Alignment: Specifies the data alignment for DAC channel.This parameter can be one of the following values:
*          @arg DAC_ALIGN_8B_R   @arg DAC_ALIGN_12B_L   @arg DAC_ALIGN_12B_R 
* @param : Data: The destination peripheral data. 
* @return: HAL_StatusTypeDef
******************************************************************************/
void DAC_SetChannel2Data(DAC_TypeDef* DACx, uint32_t DAC_Align, uint32_t Data)
{
    __IO uint32_t tmp = 0;

    /* Check the parameters */
    assert_param(IS_DAC_ALL_PERIPH(DACx));
    assert_param(IS_DAC_ALIGN(DAC_Align)); 

    tmp = (uint32_t)DACx;

    tmp += DAC_DHR12R1_ALIGNMENT(DAC_Align);

    /* Calculate and set dual DAC data holding register value */
    if (DAC_Align == DAC_ALIGN_12B_L)
    {
        Data = (uint32_t)Data << 4;
    }
    /* Set the DAC channel selected data holding register */
    *(__IO uint32_t *) tmp = Data;
}
/******************************************************************************
* @brief : Set the specified data holding register value for dual DAC channel.
* @param : hdac : pointer to a DAC_HandleTypeDef structure that contains
*                      the configuration information for DAC module.
* @param : Channel : This parameter can be one of the following values:  @arg  DAC_CHANNEL_1   @argDAC_CHANNEL_2
* @param : Alignment: Specifies the data alignment for DAC channel.This parameter can be one of the following values:
*          @arg DAC_ALIGN_8B_R   @arg DAC_ALIGN_12B_L   @arg DAC_ALIGN_12B_R 
* @param : Data1: The destination peripheral data1. 
* @param : Data2: The destination peripheral data2. 
* @return: HAL_StatusTypeDef
******************************************************************************/
void DAC_SetDualChannelData(DAC_TypeDef* DACx, uint32_t DAC_Align, uint16_t Data2, uint16_t Data1)
{
    uint32_t data, tmp;
    /* Check the parameters */
    assert_param(IS_DAC_ALL_PERIPH(DACx));
    assert_param(IS_DAC_ALIGN(DAC_Align)); 

    /* Calculate and set dual DAC data holding register value */
    if (DAC_Align == DAC_ALIGN_12B_L)
    {
        data = ((uint32_t)Data2 << 20U) | (Data1<<4);
    }
    else
    {
        data = ((uint32_t)Data2 << 16U) | Data1;
    }
    
    /* DAC 寄存器基地址 */
    tmp = (uint32_t)DACx;
    /* 根据DAC_Align，加上对应的寄存器的偏移，如DAC_DHR12RD， DAC_DHR12LD， DAC_DHR8RD  */
    tmp += DAC_DHR12RD_ALIGNMENT(DAC_Align);

    /* Set the dual DAC selected data holding register */
    *(__IO uint32_t *)tmp = data;
}

/******************************************************************************
* @brief : Returns the last data output value of the selected DAC channel.
* @param : hdac : pointer to a DAC_HandleTypeDef structure that contains
*                      the configuration information for DAC module.
* @param : Channel : This parameter can be one of the following values:  @arg  DAC_CHANNEL_1   @argDAC_CHANNEL_2
* @return: The selected DAC channel data output value
******************************************************************************/
uint32_t DAC_GetDataOutputValue(DAC_TypeDef* DACx, uint32_t Channel)
{
    /* Check the parameters */
    assert_param(IS_DAC_ALL_PERIPH(DACx));
    assert_param(IS_DAC_CHANNEL(Channel)); 

    /* Returns the DAC channel data output register value */
    if(Channel == DAC_CHANNEL_1)
    {
        return DACx->DOR1;
    }
    else
    {
        return DACx->DOR2;
    }
}

/******************************************************************************
* @brief : Returns the last data output value of two DAC channels.
* @param : hdac : pointer to a DAC_HandleTypeDef structure that contains
*                      the configuration information for DAC module.
* @return: The two DAC channels data output value
******************************************************************************/
uint32_t DAC_GetDualChannelData(DAC_TypeDef* DACx)
{
    /* Check the parameters */
    assert_param(IS_DAC_ALL_PERIPH(DACx));
    uint32_t tmp = 0U;

    tmp |= DACx->DOR1;

    tmp |= DACx->DOR2 << 16U;

    /* Returns the DAC channel data output register value */
    return tmp;
}

/******************************************************************************
* @brief : Enable or disable the selected DAC channel wave generation.
* @param : hdac : pointer to a DAC_HandleTypeDef structure that contains
*                      the configuration information for DAC module.
* @param : Channel:The selected DAC channel. his parameter can be one of the following values:
*                      @arg DAC_CHANNEL_1: DAC Channel1 selected
*                      @arg DAC_CHANNEL_2: DAC Channel2 selected
* @param : Amplitude:  Amplitude Select max triangle amplitude. This parameter can be one of the following values:
*                      @arg DAC_TRIANGLEAMPLITUDE_1: Select max triangle amplitude of 1
*                      @arg DAC_TRIANGLEAMPLITUDE_3: Select max triangle amplitude of 3
*                      @arg DAC_TRIANGLEAMPLITUDE_7: Select max triangle amplitude of 7
*                      @arg DAC_TRIANGLEAMPLITUDE_15: Select max triangle amplitude of 15
*                      @arg DAC_TRIANGLEAMPLITUDE_31: Select max triangle amplitude of 31
*                      @arg DAC_TRIANGLEAMPLITUDE_63: Select max triangle amplitude of 63
*                      @arg DAC_TRIANGLEAMPLITUDE_127: Select max triangle amplitude of 127
*                      @arg DAC_TRIANGLEAMPLITUDE_255: Select max triangle amplitude of 255
*                      @arg DAC_TRIANGLEAMPLITUDE_511: Select max triangle amplitude of 511
*                      @arg DAC_TRIANGLEAMPLITUDE_1023: Select max triangle amplitude of 1023
*                      @arg DAC_TRIANGLEAMPLITUDE_2047: Select max triangle amplitude of 2047
*                      @arg DAC_TRIANGLEAMPLITUDE_4095: Select max triangle amplitude of 4095
* @return: HAL_StatusTypeDef
******************************************************************************/
void SPL_DACEx_TriangleWaveGenerate(DAC_TypeDef* DACx, uint32_t Channel, uint32_t Amplitude)
{
    /* Check the parameters */
    assert_param(IS_DAC_ALL_PERIPH(DACx));
    assert_param(IS_DAC_CHANNEL(Channel));
    assert_param(IS_DAC_LFSR_UNMASK_TRIANGLE_AMPLITUDE(Amplitude));
    /* Enable the triangle wave generation for the selected DAC channel */
    MODIFY_REG(DACx->CR, ((DAC_CR_WAVE1) | (DAC_CR_MAMP1)) << (Channel & 0x10UL), (DAC_CR_WAVE_TRIANGLE | Amplitude) << (Channel & 0x10UL));
}

/******************************************************************************
* @brief : Enable or disable the selected DAC channel wave generation.
* @param : hdac: pointer to a DAC_HandleTypeDef structure that contains
*                      the configuration information for DAC module.
* @param : Channel: The selected DAC channel. his parameter can be one of the following values:
*                      @arg DAC_CHANNEL_1: DAC Channel1 selected
*                      @arg DAC_CHANNEL_2: DAC Channel2 selected
* @param : Amplitude: Amplitude Unmask DAC channel LFSR for noise wave generation. This parameter can be one of the following values:
*                     @arg DAC_LFSRUNMASK_BIT0: Unmask DAC channel LFSR bit0 for noise wave generation
*                     @arg DAC_LFSRUNMASK_BITS1_0: Unmask DAC channel LFSR bit[1:0] for noise wave generation
*                     @arg DAC_LFSRUNMASK_BITS2_0: Unmask DAC channel LFSR bit[2:0] for noise wave generation
*                     @arg DAC_LFSRUNMASK_BITS3_0: Unmask DAC channel LFSR bit[3:0] for noise wave generation
*                     @arg DAC_LFSRUNMASK_BITS4_0: Unmask DAC channel LFSR bit[4:0] for noise wave generation
*                     @arg DAC_LFSRUNMASK_BITS5_0: Unmask DAC channel LFSR bit[5:0] for noise wave generation
*                     @arg DAC_LFSRUNMASK_BITS6_0: Unmask DAC channel LFSR bit[6:0] for noise wave generation
*                     @arg DAC_LFSRUNMASK_BITS7_0: Unmask DAC channel LFSR bit[7:0] for noise wave generation
*                     @arg DAC_LFSRUNMASK_BITS8_0: Unmask DAC channel LFSR bit[8:0] for noise wave generation
*                     @arg DAC_LFSRUNMASK_BITS9_0: Unmask DAC channel LFSR bit[9:0] for noise wave generation
*                     @arg DAC_LFSRUNMASK_BITS10_0: Unmask DAC channel LFSR bit[10:0] for noise wave generation
*                     @arg DAC_LFSRUNMASK_BITS11_0: Unmask DAC channel LFSR bit[11:0] for noise wave generation
* @return: HAL_StatusTypeDef
******************************************************************************/
void SPL_DACEx_NoiseWaveGenerate(DAC_TypeDef* DACx, uint32_t Channel, uint32_t Amplitude)
{
    /* Check the parameters */
    assert_param(IS_DAC_ALL_PERIPH(DACx));
    assert_param(IS_DAC_CHANNEL(Channel));
    assert_param(IS_DAC_LFSR_UNMASK_TRIANGLE_AMPLITUDE(Amplitude));
    /* Enable the noise wave generation for the selected DAC channel */
    MODIFY_REG(DACx->CR, ((DAC_CR_WAVE1) | (DAC_CR_MAMP1)) << (Channel & 0x10UL), (DAC_CR_WAVE_NOISE | Amplitude) << (Channel & 0x10UL));
}

/******************************************************************************
* @brief : Run the self calibration of one DAC channel.
* @param : hdac : pointer to a DAC_HandleTypeDef structure that contains
*                      the configuration information for DAC module.
* @param : sConfig : sConfig DAC channel configuration structure
* @param : Channel : The selected DAC channel. his parameter can be one of the following values:
*                      @arg DAC_CHANNEL_1: DAC Channel1 selected
*                      @arg DAC_CHANNEL_2: DAC Channel2 selected
* @return: HAL_StatusTypeDef
******************************************************************************/
void SPL_DACEx_SelfCalibrate(DAC_TypeDef* DACx, DAC_InitTypeDef *sConfig, uint32_t Channel)
{    
    __IO uint32_t tmp;
    uint32_t trimmingvalue;
    uint32_t laststatus = 0;
    uint32_t nowstatus = 0;
    
    /* Check the parameters */
    assert_param(IS_DAC_ALL_PERIPH(DACx));
    assert_param(IS_DAC_CHANNEL(Channel));

    SET_BIT((DACx->CR), (DAC_CR_EN1 << (Channel & 0x10UL)));
    tmp = (uint32_t)DACx;
    if (Channel == DAC_CHANNEL_1)
    {
        tmp += DAC_DHR12R1_ALIGNMENT(DAC_ALIGN_12B_R);
    }
    else
    {
        tmp += DAC_DHR12R2_ALIGNMENT(DAC_ALIGN_12B_R);
    }

    *(__IO uint32_t *) tmp = 0x0800U;

    /* Enable the selected DAC channel calibration */
    /* i.e. set DAC_CR_CENx bit */
    SET_BIT((DACx->CR), (DAC_CR_CEN1 << (Channel & 0x10UL)));

    /* Init trimming counter */
    /* Medium value ,trimmingvalue:0-31(0x1f)*/
    for(trimmingvalue=0;trimmingvalue<32;trimmingvalue++)
    {
        /* Set candidate trimming */
        MODIFY_REG(DACx->CCR, (DAC_CCR_OTRIM1 << (Channel & 0x10UL)), (trimmingvalue << (Channel & 0x10UL)));
        
        System_Delay(50000);
        
        laststatus=nowstatus;
        nowstatus=(DACx->SR & (DAC_SR_CALFLAG1 << (Channel & 0x10UL)))>>(DAC_SR_CALFLAG1_Pos +Channel);
        /* tOFFTRIMmax delay x ms as per datasheet (electrical characteristics */
        /* i.e. minimum time needed between two calibration steps */
        if (nowstatus==1&&laststatus==0)
        {
            break;
        }
    }

    /* Disable the selected DAC channel calibration */
    /* i.e. clear DAC_CR_CENx bit */
    CLEAR_BIT((DACx->CR), (DAC_CR_CEN1 << (Channel & 0x10UL)));

    /* Disable the selected DAC channel */
    CLEAR_BIT((DACx->CR), (DAC_CR_EN1 << (Channel & 0x10UL)));

    sConfig->DAC_TrimmingValue = trimmingvalue;
    sConfig->DAC_UserTrimming = DAC_TRIMMING_USER;

}



/******************************************************************************
* @brief : Set the trimming mode and trimming value (user trimming mode applied).
* @param : hdac : pointer to a DAC_HandleTypeDef structure that contains
*                      the configuration information for DAC module.
* @param : sConfig : sConfig DAC channel configuration structure
* @param : Channel : The selected DAC channel. his parameter can be one of the following values:
*                      @arg DAC_CHANNEL_1: DAC Channel1 selected
*                      @arg DAC_CHANNEL_2: DAC Channel2 selected
* @param : NewTrimmingValue: DAC new trimming value
* @return: HAL_StatusTypeDef
******************************************************************************/
void SPL_DACEx_SetUserTrimming(DAC_TypeDef* DACx, DAC_InitTypeDef *sConfig, uint32_t Channel, uint32_t NewTrimmingValue)
{
    /* Check the parameters */
    assert_param(IS_DAC_ALL_PERIPH(DACx));
    assert_param(IS_DAC_CHANNEL(Channel));
    assert_param(IS_DAC_Calibration_TRIM(NewTrimmingValue));

    /* Check the DAC handle allocation */
    if (DACx == NULL)
    {
        return;
    }
    else
    {
        /* Set new trimming */
        MODIFY_REG(DACx->CCR, (DAC_CCR_OTRIM1 << (Channel & 0x10UL)), (NewTrimmingValue << (Channel & 0x10UL)));
        /* Update trimming mode */
        sConfig->DAC_UserTrimming = DAC_TRIMMING_USER;
        sConfig->DAC_TrimmingValue = NewTrimmingValue;
    }
}



/******************************************************************************
* @brief : Return the DAC trimming value.
* @param : hdac : pointer to a DAC_HandleTypeDef structure that contains
*                      the configuration information for DAC module.
* @param : Channel : The selected DAC channel. his parameter can be one of the following values:
*                      @arg DAC_CHANNEL_1: DAC Channel1 selected
*                      @arg DAC_CHANNEL_2: DAC Channel2 selected
* @return: Trimming value : range: 0->31
******************************************************************************/
uint32_t SPL_DACEx_GetTrimOffset(DAC_TypeDef* DACx, uint32_t Channel)
{
    /* Check the parameters */
    assert_param(IS_DAC_ALL_PERIPH(DACx));
    assert_param(IS_DAC_CHANNEL(Channel));

    /* Retrieve trimming  */
    return ((DACx->CCR & (DAC_CCR_OTRIM1 << (Channel & 0x10UL))) >> (Channel & 0x10UL));
}

