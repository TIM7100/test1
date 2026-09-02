#ifndef __CRC_H
#define __CRC_H

#include "stdint.h"

u16 GetCrc16(const u8* pData, u32 nLength);
u8 GetCrc8(const u8* dataarray, u32 datalength);


#endif
/**/

