#include "hw_config.h"
#include "cmd_queue.h"
#include "hmi_user_uart.h"

/*!
*   \brief   串口初始化
*   \param   BaudRate-波特率设置
*/
void UartInit(uint32 BaudRate)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    /* Enable GPIO clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    /* Configure USART Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //  /* Configure USART Rx as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    USART_DeInit(USART1);                                           //复位USART1
    USART_InitStructure.USART_BaudRate = BaudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode =   USART_Mode_Tx|USART_Mode_Rx;

    /* USART configuration */
    USART_Init(USART1, &USART_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                  //接收中断使能

    /* Enable USART */
    USART_Cmd(USART1, ENABLE);
}

/*!
*   \brief  发送1个字节
*   \param  t 发送的字节
*/
void SendChar(uchar t)
{
    USART_SendData(USART1,t);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    while((USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET));//等待串口发送完毕
}

void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        uint8_t data = USART_ReceiveData(USART1);
        queue_push(data);
//SendChar(data);
    }

}

