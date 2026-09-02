#ifndef USER_OS_H
#define USER_OS_H

#include  "stdint.h"
#include  "w25qxx.h"


extern u32 Index_Select;  /*箭头选中项*/
extern u32 Index_Select_back;  /*箭头选中项*/
extern struct MenuItem(*MenuPoint);  //结构体指针,指向结构体后由内部函数指针指向功能函数
extern u8 DisplayStart; //显示时的第一个菜单项
extern u8 DisplayEnd;   //显示时的最后一个菜单项
extern u8 UserChoose;   //用户所选菜单项
extern u8 DisplayPoint; //显示指针
extern u8 MaxItems;       //同级最大菜单数
extern u8 ShowCount;        //同屏显示菜单数
extern u8 Select_Point;
extern volatile u8 LayerSwitchFlag;  //界面层切换标志位
extern volatile u8 UserLastChoose;
extern volatile u8 LastMenuRecall;
extern volatile u8 LastDispalyStartRecall;
extern volatile u8 ScreenRefresh;
extern u32 MainCount_Hex;


//读出Eflash中记录的MainCount
void ReadMainCount(void);

//初始化开始界面
void SystemParaInit(void);

//操作系统函数
void OperatingSystem(void);

//版本显示函数
void ShowVersionInterface(void);

//开机界面函数
void ShowSystemStartupInformation(void);

//系统启动数据准备函数
void SystemInformationInit(void);

//系统模块初始化函数
void SystemDrvInit(void);

void StartupScreen(u8 cnt);

void GetVersionScreen(void);

//提前进入等待检测界面
void WaitingCheckInterface(void);

//提前进入等待烧录界面
void WaitingBurnInterface(void);

//烧录等待界面
void WaitingSurfaceBurn(u8 seconds);

//检测等待函数
void WaitingSurfaceCheck(u8 StartTime, u8 EndTime);

//获取安全芯片版本
//void GetSecurityChipVersion(u8 X_base, u8 Y_base);
void GetSecurityChipVersion(u8* SecurityChipVersionTmp, u8* ScVersionStatus);

//获取从机版本
void GetSlaveVersion(u8* SlaveVersionTmp, u8* SlaveVersionStatus);

void ShowVersionFunction(u8 X_base, u8 Y_base, u8* version_buf, u8* version_status);

//获取外部Flash版本
void GetFlashVersion(u8 X_base, u8 Y_base);

//显示菜单函数
void ShowMenu(void);

//确认信息界面
void Confirm_Selection(u8 *Title_Name);

//显示RTC时间函数
void ShowRtcTime(void);

//设置RTC时间函数
void UserSetRtcTime(void);

//主板版本号获取
void TranVersionToArray(u8* VersionArr);

//从机版本号获取
void TranSlaveVersionToArray(u8* SlaveVersionArr);

void MenuTableCalculate(void);

#endif
