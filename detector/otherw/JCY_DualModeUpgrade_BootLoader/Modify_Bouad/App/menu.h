
/****************************************************
* 支持库说明：提供了使用菜单的基本数据结构          *
****************************************************/
/****************************************************
*   使用说明：                                      *
*      要定义一个菜单，需要做以下几步工作           *
*      1、定义一个结构体数组                        *
*      2、如果这个菜单是子菜单，那么把上级菜单的    *
*         *ChildrenMenus指针指向该数组的首地址,     *
*         并且设置ParentMenus为上层目录的地址       *
*      3、如果这个菜单是跟菜单，那么直接将MenuPoint *
*         指针指向该数组的首地址                    *
*      4、通过写一个函数为该数组的每一个元素初始化 *
*      5、如果菜单项是最终选项，那么将*ChildrenMenus*
*         置为NULL，将函数指针*Subs指向实现功能的   *
*         函数。                                    *
****************************************************/

#ifndef __MENU_H
#define __MENU_H

#include "fxx_std.h"
#include "crc.h"




typedef enum
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* 测试Command指令码                                                                            */
    /*---------------------------------------------------------------------------------------------------------*/

    CMD_RESERVE,
		CMD_CHECK


} CommandMenuType;

/* 目录结构体声明 */
struct MenuItem
{
    unsigned char MenuCount;          /* 当前层节点数 */

    char *DisplayString;   /* 菜单标题 */
    struct MenuItem *ChildrenMenus; /* 子节点 */
    struct MenuItem *ParentMenus;   /* 父节点 */
    const u8 AreaCode;               /* 区域编号 */
    const u16 VolumeSort;               /* 容量*/
    CommandMenuType Command;         /* 命令选项 */

};

extern struct MenuItem NullMenuItem;
extern struct MenuItem NullPMenuItem;

/* 第一级Menu */
extern struct MenuItem MainMenu[];

/* 第二级Menu */
/* ------------------------------------ 检测部分 ----------------------------------------------------- */

//extern struct MenuItem NTestAuto[];
//extern struct MenuItem LanguageMenu[];
//extern struct MenuItem NTestAutoEN[];
//extern struct MenuItem MainMenuEN[];





void NullSubs(void);   /* 函数默认extern属性,不需加extern */

#endif

