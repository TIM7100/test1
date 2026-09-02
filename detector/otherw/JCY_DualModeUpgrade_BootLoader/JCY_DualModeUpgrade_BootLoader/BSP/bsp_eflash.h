#ifndef __MYEFLASH_H__
#define __MYEFLASH_H__

#include "fxx_std.h"

void EflashEraseArea(uint32_t StartAddress, uint32_t EraseSizeByte);
void EflashWritePage(uint32_t StartAddress, uint32_t WriteSizeByte, uint8_t *WriteBuffer);
void EflashReadPage(uint32_t StartAddress, uint32_t ReadSize, uint32_t *ReadBuf);
void EflashWriteByte(uint32_t StartAddress, uint32_t WriteSizeByte, uint8_t *WriteBuffer);
void EflashReadByte(uint32_t StartAddress, uint32_t ReadSize, uint32_t *ReadBuf);

#endif

