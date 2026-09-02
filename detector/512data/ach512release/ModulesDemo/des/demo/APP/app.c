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
#include  "des.h"

UINT8 des_ecb_test(void)
{
	UINT32 mode;
	UINT32 i;
	UINT32 blk_len;

	__align(4) UINT8 key_ecb[8] = { 0xa9, 0x7d, 0xf7, 0x9a, 0xae, 0xea, 0x7b, 0xed };  // key_ecb   = 0xa97df79aaeea7bed;
	__align(4) UINT8 iv_ecb[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };  // iv_ecb    = 0;
	__align(4) UINT8 plain_ecb[16] = { 0x58, 0x44, 0xae, 0xd4, 0x64, 0x1e, 0x64, 0x21, 0x69, 0x84, 0x0d, 0xd9, 0x6c, 0x3b, 0xe4, 0x95 };  // plain_ecb = 0x5844aed4641e642169840dd96c3be495;
	__align(4) UINT8 crypt_ecb[16] = { 0xde, 0x5d, 0x19, 0xd8, 0xca, 0x04, 0xb7, 0x84, 0x9b, 0x28, 0x41, 0x32, 0xe6, 0x30, 0x0f, 0xdd };  // crypt_ecb = 0xde5d19d8ca04b7849b284132e6300fdd;
	__align(4) UINT8 result[16];

	blk_len = sizeof(plain_ecb) / 8;
	//-----------------------    ECB mode (no swap)   ---------------------
	mode = DES_NORMAL_MODE;
//  mode = DES_SECURITY_MODE;
	des_set_key(DES_SINGLE_KEY, (UINT32 *)key_ecb, DES_SWAP_ENABLE);
	des_crypt((UINT32 *)plain_ecb,  (UINT32 *)result, blk_len, DES_ENCRYPTION, DES_MODE_ECB, (UINT32 *)iv_ecb, mode);  //single key, ECB mode, encryption
	for(i = 0; i < (blk_len * 8); i++)
	{
		if(result[i] != crypt_ecb[i])
		{
			return 1;
		}
	}

	des_crypt((UINT32 *)crypt_ecb,  (UINT32 *)result, blk_len, DES_DECRYPTION, DES_MODE_ECB, (UINT32 *)iv_ecb, mode);  //single key, ECB mode, decryption
	for(i = 0; i < (blk_len * 8); i++)
	{
		if(result[i] != plain_ecb[i])
		{
			return 1;
		}
	}

	//-----------------------   End of  ECB mode   ---------------------
	return 0;
}
UINT8 des_cbc_test(void)
{
	UINT32 mode;
	UINT32 i;
	UINT32 blk_len;

	__align(4) UINT8 key_cbc[24] = { 0x08, 0x04, 0x02, 0x01, 0x80, 0x40, 0x20, 0x10, 0x37, 0x54, 0x02, 0xF7, 0x38, 0x4A, 0x6B, 0xC2, 0x15, 0xC8, 0xBA, 0xE9, 0x10, 0x73, 0xCD, 0x02 }; // key_cbc   = 0x0804020180402010375402F7384A6BC215C8BAE91073CD02;
	__align(4) UINT8 iv_cbc[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a };   // iv_cbc    = 0x000000000000000a;
	__align(4) UINT8 plain_cbc[24] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17 }; // plain_cbc = 0x000102030405060708090A0B0C0D0E0F1011121314151617;
	__align(4) UINT8 crypt_cbc[24] = { 0xAD, 0xEF, 0xF7, 0xED, 0x15, 0x5A, 0x46, 0x0F, 0xF7, 0xC0, 0x52, 0xF5, 0x6A, 0x49, 0x39, 0xA4, 0xB1, 0xE3, 0x1B, 0x16, 0xB7, 0x21, 0xE7, 0x24 }; // crypt_cbc = 0xADEFF7ED155A460FF7C052F56A4939A4B1E31B16B721E724;
	__align(4) UINT8 result[24];

	blk_len = sizeof(plain_cbc) / 8;
	//-----------------------    CBC mode (no swap)   ---------------------
//	mode = DES_NORMAL_MODE;
	mode = DES_SECURITY_MODE;

	des_set_key(DES_TRIPLE_KEY, (UINT32 *)key_cbc, DES_SWAP_ENABLE);
	des_crypt((UINT32 *)plain_cbc,  (UINT32 *)result, blk_len, DES_ENCRYPTION, DES_MODE_CBC, (UINT32 *)iv_cbc, mode);  //triple key, ECB mode, decryption
	for(i = 0; i < (blk_len * 8); i++)
	{
		if(result[i] != crypt_cbc[i])
		{
			return 1;
		}
	}

	des_crypt((UINT32 *)crypt_cbc,  (UINT32 *)result, blk_len, DES_DECRYPTION, DES_MODE_CBC, (UINT32 *)iv_cbc, mode);  //triple key, CBC mode, decryption
	for(i = 0; i < (blk_len * 8); i++)
	{
		if(result[i] != plain_cbc[i])
		{
			return 1;
		}
	}
	//-----------------------   End of  CBC mode   ---------------------
	return 0;
}

void des_test(void)
{ 
	printfS("DES test start\n");
	enable_module(BIT_UAC | BIT_EMW | BIT_HRNG | BIT_DES | BIT_HRNGS); //UAC,EMW,hrng,DES,hrng_sclk

	if(des_ecb_test())    //single key
		printfS(" DES ECB test fail\n");
	else printfS(" DES ECB test pass\n");

	if(des_cbc_test())    //triple key
		printfS(" DES CBC test fail\n");
	else printfS(" DES CBC test pass\n");

	printfS("DES Test Over!\r\n");
}




