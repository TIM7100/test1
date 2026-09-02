#ifndef __LCD_H__
#define __LCD_H__

#include  "common.h"
     
char* CMD_DC16(u16 x, u16 y, u8 *str, u8 Color);
char* CMD_DC16_Len(u16 x, u16 y, u8 *str, u32 length, u8 Color);
void ClearShow(void);
void LCD_UartSend(char *databuf);
char* CMD_DC32(u16 x, u16 y, u8 *str, u8 Color);
char* CMD_DC24(u16 x, u16 y, u8 *str, u8 Color);

#endif

