/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : iso7816_app.h
 * Description : iso7816 application header file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#ifndef __ISO7816_APP_H__
#define __ISO7816_APP_H__

#include  "common.h"
#include  "iso7816ms.h"

//cmd_code definition
//bits[7:4] means operation, bits[3:0] means mode.
#define CMD_ISO7816_RESET          0x10
#define CMD_ISO7816_PPS            0x20
#define CMD_ISO7816_APDU           0x30
#define CMD_ISO7816_OPMODE_MASK    0xF0

#define CMD_ISO7816_PROTOCOL_T0    0x01
#define CMD_ISO7816_PROTOCOL_T1    0x02
#define CMD_ISO7816_PROTOCOL_MASK  0x0F 

#define ISO7816_SUCCEED            0x00
#define ISO7816_FAILED             0xFF

typedef struct _crypt_iso7816_t
{
	UINT8 cmd;
	UINT8 reqLen;
	UINT8 respLen;
	UINT8 *reqBuf;
	UINT8 *respBuf;
} CRYPT_ISO7816;

typedef struct _scard_apdu_t
{
	UINT8 CLA;
	UINT8 INS;
	UINT8 P1;
	UINT8 P2;
	UINT8 P3;
	UINT8 *data;
} SCARD_APDU;

UINT8 IP_ISO7816_event(CRYPT_ISO7816 *crypt_iso7816);

#endif
