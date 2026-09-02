/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : uart.c
 * Description : uart driver source file
 * Author(s)   : Eric
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#include  "uart.h"
#include "delay.h"
#include <stdarg.h>
#include "LoopList.h"   
#include "cmd_queue.h"

volatile u8 tx_flag = 0;
volatile u8 rx_flag = 0;
volatile u8 uart_rx_buf[32];
volatile u8 rx_count = 0;

void CheckBusy(void)  
{
    
    u32 time_out = 0;
    for (time_out = 0; time_out < 1000; time_out++)
    {
        if (rx_flag)
        {
            Timer0DelayUs(20);
            rx_count = 0;
            rx_flag = 0;
        }
        Timer0DelayUs(30);
    }
}


void UARTA_IRQHandler(void)
{
    u32 temp;
  u8 Res;
    temp = REG_UART_RIS(UARTA);
    //temp = REG_UART_MIS(UARTA);
    if (temp & 0x10)      // Rx int
    {
        while ((REG_UART_FR(UARTA) & 0x10) != 0x10) //read the DR ential Rx fifo  empty
        {

						Res = REG_UART_DR(UARTA);          // 新增：读出数据到Res
// 			uart_rx_buf[rx_count] = Res;

// 			queue_push(Res); 
// 			
// 			rx_count++;
					if (rx_count < 32)
        {
          uart_rx_buf[rx_count] = Res;
          rx_count++;
        }
          queue_push(Res);
          rx_flag = 1;
        }
    }
    else if (temp & 0x20) // Tx int
    {
        if ((REG_UART_FR(UARTA) & 0x80) == 0x80) //Tx fifo empty
        {
            tx_flag = 1;
        }
    }
    else
    {
        REG_UART_ICR(UARTA) = 0xfff; //clear int
    }
}

void UARTB_IRQHandler(void)
{
    u32 temp;
    u8  byte;

    temp = REG_UART_RIS(UARTB);

    if (temp & 0x10)      // Rx int
    {
        while ((REG_UART_FR(UARTB) & 0x10) != 0x10) //排空 Rx FIFO
        {
            byte = (u8)REG_UART_DR(UARTB);   // 只读一次
            rbWriteByte(&rb_t_ssl, byte);
        }
    }
    else if (temp & 0x20) // Tx int
    {
        if ((REG_UART_FR(UARTB) & 0x80) == 0x80) //Tx fifo empty
        {
            tx_flag = 1;
        }
    }
    else
    {
        REG_UART_ICR(UARTB) = 0xfff; //clear int
    }
}



/************************************************************************
 * function   : uart_set_baud_rate
 * Description: uart set baud rate
 * input :
 *         u32 uart_index: Serial port number
 *         u32 cpu_mhz: cpu frequency
 *         u32 baud_rate: Series rate
 * return: none
 ************************************************************************/
void uart_set_baud_rate(u32 uart_index, u32 clk_hz, u32 baud_rate)
{
    u32 temp, divider, remainder, fraction;

    temp = 16 * baud_rate;
    divider = clk_hz / temp;
    remainder = clk_hz % temp;
    temp = 1 + (128 * remainder) / temp;
    fraction = temp / 2;

    REG_UART_IBRD(uart_index) = divider + (fraction >> 6);
    REG_UART_FBRD(uart_index) = fraction & 0x3f;
}
/************************************************************************
 * function   : uart_init
 * Description: uart initial for uart_index, cpu_mhz, baud_rate
 * input :
 *         u32 uart_index: Serial port number
 *         u32 baud_rate: Series rate
 * return: none
 ************************************************************************/
void UartInit(u32 uart_index, u32 baud_rate)
{
    u32 uart_clk_hz;

    if (uart_index == UARTA)
    {
#ifdef LOW_POWER
        enable_module(BIT_UARTA); //enable UARTA
#endif
        uart_clk_hz = PClock;
        REG_SCU_MUXCTRLA = ((REG_SCU_MUXCTRLA & (~(0x0f << 0)))) | (0x05 << 0); //复用成UARTA
        NVIC_ClearPendingIRQ(UARTA_IRQn);
        NVIC_EnableIRQ(UARTA_IRQn);
    }
    else
    {
#ifdef LOW_POWER
        enable_module(BIT_UARTB); //enable UARTB
#endif
        uart_clk_hz = SRCClock / (((REG_SCU_CLKDIV >> 24) & 0x0f) + 1); //默认为5分频
        REG_SCU_MUXCTRLC = ((REG_SCU_MUXCTRLC & (~(0x0f << 18)))) | (0x05 << 18); //复用成UARTB

// #ifdef UARTB_USE_RTSMODE
//         REG_SCU_MUXCTRLC = ((REG_SCU_MUXCTRLC & (~(0x03 << 24)))) | (0x01 << 24);
//         REG_UART_CR(UARTB) |= (1 << 14);
// #endif
// #ifdef UARTB_USE_CTSMODE
//         REG_SCU_MUXCTRLC = ((REG_SCU_MUXCTRLC & (~(0x03 << 22)))) | (0x01 << 22);
//         REG_UART_CR(UARTB) |= (1 << 15);
// #endif
        NVIC_ClearPendingIRQ(UARTB_IRQn);
        NVIC_EnableIRQ(UARTB_IRQn);
    }

    tx_flag = 0;
    rx_flag = 0;

    REG_UART_CR(uart_index) &= ~0x01;            //disable uart
    uart_set_baud_rate(uart_index, uart_clk_hz, baud_rate);

    REG_UART_LCRH(uart_index) = 0x60; //8位数据位?1位停止位?无校验位?关闭FIFO功能
    REG_UART_CR(uart_index) = 0x0301; //enable uart

#ifdef UART_Tx_INT_MODE
    REG_UART_IMSC(uart_index) = 0x030;  //enable Rx/Tx_INT,disable else int
#else
    REG_UART_IMSC(uart_index) = 0x010;  //enable Rx_INT,disable Tx_INT and else int
#endif

    REG_UART_ICR(uart_index) = 0xfff; //clear int
	Timer0DelayMs(1000);
}

/************************************************************************
 * function   : outbyte
 * Description: uart out byte
 * input :
 *         u32 uart_index: Serial port number
 *         char c: out byte
 * return: none
 ************************************************************************/
void outbyte(u32 uart_index, char c)
{
    REG_UART_DR(uart_index) = c;

#ifdef UART_Tx_INT_MODE
    while (!tx_flag);
    tx_flag = 0;
#else
    while (REG_UART_FR(uart_index) & 0x08); //wait for idle
#endif
}
/************************************************************************
 * function   : uart_send_bytes
 * Description: uart send bytes
 * input :
 *         u32 uart_index: Serial port number
 *         u8* buff: out buffer
 *         u32 length: buffer length
 * return: none
 ************************************************************************/
void uart_send_bytes(u32 uart_index, u8 *buff, u32 length)
{
    u32 i;

    for (i = 0; i < length; i++)
    {
        outbyte(uart_index, *buff++);
    }
}

void UartSend(char *databuf)
{
	while (1)
    {
        if ((*databuf) != 0) //直到数据都发送完成
        {
            uart_send_bytes(LCDPORT, (u8*)databuf, 1); //发送一个字节数据
            databuf++;//i++;
        }
        else
        {
            return;
        }
    }

}


struct __FILE  //please select UART NO( UARTA or UARTB)
{
    int handle;
    /* Add whatever you need here */
};
FILE __stdout;
FILE __stdin;

//该函数指向UARTA
int fputc(int ch, FILE *f)
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART */

    outbyte(DEBUG_UART, ch); //debug uart: UARTA or UARTB
    return ch;
}

void printf_arraydata(u8 *arraydata, u32 data_len)
{
    int lenth;

    for (lenth = 0; lenth < data_len; lenth++)
    {
        printf("%02X ", arraydata[lenth]);
    }

    printf("\n");
}

// void UART_printf(u32 uart_index, const char *fmt, ...)
// {
//     uint8_t at_cmd[512];  // 固定小缓冲区
//     va_list args;
// //     uint32_t len;
// // 	  uint32_t i;
//     
//     // 1. 处理可变参数
//     va_start(args, fmt);
// //     len = vsnprintf(buf, sizeof(buf), fmt, args);
//     va_end(args);
//     
//     // 2. 直接发送数据
// //     for(i = 0; i < len; i++) {
// //         uart_send_bytes(,(u8*)&buf[i], 1);
// 			uart_send_bytes(uart_index, at_cmd, sizeof(at_cmd));
// //     }
// }

void UART_printf(u32 uart_index, const char *fmt, ...)
{
    uint8_t at_cmd[512];
    va_list args;
    int len;

    va_start(args, fmt);
    len = vsprintf((char *)at_cmd, fmt, args);   /* 用 vsprintf 替代 vsnprintf */
    va_end(args);

    if (len < 0) {
        return;
    }
    if ((uint32_t)len > sizeof(at_cmd)) {
        len = sizeof(at_cmd);
    }

    uart_send_bytes(uart_index, at_cmd, (uint32_t)len);
}

void SendChar(u8 t)
{
	    REG_UART_DR(UARTA) = t;

#ifdef UART_Tx_INT_MODE
    while (!tx_flag);
    tx_flag = 0;
#else
    while (REG_UART_FR(UARTA) & 0x08); //wait for idle
#endif

	}


