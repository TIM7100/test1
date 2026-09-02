#include "bsp_key.h"


#define KEY 0x1E000000
#define KeyTime   0x18000

#define ENTER_GPIO_PIN     GPIO_PIN_9
#define CANCEL_GPIO_PIN    GPIO_PIN_10
#define UP_GPIO_PIN        GPIO_PIN_11
#define DOWN_GPIO_PIN      GPIO_PIN_12


#define KEY_ENTER   (!GPIO_ReadInputDataBit(GPIOD, ENTER_GPIO_PIN))
#define KEY_ESC     (!GPIO_ReadInputDataBit(GPIOD, CANCEL_GPIO_PIN))
#define KEY_UP      (!GPIO_ReadInputDataBit(GPIOD, UP_GPIO_PIN))
#define KEY_DOWN    (!GPIO_ReadInputDataBit(GPIOD, DOWN_GPIO_PIN))

void KeyInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /* Enable CLK */

//    System_Module_Enable(EN_GPIOCD);
    GPIO_InitStructure.Pin       = ENTER_GPIO_PIN | CANCEL_GPIO_PIN | UP_GPIO_PIN | DOWN_GPIO_PIN;
    GPIO_InitStructure.Mode      = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull      = GPIO_PULLUP;
    GPIO_InitStructure.Alternate = GPIO_FUNCTION_0;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* 初始引脚置高 */
    GPIOCD->IDATA |= KEY;

}

u8 KeyScan(void)
{
    u8 KeyValue = KEY_NO_DOWN;
    static u8 KeyLock = 0;
	static u32 AutoTime = KeyTime;

    /* 按键按下, 且按键处于解锁状态时，会将按键锁住, 并返回按键值 */
    if ((KEY_ENTER || KEY_ESC || KEY_UP || KEY_DOWN) && !KeyLock)
    {
        System_Delay_MS(30);     //消抖

        if (KEY_ENTER)
        {
            KeyLock = 1;
            KeyValue = KEY_ENTER_VALUE;
        }
        else if (KEY_ESC)
        {
            KeyLock = 1;
            KeyValue = KEY_ESC_VALUE;
        }
        else if (KEY_UP)
        {
            KeyLock = 1;
            KeyValue = KEY_UP_VALUE;
        }
        else if (KEY_DOWN)
        {
            KeyLock = 1;
            KeyValue = KEY_DOWN_VALUE;
        }
		if (AutoTime == 0)
			AutoTime = KeyTime;
    }

	/* 按键锁自动解锁时间 */
	if (KeyLock)
	{
		AutoTime -= 1;
	}
	
    /* 所有按键松开或自动解锁时间到，将按键解锁 */
    if ((!KEY_ENTER && !KEY_ESC && !KEY_UP && !KEY_DOWN) || AutoTime == 0)
    {
		System_Delay_MS(10);     //消抖
		if (!KEY_ENTER && !KEY_ESC && !KEY_UP && !KEY_DOWN)
        {
			KeyLock = 0;
			AutoTime = KeyTime;
		}
		else if(AutoTime == 0)
		{
			KeyLock = 0;
			AutoTime = KeyTime - 0x11000;
		}
    }
    return KeyValue;
}

