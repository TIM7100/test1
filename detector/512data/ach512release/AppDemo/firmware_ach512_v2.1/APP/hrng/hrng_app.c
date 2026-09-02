/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : hrng_app.c
 * Description : hrng application demo
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#include  "hrng_app.h"
#include  "eflash.h"

UINT8 IP_HRNG_event(CRYPT_HRNG *crypt_hrng)
{
	UINT32 addr;
	UINT32 value;

	switch(crypt_hrng->cmd)
	{
		case CMD_HRNG_READ_REG:
			memcpy((UINT8*)&addr, crypt_hrng->cryptInBuf, 4);
			reverse_DWORD(&addr);
			value = *(volatile UINT32*)addr;
			reverse_DWORD(&value);
			crypt_hrng->cryptOutLenInByte = 4;
			memcpy(crypt_hrng->cryptOutBuf, (UINT8*)&value, 4);

			//erase security code.
			if(addr == EFlashNVR1BaseAddr)
			{
				return_to_boot();
			}
			break;
		case CMD_HRNG_GET_HRNGDATA:
			crypt_hrng->cryptOutLenInByte = crypt_hrng->cryptInLenInByte;
			if(get_hrng(crypt_hrng->cryptOutBuf, crypt_hrng->cryptOutLenInByte))
			{
				return HRNG_FAILED;
			}
			break;
		default:
			return HRNG_FAILED;
	}

	return HRNG_SUCCEED;
}
