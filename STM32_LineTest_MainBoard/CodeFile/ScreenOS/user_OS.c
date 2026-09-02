#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "usart_lcd.h"
#include "user_Peripheral.h"
#include "Timer0.h"
#include "menu.h"
#include "sys.h"
#include "flash.h"
#include "commandlist_NPG83_10P.h"
#include "user_OS.h"
#include "w25qxx.h"
#include "rtc.h"
#include "math.h"
#include "user_system_init.h"
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
u8 MaxItems;         //同级最大菜单数
u8 ShowCount = 6;    //同屏显示菜单数
u8 Select_Point;

volatile u8 LayerSwitchFlag;  //界面层切换标志位
volatile u8 UserLastChoose;

volatile u8 LastMenuRecall = 0;
volatile u8 LastDispalyStartRecall = 0;
volatile u8 LastDispalyEndRecall = 0;

volatile u8 SecMenuRecall = 0;
volatile u8 SecDispalyStartRecall = 0;
volatile u8 SecDispalyEndRecall = 0;
//目前仅支持四层选择  超出会跑飞  需添加

volatile u8 ScreenRefresh;
u32 MainCount_Hex;
//=======================//
extern const u8 Main_Board_Version[18];//主板版本
u8 SlaveVersion_backdoor[18];

void ReadMainCount(void)             //读出Eflash中记录的MainCount
{
    u32 testmaincout[1] = {100}; //首次上电写入的烧录计数

    if ((*(u32 *)MAIN_COUNT_BASEADDR) == 0xFFFFFFFF)
    {
        Write_Flash_u32(MAIN_COUNT_BASEADDR, testmaincout, 1);
    }
    MainCount_Hex = (*(u32 *)MAIN_COUNT_BASEADDR);
}

void SystemParaInit(void)            //初始化开始界面
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

void OperatingSystem(void)           //操作系统函数
{
    u32 PressKeyData;

    PressKeyData =  0x0000;
    PressKeyData =  PressKeyScan();     /* 等待有key按下,并获得是哪个key */
    switch (PressKeyData)
    {
    case Key_Enter:
    {
        LayerSwitchFlag = 1 ;      //界面切换层数

        if (MenuPoint[UserChoose].ChildrenMenus == &NullMenuItem) /* 无子menu，已到底层，进入基础界面 */
        {
            UserFunctionInterface();
        }
        else if (MenuPoint[UserChoose].ChildrenMenus != &NullMenuItem) /* 还有子menu，则进入子menu */
        {
            SecMenuRecall = LastMenuRecall;
            SecDispalyStartRecall = LastDispalyStartRecall;
            SecDispalyEndRecall = LastDispalyEndRecall;

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
            else //if (MenuPoint[0].ParentMenus == Program_list || MenuPoint[0].ParentMenus == Test_list)NonHP110ProMenu    //判断是否为3级目录，
            {
                MenuPoint    = MenuPoint[0].ParentMenus;         //从当前目录结构体切换至父节点
                UserChoose   = LastMenuRecall;                  //返回之前父目录的菜单起始项以及位置
                DisplayStart = LastDispalyStartRecall;
                DisplayEnd   = LastDispalyEndRecall;

                LastMenuRecall = SecMenuRecall;
                LastDispalyStartRecall  = SecDispalyStartRecall;
                LastDispalyEndRecall = SecDispalyEndRecall;

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
}


void ShowVersionInterface(void)      //版本显示函数
{
//     u8 SecurityChipVersion[16];
//     u8 SecurityChipVersionStatus[10];//10片
    u8 SlaveVersion[16];
    u8 SlaveVersionStatus[40];//40片

//     ResetSlaveChip();            //显示安全芯片版本
//     GetSecurityChipVersion(SecurityChipVersion, SecurityChipVersionStatus);
		
    ResetSlaveChip();
    GetSlaveVersion(SlaveVersion, SlaveVersionStatus);

    UartSend("CLR(0);\r\n");
    CheckBusy();

    UartSend(T_DC32(100, 10, "各模块软件版本信息", 15));
    CheckBusy();

//     UartSend(T_DC32(22, 50, "Flash :", 15));
//     CheckBusy();
//     GetFlashVersion(180, 90);

    UartSend(T_DC32(22, 70, "主板  :", 15));
    CheckBusy();

    UartSend(T_DC32(170, 70, (u8 *)Main_Board_Version, 4));
    CheckBusy();

//    UartSend(T_DC32(25, 165, "安全芯片:", 15));
//    CheckBusy();
//      ShowVersionFunction(180, 165, SecurityChipVersion, SecurityChipVersionStatus);

    UartSend(T_DC32(22, 110, "功能板:", 15));
    CheckBusy();
    ShowVersionFunction(170, 110, SlaveVersion, SlaveVersionStatus);

}

void ShowSystemStartupInformation(void) //开机界面函数
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
    RTC_NOW_time();
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


void SystemInformationInit(void)     //系统启动数据准备函数
{
    ReadMainCount();         //获取片内Flash中储存的烧录计数

//     Get_TableHead();         //获得Flash中的数据表头

    RTC_Init();              //RTC初始化时间

    StartupScreen(3);        //显示开机界面
}

void SystemDrvInit(void)             //系统模块初始化函数
{
    SystemClkInit();         //配置系统时钟
    InitDelayTimer();        //初始化定时器时钟
    NVIC_Configuration();    //中断初始化
    PressKeyInit();          //硬件初始化
    MainSlaveGpioInit();     //初始化大小板通信接口
//     W25QXX_Init();           //初始化Flash通信引脚
    LcdDrvInit();            //初始化Uart,用于驱动LCD3.5寸
//     Speak_init();            //初始化蜂鸣器
}

void StartupScreen(u8 cnt)           //显示开机界面 cnt：进度条加载次数
{
    u16 i;
    u8 time;
    u8 color = 1;          //红色

    UartSend("CLR(0);\r\n");
    CheckBusy();
    UartSend(T_DC32(112, 130, "系统初始化中...", 15));
    CheckBusy();
    UartSend("BOX(112,180,355,200,15);\r\n");
    CheckBusy();

    for (time = 0; time < cnt; time++)
    {
        for (i = 113; i < 230; i += 4)
        {
            UartSend(T_BOXF(i, 181, i + 4, 199, color));
            CheckBusy();
            UartSend(T_BOXF(i + 121, 181, i + 125, 199, 0));
            CheckBusy();
        }

        for (; i < 350; i += 4)
        {
            UartSend(T_BOXF(i, 181, i + 4, 199, color));
            CheckBusy();
            UartSend(T_BOXF(i - 120, 181, i - 116, 199, 0));
            CheckBusy();
        }
        color++;
    }

    for (i = 113; i < 230; i += 4)
    {
        UartSend(T_BOXF(i, 181, i + 4, 199, color));
        CheckBusy();
        UartSend(T_BOXF(i + 121, 181, i + 125, 199, 0));
        CheckBusy();
    }

    for (; i < 350; i += 4)
    {
        UartSend(T_BOXF(i, 181, i + 4, 199, color));
        CheckBusy();
    }
}

void GetVersionScreen(void)          //获取版本信息
{
    UartSend("CLR(0);\r\n");
    CheckBusy();
    UartSend(T_DC32(96, 140, "正在获取版本信息...", 15));
    CheckBusy();
}


void WaitingCheckInterface(void)     //检测等待界面
{
    UartSend("CLR(0);\r\n");
    CheckBusy();
    UartSend(T_DC32(112, 120, "正在执行检测命令", 15));
    CheckBusy();
    UartSend(T_DC32(130, 250, "剩余时间：", 1));
    CheckBusy();
}

void WaitingSurfaceCheck(u8 StartTime, u8 EndTime) //检测等待
{
    int i;
    u8 LCD_Buf[2];
		u8 time;
	
	time = StartTime - EndTime;

    for (i = 0; i <= time; i++)
    {
        LCD_Buf[0] = StartTime / 10 + 0x30;
        LCD_Buf[1] = StartTime % 10 + 0x30;
        UartSend(T_DC32(280, 250, LCD_Buf, 1));
        CheckBusy();
        TimerDelayMS(950);
        UartSend(T_DC32(280, 250, LCD_Buf, 0));
        CheckBusy();
        StartTime--;
    }
    if (EndTime == 0)
    {
        UartSend(T_DC32(130, 250, "剩余时间：", 0));
        CheckBusy();
        UartSend(T_DC32(280, 250, LCD_Buf, 0));
        CheckBusy();
        UartSend(T_DC32(200, 250, "完成！", 2));
        CheckBusy();
    }
}

void GetSecurityChipVersion(u8* SecurityChipVersionTmp, u8* ScVersionStatus)   //获取安全芯片版本
{
    int i;
    u8 slavenum;
    u8 version_buf[18] = {"0000000000000000"};
    u8 version_status[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};
    u8 SecurityChip_Version[10][18]; //20171229 HP 252 Beta 0.00a
//     u8 char_buf[3] = {0, 0, 0};
//     u8 color;
    u16 SendDataLen;
    u8 SendDataBuf[256];

    SendDataBuf[0] = 0;
    for (slavenum = 0; slavenum < 10; slavenum++)
    {
        SendDataLen = 0;
        SendCommandData(slavenum, SECURITYCHIP_VERSION, SendDataBuf, SendDataLen);
        memset_data(SecurityChip_Version[slavenum], 0, 0x10);
    }
    TimerDelayMS(250);
    for (slavenum = 0; slavenum < 10; slavenum++)
    {
        SendCommandData(slavenum, SLVAE_RESPOND_SCPVERSION, SendDataBuf, SendDataLen);
        TimerDelayMS(10);
        MainReceiveData(slavenum, SecurityChip_Version[slavenum]);
    }

    for (slavenum = 0; slavenum < 10; slavenum++)
    {
        for (i = 0; i < 8; i++)
        {
            if ((SecurityChip_Version[slavenum][1 + i] >= 0x30) && (SecurityChip_Version[slavenum][1 + i] <= 0x39))
            {

                if (version_buf[i] > SecurityChip_Version[slavenum][1 + i])
                {
                    break;
                }
                else if (version_buf[i] < SecurityChip_Version[slavenum][1 + i])
                {
                    version_buf[i] = SecurityChip_Version[slavenum][1 + i];
                    for (i = 0; i < 18; i++)
                    {
                        version_buf[i] = SecurityChip_Version[slavenum][1 + i];
                    }
                    break;
                }
            }
        }
    }

    for (slavenum = 0; slavenum < 10; slavenum++)
    {
        version_status[slavenum] = 0;
        for (i = 0; i < 16; i++)
        {
            if (version_buf[i] != SecurityChip_Version[slavenum][i + 1])
            {
                version_status[slavenum] = 1;
            }
        }
    }

    memcpy_data(SecurityChipVersionTmp, version_buf, 16);
    memcpy_data(ScVersionStatus, version_status, 10);
}

void TranVersionToArray(u8* VersionArr)
{
    u8 i;

    for (i = 0; i < 16; i++)
    {
        VersionArr[i] = Main_Board_Version[i];
    }

}

void TranSlaveVersionToArray(u8* SlaveVersionArr)
{
    u8 i;

    for (i = 0; i < 16; i++)
    {
        SlaveVersionArr[i] = SlaveVersion_backdoor[i];
    }
}

void GetSlaveVersion(u8* SlaveVersionTmp, u8* SlaveVersionStatus)             //获取从机版本
{
    int i;
    u8 slavenum;
    u8 version_buf[18] = {"000000000000000000"};
    u8 version_status[40] = {0};
    u8 version_slavenum = 0xFF;

    u16 SendDataLen;
    u8 SendDataBuf[256];
    u8 Slave_Board_Version[40][17] = {0x32, 0x30, 0x31, 0x37, 0x31, 0x32, 0x32, 0x39, 0x48, 0x50, 0x32, 0x35, 0x32, 0x42, 0x30, 0x30,}; //20171229 HP 252 Beta 0.00a

    SendDataBuf[0] = 0;
    for (slavenum = 0; slavenum < 40; slavenum++)
    {
        SendDataLen = 0;

        SendCommandData(slavenum, SLAVE_VERSION_REPLY, SendDataBuf, SendDataLen);

        TimerDelayMS(10);

        memset_data(Slave_Board_Version[slavenum], 0, 0x10);
        MainReceiveData(slavenum, Slave_Board_Version[slavenum]);
    }

    for (slavenum = 0; slavenum < 40; slavenum++)
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
        for (i = 0; i < 17; i++)
        {
            version_buf[i] = Slave_Board_Version[version_slavenum][i + 1];
        }
    }

    for (slavenum = 0; slavenum < 40; slavenum++)
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

    memcpy_data(SlaveVersionTmp, version_buf, 17);
    memcpy_data(SlaveVersion_backdoor, version_buf, 18);
    memcpy_data(SlaveVersionStatus, version_status, 40);
}

void ShowVersionFunction(u8 X_base, u8 Y_base, u8* version_buf, u8* version_status)  //x =180, y =70			Y_Line_max为1时，循环10次
{
    int i = 0;
    int Y_START = 250;
    int Y_STEP = 30;
    int Y_line = 0;    //Y轴起始列
    int Y_Line_max = 4;  //Y轴最大列数
    int X_axis = 0;   //X轴位置
    int X_START = 40;
    int X_STEP = 40;

    u8 color;   //字体的颜色
    u8 COLOR_RED = 1; //字体显示红色
    u8 COLOR_GREEN = 2;  //字体显示绿色
    u8 line[1] = {0};        //显示屏显示的文字   A-E
    u8 char_buf[3] = {0};

    UartSend(T_DC32(X_base, Y_base, version_buf, 4));  //功能板
    CheckBusy();

    for (; Y_line < Y_Line_max ; Y_line++)   //1列 Y_Line_max * 10
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

void GetFlashVersion(u8 X_base, u8 Y_base)  //获取外部Flash版本
{
    u8 flash_version_buf[18] = {0};

		memset_data(flash_version_buf, 0, 18);
    W25QXX_Read(flash_version_buf, 0x700, 0x10);

    UartSend(T_DC32(X_base, Y_base, (u8 *)flash_version_buf, 4));


    CheckBusy();
}

void ShowMenu(void)         //显示菜单函数
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

        UartSend("DC32(35,40,'请选择需要执行的命令',15);\r\n");
        CheckBusy();

        n = 0;
        for (Display_count = DisplayStart; Display_count < DisplayEnd; Display_count++)
        {
            if ((n + 1) == Select_Point) //如果是选中行，则显示->
            {
                color = 6;
                UartSend(D_CIRF(45, 101 + (n * 36), 6, color));
                CheckBusy();
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
        UartSend(D_CIRF(45, 101 + (last_cow * 36), 6, 0));
        CheckBusy();

        now_cow = Select_Point - 1;
        UartSend(T_DC32(65, 85 + (now_cow * 36), MenuPoint[DisplayStart + now_cow].DisplayString, 0));
        CheckBusy();
        UartSend(T_DC32(65, 85 + (now_cow * 36), MenuPoint[DisplayStart + now_cow].DisplayString, 6));
        CheckBusy();
        UartSend(D_CIRF(45, 101 + (now_cow * 36), 6, 6));
        CheckBusy();
    }
    LayerSwitchFlag = 0;
    UserLastChoose = Select_Point;
}

void Confirm_Selection(u8 *Title_Name) //确认信息界面
{
    UartSend("CLR(0);\r\n");
    CheckBusy();

    UartSend(T_DC32(35, 40, "请确认你选择的操作指令", 15));
    CheckBusy();

    UartSend(T_DC32(20, 100, Title_Name, 5));
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



