/*---------------------------------------------------------------------------------------------------------*/
/* Includes of local headers                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#include "menu.h"

const u8 EmptyHex[2] = {0x00, 0x00}; /* 空的Hex */

struct MenuItem NullMenuItem;       /* 空的Menu */
/* 第一级Menu *******************************************************************************************************************/
struct MenuItem MainMenu[] =
{
    /*MenuCount     DisplayString           					  ChildrenMenus            ParentMenus             Command            */
    {5,             {"01.应用入口"    ,"01.APP Entry"},        	 &NullMenuItem,           &NullMenuItem,          JUMP_APP},
    {5,             {"02.更新固件"    ,"02.UpdateFirmware"},     &NullMenuItem,           &NullMenuItem,          OTA_EVENT},
	{5,             {"03.更新点数"    ,"03.UpdateCount"},     	 &NullMenuItem,           &NullMenuItem,          UPDATE_COUNT},
	{5,             {"04.语言选择"    ,"04.Language"},     	 	 LanguageMenu,            &NullMenuItem,          CMD_RESERVE},
	{5,             {"05.恢复出厂设置","05.FactoryReset"},       &NullMenuItem,           &NullMenuItem,          FACTORY_RESET_EVENT},
};

struct MenuItem LanguageMenu[] =
{
    /*MenuCount     DisplayString          				 ChildrenMenus      ParentMenus        EventCode */
    {2,    			{"1.English", "1.English"},         &NullMenuItem,     MainMenu,          LANGUAGE_EN },
    {2,    			{"2.中文",    "2.中文"},            &NullMenuItem,     MainMenu,          LANGUAGE_CN },
};


void NullSubs(void)  /* 空函数 */
{

}












