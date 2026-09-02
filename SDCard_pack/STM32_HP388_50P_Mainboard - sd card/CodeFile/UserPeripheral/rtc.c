#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "usart_lcd.h"
#include "user_OS.h"
#include "rtc.h"
#include "math.h"

//======================================
//1.更新日志 20201231: 改进日期算法，修复出现闰年13.01问题
//======================================


extern u32 MainCount_Hex;
volatile _calendar_obj calendar;//时钟结构体

static void RTC_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;      //RTC全局中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //先占优先级1位,从优先级3位
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //先占优先级0位,从优先级4位
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //使能该通道中断
    NVIC_Init(&NVIC_InitStructure);     //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}

//实时时钟配置
//初始化RTC时钟,同时检测时钟是否工作正常
//BKP->DR1用于保存是否第一次配置的设置
//返回0:正常
//其他:错误代码
u8 RTC_Init(void)
{
    //检查是不是第一次配置时钟
    u8 temp = 0;
	  u32 tmp_test;
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);    //使能PWR和BKP外设时钟
    PWR_BackupAccessCmd(ENABLE);    //使能后备寄存器访问
//     if (MainCount_Hex == 100)
//     {
//         RTC_Set(2020, 7, 2, 9, 50, 00); //设置时间
//         MainCount_Hex += 10;
//     }
    //if (1)      //从指定的后备寄存器中读出数据:读出了与写入的指定数据不相乎
    tmp_test = BKP_ReadBackupRegister(BKP_DR1);
    if (tmp_test != 0x5050)      //从指定的后备寄存器中读出数据:读出了与写入的指定数据不相乎
    {
        BKP_DeInit();   //复位备份区域
        RCC_LSEConfig(RCC_LSE_ON);  //设置外部低速晶振(LSE),使用外设低速晶振
        while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET && temp < 250) //检查指定的RCC标志位设置与否,等待低速晶振就绪
        {
            temp++;
            TimerDelayMS(10);
        }
        if (temp >= 250)
        {
            return 1;//初始化时钟失败,晶振有问题
        }
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);     //设置RTC时钟(RTCCLK),选择LSE作为RTC时钟
        RCC_RTCCLKCmd(ENABLE);  //使能RTC时钟
        RTC_WaitForLastTask();  //等待最近一次对RTC寄存器的写操作完成
        RTC_WaitForSynchro();       //等待RTC寄存器同步
        RTC_ITConfig(RTC_IT_SEC, ENABLE);       //使能RTC秒中断
        RTC_WaitForLastTask();  //等待最近一次对RTC寄存器的写操作完成
        RTC_EnterConfigMode();/// 允许配置
        RTC_SetPrescaler(32767); //设置RTC预分频的值
        RTC_WaitForLastTask();  //等待最近一次对RTC寄存器的写操作完成
        if (MainCount_Hex == 0xffffffff)
        {
            RTC_Set(2019, 6, 19, 14, 24, 55); //设置时间
            MainCount_Hex++;
        }
        RTC_ExitConfigMode(); //退出配置模式
        BKP_WriteBackupRegister(BKP_DR1, 0X5050);   //向指定的后备寄存器中写入用户程序数据
    }
    else//系统继续计时
    {
        RTC_WaitForSynchro();   //等待最近一次对RTC寄存器的写操作完成
        RTC_ITConfig(RTC_IT_SEC, ENABLE);   //使能RTC秒中断
        RTC_WaitForLastTask();  //等待最近一次对RTC寄存器的写操作完成
    }
    RTC_NVIC_Config();//RCT中断分组设置
		if (MainCount_Hex == 100)
    {
        RTC_Set(2021, 2, 29, 0, 00, 00); //设置时间
        MainCount_Hex += 10;
    }
    RTC_Get();//更新时间
    return 0; //ok

}
//RTC时钟中断
//每秒触发一次
//extern u16 tcnt;
void RTC_IRQHandler(void)
{
    if (RTC_GetITStatus(RTC_IT_SEC) != RESET)//秒钟中断
    {
        RTC_Get();//更新时间
    }
    if (RTC_GetITStatus(RTC_IT_ALR) != RESET) //闹钟中断
    {
        RTC_ClearITPendingBit(RTC_IT_ALR);      //清闹钟中断
        RTC_Get();                //更新时间
    }
    RTC_ClearITPendingBit(RTC_IT_SEC | RTC_IT_OW);      //清闹钟中断
    RTC_WaitForLastTask();
}
//判断是否是闰年函数
//月份   1  2  3  4  5  6  7  8  9  10 11 12
//闰年   31 29 31 30 31 30 31 31 30 31 30 31
//非闰年 31 28 31 30 31 30 31 31 30 31 30 31
//输入:年份
//输出:该年份是不是闰年.1,是.0,不是
u8 Is_Leap_Year(u16 year)
{
    if (year % 4 == 0) //必须能被4整除
    {
        if (year % 100 == 0)
        {
            if (year % 400 == 0)
            {
                return 1;    //如果以00结尾,还要能被400整除
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 0;
    }
}
//设置时钟
//把输入的时钟转换为秒钟
//以1970年1月1日为基准
//1970~2099年为合法年份
//返回值:0,成功;其他:错误代码.
//月份数据表
u8 const table_week[12] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5}; //月修正数据表
//平年的月份日期表
const u8 mon_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
u8 RTC_Set(u16 syear, u8 smon, u8 sday, u8 hour, u8 min, u8 sec)
{
    u16 t;
    u32 seccount = 0;
    if (syear < 1970 || syear > 2099)
    {
        return 1;
    }
    for (t = 1970; t < syear; t++) //把所有年份的秒钟相加
    {
        if (Is_Leap_Year(t))
        {
            seccount += 31622400;    //闰年的秒钟数
        }
        else
        {
            seccount += 31536000;    //平年的秒钟数
        }
    }
    smon -= 1;
    for (t = 0; t < smon; t++) //把前面月份的秒钟数相加
    {
        seccount += (u32)mon_table[t] * 86400; //月份秒钟数相加
        if (Is_Leap_Year(syear) && t == 1)
        {
            seccount += 86400;    //闰年2月份增加一天的秒钟数
        }
    }
    seccount += (u32)(sday - 1) * 86400; //把前面日期的秒钟数相加
    seccount += (u32)hour * 3600; //小时秒钟数
    seccount += (u32)min * 60; //分钟秒钟数
    seccount += sec; //最后的秒钟加上去

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);    //使能PWR和BKP外设时钟
    PWR_BackupAccessCmd(ENABLE);    //使能RTC和后备寄存器访问
    RTC_SetCounter(seccount);   //设置RTC计数器的值

    RTC_WaitForLastTask();  //等待最近一次对RTC寄存器的写操作完成
    return 0;
}

//初始化闹钟
//以1970年1月1日为基准
//1970~2099年为合法年份
//syear,smon,sday,hour,min,sec：闹钟的年月日时分秒
//返回值:0,成功;其他:错误代码.
u8 RTC_Alarm_Set(u16 syear, u8 smon, u8 sday, u8 hour, u8 min, u8 sec)
{
    u16 t;
    u32 seccount = 0;
    if (syear < 1970 || syear > 2099)
    {
        return 1;
    }
    for (t = 1970; t < syear; t++) //把所有年份的秒钟相加
    {
        if (Is_Leap_Year(t))
        {
            seccount += 31622400;    //闰年的秒钟数
        }
        else
        {
            seccount += 31536000;    //平年的秒钟数
        }
    }
    smon -= 1;
    for (t = 0; t < smon; t++) //把前面月份的秒钟数相加
    {
        seccount += (u32)mon_table[t] * 86400; //月份秒钟数相加
        if (Is_Leap_Year(syear) && t == 1)
        {
            seccount += 86400;    //闰年2月份增加一天的秒钟数
        }
    }
    seccount += (u32)(sday - 1) * 86400; //把前面日期的秒钟数相加
    seccount += (u32)hour * 3600; //小时秒钟数
    seccount += (u32)min * 60; //分钟秒钟数
    seccount += sec; //最后的秒钟加上去
    //设置时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);    //使能PWR和BKP外设时钟
    PWR_BackupAccessCmd(ENABLE);    //使能后备寄存器访问
    //上面三步是必须的!

    RTC_SetAlarm(seccount);

    RTC_WaitForLastTask();  //等待最近一次对RTC寄存器的写操作完成

    return 0;
}


//得到当前的时间
//返回值:0,成功;其他:错误代码.
u8 RTC_Get(void)
{
    static u16 daycnt = 0;
    u32 timecount = 0;
    u32 temp = 0;
	  u16 CurrentYear = 0;
    u16 CurrentMonth = 0;
	  u16 CurrentMonDays;
	  u16 RemainDays;
    
    timecount = RTC_GetCounter();
    temp = timecount / 86400; //得到天数(秒钟数对应的)
    if (daycnt != temp) //超过一天了
    {
        daycnt = temp;
        CurrentYear = 1970; //从1970年开始
        while (temp >= 365)
        {
            if (Is_Leap_Year(CurrentYear)) //是闰年
            {
                if (temp >= 366)
                {
                    temp -= 366;    //闰年的秒钟数
                }
                else
                {
                    //temp1++;
                    break;
                }
            }
            else
            {
                temp -= 365;    //平年
            }
            CurrentYear++;
        }
        calendar.w_year = CurrentYear;          //得到年份
				
        CurrentMonth = 1;                       //当前月份
				CurrentMonDays = mon_table[0];          //1月份总日期数
				RemainDays = temp;                      //算完年份之后的剩余日子数
				
        while (RemainDays >= CurrentMonDays)     //超过了一个月
        {
					  RemainDays -= CurrentMonDays;
            CurrentMonth++;                     //月份+1
					
						CurrentMonDays = mon_table[CurrentMonth - 1];  //下一月份总日期数
				    if ((CurrentMonth == 2) && Is_Leap_Year(calendar.w_year))
				    {
				    	CurrentMonDays++;    //闰年2月份，则当前月份天数加1 (即29)
				    }
        }
        calendar.w_month = CurrentMonth;            //得到月份
        calendar.w_date =  RemainDays + 1;          //得到日期
				//--------------------
//         temp1 = 0;
//         while (temp >= 28) //超过了一个月
//         {
//             if (Is_Leap_Year(calendar.w_year) && temp1 == 1) //当年是不是闰年/2月份
//             {
//                 if (temp >= 29)
//                 {
//                     temp -= 29;    //闰年的秒钟数
//                 }
//                 else
//                 {
//                     break;
//                 }
//             }
//             else
//             {
//                 if (temp >= mon_table[temp1])
//                 {
//                     temp -= mon_table[temp1];    //平年
//                 }
//                 else
//                 {
//                     break;
//                 }
//             }
//             temp1++;
//         }
//         calendar.w_month = temp1 + 1; //得到月份
//         calendar.w_date = temp + 1; //得到日期
				
				//--------------------------
    }
    temp = timecount % 86400;       //得到秒钟数
    calendar.hour = temp / 3600;    //小时
    calendar.min = (temp % 3600) / 60; //分钟
    calendar.sec = (temp % 3600) % 60; //秒钟
    calendar.week = RTC_Get_Week(calendar.w_year, calendar.w_month, calendar.w_date); //获取星期

    return 0;
}
//获得现在是星期几
//功能描述:输入公历日期得到星期(只允许1901-2099年)
//输入参数：公历年月日
//返回值：星期号
u8 RTC_Get_Week(u16 year, u8 month, u8 day)
{
    u16 temp2;
    u8 yearH, yearL;

    yearH = year / 100;
    yearL = year % 100;
    // 如果为21世纪,年份数加100
    if (yearH > 19)
    {
        yearL += 100;
    }
    // 所过闰年数只算1900年之后的
    temp2 = yearL + yearL / 4;
    temp2 = temp2 % 7;
    temp2 = temp2 + day + table_week[month - 1];
    if (yearL % 4 == 0 && month < 3)
    {
        temp2--;
    }
    return (temp2 % 7);
}

void Tran_Time_To_Array(u8* time_arr)
{
    u32 tmp;

    tmp = calendar.w_year;

    time_arr[0] = tmp / 1000  + 0x30;
    tmp = calendar.w_year % 1000;
    time_arr[1] = tmp / 100  + 0x30;
    tmp = calendar.w_year % 100;
    time_arr[2] = tmp / 10  + 0x30;
    tmp = calendar.w_year % 10;
    time_arr[3] = tmp  + 0x30;
    time_arr[4] = '-';

    tmp = calendar.w_month;
    tmp = calendar.w_month % 100;
    time_arr[5] = tmp / 10  + 0x30;
    tmp = calendar.w_month % 10;
    time_arr[6] = tmp  + 0x30;
    time_arr[7] = '-';

    tmp = calendar.w_date;
    tmp = calendar.w_date % 100;
    time_arr[8] = tmp / 10  + 0x30;
    tmp = calendar.w_date % 10;
    time_arr[9] = tmp  + 0x30;
    time_arr[10] = ' ';

    tmp = calendar.hour;
    tmp = calendar.hour % 100;
    time_arr[11] = tmp / 10  + 0x30;
    tmp = calendar.hour % 10;
    time_arr[12] = tmp  + 0x30;
    time_arr[13] = ':';

    tmp = calendar.min;
    tmp = calendar.min % 100;
    time_arr[14] = tmp / 10  + 0x30;
    tmp = calendar.min % 10;
    time_arr[15] = tmp  + 0x30;
    time_arr[16] = ':';

    tmp = calendar.sec;
    tmp = calendar.sec % 100;
    time_arr[17] = tmp / 10  + 0x30;
    tmp = calendar.sec % 10;
    time_arr[18] = tmp  + 0x30;
    time_arr[19] = 0;

}
void RTC_NOW_time()
{
    u32 tmp;
    u8 time_arr[20];

    tmp = calendar.w_year;

    time_arr[0] = tmp / 1000  + 0x30;
    tmp = calendar.w_year % 1000;
    time_arr[1] = tmp / 100  + 0x30;
    tmp = calendar.w_year % 100;
    time_arr[2] = tmp / 10  + 0x30;
    tmp = calendar.w_year % 10;
    time_arr[3] = tmp  + 0x30;
    time_arr[4] = '-';

    tmp = calendar.w_month;
    tmp = calendar.w_month % 100;
    time_arr[5] = tmp / 10  + 0x30;
    tmp = calendar.w_month % 10;
    time_arr[6] = tmp  + 0x30;
    time_arr[7] = '-';

    tmp = calendar.w_date;
    tmp = calendar.w_date % 100;
    time_arr[8] = tmp / 10  + 0x30;
    tmp = calendar.w_date % 10;
    time_arr[9] = tmp  + 0x30;
    time_arr[10] = ' ';

    tmp = calendar.hour;
    tmp = calendar.hour % 100;
    time_arr[11] = tmp / 10  + 0x30;
    tmp = calendar.hour % 10;
    time_arr[12] = tmp  + 0x30;
    time_arr[13] = ':';

    tmp = calendar.min;
    tmp = calendar.min % 100;
    time_arr[14] = tmp / 10  + 0x30;
    tmp = calendar.min % 10;
    time_arr[15] = tmp  + 0x30;
    time_arr[16] = ':';

    tmp = calendar.sec;
    tmp = calendar.sec % 100;
    time_arr[17] = tmp / 10  + 0x30;
    tmp = calendar.sec % 10;
    time_arr[18] = tmp  + 0x30;
    time_arr[19] = 0;

    TimerDelayMS(200);
    UartSend("CLR(0);\r\n"); //LCD 清屏
    CheckBusy();

    UartSend(T_DC32(80, 150,  time_arr, 4));
    CheckBusy();
}

void RTC_change_show_time(u16 year, u8 month, u8 day, u8 hour, u8 min, u8 sec, u8 cnt)
{
    u32 tmp;
    u8 time_arr[20];
    u8 time_setting[2];

    tmp = year;

    time_arr[0] = tmp / 1000  + 0x30;
    tmp = year % 1000;
    time_arr[1] = tmp / 100  + 0x30;
    tmp = year % 100;
    time_arr[2] = tmp / 10  + 0x30;
    tmp = year % 10;
    time_arr[3] = tmp  + 0x30;
    time_arr[4] = '-';

    tmp = month;
    tmp = month % 100;
    time_arr[5] = tmp / 10  + 0x30;
    tmp = month % 10;
    time_arr[6] = tmp  + 0x30;
    time_arr[7] = '-';

    tmp = day;
    tmp = day % 100;
    time_arr[8] = tmp / 10  + 0x30;
    tmp = day % 10;
    time_arr[9] = tmp  + 0x30;
    time_arr[10] = ' ';

    tmp = hour;
    tmp = hour % 100;
    time_arr[11] = tmp / 10  + 0x30;
    tmp = hour % 10;
    time_arr[12] = tmp  + 0x30;
    time_arr[13] = ':';

    tmp = min;
    tmp = min % 100;
    time_arr[14] = tmp / 10  + 0x30;
    tmp = min % 10;
    time_arr[15] = tmp  + 0x30;
    time_arr[16] = ':';

    tmp = sec;
    tmp = sec % 100;
    time_arr[17] = tmp / 10  + 0x30;
    tmp = sec % 10;
    time_arr[18] = tmp  + 0x30;
    time_arr[19] = 0;

    TimerDelayMS(100);
    UartSend("CLR(0);\r\n");
    CheckBusy();
		
    UartSend(T_DC32(140, 50,  "更改RTC时间", 18));
    CheckBusy();

    UartSend(T_DC32(80, 160,  time_arr, 4));
    CheckBusy();
    if (cnt < 19)
    {
        time_setting[0] = time_arr[cnt];
        time_setting[1] = 0;
        UartSend(T_DC32(80 + (16 * cnt), 160,  time_setting, 42));
        CheckBusy();
    }


    UartSend(T_DC16(20, 280,  "Esc <-", 19));
    CheckBusy();
    UartSend(T_DC16(135, 280,  "Enter ->", 7));
    CheckBusy();
    UartSend(T_DC16(290, 280, "Up +", 19));
    CheckBusy();
    UartSend(T_DC16(370, 280, "Down -", 7));
    CheckBusy();
}

//void time_change(void)
//{
//    int i = 0;
//    int k = 0;
//    u8 time_arry[20];
//    int key_data = 0;

//    u16 year;
//    u8 month;
//    u8 day;
//    u8 hour;
//    u8 min;
//    u8 sec;
//    u8 change_buf;

//    while (1)
//    {
//        key_data = scanKey();

//        if (key_data == Key_Enter)
//        {
//            year = calendar.w_year;
//            month = calendar.w_month;
//            day = calendar.w_date;
//            hour = calendar.hour;
//            min = calendar.min;
//            sec = calendar.sec;

//            for (k = 0; k < 19; k++)
//            {
//                time_arry[k] = ' ';
//            }

//            time_arry[0] = '-';
//            RTC_change_show_time(year, month, day, hour, min, sec);
//            UartSend(T_DC32(50, 170,  time_arry, 5));
//            CheckBusy();
//            break;
//        }
//        else if (key_data == Key_Esc)        /* 返回按键 */
//        {
//            return;
//        }
//    }
//    while (1)
//    {
//        key_data = scanKey();


//        if (key_data == Key_Enter)
//        {
//            i++;
//            if (i == 4)
//            {
//                i = 5;
//            }
//            else if (i == 7)
//            {
//                i = 8;
//            }
//            else if (i == 10)
//            {
//                i = 11;
//            }
//            else if (i == 13)
//            {
//                i = 14;
//            }
//            else if (i == 16)
//            {
//                i = 17;
//            }
//            else if (i >= 19)
//            {
//                calendar.w_year = year;
//                calendar.w_month = month;
//                calendar.w_date = day;
//                calendar.hour = hour;
//                calendar.min = min;
//                calendar.sec = sec;
//                RTC_Set(calendar.w_year, calendar.w_month, calendar.w_date, calendar.hour, calendar.min, 0);
//                return;
//            }
//        }
//        else if (key_data == Key_Esc)        /* 返回按键 */
//        {
//            if (i == 0)
//            {
//                return;
//            }
//            i--;
//            if (i == 16)
//            {
//                i = 15;
//            }
//            else if (i == 13)
//            {
//                i = 12;
//            }
//            else if (i == 10)
//            {
//                i = 9;
//            }
//            else if (i == 7)
//            {
//                i = 6;
//            }
//            else if (i == 4)
//            {
//                i = 3;
//            }

//            for (k = 0; k < 19; k++)
//            {
//                time_arry[k] = ' ';
//            }
//            time_arry[i] = '-';
//            RTC_change_show_time(year, month, day, hour, min, sec);

//            UartSend(T_DC32(50, 170,  time_arry, 5));
//            CheckBusy();
//        }

//        else if ((key_data == Key_Up) || (key_data == Key_Down))
//        {
//            switch (i)
//            {
//            case 0:
//            case 1:
//            case 2:
//            case 3:
//            {
//                change_buf = (year % ((u16)pow(10, 4 - i))) / (u16)pow(10, 3 - i);
//                year -= change_buf * pow(10, 3 - i);
//                if ((key_data == Key_Up))
//                {
//                    change_buf += 1;
//                }
//                else if (key_data == Key_Down)
//                {
//                    change_buf += 9;
//                }

//                year += (change_buf % 10) * pow(10, 3 - i);
//                break;
//            }

//            case 5:
//            case 6:
//            {
//                change_buf = (month % ((u16)pow(10, 7 - i))) / (u16)pow(10, 6 - i);
//                month -= change_buf * pow(10, 6 - i);
//                if ((key_data == Key_Up))
//                {
//                    change_buf += 1;
//                }
//                else if (key_data == Key_Down)
//                {
//                    change_buf += 9;
//                }

//                month += (change_buf % 10) * pow(10, 6 - i);
//                break;
//            }

//            case 8:
//            case 9:
//            {
//                change_buf = (day % ((u16)pow(10, 10 - i))) / (u16)pow(10, 9 - i);
//                day -= change_buf * pow(10, 9 - i);
//                if ((key_data == Key_Up))
//                {
//                    change_buf += 1;
//                }
//                else if (key_data == Key_Down)
//                {
//                    change_buf += 9;
//                }

//                day += (change_buf % 10) * pow(10, 9 - i);
//                break;
//            }

//            case 11:
//            case 12:
//            {
//                change_buf = (hour % ((u16)pow(10, 13 - i))) / (u16)pow(10, 12 - i);
//                hour -= change_buf * pow(10, 12 - i);
//                if ((key_data == Key_Up))
//                {
//                    change_buf += 1;
//                }
//                else if (key_data == Key_Down)
//                {
//                    change_buf += 9;
//                }

//                hour += (change_buf % 10) * pow(10, 12 - i);
//                break;
//            }

//            case 14:
//            case 15:
//            {
//                change_buf = (min % ((u16)pow(10, 16 - i))) / (u16)pow(10, 15 - i);
//                min -= change_buf * pow(10, 15 - i);
//                if ((key_data == Key_Up))
//                {
//                    change_buf += 1;
//                }
//                else if (key_data == Key_Down)
//                {
//                    change_buf += 9;
//                }

//                min += (change_buf % 10) * pow(10, 15 - i);
//                break;
//            }

//            case 17:
//            case 18:
//            {
//                change_buf = (sec % ((u16)pow(10, 19 - i))) / (u16)pow(10, 18 - i);
//                sec -= change_buf * pow(10, 18 - i);
//                if ((key_data == Key_Up))
//                {
//                    change_buf += 1;
//                }
//                else if (key_data == Key_Down)
//                {
//                    change_buf += 9;
//                }

//                sec += (change_buf % 10) * pow(10, 18 - i);
//                break;
//            }
//            }
//        }

//        for (k = 0; k < 19; k++)
//        {
//            time_arry[k] = ' ';
//        }
//        time_arry[i] = '-';
//        RTC_change_show_time(year, month, day, hour, min, sec);

//        UartSend(T_DC32(50, 170,  time_arry, 5));
//        CheckBusy();

//    }
//}








