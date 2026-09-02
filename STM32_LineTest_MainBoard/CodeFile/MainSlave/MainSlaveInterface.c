#include "crc.h"
#include "Timer0.h"
#include "CommonFunction.h"
#include "MainSlaveInterface.h"
#include "commandlist_NPG83_10P.h"


u16 MainSendDataLen;      //主机发送给从机的数据长度，包括CRC的长度，4Byte命令位 + 2Byte长度位(命令位+长度位+数据总长度)

extern u8 Receive_SN[0x95E];

void MainSlaveGpioInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOC, ENABLE); //使能PA,PE,PC端口时钟
    /* 配置DATA IO */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_HIGH;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  /* DATA IO 配置成输出状态 PE8--PE15*/
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(Bur_DATA_PORT, &GPIO_InitStructure);/* data port gpio */
    Bur_DATA_PORT->BRR  = GPIO_Pin_HIGH;  /* DATA IO 输出高 */

    /* init RST CLK      Bur_RST_PORT = Bur_CLK_PORT = GPIOC */
    GPIO_InitStructure.GPIO_Pin = Bur_RST + Bur_CLK;
    GPIO_Init(Bur_RST_PORT, &GPIO_InitStructure);
    Bur_RST_PORT->BRR  = Bur_RST + Bur_CLK;      //输出高

    /* init CS */
    GPIO_InitStructure.GPIO_Pin = Bur_CS0 + Bur_CS1 + Bur_CS2 + Bur_CS3 + Bur_CS4 + Bur_CS5;
    GPIO_Init(Bur_CS_PORT, &GPIO_InitStructure);
    Bur_CS_PORT->BSRR  = Bur_CS0 + Bur_CS1 + Bur_CS2 + Bur_CS3 + Bur_CS4 + Bur_CS5;  /* CS 脚输出高 */

    /* init CSALL */
    GPIO_InitStructure.GPIO_Pin = Bur_CSALL;
    GPIO_Init(Bur_CSALL_PORT, &GPIO_InitStructure);

    Bur_CSALL_PORT->BSRR  = Bur_CSALL;                           /* CSALL 输出高 */



}

void Speak_init()
{

    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);  //使能时钟

    /* Data IO 配置 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void Speak()
{
    int i = 0;
    Speak_init();
    for (i = 0; i < 100; i++)
    {
        GPIO_SetBits(GPIOD, GPIO_Pin_5);   //置1
        delay_ms(1);
        GPIO_ResetBits(GPIOD, GPIO_Pin_5);   //置0
        delay_ms(1);
    }
    delay_ms(50);
    for (i = 0; i < 100; i++)
    {
        GPIO_SetBits(GPIOD, GPIO_Pin_5);   //置1
        delay_ms(1);
        GPIO_ResetBits(GPIOD, GPIO_Pin_5);   //置0
        delay_ms(1);
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* 函数名 :    DATAIO_INPUT_MODE                                                                           */
/*                                                                                                         */
/* 参数   :    无                                                                                          */
/*                                                                                                         */
/* 返回值 :    无                                                                                          */
/*                                                                                                         */
/* 作用:                                                                                                   */
/*             计算传输数据的CRC，保证大小板通信正常                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void MainDataIn(void)//MainDataOut
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Data IO 配置 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_HIGH;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; /* DATA IO 改成输入模式 */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(Bur_DATA_PORT, &GPIO_InitStructure);

    TimerDelayUS(1000);
}

void MainDataOut(void)//MainDataIn
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Data IO 配置 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_HIGH;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      /* DATA IO 改成输出模式 */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(Bur_DATA_PORT, &GPIO_InitStructure);

    TimerDelayUS(1000);
}


void MainSendDataByte(u8 burData8Bit) //MainSendDataByte
{
    u16 temp;

    temp = (u16)burData8Bit;

    Bur_DATA_PORT->ODR &= 0x00FF;
    Bur_DATA_PORT->ODR |= (temp << 8);
    SET_BUR_CLK();
    TimerDelayUS(3);

    Bur_DATA_PORT->ODR &= 0x00FF;
    Bur_DATA_PORT->ODR |= ((temp + 1) << 8);
    CLR_BUR_CLK();
    TimerDelayUS(3);
}

void MainSendDataWord(u8 *p_burData8Bit) //TmpBuf[]
{
    u16 temp;
    u16 temp2;

    temp = *p_burData8Bit;
    temp2 = *(p_burData8Bit + 1);

    Bur_DATA_PORT->ODR &= 0x00FF;
    Bur_DATA_PORT->ODR |= (temp << 8);     //上升沿发送一次数据
    SET_BUR_CLK();
    TimerDelayUS(5);

    Bur_DATA_PORT->ODR &= 0x00FF;
    Bur_DATA_PORT->ODR |= (temp2 << 8);  //下降沿发送一次数据
    CLR_BUR_CLK();
    TimerDelayUS(5);
}


void MainReDataWord(u8* burData8Bit) //MainReDataWord
{
    u16 temp;

    SET_BUR_CLK();
    TimerDelayUS(7);
    temp = Bur_DATA_PORT->IDR;
    burData8Bit[0] = (temp >> 8) & 0xFF;

    CLR_BUR_CLK();
    TimerDelayUS(7);
    temp = Bur_DATA_PORT->IDR;
    burData8Bit[1] = (temp >> 8) & 0xFF;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     Set_CS                                                                                    */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               SlaveNum  从机号码                                                                        */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               根据从机号码片选                                                                          */
// /*---------------------------------------------------------------------------------------------------------*/
void Set_CS(u16 SlaveNum)
{
    u32 tmp;
    tmp = Bur_CS_PORT->ODR & ((SlaveNum) | 0xffc0);
    GPIOA->ODR = tmp;

    TimerDelayUS(1000);

}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     Clr_CS                                                                                    */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*                                                                                                         */
/*----------------------------- 全选从机信号----------------------------------------------------------------------------*/
void SlaveBoardDisconnect(void)
{
    u16 tmp;
    tmp = GPIOA->ODR | 0x003F;
    GPIOA->ODR |= tmp;
    TimerDelayUS(2000);
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     ResetSlaveChip                                                                            */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void ResetSlaveChip(void)
{
    CLR_BUR_RST();    /* RST电平先低10us后置高，  */
    TimerDelayUS(20);
    SET_BUR_RST();

    SlaveBoardDisconnect();         /* 置0 SN74HC42N的A，B，C，D片选脚 */
    TimerDelayMS(80);
    CLR_BUR_RST();    /* 拉低RST复位脚的电平 */
    TimerDelayMS(200);
}

void MainSendData(u8 slavenum, u8* DataSrc, u16 DataLen)
{
    int i;

    Set_CS(slavenum);

    TimerDelayMS(2);

    for (i = 0; i < DataLen; i += 2)
    {
        MainSendDataWord(DataSrc + i);
    }

    SlaveBoardDisconnect();
}

//发送烧录命令          从机烧录个数     烧录命令      准备发送数据   数据长度
void SendCommandData(u8 slavenum, u32 Command_Code, u8* DataSrc, u16 DataLen)
{
    int i;
    u8 TmpBuf[0xA00];  //发送数据数组
    u16 Crc16;
    u16 DataLenTmp;

    memset_data(TmpBuf, 0, 0x300);        //清空发送数组

    //--------配置传输格式--------//

    //-------- 2Byte命令位 + 2Byte长度位(命令位+长度位+数据总长度,
    //  即 N + 6) + N位数据  + 2位CRC校验(N+6长度计算)--------//
    TmpBuf[0] = (u8)(Command_Code >> 24);
    TmpBuf[1] = (u8)(Command_Code >> 16);
    TmpBuf[2] = (u8)(Command_Code >> 8);
    TmpBuf[3] = (u8)(Command_Code);

    DataLenTmp = DataLen;
    DataLen = DataLen + 6;

    //2Byte长度位
    TmpBuf[4] = (u8)((DataLen >> 8) & 0xFF);
    TmpBuf[5] = (u8)(DataLen & 0xFF);

    for (i = 0; i < DataLenTmp; i++)
    {
        TmpBuf[i + 6] = DataSrc[i];
    }

    Crc16 = GetCrc16(TmpBuf, DataLen);

    TmpBuf[DataLen] = (u8)((Crc16 >> 8) & 0xFF);
    TmpBuf[DataLen + 1] = (u8)(Crc16 & 0xFF);
    DataLen = DataLen + 2;

    Set_CS(slavenum);

    // TimerDelayMS(1);

    for (i = 0; i < DataLen; i += 2)
    {
        MainSendDataWord(TmpBuf + i);
    }
    SlaveBoardDisconnect();

}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     Receive_Data                                                                              */
/*                                                                                                         */
/* Parameter:    SlaveNum      从机号码                                                                    */
/*               length        主机从选定从机中接收的数据长度                                              */
/*               redatabuf     主机接收到的数据存放地址指针                                                */
/* Returns:      crcFLAG       CRC标志，接收数据正确返回0，接收数据错误返回1                               */
/*                                                                                                         */
/*Description:   主机向选定从机接收长度个数据(单数个数据会多接收一位)                                      */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
u8 MainReceiveData(u8 slavenum, u8 *redatabuf)
{
    int i;
    u8 receive_status = 0xFF;
    u8 CrcCalDataLen = 0;    //计算CRC的长度
    u8 rebuffer[0x100] = {0}; //暂存接收到的数据，若CRC校验通过，则传递到输出数组
    u16 Cal_CRC_u16;         //计算的CRC
    u16 PreCRC = 0;          //从机发过来的CRC

    MainDataIn();            //切换GPIO为接收状态(输入)

    Set_CS(slavenum);        //片选芯片并开始测试

    i = 0;
    do
    {
        MainReDataWord(rebuffer + i);//获取从机值  (两个成功后后面的获取的数据都为0xFF,在第二个成功前获取的数据都为0x00)
        i += 2;
    }
    while ((i < rebuffer[0] + 2) && (rebuffer[0] != 0xFF));//总长度加两位校验位

    CrcCalDataLen = rebuffer[0];     //获得接收数据的长度
    Cal_CRC_u16 = GetCrc16(rebuffer, CrcCalDataLen);
    PreCRC = (rebuffer[CrcCalDataLen] << 8) + rebuffer[CrcCalDataLen + 1]; //获取从机的CRC值

    if ((Cal_CRC_u16 == PreCRC) && (CrcCalDataLen != 0))
    {
        memcpy_data(redatabuf, rebuffer, CrcCalDataLen + 2);  //若CRC校验通过，则将接收数据buffer传递到输出数组 rebuffer赋值给redtabuf
        receive_status = 0;  //CRC正确
    }
    else
    {
        TimerDelayUS(3000);
    }

    SlaveBoardDisconnect();      //关闭片选

    MainDataOut();    //切换GPIO为默认模式(输出/发送)

    return receive_status;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     MainReceiveSN                                                                             */
/*                                                                                                         */
/* Parameter:    SlaveNum      从机号码                                                                    */
/*                                                                                                         */
/* Returns:      SNbuf               从机真实序列号                                                        */
/*                                                                                                         */
/*Description:   主机向选定从机接收真实序号             （检测部分使用）                                   */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
u16 MainReceiveSN(u8 slavenum)
{
    int i;
    u8 CrcCalDataLen = 0;    //计算CRC的长度
    u8 rebuffer[0x100] = {0}; //暂存接收到的数据，若CRC校验通过，则传递到输出数组
    u16 SNbuf;               //读序列值
    u16 Cal_CRC_u16;         //计算的CRC
    u16 PreCRC = 0;          //从机发过来的CRC
    MainDataIn();            //切换GPIO为接收状态(输入)

    Set_CS(slavenum);        //片选芯片并开始测试

    i = 0;
    do
    {
        MainReDataWord(rebuffer + i);//获取从机值
        i += 2;
    }
    while ((i < rebuffer[0] + 2) && (rebuffer[0] != 0xFF));//总长度加两位校验位

    CrcCalDataLen = rebuffer[0];     //获得接收数据的长度
    Cal_CRC_u16 = GetCrc16(rebuffer, CrcCalDataLen);
    PreCRC = (rebuffer[CrcCalDataLen] << 8) + rebuffer[CrcCalDataLen + 1]; //获取从机的CRC值

    if ((Cal_CRC_u16 == PreCRC) && (CrcCalDataLen != 0))   //校验CRC通过返回真实序号
    {
        SNbuf = (rebuffer[3] << 8) + rebuffer[2];
    }

    SlaveBoardDisconnect();                //关闭片选

    MainDataOut();    //切换GPIO为默认模式(输出/发送)

    return SNbuf;
}

void GetReplyResult(u8 * Resultbuf)
{
    u8 slavenum;
    u8 SendDataBuf[10];
    u8 ReTmp[30];
    u8 CrcFlag[40];
    u32 SendDataLen;

    for (slavenum = 0; slavenum < 40; slavenum++)
    {
        SendDataLen = 0;
        SendCommandData(slavenum, SLAVE_RESPOND_STATUS, SendDataBuf, SendDataLen); //发送接收小板状态命令

        //TimerDelayMS(1);
        TimerDelayUS(1000);
        memset_data(ReTmp, 0, 30);   //清空数据接收buf
        CrcFlag[slavenum] = MainReceiveData(slavenum, ReTmp);           //接收小板返回的状态
        if ((CrcFlag[slavenum] == 0) && (ReTmp[1] == 0x1A))
        {
            Resultbuf[slavenum] = 0;
        }
        else
        {
            Resultbuf[slavenum] = 1;
        }
    }
}

