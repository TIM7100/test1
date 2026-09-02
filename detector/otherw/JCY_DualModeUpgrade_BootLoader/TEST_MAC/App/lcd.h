#ifndef __LCD_H__
#define __LCD_H__

#include "fxx_std.h"
#include "uart.h"
#include "delay.h"
#include "gpio.h"
#include "menu.h"
#include  "crc.h"
#include "usart_lcd.h"


#define  GREEN_ON            					GPIO_WriteBit(GPIOB, GPIO_LED_GREEN_Pin,Bit_SET) 		//ÂÌµÆ ON
#define  GREEN_OFF           					GPIO_WriteBit(GPIOB, GPIO_LED_GREEN_Pin,Bit_RESET) 	 	//ÂÌµÆ OFF
#define  RED_ON              					GPIO_WriteBit(GPIOB, GPIO_LED_RED_Pin,Bit_SET) 		 	//ºìµÆ ON
#define  RED_OFF             					GPIO_WriteBit(GPIOB, GPIO_LED_RED_Pin,Bit_RESET) 		//ºìµÆ OFF

#define  GPIO_LED_Port                          GPIOB
#define  GPIO_LED_GREEN_Pin                     GPIO_PIN_5
#define  GPIO_LED_RED_Pin                       GPIO_PIN_3
#define  GPIO_LED_CLK                       	EN_GPIOAB


#define	 DATA_1  GPIOAB->ODATA |= (GPIO_PIN_8 << 16)
#define	 DATA_0  GPIOAB->ODATA &= ~(GPIO_PIN_8 << 16)
#define	 GET_DATA GPIOAB->IDATA & (GPIO_PIN_8 << 16)
//#define	 GET_DATA GPIO_ReadInputDataBit(GPIOC ,GPIO_PIN_12)
void LedOn(void);

void LedOff(void);

void ClearShow(void);

char* CMD_DC16(u16 x, u16 y, char *str, u8 Color);

char* CMD_DC16_Len(u16 x, u16 y, char *str, u32 length, u8 Color);

void  MainpageShow(char *Version);

const char* GetName(u16 NameIndex)  ;

void AutoTestShow(u16 NameIndex);

void MatchSuccessShow(u16 NameIndex);

void MatchFailShow(u16 NameIndex);;

void ErrorShow(void) ;

void ButtonShow(void)	;

void LanguageChangeShow(u8 LanguageMode);

void ProgressBar(u32 cnt, u32 sector);
#endif