#include "flash.h"
#include "Timer0.h"
#include "stm32f10x_flash.h"

volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
void ReadFlashNBtye(u32 ReadAddress, u8 *ReadBuf, u32 ReadNum)
{
    int DataNum = 0;
    ReadAddress = (uint32_t)STARTADDR + ReadAddress;
    while (DataNum < ReadNum)
    {
        *(ReadBuf + DataNum) = *(__IO uint8_t*) ReadAddress++;
        DataNum++;
    }
}


void Write_Flash_u8(u32 WriteAddress, u8* WriteData, u32 len)
{
    u32 i;
    u32 buf;
    u16 databuf;
    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    FLASHStatus = FLASH_ErasePage(STARTADDR);

    for (i = 0; i < len / 2; i++)
    {
        if (FLASHStatus == FLASH_COMPLETE)
        {
            buf = WriteAddress + (i * 2);
            databuf = (*(WriteData + (2 * i) + 1) << 8) + *(WriteData + (2 * i));
            FLASHStatus = FLASH_ProgramHalfWord(buf, databuf);
        }
    }
    FLASH_Lock();
}

void Write_Flash_u16(u32 WriteAddress, u16* WriteData, u32 len)
{
    u32 i;
    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    FLASHStatus = FLASH_ErasePage(STARTADDR);

    for (i = 0; i < len; i++)
    {
        if (FLASHStatus == FLASH_COMPLETE)
        {
            FLASHStatus = FLASH_ProgramHalfWord(WriteAddress + (i * 2), *(WriteData + i));
        }
    }
    FLASH_Lock();
}


void Write_Flash_u32(u32 WriteAddress, u32* WriteData, u32 len)
{
    u32 i;
    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    FLASHStatus = FLASH_ErasePage(WriteAddress);

    for (i = 0; i < len; i++)
    {
        if (FLASHStatus == FLASH_COMPLETE)
        {
            FLASHStatus = FLASH_ProgramWord(WriteAddress + (i * 4), *(WriteData + i));
        }
    }
    FLASH_Lock();
}


void Read_Flash_u8(uint32_t ReadAddress, uint8_t *ReadBuf, int32_t ReadNum)
{
    int DataNum = 0;
    ReadAddress = ReadAddress;
    while (DataNum < ReadNum)
    {
        *(ReadBuf + DataNum) = *(__IO uint8_t*) ReadAddress++;
        DataNum++;
    }
}
