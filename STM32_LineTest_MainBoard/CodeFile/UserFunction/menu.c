/*---------------------------------------------------------------------------------------------------------*/
/* Includes of local headers                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#include "menu.h"

const u8 EmptyHex[2] = {0x00, 0x00}; /* 空的Hex */

struct MenuItem NullMenuItem;       /* 空的Menu */

/* 第一级Menu */
struct MenuItem MainMenu[2] =
{
    /*MenuCount         DisplayString                          ChildrenMenus            ParentMenus          AreaCode          VolumeSort                      Command*/
    {2,                   "01.绑定测试",                       &NullMenuItem,            &NullMenuItem,      0xFF,               0xFF,                         CLEARSECURITYCODE},
    {2,                   "02.显示系统版本",                   &NullMenuItem,            &NullMenuItem,      0xFF,               0xFF,                         SHOWVERSIONSELECT},
};


void NullSubs(void)  /* 空函数 */
{

}

