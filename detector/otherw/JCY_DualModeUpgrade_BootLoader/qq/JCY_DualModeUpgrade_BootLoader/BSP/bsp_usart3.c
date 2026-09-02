#include "bsp_usart3.h"
#include "LoopList.h"
#include "bsp_usart1.h"
#include <stdarg.h>
#include "esp8266_config.h"

/*********************************************************************************
* Function    : UART3_Init
* Description : Init  UART3 
* Input       : None
* Output      : None
**********************************************************************************/
void UART3_Init(uint32_t Baudrate) 
{
	GPIO_InitTypeDef GPIO_InitStruct;
	UART_InitTypeDef UART3_InitStruc;

	//Enable GPIO and UART Clock
	System_Module_Enable(EN_UART3);
	System_Module_Enable(EN_GPIOAB);
	//Config GPIO
	/* A9:Tx  A10:Rx */
	GPIO_InitStruct.Pin       = GPIO_PIN_0 | GPIO_PIN_2;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Alternate = GPIO_FUNCTION_4;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	//UART Config
	UART3_InitStruc.UART_BaudRate    = Baudrate;  
	UART3_InitStruc.UART_WordLength  = UART_WORDLENGTH_8B;
	UART3_InitStruc.UART_StopBits    = UART_STOPBITS_1;
	UART3_InitStruc.UART_Parity      = UART_PARITY_NONE;
	UART3_InitStruc.UART_Mode        = UART_MODE_TX_RX;
	UART3_InitStruc.UART_HardwareFlowControl =UART_HWCONTROL_NONE;    
	UART_Init(UART3,&UART3_InitStruc);
	//Enable UART
	UART_Cmd(UART3,ENABLE);  
		/* NVIC Config */
	NVIC_ClearPendingIRQ(UART3_IRQn);
	NVIC_SetPriority(UART3_IRQn, 0);
	NVIC_EnableIRQ(UART3_IRQn);

	UART_ITConfig(UART3, UART_IT_RXNE, ENABLE);	/* 使能接收中断 */	
	
}

void UART3_IRQHandler(void)                    //串口中断
{
	if (READ_UART3_IE_RXI && READ_UART3_IE_RIS)
	{
		REG_GPIOAB_ICR |= 0x10;
		PutData(&rb_t_ssl, (u8*)&REG_UART3_DR, 1);
	}
}

void UART_printf(UART_TypeDef* UARTx, const char *fmt, ...)
{
    char buf[512];  // 固定小缓冲区
    va_list args;
    uint32_t len;
    
    // 1. 处理可变参数
    va_start(args, fmt);
    len = vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    
    // 2. 直接发送数据
    for(uint32_t i = 0; i < len; i++) {
        UART_SendData(UARTx, buf[i]);
    }
}