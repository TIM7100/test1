#ifndef __LCD_MENU_H //·ÀÖ¹Ñ­»·°üº¬
#define __LCD_MENU_H

#include  "lcd.h"


void ShowMenu(void);
void showSerialNum(u32 serialNumTmp);
void ConfirmSelection(u8 *TitleName);
void Jump_IAP(void);

#endif

