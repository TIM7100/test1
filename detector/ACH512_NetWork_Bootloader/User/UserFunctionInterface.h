#ifndef _USERFUNCTIONINTERFACE_H
#define _USERFUNCTIONINTERFACE_H

#include "common.h"

#define DEVICE_NORMAL 	 0
#define DEVICE_ABNORMAL  1

u8 OTA_Event_Handle(void);
void DeviceInit(void);
u8 Factory_Reset_Event_Handle(void);
u8 Jump_App_Handle(void);
u8 CountEventHandle(void);
void SetLanguageHandle(u8 Language);
#endif


