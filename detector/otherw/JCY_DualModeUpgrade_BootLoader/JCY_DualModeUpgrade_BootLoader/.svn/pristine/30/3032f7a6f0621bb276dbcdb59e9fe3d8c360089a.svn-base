/*
  ******************************************************************************
  * @file    fxx_uart.c
  * @version V1.0.0
  * @date    2021
  * @brief   UART  module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Universal Asynchronous Receiver Transmitter Peripheral (UART).
  *           @ Initialization and de-initialization functions
  *           @ IO operation functions
  *           @ Peripheral Control functions
  ******************************************************************************
*/
#include "fxx_std.h"


/* If Use 'UART_MODE_TX_RX_DEBUG', Point to Debug Uart */
UART_TypeDef    *Uart_Debug = NULL;

/* Private function prototypes -----------------------------------------------*/
static void UART_Config_BaudRate(UART_TypeDef* UARTx,UART_InitTypeDef *UART_InitStructure);



/*********************************************************************************
* Function    : UART_Cmd
* Description : Enables or disables the specified USART peripheral.
* Input       : UARTx: Select tthe UART peripheral. 
*               This parameter can be one of the following values:
*               UART1, UART2, UART3, UART4 .
* Input       : NewState: new state of the UARTx peripheral.
*               This parameter can be: ENABLE or DISABLE. 
*               peripheral.
* Output      : None
**********************************************************************************/
void UART_Cmd(UART_TypeDef* UARTx, FunctionalState NewState)
{

#if (USE_FULL_ASSERT == 1)
  /* Check the parameters */
  assert_param(IS_UART_ALL_INSTANCE(UARTx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
#endif

  if (NewState != DISABLE)
  {
    /* Enable the selected UART by setting the UARTEN bit in the CR register */
    UARTx->CR |= UART_CR_UARTEN;
  }
  else
  {
    /* Disable the selected USART by clearing the UARTEN bit in the CR register */
    UARTx->CR &= UART_CR_UARTEN;
  }
}

/*********************************************************************************
* Function    : HAL_UART_Init
* Description : Initialize the UART mode according to the specified parameters 
*               in the UART_InitTypeDef
* Input       : UARTx: Select tthe UART peripheral. 
*               This parameter can be one of the following values:
*               UART1, UART2, UART3, UART4 .
* Input       : UART_InitStruct: pointer 
*               that contains the configuration information for the specified UART 
*               peripheral.
* Output      : None
**********************************************************************************/
void UART_Init(UART_TypeDef* UARTx, UART_InitTypeDef* UART_InitStruct)
{

#if (USE_FULL_ASSERT == 1)
    assert_param(IS_UART_ALL_INSTANCE(UARTx));
    assert_param(IS_UART_WORD_LENGTH(UART_InitStruct->UART_WordLength));
    assert_param(IS_UART_STOPBITS(UART_InitStruct->UART_StopBits));
    assert_param(IS_UART_PARITY(UART_InitStruct->UART_Parity));
    assert_param(IS_UART_MODE(UART_InitStruct->UART_Mode));
    assert_param(IS_UART_HARDWARE_FLOW_CONTROL(UART_InitStruct->UART_HardwareFlowControl));
#endif

    /* Config BaudRate */
    UART_Config_BaudRate(UARTx,UART_InitStruct);
    
    /* Set the UART Communication parameters */
    UARTx->LCRH = UART_InitStruct->UART_WordLength | UART_LCRH_FEN | UART_InitStruct->UART_StopBits | UART_InitStruct->UART_Parity;
    UARTx->CR   = UART_InitStruct->UART_HardwareFlowControl | UART_InitStruct->UART_Mode;
	UARTx->LCRH &= ~UART_LCRH_FEN;
	
    
    if (UART_InitStruct->UART_Mode == UART_MODE_TX_RX_DEBUG) 
    {
        Uart_Debug = UARTx;
    }
    else if (UART_InitStruct->UART_Mode == UART_MODE_HALF_DUPLEX) 
    {
        UARTx->CR2 = UART_CR2_TXOE_SEL;
    }

}


/*********************************************************************************
* Function    : UART_Init
* Description : Initialize the UART mode according to the specified parameters 
*               in the UART_InitTypeDef
* Input       : huart: UART handle.
* Output      : 
**********************************************************************************/
void UART_DeInit(UART_TypeDef* UARTx)
{

#if (USE_FULL_ASSERT == 1)
    
#endif
    
}

/*********************************************************************************
* Function    : UART_StructInit
* Description : Fills each UART_InitStruct member with its default value.
* Input       : UART_InitStruct: UART_InitTypeDef .
* Output      : None
**********************************************************************************/
void UART_StructInit(UART_InitTypeDef* UART_InitStruct)
{
  /* USART_InitStruct members default value */
  UART_InitStruct->UART_BaudRate = 9600;
  UART_InitStruct->UART_WordLength = UART_WORDLENGTH_8B;
  UART_InitStruct->UART_StopBits = UART_STOPBITS_1;
  UART_InitStruct->UART_Parity = UART_PARITY_NONE ;
  UART_InitStruct->UART_Mode = UART_MODE_TX_RX;
  UART_InitStruct->UART_HardwareFlowControl = UART_HWCONTROL_NONE;  
}

/*********************************************************************************
* Function    : UART_ITConfig
* Description : Enables or disables the specified USART interrupts.              
* Input       : UARTx: Select tthe UART peripheral. 
*               This parameter can be one of the following values:
*               UART1, UART2, UART3, UART4 .
* Input       : UART_IT: specifies the UART interrupt sources to be enabled or disabled.
*               This parameter can be  UART_IT_TXE UART_IT_RXNE ...
* Input       : NewState: new state of the specified USARTx interrupts.
*               This parameter can be: ENABLE or DISABLE.     
* Output      : None
**********************************************************************************/
void UART_ITConfig(UART_TypeDef* UARTx, uint16_t UART_IT, FunctionalState NewState)
{
   uint32_t uartxbase = 0x00;
   uint32_t itmask = 0x00,itpos = 0x00;

#if (USE_FULL_ASSERT == 1)
    /* Check the parameters */
   assert_param(IS_UART_ALL_INSTANCE(UARTx));
   assert_param(IS_UART_CONFIG_IT(UART_IT));
   assert_param(IS_FUNCTIONAL_STATE(NewState));
#endif

   uartxbase = (uint32_t)UARTx->IE;
   /* Disable FIFO */
   UARTx->LCRH &= ~(UART_LCRH_FEN);

  /* Get the interrupt position */
   itpos    = UART_IT & IT_MASK;
   itmask   = 1 << itpos ;
	
   if (NewState != DISABLE)
   {
		 UARTx->IE |= itmask;
   }
   else
   {
		 UARTx->IE &= ~itmask;
   }
}

/*********************************************************************************
* Function    : UART_ClearITPendingBit
* Description : Clears the UARTx's interrupt pending bits.              
* Input       : UARTx: Select the UART peripheral. 
*               This parameter can be one of the following values:
*               UART1, UART2, UART3, UART4 .
* Input       : UART_IT: specifies the UART interrupt sources to be enabled or disabled.
*               This parameter can be  UART_IT_TXE UART_IT_RXNE ...   
* Output      : None
**********************************************************************************/
void UART_ClearITPendingBit(UART_TypeDef* UARTx, uint32_t UART_IT)
{
  uint32_t bitpos = 0x00, itmask = 0x00;
	
#if (USE_FULL_ASSERT == 1)
  /* Check the parameters */
  assert_param(IS_UART_ALL_INSTANCE(UARTx));
  assert_param(IS_UART_CLEAR_IT(UART_IT));
#endif
   
    bitpos = UART_IT;

	itmask = 1 << bitpos;
	
    UARTx->ICR = (uint32_t)itmask;
}

/*********************************************************************************
* Function    : USART_SendData
* Description : Transmits single data through the UARTx peripheral.
* Input       : UARTx: Select the UART peripheral. 
*               This parameter can be one of the following values:
*               UART1, UART2, UART3, UART4 .
* Input       : Data: the data to transmit.
* Output      : None
**********************************************************************************/
void UART_SendData(UART_TypeDef* UARTx, uint8_t Data)
{

#if (USE_FULL_ASSERT == 1)
  /* Check the parameters */
  assert_param(IS_UART_ALL_INSTANCE(UARTx));
#endif
    
  /* Transmit Data */
  UARTx->DR = Data ;

  while(UARTx->FR & UART_FR_BUSY);
}

/*********************************************************************************
* Function    : UART_ReceiveData
* Description : Transmits single data through the UARTx peripheral.
* Input       : UARTx: Select the UART peripheral. 
*               This parameter can be one of the following values:
*               UART1, UART2, UART3, UART4 .
* Input       : Data: the data to transmit.
* retval      : The received data.
**********************************************************************************/
uint8_t UART_ReceiveData(UART_TypeDef* UARTx)
{
  /* Check the parameters */
  assert_param(IS_UART_ALL_INSTANCE(UARTx));
  
  while(UARTx->FR & UART_FR_RXFE);
  /* Receive Data */
  return (uint8_t)UARTx->DR;
}

/*********************************************************************************
* Function    : UART_GetITStatus
* Description : Checks whether the specified USART interrupt has occurred or not.
* Input       : UARTx: Select the UART peripheral. 
*               This parameter can be one of the following values:
*               UART1, UART2, UART3, UART4 .
* Input       : UART_IT: specifies the UART interrupt source to check.
*               This parameter can be :UART_IT_TXE,USART_IT_RXNE...
* retval      : The new state of UART_IT (SET or RESET).
**********************************************************************************/
ITStatus UART_GetITStatus(UART_TypeDef* UARTx, uint16_t UART_IT)
{
  uint32_t bitpos = 0x00, itmask = 0x00;
  ITStatus bitstatus = RESET;

#if (USE_FULL_ASSERT == 1)
  /* Check the parameters */
  assert_param(IS_UART_ALL_INSTANCE(UARTx));
  assert_param(IS_UART_GET_IT(UART_IT));
#endif

  
  /* Get the interrupt position */
  itmask  = UART_IT & IT_MASK;
  itmask  = (uint32_t)0x01 << itmask;
  itmask &= UARTx->IE;
  
  bitpos = UART_IT & IT_MASK;
  bitpos = (uint32_t)0x01 << bitpos;
  bitpos &= UARTx->RIS;

  if ((itmask != (uint16_t)RESET)&&(bitpos != (uint16_t)RESET))
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  
  return bitstatus;  
}

/*********************************************************************************
* Function    : UART_DMACmd
* Description : Enables or disables the UART DMA interface.              
* Input       : UARTx: Select the UART peripheral. 
*               This parameter can be one of the following values:
*               UART1, UART2, UART3, UART4 .
* Input       : UART_DMAReq: specifies the DMA request.
*               This parameter can be  UART_DMACR_TXDMAE or UART_DMACR_RXDMAE ...
* Input       : NewState: new state of the specified UARTx interrupts.
*               This parameter can be: ENABLE or DISABLE.     
* Output      : None
**********************************************************************************/
void UART_DMACmd(UART_TypeDef* UARTx, uint32_t UART_DMAReq, FunctionalState NewState)
{
#if (USE_FULL_ASSERT == 1)
	/* Check the parameters */
	assert_param(IS_UART_ALL_INSTANCE(UARTx));  
	assert_param(IS_FUNCTIONAL_STATE(NewState)); 
#endif
	
	if (NewState != DISABLE)
	{
	/* Enable the DMA transfer for selected requests by setting the TXDMAE and/or
	   RXDMAE bits in the UART DMACR register */
	 UARTx->DMACR |= UART_DMAReq;
	}
	else
	{
	/* Disable the DMA transfer for selected requests by clearing the TXDMAE and/or
	   RXDMAE bits in the UART DMACR register */
	 UARTx->DMACR &= (uint32_t)~UART_DMAReq;
	}
}


/*********************************************************************************
* Function    : UART_Config_BaudRate
* Description : Config BaudRate
* Input       : UARTx: Select the UART peripheral. 
*               This parameter can be one of the following values:
*               UART1, UART2, UART3, UART4 . 
* Input       : UART_InitStruct: pointer that contains the configuration 
*               information for the specified UART 
*               peripheral.
* Output      : None
**********************************************************************************/
static void UART_Config_BaudRate(UART_TypeDef* UARTx,UART_InitTypeDef *UART_InitStructure)
{
    uint32_t lu32_PCLK;
    uint32_t lu32_IBAUD, lu32_FBAUD;
    uint64_t lu64_TempValue;

    lu32_PCLK = System_Get_APBClock();

    /* Integral part */
    lu32_IBAUD = lu32_PCLK / (UART_InitStructure->UART_BaudRate * 16);

    /* Fractional part */
    lu64_TempValue = lu32_PCLK % (UART_InitStructure->UART_BaudRate * 16);
    lu64_TempValue = (lu64_TempValue * 1000000) / (UART_InitStructure->UART_BaudRate * 16);
    lu32_FBAUD     = (lu64_TempValue * 64 + 500000) / 1000000;

    if (lu32_FBAUD >= 64) 
    {
        UARTx->IBRD = lu32_IBAUD + 1;
        UARTx->FBRD = 0;
    }
    else 
    {
        UARTx->IBRD = lu32_IBAUD;
        UARTx->FBRD = lu32_FBAUD;
    }
}


/*********************************************************************************
* Function    : fputc
* Description : 
* Input       : 
* Output      : 
* Author      : Chris_Kyle                         Data : 2020 
**********************************************************************************/
int fputc(int ch, FILE *f)
{
    if (Uart_Debug == NULL) 
    {
        return 0;
    }

    Uart_Debug->DR = ch;
    
    while ((Uart_Debug->FR & UART_FR_BUSY));
    
    return ch;
}

