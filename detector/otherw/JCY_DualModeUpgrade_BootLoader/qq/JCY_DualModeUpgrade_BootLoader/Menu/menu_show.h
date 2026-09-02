#ifndef _MENU_SHOW_H__
#define _MENU_SHOW_H__

#include "fxx_std.h"

#define Chinese			1
#define English			0

#define PasswordRight        0
#define PasswordError        1
#define KeyExit              3

// 事件运行加载时的对应代码合集
enum {
	LOAD_RUNNING = 0,
	DOWNLOAD_RUNNING,
	UPDATE_RUNNING
};

//更新固件包时，固件包执行的操作的对应Flag
enum {
	ADD_COMMAND = 0,
	UPDATA_COMMAND
};


enum {
	LANGUAGE_SET_SUCC = 0xF0,
	FACTORY_RESET = 0xF1
};


void DeviceInitShow(void);
void CheckSDCardShow(void);
void WifiInitShow(void);

void MainPageShow(u8 ShowLanguage, char* Version);
void MenuShow(u8 ShowLanguage);
void ProgressBarShow(u32 AllCut, u32 CurrentCut);

//跳转事件相关显示
void JumpResultShow(u8 ShowLanguage, u8 JumpResultCode);
//END

//恢复出厂设置相关显示
void ChooseFactoryResetShow(u8 ShowLanguage);
void FactoryResetShow(u8 ShowLanguage);
//设置语言和出厂设置结果显示
void SetResultShow(u8 ShowLanguage, u8 Result);
//END

//加载过程显示
void RunningShow(u8 ShowLanguage, u8 Function);

//OTA相关显示
void AnalysisShow(u8 ShowLanguage);
void ConnectShow(u8 ShowLanguage);
void TimeOutShow(u8 ShowLanguage);
void WaitingFirmwareShow(u8 ShowLanguage);
void ChooseDownloadShow(u8 ShowLanguage);
void DownloadModeShow(u8 ShowLanguage, u8 Mode);
void OTAResultShow(u8 ShowLanguage, u8 Result);

//影子设备--升级计数显示
//void ShadowCountShow(u8 ShowLanguage, u8 Result, u32 HP95_90Count, u32 HP97Count);
u8 Password_Control(u8 *RightPassword);
void Password_ErrorShow(void);
void Password_SuccessShow(void);
void ConfirmNetworkConnect_ErrorsShow(void);
void StartResume_Show(void);
#endif
