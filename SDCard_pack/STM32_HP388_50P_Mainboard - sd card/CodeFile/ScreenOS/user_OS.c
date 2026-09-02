#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "usart_lcd.h"
#include "user_Peripheral.h"
#include "user_system_init.h"
#include "Timer0.h"
#include "menu.h"
#include "flash.h"
#include "commandlist_HP388.h"
#include "SD.h"
#include "user_OS.h"
#include "rtc.h"
#include "math.h"
#include "CommonFunction.h"
#include "MainSlaveInterface.h"
#include "UserFunctionInterface.h"

#define  MAIN_COUNT_BASEADDR  0x20000

//=======================//
u32 Index_Select = 0xFFFF;  /*箭头选中项*/
u32 Index_Select_back = 0xFFFF;  /*箭头选中项*/
struct MenuItem(*MenuPoint) = MainMenu;  //结构体指针,指向结构体后由内部函数指针指向功能函数
u8 DisplayStart = 0; //显示时的第一个菜单项
u8 DisplayEnd = 0;   //显示时的最后一个菜单项
u8 UserChoose = 0;   //用户所选菜单项
u8 DisplayPoint = 0; //显示指针
u8 MaxItems;       //同级最大菜单数
const u8 ShowCount = 6;        //同屏显示菜单数
u8 Select_Point;
volatile u8 LayerSwitchFlag;  //界面层切换标志位
volatile u8 UserLastChoose;
volatile u8 LastMenuRecall;
volatile u8 LastDispalyStartRecall;
volatile u8 LastDispalyEndRecall;
volatile u8 ScreenRefresh;
u32 MainCount_Hex;
//=======================//
extern const u8 Main_Board_Version[18];//主板版本
u8 SlaVersionBuf[16] = {"0000000000000000"};//从机版本
u8 SlaveVersion_Backdoor[16];
//读出Eflash中记录的MainCount
void ReadMainCount(void)
{
    u32 testmaincout[1] = {100}; //首次上电写入的烧录计数

    if ((*(u32 *)MAIN_COUNT_BASEADDR) == 0xFFFFFFFF)
    {
        Write_Flash_u32(MAIN_COUNT_BASEADDR, testmaincout, 1);
    }
    MainCount_Hex = (*(u32 *)MAIN_COUNT_BASEADDR);
}

//初始化开始界面
void SystemParaInit(void)
{
    MenuPoint = MainMenu;
    UserChoose = 0;
    DisplayStart = 0;
    MaxItems = MenuPoint[0].MenuCount;
    if ((MaxItems / ShowCount) != 0)
    {
        DisplayEnd = ShowCount;
    }
    else
    {
        DisplayEnd = MaxItems;
    }
    ScreenRefresh = 1;
    Select_Point = 1;
    ShowMenu();  /* 开始界面 */
}


//操作系统函数
void OperatingSystem(void)
{
    u32 PressKeyData;

    PressKeyData =  0x0000;
    PressKeyData =  PressKeyScan();     /* 等待有key按下,并获得是哪个key */
    switch (PressKeyData)
    {
    case Key_Enter:
    {
				
        //BackChoose
        LayerSwitchFlag = 1 ;      //界面切换层数

        if (MenuPoint[UserChoose].ChildrenMenus == &NullMenuItem) /* 无子menu，已到底层，进入基础界面 */
        {
            UserFunctionInterface();
        }
        else if (MenuPoint[UserChoose].ChildrenMenus != &NullMenuItem) /* 还有子menu，则进入子menu */
        {
            LastMenuRecall = UserChoose;      //记录选择值
            LastDispalyStartRecall = DisplayStart;      //菜单起始项
            LastDispalyEndRecall = DisplayEnd;    //菜单终止项

            MenuPoint = MenuPoint[UserChoose].ChildrenMenus;  //进入子菜单
            UserChoose = 0;    //重新开始选择
            DisplayStart = 0;   //起始项从零开始
        }
        break;
    }
    case Key_Esc:
    {
        LayerSwitchFlag = +1;
        if (MenuPoint[0].ParentMenus != &NullMenuItem)     /* 如果不是主menu，则进入其父menu */
        {
            if (MenuPoint[0].ParentMenus == MainMenu)          //主menu+二级MENU
            {
                MenuPoint    = MenuPoint[0].ParentMenus;  /* 3个参数决定显示 */
                UserChoose   = 0;
                DisplayStart = 0;
            }
            else if (MenuPoint[0].ParentMenus == HP388ProMenu || MenuPoint[0].ParentMenus == HP388TestMenu)
            {
                MenuPoint    = MenuPoint[0].ParentMenus;         //判断是否为3级目录，
                UserChoose   = LastMenuRecall;                  //返回之前的3级目录的菜单起始项以及位置
                DisplayStart = LastDispalyStartRecall;
                DisplayEnd   = LastDispalyEndRecall;
            }
        }
        break;
    }
    case Key_Up:                /* 光标向上移动，这里不判断显示界面的变化 */
    {
        UserChoose --;

        if (UserChoose == 0xff) /* 这个地方如果用 <0 UserChoose要为有符号数 */
        {
            UserChoose = MaxItems - 1;
        }

        break;
    }
    case Key_Down:              /* 光标向下移动，这里不判断显示界面的变化 */
    {
        UserChoose ++;
        if (UserChoose == MaxItems)
        {
            UserChoose = 0;
        }
        break;
    }
    default:
    {
        break;
    }
    }

    if ((UserChoose == 0) && (DisplayStart == 0))
    {
        if ((UserLastChoose == 2) && (LayerSwitchFlag == 0))
        {
            ScreenRefresh = 0;
        }
        else
        {
            ScreenRefresh = 1;
        }

        DisplayStart = 0;
        MaxItems = MenuPoint[0].MenuCount;
        if ((MaxItems / ShowCount) != 0)
        {
            DisplayEnd = ShowCount;
        }
        else
        {
            DisplayEnd = MaxItems;
        }
        Select_Point = 1;
    }
    else if ((UserChoose >= DisplayStart) && (UserChoose < DisplayEnd))
    {
        ScreenRefresh = 0;
        Select_Point =  UserChoose - DisplayStart + 1;
    }
    else if (UserChoose < DisplayStart)
    {
        ScreenRefresh = 1;
        if (UserChoose <= 6)
        {
            if (UserChoose == 0)
            {
                Select_Point = 1;
            }
            else
            {
                Select_Point = DisplayStart;
            }
            DisplayStart = 0;
            if ((MaxItems / ShowCount) != 0)
            {
                DisplayEnd = ShowCount;
            }
            else
            {
                DisplayEnd = MaxItems;
            }
        }
        else
        {
            DisplayStart -= 6;
            DisplayEnd = DisplayStart + 6;
            Select_Point = 6;
        }
    }
    else if (UserChoose >= DisplayEnd)
    {
        ScreenRefresh = 1;
        if ((MaxItems - UserChoose) < ShowCount)
        {
            //Select_Point = 6;
            Select_Point = 7 + UserChoose - MaxItems;
            DisplayStart = MaxItems - 6;
            DisplayEnd = MaxItems;
        }
        else
        {
            DisplayStart += 6;
            DisplayEnd = DisplayStart + 6;
            Select_Point = 1;
        }
    }
    if (LayerSwitchFlag == 1)
    {
        ScreenRefresh = 1;
    }
    ShowMenu();
    delay_ms(100);      //延时按键
}

//版本显示函数
void ShowVersionInterface(void)
{
    u8 SlaveVersion[16];
    u8 SlaveVersionStatus[50];

    ResetSlaveChip();
    GetSlaveVersion(SlaveVersion, SlaveVersionStatus);

    UartSend("CLR(0);\r\n");
    CheckBusy();

    UartSend(T_DC32(100, 10, "各模块软件版本信息", 15));
    CheckBusy();

//     UartSend(T_DC32(25, 50, "Flash :", 15));
//     CheckBusy();
//     GetFlashVersion(170, 50);

    UartSend(T_DC32(22, 60, "主板  :", 15));
    CheckBusy();

    UartSend(T_DC32(170, 60, (u8 *)Main_Board_Version, 4));
    CheckBusy();

    UartSend(T_DC32(22, 110, "功能板:", 15));
    CheckBusy();
    ShowVersionFunction(170, 110, SlaveVersion, SlaveVersionStatus);

}

//开机界面函数
void ShowSystemStartupInformation(void)
{
    u32 key_data;

    ShowVersionInterface();
    while (1)
    {
        key_data =  0x0000;
        key_data =  PressKeyScan();     /* 等待有key按下,并获得是哪个key */
        if (key_data != 0)
        {
            break;
        }
    }
    //RTC_NOW_time();
    while (1)
    {
        key_data =  0x0000;
        key_data =  PressKeyScan();     /* 等待有key按下,并获得是哪个key */
        if (key_data != 0)
        {
            break;
        }
    }

    SystemParaInit();               //LCD操作系统OS初始化
}

//系统启动数据准备函数
void SystemInformationInit(void)
{
    ReadMainCount();         //获取片内Flash中储存的烧录计数

    Get_TableHead();         //获得Flash中的数据表头

    //RTC_Init();              //RTC初始化时间

    StartupScreen();
}

//系统模块初始化函数
void SystemDrvInit(void)
{
    SystemClkInit();         //配置系统时钟
    InitDelayTimer();        //初始化定时器时钟
    NVIC_Configuration();    //中断初始化
    PressKeyInit();          //硬件初始化
    MainSlaveGpioInit();     //初始化大小板通信接口
    SD_Initialize();           //初始化Flash通信引脚
    LcdDrvInit();            //初始化Uart,用于驱动LCD3.5寸
    Speak_init();                        //初始化蜂鸣器
    ChooseVoltageInit();            //参考电压选择初始化
}

void StartupScreen(void)
{
    UartSend("CLR(0);\r\n");
    CheckBusy();
    UartSend(T_DC32(112, 140, "系统正在启动...", 15));
    CheckBusy();
}

void GetVersionScreen(void)
{
    UartSend("CLR(0);\r\n");
    CheckBusy();
    UartSend(T_DC32(96, 140, "正在获取版本信息...", 15));
    CheckBusy();
}

//烧录等待界面
void WaitingSurfaceBurn(u8 StartTime, u8 EndTime)
{
    int i;
    u8 LCD_Buf[2];
    u8 MainCount[7];

    UartSend("CLR(0);\r\n");
    CheckBusy();
    UartSend(T_DC32(112, 40, "正在执行烧录命令", 15));
    CheckBusy();

    Train_MainCount_to_Array(MainCount);

    UartSend(T_DC32(90, 100, "烧录型号: ", 4));
    CheckBusy();
    UartSend(T_DC32(90, 140, MenuPoint[UserChoose].DisplayString, 4));
    CheckBusy();

    UartSend(T_DC32(90, 240, "剩余时间: ", 4));
    CheckBusy();
    //烧录等待时间的调节
    for (i = StartTime; i > EndTime; i--)
    {
        LCD_Buf[0] = i / 10 + 0x30;
        LCD_Buf[1] = i % 10 + 0x30;

        UartSend(T_DC32(280, 270, LCD_Buf, 4));
        CheckBusy();
        delay_ms(1050);
        UartSend(T_DC32(280, 270, LCD_Buf, 0));
        CheckBusy();
    }

}

// //检测等待函数
void WaitingSurfaceCheck(u8 StartTime, u8 EndTime)
{
    int i;
    u8 LCD_Buf[2];

    UartSend("CLR(0);\r\n");
    CheckBusy();
    UartSend(T_DC32(112, 40, "正在执行检测命令", 15));
    CheckBusy();
    UartSend(T_DC32(90, 100, "检测型号: ", 4));
    CheckBusy();
    UartSend(T_DC32(90, 140, MenuPoint[UserChoose].DisplayString, 4));
    CheckBusy();
    //烧录等待时间的调节
    for (i = StartTime; i > EndTime; i--)
    {
        LCD_Buf[0] = i / 10 + 0x30;
        LCD_Buf[1] = i % 10 + 0x30;

        UartSend(T_DC32(90, 240, "剩余时间: ", 4));
        CheckBusy();
        UartSend(T_DC32(280, 270, LCD_Buf, 4));
        CheckBusy();
        delay_ms(1050);
        UartSend(T_DC32(280, 270, LCD_Buf, 0));
        CheckBusy();
    }
}

void WaittingBurnTime(u8 BurnEECnt)
{
    switch (BurnEECnt)  //根据EE的数据长度作出等待时间的修改
    {
    case 1:
        WaitingSurfaceBurn(3, 0);//1
        break;
    case 2:
        WaitingSurfaceBurn(5, 0);//2
        break;
    case 3:
        WaitingSurfaceBurn(9, 0);//3
        break;
    case 4:
        WaitingSurfaceBurn(10, 0);//4
        break;
    case 5:
        WaitingSurfaceBurn(12, 0);//5
        break;
    case 6:
        WaitingSurfaceBurn(13, 0);//6
        break;
    case 7:
        WaitingSurfaceBurn(16, 0);//7
        break;
    default:
        UartSend("CLR(0);\r\n");
        CheckBusy();
        UartSend(T_DC32(176, 280, "烧录出现通信异常", 23));
        CheckBusy();
        while (1);
    }
}

void WaittingCheckTime(u8 CheckEECnt)
{
    switch (CheckEECnt)  //根据EE的数据长度作出等待时间的修改
    {
    case 1:
    case 2:
        WaitingSurfaceCheck(4, 0);//1
        break;
    case 3:
        WaitingSurfaceCheck(7, 0);//2
        break;
    case 4:
        WaitingSurfaceCheck(11, 0);//3
        break;
    case 5:
    case 6:
        WaitingSurfaceCheck(12, 0);//4
        break;
    case 7:
        WaitingSurfaceCheck(16, 0);//5
        break;
    default:
        UartSend("CLR(0);\r\n");
        CheckBusy();
        UartSend(T_DC32(176, 280, "检测出现通信异常", 23));
        CheckBusy();
        while (1);
    }
}

//获取从机版本
void GetSlaveVersion(u8* SlaveVersionTmp, u8* SlaveVersionStatus)
{
    int i;
    u8 slavenum;
    u8 version_buf[18] = {"0000000000000000"};
    u8 version_status[50] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};
    u8 version_slavenum = 0xFF;

    u16 SendDataLen;
    u8 SendDataBuf[256];
    u8 Slave_Board_Version[50][17] = {0x32, 0x30, 0x31, 0x37, 0x31, 0x32, 0x32, 0x39, 0x48, 0x50, 0x32, 0x35, 0x32, 0x42, 0x30, 0x30,}; //20171229 HP 252 Beta 0.00a


    SendDataBuf[0] = 0;
    for (slavenum = 0; slavenum < 50; slavenum++)
    {
        SendDataLen = 0;

        SendCommandData(slavenum, SLAVE_VERSION_REPLY, SendDataBuf, SendDataLen);

        TimerDelayMS(5);

        memset_data(Slave_Board_Version[slavenum], 0, 0x10);
        MainReceiveData(slavenum, Slave_Board_Version[slavenum]);
    }

    for (slavenum = 0; slavenum < 50; slavenum++)
    {
        for (i = 0; i < 8; i++)
        {
            if ((Slave_Board_Version[slavenum][i + 1] >= 0x30) && (Slave_Board_Version[slavenum][i + 1] <= 0x39))
            {
                if (version_buf[i] > Slave_Board_Version[slavenum][i + 1])
                {
                    break;
                }
                else if (version_buf[i] < Slave_Board_Version[slavenum][i + 1])
                {
                    version_buf[i] = Slave_Board_Version[slavenum][i + 1];
                    version_slavenum = slavenum;
                    for (i = 0; i < 16; i++)
                    {
                        version_buf[i] = Slave_Board_Version[version_slavenum][i + 1];
                    }
                    break;
                }
            }
        }
    }

    if (version_slavenum != 0xFF)
    {
        for (i = 0; i < 16; i++)
        {
            version_buf[i] = Slave_Board_Version[version_slavenum][i + 1];
        }
    }

    for (slavenum = 0; slavenum < 50; slavenum++)
    {
        version_status[slavenum] = 0;
        for (i = 0; i < 16; i++)
        {
            if (version_buf[i] != Slave_Board_Version[slavenum][i + 1])
            {
                version_status[slavenum] = 1;
            }
        }
    }

    memcpy_data(SlaveVersionTmp, version_buf, 16);
    memcpy_data(SlaveVersion_Backdoor, version_buf, 16);
    memcpy_data(SlaveVersionStatus, version_status, 50);
}

void ShowVersionFunction(u8 X_base, u8 Y_base, u8* version_buf, u8* version_status)  //x =180, y =70
{
    int i = 0;
    int Y_START = 270;
    int Y_STEP = 30;
    int Y_line = 0;    //Y轴起始列
    int Y_Line_max = 5;  //Y轴最大列数
    int X_axis = 0;   //X轴位置
    int X_START = 60;
    int X_STEP = 40;

    u8 color;   //字体的颜色
    u8 COLOR_RED = 1; //字体显示红色
    u8 COLOR_GREEN = 2;  //字体显示绿色
    u8 line[1] = {0};        //显示屏显示的文字   A-E

    u8 char_buf[3] = {0};

    UartSend(T_DC32(X_base, Y_base, version_buf, 4));  //功能板
    CheckBusy();

    for (; Y_line < Y_Line_max ; Y_line++)   //5列 5*10
    {
        line[0] = 0x41 + Y_line ;
        UartSend(T_DC32(20, Y_START - Y_STEP * Y_line, line, 15));
        CheckBusy();

        for (; X_axis < 10; X_axis++)
        {
            char_buf[1] = 0;
            char_buf[0] = X_axis + 0x31;
            if (X_axis == 9)
            {
                char_buf[0] = 0x31;
                char_buf[1] = 0x30;
            }
            if (version_status[i] != 0)
            {
                color = COLOR_RED;
            }
            else
            {
                color = COLOR_GREEN;
            }
            i++;
            UartSend(T_DC32(X_START + X_axis * X_STEP, Y_START - Y_STEP * Y_line, char_buf, color));
            CheckBusy();

            if (X_axis == 9)
            {
                X_axis = 0;
                break;

            }
        }
    }
}

//获取外部Flash版本
void GetFlashVersion(u8 X_base, u8 Y_base)
{
    u8 flash_version_buf[18] = {0};

    SD_ReadDisk(flash_version_buf, 200, 1);

    UartSend(T_DC32(X_base, Y_base, (u8 *)flash_version_buf, 4));

    CheckBusy();
}

//显示菜单函数
void ShowMenu(void)
{
    u8 color;
    unsigned char n;
    //u8 show_loop;
    u8 last_cow;
    u8 now_cow;
    u8 Display_count;

    n = 0;
    MaxItems = MenuPoint[0].MenuCount; //第一个目录指向了MainMenu
    DisplayPoint = DisplayStart;

    if (ScreenRefresh != 0)         //判断进行菜单显示还是上下显示
    {
        UartSend("CLR(0);\r\n");
        CheckBusy();
        if (MenuPoint->ParentMenus == HP388ProMenu)
        {
            UartSend("DC32(35,40,'请选择需要烧录的型号',15);\r\n");
            CheckBusy();

        }
        else if (MenuPoint->ParentMenus == HP388TestMenu)
        {
            UartSend("DC32(35,40,'请选择需要检测的型号',15);\r\n");
            CheckBusy();
        }
        else
        {
            UartSend("DC32(35,40,'请选择需要执行的命令',15);\r\n");
            CheckBusy();
        }
        n = 0;
        for (Display_count = DisplayStart; Display_count < DisplayEnd; Display_count++)
        {
            if ((n + 1) == Select_Point) //如果是选中行，则显示->
            {
                color = 6;
            }
            else
            {
                color = 4;
            }
            UartSend(T_DC32(65, 85 + (n * 36), MenuPoint[Display_count].DisplayString, color));
            CheckBusy();
            n++;
        }
    }
    else   //上下显示
    {
        last_cow = UserLastChoose - 1;
        UartSend(T_DC32(65, 85 + (last_cow * 36), MenuPoint[DisplayStart + last_cow].DisplayString, 0));
        CheckBusy();
        UartSend(T_DC32(65, 85 + (last_cow * 36), MenuPoint[DisplayStart + last_cow].DisplayString, 4));
        CheckBusy();

        now_cow = Select_Point - 1;
        UartSend(T_DC32(65, 85 + (now_cow * 36), MenuPoint[DisplayStart + now_cow].DisplayString, 0));
        CheckBusy();
        UartSend(T_DC32(65, 85 + (now_cow * 36), MenuPoint[DisplayStart + now_cow].DisplayString, 6));
        CheckBusy();
    }
    LayerSwitchFlag = 0;
    UserLastChoose = Select_Point;
}



//确认信息界面
void Confirm_Selection(u8 *Title_Name)
{
    UartSend("CLR(0);\r\n");
    CheckBusy();

    UartSend(T_DC32(35, 40, "请确认你选择的操作指令", 15));
    CheckBusy();

    UartSend(T_DC32(100, 100, Title_Name, 11));
    CheckBusy();

    UartSend("BOXF(220,215,260,255,4);\r\n");
    CheckBusy();

    UartSend("BOXF(170,170,210,210,4);\r\n");
    CheckBusy();
    UartSend(T_DC32(100, 174, "返回", 4));
    CheckBusy();

    UartSend("BOXF(270,170,310,210,4);\r\n");
    CheckBusy();
    UartSend(T_DC32(100, 264, "确认", 4));
    CheckBusy();

    UartSend("BOXF(170,260,210,300,4);\r\n");
    CheckBusy();

    UartSend("BOXF(270,260,310,300,4);\r\n");
    CheckBusy();
}

//显示RTC时间函数
void ShowRtcTime(void)
{
    RTC_Get();//更新时间
    RTC_NOW_time();
}

//设置RTC时间函数
void UserSetRtcTime(void)
{
    int cnt;   /*cnt(0-3 year/ 5-6 month/ 8-9 day/ 11-12 hour/ 14-15 minue/ 17-18 second)*/
    u32 year;
    u32 month;
    u32 day;
    u32 hour;
    u32 minue;
    u32 second;
    u32 readkey = 0;
    u16 ChangeBit;

    RTC_Get();//更新时间
    year = calendar.w_year;
    month = calendar.w_month;
    day = calendar.w_date;
    hour = calendar.hour;
    minue = calendar.min;
    second = calendar.sec;

    cnt = 0;
    while (1)
    {
        if ((cnt == 4) || (cnt == 7) || (cnt == 10) || (cnt == 13) || (cnt == 16))
        {
            if (readkey == Key_Enter)
            {
                cnt++;
            }
            else if (readkey == Key_Esc)
            {
                cnt--;
            }
        }
        RTC_change_show_time(year, month, day, hour, minue, second, cnt);
        readkey =  0x0000;
        readkey =  PressKeyScan();     /* 等待有key按下,并获得是哪个key */
        switch (readkey)
        {
        case Key_Enter:
        {
            if (cnt >= 18)
            {
                if ((year < 1970) || (year > 2200))
                {
                    year = 2019;
                }
                if (month == 0)
                {
                    month = 1;
                }
                if (day == 0)
                {
                    day = 1;
                }
                if (hour >= 24)
                {
                    hour = 0;
                }
                if (minue >= 60)
                {
                    minue = 0;
                }
                if (second >= 60)
                {
                    second = 0;
                }

                if (!Is_Leap_Year(year) && (month == 2) && (day >= 29))
                {
                    day = 28;
                }
                if (((month == 4) || (month == 6) || (month == 9) || (month == 11)) && (day >= 31))
                {
                    day = 30;
                }
                if (((month == 1) || (month == 3) || (month == 5) || (month == 7) || (month == 8) || (month == 10) || (month == 12)) && (day >= 31))
                {
                    day = 31;
                }
                RTC_Set(year, month, day, hour, minue, second); //设置时间
                return;
            }
            else
            {
                cnt++;
            }
            break;
        }
        case Key_Esc:
        {
            if (cnt != 0)
            {
                cnt--;
            }
            else
            {
                return;
            }
            break;
        }
        case Key_Up:
        case Key_Down:
        {
            switch (cnt)
            {
            case 0:
            case 1:
            case 2:
            case 3:
            {
                ChangeBit = year / pow(10, 3 - cnt);
                if (readkey == Key_Down)
                {
                    year = year - ((ChangeBit % 10) * pow(10, 3 - cnt)) + (((ChangeBit + 9) % 10) * pow(10, 3 - cnt));
                }
                if (readkey == Key_Up)
                {
                    year = year - ((ChangeBit % 10) * pow(10, 3 - cnt)) + (((ChangeBit + 1) % 10) * pow(10, 3 - cnt));
                }
                break;
            }
            case 5:
            case 6:
            {
                ChangeBit = month / pow(10, 6 - cnt);
                if (readkey == Key_Down)
                {
                    month = month - ((ChangeBit % 10) * pow(10, 6 - cnt)) + (((ChangeBit + 9) % 10) * pow(10, 6 - cnt));
                }
                if (readkey == Key_Up)
                {
                    month = month - ((ChangeBit % 10) * pow(10, 6 - cnt)) + (((ChangeBit + 1) % 10) * pow(10, 6 - cnt));
                }
                if (month >= 90)
                {
                    month = month % 10 + 10;
                }
                else if (month >= 20)
                {
                    if (cnt == 5)
                    {
                        month = month % 10;
                    }
                }
                else if (month >= 13)
                {
                    if (cnt == 6)
                    {
                        month = 10;
                    }
                }

                break;
            }
            case 8:
            case 9:
            {
                ChangeBit = day / pow(10, 9 - cnt);
                if (readkey == Key_Down)
                {
                    day = day - ((ChangeBit % 10) * pow(10, 9 - cnt)) + (((ChangeBit + 9) % 10) * pow(10, 9 - cnt));
                }
                if (readkey == Key_Up)
                {
                    day = day - ((ChangeBit % 10) * pow(10, 9 - cnt)) + (((ChangeBit + 1) % 10) * pow(10, 9 - cnt));
                }

                if (day >= 90)
                {
                    if (cnt == 8)
                    {
                        day = day % 10 + 30;
                    }
                }
                else if (day >= 39)
                {
                    if (cnt == 8)
                    {
                        day = day % 10;
                    }
                }
                else if (day >= 32)
                {
                    day = 30;
                }
                break;
            }
            case 11:
            case 12:
            {
                ChangeBit = hour / pow(10, 12 - cnt);
                if (readkey == Key_Down)
                {
                    hour = hour - ((ChangeBit % 10) * pow(10, 12 - cnt)) + (((ChangeBit + 9) % 10) * pow(10, 12 - cnt));
                }
                if (readkey == Key_Up)
                {
                    hour = hour - ((ChangeBit % 10) * pow(10, 12 - cnt)) + (((ChangeBit + 1) % 10) * pow(10, 12 - cnt));
                }
                if (hour >= 90)
                {
                    if (cnt == 11)
                    {
                        hour = hour % 10 + 20;
                    }
                }
                else if (hour >= 29)
                {
                    if (cnt == 11)
                    {
                        hour = hour % 10;
                    }
                }
                else if (hour >= 24)
                {
                    hour = 20;
                }
                break;
            }
            case 14:
            case 15:
            {
                ChangeBit = minue / pow(10, 15 - cnt);
                if (readkey == Key_Down)
                {
                    minue = minue - ((ChangeBit % 10) * pow(10, 15 - cnt)) + (((ChangeBit + 9) % 10) * pow(10, 15 - cnt));
                }
                if (readkey == Key_Up)
                {
                    minue = minue - ((ChangeBit % 10) * pow(10, 15 - cnt)) + (((ChangeBit + 1) % 10) * pow(10, 15 - cnt));
                }
                if (minue >= 90)
                {
                    if (cnt == 14)
                    {
                        minue = minue % 10 + 50;
                    }
                }
                else if (minue >= 69)
                {
                    if (cnt == 14)
                    {
                        minue = minue % 10;
                    }
                }
                else if (minue >= 61)
                {
                    minue = 50;
                }
                break;
            }
            case 17:
            case 18:
            {
                ChangeBit = second / pow(10, 18 - cnt);
                if (readkey == Key_Down)
                {
                    second = second - ((ChangeBit % 10) * pow(10, 18 - cnt)) + (((ChangeBit + 9) % 10) * pow(10, 18 - cnt));
                }
                if (readkey == Key_Up)
                {
                    second = second - ((ChangeBit % 10) * pow(10, 18 - cnt)) + (((ChangeBit + 1) % 10) * pow(10, 18 - cnt));
                }
                if (second >= 90)
                {
                    if (cnt == 14)
                    {
                        second = second % 10 + 50;
                    }
                }
                else if (second >= 69)
                {
                    if (cnt == 14)
                    {
                        second = second % 10;
                    }
                }
                else if (second >= 61)
                {
                    second = 50;
                }
                break;
            }
            }
        }
        }
    }
}

