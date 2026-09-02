#include "CommonFunction.h"
#include "stm32f10x.h" 
extern u32 MainCount_Hex;

void memcpy_data(u8* des, u8* src_data, u32 len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        des[i] = src_data[i];
    }
}

void memcpy_data_1(u8* des, u8* src_data, u32 len ,u16 StartAdd)
{
    int i;
    for (i = 0; i < len; i++)
    {
        des[i + StartAdd] = src_data[i];
    }
}
void memcpy_data_2(char* des,  unsigned char* src_data, int len, int start_len)
{
    int i;
    for (i = 0; i < len; i++)
    {
      des[ i + start_len ] = src_data[i];
    }
	}

void memset_data(u8* des, u8 setdata, u32 len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        des[i] = setdata;
    }
}

//将MainCount_Hex 以10进制形式存放到MainCount[]
void Train_MainCount_to_Array(u8* MainCount)
{
    u32 tmp_u32;

    tmp_u32 = MainCount_Hex;
    MainCount[0] = tmp_u32 / 100000 + 0x30;
    tmp_u32 = MainCount_Hex % 100000;
    MainCount[1] = tmp_u32 / 10000 + 0x30;
    tmp_u32 = MainCount_Hex % 10000;
    MainCount[2] = tmp_u32 / 1000 + 0x30;
    tmp_u32 = MainCount_Hex % 1000;
    MainCount[3] = tmp_u32 / 100 + 0x30;
    tmp_u32 = MainCount_Hex % 100;
    MainCount[4] = tmp_u32 / 10 + 0x30;
    tmp_u32 = MainCount_Hex % 10;
    MainCount[5] = tmp_u32 + 0x30;
}

void transform_MainCount(u32 Hex_in, u8*Array_Out)
{
    Array_Out[0] = (u8)(Hex_in >> 24);
    Array_Out[1] = (u8)(Hex_in >> 16);
    Array_Out[2] = (u8)(Hex_in >> 8);
    Array_Out[3] = (u8)(Hex_in);
}

