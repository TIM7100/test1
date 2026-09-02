#include "bsp_usart1.h"

/*********************************************************************************
* Function    : UART1_Init
* Description : Init  UART1
* Input       : None
* Output      : None
**********************************************************************************/
/*
    只使用串口接收中断
*/
void UART1_Init(uint32_t Baudrate)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    UART_InitTypeDef UART1_InitStruc;

    //Enable GPIO and UART Clock
    System_Module_Enable(EN_UART1);
//  System_Module_Enable(EN_GPIOAB);
    //Config GPIO
    /* A9:Tx  A10:Rx */
    GPIO_InitStruct.Pin       = GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Alternate = GPIO_FUNCTION_2;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    //UART Config
    UART1_InitStruc.UART_BaudRate    = Baudrate;
    UART1_InitStruc.UART_WordLength  = UART_WORDLENGTH_8B;
    UART1_InitStruc.UART_StopBits    = UART_STOPBITS_1;
    UART1_InitStruc.UART_Parity      = UART_PARITY_NONE;
    UART1_InitStruc.UART_Mode        = UART_MODE_TX_RX_DEBUG;
    UART1_InitStruc.UART_HardwareFlowControl = UART_HWCONTROL_NONE;
    UART_Init(UART1, &UART1_InitStruc);
    //Enable UART
    UART_Cmd(UART1, ENABLE);

    /* NVIC Config */
    NVIC_ClearPendingIRQ(UART1_IRQn);
    NVIC_SetPriority(UART1_IRQn, 5);
    NVIC_EnableIRQ(UART1_IRQn);

    UART_ITConfig(UART1, UART_IT_RXNE, ENABLE); /* 使能接收中断 */
	printf("DEBUG INIT");
}

void UartSend_bytes(UART_TypeDef* UARTx, uint8_t *SendBuf, uint32_t DataLen)
{
    uint32_t i;

    for (i = 0; i < DataLen; i++)
    {
        UART_SendData(UARTx, SendBuf[i]);
    }


}

void UART1_ParaInit(Type_UART_t* UARTHandle, uint8_t* RxBuffer, u32 Size)
{
    UARTHandle->uart = UART1;

    UARTHandle->pRxBuf = RxBuffer;

    UARTHandle->RxBufSize = Size;

    UARTHandle->RxWrite = 0;
    UARTHandle->RxLength = 0;

    UARTHandle->Timeout = 30000;                //1 ms/次
}


void UART1_ParaClear(Type_UART_t* UARTHandle, uint8_t* RxBuffer, u32 Size)
{

    UARTHandle->pRxBuf = RxBuffer;
    UARTHandle->pRxBuf[0] = 0;
    UARTHandle->RxBufSize = Size;

    UARTHandle->RxLength = 0;
    UARTHandle->RxWrite = 0;

    UARTHandle->Timeout = 0xffffffff;
}


void UART1_WaitRxDataBeForeTimeOut(Type_UART_t* UARTHandle)
{
    printfS("Waiting Receive UpData\r\n");
    while (UARTHandle->Timeout--)
    {
        System_Delay_MS(1);
    }

    /* 200ms后，还没接到下一个数据，就表示数据接收成功 */
    printfS("UpData Receive Success: Size = %d\r\n", UARTHandle->RxLength);
    UARTHandle->Timeout = 0xffffffff;
}



void UART1_IRQ(Type_UART_t *pUart)
{
    static uint32_t Start_Address = 0x100000;


    /* 接收中断 */
    if (UART_GetITStatus(pUart->uart, UART_IT_RXNE) != RESET)
    {
        UART_ClearITPendingBit(pUart->uart, UART_IT_RXNE);

        /* Read one byte from the receive data register */
        pUart->pRxBuf[pUart->RxLength] = (uint32_t)UART_ReceiveData(pUart->uart);

        /* 接收的长度 */
        pUart->RxLength++;

//      /* 数组的接收指针 */
//      pUart->RxWrite++;

        //收到 g_UARTHandle->lu16_RxSize 长度的数据，代表需要写入一次Flash区  (pUart->lu32_RxLength > 0) &&
//      if (((pUart->lu32_RxWrite % pUart->RxSize) == 0))
//      {
//          //回调函数
//          if (pUart->Rx_Func)
//          {
//              pUart->Rx_Func(Start_Address, pUart->lu16_RxSize, pUart->pRxBuf);
//              Start_Address = 0x100000 + pUart->lu32_RxLength;
//          }
//      }

        if (pUart->RxLength >= pUart->RxBufSize) //防止溢出
        {
            pUart->RxLength = 0;
        }
        pUart->Timeout = 200;           //超时时间
    }
}
