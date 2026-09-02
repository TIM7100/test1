/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : app.c
 * Description : app source file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#include  "app.h"
#include  "aes.h"

UINT32 aes_ecb_test(void)  //ECB Test
{
	UINT32 mode; 
	UINT32 i;
	UINT32 blk_len;

	__align(4) UINT8 key_ecb[16] = { 0xe0, 0x4a, 0xd1, 0xa4, 0x65, 0xfe, 0x70, 0x6b, 0x80, 0xcf, 0x29, 0x60, 0x5d, 0xa2, 0x63, 0x6a };  // key_ecb    = 0xe04ad1a4 65fe706b 80cf2960 5da2636a
	__align(4) UINT8 iv_ecb[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };  // iv_ecb     = 0x0;
	__align(4) UINT8 plain_ecb[16] = { 0xc4, 0x28, 0x21, 0xe1, 0x3a, 0xc9, 0xb6, 0x22, 0x72, 0x30, 0x4a, 0xd5, 0xe5, 0xf5, 0x93, 0x7d };  // plain_ecb  = 0xc42821e1 3ac9b622 72304ad5 e5f5937d
	__align(4) UINT8 cipher_ecb[16] = { 0x1f, 0x4f, 0x49, 0xe4, 0xfb, 0x96, 0xcc, 0xa5, 0xa3, 0x09, 0x03, 0x2f, 0xb1, 0xa4, 0x6d, 0xde };  // cipher_ecb = 0x1f4f49e4 fb96cca5 a309032f b1a46dde
	__align(4) UINT8 result[16];

	blk_len = sizeof(plain_ecb) / 16;
	//-----------------------    ECB mode (no swap)   ---------------------
//	  mode = AES_NORMAL_MODE;
	mode = AES_SECURITY_MODE;

	aes_set_key((UINT32 *)key_ecb, AES_KEY_128, AES_SWAP_ENABLE);     //128bit key
	aes_crypt((UINT32 *)plain_ecb, (UINT32 *)result, blk_len, AES_ENCRYPTION, AES_ECB_MODE, (UINT32 *)iv_ecb, mode);

	for(i = 0; i < (blk_len * 16); i++)
	{
		if(result[i] != cipher_ecb[i])
		{
			return 1;
		}
	}

	aes_crypt((UINT32 *)cipher_ecb, (UINT32 *)result, blk_len, AES_DECRYPTION, AES_ECB_MODE, (UINT32 *)iv_ecb, mode);
	for(i = 0; i < (blk_len * 16); i++)
	{
		if(result[i] != plain_ecb[i])
		{
			return 1;
		}
	}

	//-----------------------   End of  ECB mode   ---------------------
	return 0;
}

UINT32 aes_cbc_test(void)   //CBC Test
{
	UINT32 mode;
	UINT32 i;
	UINT32 blk_len;

	__align(4) UINT8 key_cbc[16] = { 0xe0, 0x4a, 0xd1, 0xa4, 0x65, 0xfe, 0x70, 0x6b, 0x80, 0xcf, 0x29, 0x60, 0x5d, 0xa2, 0x63, 0x6a };  //key_cbc    = 0xe04ad1a4 65fe706b 80cf2960 5da2636a
	__align(4) UINT8 iv_cbc[16] = { 0xc4, 0x28, 0x21, 0xe1, 0x3a, 0xc9, 0xb6, 0x22, 0x72, 0x30, 0x4a, 0xd5, 0xe5, 0xf5, 0x93, 0x7d };  //iv_cbc     = 0xc42821e1 3ac9b622 72304ad5 e5f5937d
	__align(4) UINT8 plain_cbc[16] = { 0xf3, 0x8d, 0x8a, 0xdf, 0x36, 0x7b, 0x0c, 0x53, 0x1d, 0x27, 0x3e, 0xb7, 0x60, 0x63, 0x20, 0x39 };  //plain_cbc  = 0xf38d8adf 367b0c53 1d273eb7 60632039
	__align(4) UINT8 cipher_cbc[16] = { 0x09, 0x9e, 0xd1, 0x43, 0x4a, 0xf1, 0x21, 0x94, 0xc2, 0xcb, 0x03, 0x51, 0x07, 0x5e, 0xa9, 0x70 };  //cipher_cbc = 0x099ed143 4af12194 c2cb0351 075ea970
	__align(4) UINT8 result[16];

	blk_len = sizeof(plain_cbc) / 16;
	//-----------------------    CBC mode (no swap)   ---------------------
	mode = AES_NORMAL_MODE;
//  mode = AES_SECURITY_MODE;

	aes_set_key((UINT32 *)key_cbc, AES_KEY_128, AES_SWAP_ENABLE);     //128bit key
	aes_crypt((UINT32 *)plain_cbc, (UINT32 *)result, blk_len, AES_ENCRYPTION, AES_CBC_MODE, (UINT32 *)iv_cbc, mode);

	for(i = 0; i < (blk_len * 16); i++)
	{
		if(result[i] != cipher_cbc[i])
		{
			return 1;
		}
	}

	aes_crypt((UINT32 *)cipher_cbc, (UINT32 *)result, blk_len, AES_DECRYPTION, AES_CBC_MODE, (UINT32 *)iv_cbc, mode);
	for(i = 0; i < (blk_len * 16); i++)
	{
		if(result[i] != plain_cbc[i])
		{
			return 1;
		}
	}

	//-----------------------   End of  CBC mode   ---------------------
	return 0;
}

void aes_test(void)
{
	printfS("AES test start\n");

	enable_module(BIT_UAC | BIT_EMW | BIT_HRNG | BIT_AES | BIT_HRNGS); //AES,hrng_sclk,UAC,EMW,hrng

	if(aes_ecb_test()) printfS(" AES ECB Test Fail!\r\n");
	else printfS(" AES ECB Test Pass!\r\n");

	if(aes_cbc_test()) printfS(" AES CBC Test Fail!\r\n");
	else printfS(" AES CBC Test Pass!\r\n");

	printfS("AES Test Over!\r\n");

}



