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
#define WiFi_Send(buf)    uart_send_bytes(UARTB, (UINT8*)buf, sizeof(buf)-1)
extern volatile UINT16 rx_count;
int32_t timeout = 50;
extern volatile UINT8 uart_rx_buf[0x1000];
/***********************************************************************
 * main??????
 * ??????? ????
 * ?????   ????
 * ???????? ?????????????????????????????????????????????????
 ***********************************************************************/
int main(void)
{	uint8_t at_cmd[] = "AT\r\n";
	uint8_t at_cmd1[] = "ATE0\r\n";
		uint8_t at_cmd2[] = "AT+CWMODE=1\r\n";
			uint8_t at_cmd3[] = "AT+CWAUTOCONN=0\r\n";
		uint8_t at_cmd4[] = "AT+CWJAP\r\n";
	uint8_t at_cmd5[] = "AT+CWJAP=\"XXY\",\"goodeveryday\"\r\n";
		uint8_t at_cmd6[] = "AT+CWJAP?\r\n";
	
    SystemInit();
    uart_init(UARTB, UART_BAUD_RATE);
    Timer0DelayInit();


    WiFi_IO_Init();
    WIFI_RESET_RST_IO;                                //??¦ËIO??????
    Timer0DelayMs(50)  ;                        //?????????200us???????
    WIFI_SET_RST_IO;                                  //??¦ËIO??????
    Timer0DelayMs(1500)  ;                         //???
//   rx_count = 0;
  
timeout = 10000;
    memset((void *)&uart_rx_buf, 0, sizeof(uart_rx_buf));
// 	WiFi_printf("AT\r\n");
//   WiFi_printf("%s\r\n", at_cmd);
	WiFi_Send("AT\r\n");
// uart_send_bytes(UARTB, at_cmd, 4);

    while (timeout--)                                   //?????????0
    {
        //???10ms
        if (strstr((char *)uart_rx_buf, "OK") != NULL)
        {
            Timer0DelayMs(50)  ;
            // ??? OK
            break;
        }
    }
		
// 	  rx_count = 0;
// timeout = 500000;
// 	memset((void *)&uart_rx_buf, 0, sizeof(uart_rx_buf));


// uart_send_bytes(UARTB, at_cmd1, 6);

// 	while (timeout--)                                   //?????????0
// 	{
// 			//???10ms
// 			if (strstr((char *)uart_rx_buf, "OK") != NULL)
// 			{
// 					Timer0DelayMs(50)  ;
// 					// ??? OK
// 					break;
// 			}
// 	}	
		
	  rx_count = 0;
timeout = 500000;
	memset((void *)&uart_rx_buf, 0, sizeof(uart_rx_buf));


uart_send_bytes(UARTB, at_cmd2, 14);

	while (timeout--)                                   //?????????0
	{
			//???10ms
			if (strstr((char *)uart_rx_buf, "OK") != NULL)
			{
					Timer0DelayMs(50)  ;
					// ??? OK
					break;
			}
	}	
	
  rx_count = 0;
timeout = 500000;
	memset((void *)&uart_rx_buf, 0, sizeof(uart_rx_buf));


uart_send_bytes(UARTB, at_cmd3, 17);

	while (timeout--)                                   //?????????0
	{
			//???10ms
			if (strstr((char *)uart_rx_buf, "OK") != NULL)
			{
					Timer0DelayMs(50)  ;
					// ??? OK
					break;
			}
	}	
// 	
// 	
// 	  rx_count = 0;
// timeout = 1000000;
// 	memset((void *)&uart_rx_buf, 0, sizeof(uart_rx_buf));


// uart_send_bytes(UARTB, at_cmd4, 10);

// 	while (timeout--)                                   //?????????0
// 	{
// 			//???10ms
// 			if (strstr((char *)uart_rx_buf, "OK") != NULL)
// 			{
// 					Timer0DelayMs(50)  ;
// 					// ??? OK
// 					break;
// 			}
// 	}	
		  rx_count = 0;
timeout = 5000000;
	memset((void *)&uart_rx_buf, 0, sizeof(uart_rx_buf));
	uart_send_bytes(UARTB, at_cmd5, 34);

	while (timeout--)                                   //?????????0
	{
			//???10ms
			if (strstr((char *)uart_rx_buf, "OK") != NULL)
			{
					Timer0DelayMs(50)  ;
					// ??? OK
					break;
			}
	}	
    while (1)
    {
        ;
    }
}

