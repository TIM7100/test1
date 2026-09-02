#ifndef __USART_H
#define __USART_H

 #include "delay.h"
 #include "stdio.h"
 #include "stdint.h"
#include "uart.h"
#include "crc.h"





extern u8 USART_RX_BUF[64];     //接收缓冲,最大63个字节.末字节为换行符
extern u8 USART_RX_STA;         //接收状态标记

void UartSend(char * databuf) ;
void CheckBusy(void);
void UartLcdInit(void);
char* T_DC16(u16 x, u16 y, unsigned char *str, u8 color);
void Show_10_Result(void);
void LcdShowType(u8* LcdData, u8 Color);
char* T_DC16_Len(u16 x, u16 y, unsigned char *str, u32 length, u8 color);
void BasicUserInterface(void);
#endif
