#include  "user_os.h"
#include  "delay.h"
#include  "gpio.h"
#include  "lcd_menu.h"
#include  "usart_lcd.h"
#include  "menu.h"


/********** 来源：lcd_menu ************/
extern struct MenuItem(*MenuPoint);  	//结构体指针,指向结构体后由内部函数指针指向功能函数
extern unsigned char DisplayStart; 		//显示时的第一个菜单项
extern unsigned char DisplayEnd;		 //显示时的第一个菜单项
extern unsigned char UserChoose;   		//用户所选菜单项
extern unsigned char DisplayPoint; 		//显示指针
extern unsigned MaxItems;              //同级最大菜单数
extern unsigned char ShowCount;        //同屏显示菜单数

extern u8 UserLastChoose;
extern u8 EnterEscFlag;
/************************/


/********** lcd_menu.c 中外部声明了这些变量 ************/
u8  SelectPoint;
u8  RefreshFlag;
/************************/


u8 LastMenuRecall;
u8 LastDispalyStartRecall;
volatile u8 LastDispalyEndRecall;



volatile u8 SecMenuRecall = 0;
volatile u8 SecDispalyStartRecall = 0;
volatile u8 SecDispalyEndRecall = 0;

void OperatingSystem(struct MenuItem *HeadMenu)           //操作系统函数
{
    u32 KeyValue;
	
	
	//--------检测识别仪界面，需要按Enter键才能显示下面的界面--------//
    while (1)
    {
        KeyValue =  0x0000;
        KeyValue =  WaitKey();
        if (KeyValue != 0)
        {
            break;
        }
    }

    //--------主界面显示--------//
    Delay_ms(150);
    MenuPoint = HeadMenu;			
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
    RefreshFlag = 1;				//更新界面
    SelectPoint = 1;				//选择


    ShowMenu();
    //--------主界面显示--------//

	
    while (1)
    {
        KeyValue = 0x0000;
        KeyValue = WaitKey();
        Delay_ms(150);
        switch (KeyValue)
        {
        case Key_Enter:
        {
            EnterEscFlag = 1;

            if (MenuPoint[UserChoose].ChildrenMenus == &NullMenuItem) //--------无子层menu，进入基础界面--------//
            {
                BasicUserInterface();
            }
            else if (MenuPoint[UserChoose].ChildrenMenus != &NullMenuItem) //--------有子层menu，继续进入子menu--------//
            {
                SecMenuRecall = LastMenuRecall;
                SecDispalyStartRecall = LastDispalyStartRecall;
                SecDispalyEndRecall = LastDispalyEndRecall;


                LastMenuRecall = UserChoose;
                LastDispalyStartRecall = DisplayStart;
                LastDispalyEndRecall = DisplayEnd;    //菜单终止项

                MenuPoint = MenuPoint[UserChoose].ChildrenMenus;			//更新指向子层页面，在lcd_menu.c的ShowMenu中显示
                UserChoose = 0;
                DisplayStart = 0;
            }
            break;
        }

        case Key_Esc:
        {
            EnterEscFlag = +1;

            if (MenuPoint[0].ParentMenus != &NullMenuItem)     //--------如果不是主目录，则进入父目录--------//
            {
                if (MenuPoint[0].ParentMenus == MainMenu)           //主menu+二级MENU
                {
                    MenuPoint = MenuPoint[0].ParentMenus;
                    UserChoose   = 0;
                    DisplayStart = 0;
                }
                else
                {
                    if (MenuPoint[0].ParentMenus == &NullPMenuItem)
                    {


                    }
                    else
                    {
                        MenuPoint = MenuPoint[0].ParentMenus;
                    }

                    UserChoose   = LastMenuRecall;                  //返回之前的3级目录的菜单起始项以及位置
                    DisplayStart = LastDispalyStartRecall;
                    DisplayEnd   = LastDispalyEndRecall;

                    LastMenuRecall = SecMenuRecall;
                    LastDispalyStartRecall  = SecDispalyStartRecall;
                    LastDispalyEndRecall = SecDispalyEndRecall;
                }
            }
			else if (MenuPoint[0].ParentMenus == &NullMenuItem)
			{
				Jump_IAP();
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
            UserChoose++;

            if (UserChoose == MaxItems)
            {
                UserChoose = 0;
            }
            break;
        }
        default:
            break;
        }
		
		/* 这是什么 */
        if ((UserChoose == 0) && (DisplayStart == 0))
        {
            if ((UserLastChoose == 2) && (EnterEscFlag == 0))
            {
                RefreshFlag = 0;
            }
            else
            {
                RefreshFlag = 1;
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
            SelectPoint = 1;
        }
        else if ((UserChoose >= DisplayStart) && (UserChoose < DisplayEnd))
        {
            RefreshFlag = 0;
            SelectPoint =  UserChoose - DisplayStart + 1;
        }
        else if (UserChoose < DisplayStart)
        {
            RefreshFlag = 1;
            if (UserChoose <= 4)                //
            {
                if (UserChoose == 0)
                {
                    SelectPoint = 1;
                }
                else
                {
                    SelectPoint = DisplayStart;
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
                DisplayStart -= 5;
                DisplayEnd = DisplayStart + 5;
                SelectPoint = 5;
            }
        }
        else if (UserChoose >= DisplayEnd)
        {
            RefreshFlag = 1;
            if ((MaxItems - UserChoose) < ShowCount)
            {
                SelectPoint = 6 + UserChoose - MaxItems;
                DisplayStart = MaxItems - 5;
                DisplayEnd = MaxItems;
            }
            else
            {
                DisplayStart += 5;
                DisplayEnd = DisplayStart + 5;
                SelectPoint = 1;
            }
        }

        if (EnterEscFlag == 1)
        {
            RefreshFlag = 1;
        }
		
        ShowMenu();
    }
}
