/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : app.c
 * Description : app source file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#include  "common.h"
#include  "app.h"
#include  "hrng.h"

UINT32 g_data_buf[1024];

void hrng_test()
{
	UINT32 i;
	UINT32 j;

	enable_module(BIT_UAC | BIT_HRNG | BIT_HRNGS);

	printfS("---------HRNG test-------\n");

	for(i = 1; i < 64; i++)
	{
		printfS("get random number: %d\n", i);
		if(get_hrng(DATABUF, i))
		{
			printfS("random number is error\n");
			return;
		}
		for(j = 0; j < i; j++)
		{
			printfS("%x", DATABUF[j]);
		}
		printfS("\n");
	}
	printfS("-------HRNG test end-------\n");

}



