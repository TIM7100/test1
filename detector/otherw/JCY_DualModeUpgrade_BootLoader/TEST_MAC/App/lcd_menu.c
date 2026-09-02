#include "lcd_menu.h"
#include "menu.h"
#include "gpio.h"
#include "delay.h"
#include "uart.h"
#include "lcd.h"
#include "Sx_Check.h"


/*--------------全局变量------------------------------------------------------
 * 在user_os中被使用 *
**************************************/
struct MenuItem(*MenuPoint) = MainMenu;  //结构体指针,指向结构体后由内部函数指针指向功能函数

unsigned char DisplayStart = 0; //显示时的第一个菜单项
unsigned char DisplayEnd = 0; //显示时的第一个菜单项
unsigned char DisplayPoint = 0; //显示指针


unsigned char UserChoose = 0;   //用户所选菜单项
unsigned MaxItems;              //同级最大菜单数
const unsigned char ShowCount = 5;        //同屏显示菜单数


u8 EnterEscFlag;
u8 UserLastChoose;
/**************************************/



/*  user_os中定义的全局变量  */
extern u8 RefreshFlag;
extern u8 SelectPoint;
//********************************//








/*******************************************************************************
* Function Name  : ShowMenu
* Description    : 目录显示函数
* Input          : None
* Output         : None
* Return         : None
* Remark         :
通过修改 MenuPoint的指向
    DisplayStart 来修改menu的开始位置
    UserChoose 修改指针的位置
*******************************************************************************/
void ShowMenu(void)
{
    u8 color;
    unsigned char n;
    u8 last_cow;
    u8 now_cow;
    u8 Display_count;

    n = 0;
    MaxItems = MenuPoint[0].MenuCount; //第一个目录指向了MainMenu
    DisplayPoint = DisplayStart;

    if (RefreshFlag != 0)
    {
        UartSend("CLR(0);\r\n");				
        CheckBusy();

        UartSend("DC16(35,0,'请执行操作',15);\r\n");
        CheckBusy();

        n = 0;
		
		/* 显示当前页面的可选项 */
        for (Display_count = DisplayStart; Display_count < DisplayEnd; Display_count++)
        {
            if ((n + 1) == SelectPoint)  // 如果是选中行，则显示不同的颜色
            {
                color = 6;
            }
            else
            {
                color = 4;
            }
            UartSend(CMD_DC16(10, 25 + (n * 20), MenuPoint[Display_count].DisplayString, color));
            CheckBusy();
            n++;
        }
    }
    else                    //上下行切换时颜色变化
    {
        last_cow = UserLastChoose - 1;
        UartSend(CMD_DC16(10, 25 + (last_cow * 20), MenuPoint[DisplayStart + last_cow].DisplayString, 0));
        CheckBusy();
        UartSend(CMD_DC16(10, 25 + (last_cow * 20), MenuPoint[DisplayStart + last_cow].DisplayString, 4));
        CheckBusy();

        now_cow = SelectPoint - 1;
        UartSend(CMD_DC16(10, 25 + (now_cow * 20), MenuPoint[DisplayStart + now_cow].DisplayString, 0));
        CheckBusy();
        UartSend(CMD_DC16(10, 25 + (now_cow * 20), MenuPoint[DisplayStart + now_cow].DisplayString, 6));
        CheckBusy();
    }
    EnterEscFlag = 0;
    UserLastChoose = SelectPoint;
}


void ConfirmSelection(u8 *TitleName)
{
    u32 key_data1;
    u32 key_data2;

    UartSend("CLR(0);\r\n");
    CheckBusy();


    UartSend(CMD_DC16(35, 0, "请确认命令", 15));
    CheckBusy();

    UartSend(CMD_DC16(10, 30, (char*) TitleName, 15));
    CheckBusy();

    UartSend("DC16(16,90,'- 按\"确定\"继续 -',4);\r\n");
    CheckBusy();

}

void BasicUserInterface(void)
{
    u32 keyDataBasic;
    u8 AreaCodeSel;

    ConfirmSelection((u8 *)MenuPoint[UserChoose].DisplayString);
    while (1)  //基础用户界面循环，只接受Enter和Esce
    {
        keyDataBasic = 0;
        keyDataBasic = WaitKey();

        //--------配置为 GPIO供电--------//

        if (keyDataBasic != 0x1A000000 && keyDataBasic != 0x1C000000)
        {
            UartSend("CLR(0);\r\n");
            CheckBusy();
        }

        if (keyDataBasic == Key_Enter)             /*执行对应的命令*/
        {
            AreaCodeSel = MenuPoint[UserChoose].AreaCode;

            //检测型号
            switch (MenuPoint[UserChoose].Command)
            {


            //NPG
            case 0:
                //GetTableHead(0);
                break;

            /*自动检*/
            case CMD_CHECK:
                AutoTest();
								Delay_ms(500);
                break;

            default:
                break;

            }


        }
        else if (keyDataBasic == Key_Esc)          /*退出用户底层用户界面，返回显示目录*/
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
            Delay_ms(80);
            break;
        }
    }
}

void Jump_IAP(void)
{
	u32 appxaddr = 0x00000000;
	UartSend("CLR(0);\r\n");
    CheckBusy();
//	iap_load_app(appxaddr);
}

