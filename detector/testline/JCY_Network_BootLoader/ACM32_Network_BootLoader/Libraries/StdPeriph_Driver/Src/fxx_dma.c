/*
  ******************************************************************************
  * @file    fxx_dma.c
  * @version V1.0.0
  * @date    2020
  * @brief   DMA module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Direct Memory Access (DMA) peripheral:
  *           @ Initialization and de-initialization functions
  *           @ IO operation functions
  ******************************************************************************
*/
#include "fxx_std.h"

/**************** Used in cycle mode ****************/
static DMA_LLI_InitTypeDef Cycle_Channel[DMA_CHANNEL_NUM];

/**
  * @}
  */

/** @defgroup DMA_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup DMA_Private_Variables
  * @{
  */

/**
  * @}
  */

/** @defgroup DMA_Private_FunctionPrototypes
  * @{
  */

/**
  * @}
  */

/** @defgroup DMA_Private_Functions
  * @{
  */

/**
  * @brief  Deinitializes the DMA Channelx registers to their default reset
  *         values.
  * @param  DMA_Channelx: where DMA_Channelx can be DMA_Channel0 to 7 for DMA to select the DMA Channel.
  * @retval None
  */
void DMA_DeInit(DMA_Channel_TypeDef* DMA_Channelx)
{
    /* Check the parameters */
    assert_param(IS_DMA_ALL_PERIPH(DMA_Channelx));

    /* Disable the selected DMA Channelx */
    DMA->CONFIG &= ~DMA_CONFIG_EN;
    
    DMA->INT_TC_CLR = 0xFFFFFFFF;
    
    DMA->INT_ERR_CLR = 0xFFFFFFFF;

    /* Reset DMA Channelx CONFIG register */
    DMA_Channelx->CONFIG  = 0;

    /* Reset DMA Channelx CTRL register */
    DMA_Channelx->CTRL = 0;
}

/**
  * @brief  Initializes the DMA Channelx according to the specified
  *         parameters in the DMA_InitStruct.
  * @param  DMA_Channelx: where DMA_Channelx can be DMA_Channel0 to 7 for DMA to select the DMA Channel.
  * @param  DMA_InitStruct: pointer to a DMA_InitTypeDef structure that
  *         contains the configuration information for the specified DMA Channel.
  * @retval None
  */
void DMA_Init(DMA_Channel_TypeDef* DMA_Channelx, DMA_InitTypeDef* DMA_InitStruct)
{
    uint32_t lu32_Channel_Index;
    
    /* Check DMA Parameter */
    assert_param(IS_DMA_DATA_FLOW(DMA_InitStruct->Data_Flow));
    assert_param(IS_DMA_REQUEST_ID(DMA_InitStruct->Request_ID)); 
    assert_param(IS_DMA_SRC_WIDTH(DMA_InitStruct->Source_Width));
    assert_param(IS_DMA_DST_WIDTH(DMA_InitStruct->Desination_Width));
    assert_param(IS_DMA_MODE(DMA_InitStruct->DMA_Mode));

    /* Default Little-Endian、Enable DMA */
    DMA->CONFIG = DMA_CONFIG_EN;

    /* Clear Channel Config */
    DMA_Channelx->CONFIG = 0x00000000; 
    
    if (DMA_InitStruct->Data_Flow == DMA_DATA_FLOW_M2P) 
    {
        DMA_InitStruct->Request_ID <<= DMA_CHANNEL_CONFIG_DEST_PERIPH_POS;
    }
    else if (DMA_InitStruct->Data_Flow == DMA_DATA_FLOW_P2M) 
    {
        DMA_InitStruct->Request_ID <<= DMA_CHANNEL_CONFIG_SRC_PERIPH_POS;
    }

    DMA_Channelx->CONFIG = DMA_InitStruct->Data_Flow | DMA_InitStruct->Request_ID;
    
    /* Config Channel Control */
    DMA_Channelx->CTRL = DMA_CHANNEL_CTRL_ITC;
    /* Source or Desination address increase */
    DMA_Channelx->CTRL |= (DMA_InitStruct->Desination_Inc | DMA_InitStruct->Source_Inc);
    /* Source or Desination date width */
    DMA_Channelx->CTRL |= (DMA_InitStruct->Desination_Width | DMA_InitStruct->Source_Width);
    
    /* Set source address and desination address */
    DMA_Channelx->SRC_ADDR  = DMA_InitStruct->SrcAddr;
    DMA_Channelx->DEST_ADDR = DMA_InitStruct->DstAddr;

    if(DMA_InitStruct->DMA_Mode == DMA_CIRCULAR)
    {
        /* Get DMA Channel number */
        lu32_Channel_Index = ((uint32_t)(DMA_Channelx) - (uint32_t)(DMA_Channel0)) / 0x20;
        
        /* Set Next Link */
        DMA_Channelx->LLI = (uint32_t)&Cycle_Channel[lu32_Channel_Index];
        
        /* The list point to oneself */
        Cycle_Channel[lu32_Channel_Index].SrcAddr = DMA_InitStruct->SrcAddr;
        Cycle_Channel[lu32_Channel_Index].DstAddr = DMA_InitStruct->DstAddr;
        Cycle_Channel[lu32_Channel_Index].Next    = &Cycle_Channel[lu32_Channel_Index];
        Cycle_Channel[lu32_Channel_Index].Control = (DMA_Channelx->CTRL & (~0xFFF)) | DMA_InitStruct->DMA_BufferSize;    
    }
    
    /* Set Transfer Size */
    DMA_Channelx->CTRL = (DMA_Channelx->CTRL & (~0xFFF)) | DMA_InitStruct->DMA_BufferSize;
}

/**
  * @brief  Fills each DMA_InitStruct member with its default value.
  * @param  DMA_InitStruct : pointer to a DMA_InitTypeDef structure which will
  *         be initialized.
  * @retval None
  */
void DMA_StructInit(DMA_InitTypeDef* DMA_InitStruct)
{
/*-------------- Reset DMA init structure parameters values ------------------*/
    /* Initialize the DMA source address member */
    DMA_InitStruct->SrcAddr = 0;
    /* Initialize the DMA desination address member */
    DMA_InitStruct->DstAddr = 0;
    /* Initialize the Data_Flow member */
    DMA_InitStruct->Data_Flow = DMA_DATA_FLOW_M2M;
    /* Initialize the Request_ID member */
    DMA_InitStruct->Request_ID = REG_M2M;
    /* Initialize the DMA_BufferSize member */
    DMA_InitStruct->DMA_BufferSize = 0;
    /* Initialize the Source_Inc member */
    DMA_InitStruct->Source_Inc = DMA_SOURCE_ADDR_INCREASE_DISABLE;
    /* Initialize the Desination_Inc member */
    DMA_InitStruct->Desination_Inc = DMA_DST_ADDR_INCREASE_DISABLE;
    /* Initialize the Source_Width member */
    DMA_InitStruct->Source_Width = DMA_SRC_WIDTH_BYTE;
    /* Initialize the Desination_Width member */
    DMA_InitStruct->Desination_Width = DMA_DST_WIDTH_BYTE;
    /* Initialize the DMA_Mode member */
    DMA_InitStruct->DMA_Mode = DMA_NORMAL;
}

/**
  * @brief  Enables or disables the specified DMA Channelx.
  * @param  DMA_Channelx: where DMA_Channelx can be DMA_Channel0 to 7 for DMA to select the DMA Channel.
  * @param  NewState: new state of the DMA Channelx. 
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void DMA_Cmd(DMA_Channel_TypeDef* DMA_Channelx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_DMA_ALL_PERIPH(DMA_Channelx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the selected DMA Channelx */
        DMA_Channelx->CONFIG |= DMA_CHANNEL_CONFIG_EN;
    }
    else
    {
        /* Disable the selected DMA Channelx */
        DMA_Channelx->CONFIG &= (~DMA_CHANNEL_CONFIG_EN);
    }
}

/**
  * @brief  Enables or disables the specified DMA Channelx interrupts.
  * @param  DMA_Channelx: where DMA_Channelx can be DMA_Channel0 to 7 for DMA to select the DMA Channel.
  * @param  DMA_IT: specifies the DMA interrupts sources to be enabled
  *   or disabled. 
  *   This parameter can be any combination of the following values:
  *     @arg DMA_CHANNEL_CONFIG_ITC:  Transfer complete interrupt mask
  *     @arg DMA_CHANNEL_CONFIG_IE:  Transfer error interrupt mask
  * @param  NewState: new state of the specified DMA interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void DMA_ITConfig(DMA_Channel_TypeDef* DMA_Channelx, uint32_t DMA_IT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_DMA_ALL_PERIPH(DMA_Channelx));
    assert_param(IS_DMA_CONFIG_IT(DMA_IT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE)
    {
        /* Enable the selected DMA interrupts */
        DMA_Channelx->CONFIG |= DMA_IT;
    }
    else
    {
        /* Disable the selected DMA interrupts */
        DMA_Channelx->CONFIG &= ~DMA_IT;
    }
}

/**
  * @brief  Sets the number of data units in the current DMA Channelx transfer.
  * @param  DMA_Channelx: where DMA_Channelx can be DMA_Channel0 to 7 for DMA to select the DMA Channel.
  * @param  DataNumber: The number of data units in the current DMA Channelx
  *         transfer.   
  * @note   This function can only be used when the DMA_Channelx is disabled.                 
  * @retval None.
  */
void DMA_SetCurrDataCounter(DMA_Channel_TypeDef* DMA_Channelx, uint16_t DataNumber)
{
    /* Check the parameters */
    assert_param(IS_DMA_ALL_PERIPH(DMA_Channelx));
  
/*--------------------------- DMA Channelx Transfer Size Configuration ---------------*/
    /* Set Transfer Size */
    DMA_Channelx->CTRL = (DMA_Channelx->CTRL & (~0xFFF)) | DataNumber;
}

/**
  * @brief  Returns the number of remaining data units in the current
  *         DMA Channelx transfer.
  * @param  DMA_Channelx: where DMA_Channelx can be DMA_Channel0 to 7 for DMA to select the DMA Channel.
  * @retval The number of remaining data units in the current DMA Channelx
  *         transfer.
  */
uint16_t DMA_GetCurrDataCounter(DMA_Channel_TypeDef* DMA_Channelx)
{
    /* Check the parameters */
    assert_param(IS_DMA_ALL_PERIPH(DMA_Channelx));
    /* Return the number of remaining data units for DMA Channelx */
    return ((uint16_t)(DMA_Channelx->CTRL & 0xFFF));
}

/**
  * @brief  Checks whether the specified DMA Channelx flag is set or not.
  * @param  DMA_FLAG: specifies the flag to check.
  *   This parameter can be one of the following values:
  *     @arg DMA_IT_FLAG_TC0~7: DMA Channel0~7 Transfer complete interrupt or flag.
  *     @arg DMA_IT_FLAG_TE0~7: DMA Channel0~7 Transfer error interrupt or flag.
  * @retval The new state of DMA_FLAG (SET or RESET).
  */
FlagStatus DMA_GetFlagStatus(uint32_t DMA_FLAG)
{
    FlagStatus bitstatus = RESET;
    uint32_t tmpreg = 0;

    /* Check the parameters */
    assert_param(IS_DMA_IT_FLAG(DMA_FLAG));

    if(DMA_FLAG < DMA_IT_FLAG_TE0)
    {
        /* Get DMA RAW_INT_TC_STATUS register value */
        tmpreg = DMA->RAW_INT_TC_STATUS ;
    }
    else
    {
        /* Get DMA RAW_INT_ERR_STATUS register value */
        tmpreg = DMA->RAW_INT_ERR_STATUS ;
        tmpreg <<= 16;
    }

    /* Check the status of the specified DMA flag */
    if ((tmpreg & DMA_FLAG) != (uint32_t)RESET)
    {
        /* DMA_FLAG is set */
        bitstatus = SET;
    }
    else
    {
        /* DMA_FLAG is reset */
        bitstatus = RESET;
    }

    /* Return the DMA_FLAG status */
    return  bitstatus;
}

/**
  * @brief  Clears the DMA Channelx's pending flags.
  * @param  DMA_FLAG: specifies the flag to clear.
  *   This parameter can be any combination (for the same DMA) of the following values:
  *     @arg DMA_IT_FLAG_TC0~7: DMA Channel0~7 Transfer complete interrupt or flag.
  *     @arg DMA_IT_FLAG_TE0~7: DMA Channel0~7 Transfer error interrupt or flag.
  * @retval None
  */
void DMA_ClearFlag(uint32_t DMA_FLAG)
{
    /* Check the parameters */
    assert_param(IS_DMA_IT_FLAG(DMA_FLAG));

    if(DMA_FLAG < DMA_IT_FLAG_TE0)
    {
        /* Get DMA INT_TC_CLR register value */
        DMA->INT_TC_CLR = DMA_FLAG ;
    }
    else
    {
        /* Get DMA INT_ERR_CLR register value */
        DMA->INT_ERR_CLR = (DMA_FLAG >> 16);
    }
}

/**
  * @brief  Checks whether the specified DMA Channelx interrupt has occurred or not.
  * @param  DMA_IT: specifies the DMA interrupt source to check. 
  *   This parameter can be one of the following values:
  *     @arg DMA_IT_FLAG_TC0~7: DMA Channel0~7 Transfer complete interrupt or flag.
  *     @arg DMA_IT_FLAG_TE0~7: DMA Channel0~7 Transfer error interrupt or flag.
  * @retval The new state of DMA_IT (SET or RESET).
  */
ITStatus DMA_GetITStatus(uint32_t DMA_IT)
{
    FlagStatus bitstatus = RESET;
    uint32_t tmpreg = 0;

    /* Check the parameters */
    assert_param(IS_DMA_IT_FLAG(DMA_IT));

    if(DMA_IT < DMA_IT_FLAG_TE0)
    {
        /* Get DMA INT_TC_STATUS register value */
        tmpreg = DMA->INT_TC_STATUS ;
    }
    else
    {
        /* Get DMA INT_ERR_STATUS register value */
        tmpreg = DMA->INT_ERR_STATUS ;
        tmpreg <<= 16;
    }

    /* Check the status of the specified DMA flag */
    if ((tmpreg & DMA_IT) != (uint32_t)RESET)
    {
        /* DMA_FLAG is set */
        bitstatus = SET;
    }
    else
    {
        /* DMA_FLAG is reset */
        bitstatus = RESET;
    }

    /* Return the DMA_FLAG status */
    return  bitstatus;
}

/**
  * @brief  Clears the DMA Channelx's interrupt pending bits.
  * @param  DMA_IT: specifies the DMA interrupt pending bit to clear.
  *   This parameter can be any combination (for the same DMA) of the following values:
  *     @arg DMA_IT_FLAG_TC0~7: DMA Channel0~7 Transfer complete interrupt or flag.
  *     @arg DMA_IT_FLAG_TE0~7: DMA Channel0~7 Transfer error interrupt or flag.
  * @retval None
  */
void DMA_ClearITPendingBit(uint32_t DMA_IT)
{
    /* Check the parameters */
    assert_param(IS_DMA_IT_FLAG(DMA_IT));

    if(DMA_IT < DMA_IT_FLAG_TE0)
    {
        /* Get DMA INT_TC_CLR register value */
        DMA->INT_TC_CLR = DMA_IT ;
    }
    else
    {
        /* Get DMA INT_ERR_CLR register value */
        DMA->INT_ERR_CLR = (DMA_IT >> 16);
    }
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
