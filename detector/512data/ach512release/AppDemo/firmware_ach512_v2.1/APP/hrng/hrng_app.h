/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : hrng_app.h
 * Description : hrng application header file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#ifndef __HRNG_APP_H__
#define __HRNG_APP_H__

#include  "common.h"
#include  "hrng.h"

//cmd_code definition
//bits[7:0] means operation.
#define CMD_HRNG_READ_REG          0x01
#define CMD_HRNG_GET_HRNGDATA      0x02

#define HRNG_SUCCEED               0x00
#define HRNG_FAILED                0xFF

#define HRNG_MAX_DATA_NUM          4096

typedef struct _crypt_hrng_t
{
	UINT8  cmd;
	UINT32 cryptInLenInByte;
	UINT32 cryptOutLenInByte;
	UINT8  *cryptInBuf;
	UINT8  *cryptOutBuf;
} CRYPT_HRNG;

UINT8 IP_HRNG_event(CRYPT_HRNG *crypt_hrng);

#endif
