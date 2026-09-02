/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : main.c
 * Description : main source file
 * Author(s)   : Eric
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#include "delay.h"
#include "uart.h"
#include "ExternalFlash.h"
#include "w5500_user_conf.h"
#include "UserFunctionInterface.h"
#include "lcd.h"
#include "gpio.h"
#include "rtc.h"
#include "esp8266_config.h"
#include "use_lcd.h"
/***********************************************************************
 * main主函数
 * 输入参数 ：无
 * 返回值   ：无
 * 函数功能 ：主程序入口函数，各个模块初始化以及各个模块分支子函数的轮询
 ***********************************************************************/
int main(void)
{
	__set_PRIMASK(0);
  SystemInit();                         //系统初始化配置
  Timer0DelayInit();                    //定时器配置
	UartInit(UARTB, 921600);
	WiFi_Init();
	UartInit(LCDPORT, UART_BAUD_RATE);  //串口A初始化
	ExFlashInit();			               //SPI接口初始化
	W5500Config();
	Ds1302Init();
	FLASH_readinit();
	Sensitivity();
	DeviceInit();
	
    while (1)
    {
			   User_App();
    }
}
