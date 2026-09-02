#include "uart.h"
#include "crc.h "
#include <stdarg.h>

//#define UARTB

u8 RX_BUF[3]; //全局变量，中断中修改接收值
u8 UartRxFinished = 0;
u8 UartRxCount = 0;

volatile u8 rx_flag = 0;
volatile u8 rx_count = 0;

GPIO_InitTypeDef GPIO_InitStruct;
UART_InitTypeDef UART1_InitStruc;
UART_InitTypeDef UART2_InitStruc;

void UartInit(uint32_t Baudrate)
{

    //Enable GPIO and UART Clock
    System_Module_Enable(EN_UART1);
    System_Module_Enable(EN_GPIOAB);
    //Config GPIO
    /* A2:Tx  A3:Rx */
    GPIO_InitStruct.Pin       = GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Alternate = GPIO_FUNCTION_2;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

//    GPIO_InitStruct.Pin       = GPIO_PIN_10;
//    GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
//    GPIO_Init(GPIOA, &GPIO_InitStruct);

    //UART Config
    UART1_InitStruc.UART_BaudRate    = 115200;
    UART1_InitStruc.UART_WordLength  = UART_WORDLENGTH_8B;
    UART1_InitStruc.UART_StopBits    = UART_STOPBITS_1;
    UART1_InitStruc.UART_Parity      = UART_PARITY_NONE;
    UART1_InitStruc.UART_Mode        = UART_MODE_TX_RX;
    UART1_InitStruc.UART_HardwareFlowControl = UART_HWCONTROL_NONE;
    UART_Init(UART1, &UART1_InitStruc);
    /* NVIC Config */
    NVIC_ClearPendingIRQ(UART1_IRQn);
    NVIC_SetPriority(UART1_IRQn, 5);
    NVIC_EnableIRQ(UART1_IRQn);

    UART_ITConfig(UART1, UART_IT_RXNE, ENABLE);   //中断初始化

    UART_Cmd(UART1, ENABLE);
    Delay_ms(1000);

    //Enable GPIO and UART Clock
    System_Module_Enable(EN_UART2);
    System_Module_Enable(EN_GPIOAB);
    //Config GPIO
    /* A2:Tx  A3:Rx */
    GPIO_InitStruct.Pin       = GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Alternate = GPIO_FUNCTION_2;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

//    GPIO_InitStruct.Pin       = GPIO_PIN_10;
//    GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
//    GPIO_Init(GPIOA, &GPIO_InitStruct);

    //UART Config
    UART2_InitStruc.UART_BaudRate    = 115200;
    UART2_InitStruc.UART_WordLength  = UART_WORDLENGTH_8B;
    UART2_InitStruc.UART_StopBits    = UART_STOPBITS_1;
    UART2_InitStruc.UART_Parity      = UART_PARITY_NONE;
    UART2_InitStruc.UART_Mode        = UART_MODE_TX_RX;
    UART2_InitStruc.UART_HardwareFlowControl = UART_HWCONTROL_NONE;
    UART_Init(UART2, &UART2_InitStruc);
    /* NVIC Config */
    NVIC_ClearPendingIRQ(UART2_IRQn);
    NVIC_SetPriority(UART2_IRQn, 5);
    NVIC_EnableIRQ(UART2_IRQn);

    UART_ITConfig(UART2, UART_IT_RXNE, ENABLE);   //中断初始化

    UART_Cmd(UART2, ENABLE);
    Delay_ms(1000);
	printfS("sss");
}





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

u8 Res[0x1000];
u16 i = 0;;
void UART3_IRQHandler(void)                    //串口中断
{
    if (UART_GetITStatus(UART3, UART_IT_RXNE) != RESET) //0x0d 0x0a  串口传输状态获取  中断产生
    {
		Res[i] = UART_ReceiveData(UART3); //(USART1->DR);   获取串口的数据
		i++;
		
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




void Uart1Init(uint32_t Baudrate)
{


    //Enable GPIO and UART Clock
    System_Module_Enable(EN_UART1);
    System_Module_Enable(EN_GPIOAB);
    //Config GPIO
    /* A2:Tx  A3:Rx */
    GPIO_InitStruct.Pin       = GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Alternate = GPIO_FUNCTION_2;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

//    GPIO_InitStruct.Pin       = GPIO_PIN_10;
//    GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
//    GPIO_Init(GPIOA, &GPIO_InitStruct);

    //UART Config
    UART1_InitStruc.UART_BaudRate    = 115200;
    UART1_InitStruc.UART_WordLength  = UART_WORDLENGTH_8B;
    UART1_InitStruc.UART_StopBits    = UART_STOPBITS_1;
    UART1_InitStruc.UART_Parity      = UART_PARITY_NONE;
    UART1_InitStruc.UART_Mode        = UART_MODE_TX_RX;
    UART1_InitStruc.UART_HardwareFlowControl = UART_HWCONTROL_NONE;
    UART_Init(UART1, &UART1_InitStruc);
    /* NVIC Config */
    NVIC_ClearPendingIRQ(UART1_IRQn);
    NVIC_SetPriority(UART1_IRQn, 5);
    NVIC_EnableIRQ(UART1_IRQn);

    UART_ITConfig(UART1, UART_IT_RXNE, ENABLE);   //中断初始化

    UART_Cmd(UART1, ENABLE);
    Delay_ms(1000);


}


void CheckBusy(void)					//忙检测
{
    u32 time_out = 0;
    for (;;)
    {
        if (UartRxFinished == 0xc0)
        {
            if ((RX_BUF[0] == 'O') && (RX_BUF[1] == 'K'))
            {
                RX_BUF[0] = 0;
                RX_BUF[1] = 0;
                UartRxFinished = 0;
                UartRxCount = 0;
                break;
            }
        }
        if (time_out++ >= 800000)
        {
            UartRxFinished = 0;
            UartRxCount = 0;
            break;
        }
    }
}

void UartSend(char *databuf)				//串口发送字符串，遇到'\0'结束
{

    while (*databuf != 0) //直到数据都发送完成
    {
        UART_SendData(UART2, *databuf); //发送一个字节数据
        while (UART2->FR & UART_FR_BUSY); //用于检查串口UART1是否发送完成，完成时，TC中断标志置位，
        databuf++;//i++;
    }

}

void UART2_IRQHandler(void)                    //串口中断
{
    u8 Res = 1;
    if (UART_GetITStatus(UART2, UART_IT_RXNE) != RESET) //0x0d 0x0a  串口传输状态获取  中断产生
    {
        Res = UART_ReceiveData(UART2); //(USART1->DR);   获取串口的数据

        if (UartRxFinished != 0xc0) //
        {
            if (UartRxFinished & 0x40) //0x0d 0x0a -->     OK\r\n   模块执行完
            {
                if (Res == 0x0a)
                {
                    UartRxFinished |= 0x80; //  得到UartRxFinished=0xc0，
                    RX_BUF[UartRxCount] = '\0'; //
                }
                else
                {
                    UartRxFinished = 0; 
                    UartRxCount = 0;
                }
            }
            else 
            {
                if (Res != 0x0d)
                {
                    RX_BUF[UartRxCount++] = Res ;
                }
                else
                {
                    UartRxFinished |= 0x40;
                }
            }
        }

    }
}
