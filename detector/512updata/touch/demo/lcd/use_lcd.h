#ifndef _use_lcd_H_
#define _use_lcd_H_
#include "cmd_process.h"
#include "cmd_queue.h"
#include "common.h"
void User_App(void);
void AddStr(u8* str1, u8* str2, u8 sp, u8 len);//Ìí¼Ó×Ö·û
void Num2Str(u8* str, u16 num, u8 sp, u8 ep);
void Postback_Get_PageID(void);
void Process_Touch(PCTRL_MSG msg, uint16 size);
void Process_Postback(PCTRL_MSG msg, uint16 size) ;
void LcdMainpage(void);
#endif 
