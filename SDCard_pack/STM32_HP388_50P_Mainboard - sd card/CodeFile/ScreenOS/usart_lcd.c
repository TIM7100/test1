#include "sys.h"
#include "usart_lcd.h"
#include "Timer0.h"
#include "menu.h"
#include "user_OS.h"
#include "string.h"
#include "CommonFunction.h"
#include "MainSlaveInterface.h"

u8 RX_BUF[3]; //全局变量，中断中修改接收值
u8 UartRxFinished = 0;
u8 UartRxCount = 0;
char CMP_BUF[100];            //需要返回指针，开是否使用static局部变量

/*
//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
    int handle;

};

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
_sys_exit(int x)
{
    x = x;
}
//重定义fputc函数
int fputc(int ch, FILE *f)
{
    while ((USART1->SR & 0X40) == 0); //循环发送,直到发送完毕
    USART1->DR = (u8) ch;
    return ch;
}
#endif
*/

int UartGet(void)
{

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}


void CheckBusy(void)
{
    u32 time_out = 0;
    for (;;)
    {
        if (UartRxFinished == 0xc0)
        {
            if ((RX_BUF[0] == 'O') && (RX_BUF[1] == 'K'))
            {
                RX_BUF[0] = 0;
                RX_BUF[1] = 0;
                UartRxFinished = 0;
                UartRxCount = 0;
                break;
            }
        }
        if (time_out++ >= 800000)
        {
            UartRxFinished = 0;
            UartRxCount = 0;
            break;
        }
    }
}

void UartSend(char * databuf)
{
    while (1)
    {
        if ((*databuf) != 0) //直到数据都发送完成
        {
            USART_SendData(USART1, *databuf); //发送一个字节数据
            while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {}; //用于检查串口UART1是否发送完成，完成时，TC中断标志置位，
            databuf++;//i++;
        }
        else
        {
            return;
        }
    }
}

void LcdDrvInit(void)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;           //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;         //复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //USART1_RX   PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //Usart1 NVIC 配置

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;               //设置串口1 中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ;      //抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;      //子优先级为3

    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1

    //USART 初始化设置

    USART_InitStructure.USART_BaudRate = 115200;                        //波特率115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //设置接受一个数据尾数为八位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;      //设置结束或者发送数据后加入的个数
    USART_InitStructure.USART_Parity = USART_Parity_No;                     //奇偶使能
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;         //硬件流控制使能
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;   //使能接受发送模式

    USART_Init(USART1, &USART_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART1, ENABLE);                    //使能串口

    TimerDelayMS(1000);

    UartSend("DIR(1);\r\n");
    CheckBusy();
    TimerDelayMS(500);
}


// void ShowResult_50pcs(u8* slave_result)       //横排显示五十个片选信号
// {
//     int i = 0;
//     int Y_START = 250;
//     int Y_STEP = 35;
//     int Y_line = 0;    //Y轴起始列
//     int Y_LINE_MAX = 5;  //Y轴最大列数
//     int X_START_LINE = 10;
//     int X_START = 50;
//     int X_STEP = 40;
//     int X_axis = 0;   //X轴位置

//     u8 color;   //字体的颜色
//     u8 COLOR_RED = 1; //字体显示红色
//     u8 COLOR_GREEN = 2;  //字体显示绿色
//     u8 COLOR_WHITE = 15;  //字体显示绿色
//     u8 Error_Count = 0;   //错误值
//     u8 Lcd_Status_Buf[2];    //显示屏显示的文字  1-10
//     u8 line[1] = {0};        //显示屏显示的文字   A-E
//     u8 error_buf[] = {"DC32(144,280,'存在  个错误',1);\r\n"};

//     UartSend("CLR(0);\r\n");  //清屏
//     CheckBusy();
//     UartSend(T_DC32(90, 20, IndexType[Index_Select].name, 5));  //显示型号
//     CheckBusy();
//     UartSend(T_DC32(144, 60, "本轮结果如下", COLOR_WHITE));
//     CheckBusy();

//     for (Y_line = 0; Y_line < Y_LINE_MAX ; Y_line++)    //循环Y轴5次
//     {
//         line[0] = 0x41 + Y_line ;   //abcde
//         UartSend(T_DC32(X_START_LINE, Y_START - (Y_STEP * Y_line), line, COLOR_WHITE));
//         CheckBusy();
//         for (; X_axis < 10; X_axis++)    //10次信号循环
//         {
//             Lcd_Status_Buf[1] = 0;
//             Lcd_Status_Buf[0] = X_axis + 0x31;
//             if (X_axis == 9)
//             {
//                 Lcd_Status_Buf[0] = 0x31;
//                 Lcd_Status_Buf[1] = 0x30;
//             }
//             if (slave_result[i] == 0x01) //接收从机发过来的数据是错误（即CrcFlag[i]==1）,则5个都为错
//             {

//                 color = COLOR_RED;  //数据错误，显示红色字体
//                 Error_Count++;      //错误统计
//             }
//             else
//             {
//                 color = COLOR_GREEN;   //数据正确，显示绿色字体

//             }
//             UartSend(T_DC32(X_START + (X_axis * X_STEP), Y_START - (Y_line * Y_STEP), Lcd_Status_Buf, color));
//             CheckBusy();
//             slave_result[i] >>= 1;
//             i++;
//                      if (i > 50) //i>50,跳出循环
//             {
//                 break;
//             }
//             if (X_axis == 9)   // 一行结束，新起一行
//             {
//                 X_axis = 0;
//                 break;
//             }
//         }
//     }
//     if (Error_Count == 0)
//     {
//         UartSend(T_DC32(176, 280, "全部通过", 23));
//         CheckBusy();
//     }
//     else
//     {
//         error_buf[18] = (Error_Count / 10) + 0x30;
//         error_buf[19] = (Error_Count % 10) + 0x30;
//         UartSend((char *)error_buf);
//         CheckBusy();
//
// //           Speak();
// //           delay_ms(500);
//     }
// }


//例子2    1-5  6-9显示
// void ShowResult_50pcs(u8* slave_result)       //TEST 显示
// {
//     int i = 0;
//    int Y_START = 110;
//     int Y_STEP = 35;
//     int Y_line = 0;    //Y轴起始列
//     int Y_LINE_MAX = 10;  //Y轴最大列数
//     int X_axis = 0;   //X轴位置
//     int X_START = 30;
//     int X_STEP = 45;
//

//     u8 color;   //字体的颜色
//     u8 COLOR_RED = 1; //字体显示红色
//     u8 COLOR_GREEN = 2;  //字体显示绿色
//     u8 COLOR_WHITE = 15;  //字体显示绿色
//     u8 Error_Count = 0;   //错误值
//     u8 Lcd_Status_Buf[2];    //显示屏显示的文字  1-10
//     u8 line[1] = {0};        //显示屏显示的文字   A-E
//     u8 error_buf[] = {"DC32(144,280,'存在  个错误',1);\r\n"};

//     UartSend("CLR(0);\r\n");  //清屏
//     CheckBusy();
//     UartSend(T_DC32(90, 20, IndexType[Index_Select].name, 5));  //显示型号
//     CheckBusy();

//      UartSend(T_DC32(48, 60, "A     B     C    D     E", COLOR_WHITE));   //显示ABCDE
//      CheckBusy();

//     for (Y_line = 0; Y_line < Y_LINE_MAX ; Y_line++)    //循环Y轴5次
//     {

//         for (; X_axis < 5; X_axis++)    //10次信号循环
//         {
//             Lcd_Status_Buf[1] = 0;
//             Lcd_Status_Buf[0] = X_axis + 0x31;
//             if (X_axis == 9)
//             {
//                 Lcd_Status_Buf[0] = 0x31;
//                 Lcd_Status_Buf[1] = 0x30;
//             }
//             if (slave_result[i] == 0x01) //接收从机发过来的数据是错误（即CrcFlag[i]==1）,则5个都为错
//             {

//                 color = COLOR_RED;  //数据错误，显示红色字体
//                 Error_Count++;      //错误统计
//             }
//             else
//             {
//                 color = COLOR_GREEN;   //数据正确，显示绿色字体

//             }
//             UartSend(T_DC32(X_START + (Y_line * X_STEP), Y_START + (X_axis * Y_STEP), Lcd_Status_Buf, color));
//             CheckBusy();
//             slave_result[i] >>= 1;
//             i++;
//                      if (i > 50) //i>50,跳出循环
//             {
//                 break;
//             }
//             if (X_axis == 4 )   // 同一组新的一行
//             {
//                                  Y_line++;
//
//                                  for (X_axis=5; X_axis < 10; X_axis++)    //5次信号循环  另起一行6-10显示
//                                  {
//                                      Lcd_Status_Buf[1] = 0;
//                                      Lcd_Status_Buf[0] = X_axis + 0x31;
//                                      if (X_axis == 9)
//                                      {
//                                              Lcd_Status_Buf[0] = 0x31;
//                                              Lcd_Status_Buf[1] = 0x30;
//                                      }
//                                      if (slave_result[i] == 0x01) //接收从机发过来的数据是错误（即CrcFlag[i]==1）,则5个都为错
//                                      {

//                                              color = COLOR_RED;  //数据错误，显示红色字体
//                                              Error_Count++;      //错误统计
//                                      }
//                                      else
//                                      {
//                                              color = COLOR_GREEN;   //数据正确，显示绿色字体

//                                      }
//                                      UartSend(T_DC32(X_START + (Y_line * X_STEP), Y_START + ((X_axis-5) * Y_STEP), Lcd_Status_Buf, color));
//                                      CheckBusy();
//                                      slave_result[i] >>= 1;
//                                      i++;
//                                      if (i > 50) //i>50,跳出循环
//                                      {
//                                              break;
//                                      }
//
//                                      if (X_axis == 9)   // 该组结束，新起一行
//                                      {
//                                              X_axis = 0;
//                                              break;
//                                      }
//                              }
//
//                              X_axis = 0;
//                 break;
//             }
//         }
//     }
//     if (Error_Count == 0)
//     {
//         UartSend(T_DC32(176, 280, "全部通过", 23));
//         CheckBusy();
//     }
//     else
//     {
//         error_buf[18] = (Error_Count / 10) + 0x30;
//         error_buf[19] = (Error_Count % 10) + 0x30;
//         UartSend((char *)error_buf);
//         CheckBusy();
//
//     }
// }



//例子1         1-5显示
void ShowResult_50pcs(u8* slave_result)       //TEST 显示
{
    int i = 0;
    int Y_START = 110;
    int Y_STEP = 35;
    int Y_line = 0;    //Y轴起始列
    int Y_LINE_MAX = 10;  //Y轴最大列数
    int X_axis = 0;   //X轴位置
    int X_START = 30;
    int X_STEP = 45;
    u8 Accor[2];

    u8 color;   //字体的颜色
    u8 COLOR_RED = 1; //字体显示红色
    u8 COLOR_GREEN = 2;  //字体显示绿色
    u8 COLOR_WHITE = 15;  //字体显示绿色
    u8 Error_Count = 0;   //错误值
    u8 Lcd_Status_Buf[2];    //显示屏显示的文字  1-10
//     u8 line[1] = {0};        //显示屏显示的文字   A-E
    u8 error_buf[] = {"DC32(144,280,'存在  个错误',1);\r\n"};

    UartSend("CLR(0);\r\n");  //清屏
    CheckBusy();
    UartSend(T_DC32(90, 20, MenuPoint[UserChoose].DisplayString, 5));  //显示型号
    CheckBusy();

//      UartSend(T_DC32(16, 60, "① ② ③ ④ ⑤ ⑥ ⑦ ⑧ ⑨ ⑩", COLOR_WHITE));   //显示ABCDE
//      UartSend(T_DC32(40, 60, "1 |2 |3 |4 |5 |6 |7|8 |9 10", COLOR_WHITE));   //显示ABCDE
    for (i = 0; i < 10; i++) //显示1-10列
    {
        Accor[1] = 0;
        Accor[0] = i + 0x31;

        if (i == 9)
        {
            Accor[0] = 0x31;
            Accor[1] = 0x30;
        }

        UartSend(T_DC32(30 + 45 * i, 70, Accor, COLOR_WHITE));
        CheckBusy();
    }
    i = 0;

    for (Y_line = 0; Y_line < Y_LINE_MAX ; Y_line++)    //循环Y轴5次
    {

        for (; X_axis < 5; X_axis++)    //10次信号循环
        {
            Lcd_Status_Buf[1] = 0;
            Lcd_Status_Buf[0] = X_axis + 0x31;
            if (X_axis == 9)
            {
                Lcd_Status_Buf[0] = 0x31;
                Lcd_Status_Buf[1] = 0x30;
            }
            if (slave_result[i] == 0x01) //接收从机发过来的数据是错误（即CrcFlag[i]==1）,则5个都为错
            {
                color = COLOR_RED;  //数据错误，显示红色字体
                Error_Count++;      //错误统计
            }
            else
            {
                color = COLOR_GREEN;   //数据正确，显示绿色字体
            }
            UartSend(T_DC32(X_START + (Y_line * X_STEP), Y_START + (X_axis * Y_STEP), Lcd_Status_Buf, color));
            CheckBusy();
            slave_result[i] >>= 1;
            i++;
            if (i > 50) //i>50,跳出循环
            {
                break;
            }
            if (X_axis == 4)    // 同一组新的一行
            {
                X_axis = 0;
                break;

            }
            if (X_axis == 9)   // 该组结束，新起一行
            {
                X_axis = 0;
                break;
            }
        }
    }
    if (Error_Count == 0)
    {
        UartSend(T_DC32(176, 280, "全部通过", 23));
        CheckBusy();
    }
    else
    {
        error_buf[18] = (Error_Count / 10) + 0x30;
        error_buf[19] = (Error_Count % 10) + 0x30;
        UartSend((char *)error_buf);
        CheckBusy();

    }
}




void USART1_IRQHandler(void)                    //串口中断
{
    u8 Res = 1;
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //0x0d 0x0a  串口传输状态获取  中断产生
    {
        Res = USART_ReceiveData(USART1); //(USART1->DR);   获取串口的数据

        if (UartRxFinished != 0xc0) //
        {
            if (UartRxFinished & 0x40) //0x0d 0x0a -->     OK\r\n   模块执行完
            {
                if (Res == 0x0a)
                {
                    UartRxFinished |= 0x80; //  得到UartRxFinished=0xc0，
                    RX_BUF[UartRxCount] = '\0'; //
                }
                else
                {
                    UartRxFinished = 0; //????,????
                    UartRxCount = 0;
                }
            }
            else //????0X0D
            {
                if (Res != 0x0d)
                {
                    RX_BUF[UartRxCount++] = Res ;
                }
                else
                {
                    UartRxFinished |= 0x40;
                }
            }
        }

    }
}



//LCD转换函数(DC32)
char* T_DC32(u16 x, u16 y, unsigned char *str, u8 color)
{
    int i = 0;
//    int len;
    u16 Local_X;                           //X坐标
    u16 Local_Y;                           //Y坐标
    unsigned char BUF_DC32_HEAD[5] = {"DC32("};     //命令头
    unsigned char BUF_DC32_TAIL[4] = {");\r\n"};    //命令尾
    //char BUF_DC32[100];
    //配置命令头
    memcpy_data_2(CMP_BUF, BUF_DC32_HEAD, 5, 0);
    //配置X Y 坐标
    Local_X = x;
    CMP_BUF[5] = Local_X / 100 + 0x30;
    CMP_BUF[6] = Local_X % 100 / 10 + 0x30;
    CMP_BUF[7] = Local_X % 10 + 0x30;
    CMP_BUF[8] = ',';
    Local_Y = y;
    CMP_BUF[9] = Local_Y / 100 + 0x30;
    CMP_BUF[10] = Local_Y % 100 / 10 + 0x30;
    CMP_BUF[11] = Local_Y % 10 + 0x30;
    CMP_BUF[12] = ',';
    CMP_BUF[13] = '\'';
    //放入需要输出的内容

    while (str[i] != 0)
    {
        CMP_BUF[i + 14] = str[i];
        i++;
    }
//
//     len = strlen(str);   //判断显示的长度
//     Memcpy_Data_char1(CMP_BUF, str, len, 14);
//     i = len;
    //配置输出颜色
    CMP_BUF[i + 14] = '\'';
    CMP_BUF[i + 15] = ',';
    CMP_BUF[i + 16] = color / 10 + 0x30;
    CMP_BUF[i + 17] = color % 10 + 0x30;
    //配置命令尾
    memcpy_data_2(CMP_BUF, BUF_DC32_TAIL, 4, i + 18);
    CMP_BUF[i + 22] = 0;
    //返回指针
    return CMP_BUF;
}

//LCD转换函数(DC24)
char* T_DC16(u16 x, u16 y, unsigned char *str, u8 color)
{
    int i = 0;
//    int len;
    u16 Local_X;                           //X坐标
    u16 Local_Y;                           //Y坐标
    unsigned  char BUF_DC32_HEAD[5] = {"DC24("};     //命令头
    unsigned char BUF_DC32_TAIL[4] = {");\r\n"};    //命令尾
    //配置命令头

    memcpy_data_2(CMP_BUF, BUF_DC32_HEAD, 5, 0); //函数复制

    //配置X Y 坐标
    Local_X = x;
    CMP_BUF[5] = Local_X / 100 + 0x30;
    CMP_BUF[6] = Local_X % 100 / 10 + 0x30;
    CMP_BUF[7] = Local_X % 10 + 0x30;
    CMP_BUF[8] = ',';
    Local_Y = y;
    CMP_BUF[9]  = Local_Y / 100 + 0x30;
    CMP_BUF[10] = Local_Y % 100 / 10 + 0x30;
    CMP_BUF[11] = Local_Y % 10 + 0x30;
    CMP_BUF[12] = ',';
    CMP_BUF[13] = '\'';
    //放入需要输出的内容
    while (str[i] != 0)
    {
        CMP_BUF[i + 14] = str[i];
        i++;
    }
//
//  len = strlen(str); //判断显示的长度
//  Memcpy_Data_char1(CMP_BUF, str, len, 14);
//  i = len;

    //配置输出颜色
    CMP_BUF[i + 14] = '\'';
    CMP_BUF[i + 15] = ',';
    CMP_BUF[i + 16] = color / 10 + 0x30;
    CMP_BUF[i + 17] = color % 10 + 0x30;
    //配置命令尾
    memcpy_data_2(CMP_BUF, BUF_DC32_TAIL, 4, i + 18);
    CMP_BUF[i + 22] = 0;
    //返回指针
    return CMP_BUF;
}
