/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : des_app.c
 * Description : des application demo
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#include  "des_app.h"

UINT8 IP_DES_event(CRYPT_DES *crypt_des)
{
	UINT8 work_mode;
	UINT8 crypt_mode;
	UINT8 key_mode;

	if((crypt_des->cmd & CMD_DES_OPMODE_MASK) == CMD_DES_ENC)
	{
		work_mode = DES_ENCRYPTION;
	}
	else
	{
		work_mode = DES_DECRYPTION;
	}

	switch(crypt_des->cmd & CMD_DES_MODE_MASK)
	{
		case CMD_DES_CBC_KEYONE:
		case CMD_DES_CBC_KEYTWO:
		case CMD_DES_CBC_KEYTHREE:
			crypt_mode = DES_MODE_CBC;
			crypt_des->keyLenInWord -= 2;
			crypt_des->iv = crypt_des->keyBuf + crypt_des->keyLenInWord;
			break;
		case CMD_DES_ECB_KEYONE:
		case CMD_DES_ECB_KEYTWO:
		case CMD_DES_ECB_KEYTHREE:
			crypt_mode = DES_MODE_ECB;
			crypt_des->iv = NULL;
			break;
		default:
			return DES_FAILED;
	}

	switch(crypt_des->keyLenInWord / 2)
	{
		case 1:
			key_mode = DES_SINGLE_KEY;
			break;
		case 2:
			key_mode = DES_DOUBLE_KEY;
			break;
		case 3:
			key_mode = DES_TRIPLE_KEY;
			break;
		default:
			return DES_FAILED;
	}

	des_set_key(key_mode, crypt_des->keyBuf, DES_SWAP_ENABLE);	
	if(DES_PASS != des_crypt(crypt_des->cryptInBuf, crypt_des->cryptOutBuf, crypt_des->cryptInLenInWord/2, work_mode, crypt_mode, crypt_des->iv, DES_NORMAL_MODE))
	{
		return DES_FAILED;
	}
		
	crypt_des->cryptOutLenInWord = crypt_des->cryptInLenInWord;

	return DES_SUCCEED;
}
