#include "rtc.h"

//月份数据表
uint8_t const table_week[12] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5}; //月修正数据表
//平年的月份日期表
const uint8_t mon_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

u8 Time_Set[8] = {0x20, 0x26, 0x01, 0x08, 0x20, 0x24, 0x30, 0x02};

 _calendar_obj calendar;					 //日历结构体

/*函数名: Ds1302Init                              */
/*函数功能: DS1302模块的初始化函数                   */
/*返回值: 无                                         */
/*参数  : 无                                         */
void Ds1302Init(void)
{
	REG_SCU_MUXCTRLC &= ~(0xF << 22);		//GPIO43.44作GPIO使用
	REG_GPIO_DIR(GPIOB) |= (0x03 << 11);	//GPIO43.44输出
	REG_SCU_MUXCTRLC &= ~(0x3 << 0);		//GPIO32作GPIO使用
	REG_GPIO_DIR(GPIOB) |= (0x01 << 0);		//GPIO32输出

	REG_GPIO_CLR(GPIOB) |= (1 << 11);			//GPIO43 SCLK 0
	REG_GPIO_SET(GPIOB) |= (1 << 12);			//GPIO44 DATA 1
	REG_GPIO_CLR(GPIOB) |= (1 << 0);			//GPIO32 RST  0

// 	Ds1302GetTime(&calendar);
}

/*函数名: Ds1302WriteByte                            */
/*函数功能: 向DS1302写入一个字节数据                 */
/*返回值: 无                                         */
/*参数  :                                            */
/*addr     输入参数,DS1302的时钟数据地址      		 */
/*data     输入参数,待写入的一个时钟字节数据      	 */
void Ds1302WriteByte(u8 addr, u8 data)
{
	u8 i;
	
	DS1302_RST_0;					//停止DS1302总线
	delay(10);
	DS1302_RST_1;					//启动DS1302总线
	addr = addr & 0xFE;				//最低位置零，写数据
	DS1302_IO_OUT();
	delay(10);
	
	for (i = 0; i < 8; i ++)		//写地址
	{
		if (addr & 0x01)
		{
			DS1302_DATA_OUT_1;
		}
		else
		{
			DS1302_DATA_OUT_0;
		}

		DS1302_SCK_1;				//产生时钟
		delay(10);
		DS1302_SCK_0;
		delay(10);
		addr = addr >> 1;
	}
	
	for (i = 0; i < 8; i ++)		//写数据
	{
		if (data & 0x01)
		{
			DS1302_DATA_OUT_1;
		}
		else
		{
			DS1302_DATA_OUT_0;
		}

		DS1302_SCK_1;				//产生时钟
		delay(10);
		DS1302_SCK_0;
		delay(10);
		data = data >> 1;
	}
	
	DS1302_RST_0;					//停止DS1302总线
}

/*函数名: Ds1302ReadByte                             */
/*函数功能: 从DS1302读出一个字节数据                 */
/*返回值:                                            */
/*temp     返回值,读取到的一个字节数据		      	 */
/*参数  :                                            */
/*addr     输入参数,DS1302的时钟数据地址      		 */
u8 Ds1302ReadByte(u8 addr)
{
	u8 i, temp = 0;
	
	DS1302_RST_0;					//停止DS1302总线
	delay(10);
	DS1302_RST_1;					//启动DS1302总线
	addr = addr | 0x01;				//最低位置高，读数据
	DS1302_IO_OUT();
	delay(10);
	
	for (i = 0; i < 8; i ++)		//写地址
	{
		if (addr & 0x01)
		{
			DS1302_DATA_OUT_1;
		}
		else
		{
			DS1302_DATA_OUT_0;
		}

		DS1302_SCK_1;				//产生时钟
		delay(10);
		DS1302_SCK_0;
		delay(10);
		addr = addr >> 1;
	}
	
	DS1302_IO_IN();
	delay(10);
	
	for (i = 0; i < 8; i ++)		//读数据
	{
		temp = temp >> 1;
		if (DS1302_DATA_IN)
		{
			temp |= 0x80;
		}
		else
		{
			temp &= 0x7F;
		}

		DS1302_SCK_1;				//产生时钟
		delay(10);
		DS1302_SCK_0;
		delay(10);
	}
	
	DS1302_RST_0;					 //停止DS1302总线
	return temp;
}

/*函数名: Ds1302WriteTime                             */
/*函数功能: 向DS1302写入时间数据		              */
/*返回值:                                             */
/*参数  :                                             */
/**SetBuf     输入参数,待写入的时钟数组指针	     	  */
void Ds1302WriteTime(u8 *SetBuf)
{
	Ds1302WriteByte(ds1302_control_add, 0x00);      //关闭写保护
	Ds1302WriteByte(ds1302_sec_add, 0x80);          //暂停时钟
	//Ds1302WriteByte(ds1302_charger_add, 0xA9);    //涓流充电
	Ds1302WriteByte(ds1302_year_add, SetBuf[1]);    //年
	Ds1302WriteByte(ds1302_month_add, SetBuf[2]);   //月
	Ds1302WriteByte(ds1302_date_add, SetBuf[3]);    //日
	Ds1302WriteByte(ds1302_hr_add, SetBuf[4]);      //时
	Ds1302WriteByte(ds1302_min_add, SetBuf[5]);     //分
	Ds1302WriteByte(ds1302_sec_add, SetBuf[6]);     //秒
	Ds1302WriteByte(ds1302_day_add, SetBuf[7]);     //周
	Ds1302WriteByte(ds1302_control_add, 0x80);      //打开写保护
}

/*函数名: Ds1302ReadTime                             */
/*函数功能: 从DS302读出时钟数据		                 */
/*返回值:                                            */
/*参数  :                                            */
/**time_buf     输入参数,存放时钟数据的数组指针      */
void Ds1302ReadTime(u8 *time_buf)
{
	time_buf[1] = Ds1302ReadByte(ds1302_year_add);           	//年
	time_buf[2] = Ds1302ReadByte(ds1302_month_add);          	//月
	time_buf[3] = Ds1302ReadByte(ds1302_date_add);           	//日
	time_buf[4] = Ds1302ReadByte(ds1302_hr_add);           		//时
	time_buf[5] = Ds1302ReadByte(ds1302_min_add);          		//分
	time_buf[6] = (Ds1302ReadByte(ds1302_sec_add)) & 0x7f; 		//秒，屏蔽秒的第7位，避免超出59
	time_buf[7] = Ds1302ReadByte(ds1302_day_add);          		//周
}

/*函数名: Ds1302GetTime                              */
/*函数功能: DS1302向上层返回时间数据		         */
/*返回值:  无                                        */
/*参数  :                                            */
/**calendar_tmp     输入参数,存放时钟数据的结构体指针*/
void Ds1302GetTime(_calendar_obj *calendar_tmp)
{
	u8 time_buf[8]; 

	Ds1302ReadTime(time_buf);									//得到当前时间
	
	calendar_tmp->w_year = 2 * 1000 + 0 * 100 + (time_buf[1] >> 4) * 10 + (time_buf[1] & 0x0f);	//年

	calendar_tmp->w_month = (time_buf[2] >> 4) * 10 + (time_buf[2] & 0x0f); 					//月

	calendar_tmp->w_date = (time_buf[3] >> 4) * 10 + (time_buf[3] & 0x0f); 						//日

	calendar_tmp->hour = (time_buf[4] >> 4) * 10 + (time_buf[4] & 0x0f); 						//时

	calendar_tmp->min = (time_buf[5] >> 4) * 10 + (time_buf[5] & 0x0f); 						//分

	calendar_tmp->sec = (time_buf[6] >> 4) * 10 + (time_buf[6] & 0x0f); 						//秒
	
	calendar_tmp->week = (time_buf[7] & 0x07); 													//星期
}


uint8_t RTC_ByteToBcd2(uint8_t Value)
{
    uint32_t bcdhigh = 0U;

    while (Value >= 10U)
    {
        bcdhigh++;
        Value -= 10U;
    }

    return ((uint8_t)(bcdhigh << 4U) | Value);
}

uint8_t RTC_Bcd2ToByte(uint8_t Value)
{
    uint32_t tmp = 0U;
    tmp = ((uint8_t)(Value & (uint8_t)0xF0) >> (uint8_t)0x4) * 10;
    return (tmp + (Value & (uint8_t)0x0F));
}

//判断是否是闰年函数
//月份   1  2  3  4  5  6  7  8  9  10 11 12
//闰年   31 29 31 30 31 30 31 31 30 31 30 31
//非闰年 31 28 31 30 31 30 31 31 30 31 30 31
//输入:年份
//输出:该年份是不是闰年.1,是.0,不是
uint8_t Is_Leap_Year(uint16_t year)
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

//获得现在是星期几
//功能描述:输入公历日期得到星期(只允许1901-2099年)
//输入参数：公历年月日
//返回值：星期号
uint8_t RTC_Get_Week(uint16_t year, uint8_t month, uint8_t day)
{
    uint16_t temp2;
    uint8_t yearH, yearL;

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

uint8_t RTC_Set(uint64_t timecount)
{
    static uint16_t daycnt = 0;
    uint32_t temp = 0;
    uint16_t temp1 = 0;

    temp = (timecount / 86400); //得到天数(秒钟数对应的)
    if (daycnt != temp) //超过一天了
    {
        daycnt = temp;
        temp1 = 1970; //从1970年开始
        while (temp >= 365)
        {
            if (Is_Leap_Year(temp1)) //是闰年
            {
                if (temp >= 366)
                {
                    temp -= 366;    //闰年的秒钟数
                }
                else
                {
                    temp1++;
                    break;
                }
            }
            else
            {
                temp -= 365;    //平年
            }
            temp1++;
        }
        calendar.w_year = temp1; //得到年份
        temp1 = 0;
        while (temp >= 28) //超过了一个月
        {
            if (Is_Leap_Year(calendar.w_year) && temp1 == 1) //当年是不是闰年/2月份
            {
                if (temp >= 29)
                {
                    temp -= 29;    //闰年的秒钟数
                }
                else
                {
                    break;
                }
            }
            else
            {
                if (temp >= mon_table[temp1])
                {
                    temp -= mon_table[temp1];    //平年
                }
                else
                {
                    break;
                }
            }
            temp1++;
        }
        calendar.w_month = temp1 + 1;  //得到月份
        calendar.w_date = temp + 1;    //得到日期
    }
    temp = (timecount % 86400);        //得到秒钟数
    calendar.hour = temp / 3600;       //小时
    calendar.min = (temp % 3600) / 60; //分钟
    calendar.sec = (temp % 3600) % 60; //秒钟
    calendar.week = RTC_Get_Week(calendar.w_year, calendar.w_month, calendar.w_date); //获取星期
	//设置DS1302时间
	Time_Set[0] = RTC_ByteToBcd2((uint8_t)(calendar.w_year / 100));
	Time_Set[1] = RTC_ByteToBcd2((uint8_t)(calendar.w_year % 100));
    Time_Set[2] = RTC_ByteToBcd2(calendar.w_month);
    Time_Set[3] = RTC_ByteToBcd2(calendar.w_date);
    Time_Set[4] = RTC_ByteToBcd2(calendar.hour + 8);
    Time_Set[5] = RTC_ByteToBcd2(calendar.min);
    Time_Set[6] = RTC_ByteToBcd2(calendar.sec);
	Time_Set[7] = RTC_ByteToBcd2(calendar.week);
	Ds1302WriteTime(Time_Set);
    return 0;
}
