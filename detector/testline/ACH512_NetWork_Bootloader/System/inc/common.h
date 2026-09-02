/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : common.h
 * Description : common header file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/

#ifndef __COMMON_H__
#define __COMMON_H__

#include  "stdio.h"	   //printf .....
#include  "string.h"   //strlen ,memset,strcmp,memcmp,strcpy .....
#include  "types.h"
#include  "sys_config.h"
#include  "ach512.h"


#define SWAP(x)             ((((x) & 0xFF) << 8) | (((x) >> 8) & 0xFF))
#define max(a, b)		    (((a) > (b)) ? (a) : (b))
#define min(a, b)		    (((a) < (b)) ? (a) : (b))
typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;
/************************************************************************
 * function   : printf_buff_byte
 * Description: printf data block by byte
 * input :
 *         UINT8* buff: buff
 *         UINT32 length: byte length
 * return: none
 ************************************************************************/
void printf_buff_byte(UINT8* buff, UINT32 length);

/************************************************************************
 * function   : printf_buff_word
 * Description: printf data block by word
 * input :
 *         UINT8* buff: buff
 *         UINT32 length: word length
 * return: none
 ************************************************************************/
void printf_buff_word(UINT32* buff, UINT32 length);

void delay(UINT32 count);
void reverse_DWORD(UINT32 *var);
void reverse_memory(UINT8 *buff, UINT32 length);
void TransformMainCount(u32 Hex_in, u8*Array_Out);

void memset_data(u8* des, u8 data, u32 len);
void MemcpyData(char* Des, u8* Src, u32 Len);
void MemsetData(char* Des, u8 Setdata, u32 Len);
u8 MemcmpData(char* Des, u8* Src, u32 Len);

#endif

