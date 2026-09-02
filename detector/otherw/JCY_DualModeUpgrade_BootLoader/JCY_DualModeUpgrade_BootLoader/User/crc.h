#ifndef __CRC16_H__
#define __CRC16_H__

#include "fxx_std.h"


void memset_data(u8* des, u8 data, u32 len);

u16 GetCrc16(char* pData, u32 nLength);

void MemcpyData(char* Des, u8* Src, u32 Len);

void MemsetData(char* Des, u8 Setdata, u32 Len);

u8 MemcmpData(char* Des, u8* Src, u32 Len);

u16 GetTolCrc16(u16 CurCrc, char* pData, u32 nLength);

#endif