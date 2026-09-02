
#ifndef _CMD_PROCESS_H
#define _CMD_PROCESS_H
#include "hmi_driver.h"


#define IS_ADDR                     0     //是否有地址
#define IS_CRC16                    0     //是否需要CRC效验

#define NOTIFY_HEAD_TOUCH               0XCC  //触摸通知
#define NOTIFY_HEAD_RES                 0XEE  //回复通知
#define NOTIFY_HEAD_USER                 0XAA  //客户定制帧头

#define NOTIFY_TYPE_TOUCH_PRESS         0X06  //触摸屏按下通知

#define PTR2U16(PTR) ((((uint8 *)(PTR))[0]<<8)|((uint8 *)(PTR))[1])  //从缓冲区取16位数据
#define PTR2U32(PTR) ((((uint8 *)(PTR))[0]<<24)|(((uint8 *)(PTR))[1]<<16)|(((uint8 *)(PTR))[2]<<8)|((uint8 *)(PTR))[3])  //从缓冲区取32位数据

#define Get16(PTR,i) ((((uint8 *)(PTR))[i]<<8)|((uint8 *)(PTR))[i+1])  //取16位数据
#define Get_Buff_8(i) cmd_buffer[i]   //从cmd_buffer缓冲区取8位数据
#define Get_Buff_16(i) Get16(cmd_buffer,i)    //从cmd_buffer缓冲区取16位数据

#pragma pack(push)
#pragma pack(1)                           //按字节对齐

typedef struct
{
    uint8    cmd_head;                    //帧头
    uint8    param[256];                  //可变长度参数，最多256个字节
    uint8  cmd_tail[4];                   //帧尾
}CTRL_MSG,*PCTRL_MSG;

#pragma pack(pop)//触摸指令处理
void Process_Touch( PCTRL_MSG msg, uint16 size );
//回发指令处理
void Process_Postback( PCTRL_MSG msg, uint16 size );

void Touch_LED_ONOFF(u16 val); //控制继电器点亮台灯
void Touch_Color_LED_ONOFF(u16 val); //显示彩灯状态
void Touch_Curtain(u16 val); //窗帘状态（全开/全关/关开）
void Touch_Get_PageID(u16 val); //读取ID
void Touch_BL_Up(void); //亮度增加
void Touch_BL_Down(void); //亮度减少
void Touch_Bat_State(u16 val); //电池状态

//=============================
void Postback_Get_Bat_Vol(u8* p);   //获取电池电压label值
void Postback_Get_Bat_Power(u8* p);  //获取电池电压label值
void Postback_Get_PageID(void);  //获取电池电压label值


#endif


