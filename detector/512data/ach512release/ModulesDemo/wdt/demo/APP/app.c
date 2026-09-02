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
#include  "wdt.h"

void wdt_test(void)
{
    wdt_init();
    wdt_start();
    while(1)
	{
		//wdt_feed();
    }
}

