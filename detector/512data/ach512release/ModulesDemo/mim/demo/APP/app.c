/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : app.c
 * Description : application example source file
 * Author(s)   : Eric
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#include  "app.h"
#include  "mim.h"


//IS61WVV51216BLL  512K*16bit
void mem_test(void)
{
	UINT32 i, result;
	UINT32 wdata[8];
	UINT32 rdata[8];
	UINT8  *buff8;
	UINT16 *buff16;
	UINT32 *buff32;

	printfS("mim(ex_sram) test\n");
	result = 0;
	mem_bus_init();

	for(i = 0; i < 8; i++)
	{
		wdata[i] = 0x01234567;
		rdata[i] = 0;
	}

//8bit write,8bit 16bit 32bit read
	printfS("write(8bit) test\n");
	buff8 = (UINT8 *)wdata;
	for(i = 0; i < 32; i++)
	{
		MEM0_PORT8(i) = buff8[i];
	}
	printfS("read(8bit) test\n");
	buff8 = (UINT8 *)rdata;
	for(i = 0; i < 32; i++)
	{
		buff8[i] = MEM0_PORT8(i);
	}
	for(i = 0; i < 8; i++)
	{
		if(rdata[i] != wdata[i])
		{
			printfS("error, rdata[%d]=0x%x, wdata[%d]=0x%x\n", i, rdata[i], i, wdata[i]);
			result = 1;
		}
	}

	printfS("read(16bit) test \n");
	buff16 = (UINT16 *)rdata;
	for(i = 0; i < 16; i++)
	{
		buff16[i] = MEM0_PORT16(2 * i);
	}

//	printfB8((UINT8 *)rdata,32);
	for(i = 0; i < 8; i++)
	{
		if(rdata[i] != wdata[i])
		{
			printfS("error, rdata[%d]=0x%x, wdata[%d]=0x%x\n", i, rdata[i], i, wdata[i]);
			result = 2;
		}
	}

	printfS("read(32bit) test\n");
	buff32 = (UINT32 *)rdata;
	for(i = 0; i < 8; i++)
	{
		buff32[i] = MEM0_PORT32(4 * i);
	}
	for(i = 0; i < 8; i++)
	{
		if(rdata[i] != wdata[i])
		{
			printfS("error, rdata[%d]=0x%x, wdata[%d]=0x%x\n", i, rdata[i], i, wdata[i]);
			result = 3;
		}
	}

//16bit write,8bit 16bit 32bit read
	printfS("write(16bit) test\n");
	buff16 = (UINT16 *)wdata;
	for(i = 0; i < 16; i++)
	{
		MEM0_PORT16(2 * i) = buff16[i];
	}
	printfS("read(8bit) test\n");
	buff8 = (UINT8 *)rdata;
	for(i = 0; i < 32; i++)
	{
		buff8[i] = MEM0_PORT8(i);
	}
	for(i = 0; i < 8; i++)
	{
		if(rdata[i] != wdata[i])
		{
			printfS("error, rdata[%d]=0x%x, wdata[%d]=0x%x\n", i, rdata[i], i, wdata[i]);
			result = 4;
		}
	}

	printfS("read(16bit) test \n");
	buff16 = (UINT16 *)rdata;
	for(i = 0; i < 16; i++)
	{
		buff16[i] = MEM0_PORT16(2 * i);
	}

//	printfB8((UINT8 *)rdata,32);
	for(i = 0; i < 8; i++)
	{
		if(rdata[i] != wdata[i])
		{
			printfS("error, rdata[%d]=0x%x, wdata[%d]=0x%x\n", i, rdata[i], i, wdata[i]);
			result = 5;
		}
	}

	printfS("read(32bit) test\n");
	buff32 = (UINT32 *)rdata;
	for(i = 0; i < 8; i++)
	{
		buff32[i] = MEM0_PORT32(4 * i);
	}
	for(i = 0; i < 8; i++)
	{
		if(rdata[i] != wdata[i])
		{
			printfS("error, rdata[%d]=0x%x, wdata[%d]=0x%x\n", i, rdata[i], i, wdata[i]);
			result = 6;
		}
	}

	if(result) 	printfS("test fail\n");
	else		printfS("test pass\n");
}


void mim_test(void)
{
	mem_test();
}


