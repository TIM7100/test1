#include "UserFunctionInterface.h"
#include "menu.h"
#include "menu_show.h"
#include "bsp_timer7.h"
#include "bsp_key.h"
#include "bsp_eflash.h"
#include "ota_firmware_updata.h"
#include "handle_firmware.h"
#include "shadow.h"
#include "tcp_client.h"
#include "w5500_user_conf.h"

#define WAITING_USER_CHOOSE  0x00
#define RETURN_MENU          0xFF

u8 UserPassword[4] = {1, 2, 3, 4};

extern u8 *LanguageChoose;

static u8 JumpEventHandle();
static u8 LanguageEventHandle(u8 ChooseLanguage);
static u8 FactoryResetEventHandle(void);
static u8 OTAEventHandle(void);
static u8 StartDownloadFirmwareFunc(u8 DownloadCommand);
static u8 CountEventHandle(void);

__IO u8 NetWork_Flag = ESP8266;

u8 WaitingGetUserChoose(void)
{
    u8 KeyValue = KEY_NO_DOWN;

    while (1)
    {
        KeyValue =  KeyScan();
        if (KeyValue == KEY_ENTER_VALUE)
        {
            break;
        }
        else if (KeyValue == KEY_ESC_VALUE)
        {
            break;
        }
    }
    //System_Delay_MS(100);
    return KeyValue;
}

void UserFunctionInterface(u8 UserChooseEvent, u8 ChooseNode)
{
    u8 EventResult;

    switch (UserChooseEvent)
    {
    case JUMP_EVENT:                                            //跳转功能
        RunningShow(*LanguageChoose, LOAD_RUNNING);
        EventResult = JumpEventHandle();
        JumpResultShow(*LanguageChoose, EventResult);
        EventResult = WAITING_USER_CHOOSE;
        break;
    case OTA_EVENT:                                             //OTA更新固件包功能
        EventResult = OTAEventHandle();
        if (EventResult != WAITING_USER_CHOOSE)
        {
            EventResult = StartDownloadFirmwareFunc(EventResult);
        }
        break;

//    case COUNT_EVENT:                                           //点数更新功能
//        EventResult = CountEventHandle();
//        break;
//
    case LANGUAGE_EVENT:                                        //语言选择功能
        EventResult = LanguageEventHandle(ChooseNode);
        SetResultShow(*LanguageChoose, LANGUAGE_SET_SUCC);
        break;
    case FACTORY_RESET_EVENT:                                   //恢复出厂设置功能
        //ChooseFactoryResetShow(*LanguageChoose);
        EventResult = FactoryResetEventHandle();
        if (EventResult == WAITING_USER_CHOOSE)
        {
            SetResultShow(*LanguageChoose, FACTORY_RESET);
        }
        break;
    }

    if (EventResult == WAITING_USER_CHOOSE)
    {
        while (WaitingGetUserChoose() != KEY_ESC_VALUE);        //等待用户按下返回键
    }
}

static u8 JumpEventHandle()
{
    u8 Ret;

    //Timer7_Stop();
    Ret = LoadCodeIntoFlash();                      //加载Menu程序到Flash区域
    if (Ret == JUMP_FIRMWARE_EMPTY || Ret == JUMP_READ_SD_FAIL)
    {
        return Ret;                                 //检查到固件包有错误时，跳转失败
    }
    else if (Ret == JUMP_FIRMWARE_NEW)              //检查到有新固件包在下载导致的跳转失败
    {
        return Ret;
    }
    JumpMenuCode();                                 //固件包读取成功，跳转到MENU菜单
    return Ret;
}

static u8 LanguageEventHandle(u8 ChooseLanguage)
{
    //写入Flash
    EflashEraseArea(LANGUAGE_ADDRESS, 512);                      // 擦除待写入的Flash区域
    EflashWritePage(LANGUAGE_ADDRESS, 1, &ChooseLanguage);       // 写入Menu指定的存取区域
    return WAITING_USER_CHOOSE;
}

static u8 FactoryResetEventHandle(void)
{
    u8 KeyValue;
    u8 ret;

    System_Delay_MS(100);
	
    //FactoryResetShow(*LanguageChoose);
    ret = Password_Control(UserPassword);   //密码控件
    if (PasswordRight == ret)
    {
        Password_SuccessShow();
        while (1)        //等待用户按下按键
        {
            KeyValue = WaitingGetUserChoose();
            if (KeyValue == KEY_ENTER_VALUE)    //按下确认，开始恢复出厂设置
            {
                StartResume_Show();
                break;
            }
            else if (KeyValue == KEY_ESC_VALUE) //返回界面
            {
                return RETURN_MENU;
            }
        }
		
		W5500Reset();
		if(W5500PhyLinkCheck())
		{
			NetWork_Flag = W5500;
		}
		else
		{
			NetWork_Flag = ESP8266;
		}
		
        if (FactoryReset() != CONNECT_SUCCESS)
        {
            ConfirmNetworkConnect_ErrorsShow();     //网线未连接
            return RETURN_MENU;
        }
    }
    else if (PasswordError == ret)
    {
        Password_ErrorShow();
        return RETURN_MENU;
    }
    else if (KeyExit == ret)
    {
        return RETURN_MENU;
    }

    return WAITING_USER_CHOOSE;
}

/****************************************************************
 * @brief    检测是否有新固件包界面，并确认是否更新新的固件包
 * @param    xxx:xxx
 * @return   OTA_COMMAND_ADD： 接收到的OTA升级指令为添加新系列
 *           OTA_COMMAND_UPDATA： 接收到的OTA升级指令为更新固件包
*****************************************************************/
static u8 OTAEventHandle(void)
{
    u8 TimeOut = 3;
    u8 ErrCode = 0;
    u8 FirmwareState = 0;
    u8 OTA_RunningFlag = OTA_SUCCESS;
	
    /* 1. 获取当前固件包的状态，共有三种状态：FS_IDLE、FS_DOWNLOADING、FS_ANALYSIS */
    FirmwareState = GetFirmwareDownloadState();
    //FS_ANALYSIS: 表示固件包已下载成功，但未解析成功，可以直接进行解析固件包
    if (FirmwareState == FS_ANALYSIS)
    {
        AnalysisShow(*LanguageChoose);
        //return OTA_COMMAND_ANALYSIS;
        OTA_RunningFlag = OTA_COMMAND_ANALYSIS;
    }
    //FS_IDLE 和 FS_DOWNLOADING：这两个状态都表示需要连接云平台申请下载固件包
    else
    {
        ConnectShow(*LanguageChoose);                                   //连接显示
		
		W5500Reset();
		if(W5500PhyLinkCheck())
		{
			NetWork_Flag = W5500;
		}
		else
		{
			NetWork_Flag = ESP8266;
		}
		
        while (ConnectTencentCloud() != OTA_SUCCESS)                    //连接腾讯云云平台, 连接失败时, 最多重连3次
        {
            if ((TimeOut--) == 0)
            {
                TimeOutShow(*LanguageChoose);                           //连接网络超时显示
                return OTA_RunningFlag;
            }
        }

        WaitingFirmwareShow(*LanguageChoose);                                                   //等待接收固件包显示
        OTA_RunningFlag = AnalyseOTADownloadInfo();                                             //分析OTA固件信息，判断是否拥有下载指令
        DisconnectTencentCloud();                                                               //接收固件结束，断开云平台连接
        if ((OTA_RunningFlag == OTA_COMMAND_ADD) || (OTA_RunningFlag == OTA_COMMAND_UPDATA) || \
                (OTA_RunningFlag == OTA_COMMAND_DELETE_UPDATE))             //有更新指令返回更新指令
        {
            ChooseDownloadShow(*LanguageChoose);
            return OTA_RunningFlag;
        }
        //运行到这里, 表示没有更新指令, 原因当前版本为最新 \ 没接收到固件更新指令 \ 接收固件包失败
        HandleDownloadFirmwareState(OTA_RunningFlag);
        return OTA_SUCCESS;
    }
    return OTA_RunningFlag;
}

/*************************************************************************************
 * @brief    开始下载固件阶段
 * @param
 *           DownloadCommand:  该值为固件下载的指令, 有:
 *                             ADD_SERIES      增加新系列命令
 *                             UPDATA_SERIES   更新系列命令
 * @return   无
**************************************************************************************/
static u8 StartDownloadFirmwareFunc(u8 DownloadCommand)
{
    u8 KeyValue;
    u8 AddSeriesCount = 0xff;
    u8 DownloadState = DownloadCommand;

    // 等待用户判断是否继续下载或\解析固件包
    KeyValue =  WaitingGetUserChoose();
    if (KeyValue == KEY_ESC_VALUE)
    {
        return RETURN_MENU;
    }
    else if (KeyValue == KEY_ENTER_VALUE)
    {
        if (DownloadCommand == OTA_COMMAND_ADD || DownloadCommand == OTA_COMMAND_UPDATA || DownloadCommand == OTA_COMMAND_DELETE_UPDATE)
        {
            RunningShow(*LanguageChoose, DOWNLOAD_RUNNING);                                   //显示下载进度条
            DownloadState = HandleDownloadFirmwareCommand(DownloadCommand, &AddSeriesCount);  //开始下载固件包, 并返回固件包下载结果
        }

        HandleDownloadFirmwareState(DownloadState);                                           //处理下载结果函数
    }
    return WAITING_USER_CHOOSE;
}

//static u8 CountEventHandle(void)
//{
//    u8 TimeOut = 3;
//    u8 Ret;
//    u32 HP95_90Count;
//    u32 HP97Count;
//
//    //连接云平台
//    ConnectShow(*LanguageChoose);                                 //连接显示
//    while (ConnectTencentCloud() != OTA_SUCCESS)                  //连接腾讯云云平台, 连接失败时, 最多重连3次
//    {
//        if ((TimeOut--) == 0)
//        {
//            TimeOutShow(*LanguageChoose);                         //连接网络超时显示
//            return WAITING_USER_CHOOSE;
//        }
//    }
//
//    //更新影子设备信息
//    Ret = GetShadowInfo();
//    ThingUploadUpdataCount();               //上传物模型
//    GetUpdataCount(&HP95_90Count, &HP97Count);
//    ShadowCountShow(*LanguageChoose, Ret, HP95_90Count, HP97Count);
//
//    //断开连接
//    DisconnectTencentCloud();                                     //接收固件结束，断开云平台连接
//    return WAITING_USER_CHOOSE;
//}
