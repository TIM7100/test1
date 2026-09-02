/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : sm3_app.c
 * Description : sm3 application demo
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#include  "sm3_app.h"
#include  "sm2_app.h"

UINT8 IP_SM3_event(CRYPT_SM3 *crypt_sm3)
{
	ECC_G_STR ecc_glb_str;
	UINT8 *crypt_id;
	UINT8 *crypt_msg;
	UINT8 *pubKeyX, *pubKeyY;
	static SM3_CTX context;
	static UINT8 bIsFirstPacket = TRUE;

	switch(crypt_sm3->cmd)
	{
		case CMD_SM3_HASH:
			sm3_hash(crypt_sm3->cryptInBuf, crypt_sm3->msgLenInByte, crypt_sm3->cryptOutBuf);
			
			crypt_sm3->cryptOutLenInByte = SM3_DIGISTLEN_INBYTE;
			break;
		case CMD_SM3_HASHFILE:
			crypt_sm3->cryptOutLenInByte = 0;
			if(bIsFirstPacket)
			{
				bIsFirstPacket = FALSE;
				SM3_initial(&context);
			}
			SM3_update(&context, crypt_sm3->cryptInBuf, crypt_sm3->msgLenInByte);			
			if(crypt_sm3->msgLenInByte < SM3_MAX_MSG_LEN)
			{
				SM3_final(crypt_sm3->cryptOutBuf, &context);
				bIsFirstPacket = TRUE;
				crypt_sm3->cryptOutLenInByte = SM3_DIGISTLEN_INBYTE;
			}

			break;
		case CMD_SM3_HASHFORSM2:
			sm2_curve_init(&ecc_glb_str);

			pubKeyX = crypt_sm3->keyBuf;
			pubKeyY = pubKeyX + SM2_PUBKEYLEN_INBYTE;

			crypt_id = crypt_sm3->cryptInBuf;			   
			crypt_msg = crypt_id + crypt_sm3->idLenInByte;
			//generate digest
			sm2_get_digist(&ecc_glb_str, crypt_id, crypt_sm3->idLenInByte, crypt_msg, crypt_sm3->msgLenInByte, pubKeyX, pubKeyY, crypt_sm3->cryptOutBuf);
			
			crypt_sm3->cryptOutLenInByte = SM3_DIGISTLEN_INBYTE;
			break;
		default:
			return SM3_FAILED;
	}
	
	return SM3_SUCCEED;
}
