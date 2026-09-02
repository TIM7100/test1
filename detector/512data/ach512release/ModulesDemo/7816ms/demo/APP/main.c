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

#define MASTER_TEST
//#define SLAVE_TEST
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

#ifdef MASTER_TEST
    iso7816m_test(); //ISO7816 master test
#endif

#ifdef SLAVE_TEST
    iso7816s_test(); //ISO7816 slave  test
#endif	
	while(1)
	{
	}
		
}

