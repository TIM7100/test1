#ifndef _MENU_SHOW_H
#define _MENU_SHOW_H

#include  "common.h"
//等待用户确认操作显示标志
#define  Download_Firmware   0  	//下载
#define  Factory_Reset       1		//恢复出厂设置
#define  JUMP_APPLICATION    2		//跳转APP

//用户确认操作后，显示相应进度标志
#define  DOWNLOAD_RUNNING    0      //下载中
#define  UPDATE_RUNNING      1		//校验中
#define  JUMP_APP_RUNNING    2		//跳转APP中

#define  CN  0
#define  EN  1


void DeviceInitShow(void);
void LcdMainpage(void);
void ConnectTencentCloudShow(void);
void Interface_Display(void);
void Ethernet_Action(void);
void GetOTAInfoShow(u8 Result);
void ConnectShow(void);
void DownloadFirmwareShow(u8 Result);
void VerifyFirmwareShow(u8 Result);
u32 WaitingUserPress(u8 Action);
void ProgressBarShow(u32 AllCut, u32 CurrentCut);
void RunningShow(u8 Action);
void ProgressBarShow(u32 AllCut, u32 CurrentCut);
void ResultShow(u8 Action);
void JumpAppShow(u8 Result);
void DeviveAbnormalShow(void);
void LanguageSetResultShow(void);
void ShadowCountShow(u8 Result);

//wifi连接显示
void WIFI_ConnectShow(void);
void SmartConfig_RequestShow(void);
void SmartConfig_SuccessShow(void);
void SmartConfig_ErrorShow(void);
void WIFI_ConnectSuccesShow(void);


void APPcomming(void);
#endif


