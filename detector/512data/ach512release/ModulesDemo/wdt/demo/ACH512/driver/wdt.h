/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : wdt.h
 * Description : wdt driver header file
 * Author(s)   : Eric
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#ifndef __WDT_H__
#define __WDT_H__
#include "common.h"

#define WDT_ENABLE          (1<<7)
#define WDT_ACTION_INT   	(1<<6)
#define WDT_INT_EN			(1<<4)

#define WDT_DIVIDER_1       0x00
#define WDT_DIVIDER_2       0x01
#define WDT_DIVIDER_4       0x02
#define WDT_DIVIDER_8       0x03
#define WDT_DIVIDER_16      0x04
#define WDT_DIVIDER_32      0x05
#define WDT_DIVIDER_64      0x06
#define WDT_DIVIDER_128     0x07

extern volatile UINT8 flag_wdt_int;

/************************************************************************
 * function   : wdt_init
 * Description: wdt initial
 * input : none
 * return: none
 ************************************************************************/
void wdt_init(void);

/************************************************************************
 * function   : wdt_start
 * Description: watch dog start
 * input : none
 * return: none
 ************************************************************************/
void wdt_start(void);

/************************************************************************
 * function   : wdt_stop
 * Description: watch dog stop
 * input : none
 * return: none
 ************************************************************************/
void wdt_stop(void);

/************************************************************************
 * function   : wdt_feed
 * Description: watch dog feed
 * input : none
 * return: none
 ************************************************************************/
void wdt_feed(void);
#endif


