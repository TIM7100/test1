/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : bch.h
 * Description : bch driver header file
 * Author(s)   : Eric
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#ifndef __BCH_H__
#define __BCN_H__

#include "common.h"

//TC58NVG0S3HTA00
//1 page = (2048 + 128 bytes),,1 block = (2048 + 128) bytes x 64 pages,,1 plane =  1024 blocks

#define	CMDREAD1 		             0x00
#define CMDREAD2                     0x30
#define	CMDID 		                 0x90
#define	CMDSTATUS 		             0x70
#define	CMDRESET 	                 0xFF
#define	CMDPROGRAM1		             0x80
#define	CMDPROGRAM2		             0x10
#define	CMDERASE1 		             0x60
#define	CMDERASE2		             0xD0
#define	CMDRANDOMINPUT               0x85
#define	CMDRANDOMOUTPUT1	         0x05
#define	CMDRANDOMOUTPUT2	         0xE0

#define BCH_TYPE_8BIT                0     //8BIT
#define BCH_SECTOR_SIZE		         512
#define BCH_INF_SIZE		         3
#define BCH_ECC_SIZE		         13

/************************************************************************
 * function   : bch515_8_init
 * Description: bch515_8 initial
 * input : none
 * return: none
 ************************************************************************/
void bch515_8_init(void);

/************************************************************************
 * function   : flash_wait_ready
 * Description: flash wait ready
 * input : none
 * return: none
 ************************************************************************/
void flash_wait_ready(void);

/************************************************************************
 * function   : flash_reset
 * Description: flash reset
 * input : none
 * return: none
 ************************************************************************/
void flash_reset(void);

/************************************************************************
 * function   : flash_get_id
 * Description: flash get id
 * input :
 *         UINT8* ID: id number
 *         UINT8 lenth: length
 * return:
 ************************************************************************/
void flash_get_id(UINT8 *ID, UINT8 lenth);

/************************************************************************
 * function   : flash_erase
 * Description: flash erase
 * input :
 *         DWORD Row_addr:row address
 * return: none
 ************************************************************************/
void flash_erase(DWORD Row_addr);

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
void bch_flash_program(UINT32 RowAddr, UINT16 ColumnAddr, UINT8 wdata[], UINT8 ecc_mode);

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
UINT32 bch_flash_read(UINT32 RowAddr, UINT16 ColumnAddr, UINT8 rdata[], UINT8 ecc_mode);

#endif

