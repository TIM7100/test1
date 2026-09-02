/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : app.c
 * Description : application example source file
 * Author(s)   : Eric
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#include "app.h"
#include "i2c.h"
#include "at24cxx.h"

void i2c_test(void)
{
	UINT8 i;
	UINT8 wr_buff[128], rd_buff[128];
	printfS("-------at24cxx test i2c is start!-------\n");
	i2c_init(MASTER_I2C_SPEED);


	for(i = 0; i < 128; i++)
	{
		wr_buff[i] = (UINT8)i;
	}
	printfS("i2c write data\n");
	at24cxx_write_bytes(0x08, wr_buff, 128);

	printfS("i2c read data\n");
	at24cxx_read_bytes(0x08, rd_buff, 128);


	for(i = 0; i < 128; i++)
	{
		if(wr_buff[i] != rd_buff[i])
		{
			printfS("test fail: \n"); 
            printfS("wr_buff[%d]= 0x%x,rd_buff[%d]= 0x%x \n", i, wr_buff[i], i, rd_buff[i]);
			return;
		}
	}

	printfS("-------at24cxx test i2c is end!-------\n");
}
