/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : aes_app.h
 * Description : aes application header file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#ifndef __AES_APP_H__
#define __AES_APP_H__

#include  "common.h"
#include  "aes.h"

//cmd_code definition
//bits[7:4] means operation, bits[3:0] means mode.
#define CMD_AES_ENC                0x10
#define CMD_AES_DEC                0x20
#define CMD_AES_OPMODE_MASK        0xF0

#define CMD_AES_ECB_KEY128         0x01
#define CMD_AES_ECB_KEY192         0x02
#define CMD_AES_ECB_KEY256         0x03
#define CMD_AES_CBC_KEY128         0x04
#define CMD_AES_CBC_KEY192         0x05
#define CMD_AES_CBC_KEY256         0x06
#define CMD_AES_MODE_MASK          0x0F

#define AES_SUCCEED                0x00
#define AES_FAILED                 0xFF

typedef struct _crypt_aes_t
{
	UINT8  cmd;
	UINT16 keyLenInWord;
	UINT32 cryptInLenInWord;
	UINT32 cryptOutLenInWord;
	UINT32 *keyBuf;
	UINT32 *iv;
	UINT32 *cryptInBuf;
	UINT32 *cryptOutBuf;
} CRYPT_AES;

UINT8 IP_AES_event(CRYPT_AES *crypt_aes);

#endif
