/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : app.h
 * Description : application example header file
 * Author(s)   : Eric
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#ifndef __APP_H__
#define __APP_H__
#include "common.h"

extern UINT32 g_data_buf[];
#define DATABUF             ((UINT8 *)g_data_buf)

void sm3_test(void);
void sha256_test(void);
void sha1_test(void);
void sha384_test(void);
void sha512_test(void);

#endif

