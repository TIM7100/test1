/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : sm4_app.h
 * Description : sm4 application header file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#ifndef __SM4_APP_H__
#define __SM4_APP_H__

#include  "common.h"
#include  "sm4.h"

//cmd_code definition
//bits[7:4] means operation, bits[3:0] means mode.
#define CMD_SM4_ENC                0x10
#define CMD_SM4_DEC                0x20
#define CMD_SM4_OPMODE_MASK        0xF0

#define CMD_SM4_ECB                0x01
#define CMD_SM4_CBC                0x02
#define CMD_SM4_MODE_MASK          0x0F

#define SM4_SUCCEED                0x00
#define SM4_FAILED                 0xFF

typedef struct _crypt_sm4_t
{
	UINT8  cmd;
	UINT16 keyLenInWord;
	UINT32 cryptInLenInWord;
	UINT32 cryptOutLenInWord;
	UINT32 *keyBuf;
	UINT32 *iv;
	UINT32 *cryptInBuf;
	UINT32 *cryptOutBuf;
} CRYPT_SM4;

UINT8 IP_SM4_event(CRYPT_SM4 *crypt_sm4);

#endif
