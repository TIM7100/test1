#ifndef USER_PERIPHERAL_H
#define USER_PERIPHERAL_H

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "sys.h"

/*--------KEY对应的引脚号和IO号-----------*/
/*   KEY0   KEY1   KEY2   KEY3   KEY4     */
/*   KEY1   KEY2   KEY3   KEY4   KEY5     */
/*   55     56     57     58     59       */
/*   PD8    PD9    PD10   PD11   PD12     */
/*   Enter  Left   Right  Up     Down     */
/*----------------------------------------*/
#define AutoKey	PDin(13) //PD13
#define Key_PORT    GPIOD

#define Enter		    GPIO_Pin_8
#define Esc 		    GPIO_Pin_9     /* Esc and left are the same. */
#define Left 		    GPIO_Pin_9 
#define Count		    GPIO_Pin_10
#define Up   		    GPIO_Pin_11
#define Down 		    GPIO_Pin_12

//#define KEY         ((u16)0x1F00)  /* 代表了12 11 10 9 8 */
#define KEY         ((u16)0x1B00)  /* 代表了12 11 9 8 */

#define Key_Enter   ((u16)KEY & (~Enter))
#define Key_Left    ((u16)KEY & (~Left ))
#define Key_Esc     ((u16)KEY & (~Esc  ))
#define Key_Count   ((u16)KEY & (~Count))
#define Key_Up      ((u16)KEY & (~Up   ))
#define Key_Down    ((u16)KEY & (~Down ))

#define PRESSED  1
#define RELEASE  2

void SystemDrvInit(void);
void PressKeyInit(void);
//void initAutoKey(void);
u16 PressKeyScan(void);
u16 AutoKeyScan(void);

void ChooseVoltageInit(void);     //烧录 检测参考电压选择
extern u8 AutoStatus;





#endif 

