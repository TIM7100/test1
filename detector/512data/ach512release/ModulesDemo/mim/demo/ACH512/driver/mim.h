/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : mim.h
 * Description : mim driver header file
 * Author(s)   : Eric
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#ifndef __MIM_H__
#define __MIM_H__
#include "common.h"

#define TFT_WATS                          (7 << 28)
#define TFT_WNTS                          (7 << 24)
#define TFT_RATS                          (7 << 20)
#define TFT_AATS                          (3 << 15)
#define TFT_RO                            (0 << 14)
#define TFT_PS_8B                         (3 << 12)
#define TFT_PS_16B                        (1 << 12)
#define TFT_PS_32B                        (0 << 12)
#define TFT_WWS                           (15 << 8)
#define TFT_RWS                           (15 << 4)
#define TFT_CDA                           (0 << 3)
#define TFT_CSEN                          (1 << 0)

#define SRAM_EXT_WATS                     (7 << 28)
#define SRAM_EXT_WNTS                     (7 << 24)
#define SRAM_EXT_RATS                     (7 << 20)
#define SRAM_EXT_AATS                     (3 << 15)
#define SRAM_EXT_RO                       (0 << 14)
#define SRAM_EXT_PS_8B                    (1 << 13)
#define SRAM_EXT_PS_16B                   (0 << 13)
#define SRAM_EXT_WWS                      (15 << 8)
#define SRAM_EXT_RWS                      (15 << 4)
#define SRAM_EXT_CDA                      (0 << 3)
#define SRAM_EXT_CSEN                     (1 << 0)

#define MEM0_PORT32(addr)             (*(volatile UINT32*)(MIM_MEM_ADDR(EMEM0) + addr))
#define MEM0_PORT16(addr)             (*(volatile UINT16*)(MIM_MEM_ADDR(EMEM0) + addr))
#define MEM0_PORT8(addr)              (*(volatile UINT8* )(MIM_MEM_ADDR(EMEM0) + addr))


void mem_bus_init(void);



#endif
