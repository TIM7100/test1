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
#include  "ecc.h"
#include  "rsa_keygen.h"
#include  "ecdsa.h"
#include  "sm2.h"
#include  "sm3.h"
#include  "hrng.h"

UINT32 g_data_buf[1024];

#define _SM2_256

#ifdef _SM2_256				 
UINT32 CurveLength = 8;

const UINT32 P_Array[8]={0xFFFFFFFF,0xFFFFFFFF,0x00000000,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFE};          
const UINT32 a_Array[8]={0xFFFFFFFC,0xFFFFFFFF,0x00000000,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFE};                                                        
const UINT32 b_Array[8]={0x4D940E93,0xDDBCBD41,0x15AB8F92,0xF39789F5,0xCF6509A7,0x4D5A9E4B,0x9D9F5E34,0x28E9FA9E};
const UINT32 N_Array[8]={0x39D54123,0x53BBF409,0x21C6052B,0x7203DF6B,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFE};
const UINT32 BaseX_Array[8]={0x334C74C7,0x715A4589,0xF2660BE1,0x8FE30BBF,0x6A39C994,0x5F990446,0x1F198119,0x32C4AE2C};
const UINT32 BaseY_Array[8]={0x2139F0A0,0x02DF32E5,0xC62A4740,0xD0A9877C,0x6B692153,0x59BDCEE3,0xF4F6779C,0xBC3736A2};

#endif

UINT32 dKey[WL]={0x15897263,0x0C23661D,0x171B1B65,0x2A519A55,0x3DFF7979,0x068C8D80,0xBD433C6C,0x128B2FA8};
//UINT32 QxKey[WL];
//UINT32 QyKey[WL];
UINT32 QKey[2*SM2_WL]={0x4DF2548A,0xE97C04FF,0xA5844495,0x02BB79E2,0x825BE462,0x471BEE11,0x8AA0F119,0x0AE4C779,0xB798E857,0xA9FE0C6B,0xA176D684,0x07353E53,0x17B7F16F,0x6352A73C,0x8F1CD4E1,0x7C0240F8};
UINT8 Signr[32]={0x0};
UINT8 Signs[32]={0x0};

UINT8  debug_t[64];
//end define
UINT8 error_code = 0;

void data_out_back(UINT32* out_data,UINT32 data_len)
{
	int i;
	
	printfS("%x",out_data[data_len-1]);
	for(i=(data_len-2);i>=0;i--)
	{
		if(out_data[i] <= 0xfffffff)
			printfS("0");
		if(out_data[i] <= 0xffffff)
			printfS("0");
		if(out_data[i] <= 0xfffff)
			printfS("0");
		if(out_data[i] <= 0xffff)
			printfS("0");
		if(out_data[i] <= 0xfff)
			printfS("0");
		if(out_data[i] <= 0xff)
			printfS("0");
		if(out_data[i] <= 0xf)
			printfS("0");
		printfS("%x",out_data[i]);
	}
	printfS("\n");
}

void UpdateM(UINT32 *m,UINT32 m_len)
{
	int i;
	
	hrng_initial();
	if(m_len==0)
	{
		return;
	}
	else
	{
		for(i=(m_len-1);i>=0;i--)
		{		
			m[i] = get_hrng32();
		}
	}
	
}
 	

void swap_array(UINT32 *ptr, UINT32 len)
{
	UINT32 i;
	UINT32 temp;
	
	for (i=0;i<len/2;i++) {
		temp = *(ptr+i);
		*(ptr+i) = *(ptr+len-1-i);
		*(ptr+len-1-i) = temp;
	}
}
void memcpy8(UINT8 *pDst,const UINT8 *pSrc, UINT32 cnt)
{
	if(!pDst)
	{
		return ;
	}
	if(!pSrc)
	{
		return ;
	}
	while(cnt--)
	{
		*pDst=*pSrc;
		pDst++;
		pSrc++;
	}
}

//test SM2 edcrypt
UINT32 SM2_edcrypt_test(void)
{
	ECC_G_STR ecc_glb_str;
	int i;
 	UINT32 message[17] = {0x656E6372,0x79707469,0x6F6E2073,0x74616E64,0x61726400};
 	UINT32 result[17];
 	UINT32 C2[17];
 	UINT32 C1[16];
 	UINT32 C3[8];  	
	SM2_CRYPT_CTX sm2_ctx;
	SM3_CTX sm3_ctx;

	error_code = 0;
		
	ECC_para_initial((ECC_G_STR *)(&ecc_glb_str),CurveLength,(UINT32 *)P_Array,(UINT32 *)a_Array,(UINT32 *)b_Array,(UINT32 *)N_Array,(UINT32 *)BaseX_Array,(UINT32 *)BaseY_Array);

	//SM2 crypt starts:
	printfS("\nSM2_Crypt_Test start!\n");
	UpdateM(message,17);

 	//ECC signature demo starts:
	NN_AssignZero(QKey,CurveLength<<1);
	ECDSA_keypair((ECC_G_STR *)(&ecc_glb_str),dKey,QKey,QKey+CurveLength);
	sm2_swap_array(QKey,CurveLength);
	sm2_swap_array(QKey+CurveLength,CurveLength);
	SM3_encode(debug_t,QKey,CurveLength<<3);
	for(i=0;i<17;i++)
	{
		message[i] = SM3_SWAP32(message[i]);
		//printfS("%.8x",*((UINT32*)message+i));	
	}
	//printfS("\n");
	SM2_ENCYPT_INIT:
	sm2_encrypt_init((ECC_G_STR *)(&ecc_glb_str),(SM2_CRYPT_CTX*)(&sm2_ctx),(SM3_CTX*)(&sm3_ctx),debug_t,(UINT8*)C1);
	if(sm2_encrypt_process((SM2_CRYPT_CTX*)(&sm2_ctx),(SM3_CTX*)(&sm3_ctx),(UINT8*)message,2,(UINT8*)C2))
	{
		goto SM2_ENCYPT_INIT;
	}
	if(sm2_encrypt_process((SM2_CRYPT_CTX*)(&sm2_ctx),(SM3_CTX*)(&sm3_ctx),(UINT8*)message+2,32,(UINT8*)C2+2))
	{
		goto SM2_ENCYPT_INIT;
	}
	if(sm2_encrypt_process((SM2_CRYPT_CTX*)(&sm2_ctx),(SM3_CTX*)(&sm3_ctx),(UINT8*)message+34,34,(UINT8*)C2+34))
	{
		goto SM2_ENCYPT_INIT;
	}

	sm2_encrypt_final((SM2_CRYPT_CTX*)(&sm2_ctx),(SM3_CTX*)(&sm3_ctx),(UINT8*)C3);

//	for(i=0;i<17;i++)
//	{
//		printfS("%.8x",*((UINT32*)C2+i));	
//	}
//	printfS("\n");

	sm2_swap_array(dKey,CurveLength);
	SM3_encode(debug_t,dKey,CurveLength<<2);
	sm2_decrypt_init((ECC_G_STR *)(&ecc_glb_str),(SM2_CRYPT_CTX*)(&sm2_ctx),(SM3_CTX*)(&sm3_ctx),debug_t,(UINT8*)C1,SM2_NORMAL);
	sm2_decrypt_process((SM2_CRYPT_CTX*)(&sm2_ctx),(SM3_CTX*)(&sm3_ctx),(UINT8*)C2,68,(UINT8*)result);
//	for(i=0;i<17;i++)
//	{
//		printfS("%.8x",*((UINT32*)result+i));	
//	}
//	printfS("\n");
 	if(!sm2_decrypt_final((SM2_CRYPT_CTX*)(&sm2_ctx),(SM3_CTX*)(&sm3_ctx),(UINT8*)C3))
 	{
 		error_code = 0;
 		printfS("encrypt Success!!!\n");
 	}
 	else
 	{
 		error_code |= 1;
 		printfS("encrypt error!!!\n");
 	}
	UpdateM(message,16);
 	//ECC signature demo starts:
	NN_AssignZero(QKey,CurveLength<<1);
	ECDSA_keypair((ECC_G_STR *)(&ecc_glb_str),dKey,QKey,QKey+CurveLength);
	sm2_swap_array(QKey,CurveLength);
	sm2_swap_array(QKey+CurveLength,CurveLength);
	//SM3_encode(debug_t,QKey,CurveLength<<3);
	for(i=0;i<16;i++)
	{
		QKey[i] = SM3_SWAP32(QKey[i]);
		message[i] = SM3_SWAP32(message[i]);
		//printfS("%.8x",*((UINT32*)message+i));	
	}
	//printfS("\n");

	if(sm2_encrypt((ECC_G_STR *)(&ecc_glb_str),(SM2_CRYPT_CTX*)(&sm2_ctx),(SM3_CTX*)(&sm3_ctx),(UINT8*)message,64,(UINT8*)QKey,(UINT8*)C1,(UINT8*)C2,(UINT8*)C3))
	{
		error_code |= 2;
		printfS("encrypt error!!!\n");
	}
	else
	{
		printfS("encrypt Success!!!\n");	
	}
	sm2_swap_array(dKey,CurveLength);
	SM3_encode(debug_t,dKey,CurveLength<<2);
	if(sm2_decrypt((ECC_G_STR *)(&ecc_glb_str),(SM2_CRYPT_CTX*)(&sm2_ctx),(SM3_CTX*)(&sm3_ctx),debug_t,(UINT8*)C1,(UINT8*)C2,(UINT8*)C3,64,(UINT8*)result,SM2_NORMAL))
	{
		error_code |= 4;
		printfS("decrypt error!!!\n");
	}
	else
	{
		printfS("decrypt Success!!!\n");
	}
	return error_code;
	//end test SM2 crypt
}
//end test SM2 edcrypt

 /**************************************************************************************************
* Function Name  : GetZ
* Description    : Computes  id_Z = sm3(id_bit_length||id||ECC_a||ECC_b||ECC_BaseX||ECC_BaseY||PubX||PubY)
* Input          : - *id           : input id data 
				 : - id_bit_length : the bit length of id data (id_bit_length%8=0)
				 : - * PubX        : public key x coordinate
				 : - * PubY        : public key y coordinate
* Output		 : - * id_Z        : output the hash data Z 
* Return         : carry
**************************************************************************************************/

UINT8 GetZ(ECC_G_STR *p_ecc_para,UINT8* id,UINT16 id_bit_length,UINT32* PubX,UINT32* PubY,UINT8* id_Z)
{
	UINT8 data_buf[256];
	UINT32 byte_cnt;
	UINT32 bit_remain;
//	UINT32 word_cnt;
	UINT32 temp[8];
	int i;
	
	for(i=0;i<256;i++)
	{
		data_buf[i] = 0;
	}
	byte_cnt = id_bit_length >> 3;
	bit_remain = id_bit_length & 0x07;
	if(bit_remain)
	{
		return 1;
	}	
	data_buf[0] = (UINT8)(id_bit_length>>8);
	data_buf[1] = (UINT8)id_bit_length;
	//copy id after id_bit_length
	memcpy((data_buf+2),id,byte_cnt);
	//copy ECC_a after id	
	memcpy((UINT8*)temp,(UINT8*)p_ecc_para->ECC_a,CurveLength<<2);
	swap_array(temp,CurveLength);
	for(i=0;i<8;i++)
	{
		 temp[i] = SM3_SWAP32(temp[i]);
	}
	memcpy((data_buf+2+byte_cnt),(UINT8*)temp,CurveLength<<2);
		
	//copy ECC_b after ECC_a
	//sm3_memcpy(temp,p_ecc_para->ECC_b,CurveLength);
	memcpy((UINT8*)temp,(UINT8*)p_ecc_para->ECC_b,CurveLength<<2);
	swap_array(temp,CurveLength);
	for(i=0;i<8;i++)
	{
		 temp[i] = SM3_SWAP32(temp[i]);
	}	
	memcpy((data_buf+2+byte_cnt+CurveLength*4),(UINT8*)temp,CurveLength<<2);	
	
	//copy ECC_BaseX after ECC_b	
	//sm3_memcpy(temp,p_ecc_para->ECC_BaseX,CurveLength);
	memcpy((UINT8*)temp,(UINT8*)p_ecc_para->ECC_BaseX,CurveLength<<2);
	swap_array(temp,CurveLength);
	for(i=0;i<8;i++)
	{
		 temp[i] = SM3_SWAP32(temp[i]);
	}
	memcpy((data_buf+2+byte_cnt+2*CurveLength*4),(UINT8*)temp,CurveLength<<2);
	
	//copy ECC_BaseY after ECC_BaseX
	//sm3_memcpy(temp,p_ecc_para->ECC_BaseY,CurveLength);
	memcpy((UINT8*)temp,(UINT8*)p_ecc_para->ECC_BaseY,CurveLength<<2);
	swap_array(temp,CurveLength);
	for(i=0;i<8;i++)
	{
		 temp[i] = SM3_SWAP32(temp[i]);
	}
	memcpy((data_buf+2+byte_cnt+3*CurveLength*4),(UINT8*)temp,CurveLength<<2);
	
	//copy PubX after ECC_BaseY	
	//sm3_memcpy(temp,PubX,CurveLength);
	memcpy((UINT8*)temp,(UINT8*)PubX,CurveLength<<2);
	swap_array(temp,CurveLength);
	for(i=0;i<8;i++)
	{
		 temp[i] = SM3_SWAP32(temp[i]);
	}
	memcpy((data_buf+2+byte_cnt+4*CurveLength*4),(UINT8*)temp,CurveLength<<2);
	
	//copy PubY after PubX
	//sm3_memcpy(temp,PubY,CurveLength);
	memcpy((UINT8*)temp,(UINT8*)PubY,CurveLength<<2);
	swap_array(temp,CurveLength);
	for(i=0;i<8;i++)
	{
		 temp[i] = SM3_SWAP32(temp[i]);
	}
	memcpy((data_buf+2+byte_cnt+5*CurveLength*4),(UINT8*)temp,CurveLength<<2);

//	word_cnt = ((2+byte_cnt)>>2) +  CurveLength*6;
	bit_remain = ((2+byte_cnt+24*CurveLength)<<3) & 0x1f;
	
	sm3_hash(data_buf,(2+byte_cnt+24*CurveLength),id_Z);

//	for(i=0;i<32;i++)
//	{
//		printf("%x ",id_Z[i]);
//	}
//	printf("\n");
	
	return 0;
}


void GetE(UINT8 *pMessage,UINT32 byte_len,UINT8* id_Z,UINT8 *pDigest)
{
	SM3_CTX context;
	
	SM3_initial(&context);
	SM3_update(&context,id_Z,32);
	SM3_update(&context,pMessage,byte_len);
	SM3_final(pDigest,&context);

}
UINT32 SM2_Sign_Test(ECC_G_STR *p_ecc_para,MATH_G_STR *p_math_str)
{
	UINT8 digest[32]={0xB9,0xEF,0xFE,0x76,0x5D,0x42,0xE3,0xD9,0x68,0x2D,0x48,0xBB,0x9A,0x87,0xE6,0xFC,0x5C,0x37,0x7F,0xB1,0x28,0x47,0x6E,0x00,0xCD,0x82,0xB8,0xB0,0xB5,0x24,0xF5,0x52};
	UINT8 digest1[32]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff,0x01};
	UINT8 id[32] = {0x41,0x4C,0x49,0x43,0x45,0x31,0x32,0x33,0x40,0x59,0x41,0x48,0x4F,0x4F,0x2E,0x43,0x4F,0x4D,0x12,0x34,0x56};
	UINT8 id_Z[32];
	UINT8 id_bit_length; 
	UINT8 message[36]={0x6D,0x65,0x73,0x73,0x61,0x67,0x65,0x20,0x64,0x69,0x67,0x65,0x73,0x74,0x0};
//	int i;
		
	error_code = 0;
	
	//ECC signature demo starts:
	NN_AssignZero(QKey,CurveLength<<1);
	ECDSA_keypair(p_ecc_para,dKey,QKey,QKey+CurveLength);
	id_bit_length = 0x0090;
	if(GetZ(p_ecc_para,id,id_bit_length,QKey,QKey+CurveLength,id_Z))
	{
		 error_code |= 4; //GetZ error
	}

	//UpdateM(message,9);
	GetE(message,14,id_Z,digest);
	sm2_swap_array(dKey,CurveLength);
	SM3_encode(debug_t,dKey,CurveLength<<2);
	sm2_sign(p_ecc_para,p_math_str,digest,debug_t,Signr,Signs,SM2_SECURITY); 

	//2nd step:Sign digest Success!!  
	//@digest hasn't changed!!
	sm2_swap_array(QKey,CurveLength);
	sm2_swap_array(QKey+CurveLength,CurveLength);
	SM3_encode(debug_t,QKey,CurveLength<<3);
	if(!sm2_verify(p_ecc_para,p_math_str,digest,debug_t,Signr,Signs))
	{
		 error_code |= 0;//Verification Success!!
		 printfS("Verification Success!!!\n");
	}			
	else
	{
		 error_code |= 1;//Verification Failed!!
		 printfS("Verification Failed!!!\n");
	} 	
			
	//@digest has changed!! should  Verification Failed
	if(!sm2_verify(p_ecc_para,p_math_str,digest1,debug_t,Signr,Signs))
	{
		 error_code |= 2; //Verification Success!!
		 printfS("Verification Failed!!!\n");
	} 			
	else
	{
		 error_code |= 0;//Verification Failed!!
		 printfS("Verification Success!!!\n");
	}
	
	return error_code;
}
	

UINT32 SM2_Exchange_Key_Test(ECC_G_STR *p_ecc_para)
{
	int i;


UINT8 prikeyA[32]  = {0x70,0x44,0x72,0x16,0xde,0x1b,0xe3,0x0c,0x93,0x3c,0x97,0x1e,0xa2,0x67,0x2d,0xd2,0xb1,0x42,0xfe,0x77,0x79,0x17,0xba,0x88,0x0e,0x7d,0x8f,0x00,0x6a,0x84,0x19,0x00};//prikeyA = 0x70447216 de1be30c...
UINT8 pub_keyA[64] = {0xf9,0x94,0x62,0xc4,0x59,0xc9,0x14,0x3b,0xea,0xab,0xa0,0x5c,0x14,0x9c,0x76,0x0f,0xd0,0xc6,0x6b,0x8e,0xa5,0xec,0x3b,0xb8,0x02,0x73,0x4f,0x92,0xfb,0xad,0x39,0xdc, //pub_keyA_X = 0xf99462c4 59..,
                      0xf2,0x98,0x10,0xd5,0x68,0x7d,0xc9,0x46,0xdc,0x38,0x10,0xc0,0xe2,0x45,0xc9,0x8c,0x7a,0xee,0x8b,0x15,0x86,0x0e,0x1e,0xf9,0x73,0xb4,0x5e,0xe4,0x21,0xfd,0x51,0xe7};//pub_keyA_Y = 0xf29810d5 68..,
UINT8 prikey_tempA[32]  = {0x41,0x56,0x11,0x4a,0xaa,0x09,0xf9,0xdf,0xed,0xd9,0x3f,0xd0,0xd2,0x9e,0xd3,0xe3,0x1b,0xfa,0x72,0x2e,0x7d,0x25,0x92,0xa4,0xd3,0xdf,0xd4,0xbe,0x13,0x48,0xd8,0x92};//prikey_tempA = 0x4156114a aa..
UINT8 pub_key_tempA[64] = {0x32,0x4c,0x54,0x11,0x7c,0x3b,0x9d,0x88,0xfc,0xb2,0xd4,0xb3,0xdf,0xe0,0x7a,0xf6,0x6f,0x68,0xe7,0x90,0x7f,0xb9,0x05,0x16,0xea,0x06,0x20,0xb5,0x20,0x7d,0xf9,0x3d, //pub_key_tempA_X = 0x324c5411 7c..
                           0x2a,0x61,0x28,0x4a,0x50,0xb7,0x17,0xcd,0x41,0xc9,0xee,0x09,0x73,0x1a,0xdb,0x32,0x9a,0x6f,0xfe,0x69,0x3d,0x25,0x56,0x08,0x36,0x9b,0x0b,0xa5,0xf3,0x8e,0x49,0x4a};//pub_key_tempA_Y = 0x2a61284a 50..
UINT8 ZA[32] = {0xE4,0xD1,0xD0,0xC3,0xCA,0x4C,0x7F,0x11,0xBC,0x8F,0xF8,0xCB,0x3F,0x4C,0x02,0xA7,0x8F,0x10,0x8F,0xA0,0x98,0xE5,0x1A,0x66,0x84,0x87,0x24,0x0F,0x75,0xE2,0x0F,0x31};//ZA = 0xE4D1D0C3 CA4C7F11...

UINT8 prikeyB[32]  = {0x9d,0x2b,0x23,0xe4,0x76,0x78,0x82,0x95,0x93,0x3c,0x2c,0xdb,0xa5,0xeb,0x8a,0xce,0x96,0xeb,0x3b,0x07,0x8a,0x96,0xd3,0x6c,0x30,0x09,0x82,0x5d,0xfb,0x20,0x1c,0x14};
UINT8 pub_keyB[64] = {0x2c,0x45,0xe2,0xb4,0x06,0xf3,0x54,0x43,0x63,0x89,0xb7,0xae,0xd8,0x54,0x0b,0xc6,0x9b,0x59,0xd5,0x3c,0x0c,0xcc,0x1e,0xca,0x1b,0x34,0x92,0x96,0xe9,0xd6,0x18,0x08,
                      0xc8,0xdf,0xd5,0xf3,0x09,0x7c,0x8b,0x39,0x15,0x7e,0xdb,0xcd,0xf5,0x96,0x3a,0xa9,0x25,0xc1,0x67,0x83,0x35,0xf5,0x95,0x35,0xb6,0xb2,0x63,0xf1,0xf9,0xff,0x46,0x3d};
UINT8 prikey_tempB[32]  = {0x0d,0x60,0xe2,0x35,0x2c,0x56,0x8c,0x61,0x78,0x8b,0x2e,0xcd,0xb2,0xad,0x2d,0xeb,0xc4,0x5c,0x4e,0xb7,0xd7,0x67,0xa8,0xad,0x0b,0x9e,0x3c,0xc8,0xc0,0x36,0x47,0x7a};
UINT8 pub_key_tempB[64] = {0x63,0x80,0x5c,0x11,0x61,0x17,0xe4,0x4d,0xce,0x18,0x92,0x2c,0x6d,0x35,0xa9,0x1d,0x0b,0x59,0xd4,0x12,0x85,0xa6,0xa3,0xf2,0xf0,0x11,0x5b,0xb4,0x91,0x8d,0x19,0x87,
                           0xbb,0x85,0x42,0x6f,0xd3,0xe0,0xbd,0x0d,0x27,0x44,0x0e,0xdd,0x2c,0x21,0xa6,0xe0,0x4c,0x47,0x1d,0x78,0x9f,0xfb,0xe3,0xac,0x21,0x2d,0xa6,0x3a,0x3a,0xc5,0x17,0x50};
UINT8 ZB[32] = {0x6B,0x4B,0x6D,0x0E,0x27,0x66,0x91,0xBD,0x4A,0x11,0xBF,0x72,0xF4,0xFB,0x50,0x1A,0xE3,0x09,0xFD,0xAC,0xB7,0x2F,0xA6,0xCC,0x33,0x6E,0x66,0x56,0x11,0x9A,0xBD,0x67};
	
	UINT8 Ex_KA[32];
	UINT8 S1[32];
	UINT8 SA[32];
	UINT8 Ex_KB[32];
	UINT8 S2[32];
	UINT8 SB[32];
	UINT32 klen;
	error_code = 0;
	
	klen = 128;
	i = sm2_Exchange_Key(p_ecc_para,1,prikeyA,pub_keyB,prikey_tempA,pub_key_tempA,pub_key_tempB,ZA,ZB,klen,Ex_KA,S1,SA);
	if(i)
	{
		error_code = 0x01;
		printfS("exchange key error!!!\n");
	}
//	for(i=0;i<16;i++)
//	{
//		printfS("%x ",Ex_KA[i]);
//	}
//	printfS("\n");
//	for(i=0;i<32;i++)
//	{
//		printfS("%x ",S1[i]);
//	}
//	printfS("\n");
//	for(i=0;i<32;i++)
//	{
//		printfS("%x ",SA[i]);
//	}
//	printfS("\n");	
	i = sm2_Exchange_Key(p_ecc_para,0,prikeyB,pub_keyA,prikey_tempB,pub_key_tempB,pub_key_tempA,ZB,ZA,klen,Ex_KB,SB,S2);
	if(i)
	{
		error_code |= 0x02;
		printfS("exchange key error!!!\n");
	}
	
	for(i=0;i<(klen>>3);i++)
	{
		if(Ex_KA[i]!=Ex_KB[i])
		{
			error_code |= 0x04;
			printfS("exchange key error!!!\n");
			break;
		}
	}
	for(i=0;i<32;i++)
	{
		if(S1[i]!=SB[i])
		{
			error_code |= 0x08;
			printfS("exchange key error!!!\n");
			break;
		}
	}
	for(i=0;i<32;i++)
	{
		if(SA[i]!=S2[i])
		{
			error_code |= 0x10;
			printfS("exchange key error!!!\n");
			break;
		}
	}
	return error_code;
}

void SM2_test(void)
{
	int i;
	ECC_G_STR ecc_glb_str;
	MATH_G_STR math_glb_str;

	enable_module(BIT_UAC | BIT_PKI |BIT_HRNG |BIT_HRNGS |BIT_HASH);
	
	ECC_para_initial((ECC_G_STR *)(&ecc_glb_str),CurveLength,(UINT32 *)P_Array,(UINT32 *)a_Array,(UINT32 *)b_Array,(UINT32 *)N_Array,(UINT32 *)BaseX_Array,(UINT32 *)BaseY_Array);
	i = 1;
	while(i--)
	{
		//SM2 crypt starts:
		printfS("\nSM2_Crypt_Test start!\n");
		error_code = SM2_edcrypt_test();
		if(error_code)
		{
			printfS("\nSM2_Crypt_Test end error!!!!\n");	
			return;
		}
		else
		{
			printfS("\nSM2_Crypt_Test end success!!!!\n"); 
		}
		//end test SM2 crypt

		//SM2 sign  starts:
		printfS("\nSM2_Sign_Test start!\n");
		error_code = SM2_Sign_Test((ECC_G_STR *)(&ecc_glb_str),(MATH_G_STR *)(&math_glb_str));
		if(error_code)
		{
			printfS("\nSM2_Sign_Test end error!!!!\n");	
			return;
		}
		else
		{
			printfS("\nSM2_Sign_Test end success!!!!\n"); 
		}
		//end test SM2 sign

		//SM2 exchange key starts:
		printfS("\nSM2_Exchange_Key_Test start!\n");
		error_code = SM2_Exchange_Key_Test((ECC_G_STR *)(&ecc_glb_str));
		if(error_code)
		{
			error_code = 1;
			printfS("\nSM2_Exchange_Key_Test end error!!!!\n");	
			return;
		}
		else
		{
			error_code = 0;
			printfS("\nSM2_Exchange_Key_Test end success!!!!\n"); 
		}
	}
	//end test SM2 exchange key
}
//end test SM2

void SM3_test(void)
{
	UINT32 digest[8];
	UINT32 digest1[8]={0x66c7f0f4,0x62eeedd9,0xd1f2d46b,0xdc10e4e2,0x4167c487,0x5cf2f7a2,0x297da02b,0x8f4ba8e0};
	UINT32 digest2[8]={0x7ae067b1,0x99020410,0x8c6783e0,0x7e67baee,0x9f825811,0x9c208198,0x49127103,0x805fe65};
	UINT32 digest3[8]={0xdebe9ff9,0x2275b8a1,0x38604889,0xc18e5a4d,0x6fdb70e5,0x387e5765,0x293dcba3,0x9c0c5732};
	UINT32 digest4[8]={0x9a032f0c,0xf27e4b40,0x8f252452,0xd451cac5,0x1a422d43,0xae73ab6c,0xd7ec2483,0x241358e9};
	int i;

	SM3_CTX context;
	
	enable_module(BIT_UAC|BIT_HASH);
	
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







