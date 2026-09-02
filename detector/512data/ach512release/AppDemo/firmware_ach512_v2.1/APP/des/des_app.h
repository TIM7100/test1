/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : des_app.h
 * Description : des application header file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#ifndef __DES_APP_H__
#define __DES_APP_H__

#include  "common.h"
#include  "des.h"

//cmd_code definition
//bits[7:4] means operation, bits[3:0] means mode.
#define CMD_DES_ENC                0x10
#define CMD_DES_DEC                0x20
#define CMD_DES_OPMODE_MASK        0xF0

#define CMD_DES_ECB_KEYONE         0x01
#define CMD_DES_ECB_KEYTWO         0x02
#define CMD_DES_ECB_KEYTHREE       0x03
#define CMD_DES_CBC_KEYONE         0x04
#define CMD_DES_CBC_KEYTWO         0x05
#define CMD_DES_CBC_KEYTHREE       0x06
#define CMD_DES_MODE_MASK          0x0F

#define DES_SUCCEED                0x00
#define DES_FAILED                 0xFF

typedef struct _crypt_des_t
{
	UINT8  cmd;
	UINT16 keyLenInWord;
	UINT32 cryptInLenInWord;
	UINT32 cryptOutLenInWord;
	UINT32 *keyBuf;
	UINT32 *iv;
	UINT32 *cryptInBuf;
	UINT32 *cryptOutBuf;
} CRYPT_DES;

UINT8 IP_DES_event(CRYPT_DES *crypt_des);

#endif
