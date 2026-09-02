#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "commandlist_NPG83_10P.h"
#include "menu.h"
#include "user_OS.h"
#include "w25qxx.h"
#include "user_Peripheral.h"
#include "CheckFunction.h"
#include "ClearFunction.h"
#include "UserFunctionInterface.h"
#include "stm32f10x.h"

FlashTable_Type FlashTable;

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     basicUserInterface                                                                        */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               基础用户界面                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
void UserFunctionInterface(void)
{
    u16 KeyPressed;

    Confirm_Selection(MenuPoint[UserChoose].DisplayString);

    while (1)  /*  基础用户界面循环,只接受Enter键(执行命令)和Esc键(返回父目录) */
    {
        KeyPressed = 0x0000;
        KeyPressed = AutoKeyScan();

        if (KeyPressed == Key_Enter)             /* 执行对应命令 */
        {

            switch (MenuPoint[UserChoose].Command)
            {
            case    CLEARSECURITYCODE:                              /* 解锁清除函数功能 */
                CheckChipInterface(REMOVE_SECURITY_CODE);
                break;
            case    SHOWVERSIONSELECT:                              /* 显示版本功能 */
                GetVersionScreen();
                ShowVersionInterface();
                break;

            }
        }
        else if (KeyPressed == Key_Esc)          /* 退出底层用户界面，然后返回显示目录 */
        {
            DisplayStart = UserChoose;
            DisplayEnd = UserChoose + 6;
            if (DisplayEnd > MaxItems)
            {
                DisplayEnd = MaxItems;
                if (MaxItems >= 6)
                {
                    DisplayStart = MaxItems - 6;
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

