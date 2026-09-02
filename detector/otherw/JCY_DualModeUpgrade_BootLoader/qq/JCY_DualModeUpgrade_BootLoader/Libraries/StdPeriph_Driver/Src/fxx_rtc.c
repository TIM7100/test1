/*
  ******************************************************************************
  * @file    fxx_rtc.c
  * @version V1.0.0
  * @date    2021
  * @brief   module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Real-Time Clock (RTC) peripheral:
  *           + Initialization functions
  *           + Time and Date configuration
  *           + Alarm configuration
  *           + WakeUp Timer configuration
  *           + TimeStamp configuration
  *           + Tampers configuration
  *           + Backup Data Registers configuration
  *           + RTC Tamper and TimeStamp Pins Selection
  *           + Interrupts and flags management
  ******************************************************************************
*/
#include "fxx_std.h"


/*********************************************************************************
* Function    : RTC_Config
* Description : Initialize the RTC peripheral
* Input       : RTC_ConfigTypeDef
* Output      : None
* retval      : None
**********************************************************************************/
void RTC_Config(RTC_ConfigTypeDef *hrtc)
{
#if (USE_FULL_ASSERT == 1)
    /* Check RTC Parameter */
	assert_param(IS_RTC_CLOCKSRC(hrtc->u32_ClockSource));
	assert_param(IS_RTC_CLOCKSRC(hrtc->u32_Compensation));
#endif

    /* RTC domain write enable */
    SCU->STOPCFG |= (1 << 0);

    PMU->CR1 |= RPMU_CR_RTCEN;

    switch (hrtc->u32_ClockSource)
    {
        case RTC_CLOCK_RC32K: 
        {
            PMU->ANACR |= RPMU_ANACR_RC32K_EN;
            while(!(PMU->ANACR & RPMU_ANACR_RC32K_RDY));

            PMU->CR1 &= ~RTC_CLOCK_XTL;
        }break;

        case RTC_CLOCK_XTL: 
        {
            PMU->ANACR = (PMU->ANACR & ~RPMU_ANACR_XTLDRV) | (RPMU_ANACR_XTLDRV_1 | RPMU_ANACR_XTLDRV_0);
            
            PMU->ANACR |= RPMU_ANACR_XTLEN;
            while(!(PMU->ANACR & RPMU_ANACR_XTLRDY));

            PMU->CR1 |= RTC_CLOCK_XTL;
        }break;

        default: break; 
    }

    if (hrtc->u32_CompensationValue) 
    {
        RTC->ADJUST = hrtc->u32_Compensation | hrtc->u32_CompensationValue;
    }
}

/*********************************************************************************
* Function    : RTC_EnterConfigMode
* Input       : None
* Output      : None
* retval      : None
**********************************************************************************/
void RTC_EnterConfigMode(void)
{
	/* Set the WP  to enter in the Configuration Mode */
	RTC->WP = 0xCA53CA53;
}

/*********************************************************************************
* Function    : RTC_ExitConfigMode
* Input       : None
* Output      : None
* retval      : None
**********************************************************************************/
void RTC_ExitConfigMode(void)
{
	/* Reset the WP  to exit from the Configuration Mode */
	RTC->WP = 0;
}

/*********************************************************************************
* Function    : RTC_GetFlagStatus
* Description : Checks whether the specified RTC flag is set or not.
* Input       : RTC_FLAG: specifies the flag to check.
*      This parameter can be : RTC_SR_STP2RIE RTC_SR_STP2FIE ...
* Output      : None
* retval      : None
**********************************************************************************/
FlagStatus RTC_GetFlagStatus(uint32_t RTC_FLAG)
{
	FlagStatus bitstatus = RESET;

	if (RTC->SR & RTC_FLAG) 
	{
		bitstatus = SET;
	}
	else
	{
		bitstatus = RESET;
	}
	return bitstatus;
}

/*********************************************************************************
* Function    : RTC_ClearFlag
* Description : Checks whether the specified RTC flag is set or not.
* Input       : RTC_FLAG: specifies the flag to check.
*      This parameter can be : RTC_SR_STP2RIE RTC_SR_STP2FIE ...
* Output      : None
* retval      : None
**********************************************************************************/
void RTC_ClearFlag(uint32_t RTC_FLAG)
{ 
	/* Clear the corresponding RTC flag */
	SET_BIT(RTC->SR,RTC_FLAG);
}

/*********************************************************************************
* Function    : RTC_ClearITPendingBit
* Description :  Clears the RTC's interrupt pending bits.
* Input       : RTC_IT: specifies the flag to check.
*      This parameter can be : RTC_SR_STP2RIE RTC_SR_STP2FIE ...
* Output      : None
* retval      : None
**********************************************************************************/
void RTC_ClearITPendingBit(uint32_t RTC_IT)
{
   /* Clear the corresponding RTC flag */
	SET_BIT(RTC->SR,RTC_IT);
}

/*********************************************************************************
* Function    : RTC_SetTime
* Description : Set RTC current time.
* Input       : fp_Time Pointer to Time structure.
* Output      : None
* retval      : None
**********************************************************************************/
void RTC_SetTime(RTC_TimeTypeDef *fp_Time)
{
#if (USE_FULL_ASSERT == 1)
    /* Check RTC Parameter */
	assert_param(IS_RTC_HOUR(fp_Time->u8_Hours));
	assert_param(IS_RTC_MIN(fp_Time->u8_Minutes));
	assert_param(IS_RTC_SEC(fp_Time->u8_Seconds));
#endif
    
    /* Write-Protect Disable */
    RTC_EnterConfigMode();

    RTC->HOUR = fp_Time->u8_Hours;
    RTC->MIN  = fp_Time->u8_Minutes;
    RTC->SEC  = fp_Time->u8_Seconds;

    /* Write-Protect Enable */
    RTC_ExitConfigMode();
}

/*********************************************************************************
* Function    : RTC_GetTime
* Description : Get RTC current time.
* Input       : fp_Time Pointer to Time structure.
* Output      : None
* retval      : None
**********************************************************************************/
void RTC_GetTime(RTC_TimeTypeDef *fp_Time)
{
    fp_Time->u8_Hours   = RTC->HOUR;
    fp_Time->u8_Minutes = RTC->MIN;
    fp_Time->u8_Seconds = RTC->SEC;
}

/*********************************************************************************
* Function    : RTC_SetDate
* Description : Set RTC current Date.
* Input       : fp_Date Pointer to Date structure.
* Output      : 
* retval      : None
**********************************************************************************/
void RTC_SetDate(RTC_DateTypeDef *fp_Date)
{
#if (USE_FULL_ASSERT == 1)
    /* Check RTC Parameter */
	assert_param(IS_RTC_YEAR(fp_Date->u8_Year));
	assert_param(IS_RTC_MONTH(fp_Date->u8_Month));
	assert_param(IS_RTC_DAY(fp_Date->u8_Date));
	assert_param(IS_RTC_WEEKDAY(fp_Date->u8_WeekDay));
#endif

    /* Write-Protect Disable */
    RTC_EnterConfigMode();

    RTC->YEAR  = fp_Date->u8_Year;
    RTC->MONTH = fp_Date->u8_Month;
    RTC->DATE  = fp_Date->u8_Date;
    RTC->WEEK  = fp_Date->u8_WeekDay;

    /* Write-Protect Enable */
    RTC_ExitConfigMode();
}

/*********************************************************************************
* Function    : RTC_GetDate
* Description : Get RTC current Date.
* Input       : fp_Date Pointer to Date structure.
* Output      : None
* retval      : None
**********************************************************************************/
void RTC_GetDate(RTC_DateTypeDef *fp_Date)
{
    fp_Date->u8_Year    = RTC->YEAR;
    fp_Date->u8_Month   = RTC->MONTH;
    fp_Date->u8_Date    = RTC->DATE;
    fp_Date->u8_WeekDay = RTC->WEEK;
}

/*********************************************************************************
* Function    : RTC_SetAlarm
* Description : Alarm Config
* Input       : fp_Alarm Pointer to ALarm structure.
* Output      : None
* retval      : None
**********************************************************************************/
void RTC_SetAlarm(RTC_AlarmTypeDef *fp_Alarm)
{
    uint32_t lu32_WeekDay;
    
#if (USE_FULL_ASSERT == 1)
    /* Check RTC Parameter */
	assert_param(IS_RTC_ALARM_MODE(fp_Alarm->u32_AlarmMode));
	assert_param(IS_RTC_ALARM_INT(fp_Alarm->u32_AlarmInterrupt));
	assert_param(IS_RTC_ALARM_DAY_MASK(fp_Alarm->u32_DayMask));
	assert_param(IS_RTC_ALARM_HOUR_MASK(fp_Alarm->u32_HourMask));
	assert_param(IS_RTC_ALARM_MIN_MASK(fp_Alarm->u32_MinMask));
  
    if (fp_Alarm->u32_AlarmMode == RTC_ALARM_WEEK_MODE) 
    {
		assert_param(IS_RTC_ALARM_WEEKDAY(fp_Alarm->u32_AlarmWeek));
    }
    else 
    {
		assert_param(IS_RTC_DAY(fp_Alarm->u32_AlarmDay));
    }
    assert_param(IS_RTC_HOUR(fp_Alarm->u32_Hours));
	assert_param(IS_RTC_MIN(fp_Alarm->u32_Minutes));
	assert_param(IS_RTC_SEC(fp_Alarm->u32_Seconds));
#endif

    if (fp_Alarm->u32_AlarmMode == RTC_ALARM_WEEK_MODE) 
    {
        lu32_WeekDay = fp_Alarm->u32_AlarmWeek;
    }
    else 
    {
        lu32_WeekDay = fp_Alarm->u32_AlarmDay;
    }

    /* Coinfig Week/Day、Hour、Min、Sec */
    RTC->ALM = fp_Alarm->u32_AlarmMode | lu32_WeekDay | fp_Alarm->u32_Hours << 16 | fp_Alarm->u32_Minutes << 8 | fp_Alarm->u32_Seconds;

    /* Interrupt Enable */
    if (RTC_ALARM_INT_ENABLE == fp_Alarm->u32_AlarmInterrupt) 
    {
        RTC->IE |= RTC_IE_ALM;
    }

    RTC->CR |= (fp_Alarm->u32_DayMask) ? RTC_ALARM_DAY_MASK_ENABLE : RTC_ALARM_DAY_MASK_DISABLE;

    RTC->CR |= (fp_Alarm->u32_HourMask) ? RTC_ALARM_HOUR_MASK_ENABLE : RTC_ALARM_HOUR_MASK_DISABLE;

    RTC->CR |= (fp_Alarm->u32_MinMask) ? RTC_ALARM_MIN_MASK_ENABLE : RTC_ALARM_MIN_MASK_DISABLE;
}

/*********************************************************************************
* Function    : RTC_AlarmEnable
* Description : Alarm Enable
* Input       : None
* Output      : None
* retval      : None
**********************************************************************************/
void RTC_AlarmEnable(void)
{
    RTC->CR |= RTC_CR_ALM_EN;
}

/*********************************************************************************
* Function    : RTC_AlarmDisable
* Description : Alarm Disable
* Input       : None
* Output      : None
* retval      : None
**********************************************************************************/
void RTC_AlarmDisable(void)
{
    RTC->CR &= ~RTC_CR_ALM_EN;
}

/*********************************************************************************
* Function    : RTC_Tamper
* Description : Temper1 use PC13、Temper2 use PA0
* Input       : None
* Output      : None
* retval      : None
**********************************************************************************/
void RTC_Tamper(enum_Temper_t fe_Temper, RTC_TemperTypeDef *fp_Temper)
{
#if (USE_FULL_ASSERT == 1)
    /* Check RTC Parameter */
	assert_param(IS_RTC_TEMP_EDGE(fp_Temper->u32_TemperEdge));
	assert_param(IS_RTC_TEMP_EDGE(fp_Temper->u32_InterruptEN));
	assert_param(IS_RTC_TEMP_CLEAR_BACKUP(fp_Temper->u32_ClearBackup));
	assert_param(IS_RTC_TEMP_FILTER(fp_Temper->u32_Filter));
#endif

    switch (fe_Temper)
    {
        case RTC_TEMPER_1: 
        {
            PMU->IOCR  &= ~(1<<6);  // Configure PC13 as digital IO   
			if(fp_Temper->u32_TemperEdge ==RTC_TEMP_EDGE_FALLING) 
			{
				// Configure PC13 pull up
				PMU->IOCR |= 1<<0;   
				PMU->IOCR &=~(1<<1);
			}
			else
			{
				// Configure PC13 pull down
				PMU->IOCR &= ~(1<<0);
				PMU->IOCR |=(1<<1);	
			}
            PMU->IOSEL |=  0x02;  // Configure PC13 as tamper function   
            
            /* Clear Config */
            RTC->CR &= ~(RTC_CR_TAMP1RCLR | RTC_CR_TAMP1FCLR | RTC_CR_TAMP1FLTEN | RTC_CR_TAMP1FLT | RTC_CR_TS1EDGE | RTC_CR_TAMPFLTCLK);
            /* Edge select */
            RTC->CR |= fp_Temper->u32_TemperEdge ? RTC_CR_TS1EDGE : 0x00;
            /* Auto clear backup register */
            if (fp_Temper->u32_ClearBackup) 
            {
                RTC->CR |= fp_Temper->u32_TemperEdge ? RTC_CR_TAMP1FCLR : RTC_CR_TAMP1RCLR;
            }
            /* Temper filter */
            if (fp_Temper->u32_Filter) 
            {
                if (fp_Temper->u32_Filter == RTC_TEMP_FILTER_512_RTCCLK) 
                {
                    RTC->CR |= RTC_CR_TAMPFLTCLK;
                }
                else 
                {
                    RTC->CR |= (fp_Temper->u32_Filter - 2) << 13;
                }
            }  
            
            RTC->CR |= RTC_CR_TAMP1EN;  
            System_Delay(2000);       
            RTC->SR |= (RTC_SR_STP1FIE|RTC_SR_STP1RIE);   
            RTC->IE &= (~(RTC_IE_STP1FIE|RTC_IE_STP1RIE));      
            
            /* Put Temper Interrupt enable here !!!*/   
            if (fp_Temper->u32_InterruptEN) 
            {
                RTC->IE |= fp_Temper->u32_TemperEdge ? RTC_IE_STP1FIE : RTC_IE_STP1RIE;
            }               
 
        }break;
        
        case RTC_TEMPER_2:
        {
			SCU->PABADS &= ~(1<<0);
			if(fp_Temper->u32_TemperEdge ==RTC_TEMP_EDGE_FALLING) 
			{
				// Configure PA0 pull up
				SCU->PABPUR |=  1<<0;   
				SCU->PABPDR &=~(1<<0);
			}
			else
			{
				// Configure PA0 pull down
				SCU->PABPUR &= ~(1<<0);   
				SCU->PABPDR |= 1<<0;
			}
            /* Clear Config */
            RTC->CR &= ~(RTC_CR_TAMP2RCLR | RTC_CR_TAMP2FCLR | RTC_CR_TAMP2FLTEN | RTC_CR_TAMP2FLT | RTC_CR_TS2EDGE | RTC_CR_TAMPFLTCLK);
            /* Edge select */
            RTC->CR |= fp_Temper->u32_TemperEdge ? RTC_CR_TS2EDGE : 0x00;
            /* Auto clear backup register */
            if (fp_Temper->u32_ClearBackup) 
            {
                RTC->CR |= fp_Temper->u32_TemperEdge ? RTC_CR_TAMP2FCLR : RTC_CR_TAMP2RCLR;
            }
            /* Temper filter */
            if (fp_Temper->u32_Filter) 
            {
                if (fp_Temper->u32_Filter == RTC_TEMP_FILTER_512_RTCCLK) 
                {
                    RTC->CR |= RTC_CR_TAMPFLTCLK;
                }
                else 
                {
                    RTC->CR |= (fp_Temper->u32_Filter - 2) << 19;
                }
            }
            
            RTC->CR |= RTC_CR_TAMP2EN;  
            System_Delay(2000);     
            RTC->SR |= (RTC_SR_STP2FIE|RTC_SR_STP2RIE);   
            RTC->IE &= (~(RTC_IE_STP2FIE|RTC_IE_STP2RIE));  
            
            /* Temper Interrupt */
            if (fp_Temper->u32_InterruptEN) 
            {
                RTC->IE |= fp_Temper->u32_TemperEdge ? RTC_IE_STP2FIE : RTC_IE_STP2RIE;
            }

        }break;

        default: break; 
    }
}

/*********************************************************************************
* Function    : RTC_TamperEnable
* Description : Enable  Tamper
* Input       : fe_Temper
* Output      : None
* retval      : None
**********************************************************************************/
void RTC_TamperEnable(enum_Temper_t fe_Temper)
{
    if (fe_Temper == RTC_TEMPER_1) 
    {
        RTC->CR |= RTC_CR_TAMP1EN;
    }
    else 
    {
        RTC->CR |= RTC_CR_TAMP2EN;
    }
}

/*********************************************************************************
* Function    : RTC_TamperDisable
* Description : Disable Temper
* Input       : None
* Output      : None
* retval      : None
**********************************************************************************/
void RTC_TamperDisable(enum_Temper_t fe_Temper)
{
    if (fe_Temper == RTC_TEMPER_1) 
    {
        RTC->CR &= ~RTC_CR_TAMP1EN;
    }
    else 
    {
        RTC->CR &= ~RTC_CR_TAMP2EN;
    }
}

/***************************************************************************************************
* Function    : RTC_Standby_Wakeup
* Description : wakeup source select
* Input       : fu32_Edge 0: Rising edge
*                         1: Falling edge
*               fe_Wakeup  : wakeup source select, STANDBY_WAKEUP_RISING, STANDBY_WAKEUP_FALLING  
* Output      : None
* retval      : None
*******************************************************************************************************/
void RTC_Standby_Wakeup(enum_WKUP_t fe_Wakeup, uint32_t fu32_Edge)
{
    switch (fe_Wakeup)
    {
        case RTC_WAKEUP_WKUP1: 
        case RTC_WAKEUP_WKUP2: 
        case RTC_WAKEUP_WKUP3: 
        case RTC_WAKEUP_WKUP4: 
        case RTC_WAKEUP_WKUP5: 
        case RTC_WAKEUP_WKUP6:   
        {
            /* Clear flags、Standby Enable */
            PMU->CR1 |= RPMU_CR_STB_EN | RPMU_CR_CWUF | RPMU_CR_CSBF;

            /* Wakeup IO Filter Enable */
            PMU->CR1 |= fe_Wakeup << 8;
            /* Wakeup IO Enable */
            PMU->CR1 |= fe_Wakeup;

            if (fe_Wakeup == RTC_WAKEUP_WKUP2) 
            {
                /* PC13 */
                PMU->IOCR &= ~0x40;  // must configure PC13 as digital function     
            }
            
            if (fu32_Edge) 
            {
                PMU->CR2 |= fe_Wakeup >> 16;
            }
            else 
            {
                PMU->CR2 &= ~(fe_Wakeup >> 16);   
            }

            PMU->CR1 |= RPMU_CR_CWUF; // clear wakeup flag     
            System_Enter_Standby_Mode();              
        }break;

        case RTC_WAKEUP_STAMP2:
        case RTC_WAKEUP_STAMP1:
        case RTC_WAKEUP_32S:
        case RTC_WAKEUP_SEC:
        case RTC_WAKEUP_MIN:
        case RTC_WAKEUP_HOUR:
        case RTC_WAKEUP_DATE:     
        {
            /* Clear flags、Standby Enable */
            PMU->CR1 |= RPMU_CR_STB_EN | RPMU_CR_CWUF | RPMU_CR_CSBF;

            RTC->SR |= fe_Wakeup;
            RTC->IE |= fe_Wakeup;

            System_Enter_Standby_Mode();  
        }break;

        default: break; 
    }
}

/*********************************************************************************
* Function    : RTC_GetStandbyStatus
* Description : Check MCU have entered standby mode
* Input       : None
* Output      : 0: Not Enter Standby Mode
                1: Entered Standby Mode
* retval      : None
**********************************************************************************/
bool RTC_Get_StandbyStatus(void)
{
    if (PMU->SR & RPMU_SR_SBF) 
    {
        return true;
    }
    else 
    {
        return false;
    }
}

/*********************************************************************************
* Function    : RTC_Get_StandbyWakeupSource
* Description : Get MCU Standby Wakeup Source
* Input       : 
* Output      : RTC_WAKEUP_SOURCE_BORWUF
                RTC_WAKEUP_SOURCE_IWDTWUF
                RTC_WAKEUP_SOURCE_RSTWUF
                RTC_WAKEUP_SOURCE_RTCWUF
                RTC_WAKEUP_SOURCE_WKUP6
                RTC_WAKEUP_SOURCE_WKUP5
                RTC_WAKEUP_SOURCE_WKUP4
                RTC_WAKEUP_SOURCE_WKUP3
                RTC_WAKEUP_SOURCE_WKUP2
                RTC_WAKEUP_SOURCE_WKUP1
* retval      : None
**********************************************************************************/
uint32_t RTC_Get_StandbyWakeupSource(void)
{
    return PMU->SR;
}
