#include "gpio.h"
#include "fxx_gpio.h"


void BuzzerInit(void)
{

    GPIO_InitTypeDef GPIO_InitStruct;

    System_Module_Enable(EN_GPIOCD);
    GPIO_InitStruct.Pin       = GPIO_PIN_4;
    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP ;
    GPIO_InitStruct.Alternate = GPIO_FUNCTION_0;//gpio模式
    GPIO_Init(GPIOC, &GPIO_InitStruct);

}

//蜂鸣器响起
void BuzzerSound(u8 Times)
{
    u8 i = 0;
    u8 j = 0;
    for (i = 0; i < Times; i++)
    {
        for (j = 0; j < 0xFF; j++)
        {
            GPIO_WriteBit(GPIOC, GPIO_PIN_4, Bit_RESET);
            Delay_us(100);
            GPIO_WriteBit(GPIOC, GPIO_PIN_4, Bit_SET);
            Delay_us(150);
        }
        if (Times > 1)
        {
            Delay_ms(100);
        }
    }

}



void GPIO_Configuration(void)
{
    // do nothing here
}


void KeyInit(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;
    /* Enable CLK */


    System_Module_Enable(EN_GPIOCD);
    GPIO_InitStructure.Pin       = GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_InitStructure.Mode      = GPIO_MODE_INPUT;
	  GPIO_InitStructure.Pull      = GPIO_PULLUP ;
    GPIO_InitStructure.Alternate = GPIO_FUNCTION_0;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIOCD->IDATA |= KEY;

}

u32 WaitKey(void)
{
    /* 20ms如果检测到还是低电平，则为按下*/
    u32 KeyValueBefore, KeyValueAfter;

    do
    {
        KeyValueBefore = GPIOCD->IDATA & KEY;
        Delay_ms(50);
        KeyValueAfter =  GPIOCD->IDATA & KEY;
    }
    while (!((KeyValueBefore != KEY) && (KeyValueAfter != KEY) && (KeyValueBefore == KeyValueAfter)));

    if ((KeyValueBefore == Key_Enter) || (KeyValueBefore == Key_Esc))
    {
        LedOff();
    }

    return    KeyValueBefore;
}




void LedInit(void)                                                              //灯引脚初始化
{
    GPIO_InitTypeDef GPIO_LED_Handle;
    System_Module_Enable(GPIO_LED_CLK);

    GPIO_LED_Handle.Pin       = GPIO_LED_GREEN_Pin | GPIO_LED_RED_Pin;
    GPIO_LED_Handle.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_LED_Handle.Pull      = GPIO_PULLUP;
    GPIO_LED_Handle.Alternate = GPIO_FUNCTION_0;
    GPIO_Init(GPIO_LED_Port, &GPIO_LED_Handle);

    LedOn();
}




