#ifndef __LCD_H__
#define __LCD_H__

#include "fxx_std.h"




void LCD_UartInit(uint32_t Baudrate);
void CheckBusy(void);
void LCD_UartSend(const char *DataBuf);
void StrMiddleShow(char *StrName, u8 Yaxis, u8 Color);
void UART2_IRQ(void);

char* CMD_DC16(u16 x, u16 y, char *str, u8 Color);

char* CMD_DC16_Len(u16 x, u16 y, char *str, u32 length, u8 Color);

char* CMD_BOX(u16 x1, u16 y1, u16 x2, u16 y2, u8 Color);

char* CMD_DC24(u16 x, u16 y, char *Str, u8 Color);

#endif

