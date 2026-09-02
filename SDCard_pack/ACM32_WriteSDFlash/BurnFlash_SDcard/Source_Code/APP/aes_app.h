#ifndef __AES_APP_H__
#define __AES_APP_H__

#include "ACM32Fxx_HAL.h"

extern const uint8_t AES_Key[16];
extern const uint8_t AES_Iv[16];

/* AES-128-CBC Decrypt (len must be multiple of 16) */
void AES128_CBC_Decrypt(uint8_t* pOut, const uint8_t* pIn, uint32_t len, const uint8_t* Key, const uint8_t* IV);

#endif