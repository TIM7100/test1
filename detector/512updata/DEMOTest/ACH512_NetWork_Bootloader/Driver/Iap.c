#include "Iap.h"
#include "eflash.h"
#include "uart.h"
#include "ExternalFlash.h"
#include "ach512timer0delay.h"

iapfun RUN_MCUCODE;





void IapLoadApp(u32 Addr)
{
    if ((((*(volatile u32*)(Addr + 4)) & 0xFF000000) == 0x00000000) && (((*(volatile u32*)Addr) & 0x2FFE0000) == 0x20000000)) //判断是否为0X00XXXXXX.检查栈顶地址是否合法.
    {
// 		IapStatusRespond(CMD_RUN_APP, STATUS_TRUE);
		Timer0DelayMs(50);
        RUN_MCUCODE = (iapfun) * (volatile u32*)(Addr + 4);     //用户代码区第二个字为程序开始地址(复位地址)
        MSR_MSP(*(volatile u32*)Addr);                          //初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
        RUN_MCUCODE();                                          //跳转到APP.
    }
}

//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr)
{
    MSR MSP, r0  //set Main Stack value
    BX r14
}

