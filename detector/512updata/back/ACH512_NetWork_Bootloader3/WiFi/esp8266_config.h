#ifndef _ESP8266_CONFIG_H
#define _ESP8266_CONFIG_H

// #include "fxx_std.h"
// #include "bsp_usart1.h"
#include "uart.h"
#include  "common.h"
#include "string.h"
#include "delay.h"

//#define DEBUG_ESP

#define WIFI_SSID_Addr     0x23010
#define WIFI_Password_Addr 0x23040

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
// void WiFi_IO_Init(void);
void WiFi_Init(void);
uint8_t WiFi_Connect_IoTServer(char *WiFiServerIP, uint16_t WiFiServerPort, int16_t timeout);

#endif
