#ifndef __CRC_H
#define __CRC_H
#include "stm32f10x.h" 
#include "stdint.h"

u16 GetCrc16(const u8* pData, u32 nLength);
u8 GetCrc8(const u8* dataarray, u32 datalength);
u16 CRC_DATA16_CAL(u8 * data, u32 Length);

#endif
/**/

