
#ifndef _USER_UART__
#define _USER_UART__

#include "stm32f10x_it.h"     //根据用户MCU进行修改

#define uchar    unsigned char
#define uint8    unsigned char
#define uint16   unsigned short int
#define uint32   unsigned long
#define int16    short int
#define int32    long

/****************************************************************************
* 名    称： UartInit()
* 功    能： 串口初始化
* 入口参数： 无
* 出口参数： 无
****************************************************************************/
void UartInit(uint32 Baudrate);

/*****************************************************************
* 名    称： SendChar()
* 功    能： 发送1个字节
* 入口参数： t  发送的字节
* 出口参数： 无
*****************************************************************/
void  SendChar(uchar t);

//void  Send8(u8 t);
//void  Send16(u16 t);
//void  Send32(u32 t);
//void  SendStr(u8 *str);
//void  Send8_N(u8 *pData,u16 nDataLen);
//void  Send16_N(u16 *pData,u16 nDataLen);


#endif
