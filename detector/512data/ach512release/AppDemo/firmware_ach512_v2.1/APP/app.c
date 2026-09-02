/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : app.c
 * Description : application example source file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/

#include  "common.h"
#include  "usb.h"
#include  "app.h"
#include  "usb_app.h"
#include  "sm1_app.h"
#include  "sm4_app.h"
#include  "ssf33_app.h"
#include  "aes_app.h"
#include  "des_app.h"
#include  "sm2_app.h"
#include  "sm3_app.h"
#include  "rsa_app.h"
#include  "hrng_app.h"
#include  "iso7816_app.h"

UINT8 USB_CMD_proc(UINT8 ep_index)
{
	CRYPT_REQUEST crypt_req;
	UINT32 length;
	UINT32 fifo_len;
	UINT32 crypt_length;
	CRYPT_SM1 crypt_sm1;
	CRYPT_SM4 crypt_sm4;
	CRYPT_SSF33 crypt_ssf33;
	CRYPT_AES crypt_aes;
	CRYPT_DES crypt_des;
	CRYPT_SM2 crypt_sm2;
	CRYPT_SM3 crypt_sm3;
	CRYPT_RSA crypt_rsa;
	CRYPT_HRNG crypt_hrng;
	CRYPT_ISO7816 crypt_iso7816;
	UINT32 resp_len = 0;

   	length = usb_get_fifo_length(ep_index);
 	if(length < 8)	
	{
     	#ifdef DEBUG
		    printf("fifo length<8 \n");
    	#endif  
	
//		usb_clear_fifo(out_ep_index, EP_DIR_OUT);
//		usb_send_stall(in_ep_index, EP_DIR_IN);
		return 1;
	}	
	else
	{ 
		usb_read_ep_fifo((UINT8 *)(&crypt_req), 8, ep_index);
	}

	if((crypt_req.IP_code == CMD_SM2) || (crypt_req.IP_code == CMD_SM3))
	{
		crypt_length = ((crypt_req.crypt_length >> 16) & 0xFFFF) + (crypt_req.crypt_length & 0xFFFF);
		if(crypt_req.cmd_code == CMD_SM2_VERIFY)
		{
			crypt_length += 2*SM2_CURVELEN_INBYTE;
		}
	}
	else if((crypt_req.IP_code == CMD_HRNG) && (crypt_req.cmd_code == CMD_HRNG_GET_HRNGDATA))
	{
		crypt_length = 0;  //crypt_req.crypt_length in CMD_HRNG_GET_HRNGDATA means datalen will be returned.
	}
	else
	{
		crypt_length = crypt_req.crypt_length;
	}

	//usb receive data
	(REG_USBC_UCSR & 0x10) ? (fifo_len = EPX_MAX_PACKET_SIZE_HS) : (fifo_len = EPX_MAX_PACKET_SIZE_FS);
	if((8 + crypt_req.key_length + crypt_length) > fifo_len)
	{
		usb_read_ep_fifo(&DATABUF[0], (fifo_len - 8), ep_index);
		usb_receive_data_dynamic(&DATABUF[fifo_len - 8], crypt_length+crypt_req.key_length-(fifo_len-8), ep_index);
	}
	else
	{
		usb_read_ep_fifo(&DATABUF[0], crypt_req.key_length + crypt_length, ep_index);
	}

	switch(crypt_req.IP_code)
	{
		case CMD_SM1:			
			crypt_sm1.cmd = crypt_req.cmd_code;
			crypt_sm1.keyLenInWord = crypt_req.key_length/4;
			crypt_sm1.cryptInLenInWord = crypt_req.crypt_length/4;
			crypt_sm1.keyBuf = (UINT32*)DATABUF;
			crypt_sm1.cryptInBuf = crypt_sm1.keyBuf + crypt_sm1.keyLenInWord;
			crypt_sm1.cryptOutBuf = (UINT32*)DATABUF;
		
		#ifdef LOW_POWER
			enable_module(BIT_UAC | BIT_SM1 | BIT_HRNG | BIT_HRNGS | BIT_EMW);
		#endif
			if(SM1_SUCCEED == IP_SM1_event(&crypt_sm1))
			{
				resp_len = 4*crypt_sm1.cryptOutLenInWord;
			}
			else
			{
				DATABUF[0] = SM1_FAILED;
				resp_len = 1;
			}
		#ifdef LOW_POWER
			disable_module(BIT_UAC | BIT_SM1 | BIT_HRNG | BIT_HRNGS | BIT_EMW);
		#endif
			break;
		case CMD_SM4:			
			crypt_sm4.cmd = crypt_req.cmd_code;
			crypt_sm4.keyLenInWord = crypt_req.key_length/4;
			crypt_sm4.cryptInLenInWord = crypt_req.crypt_length/4;
			crypt_sm4.keyBuf = (UINT32*)DATABUF;
			crypt_sm4.cryptInBuf = crypt_sm4.keyBuf + crypt_sm4.keyLenInWord;
			crypt_sm4.cryptOutBuf = (UINT32*)DATABUF;
		
		#ifdef LOW_POWER
			enable_module(BIT_UAC | BIT_SM4 | BIT_HRNG | BIT_HRNGS | BIT_EMW);
		#endif
			if(SM4_SUCCEED == IP_SM4_event(&crypt_sm4))
			{
				resp_len = 4*crypt_sm4.cryptOutLenInWord;
			}
			else
			{
				DATABUF[0] = SM4_FAILED;
				resp_len = 1;
			}
		#ifdef LOW_POWER
			disable_module(BIT_UAC | BIT_SM4 | BIT_HRNG | BIT_HRNGS | BIT_EMW);
		#endif
			break;
		case CMD_SSF33:			
			crypt_ssf33.cmd = crypt_req.cmd_code;
			crypt_ssf33.keyLenInWord = crypt_req.key_length/4;
			crypt_ssf33.cryptInLenInWord = crypt_req.crypt_length/4;
			crypt_ssf33.keyBuf = (UINT32*)DATABUF;
			crypt_ssf33.cryptInBuf = crypt_ssf33.keyBuf + crypt_ssf33.keyLenInWord;
			crypt_ssf33.cryptOutBuf = (UINT32*)DATABUF;
		
		#ifdef LOW_POWER
			enable_module(BIT_UAC | BIT_SSF33 |BIT_HRNG | BIT_HRNGS | BIT_EMW);
		#endif
			if(SSF33_SUCCEED == IP_SSF33_event(&crypt_ssf33))
			{
				resp_len = 4*crypt_ssf33.cryptOutLenInWord;
			}
			else
			{
				DATABUF[0] = SSF33_FAILED;
				resp_len = 1;
			}
		#ifdef LOW_POWER
			disable_module(BIT_UAC | BIT_SSF33 | BIT_HRNG | BIT_HRNGS | BIT_EMW);
		#endif
			break;
		case CMD_AES:			
			crypt_aes.cmd = crypt_req.cmd_code;
			crypt_aes.keyLenInWord = crypt_req.key_length/4;
			crypt_aes.cryptInLenInWord = crypt_req.crypt_length/4;
			crypt_aes.keyBuf = (UINT32*)DATABUF;
			crypt_aes.cryptInBuf = crypt_aes.keyBuf + crypt_aes.keyLenInWord;
			crypt_aes.cryptOutBuf = (UINT32*)DATABUF;
		
		#ifdef LOW_POWER
			enable_module(BIT_UAC | BIT_AES | BIT_HRNG | BIT_HRNGS | BIT_EMW); 
		#endif
			if(AES_SUCCEED == IP_AES_event(&crypt_aes))
			{
				resp_len = 4*crypt_aes.cryptOutLenInWord;
			}
			else
			{
				DATABUF[0] = AES_FAILED;
				resp_len = 1;
			}
		#ifdef LOW_POWER
			disable_module(BIT_UAC | BIT_AES | BIT_HRNG | BIT_HRNGS | BIT_EMW); 
		#endif
			break;
		case CMD_DES:			
			crypt_des.cmd = crypt_req.cmd_code;
			crypt_des.keyLenInWord = crypt_req.key_length/4;
			crypt_des.cryptInLenInWord = crypt_req.crypt_length/4;
			crypt_des.keyBuf = (UINT32*)DATABUF;
			crypt_des.cryptInBuf = crypt_des.keyBuf + crypt_des.keyLenInWord;
			crypt_des.cryptOutBuf = (UINT32*)DATABUF;
		
		#ifdef LOW_POWER
			enable_module(BIT_UAC | BIT_DES | BIT_HRNG | BIT_HRNGS | BIT_EMW); 
		#endif
			if(DES_SUCCEED == IP_DES_event(&crypt_des))
			{
				resp_len = 4*crypt_des.cryptOutLenInWord;
			}
			else
			{
				DATABUF[0] = DES_FAILED;
				resp_len = 1;
			}
		#ifdef LOW_POWER
			disable_module(BIT_UAC | BIT_DES | BIT_HRNG | BIT_HRNGS | BIT_EMW); 
		#endif
			break;
		case CMD_SM2:
			crypt_sm2.cmd = crypt_req.cmd_code;
			crypt_sm2.keyLenInByte = crypt_req.key_length;
			//high 16bit store id length, low 16bit store msg length
			crypt_sm2.msgLenInByte = crypt_req.crypt_length & 0xFFFF;
			crypt_sm2.idLenInByte = (crypt_req.crypt_length >> 16) & 0xFFFF;

			crypt_sm2.keyBuf = DATABUF;
			crypt_sm2.cryptInBuf = crypt_sm2.keyBuf + crypt_sm2.keyLenInByte;
			crypt_sm2.cryptOutBuf = DATABUF;
		
		#ifdef LOW_POWER
			enable_module(BIT_UAC | BIT_PKI | BIT_HRNG | BIT_HASH | BIT_HRNGS); 
		#endif
			if(SM2_SUCCEED == IP_SM2_event(&crypt_sm2))
			{
				resp_len = crypt_sm2.cryptOutLenInByte;
			}
			else
			{
				DATABUF[0] = SM2_FAILED;
				resp_len = 1;
			}
		#ifdef LOW_POWER
			disable_module(BIT_UAC | BIT_PKI | BIT_HRNG | BIT_HASH | BIT_HRNGS); 
		#endif
			break;
		case CMD_SM3:
			crypt_sm3.cmd = crypt_req.cmd_code;
			//high 16bit store id length, low 16bit store msg length
			crypt_sm3.msgLenInByte = crypt_req.crypt_length & 0xFFFF;
			crypt_sm3.idLenInByte = (crypt_req.crypt_length >> 16) & 0xFFFF;

			crypt_sm3.keyBuf = DATABUF;
			crypt_sm3.cryptInBuf = crypt_sm3.keyBuf + crypt_req.key_length;
			crypt_sm3.cryptOutBuf = DATABUF;
		
		#ifdef LOW_POWER
			enable_module(BIT_UAC | BIT_PKI | BIT_HRNG | BIT_HASH | BIT_HRNGS); 
		#endif
			if(SM3_SUCCEED == IP_SM3_event(&crypt_sm3))
			{
				resp_len = crypt_sm3.cryptOutLenInByte;
			}
			else
			{
				DATABUF[0] = SM3_FAILED;
				resp_len = 1;
			}
		#ifdef LOW_POWER
			disable_module(BIT_UAC | BIT_PKI | BIT_HRNG | BIT_HASH | BIT_HRNGS); 
		#endif
			break;
		case CMD_RSA:
			crypt_rsa.cmd = crypt_req.cmd_code;
			crypt_rsa.keyLenInWord = crypt_req.key_length/4;
			crypt_rsa.cryptInLenInWord = (crypt_req.crypt_length+3)/4;    //must make sure length of cryptIn word-aligned.
			crypt_rsa.keyBuf = (UINT32*)DATABUF;
			crypt_rsa.cryptInBuf = crypt_rsa.keyBuf + crypt_rsa.keyLenInWord;
			crypt_rsa.cryptOutBuf = (UINT32*)DATABUF;
		
		#ifdef LOW_POWER
			enable_module(BIT_UAC | BIT_PKI | BIT_HRNG | BIT_HRNGS); 
		#endif
			if(RSA_SUCCEED == IP_RSA_event(&crypt_rsa))
			{
				resp_len = 4*crypt_rsa.cryptOutLenInWord;
			}
			else
			{
				DATABUF[0] = RSA_FAILED;
				resp_len = 1;
			}
		#ifdef LOW_POWER
			disable_module(BIT_UAC | BIT_PKI | BIT_HRNG | BIT_HRNGS);
		#endif
			break;
		case CMD_HRNG:
			crypt_hrng.cmd = crypt_req.cmd_code;
			crypt_hrng.cryptInLenInByte = crypt_req.crypt_length;
			crypt_hrng.cryptInBuf = DATABUF;
			crypt_hrng.cryptOutBuf = DATABUF;
		
		#ifdef LOW_POWER
			enable_module(BIT_UAC | BIT_HRNG | BIT_HRNGS); //enable HRNG
		#endif
			if(HRNG_SUCCEED == IP_HRNG_event(&crypt_hrng))
			{
				resp_len = crypt_hrng.cryptOutLenInByte;
			}
			else
			{
				DATABUF[0] = HRNG_FAILED;
				resp_len = 1;
			}
		#ifdef LOW_POWER
			disable_module(BIT_UAC | BIT_HRNG | BIT_HRNGS); //disable HRNG
		#endif
			break;
		case CMD_ISO7816:
			crypt_iso7816.cmd = crypt_req.cmd_code;
			crypt_iso7816.reqLen = crypt_req.key_length;
			crypt_iso7816.reqBuf = DATABUF;
			crypt_iso7816.respBuf = DATABUF;
			if(ISO7816_SUCCEED == IP_ISO7816_event(&crypt_iso7816))
			{
				usb_send_data((UINT8*)&crypt_iso7816.respLen, 1, ep_index);
				resp_len = crypt_iso7816.respLen;
			}
			else
			{
				DATABUF[0] = ISO7816_FAILED;
				resp_len = 1;
			}
			break;
		default:
			usb_send_stall(ep_index, EP_DIR_IN);
			return 1;
	}

	REG_USBC_RxCSR &= (~0x01);     //命令处理结束后，再释放，以防止Buffer被接下来的数据包覆盖

	if(resp_len > 0)
	{
		usb_send_data_dynamic(DATABUF, resp_len, ep_index);
	}
	
	return 0;
} 
