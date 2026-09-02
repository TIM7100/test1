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
#include  "sm4.h"

UINT8 sm4_ecb_test(void)
{
  UINT32  mode;
	UINT32  i;
	UINT32  blk_len;

	__align(4) UINT8  key_ecb[16]    = {0x76,0x2a,0x64,0xe5,0x1c,0xae,0x49,0x95,0x65,0x8b,0x0b,0x53,0x47,0xee,0x77,0x78};  //key_ecb   = 0x762a64e51cae4995658b0b5347ee7778;	
	__align(4) UINT8  iv_ecb[16]     = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; //iv_ecb    = 0x0;	
	__align(4) UINT8  plain_ecb[16]  = {0x2b,0x3b,0x31,0x7b,0x40,0x11,0x5e,0x7d,0x5a,0x1d,0x02,0x42,0x19,0xb9,0x40,0xd3};  //plain_ecb = 0x2b3b317b40115e7d5a1d024219b940d3;	
	__align(4) UINT8  crypt_ecb[16]  = {0x64,0x14,0xe5,0x11,0xc7,0x33,0x69,0x70,0x87,0x3a,0x45,0x90,0x5a,0x4b,0x21,0x12};  //crypt_ecb = 0x6414e511c7336970873a45905a4b2112;
	__align(4) UINT8  result[16];

	blk_len = sizeof(plain_ecb)/16;

	//-----------------------    ECB mode (no swap)   ---------------------
  mode = SM4_NORMAL_MODE;
//mode = SM4_SECURITY_MODE;

	sm4_set_key((UINT32*)key_ecb, SM4_SWAP_ENABLE);
	sm4_crypt((UINT32*)plain_ecb, (UINT32*)result, blk_len, SM4_ENCRYPTION, SM4_ECB_MODE, (UINT32*)iv_ecb, mode);	//ecb encryption

	for(i = 0; i < (blk_len*16); i++)
	{
		if(result[i] != crypt_ecb[i])
		{
			return 1;
		}
	}

	sm4_crypt((UINT32*)crypt_ecb, (UINT32*)result, blk_len, SM4_DECRYPTION, SM4_ECB_MODE, (UINT32*)iv_ecb, mode);	//ecb decryption
	for(i = 0; i < (blk_len*16); i++)
	{
		if(result[i] != plain_ecb[i])
		{
			return 1;
		}
	}

	//-----------------------   End of  ECB mode   ---------------------
	return 0;
}
UINT8 sm4_cbc_test(void)
{
  UINT32 mode;
	UINT32 i;
	UINT32 blk_len;

	__align(4) UINT8  key_cbc[16]    = {0x09,0x18,0x48,0x10,0x21,0x0c,0x2d,0x9b,0x75,0xd8,0x76,0x16,0x42,0x6b,0x2e,0x6c};  //key_cbc   = 0x09184810210c2d9b75d87616426b2e6c;	
	__align(4) UINT8  iv_cbc[16]     = {0x02,0xaf,0x0f,0xb4,0x37,0xed,0x7e,0x94,0x25,0x17,0x58,0xdd,0x43,0x8f,0x3d,0x98};  //iv_cbc    = 0x02af0fb437ed7e94251758dd438f3d98;	
	__align(4) UINT8	plain_cbc[16]  = {0x0e,0xdb,0x79,0xe6,0x66,0xac,0x5d,0xd3,0x2a,0x68,0x2f,0x3e,0x1e,0xea,0x5b,0x91};  //plain_cbc = 0x0edb79e666ac5dd32a682f3e1eea5b91;	
	__align(4) UINT8	crypt_cbc[16]  = {0xbd,0x1a,0x47,0x11,0x38,0xce,0x97,0x12,0xb5,0xbc,0xb1,0x48,0xb2,0xd2,0x91,0xe5};  //crypt_cbc = 0xbd1a471138ce9712b5bcb148b2d291e5;
	__align(4) UINT8  result[16];

	blk_len = sizeof(plain_cbc)/16;

	//-----------------------    CBC mode (no swap)   ---------------------
//mode = SM4_NORMAL_MODE;
	mode = SM4_SECURITY_MODE;

	sm4_set_key((UINT32*)key_cbc, SM4_SWAP_ENABLE);
	sm4_crypt((UINT32*)plain_cbc, (UINT32*)result, blk_len, SM4_ENCRYPTION, SM4_CBC_MODE, (UINT32*)iv_cbc, mode);

	for(i = 0; i < (blk_len*16); i++)
	{
		if(result[i] != crypt_cbc[i])
		{
			return 1;
		}
	}

	sm4_crypt((UINT32*)crypt_cbc, (UINT32*)result, blk_len, SM4_DECRYPTION, SM4_CBC_MODE, (UINT32*)iv_cbc, mode);
	for(i = 0; i < (blk_len*16); i++)
	{
		if(result[i] != plain_cbc[i])
		{
			return 1;
		}
	}
	//-----------------------   End of  ECB mode   ---------------------

	return 0;
}
void sm4_test(void)
{
	printfS("SM4 test start\n");

	enable_module(BIT_UAC | BIT_EMW | BIT_HRNG | BIT_SM4 | BIT_HRNGS); //UAC,EMW,hrng,SM4,hrng_sclk

	if(sm4_ecb_test())
		printfS(" SM4 ECB test fail\n");
	else
		printfS(" SM4 ECB test pass\n");

	if(sm4_cbc_test())
		printfS(" SM4 CBC test fail\n");
	else
		printfS(" SM4 CBC test pass\n");

	printfS("SM4 Test Over!\r\n");

}







