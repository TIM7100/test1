#include "lcd.h"

extern u8 LanguageMode;
extern u8 CheckStatus;
char Cmd_Buf[64];
u32 Cmd_Len;

void LedOn(void)				//灯全开
{
    GREEN_ON;
    RED_ON;

}

void LedOff(void)				//灯关
{
    RED_OFF;
    GREEN_OFF;

}


void Led2Red(void)				//亮红灯
{
    RED_ON;
    GREEN_OFF;

}

void Led2Green(void)			//亮绿灯
{
    GREEN_ON;
    RED_OFF;

}


void ClearShow(void)    //清空全部显示效果
{
    LedOff();
    UartSend("CLR(0);\r\n");	//清屏
    CheckBusy();
    UartSend("DIR(1);\r\n");	 //横屏
    CheckBusy();
}

void  MainpageShow(char *Version)          //主页面显示
{
    //---------等待LCD上电-----------
    Delay_ms(1000);           //等一秒串口屏上电
    //-------------------------------

    //Led2Green();
    UartSend("CLR(0);\r\n");
    CheckBusy();
    UartSend("DIR(1);\r\n");
    CheckBusy();

    UartSend("DC16(26,10,'三星检测识别仪',2);\r\n");
    CheckBusy();
    UartSend("DC16(7,60,'版本：',4);\r\n");
    CheckBusy();
    UartSend(CMD_DC16(60, 60, Version, 4));        //显示版本号
    CheckBusy();
    UartSend("DC16(16,90,'- 按\"确定\"继续 -',4);\r\n");
    CheckBusy();
}



void StrMiddleShow(char *StrName, u8 Yaxis, u8 Color)      //让字符串在每行居中显示
{
    u8 Strlen = 0;
    u8 Xaxis = 0;

    Strlen = strlen(StrName);               //获取型号长度
    Xaxis = (160 - 8 * Strlen) / 2;          //计算型号要显示的位置的x轴
    UartSend(CMD_DC16(Xaxis, Yaxis, StrName, Color));
    CheckBusy();
}





char* CMD_DC16(u16 x, u16 y, char *str, u8 Color)
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

char* CMD_DC16_Len(u16 x, u16 y, char *str, u32 length, u8 Color)
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
    for(j = 0; j < length; j++)
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

void ProgressBar(u32 cnt, u32 sector)           //显示开机界面 cnt：进度条加载次数
{

    char boxbuf[32] = {0};

//    UartSend("CLR(0);\r\n");
//    CheckBusy();


    sprintf(boxbuf, "BOXF(10,75,%02d,90,4);\r\n", ((140 * cnt ) / sector) + 10);
    UartSend(boxbuf);
//	CheckBusy();


}