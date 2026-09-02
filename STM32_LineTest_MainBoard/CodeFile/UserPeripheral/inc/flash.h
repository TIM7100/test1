#ifndef __FLASH_H
#define __FLASH_H
#include "stm32f10x_flash.h"
#define  STARTADDR  0x0801F800





void ReadFlashNBtye(u32 ReadAddress, u8 *ReadBuf, u32 ReadNum);
void Write_Flash_u8(u32 WriteAddress, u8* WriteData, u32 len);
void Write_Flash_u16(u32 WriteAddress, u16* WriteData, u32 len);
void Write_Flash_u32(u32 WriteAddress, u32* WriteData, u32 len);
void Read_Flash_u8(uint32_t ReadAddress, uint8_t *ReadBuf, int32_t ReadNum) ;
#endif
