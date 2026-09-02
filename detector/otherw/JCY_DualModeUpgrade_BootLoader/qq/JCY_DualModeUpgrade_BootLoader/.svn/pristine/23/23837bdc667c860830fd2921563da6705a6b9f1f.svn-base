#include "bsp_spi.h"


/******** SPI配置 *********/
//#define 

static SPI_InitTypeDef    SPI_InitStructure;


static SPI_DataTypeDef    SPI_DataInfo;



static void SPI_GpioConfig(SPI_TypeDef* SPIx)
{
	GPIO_InitTypeDef   SPI_GpioInit;
	//Enable GPIO Clock
//	System_Module_Enable(EN_GPIOAB);
//	System_Module_Enable(EN_GPIOCD);
    
	/* SPI1 CS   PortA Pin4 */
    /* SPI1 CLK  PortA Pin5 */
    /* SPI1 MOSI PortA Pin6 */
    /* SPI1 MISO PortA Pin7 */
	if (SPIx == SPI1)
	{
		SPI_GpioInit.Pin            = GPIO_PIN_4| GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
		SPI_GpioInit.Mode           = GPIO_MODE_AF_PP;
		SPI_GpioInit.Pull           = GPIO_PULLUP;
		SPI_GpioInit.Alternate      = GPIO_FUNCTION_1;
		GPIO_Init(GPIOA, &SPI_GpioInit);
	}   
	
	/* SPI2 CS   PortB Pin12 */
    /* SPI2 CLK  PortB Pin13 */
    /* SPI2 MOSI PortB Pin15 */
    /* SPI2 MISO PortB Pin14 */
    else if (SPIx == SPI2)
    {
		SPI_GpioInit.Pin            = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
		SPI_GpioInit.Mode           = GPIO_MODE_AF_PP;
		SPI_GpioInit.Pull           = GPIO_PULLUP;
		SPI_GpioInit.Alternate      = GPIO_FUNCTION_4;
		GPIO_Init(GPIOB, &SPI_GpioInit);

		SPI_GpioInit.Pin            = GPIO_PIN_12;
		SPI_GpioInit.Mode           = GPIO_MODE_OUTPUT_PP;
		SPI_GpioInit.Pull           = GPIO_PULLUP;
		SPI_GpioInit.Alternate      = GPIO_FUNCTION_0;
		GPIO_Init(GPIOB, &SPI_GpioInit);		
    }
	
    /* SPI3 CS   PortA Pin15 */
    /* SPI3 CLK  PortC Pin10 */
    /* SPI3 MOSI PortC Pin12 */
    /* SPI3 MISO PortC Pin11 */
    else if (SPIx == SPI3)
    {
		SPI_GpioInit.Pin            = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
		SPI_GpioInit.Mode           = GPIO_MODE_AF_PP;
		SPI_GpioInit.Pull           = GPIO_PULLUP;
		SPI_GpioInit.Alternate      = GPIO_FUNCTION_3;
		GPIO_Init(GPIOC, &SPI_GpioInit);

		SPI_GpioInit.Pin            = GPIO_PIN_15;
		SPI_GpioInit.Mode           = GPIO_MODE_OUTPUT_PP;
		SPI_GpioInit.Pull           = GPIO_PULLUP;
		SPI_GpioInit.Alternate      = GPIO_FUNCTION_0;
		GPIO_Init(GPIOA, &SPI_GpioInit);
    }					
}	

/**
 * @function SPI_Configuration
 * @brief 配置SPI通信模式为主机模式
 * @param SPIx:选择配置哪个SPI
 * @return 无
 ***/
void SPI_Configuration(SPI_TypeDef* SPIx)
{
	SPI_GpioConfig(SPIx);
    /* Enable SPI */
    if (SPIx == SPI1) System_Module_Enable(EN_SPI1);
	else if (SPIx == SPI2) System_Module_Enable(EN_SPI2);
	else if (SPIx == SPI3) System_Module_Enable(EN_SPI3);

    SPI_InitStructure.SPI_Mode               = SPI_MODE_MASTER;
    SPI_InitStructure.SPI_CPOL               = SPI_CPOL_HIGH;
    SPI_InitStructure.SPI_CPHA               = SPI_CPHA_2EDGE;
    SPI_InitStructure.SPI_X_Mode             = SPI_1X_MODE;
    SPI_InitStructure.SPI_FirstBit           = SPI_FIRSTBIT_MSB;
    SPI_InitStructure.SPI_BaudRatePrescaler  = SPI_BAUDRATE_PRESCALER_64;

    SPI_Init(SPIx, &SPI_InitStructure);
}

/**
 * @function SPIx_SetSpeed
 * @brief 设置硬件SPI的通信速度
 * @param SPIx:选择设置哪个SPI
 * @param Speed:设置的通信速率
 * @return 无
 ***/
void SPIx_SetSpeed(SPI_TypeDef* SPIx, u8 Speed)
{
	/* Enable SPI */
    if (SPIx == SPI1) System_Module_Enable(EN_SPI1);
	else if (SPIx == SPI2) System_Module_Enable(EN_SPI2);
	else if (SPIx == SPI3) System_Module_Enable(EN_SPI3);
	
	SPI_InitStructure.SPI_BaudRatePrescaler  = Speed;
	SPI_Init(SPIx, &SPI_InitStructure);
}


/**
 * @function   : SPI_Transmit
 * @brief 传输SPI的消息
 * @param pData: Pointer to data buffer
 * @param Size:  Amount of data to be sent
 * @param Timeout: Transmit Timeout
 * @return 
 ***/
SPI_StatusTypeDef SPI_Transmit(SPI_TypeDef* SPIx, uint8_t *pData, uint32_t Size, uint32_t Timeout)
{
    uint32_t i;
    SPI_StatusTypeDef Status = SPI_OK;
    __IO uint32_t uiTimeout;

    /* Check SPI Parameter */
    if (!Size)    return SPI_ERROR;
    if (pData == NULL)    return SPI_ERROR;
    
    SPI_DataInfo.Tx_Count = 0;
    SPI_DataInfo.Tx_Size = Size;
    SPI_DataInfo.Tx_Buffer = pData;
    
    uiTimeout = Timeout;

    /* Clear Batch Done Flag  */
    SPI_ClearFlag(SPIx, SPI_CLEAR_BATCH_DONE);
    
    /* Clear TX FIFO */
    SPI_ClearTxFifo(SPIx);
    
    /* Set Data Size */
    SPI_WriteBatch(SPIx, Size);
    
    /* Tx Enable */
    SPI_TxCmd(SPIx, ENABLE);

    if (SPI_InitStructure.SPI_Mode == SPI_MODE_MASTER) 
    {
        /* Transmit Start */
        SPI_SSOutputCmd(SPIx, ENABLE);
    }
    else
    {
        /* Rx Disable */
        SPI_RxCmd(SPIx, DISABLE);
    }
    
    while(SPI_DataInfo.Tx_Size > 0)
    {
        /* Wait Tx FIFO Not Full */
        while(SPI_GetFlagStatus(SPIx, SPI_STATUS_TX_FIFO_FULL) == SET)
        {
            if(uiTimeout)
            {
                uiTimeout--;
                if (uiTimeout == 0)
                {
                    Status = SPI_TIMEOUT;
                    goto End;
                }
            }
        }        
        SPI_SendData(SPIx, SPI_DataInfo.Tx_Buffer[SPI_DataInfo.Tx_Count++]);
        SPI_DataInfo.Tx_Size--;
        uiTimeout = Timeout;
    }
    
    if (SPI_InitStructure.SPI_Mode == SPI_MODE_SLAVE) 
    {
        /* Wait Transmit Done */
        while(SPI_GetFlagStatus(SPIx, SPI_STATUS_TX_BUSY) == RESET);
        while(SPI_GetFlagStatus(SPIx, SPI_STATUS_TX_BUSY) == SET)
        {
            if(uiTimeout)
            {
                uiTimeout--;
                if (uiTimeout == 0)
                {
                    Status = SPI_TIMEOUT;
                    goto End;
                }
            }
        }
    }
    else
    {
        /* Wait Transmit Done */
        while (SPI_GetFlagStatus(SPIx, SPI_STATUS_TX_BATCH_DONE) == RESET);
        Status = SPI_OK;
    }
    
End:
    /* Clear Batch Done Flag  */
    SPI_ClearFlag(SPIx, SPI_CLEAR_BATCH_DONE);

    /* Tx Disable */
    SPI_TxCmd(SPIx, DISABLE);
    
    if (SPI_InitStructure.SPI_Mode == SPI_MODE_MASTER)  
    {
        /* Transmit End */
        SPI_SSOutputCmd(SPIx, DISABLE);
    }

    return Status;
}

/************************************************************************
 * function   : SPI_Receive
 * Description: Receive an amount of data in blocking mode.
 * input      : pData : Pointer to data buffer
 *              Size  : Amount of data to be Receive
 *              Timeout  : Receive Timeout
 ************************************************************************/
SPI_StatusTypeDef SPI_Receive(SPI_TypeDef* SPIx, uint8_t *pData, uint32_t Size, uint32_t Timeout)
{
    uint32_t i;
    SPI_StatusTypeDef Status = SPI_OK;
    __IO uint32_t uiTimeout;
    
    /* Check SPI Parameter */
    if (pData == NULL)    return SPI_ERROR;
    
    SPI_DataInfo.Rx_Count = 0;
    SPI_DataInfo.Rx_Size = Size;
    SPI_DataInfo.Rx_Buffer = pData;
    uiTimeout = Timeout;

//    if (SPI_InitStructure.SPI_Mode == SPI_MODE_SLAVE) 
//    {
//        SPI_WriteBatch(SPIx, 1);
//        /* Rx Enable */
//        SPI_RxCmd(SPIx, ENABLE);
//        
//        while ( SPI_DataInfo.Rx_Size > 0) 
//        { 
//            while(SPI_GetFlagStatus(SPIx, SPI_STATUS_RX_FIFO_EMPTY) == SET)
//            {
//                if(uiTimeout)
//                {
//                    uiTimeout--;
//                    if (uiTimeout == 0) 
//                    {
//                        /* Rx Disable */
//                        SPI_RxCmd(SPIx, DISABLE);
//                        return SPI_TIMEOUT;
//                    }
//                }
//            }           
//            
//            SPI_DataInfo.Rx_Buffer[SPI_DataInfo.Rx_Count++] = SPI_ReceiveData(SPIx);
//            SPI_DataInfo.Rx_Size--;
//            uiTimeout = Timeout;
//        }

//        /* Rx Disable */
//        SPI_RxCmd(SPIx, DISABLE);

//        return SPI_OK;
//    }

    /* Clear Batch Done Flag  */
    SPI_ClearFlag(SPIx, SPI_CLEAR_BATCH_DONE);
    
    /* Set Data Size */
    SPI_WriteBatch(SPIx, Size);
    
    /* Rx Enable */
    SPI_RxCmd(SPIx, ENABLE);
    
    /* Receive Start */
    SPI_SSOutputCmd(SPIx, ENABLE);
    
    while(SPI_DataInfo.Rx_Size > 0)
    {
        /* have no timeout */
        if (uiTimeout == 0) 
        {
            /* Wait Rx FIFO Not Empty */
            while(SPI_GetFlagStatus(SPIx, SPI_STATUS_RX_FIFO_EMPTY) == SET);
        }
        else
        {
            while(SPI_GetFlagStatus(SPIx, SPI_STATUS_RX_FIFO_EMPTY) == SET)
            {
                if (uiTimeout-- == 0) 
                {
                    Status = SPI_TIMEOUT;
                    goto End; 
                }
            }
        }
        
        SPI_DataInfo.Rx_Buffer[SPI_DataInfo.Rx_Count++] = SPI_ReceiveData(SPIx);
        SPI_DataInfo.Rx_Size--;
    }

    Status = SPI_OK;
    
    /* Wait Transmit Done */
    while(SPI_GetFlagStatus(SPIx, SPI_STATUS_RX_BATCH_DONE) == RESET);

End:   
    /* Clear Batch Done Flag  */
    SPI_ClearFlag(SPIx, SPI_CLEAR_BATCH_DONE);

    /* Rx Disable */
    SPI_RxCmd(SPIx, DISABLE);

    /* Receive End */
    SPI_SSOutputCmd(SPIx, DISABLE);
    
    return Status;
}

/************************************************************************
 * function   : SPI_TransmitReceive
 * Description: Transmits and recieve an amount of data in blocking mode.
 * input      : pTxData : Pointer to transmit data buffer
 *              pRxData : Pointer to recieve data buffer
 *              Size  : Amount of data to be sent
 *              Timeout  : TransmitReceive Timeout
 ************************************************************************/
SPI_StatusTypeDef SPI_TransmitReceive(SPI_TypeDef* SPIx, uint8_t *pTxData, uint8_t *pRxData, uint32_t Size, uint32_t Timeout)
{
    uint32_t i;
    __IO uint32_t TxFlag = 1U, uiTimeout;
    SPI_StatusTypeDef Status = SPI_OK;

    /* Check SPI Parameter */
    if ((pTxData == NULL)||(pRxData == NULL))    return SPI_ERROR;
    
    SPI_DataInfo.Tx_Count = 0;
    SPI_DataInfo.Rx_Count = 0;
    SPI_DataInfo.Tx_Buffer = pTxData;
    SPI_DataInfo.Rx_Buffer = pRxData;
    SPI_DataInfo.Tx_Size = Size;   
    SPI_DataInfo.Rx_Size = Size;   
    uiTimeout = Timeout;
    
    /* Clear Batch Done Flag  */
    SPI_ClearFlag(SPIx, SPI_CLEAR_BATCH_DONE);

    /* Tx Enable */
    SPI_TxCmd(SPIx, ENABLE);

    /* Rx Enable */
    SPI_RxCmd(SPIx, ENABLE);

    /* Clear TX FIFO */
    SPI_ClearTxFifo(SPIx);
        
    if (SPI_InitStructure.SPI_Mode == SPI_MODE_SLAVE) 
    {        
        while((SPI_GetFlagStatus(SPIx, SPI_STATUS_TX_FIFO_FULL) == RESET) && (SPI_DataInfo.Tx_Size>0))
        {
            SPI_SendData(SPIx, SPI_DataInfo.Tx_Buffer[SPI_DataInfo.Tx_Count++]);
            SPI_DataInfo.Tx_Size--;
        }
        TxFlag = 0;
    }
    else
    {       
        /* Set Data Size */
        SPI_WriteBatch(SPIx, SPI_DataInfo.Tx_Size);

        /* Transmit Start */
        SPI_SSOutputCmd(SPIx, ENABLE);
        TxFlag = 1;
    }
    
    while ((SPI_DataInfo.Tx_Size > 0) || (SPI_DataInfo.Rx_Size > 0))
    {
        if (SPI_InitStructure.SPI_Mode == SPI_MODE_SLAVE) 
        {
            /* Wait Rx FIFO Not Empty */
            if ((SPI_GetFlagStatus(SPIx, SPI_STATUS_RX_FIFO_EMPTY) == RESET) && (SPI_DataInfo.Rx_Size > 0))
            {
                SPI_DataInfo.Rx_Buffer[SPI_DataInfo.Rx_Count++] = SPI_ReceiveData(SPIx);
                SPI_DataInfo.Rx_Size--;
                TxFlag = 1U;
            }        
            /* Wait Tx FIFO Not Full */
            if((SPI_GetFlagStatus(SPIx, SPI_STATUS_TX_FIFO_FULL) == RESET) && (SPI_DataInfo.Tx_Size>0) && (TxFlag == 1U))
            {
                while((SPI_GetFlagStatus(SPIx, SPI_STATUS_TX_FIFO_FULL) == RESET) && (SPI_DataInfo.Tx_Size>0))
                {
                    SPI_SendData(SPIx, SPI_DataInfo.Tx_Buffer[SPI_DataInfo.Tx_Count++]);
                    SPI_DataInfo.Tx_Size--;
                }
                TxFlag = 0;
            }
        }
        else
        {
            /* Wait Tx FIFO Not Full */
            if((SPI_GetFlagStatus(SPIx, SPI_STATUS_TX_FIFO_FULL) == RESET) && (SPI_DataInfo.Tx_Size>0) && (TxFlag == 1U))
            {
                SPI_SendData(SPIx, SPI_DataInfo.Tx_Buffer[SPI_DataInfo.Tx_Count++]);
                SPI_DataInfo.Tx_Size--;
                TxFlag = 0;
            }
            
            /* Wait Rx FIFO Not Empty */
            if((SPI_GetFlagStatus(SPIx, SPI_STATUS_RX_FIFO_EMPTY) == RESET) && (SPI_DataInfo.Rx_Size>0))
            {
                SPI_DataInfo.Rx_Buffer[SPI_DataInfo.Rx_Count++] = SPI_ReceiveData(SPIx);
                SPI_DataInfo.Rx_Size--;
                TxFlag = 1U;
            }
        }
        
        /* Wait Timeout */
        if(uiTimeout)
        {
            uiTimeout--;
            if(uiTimeout == 0)
            {
                Status = SPI_TIMEOUT;
                goto End;
            }
        }
    }        
    /* Wait Transmit Done */
    while(SPI_GetFlagStatus(SPIx, SPI_STATUS_TX_BATCH_DONE) == RESET);
    
    Status = SPI_OK;

End:    
    /* Clear Batch Done Flag  */
    SPI_ClearFlag(SPIx, SPI_CLEAR_BATCH_DONE);

    /* Tx Disable */
    SPI_TxCmd(SPIx, DISABLE);
    
    /* Rx Disable */										
    SPI_RxCmd(SPIx, DISABLE);

    if (SPI_InitStructure.SPI_Mode == SPI_MODE_MASTER) 
    {
        /* Transmit End */
        SPI_SSOutputCmd(SPIx, DISABLE);
    }

    return Status;
}

//SPI_StatusTypeDef SPIx_WriteByte(SPI_TypeDef* SPIx, u8 TxData)
//{
//	return SPI_Transmit(SPIx, TxData, 1, 0);
//}

//SPI_StatusTypeDef SPIx_ReadByte(SPI_TypeDef* SPIx, u8 *RxData)
//{
//	return SPI_Receive(SPIx, &RxData, 1, 0);
//}

/**
 * @function 
 * @brief 
 * @param xxx:xxx
 * @return 
 ***/
u8 SPIx_WriteReadByte(SPI_TypeDef* SPIx, u8 TxData)
{
	u8 RxData;
	SPI_TransmitReceive(SPIx, &TxData, &RxData, 1, 0);
	return RxData;
}

