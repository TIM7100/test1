/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : aes_app.c
 * Description : aes application demo
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#include  "aes_app.h"

UINT8 IP_AES_event(CRYPT_AES *crypt_aes)
{
	UINT8 work_mode;
	UINT8 crypt_mode;
	UINT8 key_mode;

	if((crypt_aes->cmd & CMD_AES_OPMODE_MASK) == CMD_AES_ENC)
	{
		work_mode = AES_ENCRYPTION;
	}
	else
	{
		work_mode = AES_DECRYPTION;
	}

	switch(crypt_aes->cmd & CMD_AES_MODE_MASK)
	{
		case CMD_AES_CBC_KEY128:
		case CMD_AES_CBC_KEY192:
		case CMD_AES_CBC_KEY256:
			crypt_mode = AES_CBC_MODE;
			crypt_aes->keyLenInWord /= 2;
			crypt_aes->iv = crypt_aes->keyBuf + crypt_aes->keyLenInWord;
			break;
		case CMD_AES_ECB_KEY128:
		case CMD_AES_ECB_KEY192:
		case CMD_AES_ECB_KEY256:
			crypt_mode = AES_ECB_MODE;
			crypt_aes->iv = NULL;
			break;
		default:
			return AES_FAILED;
	}
	
	switch(crypt_aes->keyLenInWord / 2)
	{
		case 2:
			key_mode = AES_KEY_128;
			break;
		case 3:
			key_mode = AES_KEY_192;
			break;
		case 4:
			key_mode = AES_KEY_256;
			break;
		default:
			return AES_FAILED;
	}

	aes_set_key(crypt_aes->keyBuf, key_mode, AES_SWAP_ENABLE);	
	if(AES_PASS != aes_crypt(crypt_aes->cryptInBuf, crypt_aes->cryptOutBuf, crypt_aes->cryptInLenInWord/4, work_mode, crypt_mode, crypt_aes->iv, AES_NORMAL_MODE))
	{
		return AES_FAILED;
	}
		
	crypt_aes->cryptOutLenInWord = crypt_aes->cryptInLenInWord;

	return AES_SUCCEED;
}
