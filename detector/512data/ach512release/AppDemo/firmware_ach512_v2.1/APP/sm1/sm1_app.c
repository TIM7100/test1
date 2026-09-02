/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : sm1_app.c
 * Description : sm1 application demo
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#include  "sm1_app.h"

UINT8 IP_SM1_event(CRYPT_SM1 *crypt_sm1)
{
	UINT8 work_mode;
	UINT8 crypt_mode;
	UINT8 key_mode;

	if((crypt_sm1->cmd & CMD_SM1_OPMODE_MASK) == CMD_SM1_ENC)
	{
		work_mode = SM1_ENCRYPTION;
	}
	else
	{
		work_mode = SM1_DECRYPTION;
	}

	switch(crypt_sm1->cmd & CMD_SM1_MODE_MASK)
	{
		case CMD_SM1_CBC_INNERPARA:
		case CMD_SM1_CBC_OUTERPARA:
			crypt_mode = SM1_CBC_MODE;
			crypt_sm1->keyLenInWord -= 4;
			crypt_sm1->iv = crypt_sm1->keyBuf + crypt_sm1->keyLenInWord;
			break;
		case CMD_SM1_ECB_INNERPARA:
		case CMD_SM1_ECB_OUTERPARA:
			crypt_mode = SM1_ECB_MODE;
			crypt_sm1->iv = NULL;
			break;
		default:
			return SM1_FAILED;
	}

	switch(crypt_sm1->cmd & CMD_SM1_MODE_MASK)
	{
		case CMD_SM1_ECB_INNERPARA:
		case CMD_SM1_CBC_INNERPARA:
			key_mode = SM1_INTERPRAR;
			break;
		case CMD_SM1_ECB_OUTERPARA:
		case CMD_SM1_CBC_OUTERPARA:
			key_mode = SM1_EXTERPRAR;
			break;
		default:
			return SM1_FAILED;
	}

	sm1_set_key(crypt_sm1->keyBuf, key_mode, SM1_SWAP_ENABLE);	
	if(SM1_PASS != sm1_crypt(crypt_sm1->cryptInBuf, crypt_sm1->cryptOutBuf, crypt_sm1->cryptInLenInWord/4, work_mode, crypt_mode, crypt_sm1->iv, SM1_NORMAL_MODE))
	{
		return SM1_FAILED;
	}
		
	crypt_sm1->cryptOutLenInWord = crypt_sm1->cryptInLenInWord;

	return SM1_SUCCEED;
}
