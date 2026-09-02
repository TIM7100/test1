#include "lcd.h"
#include "uart.h"
#include "delay.h"
#include "gpio.h"
#include "w5500_user_conf.h"
char nameBuf[20] = {0};
char Cmd_Buf[64];
u32 Cmd_Len;


char* CMD_DC16(u16 x, u16 y, u8 *str, u8 Color)
{
    u8 i, j;
    u16 Set_X;                          //X坐标
    u16 Set_Y;                          //Y坐标
    u8 Cmd_Start[5] = {"DC16("};     //开始指令
    u8 Cmd_End[4] = {");\r\n"};      //结束指令

    MemsetData(Cmd_Buf, 0, 64);

    for (i = 0; i < 5; i++)
    {
        Cmd_Buf[i] = Cmd_Start[i];    //传“DC16(”
    }

//传坐标X和Y 128X160像素点
    Set_X = x;
    Cmd_Buf[i++] = Set_X / 100 + 0x30;       //百位
    Cmd_Buf[i++] = Set_X % 100 / 10 + 0x30;  //十位
    Cmd_Buf[i++] = Set_X % 10 + 0x30;        //个位
    Cmd_Buf[i++] = ',';

    Set_Y = y;
    Cmd_Buf[i++] = Set_Y / 100 + 0x30;       //百位
    Cmd_Buf[i++] = Set_Y % 100 / 10 + 0x30;  //十位
    Cmd_Buf[i++] = Set_Y % 10 + 0x30;        //个位
    Cmd_Buf[i++] = ',';
    Cmd_Buf[i++] = '\'';

//传显示字符串
    j = 0;
    while (str[j] != '\0')
    {
        Cmd_Buf[i + j] = str[j];
        j++;
    }
    i += j;

//传显示颜色
    Cmd_Buf[i++] = '\'';
    Cmd_Buf[i++] = ',';
    Cmd_Buf[i++] = Color / 10 + 0x30;     //十位
    Cmd_Buf[i++] = Color % 10 + 0x30;     //个位

//传输结束指令
    for (j = 0; j < 4; j++)
    {
        Cmd_Buf[i + j] = Cmd_End[j];
    }

    i += j;

    return Cmd_Buf;                   //返回命令指针
}

char* CMD_DC16_Len(u16 x, u16 y, u8 *str, u32 length, u8 Color)
{
    u8 i, j;
    u16 Set_X;                          //X坐标
    u16 Set_Y;                          //Y坐标
    u8 Cmd_Start[5] = {"DC16("};     //开始指令
    u8 Cmd_End[4] = {");\r\n"};      //结束指令

    MemsetData(Cmd_Buf, 0, 64);

    for (i = 0; i < 5; i++)
    {
        Cmd_Buf[i] = Cmd_Start[i];    //传“DC16(”
    }

//传坐标X和Y 128X160像素点
    Set_X = x;
    Cmd_Buf[i++] = Set_X / 100 + 0x30;       //百位
    Cmd_Buf[i++] = Set_X % 100 / 10 + 0x30;  //十位
    Cmd_Buf[i++] = Set_X % 10 + 0x30;        //个位
    Cmd_Buf[i++] = ',';

    Set_Y = y;
    Cmd_Buf[i++] = Set_Y / 100 + 0x30;       //百位
    Cmd_Buf[i++] = Set_Y % 100 / 10 + 0x30;  //十位
    Cmd_Buf[i++] = Set_Y % 10 + 0x30;        //个位
    Cmd_Buf[i++] = ',';
    Cmd_Buf[i++] = '\'';

//传显示字符串
    for (j = 0; j < length; j++)
    {
        Cmd_Buf[i + j] = str[j];
    }
    i += j;

//传显示颜色
    Cmd_Buf[i++] = '\'';
    Cmd_Buf[i++] = ',';
    Cmd_Buf[i++] = Color / 10 + 0x30;     //十位
    Cmd_Buf[i++] = Color % 10 + 0x30;     //个位

//传输结束指令
    for (j = 0; j < 4; j++)
    {
        Cmd_Buf[i + j] = Cmd_End[j];
    }

    i += j;

    return Cmd_Buf;                   //返回命令指针
}

void ClearShow(void)   
{
	LCD_UartSend("CLR(0);\r\n");
	LCD_UartSend("DIR(1);\r\n");
}

void LCD_UartSend(char *databuf)
{
	UartSend(databuf);
// 	CheckBusy();
}
