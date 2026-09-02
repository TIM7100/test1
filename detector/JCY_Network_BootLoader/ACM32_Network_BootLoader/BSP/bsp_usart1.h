#ifndef __USART1_H__
#define __USART1_H__

#include "fxx_std.h"



//typedef void (*ReciveCallFunc)(uint32_t, uint32_t, uint8_t*); // 声明一个函数指针类型

typedef struct
{
	UART_TypeDef *uart;	            /* 对应的串口指针 */	
	
	uint8_t *pTxBuf;			    /* 发送缓冲区 */
	uint8_t *pRxBuf;			    /* 接收缓冲区 */
	uint32_t TxBufSize;	    /* 发送缓冲区大小 */
	uint32_t RxBufSize;	    /* 接收缓冲区大小 */
	
	uint32_t RxWrite;		    /* 接收缓冲区写指针 */
	volatile uint32_t RxLength;/* 接收缓冲区接收的长度 */
	
	volatile uint32_t Timeout;		/* 超时判断 */
}Type_UART_t;




void UART1_Init(uint32_t Baudrate);
void UART1_ParaInit(Type_UART_t* UARTHandle, uint8_t* RxBuffer, u32 Size);
void UART1_ParaClear(Type_UART_t* UARTHandle, uint8_t* RxBuffer, u32 Size);
void UART1_IRQ(Type_UART_t *pUart);
void UART1_WaitRxDataBeForeTimeOut(Type_UART_t* UARTHandle);


#endif
