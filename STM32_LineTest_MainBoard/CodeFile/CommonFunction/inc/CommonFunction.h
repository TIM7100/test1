#ifndef COMMON_FUNCTION_H
#define COMMON_FUNCTION_H
#include "stdint.h"
#include "stm32f10x.h" 
void memcpy_data(u8* des, u8* src_data, u32 len);

void memcpy_data_1(u8* des, u8* src_data, u32 len ,u16 StartAdd);

void memset_data(u8* des, u8 setdata, u32 len);

void memcpy_data_2(char* des,  unsigned char* src_data, int len, int start_len);


//将MainCount_Hex 以10进制形式存放到MainCount[]
void Train_MainCount_to_Array(u8* MainCount);

void transform_MainCount(u32 Hex_in, u8*Array_Out);

#endif
