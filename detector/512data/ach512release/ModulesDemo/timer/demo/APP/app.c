/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : app.c
 * Description : application example source file
 * Author(s)   : Eric
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#include  "app.h"
#include  "timer.h"

void timer_1s_printf(void)
{
    printfS("timer delay 1s!\n");
}

void timer_delay_test(void)
{
    printfS("timer test is start!\n"); //仅测试TIMER0，其他类似

    timer_set_ms(TIMER0, 1000, timer_1s_printf); //timer每延时1s会执行打印函数
    //timer_set_us(TIMER0, 1000000, timer_1s_printf); //timer每延时1s会执行打印函数
    timer_start(TIMER0); //enable timer0

    while(1)
    {
        ;
    }
}

void timer_cc_test(void)
{

    printfS("timer cc test is start!\n"); //仅测试TIMER0的capture

    capture_set(TIMER0, CAPTURE_TRIGGER_FALLING);

    timer_start(TIMER0); //enable timer0
    capture_start(TIMER0);

    printfS("timer capture is waiting......!\n\n");
    while(1)
    {
        if(flag_capture_int[TIMER0])
        {
            flag_capture_int[TIMER0] = 0;
            printfS("Timer0 capture = 0x%x\n", CaptureCounter[TIMER0]);
        }
    }
}

void timer_pwm_test(void)
{
    printfS("timer pwm test is start!\n"); //仅测试TIMER0的PWM，其他类似

    pwm_set(TIMER0, 0xbb8, 0x5dc); //产生一个占空比为50%的方波

    timer_start(TIMER0); //enable timer0
    pwm_start(TIMER0);

    while(1)
    {
        ;
    }
}


void timer_test(void)
{
    timer_init();
    timer_delay_test(); //timer delay 1s test
//  timer_cc_test(); //timer capture test
//  timer_pwm_test();//timer output pwm test
}


