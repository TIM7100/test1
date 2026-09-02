/*
  ******************************************************************************
  * @file    APP.h
  * @author  Chris_Kyle
  * @version V1.0.0
  * @date    2020
  * @brief   GPIO demo Header file.
  ******************************************************************************
*/
#ifndef _MENU_APP_H__
#define _MENU_APP_H__

#include "fxx_std.h"
#include "menu.h"

extern __IO u8 RefreshFlag;
extern __IO u8 SelectPoint;
extern __IO u8 NodeChangFlag;

extern __IO MenuItem *MenuPoint;                 //结构体指针,指向结构体后由内部函数指针指向功能函数
extern __IO unsigned char NodeNum;           //用户所选菜单节点

extern __IO unsigned char MaxNodes;         //所选菜单下节点数量

extern __IO unsigned char ShowCount;        //同屏显示时允许的菜单节点数量
extern __IO unsigned char DisplayStart;     //同屏显示时的起始菜单节点
extern __IO unsigned char DisplayEnd;       //同屏显示时的最后一个菜单节点


void MenuBspInit(void);
void MainMenu(void);
void MenuOperatingSystem(void);

#endif
