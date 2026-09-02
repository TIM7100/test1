#ifndef _use_lcd_H_
#define _use_lcd_H_
#include "cmd_process.h"
#include "cmd_queue.h"
#include "common.h"


 /*-触屏灵敏度-*/
#define  LEVEL_cnt1  100000       
#define  LEVEL_cnt2  150000
#define  LEVEL_cnt3  200000//适中
#define  LEVEL_cnt4  250000
#define  LEVEL_cnt5  300000

/*-页面ID-*/
enum PAGE_ID {
    PAGE_MENU             = 0x0000,   // 菜单
    PAGE_RESET_FACTORY    = 0x0001,   // 恢复出厂设置
    PAGE_UPDATE_POINT     = 0x0002,   // 更新点数
    PAGE_WIFI_SETTING     = 0x0003,   // WIFI 设置
    PAGE_RESET_DEVICE     = 0x0004,   // 重置设备
    PAGE_BOOT_START       = 0x0005,   // 开机动画开始页
    PAGE_BOOT_MAIN        = 0x0006,   // 开机页面
    PAGE_FW_UPGRADE       = 0x0007,   // 升级固件
    PAGE_FW_UPDATING      = 0x0008,   // 固件升级过程
    PAGE_FW_STATUS        = 0x0009,   // 获取固件状态
    PAGE_FW_READY         = 0x000A,   // 成功获取固件
    PAGE_JUMP_TRANSIT     = 0x000B,   // 跳转检测中转站
    PAGE_JUMP_APP_CONFIRM = 0x000C,   // 跳转 APP 确认
    PAGE_UPDATE_CONFIRM   = 0x000D,   // 更新点数确认
    PAGE_SETTING_MENU     = 0x000E,   // 设置菜单页面
    PAGE_JUMP_APP         = 0x000F,   // 跳转 APP
    PAGE_BACKLIGHT        = 0x0010,   // 背光设置
    PAGE_BOOT_END         = 0x0011,   // 开机动画结束页

    PAGE_SETTING_2        = 0x0020,   // 设置菜单页面2
    PAGE_SENSITIVITY      = 0x0021,   // 灵敏度设置
    PAGE_BUZZER           = 0x0022,   // APP蜂鸣器设置
};


void User_App(void);
void AddStr(u8* str1, u8* str2, u8 sp, u8 len);//添加字符
void Num2Str(u8* str, u16 num, u8 sp, u8 ep);
void Postback_Get_PageID(void);
void Process_Touch(PCTRL_MSG msg, uint16 size);
void Process_Postback(PCTRL_MSG msg, uint16 size) ;
void LcdMainpage(void);
void MenuMain(void);
void OTAUPDATA(void);
void OTAfail(void);
void wifijump(void);
void LcdMainpage0(void);
#endif 
