/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : scsi.c
 * Description : ums scsi cmd process source file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#include  "common.h"
#include  "usb.h"
#include  "scsi.h"
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

static UINT8 *CryptBuf = (UINT8*)g_data_buf;
static UINT32 CryptBufLen = sizeof(g_data_buf);

static UINT8 gbScsiInEnp;
static UINT8 gbScsiOutEnp;	
MSCCMD ScsiCBW;
MSCCSW ScsiCSW =
{
	0x53425355, // USBS
	0x00,       // TAG
	0x00,       // Residue
	0x00        // status
};

const UINT8 SCSIInquiryData[36] = 
{
	0x00,	// Peripheral Device Type: direct access devices , 0x05表示光盘，
	0x80,	// Removable: UFD is removable,	0x00表示不显示盘符
	0x02,	// ANSI version
	0x02, 	// Response Data Format: compliance with UFI  
	0x1F,	// Additional Length (Number of unsigned chars following this one): 31, totally 36 unsigned chars	
	0x00, 0x00, 0x00,	// reserved
	'U',	// -- Vender information start
	'S',	//
	'B',	//
	' ',	//
	' ',	//
	' ', 	//
	' ',	//
	' ',    //-- Vend Information end 
	'F', 	// -- Production Identification start
	'l', 	//
	'a',	//
	's',	//
	'h',	//
	'D',	//
	'i',	//
	's',	//
	'k', 	//
	' ',	//
	' ',	//
	' ', 	//
	' ',	// 
	' ',	//	
	' ', 	//
	' ',	//-- Production Identification end
	0x31,	//"1" -- Production Revision Level start
	0x2e,	//"."	
	0x30,	//"0"
	0x30	//"0" -- Production Revision Level end
};

const UINT8 SCSIInquiryData_Nodev[36] = 
{
	0x00,	// Peripheral Device Type: direct access devices
	0x00,	// Removable: UFD is removable
	0x02,	// ANSI version
	0x02, 	// Response Data Format: compliance with UFI  
	0x1F,	// Additional Length (Number of unsigned chars following this one): 31, totally 36 unsigned chars	
	0x00, 0x00, 0x00,	// reserved
	'U',	// -- Vender information start
	'S',	//
	'B',	//
	' ',	//
	' ',	//
	' ', 	//
	' ',	//
	' ',    //-- Vend Information end 
	'A', 	// -- Production Identification start
	'i', 	//
	's',	//
	'i',	//
	'n',	//
	'o',	//
	'K',	//
	'e',	//
	'y', 	//
	' ',	//
	' ',	//
	' ', 	//
	' ',	// 
	' ',	//	
	' ', 	//
	' ',	//-- Production Identification end
	0x31,	//"1" -- Production Revision Level start
	0x2e,	//"."	
	0x30,	//"0"
	0x30	//"0" -- Production Revision Level end
};

const SENSE_CODE MscSenseCode[] = 
{
	{0x00,0x00,0x00},	/*NO_SENSE 0*/ 
	{0x05,0x24,0x00},	/*INVALID_FIELD_IN_COMMAND 1*/ 
	{0x06,0x28,0x00},	/*NEW_MEDIUM_ARRIVEL 2*/ 
	{0x07,0x27,0x00},	/*WRITE_PROTECTED 3*/ 
	{0x09,0x01,0x00},	/*PASSWORD_ERROR 4*/ 
	{0x09,0x00,0x00},	/*NOT_CONFIG 5*/ 
	{0x09,0x02,0x00},	/*USER_LOCKED 6*/ 
	{0x02,0x3A,0x00},	/*MEDIUM_NOT_PRESENT 7*/ 
	{0x02,0x04,0x01},	/*LOGDRIVER_NOT_READY 8*/  /*Repeat send the data*/
	{0x05,0x21,0x00},	/*ADDRESS_OUT_RANGE 9*/	
	{0x03,0x03,0x00},	/*WRITE FAULT 10*/	 /*将128个sector 拆成8个sector 发送*/
	{0x00,0x00,0x00},	/*CBW_PHASE_ERROR 11*/	 
	{0x03,0x11,0x00},	/*UNRECOVERED_READ_ERROR 12*/	 
	{0x02,0x53,0x00},	/*MEDIA REMOVAL PREVENTED 13*/	 
	{0x05,0x20,0x00},	/*INVALID_COMMAND_OPERATION_CODE 14*/  
	{0x04,0x00,0x00},   /*HARDWARE ERROR 15*/
	{0x09,0x02,0x00},   /*AppCos Crypt ERROR 16*/ 
};

static SENSE_CODE MscStatusCode;

static void PadData(void)
{
	UINT8 dummy[512];
	UINT32 length;

	memset(dummy, 0x00, 512);
	while(ScsiCBW.CbwTrxLength > 0)
	{
		if(ScsiCBW.CbwTrxLength < 512)
		{
			length = ScsiCBW.CbwTrxLength;
		}
		else
		{
			length = 512;
		}
		
		if(CBW_DIR_IN == ScsiCBW.CbwFlag)
		{
			usb_send_data(dummy, length, gbScsiInEnp);
		}
		else
		{
			usb_receive_data(dummy, length, gbScsiOutEnp);
		}

		ScsiCBW.CbwTrxLength -= length;
	}
}

static void mscSet_Status(UINT8 status)
{
	if(status != NO_SENSE) 
	{
		MscStatusCode = MscSenseCode[status];
		if (CBW_PHASE_ERROR == status)
		{
			ScsiCSW.Status = MSC_PHASE_ERROR;
		}
		else
		{
			ScsiCSW.Status = MSC_CMD_FAILED;
		}
	}
	else
	{
		ScsiCSW.Status = MSC_GOOD_STATUS;
	}
}

static void SendCSW(void)
{
	if(MSC_GOOD_STATUS == ScsiCSW.Status)
	{
	   MscStatusCode = MscSenseCode[NO_SENSE];
	   ScsiCSW.Residue = 0;
	}
	else 
	{
	   ScsiCSW.Residue = ScsiCBW.CbwTrxLength;
	}
	usb_send_data((UINT8*)&ScsiCSW, 13, gbScsiInEnp);
}

static void RBC_Inquiry(void)
{
	usb_send_data((UINT8*)&SCSIInquiryData, min(ScsiCBW.CbwTrxLength, sizeof(SCSIInquiryData)), gbScsiInEnp);
}

static void RBC_RequestSense(void)
{
	UINT8 sense[18];

	memset(sense, 0, 0x12);
	sense[0] = 0x70;
	sense[2] = MscStatusCode.SenseKey;
	sense[7] = 0x12-8;
	sense[12] = MscStatusCode.ASC;
	sense[13] = MscStatusCode.ASCQ;
	usb_send_data(sense, min(ScsiCBW.CbwTrxLength, 0x12), gbScsiInEnp);	
}

static void RBC_AppCos(void)
{
	if(CBW_DIR_IN == ScsiCBW.CbwFlag)
	{
		while(ScsiCBW.CbwTrxLength)
		{
			if(ScsiCBW.CbwTrxLength > CryptBufLen)
			{
				usb_send_data_dynamic(CryptBuf, CryptBufLen, gbScsiInEnp);
				ScsiCBW.CbwTrxLength -= CryptBufLen;
			}
			else
			{
				usb_send_data_dynamic(CryptBuf, ScsiCBW.CbwTrxLength, gbScsiInEnp);
				ScsiCBW.CbwTrxLength = 0;
			}
		}
	}
	else
	{
		while(ScsiCBW.CbwTrxLength)
		{
			if(ScsiCBW.CbwTrxLength > CryptBufLen)
			{
				usb_receive_data_dynamic(CryptBuf, CryptBufLen, gbScsiOutEnp);
				ScsiCBW.CbwTrxLength -= CryptBufLen;
			}
			else
			{
				usb_receive_data_dynamic(CryptBuf, ScsiCBW.CbwTrxLength, gbScsiOutEnp);
				ScsiCBW.CbwTrxLength = 0;
			}
		}
	}
}

static void RBC_Crypt(void)
{
	CRYPT_REQUEST crypt_req;
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
	static UINT32 resp_len = 0;

	if(CBW_DIR_IN == ScsiCBW.CbwFlag)
	{
		if((ScsiCBW.SubOpCode == CMD_ISO7816) && (ScsiCBW.CbwTrxLength == 1))
		{
			usb_send_data((UINT8*)&resp_len, 1, gbScsiInEnp);
		}
		else
		{
			usb_send_data_dynamic(CryptBuf, resp_len, gbScsiInEnp);
		}
		return;
	}

	crypt_req.IP_code = ScsiCBW.SubOpCode;
	crypt_req.cmd_code = (ScsiCBW.Address >> 24) & 0xFF;
	crypt_req.key_length = SWAP((ScsiCBW.Address >> 8) & 0xFFFF);
	crypt_req.crypt_length = (ScsiCBW.Address & 0xFF) | (ScsiCBW.Chip << 8) | (SWAP(ScsiCBW.Length) << 16);

	usb_receive_data_dynamic(CryptBuf, ScsiCBW.CbwTrxLength, gbScsiOutEnp);

	switch(crypt_req.IP_code)
	{
		case CMD_SM1:
			crypt_sm1.cmd = crypt_req.cmd_code;
			crypt_sm1.keyLenInWord = crypt_req.key_length/4;
			crypt_sm1.cryptInLenInWord = crypt_req.crypt_length/4;
			crypt_sm1.keyBuf = (UINT32*)CryptBuf;
			crypt_sm1.cryptInBuf = crypt_sm1.keyBuf + crypt_sm1.keyLenInWord;
			crypt_sm1.cryptOutBuf = (UINT32*)CryptBuf;
		
		#ifdef LOW_POWER
			enable_module(BIT_UAC | BIT_SM1 | BIT_HRNG | BIT_HRNGS | BIT_EMW);
		#endif
			if(SM1_SUCCEED == IP_SM1_event(&crypt_sm1))
			{
				resp_len = 4*crypt_sm1.cryptOutLenInWord;
			}
			else
			{
				mscSet_Status(APPCOS_CRYPT_ERROR);
			}
		#ifdef LOW_POWER
			disable_module(BIT_UAC | BIT_SM1 | BIT_HRNG | BIT_HRNGS | BIT_EMW);
		#endif
			break;
		case CMD_SM4:			
			crypt_sm4.cmd = crypt_req.cmd_code;
			crypt_sm4.keyLenInWord = crypt_req.key_length/4;
			crypt_sm4.cryptInLenInWord = crypt_req.crypt_length/4;
			crypt_sm4.keyBuf = (UINT32*)CryptBuf;
			crypt_sm4.cryptInBuf = crypt_sm4.keyBuf + crypt_sm4.keyLenInWord;
			crypt_sm4.cryptOutBuf = (UINT32*)CryptBuf;
		
		#ifdef LOW_POWER
			enable_module(BIT_UAC | BIT_SM4 | BIT_HRNG | BIT_HRNGS | BIT_EMW);
		#endif
			if(SM4_SUCCEED == IP_SM4_event(&crypt_sm4))
			{
				resp_len = 4*crypt_sm4.cryptOutLenInWord;
			}
			else
			{
				mscSet_Status(APPCOS_CRYPT_ERROR);
			}
		#ifdef LOW_POWER
			disable_module(BIT_UAC | BIT_SM4 | BIT_HRNG | BIT_HRNGS | BIT_EMW);
		#endif
			break;
		case CMD_SSF33:			
			crypt_ssf33.cmd = crypt_req.cmd_code;
			crypt_ssf33.keyLenInWord = crypt_req.key_length/4;
			crypt_ssf33.cryptInLenInWord = crypt_req.crypt_length/4;
			crypt_ssf33.keyBuf = (UINT32*)CryptBuf;
			crypt_ssf33.cryptInBuf = crypt_ssf33.keyBuf + crypt_ssf33.keyLenInWord;
			crypt_ssf33.cryptOutBuf = (UINT32*)CryptBuf;
		
		#ifdef LOW_POWER
			enable_module(BIT_UAC | BIT_SSF33 |BIT_HRNG | BIT_HRNGS | BIT_EMW);
		#endif
			if(SSF33_SUCCEED == IP_SSF33_event(&crypt_ssf33))
			{
				resp_len = 4*crypt_ssf33.cryptOutLenInWord;
			}
			else
			{
				mscSet_Status(APPCOS_CRYPT_ERROR);
			}
		#ifdef LOW_POWER
			disable_module(BIT_UAC | BIT_SSF33 | BIT_HRNG | BIT_HRNGS | BIT_EMW);
		#endif
			break;
		case CMD_AES:			
			crypt_aes.cmd = crypt_req.cmd_code;
			crypt_aes.keyLenInWord = crypt_req.key_length/4;
			crypt_aes.cryptInLenInWord = crypt_req.crypt_length/4;
			crypt_aes.keyBuf = (UINT32*)CryptBuf;
			crypt_aes.cryptInBuf = crypt_aes.keyBuf + crypt_aes.keyLenInWord;
			crypt_aes.cryptOutBuf = (UINT32*)CryptBuf;
		
		#ifdef LOW_POWER
			enable_module(BIT_UAC | BIT_AES | BIT_HRNG | BIT_HRNGS | BIT_EMW); 
		#endif
			if(AES_SUCCEED == IP_AES_event(&crypt_aes))
			{
				resp_len = 4*crypt_aes.cryptOutLenInWord;
			}
			else
			{
				mscSet_Status(APPCOS_CRYPT_ERROR);
			}
		#ifdef LOW_POWER
			disable_module(BIT_UAC | BIT_AES | BIT_HRNG | BIT_HRNGS | BIT_EMW); 
		#endif
			break;
		case CMD_DES:			
			crypt_des.cmd = crypt_req.cmd_code;
			crypt_des.keyLenInWord = crypt_req.key_length/4;
			crypt_des.cryptInLenInWord = crypt_req.crypt_length/4;
			crypt_des.keyBuf = (UINT32*)CryptBuf;
			crypt_des.cryptInBuf = crypt_des.keyBuf + crypt_des.keyLenInWord;
			crypt_des.cryptOutBuf = (UINT32*)CryptBuf;
		
		#ifdef LOW_POWER
			enable_module(BIT_UAC | BIT_DES | BIT_HRNG | BIT_HRNGS | BIT_EMW); 
		#endif
			if(DES_SUCCEED == IP_DES_event(&crypt_des))
			{
				resp_len = 4*crypt_des.cryptOutLenInWord;
			}
			else
			{
				mscSet_Status(APPCOS_CRYPT_ERROR);
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

			crypt_sm2.keyBuf = CryptBuf;
			crypt_sm2.cryptInBuf = crypt_sm2.keyBuf + crypt_sm2.keyLenInByte;
			crypt_sm2.cryptOutBuf = CryptBuf;
		
		#ifdef LOW_POWER
			enable_module(BIT_UAC | BIT_PKI | BIT_HRNG | BIT_HASH | BIT_HRNGS); 
		#endif
			if(SM2_SUCCEED == IP_SM2_event(&crypt_sm2))
			{
				resp_len = crypt_sm2.cryptOutLenInByte;
			}
			else
			{
				mscSet_Status(APPCOS_CRYPT_ERROR);
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

			crypt_sm3.keyBuf = CryptBuf;
			crypt_sm3.cryptInBuf = crypt_sm3.keyBuf + crypt_req.key_length;
			crypt_sm3.cryptOutBuf = CryptBuf;
		
		#ifdef LOW_POWER
			enable_module(BIT_UAC | BIT_PKI | BIT_HRNG | BIT_HASH | BIT_HRNGS); 
		#endif
			if(SM3_SUCCEED == IP_SM3_event(&crypt_sm3))
			{
				resp_len = crypt_sm3.cryptOutLenInByte;
			}
			else
			{
				mscSet_Status(APPCOS_CRYPT_ERROR);
			}
		#ifdef LOW_POWER
			disable_module(BIT_UAC | BIT_PKI | BIT_HRNG | BIT_HASH | BIT_HRNGS); 
		#endif
			break;
		case CMD_RSA:						
			crypt_rsa.cmd = crypt_req.cmd_code;
			crypt_rsa.keyLenInWord = crypt_req.key_length/4;
			crypt_rsa.cryptInLenInWord = (crypt_req.crypt_length+3)/4;    //must make sure length of cryptIn word-aligned.
			crypt_rsa.keyBuf = (UINT32*)CryptBuf;
			crypt_rsa.cryptInBuf = crypt_rsa.keyBuf + crypt_rsa.keyLenInWord;
			crypt_rsa.cryptOutBuf = (UINT32*)CryptBuf;
		
		#ifdef LOW_POWER
			enable_module(BIT_UAC | BIT_PKI | BIT_HRNG | BIT_HRNGS); 
		#endif
			if(RSA_SUCCEED == IP_RSA_event(&crypt_rsa))
			{
				resp_len = 4*crypt_rsa.cryptOutLenInWord;
			}
			else
			{
				mscSet_Status(APPCOS_CRYPT_ERROR);
			}
		#ifdef LOW_POWER
			disable_module(BIT_UAC | BIT_PKI | BIT_HRNG | BIT_HRNGS);
		#endif
			break;
		case CMD_HRNG:
			crypt_hrng.cmd = crypt_req.cmd_code;
			crypt_hrng.cryptInLenInByte = crypt_req.crypt_length;
			crypt_hrng.cryptInBuf = CryptBuf;
			crypt_hrng.cryptOutBuf = CryptBuf;
		
		#ifdef LOW_POWER
			enable_module(BIT_UAC | BIT_HRNG | BIT_HRNGS); //enable HRNG
		#endif
			if(HRNG_SUCCEED == IP_HRNG_event(&crypt_hrng))
			{
				resp_len = crypt_hrng.cryptOutLenInByte;
			}
			else
			{
				mscSet_Status(APPCOS_CRYPT_ERROR);
			}
		#ifdef LOW_POWER
			disable_module(BIT_UAC | BIT_HRNG | BIT_HRNGS); //disable HRNG
		#endif
			break;
		case CMD_ISO7816:
			crypt_iso7816.cmd = crypt_req.cmd_code;
			crypt_iso7816.reqLen = crypt_req.key_length;
			crypt_iso7816.reqBuf = CryptBuf;
			crypt_iso7816.respBuf = CryptBuf;
			if(ISO7816_SUCCEED == IP_ISO7816_event(&crypt_iso7816))
			{
				resp_len = crypt_iso7816.respLen;
			}
			else
			{
				mscSet_Status(APPCOS_CRYPT_ERROR);
			}
			break;
		default:
			mscSet_Status(INVALID_COMMAND_OPERATION_CODE);
			break;
	}
}

static void RxCBW(void)
{
	usb_read_ep_fifo((UINT8 *)&ScsiCBW,17,gbScsiOutEnp);
	usb_read_ep_fifo((UINT8 *)&(ScsiCBW.Address),4,gbScsiOutEnp);
	usb_read_ep_fifo((UINT8 *)&(ScsiCBW.Chip),1,gbScsiOutEnp);
	usb_read_ep_fifo((UINT8 *)&(ScsiCBW.Length),2,gbScsiOutEnp);
	usb_read_ep_fifo((UINT8 *)&(ScsiCBW.A8Length),1,gbScsiOutEnp);
	usb_read_ep_fifo((UINT8 *)&(ScsiCBW.Residue),6,gbScsiOutEnp);
   	 
	reverse_DWORD( &(ScsiCBW.Address) );
	ScsiCBW.Length = SWAP(ScsiCBW.Length);
}

UINT8 scsi_proc(UINT8 ep_index)
{
	UINT16  cnt;

	gbScsiInEnp = ep_index;
	gbScsiOutEnp= ep_index;	   

	cnt = usb_get_fifo_length(gbScsiOutEnp);
	if(cnt < 31)
	{
		printfS("CBW length error!\n");

		usb_send_stall(gbScsiInEnp, EP_DIR_IN);
		usb_send_stall(gbScsiOutEnp, !EP_DIR_IN);
	  	return 1;
	}

	RxCBW();
	if(0x43425355 != ScsiCBW.Signature)
	{
		printfS("Signature error!\n");

		usb_send_stall(gbScsiInEnp, EP_DIR_IN);
		usb_send_stall(gbScsiOutEnp, !EP_DIR_IN);
		return 1;
	}
		
	ScsiCSW.CswTag = ScsiCBW.CbwTag;
	mscSet_Status(NO_SENSE);

	switch(ScsiCBW.OpCode)
	{
		case 0x00:
			if(ScsiCBW.CbwTrxLength != 0)
			{
				usb_send_stall(gbScsiInEnp, EP_DIR_IN);
				usb_send_stall(gbScsiOutEnp, !EP_DIR_IN);
				return 1;
			}
//			mscSet_Status(INVALID_COMMAND_OPERATION_CODE);
			break;
		
		case 0x03:
	  		RBC_RequestSense();			
			break;
		
		case 0x12:
			RBC_Inquiry();
			break;		   
		
		case 0xf7:
			RBC_AppCos();
			break;
		
		case 0xf8:
			RBC_Crypt();
			break;
		
		default:
			PadData();		
		   	mscSet_Status(INVALID_COMMAND_OPERATION_CODE);
	       	break;
	}
	
	SendCSW();
	REG_USBC_RxCSR &= (~0x01);
	
	return 0;
}
