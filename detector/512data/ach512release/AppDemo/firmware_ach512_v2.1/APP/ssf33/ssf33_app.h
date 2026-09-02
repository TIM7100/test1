/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : ssf33_app.h
 * Description : ssf33 application header file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#ifndef __SSF33_APP_H__
#define __SSF33_APP_H__

#include  "common.h"
#include  "ssf33.h"

//cmd_code definition
//bits[7:4] means operation, bits[3:0] means mode.
#define CMD_SSF33_ENC              0x10
#define CMD_SSF33_DEC              0x20
#define CMD_SSF33_OPMODE_MASK      0xF0

#define CMD_SSF33_ECB_INNERPARA    0x01
#define CMD_SSF33_ECB_OUTERPARA    0x02
#define CMD_SSF33_CBC_INNERPARA    0x03
#define CMD_SSF33_CBC_OUTERPARA    0x04
#define CMD_SSF33_MODE_MASK        0x0F

#define SSF33_SUCCEED              0x00
#define SSF33_FAILED               0xFF

typedef struct _crypt_ssf33_t
{
	UINT8  cmd;
	UINT16 keyLenInWord;
	UINT32 cryptInLenInWord;
	UINT32 cryptOutLenInWord;
	UINT32 *keyBuf;
	UINT32 *iv;
	UINT32 *cryptInBuf;
	UINT32 *cryptOutBuf;
} CRYPT_SSF33;

UINT8 IP_SSF33_event(CRYPT_SSF33 *crypt_ssf33);

#endif
