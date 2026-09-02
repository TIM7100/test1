/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : sm1_app.h
 * Description : sm1 application header file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#ifndef __SM1_APP_H__
#define __SM1_APP_H__

#include  "common.h"
#include  "sm1.h"

//cmd_code definition
//bits[7:4] means operation, bits[3:0] means mode.
#define CMD_SM1_ENC                0x10
#define CMD_SM1_DEC                0x20
#define CMD_SM1_OPMODE_MASK        0xF0

#define CMD_SM1_ECB_INNERPARA      0x01
#define CMD_SM1_ECB_OUTERPARA      0x02
#define CMD_SM1_CBC_INNERPARA      0x03
#define CMD_SM1_CBC_OUTERPARA      0x04
#define CMD_SM1_MODE_MASK          0x0F

#define SM1_SUCCEED                0x00
#define SM1_FAILED                 0xFF

typedef struct _crypt_sm1_t
{
	UINT8  cmd;
	UINT16 keyLenInWord;
	UINT32 cryptInLenInWord;
	UINT32 cryptOutLenInWord;
	UINT32 *keyBuf;
	UINT32 *iv;
	UINT32 *cryptInBuf;
	UINT32 *cryptOutBuf;
} CRYPT_SM1;

UINT8 IP_SM1_event(CRYPT_SM1 *crypt_sm1);

#endif
