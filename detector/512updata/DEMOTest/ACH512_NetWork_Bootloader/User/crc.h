#ifndef __CRC16_H__
#define __CRC16_H__

#include "common.h"


u16 GetCrc16(char* pData, u32 nLength);


u16 GetTolCrc16(u16 CurCrc, char* pData, u32 nLength);

#endif


