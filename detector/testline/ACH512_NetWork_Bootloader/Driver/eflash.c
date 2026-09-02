/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : eflash.c
 * Description : eflash driver source file
 * Author(s)   : Eric
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#include  "eflash.h"

#ifndef ROM_DRIVER_FLASH
/************************************************************************
 * function   : eflash_write_word
 * Description: eflash write word
 * input :
 *         UINT32 addr: address
 *         UINT32 value: value
 * return: 0--success   1--fail
 ************************************************************************/
UINT8 eflash_write_word(UINT32 addr, UINT32 value)
{

    UINT8 vf;
    REG_EFC_CTRL |= EFC_WRITE_MODE;
#ifdef EFLASH_VERIFY_EN
    REG_EFC_CTRL |= EFC_PROGRAM_VRI_EN;
#endif
    REG_EFC_SEC = 0x55AAAA55;
    *((volatile UINT32 *)(addr)) = value;
    while (!(REG_EFC_STATUS & 0x01));
    REG_EFC_CTRL &= ~EFC_WRITE_MODE;
    vf = 0;

#ifdef EFLASH_VERIFY_EN
    while (!(REG_EFC_INTSTATUS & (0x01 << 4)));
    REG_EFC_INTSTATUS = (0x01 << 4);
    if (REG_EFC_INTSTATUS & (0x01 << 6)) //vf error
    {
        REG_EFC_INTSTATUS = (0x01 << 6);
        vf = 1;
    }
    REG_EFC_CTRL &= ~EFC_PROGRAM_VRI_EN;
#endif

    return vf;
}

/************************************************************************
 * function   : eflash_erase_page
 * Description: eflash erase page
 * input :
 *         UINT32 page_addr: page address
 * return: 0--success   1--fail
 ************************************************************************/
UINT8 eflash_erase_page(UINT32 page_addr)
{
    UINT8 vf;

    REG_EFC_CTRL |= EFC_PAGE_ERASE_MODE;
    REG_EFC_SEC = 0x55AAAA55;
    *((volatile UINT32 *)(page_addr)) = 0;
    while (!(REG_EFC_STATUS & 0x01));
    REG_EFC_CTRL &= ~EFC_PAGE_ERASE_MODE;
    vf = 0;

#ifdef EFLASH_VERIFY_EN
    REG_EFC_ADCT = (page_addr) >> 2;
    REG_EFC_CTRL |= EFC_ERASE_VRI_EN;
    while (!(REG_EFC_INTSTATUS & (0x01 << 4)));
    REG_EFC_INTSTATUS = (0x01 << 4);
    if (REG_EFC_INTSTATUS & (0x01 << 3)) //vf error
    {
        REG_EFC_INTSTATUS = (0x01 << 3);
        vf = 1;
    }
#endif

    return vf;
}

#endif


/* 擦除一个区域 */
void EflashEraseArea(uint32_t StartAddress, uint32_t EraseSizeByte)
{
	/* 确保输入参数的合理性 */
	StartAddress &= ~0x1ff;
	while (EraseSizeByte > 0)
	{
		eflash_erase_page(StartAddress);			//擦除一次512个字节
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
		eflash_write_word(StartAddress + (i * 4), TxData);
	}
}

void EflashReadPage(uint32_t StartAddress, uint32_t ReadSize, uint32_t *ReadBuf)
{
    uint32_t i;
	
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
		*ReadBuf = eflash_read_word(StartAddress + (i * 4));
		 ReadBuf++;
	}	
}

u8 EflashWritePageHandle(uint32_t StartAddress, uint32_t WriteSizeByte, uint8_t *WriteBuffer)
{
	uint32_t i;
	uint32_t TxData;
	uint32_t RxData;
	
	/*擦除扇区*/
	EflashEraseArea(StartAddress, WriteSizeByte);
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
		eflash_write_word(StartAddress + (i * 4), TxData);
		RxData = eflash_read_word(StartAddress + (i * 4));
		if(TxData != RxData)
		{
			return 1;
		}
	}
	return 0;
}

