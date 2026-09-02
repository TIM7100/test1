#ifndef __CRC16_H__
#define __CRC16_H__

#include "fxx_std.h"
#include "delay.h"
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;


void memset_data(u8* des, u8 data, u32 len);

u16 GetCrc16(char* pData, u32 nLength);

u16 GetTolCrc16(u16 CurCrc, char* pData, u32 nLength);

void MemcpyData(char* Des, u8* Src, u32 Len);

void MemsetData(char* Des, u8 Setdata, u32 Len);

u8 MemcmpData(char* Des, u8* Src, u32 Len);

u16 CRC_DATA16_CAL(u8 * data, u32 Length);

#endif