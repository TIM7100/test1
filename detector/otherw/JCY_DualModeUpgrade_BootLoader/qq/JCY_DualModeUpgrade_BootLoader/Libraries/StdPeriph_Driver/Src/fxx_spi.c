/*
  ******************************************************************************
  * @file    fxx_spi.c
  * @version V1.0.0
  * @date    2021
  * @brief   SPI module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Serial Peripheral Interface (SPI) peripheral.
  *           @ Initialization and de-initialization functions
  *           @ IO operation functions
  *           @ Peripheral Control functions
  ******************************************************************************
*/
#include  "fxx_std.h" 

/**
  * @brief  Deinitializes the SPIx peripheral registers to their default
  *         reset values.
  * @param  SPIx: where x can be 1, 2 ... to select the SPI peripheral.
  * @retval None
  */
void SPI_DeInit(SPI_TypeDef* SPIx)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));

    if (SPIx == SPI1)
    {
        /* Enable SPI1 reset state */
        System_Module_Reset(RST_SPI1);
    }
    else if (SPIx == SPI2)
    {
        /* Enable SPI2 reset state */
        System_Module_Reset(RST_SPI2);
    }
    else if (SPIx == SPI3)
    {
      /* Enable SPI3 reset state */
        System_Module_Reset(RST_SPI3);
    }    
    else if (SPIx == SPI4)
    {
      /* Enable SPI4 reset state */
        System_Module_Reset(RST_SPI4);
    }    
}

/**
  * @brief  Initializes the SPIx peripheral according to the specified 
  *         parameters in the SPI_InitStruct.
  * @param  SPIx: where x can be 1, 2 ... to select the SPI peripheral.
  * @param  SPI_InitStruct: pointer to a SPI_InitTypeDef structure that
  *         contains the configuration information for the specified SPI peripheral.
  * @retval None
  */
void SPI_Init(SPI_TypeDef* SPIx, SPI_InitTypeDef* SPI_InitStruct)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_SPI_ALL_MODE(SPI_InitStruct->SPI_Mode));
    assert_param(IS_SPI_CPOL(SPI_InitStruct->SPI_CPOL));
    assert_param(IS_SPI_CPHA(SPI_InitStruct->SPI_CPHA));
    assert_param(IS_SPI_X_MODE(SPI_InitStruct->SPI_X_Mode));
    assert_param(IS_SPI_FIRST_BIT(SPI_InitStruct->SPI_FirstBit));
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_InitStruct->SPI_BaudRatePrescaler));
    
    /* Automatic change direction */
    SPIx->CTL |= (SPI_CTL_IO_MODE);
    
    /* Set SPI Work mode */
    if (SPI_InitStruct->SPI_Mode == SPI_MODE_MASTER)
    {
        SET_BIT(SPIx->CTL, SPI_CTL_MST_MODE);
    }
    else 
    {
        CLEAR_BIT(SPIx->CTL, SPI_CTL_MST_MODE);
        
        SPIx->BATCH = (SPIx->BATCH & (~0x000FFFFFU)) | (1 << 0);
        
        SPIx->TX_CTL |= SPI_TX_CTL_MODE | (0x88 << 8);  // dummy data = 0x88  
        
        if (SPI_InitStruct->SPI_X_Mode != SPI_1X_MODE) 
        {
            SPIx->CTL |= SPI_CTL_SFILTER;
        }
        
        /* Slave Alternate Enable */
        SPIx->CTL |= SPI_CTL_SLAVE_EN;
        
        /* Slave Mode Enable Rx By Default */
        SPIx->RX_CTL |= SPI_RX_CTL_EN;
    }

    /* Set SPI First Bit */
    if (SPI_InitStruct->SPI_FirstBit == SPI_FIRSTBIT_LSB)
        SET_BIT(SPIx->CTL, SPI_CTL_LSB_FIRST);
    else if(SPI_InitStruct->SPI_FirstBit == SPI_FIRSTBIT_MSB)
        CLEAR_BIT(SPIx->CTL, SPI_CTL_LSB_FIRST);

    /* Set SPI Work Mode */
    SPIx->CTL = ((SPIx->CTL) & (~(SPI_CTL_CPHA | SPI_CTL_CPOL))) | (SPI_InitStruct->SPI_CPOL | SPI_InitStruct->SPI_CPHA);
    
    /* Set SPI SPI_X_Mode */
    SPIx->CTL = ((SPIx->CTL) & (~SPI_CTL_X_MODE)) | (SPI_InitStruct->SPI_X_Mode);

    /* Set SPI BaudRate Prescaler */
    SPIx->BAUD = ((SPIx->BAUD) & (~0x0000FFFF)) | (SPI_InitStruct->SPI_BaudRatePrescaler);

    /* Disable All Interrupt */
    SPIx->IE = 0x00000000;
}

/**
  * @brief  Fills each SPI_InitStruct member with its default value.
  * @param  SPI_InitStruct : pointer to a SPI_InitTypeDef structure which will be initialized.
  * @retval None
  */
void SPI_StructInit(SPI_InitTypeDef* SPI_InitStruct)
{
/*--------------- Reset SPI init structure parameters values -----------------*/
    /* initialize the SPI_Mode member */
    SPI_InitStruct->SPI_Mode = SPI_MODE_SLAVE;
    /* Initialize the SPI_CPOL member */
    SPI_InitStruct->SPI_CPOL = SPI_CPOL_LOW;
    /* Initialize the SPI_CPHA member */
    SPI_InitStruct->SPI_CPHA = SPI_CPHA_1EDGE;
    /* Initialize the SPI_X_Mode member */
    SPI_InitStruct->SPI_X_Mode = SPI_1X_MODE;
    /* Initialize the SPI_BaudRatePrescaler member */
    SPI_InitStruct->SPI_BaudRatePrescaler = SPI_BAUDRATE_PRESCALER_2;
    /* Initialize the SPI_FirstBit member */
    SPI_InitStruct->SPI_FirstBit = SPI_FIRSTBIT_MSB;
}

/**
  * @brief  Enables or disables the SPIx transmit.
  * @param  SPIx: where x can be 1, 2 ... to select the SPI peripheral.
  * @param  NewState: new state of the selected SPI transmit.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI_TxCmd(SPI_TypeDef* SPIx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the selected SPI transmit */
        SPIx->TX_CTL |= SPI_TX_CTL_EN;
    }
    else
    {
        /* Disable the selected SPI transmit */
        SPIx->TX_CTL &= ~SPI_TX_CTL_EN;
    }
}

/**
  * @brief  Enables or disables the SPIx recieve.
  * @param  SPIx: where x can be 1, 2 ... to select the SPI peripheral.
  * @param  NewState: new state of the selected SPI recieve.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI_RxCmd(SPI_TypeDef* SPIx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the selected SPI recieve */
        SPIx->RX_CTL |= SPI_RX_CTL_EN;
    }
    else
    {
        /* Disable the selected SPI recieve */
        SPIx->RX_CTL &= ~SPI_RX_CTL_EN;
    }
}

/**
  * @brief  Enables or disables the specified SPI interrupts.
  * @param  SPIx: where x can be 1, 2 ... to select the SPI peripheral.
  * @param  SPI_IT: specifies the SPI interrupt source to be enabled or disabled. 
  *   This parameter can be one of the following values:
  *     @arg SPI_IE_BATCH_DONE_EN: Batch done interrupt mask
  *     @arg ...... : Other interrupt mask
  *     @arg SPI_IE_RX_BATCH_DONE_EN: Recieve batch done interrupt mask
  * @param  NewState: new state of the specified SPI interrupt.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI_ITConfig(SPI_TypeDef* SPIx, uint32_t SPI_IT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    assert_param(IS_SPI_CONFIG_IT(SPI_IT));

    if (NewState != DISABLE)
    {
        /* Enable the selected SPI interrupt */
        SPIx->IE |= SPI_IT;
    }
    else
    {
        /* Disable the selected SPI interrupt */
        SPIx->IE &= (uint32_t)~SPI_IT;
    }
}

/**
  * @brief  Get the specified SPI interrupts flag.
  * @param  SPIx: where x can be 1, 2 ... to select the SPI peripheral.
  * @param  SPI_IT: specifies the SPI interrupt source to be read. 
  *   This parameter can be one of the following values:
  *     @arg SPI_IE_BATCH_DONE_EN: Batch done interrupt mask
  *     @arg ...... : Other interrupt mask
  *     @arg SPI_IE_RX_BATCH_DONE_EN: Recieve batch done interrupt mask
  * @retval The interrupt flag of SPI_IT (SET or RESET).
  */
FlagStatus SPI_ITGet(SPI_TypeDef* SPIx, uint32_t SPI_IT)
{
    FlagStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_SPI_CONFIG_IT(SPI_IT));

    /* Check the specified SPI interrupts flag */
    if ((SPIx->IE & SPI_IT) != (uint32_t)RESET)
    {
        /* SPI_FLAG is set */
        bitstatus = SET;
    }
    else
    {
        /* SPI_FLAG is reset */
        bitstatus = RESET;
    }
    /* Return the SPI_FLAG status */
    return  bitstatus;
}

/**
  * @brief  Set SPI DMA FIFO level.
  * @param  SPIx: where x can be 1, 2 ... to select the SPI peripheral.
  * @param  Fifo_level: This parameter can be a value of. 
  *   This parameter can be one of the following values:
  *     @arg SPI_RX_CTL_DMA_LEVEL:   15 bytes dma trig mode.
  *     @arg SPI_RX_CTL_DMA_LEVEL_0: 1 byte dma trig mode.
  *     @arg SPI_RX_CTL_DMA_LEVEL_1: 2 bytes dma trig mode.
  *     @arg SPI_RX_CTL_DMA_LEVEL_2: 4 bytes dma trig mode.
  *     @arg SPI_RX_CTL_DMA_LEVEL_3: 8 bytes dma trig mode.
  * @retval None
  */
void SPI_WireDmaFifo(SPI_TypeDef* SPIx, uint32_t Fifo_level)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    
    /* Set SPI DMA FIFO level */
    SPIx->RX_CTL |= Fifo_level;
}

/**
  * @brief  Enables or disables the SPIx DMA interface.
  * @param  SPIx: where x can be 1, 2 ... to select the SPI peripheral.
  * @param  SPI_DMAReq: specifies the SPI DMA transfer request to be enabled or disabled. 
  *   This parameter can be any combination of the following values:
  *     @arg SPI_DMAReq_TX: Tx buffer DMA transfer request
  *     @arg SPI_DMAReq_RX: Rx buffer DMA transfer request
  * @param  NewState: new state of the selected SPI DMA transfer request.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI_DMACmd(SPI_TypeDef* SPIx, uint16_t SPI_DMAReq, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    assert_param(IS_SPI_DMAREQ(SPI_DMAReq));
    if (NewState != DISABLE)
    {
        /* Enable the selected SPI DMA TX requests */
        if(SPI_DMAReq & SPI_DMAReq_TX)
            SPIx->TX_CTL |= SPI_TX_CTL_DMA_REQ_EN;
        
        /* Enable the selected SPI DMA RX requests */
        if(SPI_DMAReq & SPI_DMAReq_RX)
            SPIx->RX_CTL |= SPI_RX_CTL_DMA_REQ_EN;
    }
    else
    {
        /* Disable the selected SPI DMA TX requests */
        if(SPI_DMAReq & SPI_DMAReq_TX)
            SPIx->TX_CTL &= (~SPI_TX_CTL_DMA_REQ_EN);
        
        /* Disable the selected SPI DMA RX requests */
        if(SPI_DMAReq & SPI_DMAReq_RX)
            SPIx->RX_CTL &= (~SPI_RX_CTL_DMA_REQ_EN);
    }
}    

/**
  * @brief  Transmits a Data through the SPIx peripheral.
  * @param  SPIx: where x can be 1, 2 ... to select the SPI peripheral.
  * @param  Data : Data to be transmitted.
  * @retval None
  */
void SPI_SendData(SPI_TypeDef* SPIx, uint8_t Data)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));

    /* Write in the DR register the data to be sent */
    SPIx->DAT = Data;
}

/**
  * @brief  Returns the most recent received data by the SPIx peripheral. 
  * @param  SPIx: where x can be 1, 2 ... to select the SPI peripheral.
  * @retval The value of the received data.
  */
uint8_t SPI_ReceiveData(SPI_TypeDef* SPIx)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));

    /* Return the data in the DR register */
    return (uint8_t)SPIx->DAT;
}

/**
  * @brief  Enables or disables the SS output for the selected SPI.
  * @param  SPIx: where x can be 1, 2 ... to select the SPI peripheral.
  * @param  NewState: new state of the SPIx SS output. 
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI_SSOutputCmd(SPI_TypeDef* SPIx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState != DISABLE)
    {
        /* Enable the selected SPI SS output */
        SPIx->CS |= SPI_CS_CS0;
    }
    else
    {
        /* Disable the selected SPI SS output */
        SPIx->CS &= ~SPI_CS_CS0;
    }
}

/**
  * @brief  SPI Wire Config.
  * @param  SPIx: where x can be 1, 2 ... to select the SPI peripheral.
  * @param  X_Mode: This parameter can be a value of @ref X_MODE. 
  *   This parameter can be one of the following values:
  *     @arg SPI_1X_MODE: 1 wire mode.
  *     @arg SPI_2X_MODE: 2 wire mode.
  *     @arg SPI_4X_MODE: 4 wire mode.
  * @retval None
  */
void SPI_Wire_Config(SPI_TypeDef* SPIx, uint32_t X_Mode)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    
    /* Set SPI X_Mode */
    SPIx->CTL = ((SPIx->CTL) & (~SPI_CTL_X_MODE)) | X_Mode;
}

/**
  * @brief  Write a number in the BATCH register through the SPIx peripheral.
  * @param  SPIx: where x can be 1, 2 ... to select the SPI peripheral.
  * @param  number : number to be writed to the register.
  * @retval None
  */
void SPI_WriteBatch(SPI_TypeDef* SPIx, uint32_t number)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));

    /* Write in the BATCH register the number */
    SPIx->BATCH = number;
}

/**
  * @brief  Returns the batch number by the SPIx peripheral. 
  * @param  SPIx: where x can be 1, 2 ... to select the SPI peripheral.
  * @retval The value of the batch number.
  */
uint32_t SPI_ReadBatch(SPI_TypeDef* SPIx)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));

    /* Return the number in the BATCH register */
    return SPIx->BATCH;
}

/**
  * @brief  Clear the SPI Tx FIFO.
  * @param  SPIx: where x can be 1, 2 ... to select the SPI peripheral.
  * @retval None
  */
void SPI_ClearTxFifo(SPI_TypeDef* SPIx)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));

    /* Clear the TX FIFO */
    SET_BIT(SPIx->TX_CTL, SPI_TX_CTL_FIFO_RESET);
    CLEAR_BIT(SPIx->TX_CTL, SPI_TX_CTL_FIFO_RESET);
}

/**
  * @brief  Clear the SPI Rx FIFO.
  * @param  SPIx: where x can be 1, 2 ... to select the SPI peripheral.
  * @retval None
  */
void SPI_ClearRxFifo(SPI_TypeDef* SPIx)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));

    /* Clear the RX FIFO */
    SET_BIT(SPIx->RX_CTL, SPI_RX_CTL_FIFO_RESET);
    CLEAR_BIT(SPIx->RX_CTL, SPI_RX_CTL_FIFO_RESET);
}

/**
  * @brief  Checks whether the specified SPI flag is set or not.
  * @param  SPIx: where x can be 1, 2 ... to select the SPI peripheral.
  * @param  SPI_FLAG: specifies the SPI flag to check. 
  *   This parameter can be one of the following values:
  *     @arg SPI_STATUS_TX_BUSY: Tx busy flag
  *     @arg ...... : Other flag
  *     @arg SPI_STATUS_RX_BATCH_DONE: Recieve batch done flag
  * @retval The new state of SPI_FLAG (SET or RESET).
  */
FlagStatus SPI_GetFlagStatus(SPI_TypeDef* SPIx, uint32_t SPI_FLAG)
{
    FlagStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_SPI_GET_FLAG(SPI_FLAG));
    /* Check the status of the specified SPI flag */
    if ((SPIx->STATUS & SPI_FLAG) != (uint32_t)RESET)
    {
        /* SPI_FLAG is set */
        bitstatus = SET;
    }
    else
    {
        /* SPI_FLAG is reset */
        bitstatus = RESET;
    }
    /* Return the SPI_FLAG status */
    return  bitstatus;
}

/**
  * @brief  Clears the SPIx Status flag.
  * @param  SPIx: where x can be 1, 2 ... to select the SPI peripheral.
  * @param  SPI_FLAG: specifies the SPI flag to check. 
  *   This parameter can be one of the following values:
  *     @arg SPI_CLEAR_BATCH_DONE: Clear batch done flag
  *     @arg SPI_CLEAR_CS_POS: Clear cs rising edge flag
  * @retval None
  */
void SPI_ClearFlag(SPI_TypeDef* SPIx, uint32_t SPI_FLAG)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_SPI_CLEAR_FLAG(SPI_FLAG));

    /* Clear the selected SPI flag */
    SPIx->STATUS = SPI_FLAG;
}

/**
  * @brief  Checks whether the specified SPI IT flag is set or not.
  * @param  SPIx: where x can be 1, 2 ... to select the SPI peripheral.
  * @param  SPI_FLAG: specifies the SPI flag to check. 
  *   This parameter can be one of the following values:
  *     @arg SPI_STATUS_TX_BUSY: Tx busy flag
  *     @arg ...... : Other flag
  *     @arg SPI_STATUS_RX_BATCH_DONE: Recieve batch done flag
  * @retval The new state of SPI_FLAG (SET or RESET).
  */
ITStatus SPI_GetITStatus(SPI_TypeDef* SPIx, uint32_t SPI_IT)
{
    FlagStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_SPI_GET_FLAG(SPI_IT));
    /* Check the status of the specified SPI flag */
    if ((SPIx->STATUS & SPI_IT) != (uint32_t)RESET)
    {
        /* SPI_FLAG is set */
        bitstatus = SET;
    }
    else
    {
        /* SPI_FLAG is reset */
        bitstatus = RESET;
    }
    /* Return the SPI_FLAG status */
    return  bitstatus;
}

/**
  * @brief  Clears the SPIx IT Status flag.
  * @param  SPIx: where x can be 1, 2 ... to select the SPI peripheral.
  * @param  SPI_FLAG: specifies the SPI flag to check. 
  *   This parameter can be one of the following values:
  *     @arg SPI_CLEAR_BATCH_DONE: Clear batch done flag
  *     @arg SPI_CLEAR_CS_POS: Clear cs rising edge flag
  * @retval None
  */
void SPI_ClearITPendingBit(SPI_TypeDef* SPIx, uint32_t SPI_IT)
{
    SPI_ClearFlag(SPIx, SPI_IT);
}




