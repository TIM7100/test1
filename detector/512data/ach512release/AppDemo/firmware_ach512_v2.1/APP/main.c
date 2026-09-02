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
#include  "usb_app.h"

/***********************************************************************
 * main主函数
 * 输入参数 ：无
 * 返回值   ：无
 * 函数功能 ：主程序入口函数，各个模块初始化以及各个模块分支子函数的轮询
 ***********************************************************************/
int main(void)
{
	SystemInit();

	uart_init(DEBUG_UART, UART_BAUD_RATE);

#ifdef LOW_POWER
    enable_module(BIT_GPIO); //enable GPIO
#endif
	//设置GPIO2为输入功能
	REG_SCU_MUXCTRLA &= ~(3 << 4);
	REG_GPIO_DIR(GPIOA) &= ~(1 << 2);
	
	if(REG_GPIO_IDATA(GPIOA) & (1 << 2))
	{
		usb_protocol = USB_PROTOCOL_AISINOCHIP;
	}
	else
	{
		usb_protocol = USB_PROTOCOL_UMS;
	}	
#ifdef LOW_POWER
    disable_module(BIT_GPIO); //disable GPIO
#endif
	
	usb_initial();     

	while(1)
	{
		usb_transfer_monitor();
	}
}

