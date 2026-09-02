
#ifndef _HMI_DRIVER_
#define _HMI_DRIVER_
#include "common.h"
#define uchar    unsigned char
#define uint8    unsigned char
#define uint16   unsigned short int
#define uint32   unsigned long
#define int16    short int
#define int32    long
#include "uart.h"
//#define SEND_DATA(P) SendChar(P)          //发送一个字节
//#define TX8(P1) SendChar(P1&0xFF)                    //发送单个字节
//#define TX8_N(P,N) Send8_N((uint8 *)P,N)                 //发送N个字节
//#define TX16(P1) SendChar((P1)>>8);SendChar(P1)                 //发送16位整数
//#define TX16_N(P,N) Send16_N((uint16 *)P,N)              //发送N个16位整数
//#define TX32(P1) TX16(P1>>16);TX16(P1&0xFFFF)     //发送32位整数

#define LED0 PCout(15)// PC15

void LED_Init(void);
static void AddCRC16(uint8 *buffer,uint16 n,uint16 *pcrc);
uint16 CheckCRC16(uint8 *buffer,uint16 n);

void SendChar16(u16 t);
void SendChar32(u32 t);

void Send8(u8 t);
void Send16(u16 t);
void SendNU8(u8 *pData,u16 nDataLen);
void SendNU16(u16 *pData,u16 nDataLen);
void Send32(u32 t);
void SendStr(u8 *str);

//发送握手指令
void Send_HandShake(void);

//发送重启指令
void Send_HReset(void);

//发送页面跳转指令
void Send_SetPage(u16 pid,u16 time);

//发送读ID指令
void Send_GetPageID(void);

//发送设置背光亮度指令
void Send_SetBL(u16 val);

//发送获取背光亮度指令
void Send_GetBL(void);

//发送设置波特率指令
void Send_SetBaundRate(u16 val);

//发送设置定时器指令
void Send_SetTimmer(u16 tid, u16 sel, u16 time, u16 en);

//发送保存控件属性指令
void Send_Save(void);

//发送设置延时指令
void Send_SetDelay(u16 val);

//发送启用休眠定时器指令
void Send_SetSleep(u16 time, u16 bl_val);

//发送获取休眠定时器参数指令
void Send_GetSleep(void);

//发送设置蜂鸣器时间参数指令
void Send_SetBeep(u16 val);

//发送获蜂鸣器参数指令
void Send_GetBeep(void);

//发送设置整型变量指令
void Send_SetVAL(u16 val_id, u32 val);

//发送获取整型变量的值指令
void Send_GetVAL(u16 val_id);

//发送获取字符串变量指令
void Send_GetStrVAL(u16 val_id);

//发送设置控件X坐标指令
void Send_SetX(u16 ctr_id, u16 val);

//发送设置控件Y坐标指令
void Send_SetY(u16 ctr_id, u16 val);

//发送设置控件W宽度指令
void Send_SetW(u16 ctr_id, u16 val);

//发送设置控件H高度指令
void Send_SetH(u16 ctr_id, u16 val);

//发送设置控件的文本指令
void Send_SetText(u16 ctr_id, u8* str, u16 num);

//发送设置控件的背景色1指令(24位颜色)
void Send_SetBgColor1(u16 ctr_id, u16 state, u32 val);

//发送设置控件的背景色2指令(24位颜色)
void Send_SetBgColor2(u16 ctr_id, u16 state, u32 val);

//发送设置控件的前景色指令(24位颜色)
void Send_SetFColor(u16 ctr_id,u32 val);

//发送设置控件的值指令
void Send_SetCtrVAL(u16 ctr_id,u16 val);

//发送设置控件隐藏指令
void Send_SetHidden(u16 ctr_id, u16 state, u16 offon);

//发送获取控件的X坐标指令
void Send_GetX(u16 ctr_id);

//发送获取控件的Y坐标指令
void Send_GetY(u16 ctr_id);

//发送获取控件的W宽度指令
void Send_GetW(u16 ctr_id);

//发送获取控件的H高度指令
void Send_GetH(u16 ctr_id);

//发送获取控件的文本内容指令
void Send_GetText(u16 ctr_id);

//发送获取控件的背景色1指令
void Send_GetBgColor1(u16 ctr_id, u16 state);

//发送获取控件的背景色2指令
void Send_GetBgColor2(u16 ctr_id, u16 state);

//发送获取控件的前景色指令
void Send_GetFColor(u16 ctr_id);

//发送获取控件的值指令
void Send_GetCtrVAL(u16 ctr_id);



#endif
