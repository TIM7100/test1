/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : nfm.c
 * Description : nfm driver source file
 * Author(s)   : Eric
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#include "nfm.h"

/************************************************************************
 * function   : nfm_init
 * Description: nfm initial
 * input : none
 * return: none
 ************************************************************************/
void nfm_init(void)
{
#ifdef LOW_POWER
    enable_module(BIT_NFM); //enable NFM
#endif

//IOø⁄≈‰÷√
    REG_SCU_MUXCTRLB = (REG_SCU_MUXCTRLB & ~0xffffff00) | 0x55aaaa00;
    REG_SCU_MUXCTRLC = (REG_SCU_MUXCTRLC & ~0x0000003f) | 0x00000015; 

#ifdef EDO_MODE
    REG_NFM_CTRL |= 0x80; 
#else
    REG_NFM_CTRL &= ~0x80;
#endif
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
    REG_NFM_WST = NFM_TADL | NFM_TRHW | NFM_TWHR | NFM_TREH | NFM_TRP | NFM_TWH | NFM_TWP;
}


