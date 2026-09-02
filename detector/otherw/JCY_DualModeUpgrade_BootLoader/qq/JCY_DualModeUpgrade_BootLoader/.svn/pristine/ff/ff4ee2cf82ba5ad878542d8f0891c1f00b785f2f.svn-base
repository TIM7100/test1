#include "bsp_timer7.h"
#include "w5500_user_conf.h"

uint8_t Tim7_Update_Flag = 0;

void Timer7_Init(void)
{
    uint32_t timer_clock;
    TIM_Base_InitTypeDef TIM_TimeBaseStructure;

    System_Module_Enable(EN_TIM7);
    timer_clock = System_Get_APBClock();

    if (System_Get_SystemClock() != System_Get_APBClock())  // if hclk/pclk != 1, then timer clk = pclk * 2
    {
        timer_clock =  System_Get_APBClock() << 1;    //
    }

    TIM_TimeBaseStructure.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    TIM_TimeBaseStructure.CounterMode = TIM_COUNTERMODE_UP;
    TIM_TimeBaseStructure.Period = 10 - 1;                          //1s / 100000    10´Î--0.1ms
    TIM_TimeBaseStructure.Prescaler = (timer_clock / 100000) - 1;   //100KHz

    TIM_TimeBase_Init(TIM7, &TIM_TimeBaseStructure);
    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);

//  NVIC_SetPriorityGrouping(4);
//  NVIC_SetPriority(TIM7_IRQn, 2);
    NVIC_ClearPendingIRQ(TIM7_IRQn);
    NVIC_EnableIRQ(TIM7_IRQn);
    Timer7_Start();
}

void Timer7_SetUs(uint32_t us)
{
    TIM7->ARR = us - 1;
    TIM7->CNT = 0;

}

void Timer7_Stop(void)
{
    TIM_Cmd(TIM7, DISABLE);
    TIM7->CNT = 0;
    TIM_ClearFlag(TIM7, TIM_IT_Update);
}

void Timer7_Start(void)
{
    Timer7_Stop();
    TIM_Cmd(TIM7, ENABLE);
}

