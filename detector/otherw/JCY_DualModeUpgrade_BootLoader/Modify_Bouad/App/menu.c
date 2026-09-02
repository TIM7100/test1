/*---------------------------------------------------------------------------------------------------------*/
/* Includes of local headers                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#include "menu.h"

const u8 EmptyHex[2] = {0x00, 0x00}; /* 空的Hex */

struct MenuItem NullMenuItem;       /* 空的Menu */
struct MenuItem NullPMenuItem;
/* 第一级Menu *******************************************************************************************************************/
struct MenuItem MainMenu[] =
{
    /*MenuCount       DisplayString           ChildrenMenus            ParentMenus             AreaCode        VolumeSort      Command            */
    {2,             "01.自动检测",               &NullMenuItem,           &NullMenuItem,          0xFF,           0xFFFF,         CMD_CHECK},
    {2,             "02.选型检测",               &NullMenuItem,           &NullMenuItem,          0xFF,           0xFFFF,         CMD_RESERVE},
   
};



/* 第二级Menu ********************************************************************************************************************/

//struct MenuItem NTestAuto[] =
//{
//  /*MenuCount       DisplayString           ChildrenMenus            ParentMenus             AreaCode        VolumeSort      Command            */
//    {3,             "01.NPG系列",           NPGTestMenu,             MainMenu,               0xFF,           0xFFFF,         CMD_RESERVE},
//    {3,             "02.GPR系列",           GPRTestMenu,             MainMenu,               0xFF,           0xFFFF,         CMD_RESERVE},
//    {3,             "03.CEXV系列",          CEXVTestMenu,            MainMenu,               0xFF,           0xFFFF,         CMD_RESERVE},

//};

//struct MenuItem NTestAutoEN[] =
//{
//  /*MenuCount       DisplayString           ChildrenMenus            ParentMenus             AreaCode        VolumeSort      Command            */
//    {3,             "01.NPG Series",        NPGTestMenu,             MainMenuEN,             0xFF,           0xFFFF,         CMD_RESERVE},
//    {3,             "02.GPR Series",        GPRTestMenu,             MainMenuEN,             0xFF,           0xFFFF,         CMD_RESERVE},
//    {3,             "03.CEXV Series",       CEXVTestMenu,            MainMenuEN,             0xFF,           0xFFFF,         CMD_RESERVE},

//};

//struct MenuItem LanguageMenu[] = /* CEXV测试菜单 */
//{
//    /*MenuCount     DisplayString           ChildrenMenus               ParentMenus           AreaCode        VolumeSort      Command*/
//    {2,            "01.简体中文",         &NullMenuItem,              MainMenu,             0x00,           0xFFFF,         CMD_LANGUAGE},
//    {2,            "02.English",              &NullMenuItem,              MainMenu,             0x01,           0xFFFF,         CMD_LANGUAGE},
//
//};








//void NullSubs(void)  /* 空函数 */
//{

//}




//////////////////////////////////










