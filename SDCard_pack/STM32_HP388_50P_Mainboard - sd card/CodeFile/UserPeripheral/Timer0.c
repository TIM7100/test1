#include "Timer0.h"
//#if SYSTEM_SUPPORT_OS
//#include "includes.h"                 //ucos Ê¹ÓÃ
//#endif


static u8  fac_us = 0;                          //usÑÓÊ±±¶³ËÊý
static u16 fac_ms = 0;                          //msÑÓÊ±±¶³ËÊý,ÔÚucosÏÂ,´ú±íÃ¿¸ö½ÚÅÄµÄmsÊý

//³õÊ¼»¯ÑÓ³Ùº¯Êý
//µ±Ê¹ÓÃOSµÄÊ±ºò,´Ëº¯Êý»á³õÊ¼»¯OSµÄÊ±ÖÓ½ÚÅÄ
//SYSTICKµÄÊ±ÖÓ¹Ì¶¨ÎªHCLKÊ±ÖÓµÄ1/8
//SYSCLK:ÏµÍ³Ê±ÖÓ
void delay_init()
{
#if SYSTEM_SUPPORT_OS                           //Èç¹ûÐèÒªÖ§³ÖOS.
    u32 reload;
#endif
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);   //Ñ¡ÔñÍâ²¿Ê±ÖÓ  HCLK/8
    fac_us = SystemCoreClock / 8000000;         //ÎªÏµÍ³Ê±ÖÓµÄ1/8
#if SYSTEM_SUPPORT_OS                           //Èç¹ûÐèÒªÖ§³ÖOS.
    reload = SystemCoreClock / 8000000;         //Ã¿ÃëÖÓµÄ¼ÆÊý´ÎÊý µ¥Î»ÎªK
    reload *= 1000000 / delay_ostickspersec;    //¸ù¾Ýdelay_ostickspersecÉè¶¨Òç³öÊ±¼ä
    //reloadÎª24Î»¼Ä´æÆ÷,×î´óÖµ:16777216,ÔÚ72MÏÂ,Ô¼ºÏ1.86s×óÓÒ
    fac_ms = 1000 / delay_ostickspersec;        //´ú±íOS¿ÉÒÔÑÓÊ±µÄ×îÉÙµ¥Î»

    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;  //¿ªÆôSYSTICKÖÐ¶Ï
    SysTick->LOAD = reload;                     //Ã¿1/delay_ostickspersecÃëÖÐ¶ÏÒ»´Î
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;   //¿ªÆôSYSTICK

#else
    fac_ms = (u16)fac_us * 1000;                //·ÇOSÏÂ,´ú±íÃ¿¸ömsÐèÒªµÄsystickÊ±ÖÓÊý
#endif
}

#if SYSTEM_SUPPORT_OS                           //Èç¹ûÐèÒªÖ§³ÖOS.
//ÑÓÊ±nus
//nusÎªÒªÑÓÊ±µÄusÊý.
void delay_us(u32 nus)
{
    u32 ticks;
    u32 told, tnow, tcnt = 0;
    u32 reload = SysTick->LOAD;                 //LOADµÄÖµ
    ticks = nus * fac_us;                       //ÐèÒªµÄ½ÚÅÄÊý
    tcnt = 0;
    delay_osschedlock();                        //×èÖ¹OSµ÷¶È£¬·ÀÖ¹´ò¶ÏusÑÓÊ±
    told = SysTick->VAL;                        //¸Õ½øÈëÊ±µÄ¼ÆÊýÆ÷Öµ
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;    //ÕâÀï×¢ÒâÒ»ÏÂSYSTICKÊÇÒ»¸öµÝ¼õµÄ¼ÆÊýÆ÷¾Í¿ÉÒÔÁË.
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break;    //Ê±¼ä³¬¹ý/µÈÓÚÒªÑÓ³ÙµÄÊ±¼ä,ÔòÍË³ö.
            }
        }
    };
    delay_osschedunlock();                      //»Ö¸´OSµ÷¶È
}
//ÑÓÊ±nms
//nms:ÒªÑÓÊ±µÄmsÊý
void delay_ms(u16 nms)
{
    if (delay_osrunning && delay_osintnesting == 0) //Èç¹ûOSÒÑ¾­ÔÚÅÜÁË,²¢ÇÒ²»ÊÇÔÚÖÐ¶ÏÀïÃæ(ÖÐ¶ÏÀïÃæ²»ÄÜÈÎÎñµ÷¶È)
    {
        if (nms >= fac_ms)                      //ÑÓÊ±µÄÊ±¼ä´óÓÚOSµÄ×îÉÙÊ±¼äÖÜÆÚ
        {
            delay_ostimedly(nms / fac_ms);      //OSÑÓÊ±
        }
        nms %= fac_ms;                          //OSÒÑ¾­ÎÞ·¨Ìá¹©ÕâÃ´Ð¡µÄÑÓÊ±ÁË,²ÉÓÃÆÕÍ¨·½Ê½ÑÓÊ±
    }
    delay_us((u32)(nms * 1000));                //ÆÕÍ¨·½Ê½ÑÓÊ±
}
#else //²»ÓÃOSÊ±
//ÑÓÊ±nus
//nusÎªÒªÑÓÊ±µÄusÊý.
void delay_us(u32 nus)
{
    u32 temp;
    SysTick->LOAD = nus * fac_us;               //Ê±¼ä¼ÓÔØ
    SysTick->VAL = 0x00;                        //Çå¿Õ¼ÆÊýÆ÷
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;  //¿ªÊ¼µ¹Êý
    do
    {
        temp = SysTick->CTRL;
    }
    while ((temp & 0x01) && !(temp & (1 << 16))); //µÈ´ýÊ±¼äµ½´ï
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;  //¹Ø±Õ¼ÆÊýÆ÷
    SysTick->VAL = 0X00;                         //Çå¿Õ¼ÆÊýÆ÷
}
//ÑÓÊ±nms
//×¢ÒânmsµÄ·¶Î§
//SysTick->LOADÎª24Î»¼Ä´æÆ÷,ËùÒÔ,×î´óÑÓÊ±Îª:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLKµ¥Î»ÎªHz,nmsµ¥Î»Îªms
//¶Ô72MÌõ¼þÏÂ,nms<=1864
void delay_ms(u16 nms)
{
    u32 temp;
    SysTick->LOAD = (u32)nms * fac_ms;          //Ê±¼ä¼ÓÔØ(SysTick->LOADÎª24bit)
    SysTick->VAL = 0x00;                        //Çå¿Õ¼ÆÊýÆ÷
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;  //¿ªÊ¼µ¹Êý
    do
    {
        temp = SysTick->CTRL;
    }
    while ((temp & 0x01) && !(temp & (1 << 16))); //µÈ´ýÊ±¼äµ½´ï
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;  //¹Ø±Õ¼ÆÊýÆ÷
    SysTick->VAL = 0X00;                        //Çå¿Õ¼ÆÊýÆ÷
}
#endif


void InitDelayTimer(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    TIM_TimeBaseStructure.TIM_Prescaler = 71;

    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down; /* ??TIM???g?ì????????? */

    TIM_TimeBaseStructure.TIM_Period = 1; /* ???? */

    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; /* ??TIM?????TIM???? */

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); /* ?'????? */

    TIM_Cmd(TIM2, ENABLE); /* 'S??? */
	
#if SYSTEM_SUPPORT_OS                           //Èç¹ûÐèÒªÖ§³ÖOS.
    u32 reload;
#endif
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);   //Ñ¡ÔñÍâ²¿Ê±ÖÓ  HCLK/8
    fac_us = SystemCoreClock / 8000000;         //ÎªÏµÍ³Ê±ÖÓµÄ1/8
#if SYSTEM_SUPPORT_OS                           //Èç¹ûÐèÒªÖ§³ÖOS.
    reload = SystemCoreClock / 8000000;         //Ã¿ÃëÖÓµÄ¼ÆÊý´ÎÊý µ¥Î»ÎªK
    reload *= 1000000 / delay_ostickspersec;    //¸ù¾Ýdelay_ostickspersecÉè¶¨Òç³öÊ±¼ä
    //reloadÎª24Î»¼Ä´æÆ÷,×î´óÖµ:16777216,ÔÚ72MÏÂ,Ô¼ºÏ1.86s×óÓÒ
    fac_ms = 1000 / delay_ostickspersec;        //´ú±íOS¿ÉÒÔÑÓÊ±µÄ×îÉÙµ¥Î»

    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;  //¿ªÆôSYSTICKÖÐ¶Ï
    SysTick->LOAD = reload;                     //Ã¿1/delay_ostickspersecÃëÖÐ¶ÏÒ»´Î
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;   //¿ªÆôSYSTICK

#else
    fac_ms = (u16)fac_us * 1000;                //·ÇOSÏÂ,´ú±íÃ¿¸ömsÐèÒªµÄsystickÊ±ÖÓÊý
#endif
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     TimerDelayUS                                                                              */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               nCount : ???us?? ???65535                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               ??nCount us                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
void TimerDelayUS(u16 nCount)
{
    TIM_SetCounter(TIM2, nCount);       /* ????? */
    while (TIM_GetCounter(TIM2) != 0);  /* ?????,????????0 */
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     TimerDelayMS                                                                              */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               nCount : ???ms??                                                                     */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               ??nCount ms                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
void TimerDelayMS(u32 nCount)
{
    u32 i;
    for (i = 0; i < nCount; i++)
    {
        TimerDelayUS(1000);
    }
}



































