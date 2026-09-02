/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : rsa_app.h
 * Description : rsa application header file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#ifndef __RSA_APP_H__
#define __RSA_APP_H__

#include  "common.h"
#include  "ecc.h"
#include  "rsa_keygen.h"

//cmd_code definition
//bits[7:4] means operation, bits[3:0] means mode.
#define CMD_RSA_ENC                0x10
#define CMD_RSA_DEC                0x20
#define CMD_RSA_KEYGEN             0x30
#define CMD_RSA_OPMODE_MASK        0xF0

#define CMD_RSA_1024B              0x01
#define CMD_RSA_1024B_CRT          0x02
#define CMD_RSA_2048B              0x03
#define CMD_RSA_2048B_CRT          0x04
#define CMD_RSA_MODE_MASK          0x0F

#define RSA_SUCCEED                0x00
#define RSA_FAILED                 0xFF

#define RSA_1024B_INBYTE           (1024/8)
#define RSA_2048B_INBYTE           (2048/8)
#define RSA_1024B_INWORD           (RSA_1024B_INBYTE/4)
#define RSA_2048B_INWORD           (RSA_2048B_INBYTE/4)

typedef struct _crypt_rsa_t
{
	UINT8  cmd;
	UINT16 keyLenInWord;
	UINT32 cryptInLenInWord;
	UINT32 cryptOutLenInWord;
	UINT32 *keyBuf;
	UINT32 *cryptInBuf;
	UINT32 *cryptOutBuf;
} CRYPT_RSA;

UINT8 IP_RSA_event(CRYPT_RSA *crypt_rsa);

#endif
