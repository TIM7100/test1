/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : sm2_app.c
 * Description : sm2 application demo
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#include  "sm2_app.h"

static UINT32 N_Array[8] = {0xFFFFFFFE,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0x7203DF6B,0x21C6052B,0x53BBF409,0x39D54123};
static UINT32 a_Array[8] = {0xFFFFFFFE,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0x00000000,0xFFFFFFFF,0xFFFFFFFC};
static UINT32 b_Array[8] = {0x28E9FA9E,0x9D9F5E34,0x4D5A9E4B,0xCF6509A7,0xF39789F5,0x15AB8F92,0xDDBCBD41,0x4D940E93};
static UINT32 P_Array[8] = {0xFFFFFFFE,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0x00000000,0xFFFFFFFF,0xFFFFFFFF};
static UINT32 BaseX_Array[8] = {0x32C4AE2C,0x1F198119,0x5F990446,0x6A39C994,0x8FE30BBF,0xF2660BE1,0x715A4589,0x334C74C7};
static UINT32 BaseY_Array[8] = {0xBC3736A2,0xF4F6779C,0x59BDCEE3,0x6B692153,0xD0A9877C,0xC62A4740,0x02DF32E5,0x2139F0A0};

static UINT32 dKey[8];
static UINT32 QKey[8*2];

/**************************************************************************************************
* Function Name  : GetZ
* Description    : Computes  id_Z = sm3(id_bit_length||id||ECC_a||ECC_b||ECC_BaseX||ECC_BaseY||PubX||PubY)
* Input          : - *id           : input id data 
				 : - id_len        : the byte length of id data
				 : - * PubX        : public key x coordinate
				 : - * PubY        : public key y coordinate
* Output		 : - * id_Z        : output the hash data Z 
* Return         : carry
**************************************************************************************************/
static UINT8 GetZ(ECC_G_STR *p_ecc_para, UINT8 *id, UINT16 id_len, UINT8 *PubX, UINT8 *PubY, UINT8 *id_Z)
{
	UINT8 data_buf[256];
	UINT32 temp[8];
	UINT16 id_bit_length;
	UINT32 curLenInByte;
	int i;

	curLenInByte = p_ecc_para->ECC_CurveLength * 4;
	
	id_bit_length = 8*id_len;	
	data_buf[0] = (UINT8)(id_bit_length >> 8);
	data_buf[1] = (UINT8)id_bit_length;
	
	//copy id after id_bit_length
	memcpy(data_buf+2, id, id_len);
		
	//copy ECC_a after id
	sm3_memcpy(temp, p_ecc_para->ECC_a, p_ecc_para->ECC_CurveLength);
	sm2_swap_array(temp, p_ecc_para->ECC_CurveLength);
	for(i = 0; i < p_ecc_para->ECC_CurveLength; i++)
	{
		 temp[i] = SM3_SWAP32(temp[i]);
	}
	memcpy(data_buf+2+id_len, (UINT8*)temp, curLenInByte);
		
	//copy ECC_b after ECC_a
	sm3_memcpy(temp, p_ecc_para->ECC_b, p_ecc_para->ECC_CurveLength);
	sm2_swap_array(temp, p_ecc_para->ECC_CurveLength);
	for(i = 0; i < p_ecc_para->ECC_CurveLength; i++)
	{
		 temp[i] = SM3_SWAP32(temp[i]);
	}	
	memcpy(data_buf+2+id_len+curLenInByte, (UINT8*)temp, curLenInByte);	
	
	//copy ECC_BaseX after ECC_b	
	sm3_memcpy(temp, p_ecc_para->ECC_BaseX, p_ecc_para->ECC_CurveLength);
	sm2_swap_array(temp, p_ecc_para->ECC_CurveLength);
	for(i = 0; i < p_ecc_para->ECC_CurveLength; i++)
	{
		 temp[i] = SM3_SWAP32(temp[i]);
	}
	memcpy(data_buf+2+id_len+2*curLenInByte, (UINT8*)temp, curLenInByte);
	
	//copy ECC_BaseY after ECC_BaseX
	sm3_memcpy(temp, p_ecc_para->ECC_BaseY, p_ecc_para->ECC_CurveLength);
	sm2_swap_array(temp, p_ecc_para->ECC_CurveLength);
	for(i = 0; i < p_ecc_para->ECC_CurveLength; i++)
	{
		 temp[i] = SM3_SWAP32(temp[i]);
	}
	memcpy(data_buf+2+id_len+3*curLenInByte, (UINT8*)temp, curLenInByte);
	
	//copy PubX after ECC_BaseY	
	memcpy(data_buf+2+id_len+4*curLenInByte, PubX, curLenInByte);
	
	//copy PubY after PubX
	memcpy(data_buf+2+id_len+5*curLenInByte, PubY, curLenInByte);
		
	sm3_hash(data_buf, 2+id_len+6*curLenInByte, id_Z);

	return SM2_SUCCEED;
}

static void GetE(UINT8 *pMessage, UINT32 msg_len, UINT8 *id_Z, UINT8 *pDigest)
{
	SM3_CTX context;
	
	SM3_initial(&context);
	SM3_update(&context, id_Z, 32);
	SM3_update(&context, pMessage, msg_len);
	SM3_final(pDigest, &context);
}

void sm2_curve_init(ECC_G_STR *p_ecc_para)
{
	static UINT8 bIsCurveInited = FALSE;

	if(!bIsCurveInited)
	{
		bIsCurveInited = TRUE;

		sm2_swap_array(N_Array, SM2_CURVELEN_INWORD);
	    sm2_swap_array(a_Array, SM2_CURVELEN_INWORD);
	    sm2_swap_array(b_Array, SM2_CURVELEN_INWORD);
	    sm2_swap_array(P_Array, SM2_CURVELEN_INWORD);
	    sm2_swap_array(BaseX_Array, SM2_CURVELEN_INWORD);
	    sm2_swap_array(BaseY_Array, SM2_CURVELEN_INWORD);
	}

	ECC_para_initial(p_ecc_para, SM2_CURVELEN_INWORD, P_Array, a_Array, b_Array, N_Array, BaseX_Array, BaseY_Array);
}

void sm2_get_digist(ECC_G_STR *p_ecc_para, UINT8 *id, UINT16 id_len, UINT8 *msg, UINT32 msg_len, UINT8 *PubX, UINT8 *PubY, UINT8 *pDigest)
{
	UINT8 id_Z[SM2_CURVELEN_INBYTE];

	//generate id_Z
	GetZ(p_ecc_para, id, id_len, PubX, PubY, id_Z);			
	//generate digest
	GetE(msg, msg_len, id_Z, pDigest);
}

UINT8 IP_SM2_event(CRYPT_SM2 *crypt_sm2)
{
	static ECC_G_STR ecc_glb_str;
	MATH_G_STR math_glb_str;
	SM2_CRYPT_CTX sm2_ctx;
	SM3_CTX sm3_ctx;
	UINT8 *crypt_id;
	UINT8 *crypt_msg;
	UINT8 *signR, *signS;
	UINT8 *sm2C1, *sm2C2, *sm2C3;
	UINT8 *cryptData;
	UINT8 *privKey, *pubKeyX, *pubKeyY;
	UINT8 digest[SM2_CURVELEN_INBYTE];
	UINT32 *QKeyX, *QKeyY;

	QKeyX = QKey;
	QKeyY = QKey + SM2_CURVELEN_INWORD;
	switch(crypt_sm2->cmd)
	{
		case CMD_SM2_KEYGEN:
			sm2_curve_init(&ecc_glb_str);

			//generate key
			memset((UINT8*)QKeyX, 0x00, SM2_CURVELEN_INBYTE);
			memset((UINT8*)QKeyY, 0x00, SM2_CURVELEN_INBYTE);
			ECDSA_keypair(&ecc_glb_str, dKey, QKeyX, QKeyY);

			//check if pubkey on the curve or not
			if(ECC_PJ(&ecc_glb_str, QKeyX, QKeyY))
			{
				return SM2_FAILED;
			}

			//transform generated little-endian dKey into big-endian for output.
			reverse_memory((UINT8*)dKey, SM2_CURVELEN_INBYTE);
			//transform generated little-endian QKeyX into big-endian for output.
			reverse_memory((UINT8*)QKeyX, SM2_CURVELEN_INBYTE);
			//transform generated little-endian QKeyY into big-endian for output.
			reverse_memory((UINT8*)QKeyY, SM2_CURVELEN_INBYTE);

			//if crypt_sm2->keyLenInByte != 0, return keys, else return nothing.
			if(crypt_sm2->keyLenInByte != 0)
			{
				memcpy(crypt_sm2->cryptOutBuf, (UINT8*)dKey, SM2_CURVELEN_INBYTE);
				memcpy(crypt_sm2->cryptOutBuf + SM2_CURVELEN_INBYTE, (UINT8*)QKeyX, SM2_CURVELEN_INBYTE);
				memcpy(crypt_sm2->cryptOutBuf + 2*SM2_CURVELEN_INBYTE, (UINT8*)QKeyY, SM2_CURVELEN_INBYTE);

				crypt_sm2->cryptOutLenInByte = 3 * SM2_CURVELEN_INBYTE;
			}
			else
			{
				crypt_sm2->cryptOutBuf[0] = SM2_SUCCEED;
				crypt_sm2->cryptOutLenInByte = 1;
			}
			break;
		case CMD_SM2_SIGN:
			//if crypt_sm2->keyLenInByte != 0, use received keys, else use generated keys.
			if(crypt_sm2->keyLenInByte != 0)
			{
				privKey = crypt_sm2->keyBuf;
				pubKeyX = privKey + SM2_CURVELEN_INBYTE;
				pubKeyY = pubKeyX + SM2_CURVELEN_INBYTE;
			}
			else
			{
				privKey = (UINT8*)dKey;
				pubKeyX = (UINT8*)QKeyX;
				pubKeyY = (UINT8*)QKeyY;
			}

			crypt_id = crypt_sm2->cryptInBuf;			   
			crypt_msg = crypt_id + crypt_sm2->idLenInByte;
						
			//generate digest
			sm2_get_digist(&ecc_glb_str, crypt_id, crypt_sm2->idLenInByte, crypt_msg, crypt_sm2->msgLenInByte, pubKeyX, pubKeyY, digest);
		  
			//generate signature
			signR = crypt_sm2->cryptInBuf;
			signS = signR + SM2_CURVELEN_INBYTE;
			if(sm2_sign(&ecc_glb_str, &math_glb_str, digest, privKey, signR, signS, SM2_NORMAL))
			{
				return SM2_FAILED;
			}

			crypt_sm2->cryptOutLenInByte = 2 * SM2_CURVELEN_INBYTE;
			memcpy(crypt_sm2->cryptOutBuf, signR, crypt_sm2->cryptOutLenInByte);
			break;
		case CMD_SM2_VERIFY:
			//if crypt_sm2->keyLenInByte != 0, use received keys, else use generated keys.
			if(crypt_sm2->keyLenInByte != 0)
			{
				pubKeyX = crypt_sm2->keyBuf;
				pubKeyY = pubKeyX + SM2_CURVELEN_INBYTE;
			}
			else
			{
				pubKeyX = (UINT8*)QKeyX;
				pubKeyY = (UINT8*)QKeyY;
			}

			crypt_id = crypt_sm2->cryptInBuf;			   
			crypt_msg = crypt_id + crypt_sm2->idLenInByte;
			signR = crypt_msg + crypt_sm2->msgLenInByte;
			signS = signR + SM2_CURVELEN_INBYTE;
						
			//generate digest
			sm2_get_digist(&ecc_glb_str, crypt_id, crypt_sm2->idLenInByte, crypt_msg, crypt_sm2->msgLenInByte, pubKeyX, pubKeyY, digest);

			//verify signature
			if(sm2_verify(&ecc_glb_str, &math_glb_str, digest, pubKeyX, signR, signS))
			{
				return SM2_FAILED;
			}

			crypt_sm2->cryptOutBuf[0] = SM2_SUCCEED;
			crypt_sm2->cryptOutLenInByte = 1;
			break;
		case CMD_SM2_ENCRYPT:
			//if crypt_sm2->keyLenInByte != 0, use received keys, else use generated keys.
			if(crypt_sm2->keyLenInByte != 0)
			{
				pubKeyX = crypt_sm2->keyBuf;
				pubKeyY = pubKeyX + SM2_CURVELEN_INBYTE;
			}
			else
			{
				pubKeyX = (UINT8*)QKeyX;
				pubKeyY = (UINT8*)QKeyY;
			}

			sm2C1 = crypt_sm2->cryptInBuf + crypt_sm2->msgLenInByte;
			sm2C2 = sm2C1 + 2*SM2_CURVELEN_INBYTE;
			sm2C3 = sm2C2 + crypt_sm2->msgLenInByte;

			//encrypt
			if(SM2_SUCCESS != sm2_encrypt(&ecc_glb_str, &sm2_ctx, &sm3_ctx, crypt_sm2->cryptInBuf, crypt_sm2->msgLenInByte, pubKeyX, sm2C1, sm2C2, sm2C3))
			{
				return SM2_FAILED;
			}

			crypt_sm2->cryptOutLenInByte = 3*SM2_CURVELEN_INBYTE + crypt_sm2->msgLenInByte;
			memcpy(crypt_sm2->cryptOutBuf, sm2C1, crypt_sm2->cryptOutLenInByte);
			break;
		case CMD_SM2_DECRYPT:
			//if crypt_sm2->keyLenInByte != 0, use received keys, else use generated keys.
			if(crypt_sm2->keyLenInByte != 0)
			{
				privKey = crypt_sm2->keyBuf;
			}
			else
			{
				privKey = (UINT8*)dKey;
			}

			//cryptData used to buffer plaintext decrypted from plaintext.
			cryptData = crypt_sm2->cryptInBuf + crypt_sm2->msgLenInByte;
			crypt_sm2->msgLenInByte -= 3*SM2_CURVELEN_INBYTE;
			
			sm2C1 = crypt_sm2->cryptInBuf;
			sm2C2 = sm2C1 + 2*SM2_CURVELEN_INBYTE;
			sm2C3 = sm2C2 + crypt_sm2->msgLenInByte;

			//decrypt
			if(SM2_SUCCESS != sm2_decrypt(&ecc_glb_str, &sm2_ctx, &sm3_ctx, privKey, sm2C1, sm2C2, sm2C3, crypt_sm2->msgLenInByte, cryptData, SM2_NORMAL))
			{
				return SM2_FAILED;
			}

			crypt_sm2->cryptOutLenInByte = crypt_sm2->msgLenInByte;
			memcpy(crypt_sm2->cryptOutBuf, cryptData, crypt_sm2->cryptOutLenInByte);
			break;
		default:
			return SM2_FAILED;
	}

	return SM2_SUCCEED;
}
