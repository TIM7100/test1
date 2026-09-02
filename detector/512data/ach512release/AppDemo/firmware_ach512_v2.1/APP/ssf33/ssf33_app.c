/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : ssf33_app.c
 * Description : ssf33 application demo
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#include  "ssf33_app.h"

UINT8 IP_SSF33_event(CRYPT_SSF33 *crypt_ssf33)
{
	UINT8 work_mode;
	UINT8 crypt_mode;
	UINT8 key_mode;

	if((crypt_ssf33->cmd & CMD_SSF33_OPMODE_MASK) == CMD_SSF33_ENC)
	{
		work_mode = SSF33_ENCRYPT;
	}
	else
	{
		work_mode = SSF33_DECRYPT;
	}

	switch(crypt_ssf33->cmd & CMD_SSF33_MODE_MASK)
	{
		case CMD_SSF33_CBC_INNERPARA:
		case CMD_SSF33_CBC_OUTERPARA:
			crypt_mode = SSF33_CBC_MODE;
			crypt_ssf33->keyLenInWord -= 4;
			crypt_ssf33->iv = crypt_ssf33->keyBuf + crypt_ssf33->keyLenInWord;
			break;
		case CMD_SSF33_ECB_INNERPARA:
		case CMD_SSF33_ECB_OUTERPARA:
			crypt_mode = SSF33_ECB_MODE;
			crypt_ssf33->iv = NULL;
			break;
		default:
			return SSF33_FAILED;
	}

	switch(crypt_ssf33->keyLenInWord / 4)
	{
		case 1:
			key_mode = SSF33_INTER;
			break;
		case 2:
			key_mode = SSF33_EXTER;
			break;
		default:
			return SSF33_FAILED;
	}

	ssf33_set_key(key_mode, crypt_ssf33->keyBuf+4, crypt_ssf33->keyBuf, SSF33_SWAP_ENABLE);	
	if(SSF33_PASS != ssf33_crypt_data(crypt_ssf33->cryptInBuf, crypt_ssf33->cryptOutBuf, crypt_ssf33->cryptInLenInWord/4, work_mode, crypt_mode, crypt_ssf33->iv, SSF33_NORMAL_MODE))
	{
		return SSF33_FAILED;
	}

	crypt_ssf33->cryptOutLenInWord = crypt_ssf33->cryptInLenInWord;

	return SSF33_SUCCEED;
}
