/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : app.c
 * Description : application example source file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/

#include "app.h"
#include "sm3.h"
#include "sha256.h"
#include "sha1.h"
#include "sha384.h"

UINT32 g_data_buf[1024];

UINT32 error_code=0;

void sm3_test(void)
{
	UINT32 digest[8];
	UINT32 digest1[8]={0x66c7f0f4,0x62eeedd9,0xd1f2d46b,0xdc10e4e2,0x4167c487,0x5cf2f7a2,0x297da02b,0x8f4ba8e0};
	UINT32 digest2[8]={0x7ae067b1,0x99020410,0x8c6783e0,0x7e67baee,0x9f825811,0x9c208198,0x49127103,0x805fe65};
	UINT32 digest3[8]={0xdebe9ff9,0x2275b8a1,0x38604889,0xc18e5a4d,0x6fdb70e5,0x387e5765,0x293dcba3,0x9c0c5732};
	UINT32 digest4[8]={0x9a032f0c,0xf27e4b40,0x8f252452,0xd451cac5,0x1a422d43,0xae73ab6c,0xd7ec2483,0x241358e9};
	int i;

	SM3_CTX context;
	
	printfS("---------sm3 test-------\n");
	error_code = 0;
	
	DATABUF[0] = 0x61;	
	DATABUF[1] = 0x62;
	DATABUF[2] = 0x63;
	sm3_hash(DATABUF,3,(UINT8*)digest);
	for(i=0;i<8;i++)
	{
		digest[i] = SM3_SWAP32(digest[i]);
	}
	for(i=0;i<8;i++)
	{
		if(digest[i]!=digest1[i])
		{
			error_code = 1;
			printfS("\nsm3 error1\n");	
			return;		
		}
	}
	printfS("sm3 success1\n");
	//message is 0x11223344616263(big number format),digest is digest2
	DATABUF[0] = 0x11;	
	DATABUF[1] = 0x22;
	DATABUF[2] = 0x33;
	DATABUF[3] = 0x44;	
	DATABUF[4] = 0x61;
	DATABUF[5] = 0x62;
	DATABUF[6] = 0x63;	

	sm3_hash(DATABUF,7,(UINT8*)digest);
	for(i=0;i<8;i++)
	{
		digest[i] = SM3_SWAP32(digest[i]);
	}
	for(i=0;i<8;i++)
	{
		if(digest[i]!=digest2[i])
		{
			error_code = 1;
			printfS("\nsm3 error2\n");
			return;			
		}
	}
	printfS("sm3 success2\n");	

	//message is 0x61626364616263646162636461626364616263646162636461626364616263646162636461626364616263646162636461626364616263646162636461626364(big number format),digest is digest3
	for(i=0;i<64;i+=4)
	{
		DATABUF[i] = 0x61;
		DATABUF[i+1] = 0x62;
		DATABUF[i+2] = 0x63;
		DATABUF[i+3] = 0x64;
	}
	SM3_initial(&context);					

	SM3_update(&context,DATABUF,32);
	SM3_update(&context,DATABUF+32,32);	 
	SM3_final((UINT8*)digest,&context);
	for(i=0;i<8;i++)
	{
		digest[i] = SM3_SWAP32(digest[i]);
	}
	for(i=0;i<8;i++)
	{
		if(digest[i]!=digest3[i])
		{
			error_code = 1;
			printfS("\nsm3 error3\n");
			return;			
		}
	}
	printfS("sm3 success3\n");


	for(i=0;i<56;i+=4)
	{
		DATABUF[i] = 0x61;
		DATABUF[i+1] = 0x62;
		DATABUF[i+2] = 0x63;
		DATABUF[i+3] = 0x64;
	}
	sm3_hash(DATABUF,56,(UINT8*)digest);
	for(i=0;i<8;i++)
	{
		digest[i] = SM3_SWAP32(digest[i]);
	}
	for(i=0;i<8;i++)
	{
		if(digest[i]!=digest4[i])
		{
			error_code = 1;
			printfS("\nsm3 error4\n");
			return;			
		}
	}
	printfS("sm3 success4\n");
	
}


void sha1_test(void)
{
	UINT8 message[3]="abc";
	UINT32 expect_digest[5] = {0xA9993E36,0x4706816A,0xBA3E2571,0x7850C26C,0x9CD0D89D};
	UINT32 digest[5];
	int i;

	printf("---------SHA1_hash test-------\n");
	SHA1_hash(message,3,(UINT8*)digest);

	for(i=0;i<5;i++)
	{
		reverse_DWORD(digest+i);
	}
	for(i=0;i<5;i++)
	{
		if(digest[i]!=expect_digest[i])
		{
			printf("sha1 error.\n");  
			error_code = 1;
			 return;
		}
	}
	printf("sha1 success.\n");

}

void sha256_test(void)
{
	UINT8 message[3]="abc";
	UINT32 expect_digest[8] = {0xBA7816BF,0x8F01CFEA,0x414140DE,0x5DAE2223,0xB00361A3,0x96177A9C,0xB410FF61,0xF20015AD};
	UINT32 digest[8];
	int i;

  printf("---------SHA256_hash test-------\n");
	SHA256_hash(message,3,(UINT8*)digest);
	for(i=0;i<8;i++)
	{
		reverse_DWORD(digest+i);
	}
	for(i=0;i<8;i++)
	{
		if(digest[i]!=expect_digest[i])
		{
			 printf("sha256 error.\n"); 
			 error_code = 1;
			 return;
		}
	}
	printf("sha256 success.\n");
}

void sha384_test(void)
{
	UINT8 message[3]="abc";
	UINT32 expect_digest[12] = {0xCB00753F,0x45A35E8B,0xB5A03D69,0x9AC65007,0x272C32AB,0x0EDED163,0x1A8B605A,0x43FF5BED,0x8086072B,0xA1E7CC23,0x58BAECA1,0x34C825A7};
	UINT32 digest[12];
	int i;
//	UINT8 flag=0;

	printf("---------SHA384_hash test-------\n");
	SHA384_hash(message,3,(UINT8*)digest);
	for(i=0;i<12;i++)
	{
		reverse_DWORD(digest+i);
		//printf("0x%x\n",digest[i]);
	}
	for(i=0;i<12;i++)
	{
		if(digest[i]!=expect_digest[i])
		{
			printf("sha384 error.\n");  
			error_code = 1;
			 return;
		}
	}
	printf("sha384 success.\n");
}

void sha512_test(void)
{
	UINT8 message[3]="abc";
	UINT32 expect_digest[16] = {0xDDAF35A1,0x93617ABA,0xCC417349,0xAE204131,0x12E6FA4E,0x89A97EA2,0x0A9EEEE6,0x4B55D39A,0x2192992A,0x274FC1A8,0x36BA3C23,0xA3FEEBBD,0x454D4423,0x643CE80E,0x2A9AC94F,0xA54CA49F};
	UINT32 digest[16];
	int i;
//	UINT8 flag=0;

	printf("---------SHA512_hash test-------\n");
	SHA512_hash(message,3,(UINT8*)digest);
	for(i=0;i<16;i++)
	{
		reverse_DWORD(digest+i);
		//printf("0x%x\n",digest[i]);
	}
	for(i=0;i<16;i++)
	{
		if(digest[i]!=expect_digest[i])
		{
			printf("sha512 error.\n"); 
			error_code = 1;
			return;
		}
	}
	printf("sha512 success.\n");
}

