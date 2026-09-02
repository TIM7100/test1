/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : nfm.h
 * Description : nfm driver header file
 * Author(s)   : Eric
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#ifndef __NFM_H__
#define __NFM_H__

#include "common.h"

#define EDO_MODE  //默认EDO模式是打开的
#define NFM_BIG_ENDIAN
//#define NFM_WRITE_PROTECT

//此参数适用于开发板上NANDFLASH的读写参数，其他型号参照NANDFLASH的手册进行时间参数配置
#define NFM_TWP                       (1 << 0)  //18ns@110MHz
#define NFM_TWH                       (1 << 4)  //18ns@110MHz
#define NFM_TRP                       (1 << 8)  //18ns@110MHz
#define NFM_TREH                      (1 << 12)  //18ns@110MHz
#define NFM_TWHR                      (0xf << 16)  //144ns@110MHz
#define NFM_TRHW                      (0xf << 20)  //144ns@110MHz
#define NFM_TADL                      (0xf << 24)  //144ns@110MHz

/************************************************************************
 * function   : nfm_init
 * Description: nfm initial
 * input : none
 * return: none
 ************************************************************************/
void nfm_init(void);

#endif
