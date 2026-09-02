/*
  ******************************************************************************
  * @file    EFlash.h
  * @version V1.0.0
  * @date    2020
  * @brief   Header file of EFlash  module.
  ******************************************************************************
*/
#ifndef __EFlash_H__
#define __EFlash_H__

#include "fxx_std.h"

#define EFLASH_PAGE_SIZE    (512UL)

#define EFLASH_READ_WORD(Addr)        (*(volatile uint32_t *)(Addr))    // Read By Word
#define EFLASH_READ_HALFWORD(Addr)    (*(volatile uint16_t *)(Addr))    // Read By Half Word
#define EFLASH_READ_BYTE(Addr)        (*(volatile uint8_t *)(Addr))     // Read By Byte

/********************  Bit definition for EFC_CTRL register  ******************/
#define EFC_CTRL_CHIP_ERASE_MODE        (1 << 2)
#define EFC_CTRL_PAGE_ERASE_MODE        (1 << 1)
#define EFC_CTRL_PROGRAM_MODE           (1 << 0)

/********************  Bit definition for EFC_STATUS register  ***************/
#define EFC_STATUS_NVR4_LOCK            (1 << 4)
#define EFC_STATUS_NVR3_LOCK            (1 << 3)
#define EFC_STATUS_NVR2_LOCK            (1 << 2)
#define EFC_STATUS_NVR1_LOCK            (1 << 1)
#define EFC_STATUS_EFLASH_RDY           (1 << 0)

#define RD_WAIT_ENSURE_OK   8 
#define SET_EFC_RD_WAIT(wait)     {EFC->CTRL   = (EFC->CTRL & ~(0x1F << 7)) | (wait << 7);}     

/* Exported functions --------------------------------------------------------*/  

void EFlash_Init(uint32_t fu32_freq);  
/* _EFlash_ErasePage */
bool EFlash_ErasePage(uint32_t fu32_Addr);

/* _EFlash_Program_Word */
bool EFlash_Program_Word(uint32_t fu32_Addr, uint32_t fu32_Data);

#endif
