/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : gpio.h
 * Description : gpio driver header file
 * Author(s)   : Eric
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#ifndef __GPIO_H__
#define __GPIO_H__

#include  "common.h"

#define KEY_DATA (0x00F00000)
#define Key_Enter   ((u32)KEY_DATA & (~0x0100000))
#define Key_Esc     ((u32)KEY_DATA & (~0x0200000))
#define Key_Up      ((u32)KEY_DATA & (~0x0400000))
#define Key_Down    ((u32)KEY_DATA & (~0x0800000))

void KeyInit(void);
u32 wait_Key(void);


#endif

