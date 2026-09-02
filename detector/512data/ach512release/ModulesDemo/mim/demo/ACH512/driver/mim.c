/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : mim.c
 * Description : mim driver source file
 * Author(s)   : Eric
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#include "mim.h"

/************************************************************************
 * function   : mem_bus_init
 * Description: SRAM or NorFlash initial
 *              Region division register REG_MIM_SEG0 set tft storage area
 *              size. off-chip SRAM chip area form 0K to 256K.
 * input : none
 * return: none
 ************************************************************************/
void mem_bus_init(void)
{
#ifdef LOW_POWER
    enable_module(BIT_MIM); //enable MIM
#endif

	REG_SCU_MUXCTRLA = ((REG_SCU_MUXCTRLA & (~(0xfffff000))) | (0xAA66A000));
	REG_SCU_MUXCTRLB = ((REG_SCU_MUXCTRLB & (~(0x00ffffff))) | (0x00AAAAAA));
	REG_SCU_MUXCTRLC = ((REG_SCU_MUXCTRLC & (~(0xfffffC00))) | (0xAAAAA800));
	REG_SCU_MUXCTRLD = ((REG_SCU_MUXCTRLD & (~(0x003ffff3))) | (0x002AAAA2));
	
	REG_MIM_CSCR0 = SRAM_EXT_WATS | SRAM_EXT_WNTS | SRAM_EXT_RATS |  SRAM_EXT_RO | SRAM_EXT_PS_16B | SRAM_EXT_WWS | SRAM_EXT_RWS |  SRAM_EXT_CSEN;
//	REG_MIM_CSCR0 = SRAM_EXT_WATS | SRAM_EXT_WNTS | SRAM_EXT_RATS |  SRAM_EXT_RO | SRAM_EXT_PS_8B  | SRAM_EXT_WWS | SRAM_EXT_RWS |  SRAM_EXT_CSEN;
    REG_MIM_SEG0 = (1023 << 15);            //sram,0-1024K

}



