/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : sm2_app.h
 * Description : sm2 application header file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#ifndef __SM2_APP_H__
#define __SM2_APP_H__

#include  "common.h"
#include  "sm2.h"

//cmd_code definition
//bits[7:0] means operation.
#define CMD_SM2_KEYGEN             0x01
#define CMD_SM2_SIGN               0x02
#define CMD_SM2_VERIFY             0x03
#define CMD_SM2_ENCRYPT            0x04
#define CMD_SM2_DECRYPT            0x05

#define SM2_SUCCEED                0x00
#define SM2_FAILED                 0xFF

#define SM2_CURVELEN               256
#define SM2_CURVELEN_INBYTE        (SM2_CURVELEN/8)
#define SM2_CURVELEN_INWORD        (SM2_CURVELEN/32)

typedef struct _crypt_sm2_t
{
	UINT8  cmd;
	UINT16 keyLenInByte;
	UINT32 idLenInByte;
	UINT32 msgLenInByte;
	UINT32 cryptOutLenInByte;
	UINT8  *keyBuf;
	UINT8  *cryptInBuf;
	UINT8  *cryptOutBuf;
} CRYPT_SM2;

void sm2_curve_init(ECC_G_STR *p_ecc_para);
void sm2_get_digist(ECC_G_STR *p_ecc_para, UINT8 *id, UINT16 id_len, UINT8 *msg, UINT32 msg_len, UINT8 *PubX, UINT8 *PubY, UINT8 *pDigest);
UINT8 IP_SM2_event(CRYPT_SM2 *crypt_sm2);

#endif
