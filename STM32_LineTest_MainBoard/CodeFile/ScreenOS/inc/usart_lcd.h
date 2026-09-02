#ifndef __USART_H
#define __USART_H
#include "stdint.h"
#include "stdio.h"

extern char CMP_BUF[100];            //需要返回指针，开是否使用static局部变量

int UartGet(void);

void CheckBusy(void);

void UartSend(char * databuf);

void LcdDrvInit(void);

void Show_10_Result(u8* slave_result);

//void ShowResult_10pcs(u8* slave_result);

void ShowResult_50pcs_r(u8* slave_result);

void ShowResult_40pcs_r(u8* slave_result);


void ShowResult_50pcs_l(u8* slave_result);

void ShowResult_10pcs_r(u8* slave_result);

//LCD转换函数(DC32)
char* T_DC32(u16 x, u16 y, unsigned char *str, u8 color);

//LCD转换函数(DC16)
char* T_DC16(u16 x, u16 y, unsigned char *str, u8 color);

//LCD填充框(BOXF)
char* T_BOXF(u16 x1, u16 y1, u16 x2, u16 y2, u8 color);

//LCD填充圆
char* D_CIRF(u16 x, u16 y, u16 r, u8 color);
#endif
