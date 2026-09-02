/*
  ******************************************************************************
  * @file    main.c
  * @brief   main source File.
  ******************************************************************************
*/

#include "uart.h"

#define SSID "XXY"
#define Password "goodeveryday"
//__attribute__((section("ROM2_TEST"),used)) const u8 bufferxx[] = {
//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,
//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,
//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,
//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,
//};

extern u8 Res[0x200];
int16_t timeout = 50;
extern u16 i;
#define WIFI_SET_RST_IO      (GPIO_SetBits(GPIOB, GPIO_PIN_1))
#define WIFI_RESET_RST_IO    (GPIO_ResetBits(GPIOB, GPIO_PIN_1))

void WiFi_IO_Init(void)
{
    GPIO_InitTypeDef GPIO_Handle;
    System_Module_Enable(EN_GPIOAB);

    GPIO_Handle.Pin       = GPIO_PIN_1;
    GPIO_Handle.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_Handle.Pull      = GPIO_NOPULL;
    GPIO_Handle.Alternate = GPIO_FUNCTION_0;

    GPIO_Init(GPIOB, &GPIO_Handle);
}

int main(void)
{

    u32 Value;

    System_Init();
	UART3_Init(115200);
	//*(u32*)(0x90000000) = 0x12345678;
	
	WiFi_IO_Init();
	
	WIFI_RESET_RST_IO;                                //复位IO拉低电平
    System_Delay_MS(50);                              //延时，至少200us以上低电平
    WIFI_SET_RST_IO;                                  //复位IO拉高电平
    System_Delay_MS(1500);                             //延时
	
	i = 0;
	timeout = 50;
	memset(Res, 0, sizeof(Res));
	UART_printf(UART3,"AT\r\n");
	while (timeout--)                                   //等待超时时间到0
    {
        System_Delay_MS(10);                                   //延时10ms
		if (strstr((char *)Res, "OK"))          //如果接收到OK表示指令成功
		{
			break;                                      //主动跳出while循环
		}
	}
	
	i = 0;
	timeout = 50;
	memset(Res, 0, sizeof(Res));
	UART_printf(UART3,"ATE0\r\n");
	while (timeout--)                                   //等待超时时间到0
    {
        System_Delay_MS(10);                                   //延时10ms
		if (strstr((char *)Res, "OK"))          //如果接收到OK表示指令成功
		{
			break;                                      //主动跳出while循环
		}
	}
	
	i = 0;
	timeout = 50;
	memset(Res, 0, sizeof(Res));
	UART_printf(UART3,"AT+CWMODE=1\r\n");
	while (timeout--)                                   //等待超时时间到0
    {
        System_Delay_MS(10);                                   //延时10ms
		if (strstr((char *)Res, "OK"))          //如果接收到OK表示指令成功
		{
			break;                                      //主动跳出while循环
		}
	}
	
	i = 0;
	timeout = 50;
	memset(Res, 0, sizeof(Res));
	UART_printf(UART3,"AT+CWAUTOCONN=0\r\n");
	while (timeout--)                                   //等待超时时间到0
    {
        System_Delay_MS(10);                                   //延时10ms
		if (strstr((char *)Res, "OK"))          //如果接收到OK表示指令成功
		{
			break;                                      //主动跳出while循环
		}
	}
	
	i = 0;
	timeout = 1000;
	memset(Res, 0, sizeof(Res));
	UART_printf(UART3,"AT+CWJAP\r\n");
	while (timeout--)                                   //等待超时时间到0
    {
        System_Delay_MS(10);                                   //延时10ms
		if (strstr((char *)Res, "OK"))          //如果接收到OK表示指令成功
		{
			break;                                      //主动跳出while循环
		}
	}
	
	i = 0;
	timeout = 3000;
	memset(Res, 0, sizeof(Res));
	UART_printf(UART3,"AT+CWJAP=\"XXY\",\"goodeveryday\"\r\n");
	while (timeout--)                                   //等待超时时间到0
    {
        System_Delay_MS(10);                                   //延时10ms
		if (strstr((char *)Res, "OK"))          //如果接收到OK表示指令成功
		{
			break;                                      //主动跳出while循环
		}
	}
	
	i = 0;
	timeout = 50;
	memset(Res, 0, sizeof(Res));
	UART_printf(UART3,"AT+CWJAP?\r\n");
	while (timeout--)                                   //等待超时时间到0
    {
        System_Delay_MS(10);                                   //延时10ms
		if (strstr((char *)Res, "OK"))          //如果接收到OK表示指令成功
		{
			break;                                      //主动跳出while循环
		}
	}
	
    while (1)
    {
		


    }
}

