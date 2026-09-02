/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : bch.c
 * Description : bch driver source file
 * Author(s)   : Eric
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#include "nfm.h"
#include "bch.h"

/************************************************************************
 * function   : bch515_8_init
 * Description: bch515_8 initial
 * input : none
 * return: none
 ************************************************************************/
void bch515_8_init(void)
{
#ifdef LOW_POWER
    enable_module(BIT_NFM); //enable NFM
#endif

    REG_BCH_CONFIG = BCH_TYPE_8BIT; // 512+3+13
}

/************************************************************************
 * function   : flash_wait_ready
 * Description: flash wait ready
 * input : none
 * return: none
 ************************************************************************/
void flash_wait_ready(void)
{
    while((REG_NFM_STATUS & 0x10) == 0);
	REG_NFM_STATUS = 0x10;
}

/************************************************************************
 * function   : flash_reset
 * Description: flash reset
 * input : none
 * return: none
 ************************************************************************/
void flash_reset(void)
{
    REG_NFM_CTRL &= ~0x01;//
#ifdef NFM_BIG_ENDIAN
    REG_NFM_CTRL |= 0x20;
#else
    REG_NFM_CTRL &= ~0x20;
#endif
#ifdef NFM_WRITE_PROTECT
    REG_NFM_CTRL &= ~0x10;
#else
    REG_NFM_CTRL |= 0x10;
#endif

    REG_NFM_CMD_CH = CMDRESET;

    flash_wait_ready();

    REG_NFM_CTRL |= 0x01; //

}

/************************************************************************
 * function   : flash_get_id
 * Description: flash get id
 * input :
 *         UINT8* ID: id number
 *         UINT8 lenth: length
 * return:
 ************************************************************************/
void flash_get_id(UINT8 *ID, UINT8 lenth)
{
    UINT8 i;

    REG_NFM_CTRL &= ~0x01;
    REG_NFM_CMD_CH = CMDID; //get id
    REG_NFM_ADDR_CH = 0x00;  //write address

    for(i = 0; i < lenth; i++)
    {
        *ID++ = REG_NFM_NECC_CH8;
    }
    REG_NFM_CTRL |= 0x01; //
}

/************************************************************************
 * function   : flash_erase
 * Description: flash erase
 * input :
 *         DWORD Row_addr:row address
 * return: none
 ************************************************************************/
void flash_erase(UINT32 Row_addr)
{
    REG_NFM_CTRL &= ~0x01;
    //check if nand flash idle
    REG_NFM_CMD_CH = CMDERASE1;

    REG_NFM_ADDR_CH = (Row_addr)&0xff;      // Page address
    REG_NFM_ADDR_CH = (Row_addr >> 8) & 0xff;   // Page address
    REG_NFM_ADDR_CH = (Row_addr >> 16) & 0xff;  // Page address

    REG_NFM_CMD_CH = CMDERASE2;

    flash_wait_ready();
    REG_NFM_CTRL |= 0x01; //
}

/************************************************************************
 * function   : bch_flash_program
 * Description: bch flash program
 * input :
 *         UINT32 PageAddr: row address
 *         UINT16 ColumnAddr: colum address
 *         UINT8[] wdata: write buff
 *         UINT8 ecc_mode: ecc_mode
 * return: none
 ************************************************************************/
void bch_flash_program(UINT32 RowAddr, UINT16 ColumnAddr, UINT8 wdata[], UINT8 ecc_mode)
{
    UINT32 i;

    REG_NFM_CTRL &= ~0x01; 
    REG_NFM_CMD_CH = CMDPROGRAM1;

    REG_NFM_ADDR_CH = (UINT8)ColumnAddr;       // Column address
    REG_NFM_ADDR_CH = (UINT8)(ColumnAddr >> 8);  // Column address
    REG_NFM_ADDR_CH = (UINT8)RowAddr;           // Page address
    REG_NFM_ADDR_CH = (UINT8)(RowAddr >> 8);    // Page address
    REG_NFM_ADDR_CH = (UINT8)(RowAddr >> 16);   // Page address
	
    if(ecc_mode==1)
    {
        REG_BCH_CTRL &= ~0x01; //encode mode
        REG_BCH_STATUS = 1;  //reset encode channel

        for(i = 0; i < BCH_SECTOR_SIZE; i++)
        {
            REG_NFM_ECC_CH8   = wdata[i];
        }
        for(i = 0; i < BCH_INF_SIZE; i++)
        {
            REG_NFM_ECC_CH8   = 0xff;
        }

        REG_BCH_CODEPTR = 0;
        for(i = 0; i < BCH_ECC_SIZE; i++)
        {
            REG_NFM_NECC_CH8 = REG_BCH_CODE8;
        }
    }
    else
    {
        for(i = 0; i < BCH_SECTOR_SIZE; i++)
        {
            REG_NFM_NECC_CH8   = wdata[i];
        }
    }
    REG_NFM_CMD_CH = CMDPROGRAM2;    
    flash_wait_ready();

    REG_NFM_CTRL |= 0x01; //
}

/************************************************************************
 * function   : bch_flash_read
 * Description: bch flash read
 * input :
 *         UINT32 PageAddr: row address
 *         UINT16 ColumnAddr: column address
 *         UINT8[] rdata: read buff
 *         UINT8 ecc_mode: ecc_mode
 * return: UINT32 bch fail flag
 ************************************************************************/
UINT32 bch_flash_read(UINT32 RowAddr, UINT16 ColumnAddr, UINT8 rdata[], UINT8 ecc_mode)
{
    UINT32 i,bch_fail_flag;
    bch_fail_flag = 0;
    REG_NFM_CTRL &= ~0x01;
    REG_NFM_CMD_CH = CMDREAD1;
    REG_NFM_ADDR_CH = (UINT8)ColumnAddr;  // Column address
    REG_NFM_ADDR_CH = (UINT8)(ColumnAddr >> 8);  // Column address
    REG_NFM_ADDR_CH = (UINT8)RowAddr;  // Page address
    REG_NFM_ADDR_CH = (UINT8)(RowAddr >> 8);  // Page address
    REG_NFM_ADDR_CH = (UINT8)(RowAddr >> 16);  // Page address

    REG_NFM_CMD_CH = CMDREAD2;

    flash_wait_ready();

    if(ecc_mode == 1)//ecc mode
    {      
        REG_BCH_CTRL |= 0x01; //decode mode
        REG_BCH_CTRL |= 0x20; //

        REG_BCH_BASEADDR = (UINT32)rdata;
		for(i = 0; i < (BCH_SECTOR_SIZE); i++) //
		{
			rdata[i] = REG_NFM_ECC_CH8;
		}

        for(i = 0; i < (BCH_INF_SIZE + BCH_ECC_SIZE); i++) //
        {
            REG_NFM_ECC_CH8;
        }
		while((REG_BCH_STATUS & 0x08) == 0); //
        if(REG_BCH_STATUS & (1<<4))
        {
            bch_fail_flag = 1;
        }
    }
	else
	{
		for(i = 0; i < BCH_SECTOR_SIZE; i++) //
		{
			rdata[i] = REG_NFM_NECC_CH8;
		}
	}

    //out put err
    REG_BCH_STATUS = REG_BCH_STATUS;

    REG_NFM_CTRL |= 0x01; //
    return bch_fail_flag;
}





