/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : main.c
 * Description : main source file
 * Author(s)   : Eric
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#include  "common.h"
#include  "app.h"
#include "string.h"
#include "delay.h"
#define WIFI_SET_RST_IO     REG_GPIO_SET(GPIOA) |= 0x4000000;
#define WIFI_RESET_RST_IO   REG_GPIO_CLR(GPIOA) |= 0x4000000;
void WiFi_IO_Init(void)
{
    REG_SCU_MUXCTRLB &= ~(0x300000);
    REG_GPIO_DIR(GPIOA) |= 0x4000000;
}
void my_memset(UINT16 *ptr, int val, UINT16 size)
{
    UINT16 i;
    for (i = 0; i < size; i++)
    {
        ((unsigned char*)ptr)[i] = val;
    }
}

extern volatile UINT16 rx_count;
int32_t timeout = 50;
extern volatile UINT8 uart_rx_buf[0x1000];
/***********************************************************************
 * main主函数
 * 输入参数 ：无
 * 返回值   ：无
 * 函数功能 ：主程序入口函数，各个模块初始化以及各个模块分支子函数的轮询
 ***********************************************************************/
int main(void)
{	
// 	uint8_t at_cmd[] = "AT+UART=115200,8,1,0,0\r\n";
	uint8_t at_cmd[] = "AT+UART=921600,8,1,0,0\r\n";
    SystemInit();
    uart_init(UARTB, 115200);
    Timer0DelayInit();


    WiFi_IO_Init();
    WIFI_RESET_RST_IO;                                //复位IO拉低电平
    Timer0DelayMs(50)  ;                        //延时，至少200us以上低电平
    WIFI_SET_RST_IO;                                  //复位IO拉高电平
    Timer0DelayMs(1500)  ;                         //延时
  rx_count = 0;
  
timeout = 1000000;
    memset((void *)&uart_rx_buf, 0, sizeof(uart_rx_buf));
	

uart_send_bytes(UARTB, at_cmd, 24);

    while (timeout--)                                   //等待超时时间到0
    {
        //延时10ms
        if (strstr((char *)uart_rx_buf, "OK") != NULL)
        {
            Timer0DelayMs(50)  ;
            // 收到 OK
            break;
        }
    }
		

    while (1)
    {
        ;
    }
}

