/*
  ******************************************************************************
  * @file    main.c
  * @brief   main source File.
  ******************************************************************************
*/

#include "uart.h"

//__attribute__((section("ROM2_TEST"),used)) const u8 bufferxx[] = {
//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,
//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,
//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,
//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,
//};

extern u8 Res[];
int16_t timeout = 50;
int main(void)
{

    u32 Value;

    System_Init();
	UART3_Init(115200);
	//*(u32*)(0x90000000) = 0x12345678;
	UART_printf(UART3,"AT+UART=921600,8,1,0,0\r\n");
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

