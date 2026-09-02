/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : sm3_app.h
 * Description : sm3 application header file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#ifndef __SM3_APP_H__
#define __SM3_APP_H__

#include  "common.h"
#include  "sm3.h"

//cmd_code definition
//bits[7:0] means operation.
#define CMD_SM3_HASH               0x01
#define CMD_SM3_HASHFILE           0x02
#define CMD_SM3_HASHFORSM2         0x03

#define SM3_SUCCEED                0x00
#define SM3_FAILED                 0xFF

#define SM3_DIGISTLEN              256
#define SM3_DIGISTLEN_INBYTE       (SM3_DIGISTLEN/8)
#define SM3_DIGISTLEN_INWORD       (SM3_DIGISTLEN/32)
#define SM2_PUBKEYLEN_INBYTE       (256/8)
#define SM2_PUBKEYLEN_INWORD       (256/32)

#define SM3_MAX_MSG_LEN            4096

typedef struct _crypt_sm3_t
{
	UINT8  cmd;
	UINT32 idLenInByte;
	UINT32 msgLenInByte;
	UINT32 cryptOutLenInByte;
	UINT8  *keyBuf;                //must be 32bits-aligned
	UINT8  *cryptInBuf;
	UINT8  *cryptOutBuf;
} CRYPT_SM3;

UINT8 IP_SM3_event(CRYPT_SM3 *crypt_sm3);

#endif
