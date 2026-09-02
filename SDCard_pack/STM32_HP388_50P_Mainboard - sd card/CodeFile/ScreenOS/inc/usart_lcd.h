#ifndef __USART_H
#define __USART_H
#include "stdint.h"
#include "stdio.h"

extern char CMP_BUF[100];            //需要返回指针，开是否使用static局部变量

int UartGet(void);

void CheckBusy(void);

void UartSend(char * databuf);

void LcdDrvInit(void);

void ShowResult_50pcs(u8* slave_result);

//LCD转换函数(DC32)
char* T_DC32(u16 x, u16 y, unsigned char *str, u8 color);

//LCD转换函数(DC16)
char* T_DC16(u16 x, u16 y, unsigned char *str, u8 color);

#endif
