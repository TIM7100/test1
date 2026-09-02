#include "gpio.h"
#include "delay.h"

int Keystatus;
u32 key_data;

void KeyInit(void)
{
    REG_SCU_MUXCTRLB &= ~(0xFF << 8);         //GPIO20,21,22,23
    REG_GPIO_DIR(GPIOA) &= ~(0xF << 20);      //GPIO20,21,22,23 IN
    REG_GPIO_IDATA(GPIOA) |= KEY_DATA;
}

u32 wait_Key(void)
{
    /* 1ms如果检测到还是低电平，则为按下*/
    u32 key_data1, key_data2;

    do
    {
        key_data1 = REG_GPIO_IDATA(GPIOA) & KEY_DATA;
        Timer0DelayMs(1);
        key_data2 = REG_GPIO_IDATA(GPIOA) & KEY_DATA;
    }
    while (!((key_data1 != KEY_DATA) && (key_data2 != KEY_DATA) && (key_data1 == key_data2)));
// 		    while (((key_data1 != Key_Enter) && (key_data1 != Key_Esc) && (key_data1 != Key_Up) && (key_data1 != Key_Down) && (key_data1 == key_data2)));

    if ((key_data1 == Key_Enter) || (key_data1 == Key_Esc))
    {
    }

    return    key_data1;
}

