#include "Timer3.h"

void Timer3_Init(void)
{

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //①时钟 TIM3 使能

    //定时器 TIM3 初始化

    TIM_TimeBaseStructure.TIM_Period = 40000 - 1; //设置自动重装载寄存器周期的值

    TIM_TimeBaseStructure.TIM_Prescaler = 36000 - 1; //设置时钟频率除数的预分频值

    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割

    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM 向上计数

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);  //②初始化 TIM3

    //中断优先级 NVIC 设置

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3 中断

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //先占优先级 0 级

    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级 3 级

    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  //IRQ 通道被使能

    NVIC_Init(&NVIC_InitStructure);   //④初始化 NVIC 寄存器


    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIMx的中断待处理

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //③允许更新中断

    TIM_Cmd(TIM3, ENABLE);  //⑤使能 TIM3

}



void Timer3_start(void)
{
    TIM_Cmd(TIM3, DISABLE);       // 关闭定时器
    TIM3->CNT = 0;
    TIM_Cmd(TIM3, ENABLE);          // 打开定时器
}


float timer3_stop(void)
{
    float timer;
    timer = TIM3->CNT / 2;     //获取计数值 ms   最大计时值20s
    TIM_Cmd(TIM3, DISABLE);         // 关闭定时器

    return timer;
}
