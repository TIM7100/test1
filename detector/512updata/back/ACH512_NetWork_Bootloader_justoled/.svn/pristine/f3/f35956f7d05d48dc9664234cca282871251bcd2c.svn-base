#include "user_os.h"
#include "delay.h"
#include "gpio.h"
#include "menu.h"
#include "uart.h"
#include "lcd.h"
#include "UserFunctionInterface.h"
#include "menu_show.h"

u32 Index_Select = 0xFFFF;  /*箭头选中项*/
u32 Index_Select_back = 0xFFFF;
struct MenuItem(*MenuPoint) = MainMenu;  //结构体指针,指向结构体后由内部函数指针指向功能函数
u8 DisplayStart = 0; //显示时的第一个菜单项
u8 DisplayEnd = 0; //显示时的第一个菜单项
u8 UserChoose = 0;   //用户所选菜单项
u8 DisplayPoint = 0; //显示指针
u8 MaxItems;       //同级最大菜单数
const u8 ShowCount = 5;        //同屏显示菜单数
u8 Select_Point;
volatile u8 LayerSwitchFlag;  //界面层切换标志位
volatile u8 UserLastChoose;
volatile u8 LastMenuRecall;
volatile u8 LastDispalyStartRecall;
volatile u8 LastDispalyEndRecall;
volatile u8 ScreenRefresh;


void ShowMenu(u8 Language)
{
    u8 color;
    unsigned char n;
    u8 last_cow;
    u8 now_cow;
    u8 Display_count;

    n = 0;
    MaxItems = MenuPoint[0].MenuCount; //第一个目录指向了MainMenu
    DisplayPoint = DisplayStart;

    if (ScreenRefresh != 0)
    {
        UartSend("CLR(0);\r\n");
        CheckBusy();

        UartSend("DC16(0,0,'Please select type',15);\r\n");
        CheckBusy();

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
            UartSend(CMD_DC16(10, 25 + (n * 20), MenuPoint[Display_count].DisplayString[Language], color));
            CheckBusy();
            n++;
        }
    }
    else
    {
        last_cow = UserLastChoose - 1;
        UartSend(CMD_DC16(10, 25 + (last_cow * 20), MenuPoint[DisplayStart + last_cow].DisplayString[Language], 0));
        CheckBusy();
        UartSend(CMD_DC16(10, 25 + (last_cow * 20), MenuPoint[DisplayStart + last_cow].DisplayString[Language], 4));
        CheckBusy();

        now_cow = Select_Point - 1;
        UartSend(CMD_DC16(10, 25 + (now_cow * 20), MenuPoint[DisplayStart + now_cow].DisplayString[Language], 0));
        CheckBusy();
        UartSend(CMD_DC16(10, 25 + (now_cow * 20), MenuPoint[DisplayStart + now_cow].DisplayString[Language], 6));
        CheckBusy();
    }
    LayerSwitchFlag = 0;
    UserLastChoose = Select_Point;
}

void OperatingSystem(void)           //操作系统函数
{
    u32 key_data;
	u8 Device_Language;
	
	while (1)
    {
        key_data =  0x0000;
        key_data =  wait_Key();
        if (key_data != 0)
        {
            break;
        }
    }

    //--------主界面显示--------//
    Timer0DelayMs(300);
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
	Device_Language = GetDeviceLanguage();
	ShowMenu(Device_Language);
    //--------主界面显示--------//
	
	
	while(1)
	{
		key_data = 0;
		key_data = wait_Key();

		switch (key_data)
		{
		case Key_Enter:
		{
			LayerSwitchFlag = 1;
			if (MenuPoint[UserChoose].ChildrenMenus == &NullMenuItem) //--------无子层menu，进入基础界面--------//
			{
				basicUserInterface();
			}
			else if (MenuPoint[UserChoose].ChildrenMenus != &NullMenuItem) //--------有子层menu，继续进入子menu--------//
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
			if (MenuPoint[0].ParentMenus != &NullMenuItem)     //--------如果不是主目录，则进入父目录--------//
			{
				if (MenuPoint[0].ParentMenus == MainMenu)          //主menu+二级MENU
				{
					MenuPoint    = MenuPoint[0].ParentMenus;  /* 3个参数决定显示 */
					UserChoose   = 0;
					DisplayStart = 0;
				}
				else// if (MenuPoint[0].ParentMenus == Program_list || MenuPoint[0].ParentMenus == Text_list)
				{
					MenuPoint    = MenuPoint[0].ParentMenus;         //判断是否为3级目录，
					UserChoose   = LastMenuRecall;                  //返回之前的3级目录的菜单起始项以及位置
					DisplayStart = LastDispalyStartRecall;
					DisplayEnd   = LastDispalyEndRecall;
				}
			}
			else if(MenuPoint[0].ParentMenus == &NullMenuItem)
			{
				LcdMainpage();
			}
			break;
		}

		case Key_Up:                //--------光标向上移动，不断显示界面变化--------//
		{
			UserChoose--;

			if (UserChoose == 0xff) //--------这个条件如果用 <0 则需要为有符号数 --------//
			{
				UserChoose = MaxItems - 1;
			}

			break;
		}

		case Key_Down:              //--------光标向下移动，不断显示界面变化--------//
		{
			UserChoose ++;

			if (UserChoose == MaxItems)
			{
				UserChoose = 0;
			}
			break;
		}
		default:
			break;
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
		Device_Language = GetDeviceLanguage();
		ShowMenu(Device_Language);
		Timer0DelayMs(100);
	}
}

void basicUserInterface(void)
{
    u32 keyDataBasic;

    while (1)  //基础用户界面循环，只接受Enter和Esce
    {
        keyDataBasic = 0;
        keyDataBasic = wait_Key();

        //--------配置为 GPIO供电--------//

        if (keyDataBasic == Key_Enter)             /*执行对应的命令*/
        {

            switch (MenuPoint[UserChoose].Command)
            {
				case OTA_EVENT:
					OTA_Event_Handle();
					break;
				
				case JUMP_APP:
					Jump_App_Handle();
					break;
				
				case UPDATE_COUNT:
					CountEventHandle();
					break;
				
				case LANGUAGE_CN:
					SetLanguageHandle(CN);
					break;
				
				case LANGUAGE_EN:
					SetLanguageHandle(EN);
					break;
				
				case FACTORY_RESET_EVENT:
					Factory_Reset_Event_Handle();
					break;
				
				default:
				break;
            }

        }
        else if (keyDataBasic == Key_Esc)          /* 退出底层用户界面，然后返回显示目录 */
        {
            DisplayStart = UserChoose;
            DisplayEnd = UserChoose + 5;
            if (DisplayEnd > MaxItems)
            {
                DisplayEnd = MaxItems;
                if (MaxItems >= 5)
                {
                    DisplayStart = MaxItems - 5;
                }
                else
                {
                    DisplayStart = 0;
                }
            }
            break;
        }
    }
}
