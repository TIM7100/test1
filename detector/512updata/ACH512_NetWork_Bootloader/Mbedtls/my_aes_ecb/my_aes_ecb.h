#ifndef _MY_AES_CBC_H
#define _MY_AES_CBC_H

#include "common.h"

enum{
	Padding_ERR = 1,
	SET_KEY_ERR = 2,
	ENC_CBC_ERR,
	DEC_CBC_ERR,
	//FIRMWARE_CRC_ERR
};

#define APP_KEY              0	                               //解密密钥
#define SV_KEY               1                                 //加密密钥

u8 EncryptDataByAesECB(u8 *InData, u16 DataLength, u8 *OutEncryptedData);
u8 DecryptDataByAesECB(u8 *InData, u16 DataLength, u8 *OutDecryptedData);

#endif
