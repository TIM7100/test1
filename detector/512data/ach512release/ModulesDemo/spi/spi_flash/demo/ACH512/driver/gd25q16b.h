/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : gd25q16b.h
 * Description : gd25q16b driver header file
 * Author(s)   : Eric
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#ifndef _GD25Q16B_H_
#define _GD25Q16B_H_
#include "common.h"

#define SPI_MEM_COM   SPIA

//SPI Flash CMD define
#define READ_ID			                    0x90
#define READ     		                    0x03
#define READ_FAST		                    0x0B
#define WRITE_EN		                    0x06
#define WRITE_DISEN		                    0x04
#define READ_STATUS_L                       0x05
#define READ_STATUS_H	                    0x35
#define WRITE_STATUS 	                    0x01
#define ERASE_SECTOR	                    0x20
#define PAGE_PROGRAM	                    0x02
#define QUAD_PAGE_PROGRAM                   0x32

#define DUAL_OUT_FAST_READ                  0x3b
#define QUAD_OUT_FAST_READ                  0x6b

#define DUAL_IO_FAST_READ                   0xbb
#define QUAD_IO_FAST_READ                   0xeb
#define QUAD_IO_WORD_FAST_READ              0xe7

#define HIGH_PER_MODE                       0xA3

#define SECTOR_SHIFT                        12
#define PAGE_SHIFT                          8
#define PAGE_SIZE                           256
#define BCH_SECTOR_SIZE                         4096
#define SECTOR_NUM                          512

UINT16 read_id(void);
void flash_erase_sector(UINT32 addr);
void flash_page_program(UINT8 *pBuf, UINT32 addr, UINT32 lenth);
void flash_quad_page_program(UINT8 *pBuf, UINT32 addr, UINT32 lenth);

void flash_page_read_X1(UINT8 *pBuf, UINT32 addr, UINT32 lenth);
void flash_page_read_X2(UINT8 *pBuf, UINT32 addr, UINT32 lenth);
void flash_page_read_X4(UINT8 *pBuf, UINT32 addr, UINT32 lenth);

void flash_page_program_X1(UINT8 *pBuf, UINT32 addr, UINT32 lenth);
void flash_page_program_X4(UINT8 *pBuf, UINT32 addr, UINT32 lenth);

void flash_mem_page_read(UINT8 *pbuf, UINT32 addr, UINT32 length);

#endif


