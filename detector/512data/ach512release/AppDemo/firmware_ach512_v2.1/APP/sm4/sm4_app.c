/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : sm4_app.c
 * Description : sm4 application demo
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#include  "sm4_app.h"

UINT8 IP_SM4_event(CRYPT_SM4 *crypt_sm4)
{
	UINT8 work_mode;
	UINT8 crypt_mode;

	if((crypt_sm4->cmd & CMD_SM4_OPMODE_MASK) == CMD_SM4_ENC)
	{
		work_mode = SM4_ENCRYPTION;
	}
	else
	{
		work_mode = SM4_DECRYPTION;
	}

	switch(crypt_sm4->cmd & CMD_SM4_MODE_MASK)
	{
		case CMD_SM4_CBC:
			crypt_mode = SM4_CBC_MODE;
			crypt_sm4->keyLenInWord -= 4;
			crypt_sm4->iv = crypt_sm4->keyBuf + crypt_sm4->keyLenInWord;
			break;
		case CMD_SM4_ECB:
			crypt_mode = SM4_ECB_MODE;
			crypt_sm4->iv = NULL;
			break;
		default:
			return SM4_FAILED;
	}

	sm4_set_key(crypt_sm4->keyBuf, SM4_SWAP_ENABLE);	
	if(SM4_PASS != sm4_crypt(crypt_sm4->cryptInBuf, crypt_sm4->cryptOutBuf, crypt_sm4->cryptInLenInWord/4, work_mode, crypt_mode, crypt_sm4->iv, SM4_NORMAL_MODE))
	{
		return SM4_FAILED;
	}
		
	crypt_sm4->cryptOutLenInWord = crypt_sm4->cryptInLenInWord;

	return SM4_SUCCEED;
}
