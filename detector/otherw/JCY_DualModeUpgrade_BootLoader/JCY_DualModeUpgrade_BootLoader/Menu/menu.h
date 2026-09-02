
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

/* 事件码集合 */
typedef enum
{
    JUMP_EVENT = 0,
    OTA_EVENT,
//    COUNT_EVENT,
    LANGUAGE_EVENT,
    FACTORY_RESET_EVENT,
	Event_NULL = 0xffffffff
} EventCode;


/* 目录结构体声明 */
typedef struct _MenuItem_
{
    unsigned char MenuCount;          /* 当前层节点数 */
	char DisplayString[2][30];        /* 菜单标题 1:中文；0：英语 */
	struct _MenuItem_ *ChildrenMenus; /* 子节点 */
	struct _MenuItem_ *ParentMenus;   /* 父节点 */
	const EventCode FunctionCode;     /* 功能代码 */

}MenuItem;

extern MenuItem NullMenuItem;
extern MenuItem NullPMenuItem;

/* 第一级Menu */
extern MenuItem FunctionMenu[];

/* 第二级Menu */
extern MenuItem LanguageMenu[];


void NullSubs(void);   /* 函数默认extern属性,不需加extern */

#endif

