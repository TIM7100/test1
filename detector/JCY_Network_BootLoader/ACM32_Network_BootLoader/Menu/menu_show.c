#include "menu_show.h"
#include "menu_app.h"
#include "bsp_uart_lcd.h"
#include "bsp_sdcard.h"
#include "cloud_platform_para.h"
#include "handle_firmware.h"
#include "shadow.h"
#include "bsp_key.h"

u8 UserLastChoose;

static void ClearShow(void)    //清空全部显示效果
{
    LCD_UartSend("CLR(0);\r\n");    //清屏
    LCD_UartSend("DIR(1);\r\n");
}

void DeviceInitShow(void)
{
    ClearShow();
    LCD_UartSend("DC16(24,50,'设备初始化中...',2);\r\n");
    LCD_UartSend("DC16(32,70,'device init...',2);\r\n");
}


void CheckSDCardShow(void)
{
    while (SD_Initialize() != SD_RESPONSE_NO_ERROR)
    {
        ClearShow();
        LCD_UartSend("DC16(20,50,'请检查SD卡',1);\r\n");
        LCD_UartSend("DC16(0,70,'Please Check SD Card',1);\r\n");

        System_Delay_MS(2000);
    }
}

void ProgressBarShow(u32 AllCut, u32 CurrentCut)
{
    char BoxBuf[32] = {0};

    sprintf(BoxBuf, "BOXF(10,75,%02d,90,5);\r\n", ((140 * CurrentCut) / AllCut) + 10);
    LCD_UartSend(BoxBuf);
}

/************* 主页面 *************/
void MainPageShow(u8 ShowLanguage, char* Version)          //主页面显示
{
    ClearShow();

    if (ShowLanguage == Chinese)
    {
        LCD_UartSend("DC24(56,10,'主页',2);\r\n");

        LCD_UartSend("DC16(7,46,'设备名：',4);\r\n");
        LCD_UartSend(CMD_DC16(76, 46, DEVICE_NAME, 4));        //显示设备名

        LCD_UartSend("DC16(7,66,'设备版本：',4);\r\n");
        LCD_UartSend(CMD_DC16(88, 66, DEVICE_VERSION, 4));     //显示版本号

        LCD_UartSend("DC16(7,86,'固件：',4);\r\n");
        LCD_UartSend(CMD_DC16(60, 86, Version, 4));            //显示版本号

        LCD_UartSend("DC16(12,110,'- \"Enter\"继续 -',4);\r\n");
    }
    else if (ShowLanguage == English)
    {
        LCD_UartSend("DC24(26,10,'Home Page',2);\r\n");

        //显示设备名
        LCD_UartSend("DC16(7,38,'Device:',4);\r\n");
        LCD_UartSend(CMD_DC16(68, 38, DEVICE_NAME, 4));

        //显示设备版本
        LCD_UartSend("DC16(7,54,'DeviceVer:',4);\r\n");
        LCD_UartSend(CMD_DC16(96, 54, DEVICE_VERSION, 4));

        //显示固件版本
        LCD_UartSend("DC16(7,70,'Firmware:',4);\r\n");
        LCD_UartSend(CMD_DC16(50, 86, Version, 4));

        LCD_UartSend("DC16(10,110,'- \"Enter\"Next -',4);\r\n");
    }
}

/*******************************************************************************
* Function Name  : MenuShow
* Description    : 目录显示函数
* Input          : None
* Output         : None
* Return         : None
* Remark         :
    通过修改 MenuPoint 的指向
    DisplayStart 来修改menu节点的开始位置
    NodeNum      修改显示的菜单节点
*******************************************************************************/
void MenuShow(u8 ShowLanguage)
{
    u8 color;
    u8 n;
    u8 LastNode;
    u8 NowNode;
    u8 ShowNodePoint;
    u8 UserChoosePoint = SelectPoint;
    char Num[4] = {"00."};

    n = 0;
    MaxNodes = MenuPoint[NodeNum].MenuCount; //第一个目录指向了MainMenu

    if (RefreshFlag != 0)
    {
        ClearShow();
        RefreshFlag = 0;
        if (ShowLanguage == Chinese)
        {
            LCD_UartSend("DC16(35,0,'请执行操作',15);\r\n");
        }
        else if (ShowLanguage == English)
        {
            LCD_UartSend("DC16(51,0,'FUNCTION',15);\r\n");
        }

        /* 显示当前页面的可选项 */
        for (ShowNodePoint = DisplayStart; ShowNodePoint < DisplayEnd; ShowNodePoint++)
        {
            if ((n + 1) == UserChoosePoint)  // 如果是选中行，则显示不同的颜色
            {
                color = 6;
            }
            else
            {
                color = 4;
            }
            sprintf(Num, "%02d.", ShowNodePoint + 1);
            LCD_UartSend(CMD_DC16(10, 25 + (n * 20), Num, color));
            LCD_UartSend(CMD_DC16(10 + 24, 25 + (n * 20), MenuPoint[ShowNodePoint].DisplayString[ShowLanguage], color));
            n++;
        }
    }
    else if (NodeChangFlag)                  //上下行切换时颜色变化
    {
        NodeChangFlag = 0;
        LastNode = UserLastChoose - 1;
        NowNode = UserChoosePoint - 1;
        if (NowNode != LastNode)
        {
            sprintf(Num, "%02d.", DisplayStart + LastNode + 1);
            LCD_UartSend(CMD_DC16(10, 25 + (LastNode * 20), Num, 4));
            LCD_UartSend(CMD_DC16(34, 25 + (LastNode * 20), MenuPoint[DisplayStart + LastNode].DisplayString[ShowLanguage], 4));

            sprintf(Num, "%02d.", DisplayStart + NowNode + 1);
            LCD_UartSend(CMD_DC16(10, 25 + (NowNode * 20), Num, 6));
            LCD_UartSend(CMD_DC16(34, 25 + (NowNode * 20),  MenuPoint[DisplayStart + NowNode].DisplayString[ShowLanguage], 6));
        }
    }
    UserLastChoose = UserChoosePoint;
}

void JumpResultShow(u8 ShowLanguage, u8 JumpResultCode)
{
    ClearShow();
    if (ShowLanguage == Chinese)
    {
        if (JumpResultCode == JUMP_FIRMWARE_EMPTY)
        {
            LCD_UartSend("DC16(34,30,'没找到固件包',2);\r\n");
            LCD_UartSend("DC16(34,46,'请更新固件包',2);\r\n");
        }
        else if (JumpResultCode == JUMP_FIRMWARE_NEW)
        {
            LCD_UartSend("DC16(34,30,'存在新固件包',2);\r\n");
            LCD_UartSend("DC16(34,46,'请更新固件包',2);\r\n");
        }
        else if (JumpResultCode == JUMP_READ_SD_FAIL)
        {
            LCD_UartSend("DC16(20,50,'请检查SD卡',1);\r\n");
        }
        //LCD_UartSend("DC16(16,90,'- \"Enter\"继续 -',4);\r\n");
        StrMiddleShow("- \"Cancel\"取消 -", 106, 4);
        //LCD_UartSend("DC16(12,110,'- \"Cancel\"取消 -',4);\r\n");
    }
    else if (ShowLanguage == English)
    {
        if (JumpResultCode == JUMP_FIRMWARE_EMPTY)
        {
            LCD_UartSend("DC16(8,30,'Firmware not found',2);\r\n");
            LCD_UartSend("DC16(32,46,'Please update',2);\r\n");
        }
        else if (JumpResultCode == JUMP_FIRMWARE_NEW)
        {
            LCD_UartSend("DC16(34,30,'New firmware exist',2);\r\n");
            LCD_UartSend("DC16(0,46,'Please update the firmware',2);\r\n");
        }
        else if (JumpResultCode == JUMP_READ_SD_FAIL)
        {
            LCD_UartSend("DC16(8,70,'Please Check SD Card',1);\r\n");
        }
        //LCD_UartSend("DC16(16,90,'- \"Enter\"Next -',4);\r\n");
        LCD_UartSend("DC16(8,110,'- \"Cancel\"Return -',4);\r\n");
    }
}

void ChooseFactoryResetShow(u8 ShowLanguage)          //恢复出厂设置页面显示
{
    ClearShow();
    if (ShowLanguage == Chinese)
    {
        LCD_UartSend("DC16(16,42,'是否恢复出厂设置',2);\r\n");
        LCD_UartSend("DC16(16,90,'- \"Enter\"继续 -',4);\r\n");
        LCD_UartSend("DC16(12,110,'- \"Cancel\"取消 -',4);\r\n");
    }
    else if (ShowLanguage == English)
    {
        LCD_UartSend("DC16(20,42,'restore factory ?',2);\r\n");
        LCD_UartSend("DC16(16,90,'- \"Enter\"Next -',4);\r\n");
        LCD_UartSend("DC16(8,110,'- \"Cancel\"Return -',4);\r\n");
    }
}

void FactoryResetShow(u8 ShowLanguage)
{
    ClearShow();
    if (ShowLanguage == Chinese)
    {
        LCD_UartSend("DC16(16,55,'恢复出厂设置中',5);\r\n");
    }
    else if (ShowLanguage == English)
    {
        LCD_UartSend("DC16(16,55,'Factory Reset...',5);\r\n");
    }
}

void SetResultShow(u8 ShowLanguage, u8 Result)
{
    ClearShow();
    if (ShowLanguage == Chinese)
    {
        switch (Result)
        {
        case LANGUAGE_SET_SUCC:
        case FACTORY_RESET:
            LCD_UartSend("DC24(38,50,'设置成功',2);\r\n");
            break;
        }
        LCD_UartSend("DC16(16,110,'- \"Cancel\"返回 -',4);\r\n");
    }
    else if (ShowLanguage == English)
    {
        switch (Result)
        {
        case LANGUAGE_SET_SUCC:
        case FACTORY_RESET:
            LCD_UartSend("DC16(24,56,'Set Successfully',2);\r\n");
            break;
        }
        LCD_UartSend("DC16(8,110,'- \"Cancel\"Return -',4);\r\n");
    }
}


void RunningShow(u8 ShowLanguage, u8 Function)          //加载、下载页面显示
{
    ClearShow();
    if (ShowLanguage == Chinese)
    {
        if (Function == LOAD_RUNNING)
        {
            LCD_UartSend("DC16(50,55,'加载中...',5);\r\n");
        }
        else if (Function == DOWNLOAD_RUNNING)
        {
            LCD_UartSend("DC16(50,55,'下载中...',5);\r\n");
        }
        else if (Function == UPDATE_RUNNING)
        {
            LCD_UartSend("DC16(18,55,'更新本地固件包...',5);\r\n");
        }
    }
    else if (ShowLanguage == English)
    {
        if (Function == LOAD_RUNNING)
        {
            LCD_UartSend("DC16(50,55,'Loading...',5);\r\n");
        }
        else if (Function == DOWNLOAD_RUNNING)
        {
            LCD_UartSend("DC16(28,55,'Downloading...',5);\r\n");
        }
        else if (Function == UPDATE_RUNNING)
        {
            LCD_UartSend("DC16(40,55,'Updating...',5);\r\n");
        }
    }
    LCD_UartSend("BOX(10,75,150,90,16);\r\n");
}

void AnalysisShow(u8 ShowLanguage)
{
    ClearShow();
    if (ShowLanguage == Chinese)
    {
        LCD_UartSend("DC16(24,28,'新固件包已下载',2);\r\n");
        LCD_UartSend("DC16(8,44,'是否更新本地固件包',2);\r\n");
        LCD_UartSend("DC16(16,90,'- \"Enter\"继续 -',4);\r\n");
        LCD_UartSend("DC16(12,110,'- \"Cancel\"取消 -',4);\r\n");
    }
    else if (ShowLanguage == English)
    {
        LCD_UartSend("DC16(0,28,'Firmware has been downloaded',2);\r\n");
        LCD_UartSend("DC16(24,28,'Update or not',2);\r\n");
        LCD_UartSend("DC16(16,90,'- \"Enter\"Next -',4);\r\n");
        LCD_UartSend("DC16(8,110,'- \"Cancel\"Return -',4);\r\n");
    }
}

void ConnectShow(u8 ShowLanguage)          //连接页面显示
{
//    ClearShow();
	Interface_Display();
	Ethernet_Action();
    if (ShowLanguage == Chinese)
    {
        LCD_UartSend("DC16(30,110,'网络连接中...',4);\r\n");
    }
    else if (ShowLanguage == English)
    {
        LCD_UartSend("DC16(30,110,'Connecting...',4);\r\n");
    }
}

void TimeOutShow(u8 ShowLanguage)
{
    ClearShow();
    if (ShowLanguage == Chinese)
    {
        LCD_UartSend("DC16(8,39,'请检查网线是否连接',1);\r\n");
        LCD_UartSend("DC16(22,55,'并检查网络状态',1);\r\n");
        LCD_UartSend("DC16(16,110,'- \"Cancel\"返回 -',4);\r\n");
    }
    else if (ShowLanguage == English)
    {
        LCD_UartSend("DC16(0,39,'Please check network',1);\r\n");
        LCD_UartSend("DC16(8,55,'and network cable',1);\r\n");
        LCD_UartSend("DC16(8,110,'- \"Cancel\"Return -',4);\r\n");
    }
}

void WaitingFirmwareShow(u8 ShowLanguage)          //等待接收固件包
{
    ClearShow();
    if (ShowLanguage == Chinese)
    {
        LCD_UartSend("DC16(22,55,'等待接收固件包..',5);\r\n");
    }
    else if (ShowLanguage == English)
    {
        LCD_UartSend("DC16(16,55,'Waiting Firmware',5);\r\n");
    }
}

void ChooseDownloadShow(u8 ShowLanguage)
{
    ClearShow();
    if (ShowLanguage == Chinese)
    {
        LCD_UartSend("DC16(8,28,'新固件包，是否下载',2);\r\n");
        LCD_UartSend("DC16(16,90,'- \"Enter\"继续 -',4);\r\n");
        LCD_UartSend("DC16(12,110,'- \"Cancel\"取消 -',4);\r\n");
    }
    else if (ShowLanguage == English)
    {
        LCD_UartSend("DC16(11,40,'Download Firmware',2);\r\n");
        LCD_UartSend("DC16(16,90,'- \"Enter\"Next -',4);\r\n");
        LCD_UartSend("DC16(8,110,'- \"Cancel\"Return -',4);\r\n");
    }
}

void DownloadModeShow(u8 ShowLanguage, u8 Mode)
{
    if (ShowLanguage == Chinese)
    {
        if (Mode == ADD_COMMAND)
        {
            LCD_UartSend("DC16(34,8,'-添加新系列-',2);\r\n");
        }
        else if (Mode == UPDATA_COMMAND)
        {
            LCD_UartSend("DC16(34,8,'-下载固件包-',2);\r\n");
        }
    }
    else if (ShowLanguage == English)
    {
        if (Mode == ADD_COMMAND)
        {
            LCD_UartSend("DC16(14,8,'-ADD New Series-',2);\r\n");
        }
        else if (Mode == UPDATA_COMMAND)
        {
            LCD_UartSend("DC16(8,8,'-Download Firmware-',2);\r\n");
        }
    }
}

void OTAResultShow(u8 ShowLanguage, u8 Result)
{
    char ErrCode[15] = {0};
    ClearShow();

    sprintf(ErrCode, "Error Code %d", Result);
    if (ShowLanguage == Chinese)
    {
        switch (Result)
        {
        case OTA_COMMAND_ERR:
        case OTA_LOSE_INFO:    //固件获取失败
            LCD_UartSend("DC16(20,40,'获取固件包失败',1);\r\n");
            LCD_UartSend(CMD_DC16(30, 66, ErrCode, 1));
            break;
        case OTA_ADD_SERIES_ZERO:
            LCD_UartSend("DC16(20,40,'添加的系列数为0',1);\r\n");
            break;
        case OTA_DONT_UPDATA:
            LCD_UartSend("DC16(20,50,'当前为最新版本',2);\r\n");
            break;
        case OTA_SUCCESS:
            LCD_UartSend("DC24(26,40,'更新成功',2);\r\n");
            break;
        default:
            LCD_UartSend("DC24(34,40,'下载失败',1);\r\n");
            LCD_UartSend(CMD_DC16(30, 66, ErrCode, 1));
            break;
        }
        LCD_UartSend("DC16(16,110,'- \"Cancel\"返回 -',4);\r\n");
    }
    else if (ShowLanguage == English)
    {
        switch (Result)
        {
        case OTA_COMMAND_ERR:
        case OTA_LOSE_INFO:    //固件获取失败
            LCD_UartSend("DC16(18,50,'Get Firmware Err',1);\r\n");
            LCD_UartSend(CMD_DC16(30, 68, ErrCode, 1));
            break;
        case OTA_ADD_SERIES_ZERO:
            LCD_UartSend("DC16(18,50,'Added series is 0',1);\r\n");
            break;
        case OTA_DONT_UPDATA:
            LCD_UartSend("DC16(24,55,'Newest Firmware',2);\r\n");
            break;
        case OTA_SUCCESS:
            LCD_UartSend("DC16(10,48,'Updata Successfully',2);\r\n");
            break;
        default:
            LCD_UartSend("DC16(26,48,'Fail to download',1);\r\n");
            LCD_UartSend(CMD_DC16(30, 68, ErrCode, 1));
            break;
        }
        LCD_UartSend("DC16(8,110,'- \"Cancel\"Return -',4);\r\n");
    }
}

void ShadowCountShow(u8 ShowLanguage, u8 Result, u32 HP95_90Count, u32 HP97Count)
{
    char HP97CountBuff[6];
    char HP95_90CountBuff[6];

    ClearShow();
  sprintf(HP97CountBuff, "%d", HP97Count);
    sprintf(HP95_90CountBuff, "%d", HP95_90Count);
  if (ShowLanguage == Chinese)
  {
      switch (Result)
      {
        case SHADOW_VERSION_ALIKE:
            LCD_UartSend("DC16(80,80,'最新',15);\r\n");
            break;
      case SHADOW_SUCCESS:
            LCD_UartSend("DC16(80,80,'更新成功',2);\r\n");
            break;
        case SHADOW_VERSION_ERR:                //更新计数获取失败
      case SHADOW_GET_FAIL:
            LCD_UartSend("DC16(80,80,'更新失败',1);\r\n");
          break;
      }
        LCD_UartSend("DC16(24,4,'-剩余升级点数-',15);\r\n");

        LCD_UartSend("DC16(8,32,'HP95_90:',15);\r\n");
        LCD_UartSend(CMD_DC16(80, 32, HP95_90CountBuff, 2));

        LCD_UartSend("DC16(16,56,'HP97:',15);\r\n");
        LCD_UartSend(CMD_DC16(80, 56, HP97CountBuff, 2));

        LCD_UartSend("DC16(8,80,'更新状态:',15);\r\n");

        LCD_UartSend("DC16(16,110,'- \"Cancel\"返回 -',4);\r\n");
  }
  else if (ShowLanguage == English)
  {
        switch (Result)
      {
      case SHADOW_VERSION_ALIKE:
            LCD_UartSend("DC16(90,90,'newest',15);\r\n");
            break;
      case SHADOW_SUCCESS:
            LCD_UartSend("DC16(90,90,'update successfully',2);\r\n");
            break;
        case SHADOW_VERSION_ERR:                //更新计数获取失败
      case SHADOW_GET_FAIL:
            LCD_UartSend("DC16(90,90,'update failed',1);\r\n");
          break;
      }
        LCD_UartSend("DC16(14,4,'-upgrade points-',15);\r\n");
        LCD_UartSend("DC16(8,32,'HP95_90:',15);\r\n");
        LCD_UartSend(CMD_DC16(80, 32, HP95_90CountBuff, 2));

        LCD_UartSend("DC16(16,52,'HP97:',15);\r\n");
        LCD_UartSend(CMD_DC16(80, 52, HP97CountBuff, 2));
        LCD_UartSend("DC16(8,72,'update status:',15);\r\n");
        LCD_UartSend("DC16(8,110,'- \"Cancel\"Return -',4);\r\n");
  }
}

void Password_SuccessShow(void)
{
    ClearShow();
    LCD_UartSend("DC16(40,30,'密码正确',2);\r\n");
    LCD_UartSend("DC16(20,70,'请确认连接网线',4);\r\n");
    LCD_UartSend("DC16(10,100,'-按下-Enter-确认-',4);\r\n");
}

void StartResume_Show(void)
{
    ClearShow();
    LCD_UartSend("DC16(10,50,'恢复出厂设置中...',2);\r\n");
    System_Delay_MS(1000);
}

void ConfirmNetworkConnect_ErrorsShow(void)
{
    ClearShow();
    LCD_UartSend("DC16(30,30,'无法连接网络',1);\r\n");
    LCD_UartSend("DC16(5,70,'请确认是否连接网线',4);\r\n");
    LCD_UartSend("DC16(20,100,'即将返回菜单...',4);\r\n");
    System_Delay_MS(4000);
}

void Password_ErrorShow(void)
{
    ClearShow();
    LCD_UartSend("DC16(40,30,'密码错误',1);\r\n");
    LCD_UartSend("DC16(25,80,'即将返回菜单...',4);\r\n");
    System_Delay_MS(2000);
}



void PasswordControl_StartScreen(void)
{
    LCD_UartSend("DC16(30,10,'-请输入密码-',2);\r\n");
    LCD_UartSend("DC16(5,110,'按下Enter进入下一位',2);\r\n");
    LCD_UartSend("BOX(10,50,45,90,5);\r\n");
    LCD_UartSend("BOX(45,50,80,90,5);\r\n");
    LCD_UartSend("BOX(80,50,115,90,5);\r\n");
    LCD_UartSend("BOX(115,50,150,90,5);\r\n");
    LCD_UartSend("DC24(22,57,'0',5);\r\n");
    LCD_UartSend("DC24(57,57,'0',5);\r\n");
    LCD_UartSend("DC24(92,57,'0',5);\r\n");
    LCD_UartSend("DC24(127,57,'0',5);\r\n");
}

u8 Password_Control(u8 *RightPassword)
{
    u8 KeyValue = KEY_NO_DOWN;
    u8 PasswordLength = 4;             //密码长度
    u8 PasswordNum[4] = {0};           //当前一位的密码
    u8 EnterKeyFlag = 0;               //确认按键扫描标志
    u8 CancelKeyFlag = 0;              //退出按键扫描标志
    char str[2] = {0, '\0'};

    ClearShow();
    PasswordControl_StartScreen();
    LCD_UartSend("SBC(0);\r\n");

    for (u8 i = 0; i < PasswordLength; i++)
    {
        LCD_UartSend("BOX(10,50,45,90,5);\r\n");
        LCD_UartSend("BOX(45,50,80,90,5);\r\n");
        LCD_UartSend("BOX(80,50,115,90,5);\r\n");
        LCD_UartSend("BOX(115,50,150,90,5);\r\n");
        LCD_UartSend(CMD_BOX(10 + i * 35, 50, 45 + i * 35, 90, 4));
        PasswordNum[i] = 0;

        while (1)
        {
            KeyValue = KeyScan();

            switch (KeyValue)
            {
            case KEY_ENTER_VALUE:
                EnterKeyFlag = 1;
                break;

            case KEY_ESC_VALUE:
                CancelKeyFlag = 1;
                break;

            case KEY_UP_VALUE:
                if (PasswordNum[i] < 9)
                {
                    PasswordNum[i]++;
                }
                str[0] = PasswordNum[i] + '0';
                LCD_UartSend(CMD_DC24(22 + i * 35, 57, str, 5));
                break;

            case KEY_DOWN_VALUE:
                if (PasswordNum[i] > 0)
                {
                    PasswordNum[i]--;
                }
                str[0] = PasswordNum[i] + '0';
                LCD_UartSend(CMD_DC24(22 + i * 35, 57, str, 5));
                break;
            }
            if (EnterKeyFlag)
            {
                EnterKeyFlag = 0;
                break;
            }
            if (CancelKeyFlag)
            {
                return KeyExit;
            }
        }
        if (i == 3)
        {
            //to be done 判断密码是否正确
            if (PasswordNum[0] == RightPassword[0] && PasswordNum[1] == RightPassword[1] && \
                PasswordNum[2] == RightPassword[2] && PasswordNum[3] == RightPassword[3])
            {
                return PasswordRight;
            }
            else
            {
                return PasswordError;
            }
        }
    }
    return KeyExit;
}