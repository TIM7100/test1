#ifndef _RTC_H
#define _RTC_H

#include "common.h"

//日历结构体
typedef struct
{
    volatile u8 hour;
    volatile u8 min;
    volatile u8 sec;
    //公历日月年周
    volatile u16 w_year;
    volatile u8  w_month;
    volatile u8  w_date;
    volatile u8  week;
}_calendar_obj;

//DS1302地址定义
#define ds1302_sec_add          0x80      //秒数据地址
#define ds1302_min_add          0x82      //分数据地址
#define ds1302_hr_add           0x84      //时数据地址
#define ds1302_date_add         0x86      //日数据地址
#define ds1302_month_add        0x88      //月数据地址
#define ds1302_day_add          0x8a      //星期数据地址
#define ds1302_year_add         0x8c      //年数据地址
#define ds1302_control_add      0x8e      //控制数据地址
#define ds1302_charger_add      0x90
#define ds1302_clkburst_add     0xbe

//IO方向设置
#define DS1302_IO_IN()      (REG_GPIO_DIR(GPIOB) &= ~(1 << 12))
#define DS1302_IO_OUT()     (REG_GPIO_DIR(GPIOB) |= (1 << 12))
//IO操作函数
#define DS1302_DATA_OUT_0   (REG_GPIO_CLR(GPIOB) |= 1<<12)      //DATA 0
#define DS1302_DATA_OUT_1   (REG_GPIO_SET(GPIOB) |= 1<<12)      //DATA 1
#define DS1302_DATA_IN      (REG_GPIO_IDATA(GPIOB) & (1<<12))

#define DS1302_SCK_0        (REG_GPIO_CLR(GPIOB) |= 1<<11)      //SCLK 0
#define DS1302_SCK_1        (REG_GPIO_SET(GPIOB) |= 1<<11)      //SCLK 1
#define DS1302_RST_0        (REG_GPIO_CLR(GPIOB) |= 1<<0)       //RST 0
#define DS1302_RST_1        (REG_GPIO_SET(GPIOB) |= 1<<0)       //RST 1

/*函数功能: DS1302模块的初始化函数                   */
void Ds1302Init(void);

/*函数功能: 向DS1302写入一个字节数据                 */
void Ds1302WriteByte(u8 addr, u8 data);

/*函数功能: 从DS1302读出一个字节数据                 */
u8 Ds1302ReadByte(u8 addr);

/*函数功能: 向DS1302写入时间数据                      */
void Ds1302WriteTime(u8 *SetBuf);

/*函数功能: 从DS302读出时钟数据                      */
void Ds1302ReadTime(u8 *time_buf);

/*函数功能: DS1302向上层返回时间数据                 */
void Ds1302GetTime(_calendar_obj *calendar_tmp);

/*函数功能: 将传入的UNIX时间戳转BCD时间并设置        */
uint8_t RTC_Set(uint64_t timecount);
#endif
