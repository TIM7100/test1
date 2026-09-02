#include "menu_app.h"
#include "bsp_timer7.h"
#include "bsp_usart1.h"
#include "bsp_usart3.h"
#include "w5500_user_conf.h"
#include "esp8266_config.h"

__IO u8 NetWork_Flag = ESP8266;

int main(void)
{
    __set_PRIMASK(0);                           //开启全局中断
    System_Init();
    System_Module_Enable(EN_GPIOAB);
    System_Module_Enable(EN_GPIOCD);

    UART1_Init(115200);
	UART3_Init(921600);
    MenuBspInit();
    //Timer7_Init();
    W5500Config();
	WiFi_IO_Init();
	
    while (1)
    {
        MenuOperatingSystem();
    }
}

