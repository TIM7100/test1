#include "user_Peripheral.h"
#include "Timer0.h"
#include "MainSlaveInterface.h"

u8 AutoStatus = RELEASE;
/*---------------------------------------------------------------------------------------------------------*/
/* Function:     initKey                                                                                   */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               初始化KEYBOARD的IO                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
void PressKeyInit(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
	
	  //机械按键初始化
    GPIO_InitStructure.GPIO_Pin = KEY;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ;  /* GPIO_Mode_IN_FLOATING 浮空输入 浮空为1，按下为0 */
    GPIO_Init(Key_PORT, &GPIO_InitStructure);
	
	  //自动触发按键初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     scanKey                                                                                   */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               返回值为KEY值。                                                                           */
/* Description:                                                                                            */
/*               等待有KEY的按下,有KEY按下后返回KEY值。                                                    */
/*               1 0f00                                                                                    */
/*               2 1e00                                                                                    */
/*               3 1700                                                                                    */
/*               4 1d00                                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
u16 PressKeyScan(void)
{
    /* 20ms内持续为0，则判断有key按下。 */
    u16 key_data1, key_data2;

    do
    {
        key_data1 = GPIO_ReadInputData(Key_PORT) & KEY;
        delay_ms(15);
        key_data2 = GPIO_ReadInputData(Key_PORT) & KEY;
    }
    while (!((key_data1 != KEY) && (key_data2 != KEY) && (key_data1 == key_data2)));

    delay_ms(15);
    return    key_data1;
}

//返回1即为按钮被重新按下
u16 AutoKeyScan(void)
{
    u16 key_data1;

    if (AutoStatus == RELEASE)   //若当前状态为释放状态
    {
        if (AutoKey == 0)    //若按键按下
        {
            delay_ms(5);
            if (AutoKey == 0)
            {
                AutoStatus = PRESSED;
                return Key_Enter;
            }
        }
    }
    else if (AutoStatus == PRESSED) //若当前状态为按下状态
    {
        if (AutoKey)      //若按键释放
        {
            delay_ms(5);
            if (AutoKey)
            {
                AutoStatus = RELEASE;
                //return 0;
            }
        }
    }

    if ((GPIO_ReadInputData(Key_PORT) & KEY) != KEY)  //判断其他按钮是否触发
    {
        delay_ms(5); //5mS防抖动
        key_data1 = GPIO_ReadInputData(Key_PORT) & KEY; //记录防抖后的KEY输入
        if (key_data1 != KEY) //若KEY值不全为高(即有输入)，返回按键值
        {
            return key_data1;
        }
    }

    return 0;
}


//电压选择
void ChooseVoltageInit(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
	
	  //初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;  /* 推挽输出 */
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	  //初始状态
    SetHighVoltage();//电源板选HIGH电压，烧录电压       //比较电压
}








