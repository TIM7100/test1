#include "bsp_eflash.h"


/* 擦除一个区域 */
void EflashEraseArea(uint32_t StartAddress, uint32_t EraseSizeByte)
{
	/* 确保输入参数的合理性 */
	StartAddress &= ~0x1ff;
	while (EraseSizeByte > 0)
	{
		EFlash_ErasePage(StartAddress);			//擦除一次512个字节
		if (EraseSizeByte < 512)
		{
			return;
		}
		StartAddress += 512;
		EraseSizeByte -= 512;
	}
}


void EflashWritePage(uint32_t StartAddress, uint32_t WriteSizeByte, uint8_t *WriteBuffer)
{
    uint32_t i;
	
	uint32_t TxData;

 
//	printfS("Write FLASH\r\n");
	
	/* 确保输入参数的合理性 */
	StartAddress &= ~0x1ff;
	if (WriteSizeByte > 512)
	{
		WriteSizeByte = 512;
	}
	else if (WriteSizeByte < 4)
	{
		WriteSizeByte = 4;
	}

	/* Program Eflash */
	for (i = 0; i < WriteSizeByte / 4; i++)
	{
		TxData = WriteBuffer[i * 4 + 0] + (WriteBuffer[i * 4 + 1] << 8) + (WriteBuffer[i * 4 + 2] << 16) + (WriteBuffer[i * 4 + 3] << 24);
		EFlash_Program_Word(StartAddress + (i * 4), TxData);
	}	
}


void EflashReadPage(uint32_t StartAddress, uint32_t ReadSize, uint32_t *ReadBuf)
{
    uint32_t i;

 
//	printfS("Read Flash\r\n");
	
	/* 确保输入参数的合理性 */
	StartAddress &= ~0x1ff;
	if (ReadSize > 512)
	{
		ReadSize = 512;
	}
	else if (ReadSize < 4)
	{
		ReadSize = 4;
	}

	/* Read Data */
	for (i = 0; i < ReadSize / 4; i++)
	{
		*ReadBuf = EFLASH_READ_WORD(StartAddress + (i * 4));
		ReadBuf++;
	}	
}


