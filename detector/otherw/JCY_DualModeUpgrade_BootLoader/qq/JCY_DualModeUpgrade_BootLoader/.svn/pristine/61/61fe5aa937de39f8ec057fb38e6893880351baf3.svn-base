#ifndef _MY_AES_CBC_H
#define _MY_AES_CBC_H

#include "fxx_std.h"

enum{
	Padding_ERR = 1,
	SET_KEY_ERR = 2,
	ENC_CBC_ERR,
	DEC_CBC_ERR,
	//FIRMWARE_CRC_ERR
};

#define APP_KEY              0	                               //Ω‚√‹√‹‘ø
#define SV_KEY               1                                 //º”√‹√‹‘ø

u8 EncryptDataByAesCBC(u8 *InData, u16 DataLength, u8 *OutEncryptedData, u8 *IV);
u8 DecryptDataByAesCBC(u8 *InData, u16 DataLength, u8 *OutDecryptedData, u8 *IV, u8 Key);

#endif
