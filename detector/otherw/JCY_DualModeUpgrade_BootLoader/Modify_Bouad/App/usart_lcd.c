#include "usart_lcd.h"


u8 Color;
u8 databuf[] = {"DC32(000,500,'1 ',4);\r\n"};
u16 X_point;
u16 Y_point;
u8  Error_Count;
u8 USART_RX_BUF[64];     //接收缓冲,最大64个字节.
u8 rx_flag_finished = 0;
char CMP_BUF[64];
extern u8 rx_flag;
extern u8  rx_count;
char* T_DC16(u16 x, u16 y, unsigned char *str, u8 color)
{
    int i, j;
    u16 Local_X;                           //X坐标
    u16 Local_Y;                           //Y坐标
    char BUF_DC32_HEAD[5] = {"DC16("};     //命令头
    char BUF_DC32_TAIL[4] = {");\r\n"};    //命令尾
    //char BUF_DC32[100];

    //清空BUFF  新屏幕需要
    for (i = 0; i < 64; i++)
    {
        CMP_BUF[i] = 0;
    }

    //配置命令头
    for (i = 0; i < 5; i++)
    {
        CMP_BUF[i] = BUF_DC32_HEAD[i];
    }

    //配置X Y 坐标
    Local_X = x;
    CMP_BUF[i++] = Local_X / 100 + 0x30;
    CMP_BUF[i++] = Local_X % 100 / 10 + 0x30;
    CMP_BUF[i++] = Local_X % 10 + 0x30;
    CMP_BUF[i++] = ',';
    Local_Y = y;
    CMP_BUF[i++] = Local_Y / 100 + 0x30;
    CMP_BUF[i++] = Local_Y % 100 / 10 + 0x30;
    CMP_BUF[i++] = Local_Y % 10 + 0x30;
    CMP_BUF[i++] = ',';
    CMP_BUF[i++] = '\'';

    //放入需要输出的内容
    j = 0;
    while (str[j] != 0)
    {
        CMP_BUF[j + i] = str[j];
        j++;
    }
    i += j;

    //配置输出颜色
    CMP_BUF[i++] = '\'';
    CMP_BUF[i++] = ',';
    CMP_BUF[i++] = color / 10 + 0x30;
    CMP_BUF[i++] = color % 10 + 0x30;

    //配置命令尾
    for (j = 0; j < 4; j++)
    {
        CMP_BUF[i + j] = BUF_DC32_TAIL[j];
    }

    i += j;

    //返回指针
    return CMP_BUF;
}


char* T_DC16_Len(u16 x, u16 y, unsigned char *str, u32 length, u8 color)
{
    int i, j;
    u16 Local_X;                           //X坐标
    u16 Local_Y;                           //Y坐标
    char BUF_DC32_HEAD[5] = {"DC16("};     //命令头
    char BUF_DC32_TAIL[4] = {");\r\n"};    //命令尾
    //char BUF_DC32[100];

    //配置命令头
    for (i = 0; i < 5; i++)
    {
        CMP_BUF[i] = BUF_DC32_HEAD[i];
    }

    //配置X Y 坐标
    Local_X = x;
    CMP_BUF[i++] = Local_X / 100 + 0x30;
    CMP_BUF[i++] = Local_X % 100 / 10 + 0x30;
    CMP_BUF[i++] = Local_X % 10 + 0x30;
    CMP_BUF[i++] = ',';
    Local_Y = y;
    CMP_BUF[i++] = Local_Y / 100 + 0x30;
    CMP_BUF[i++] = Local_Y % 100 / 10 + 0x30;
    CMP_BUF[i++] = Local_Y % 10 + 0x30;
    CMP_BUF[i++] = ',';
    CMP_BUF[i++] = '\'';

    //放入需要输出的内容
//     j = 0;
//     while (str[j] != 0)
//     {
//         CMP_BUF[j + i] = str[j];
//         j++;
//     }
//     i += j;
    for (j = 0; j < length; j++)
    {
        CMP_BUF[j + i] = str[j];
    }
    i += j;

    //配置输出颜色
    CMP_BUF[i++] = '\'';
    CMP_BUF[i++] = ',';
    CMP_BUF[i++] = color / 10 + 0x30;
    CMP_BUF[i++] = color % 10 + 0x30;

    //配置命令尾
    for (j = 0; j < 4; j++)
    {
        CMP_BUF[i + j] = BUF_DC32_TAIL[j];
    }

    i += j;

    //返回指针
    return CMP_BUF;
}





u8 rx_flag_finished;
u8 RX_BUF[];
void CheckBusy(void)
{
    u32 time_out = 0;
    for (time_out = 0; time_out < 1000; time_out++)
    {
        if (rx_flag)
        {
            Delay_us(15);
            rx_count = 0;
            rx_flag = 0;
        }
        Delay_us(30);
    }
}

void UartSend(char * databuf)
{
    //u8 i=0;
    while (1)
    {
        if ((*databuf) != 0) //直到数据都发送完成
        {
            UART_SendData(UART1, *databuf); //发送一个字节数据
            databuf++;//i++;
        }
        else
        {
            return;
        }
    }
}

void UartLcdInit()
{
    //---------等待LCD上电-----------
    Delay_ms(500);           //至少需要5250000/234ms
    //-------------------------------

    UartSend("DIR(1);\r\n");
    CheckBusy();
    UartSend("CLR(0);\r\n");
    CheckBusy();

}

void LcdShowType(u8* LcdData, u8 Color)
{
    UartSend("CLR(0);\r\n");
    CheckBusy();
    UartSend(T_DC16(10, 30, "所检芯片的型号为：", 16));// (X坐标， Y坐标， 显示字符串， 颜色)
    CheckBusy();
    UartSend(T_DC16(40, 75, LcdData, Color));// (X坐标， Y坐标， 显示字符串， 颜色)
    CheckBusy();
}

