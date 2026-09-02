#ifndef _ESP8266_CONFIG_H
#define _ESP8266_CONFIG_H

#include "delay.h"
#include "common.h"
#include "LoopList.h"
#define DEBUG_ESP

#define WIFI_SSID_Addr     0x23010
#define WIFI_Password_Addr 0x23040

#define WiFi_Send(buf)  uart_send_bytes(UARTB, (UINT8*)buf, strlen((char*)buf))
#define WIFI_SET_RST_IO     REG_GPIO_SET(GPIOA) |= 0x4000000;
#define WIFI_RESET_RST_IO   REG_GPIO_CLR(GPIOA) |= 0x4000000;

typedef enum{
	SmartConfig_Uncompleted = 0,
	SmartConfig_Completed = 1
}SmartConfig_Status;

#pragma pack(1)
typedef struct WifiInfo{
	char SSID[36];
	char Password[36];
	__IO SmartConfig_Status SmartConfig_Flag;
}WifiInfo_t;
#pragma pack()

extern volatile u8 NetWork_Flag;

u8 Parse_SSID_Password(u8* buffer, WifiInfo_t* wifi_info);
void WiFi_IO_Init(void);
uint8_t WiFi_Connect_IoTServer(char *WiFiServerIP, uint16_t WiFiServerPort, int16_t timeout);
 uint8_t SmartConifg(void);
#endif
