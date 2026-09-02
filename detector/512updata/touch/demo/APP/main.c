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
#include "ExternalFlash.h"
#include "delay.h"
#include "use_lcd.h"
#include "w5500_user_conf.h"
#include "esp8266_config.h"
#include "string.h"
/***********************************************************************
 * main主函数
 * 输入参数 ：无
 * 返回值   ：无
 * 函数功能 ：主程序入口函数，各个模块初始化以及各个模块分支子函数的轮询
 ***********************************************************************/

int main(void)
{
	SystemInit();
	uart_init(UARTA, UART_BAUD_RATE);
	   uart_init(UARTB, 921600);
	
	
	ExFlashInit();			               //SPI接口初始?
    Timer0DelayInit();                    //定时器配置
		ExFlashInit();			               //SPI接口初始化
	W5500Config();

	KeyInit();
	DeviceInit();
	
	WiFi_IO_Init();



	
	while(1)
	{
		User_App();
    }
}

