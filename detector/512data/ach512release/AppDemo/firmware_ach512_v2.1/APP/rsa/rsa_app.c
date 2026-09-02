/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : rsa_app.c
 * Description : rsa application demo
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#include  "rsa_app.h"

static UINT32 rsaKeyBuf[11*HALF_NDIGITS];

static void lib_variable_initial(RSA_KEYGEN_G_STR *p_rsa_keygen_str)
{
	p_rsa_keygen_str->RSA_n    = (rsaKeyBuf);
	p_rsa_keygen_str->RSA_d    = (rsaKeyBuf + (NDIGITS));
	p_rsa_keygen_str->RSA_e    = (rsaKeyBuf + 2 * (NDIGITS));
	p_rsa_keygen_str->RSA_p    = (rsaKeyBuf + 3 * (NDIGITS));
	p_rsa_keygen_str->RSA_q    = (rsaKeyBuf + 3 * (NDIGITS)+(HALF_NDIGITS));
	p_rsa_keygen_str->RSA_dp   = (rsaKeyBuf + 4 * (NDIGITS));
	p_rsa_keygen_str->RSA_dq   = (rsaKeyBuf + 4 * (NDIGITS)+(HALF_NDIGITS));
	p_rsa_keygen_str->RSA_qInv = (rsaKeyBuf + 5 * (NDIGITS));
}

UINT8 IP_RSA_event(CRYPT_RSA *crypt_rsa)
{
	RSA_G_STR rsa_glb_str;
	MATH_G_STR math_glb_str;
	RSA_KEYGEN_G_STR rsa_keygen_glb_str;
	UINT32 ndigits;
	UINT32 temp;
	UINT8  bIsCRTMode;
	int i;

	UINT8 (*p_rsa_keygen)(RSA_KEYGEN_G_STR *p_rsa_keygen_str, MATH_G_STR *p_math_str, UINT8 nDigits);

	switch(crypt_rsa->cmd & CMD_RSA_MODE_MASK)
	{
		case CMD_RSA_1024B:
			ndigits = RSA_1024B_INWORD;
			bIsCRTMode = FALSE;
			break;
		case CMD_RSA_1024B_CRT:
			ndigits = RSA_1024B_INWORD;
			bIsCRTMode = TRUE;
			break;
		case CMD_RSA_2048B:
			ndigits = RSA_2048B_INWORD;
			bIsCRTMode = FALSE;
			break;
		case CMD_RSA_2048B_CRT:
			ndigits = RSA_2048B_INWORD;
			bIsCRTMode = TRUE;
			break;
		default:
			return RSA_FAILED;
	}

	switch(crypt_rsa->cmd & CMD_RSA_OPMODE_MASK)
	{
		case CMD_RSA_KEYGEN:
			lib_variable_initial(&rsa_keygen_glb_str);

			NN_AssignZero(rsa_keygen_glb_str.RSA_e, ndigits);
			//transform received big-endian RSA_e into little-endian required.
			reverse_memory((UINT8*)crypt_rsa->cryptInBuf, 4*crypt_rsa->cryptInLenInWord);
			for(i = 0; i < crypt_rsa->cryptInLenInWord; i++)
			{
				rsa_keygen_glb_str.RSA_e[i] = crypt_rsa->cryptInBuf[i];
			}

			bIsCRTMode ? (p_rsa_keygen = RSA_keygen_CRT) : (p_rsa_keygen = RSA_keygen);
			if(p_rsa_keygen(&rsa_keygen_glb_str, &math_glb_str, ndigits))
			{
				return RSA_FAILED;
			}

			//if crypt_rsa->keyLenInWord != 0, return keys, else return nothing.
			if(crypt_rsa->keyLenInWord != 0)
			{
				temp = 0;
				memcpy((UINT8*)&crypt_rsa->cryptOutBuf[temp], (UINT8*)rsa_keygen_glb_str.RSA_n, 4*ndigits);
				//transform generated little-endian RSA_n into big-endian for output.
				reverse_memory((UINT8*)&crypt_rsa->cryptOutBuf[temp], 4*ndigits);
				temp += ndigits;

				memcpy((UINT8*)&crypt_rsa->cryptOutBuf[temp], (UINT8*)rsa_keygen_glb_str.RSA_d, 4*ndigits);
				//transform generated little-endian RSA_d into big-endian for output.
				reverse_memory((UINT8*)&crypt_rsa->cryptOutBuf[temp], 4*ndigits);
				temp += ndigits;

				if(bIsCRTMode)
				{
					ndigits /= 2;

					memcpy((UINT8*)&crypt_rsa->cryptOutBuf[temp], (UINT8*)rsa_keygen_glb_str.RSA_p, 4*ndigits);
					//transform generated little-endian RSA_p into big-endian for output.
					reverse_memory((UINT8*)&crypt_rsa->cryptOutBuf[temp], 4*ndigits);
					temp += ndigits;

					memcpy((UINT8*)&crypt_rsa->cryptOutBuf[temp], (UINT8*)rsa_keygen_glb_str.RSA_q, 4*ndigits);
					//transform generated little-endian RSA_q into big-endian for output.
					reverse_memory((UINT8*)&crypt_rsa->cryptOutBuf[temp], 4*ndigits);
					temp += ndigits;

					memcpy((UINT8*)&crypt_rsa->cryptOutBuf[temp], (UINT8*)rsa_keygen_glb_str.RSA_dp, 4*ndigits);
					//transform generated little-endian RSA_dp into big-endian for output.
					reverse_memory((UINT8*)&crypt_rsa->cryptOutBuf[temp], 4*ndigits);
					temp += ndigits;

					memcpy((UINT8*)&crypt_rsa->cryptOutBuf[temp], (UINT8*)rsa_keygen_glb_str.RSA_dq, 4*ndigits);
					//transform generated little-endian RSA_dq into big-endian for output.
					reverse_memory((UINT8*)&crypt_rsa->cryptOutBuf[temp], 4*ndigits);
					temp += ndigits;

					memcpy((UINT8*)&crypt_rsa->cryptOutBuf[temp], (UINT8*)rsa_keygen_glb_str.RSA_qInv, 4*ndigits);
					//transform generated little-endian RSA_qInv into big-endian for output.
					reverse_memory((UINT8*)&crypt_rsa->cryptOutBuf[temp], 4*ndigits);
					temp += ndigits;
				}

				crypt_rsa->cryptOutLenInWord = temp;
			}
			else
			{
				crypt_rsa->cryptOutBuf[0] = RSA_SUCCEED;
				crypt_rsa->cryptOutLenInWord = 1;
			}
			break;
		case CMD_RSA_ENC:
			//if crypt_rsa->keyLenInWord != 0, use received keys, else use generated keys.
			if(crypt_rsa->keyLenInWord != 0)
			{
				rsa_keygen_glb_str.RSA_n = (crypt_rsa->keyBuf);
				//transform received big-endian RSA_n into little-endian required.
				reverse_memory((UINT8*)rsa_keygen_glb_str.RSA_n, 4*ndigits);

				rsa_keygen_glb_str.RSA_e = (crypt_rsa->keyBuf + (ndigits));
				//transform received big-endian RSA_e into little-endian required.
				reverse_memory((UINT8*)rsa_keygen_glb_str.RSA_e, 4*1);
			}
			else
			{
				lib_variable_initial(&rsa_keygen_glb_str);
			}

			//transform received big-endian plaintext into little-endian required.
			reverse_memory((UINT8*)crypt_rsa->cryptInBuf, 4*crypt_rsa->cryptInLenInWord);
			
			//encrypt
			crypt_rsa->cryptOutLenInWord = 0;
			if(rsa_mul_me(crypt_rsa->cryptInBuf, (UINT8)crypt_rsa->cryptInLenInWord, rsa_keygen_glb_str.RSA_e, 1, rsa_keygen_glb_str.RSA_n, (UINT8)ndigits, crypt_rsa->cryptOutBuf, (UINT8*)&crypt_rsa->cryptOutLenInWord, CNST_RSA_EXP))
			{
				return RSA_FAILED;
			}			
			//get vailed length of ciphertext, in words.
			crypt_rsa->cryptOutLenInWord = NN_Digits(crypt_rsa->cryptOutBuf, crypt_rsa->cryptOutLenInWord);

			if(crypt_rsa->cryptOutLenInWord < crypt_rsa->cryptInLenInWord)
			{
				memset(&crypt_rsa->cryptOutBuf[crypt_rsa->cryptOutLenInWord], 0x00, 4*(crypt_rsa->cryptInLenInWord - crypt_rsa->cryptOutLenInWord));
				crypt_rsa->cryptOutLenInWord = crypt_rsa->cryptInLenInWord;
			}

			//transform encrypted little-endian ciphertext into big-endian for output.
			reverse_memory((UINT8*)crypt_rsa->cryptOutBuf, 4*crypt_rsa->cryptOutLenInWord);
			break;
		case CMD_RSA_DEC:
			//if crypt_rsa->keyLenInWord != 0, use received keys, else use generated keys.
			if(crypt_rsa->keyLenInWord != 0)
			{
				rsa_keygen_glb_str.RSA_n    = (crypt_rsa->keyBuf);
				//transform received big-endian RSA_n into little-endian required.
				reverse_memory((UINT8*)rsa_keygen_glb_str.RSA_n, 4*ndigits);

				rsa_keygen_glb_str.RSA_d    = (crypt_rsa->keyBuf + (ndigits));
				//transform received big-endian RSA_d into little-endian required.
				reverse_memory((UINT8*)rsa_keygen_glb_str.RSA_d, 4*ndigits);
				
				if(bIsCRTMode)
				{
					rsa_keygen_glb_str.RSA_e    = (crypt_rsa->keyBuf + 2 * (ndigits));
					//transform received big-endian RSA_e into little-endian required.
					reverse_memory((UINT8*)rsa_keygen_glb_str.RSA_e, 4*1);

					rsa_keygen_glb_str.RSA_p    = (crypt_rsa->keyBuf + 1 + 2 * (ndigits));
					//transform received big-endian RSA_p into little-endian required.
					reverse_memory((UINT8*)rsa_keygen_glb_str.RSA_p, 2*ndigits);

					rsa_keygen_glb_str.RSA_q    = (crypt_rsa->keyBuf + 1 + 2 * (ndigits) + (ndigits/2));
					//transform received big-endian RSA_q into little-endian required.
					reverse_memory((UINT8*)rsa_keygen_glb_str.RSA_q, 2*ndigits);

					rsa_keygen_glb_str.RSA_dp   = (crypt_rsa->keyBuf + 1 + 3 * (ndigits));
					//transform received big-endian RSA_dp into little-endian required.
					reverse_memory((UINT8*)rsa_keygen_glb_str.RSA_dp, 2*ndigits);

					rsa_keygen_glb_str.RSA_dq   = (crypt_rsa->keyBuf + 1 + 3 * (ndigits) + (ndigits/2));
					//transform received big-endian RSA_dq into little-endian required.
					reverse_memory((UINT8*)rsa_keygen_glb_str.RSA_dq, 2*ndigits);

					rsa_keygen_glb_str.RSA_qInv = (crypt_rsa->keyBuf + 1 + 4 * (ndigits));
					//transform received big-endian RSA_qInv into little-endian required.
					reverse_memory((UINT8*)rsa_keygen_glb_str.RSA_qInv, 2*ndigits);
				}
			}
			else
			{
				lib_variable_initial(&rsa_keygen_glb_str);
			}

			//transform received big-endian ciphertext into little-endian required.
			reverse_memory((UINT8*)crypt_rsa->cryptInBuf, 4*crypt_rsa->cryptInLenInWord);

			//decrypt
			crypt_rsa->cryptOutLenInWord = 0;
			if(bIsCRTMode)
			{
				if(rsa_decrypt_CRT(crypt_rsa->cryptInBuf, (UINT8)crypt_rsa->cryptInLenInWord, rsa_keygen_glb_str.RSA_p, (UINT8)ndigits/2, rsa_keygen_glb_str.RSA_q, (UINT8)ndigits/2, rsa_keygen_glb_str.RSA_dp, (UINT8)ndigits/2, rsa_keygen_glb_str.RSA_dq, (UINT8)ndigits/2, rsa_keygen_glb_str.RSA_qInv, (UINT8)ndigits/2, crypt_rsa->cryptOutBuf, (UINT8*)&crypt_rsa->cryptOutLenInWord, &rsa_glb_str, &math_glb_str, rsa_keygen_glb_str.RSA_e, 1, RSA_NORMAL))
				{
					return RSA_FAILED;
				}
			}
			else
			{
				if(rsa_mul_me(crypt_rsa->cryptInBuf, (UINT8)crypt_rsa->cryptInLenInWord, rsa_keygen_glb_str.RSA_d, (UINT8)ndigits, rsa_keygen_glb_str.RSA_n, (UINT8)ndigits, crypt_rsa->cryptOutBuf, (UINT8*)&crypt_rsa->cryptOutLenInWord, CNST_RSA_EXP))
				{
					return RSA_FAILED;
				}
				//get vailed length of plaintext, in words.
				crypt_rsa->cryptOutLenInWord = NN_Digits(crypt_rsa->cryptOutBuf, crypt_rsa->cryptOutLenInWord);
			}

			if(crypt_rsa->cryptOutLenInWord < crypt_rsa->cryptInLenInWord)
			{
				memset(&crypt_rsa->cryptOutBuf[crypt_rsa->cryptOutLenInWord], 0x00, 4*(crypt_rsa->cryptInLenInWord - crypt_rsa->cryptOutLenInWord));
				crypt_rsa->cryptOutLenInWord = crypt_rsa->cryptInLenInWord;
			}

			//transform decrypted little-endian plaintext into big-endian for output.
			reverse_memory((UINT8*)crypt_rsa->cryptOutBuf, 4*crypt_rsa->cryptOutLenInWord);
			break;
		default:
			return RSA_FAILED;
	}

	return RSA_SUCCEED;
}
