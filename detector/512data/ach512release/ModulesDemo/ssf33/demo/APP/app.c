/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : app.c
 * Description : application example source file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/

#include  "app.h"
#include  "ssf33.h"

UINT8 ssf33_ecb_test(void)
{
  UINT32 mode;
	UINT32 i;
	UINT32 blk_len;

	__align(4) UINT8 para_ecb[16]  = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; //para_ecb  = 0;(inter para)
	__align(4) UINT8 iv_ecb[16]    = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; //iv_ecb    = 0;(ecb mode)
	__align(4) UINT8 key_ecb[16]   = { 0x64,0xc0,0x4b,0x20,0x49,0xc8,0x23,0x55,0x49,0x44,0x3c,0x76,0xa1,0xc1,0xb7,0x89};  //key_ecb   = 0x64c04b2049c8235549443c76a1c1b789;
	__align(4) UINT8 plain_ecb[16] = { 0x24,0x80,0x0b,0x60,0x09,0x88,0x63,0x15,0x09,0x04,0x7c,0x36,0xe1,0x81,0xf7,0xc9};  //plain_ecb = 0x24800b600988631509047c36e181f7c9;
	__align(4) UINT8 crypt_ecb[16] = { 0x2f,0x11,0xec,0xdc,0x38,0x4a,0x99,0x73,0x7c,0x08,0x53,0x6b,0xdb,0xae,0x1f,0x16};  //crypt_ecb = 0x2f11ecdc384a99737c08536bdbae1f16; 
	__align(4) UINT8 result[16];

	blk_len = sizeof(plain_ecb)/16;
	//-----------------------    ECB mode (no swap)   ---------------------
	mode = SSF33_NORMAL_MODE;
//	mode = SSF33_SECURITY_MODE;

	ssf33_set_key(SSF33_INTER, (UINT32*)para_ecb, (UINT32*)key_ecb, SSF33_SWAP_ENABLE);
	ssf33_crypt_data((UINT32*)plain_ecb, (UINT32*)result, blk_len, SSF33_ENCRYPT, SSF33_ECB_MODE, (UINT32*)iv_ecb, mode);    //encryption internal parameter

	for ( i = 0; i < (blk_len*16); i++ )
	{
		if ( result[i] != crypt_ecb[i] )
		{
			return 1;
		}
	}
	
	ssf33_crypt_data((UINT32*)crypt_ecb, (UINT32*)result, blk_len, SSF33_DECRYPT, SSF33_ECB_MODE, (UINT32*)iv_ecb, mode);    //decryption internal parameter
	for ( i = 0; i < (blk_len*16); i++ )
	{
		if ( result[i] != plain_ecb[i] )
		{
			return 1;
		}
	}
	//-----------------------   End of  ECB mode   ---------------------
	return 0;	
}

UINT8 ssf33_cbc_test(void)
{
  UINT32 mode;
	UINT32 i;
	UINT32 blk_len;

	__align(4) UINT8 para_cbc[16]  = {0x55,0x86,0xa1,0x73,0xf6,0xbc,0x0b,0x76,0xf7,0xf6,0x44,0x30,0x83,0xb5,0x19,0x90};	 //para_cbc  = 0x5586a173f6bc0b76f7f6443083b51990;
	__align(4) UINT8 iv_cbc[16]    = {0x85,0x82,0x0a,0x4d,0xd2,0x72,0xef,0x63,0x80,0xa5,0xfb,0x8f,0x69,0x48,0x42,0x7f};	 //iv_cbc    = 0x85820a4dd272ef6380a5fb8f6948427f;
	__align(4) UINT8 key_cbc[16]   = {0x64,0xc0,0x4b,0x20,0x49,0xc8,0x23,0x55,0x49,0x44,0x3c,0x76,0xa1,0xc1,0xb7,0x89};  //key_ecb   = 0x64c04b2049c8235549443c76a1c1b789;
	__align(4) UINT8 plain_cbc[16] = {0x24,0x80,0x0b,0x60,0x09,0x88,0x63,0x15,0x09,0x04,0x7c,0x36,0xe1,0x81,0xf7,0xc9};  //plain_ecb = 0x24800b600988631509047c36e181f7c9;
	__align(4) UINT8 crypt_cbc[16] = {0xaa,0x93,0xe6,0x91,0xea,0x38,0x76,0x10,0xfc,0xad,0xa8,0xe4,0xb2,0xe6,0x5d,0x69};  //crypt_ecb = 0xaa93e691ea387610fcada8e4b2e65d69; 
	__align(4) UINT8 result[16];

	blk_len = sizeof(plain_cbc)/16;
	//-----------------------    CBC mode (no swap)   ---------------------
//	mode = SSF33_NORMAL_MODE;
	mode = SSF33_SECURITY_MODE;

	ssf33_set_key(SSF33_INTER, (UINT32*)para_cbc, (UINT32*)key_cbc, SSF33_SWAP_ENABLE);
	ssf33_crypt_data((UINT32*)plain_cbc, (UINT32*)result, blk_len, SSF33_ENCRYPT, SSF33_CBC_MODE, (UINT32*)iv_cbc, mode);    //encryption internal parameter
	for ( i = 0; i < (blk_len*16); i++ )
	{
		if ( result[i] != crypt_cbc[i] )
		{
			return 1;
		}
	}
	
	ssf33_crypt_data((UINT32*)crypt_cbc, (UINT32*)result, blk_len, SSF33_DECRYPT, SSF33_CBC_MODE, (UINT32*)iv_cbc, mode);    //decryption internal parameter
	for ( i = 0; i < (blk_len*16); i++ )
	{
		if ( result[i] != plain_cbc[i] )
		{
			return 1;
		}
	}
	//-----------------------   End of  CBC mode   ---------------------
	return 0;
}
void ssf33_test(void)
{
	printfS("SSF33 test start\n");
	
	enable_module(BIT_UAC | BIT_EMW | BIT_HRNG | BIT_SSF33 | BIT_HRNGS); //UAC,EMW,hrng,ssf33,hrng_sclk

	if(ssf33_ecb_test())
		printfS("  SSF33 ECB test fail\n");
	else
		printfS("  SSF33 ECB test pass\n");

	if(ssf33_cbc_test())
		printfS("  SSF33 CBC test fail\n");
	else
		printfS("  SSF33 CBC test pass\n");
	printfS("SSF33 Test Over!\n");

}


