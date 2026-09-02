/*
  ******************************************************************************
  * @file    fxx_it.c
  * @brief   fxx_it source File.
  ******************************************************************************
*/

#include "fxx_it.h"
#include "bsp_uart_lcd.h"
#include "dhcp.h"
#include "firmware_download.h"
#include "w5500_user_conf.h"


u32  Timeus = 0;

void UART2_IRQHandler(void)
{
    UART2_IRQ();
}


void TIM7_IRQHandler(void)
{
    if (TIM7->SR & TIMER_SR_UIF)
    {
        TIM7->SR &= (~BIT0);
        Timeus++;
    }

}

