#include "delay.h"

/*函数名: 			Timer0DelayInit                        */
/*函数功能: 		初始化Timer0作为Delay延时的定时器使用  */
/*返回值: 无                                       		 */
/*参数  : 无                                       		 */
void Timer0DelayInit(void)
{
    REG_SCU_CTRLA &= ~(1 << 27);            //开TIMER模块时钟
    REG_TIMER_CR(TIMER0) &= ~0x01;          //关闭定时器0
    REG_TIMER_CR(TIMER0) = 0x1E;            //counter  边沿对齐  向下计数  定时器0中断屏蔽(无中断服务函数)  单次模式（single）
    REG_TIMER_PSC &= ~0x07;                 //定时器0预分频为1分频
}

/*函数名: 			Timer0DelayMs                          */
/*函数功能: 		Ms(毫秒)延时函数                       */
/*返回值: 			无                                     */
/*参数  :                                          		 */
/*      				DelayMs     延时毫秒参数               */
void Timer0DelayMs(UINT32 DelayMs)
{
    UINT32 tmp;
    UINT32 timer_clk_hz;

    timer_clk_hz = PClock;                  //TIMER是挂载在PCLK下的，FCLK = 2PCLK(PCLKDIV默认为2分频)，当FCLK为100MHz时，PClock=50MHz
    REG_TIMER_CR(TIMER0) &= ~0x01;          //关闭定时器0

    tmp = timer_clk_hz / (1 * 1000);        //定时器0为1分频时，1s可计数timer_clk_hz次，则1ms可计数timer_clk_hz/1000次
    tmp *= DelayMs;                         //DelayMs个ms可计数DelayMs*(timer_clk_hz/1000)次
    REG_TIMER_ARR(TIMER0) = tmp - 1;        //延时tmp us,真实定时器计数值会比ARR多1个
    REG_TIMER_CR(TIMER0) |= 0x01;           //使能计数器0

    while (!REG_TIMER_IF(TIMER0));          //定时器0计数未完(未发生中断)则一直等待
    REG_TIMER_CIF(TIMER0) = 0xFF;           //清除定时器0的中断标志
}

/*函数名: 			TimerDelayUs                           */
/*函数功能: 		Us(微秒)延时函数                       */
/*返回值: 			无                                     */
/*参数  :                                              */
/*      				DelayUs     延时微秒参数               */
void Timer0DelayUs(UINT32 DelayUs)
{
    UINT32 tmp;
    UINT32 timer_clk_hz;

    timer_clk_hz = PClock;                  //TIMER是挂载在PCLK下的，FCLK = 2PCLK(PCLKDIV默认为2分频)，当FCLK为100MHz时，PClock=50MHz
    REG_TIMER_CR(TIMER0) &= ~0x01;          //关闭定时器0

    tmp = timer_clk_hz / (1 * 1000000);     //定时器0为1分频时，1s可计数timer_clk_hz次，则1us可计数timer_clk_hz/1000000次
    tmp *= DelayUs;                         //DelayUs个us可计数DelayUs*(timer_clk_hz/1000000)次
    REG_TIMER_ARR(TIMER0) = tmp - 1;        //延时tmp us,真实定时器计数值会比ARR多1个
    REG_TIMER_CR(TIMER0) |= 0x01;           //使能计数器0

    while (!REG_TIMER_IF(TIMER0));          //定时器0计数未完(未发生中断)则一直等待
    REG_TIMER_CIF(TIMER0) = 0xFF;           //清除定时器0的中断标志
}


void Timer0Delay2_5Us(void)
{
    REG_TIMER_CR(TIMER0) &= ~0x01;

    REG_TIMER_ARR(TIMER0) = 50; //

    REG_TIMER_CR(TIMER0) |= 0x01;  // 使能计数器0

    while (!REG_TIMER_IF(TIMER0));

    REG_TIMER_CIF(TIMER0) = 0xFF;
}

void Timer0Delay5Us(void)
{
    REG_TIMER_CR(TIMER0) &= ~0x01;

    REG_TIMER_ARR(TIMER0) = 175; //

    REG_TIMER_CR(TIMER0) |= 0x01;  // 使能计数器0

    while (!REG_TIMER_IF(TIMER0));

    REG_TIMER_CIF(TIMER0) = 0xFF;
}


void delay(UINT32 count)
{
    while (count--);
}

