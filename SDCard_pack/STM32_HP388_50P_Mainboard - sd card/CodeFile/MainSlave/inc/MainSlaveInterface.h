#ifndef MAIN_SLAVE_INTERFACE_H
#define MAIN_SLAVE_INTERFACE_H

#include "stm32f10x_gpio.h"


#define Bur_DATA0       GPIO_Pin_8   //8
#define Bur_DATA1       GPIO_Pin_9   //9
#define Bur_DATA2       GPIO_Pin_10  //10
#define Bur_DATA3       GPIO_Pin_11  //11
#define Bur_DATA4       GPIO_Pin_12  //12
#define Bur_DATA5       GPIO_Pin_13  //13
#define Bur_DATA6       GPIO_Pin_14  //14
#define Bur_DATA7       GPIO_Pin_15  //15

#define Bur_RST       GPIO_Pin_0     //7
#define Bur_CLK       GPIO_Pin_1     //6

#define Bur_CSALL     GPIO_Pin_6			//PC4
#define Bur_CS0       GPIO_Pin_0     //12
#define Bur_CS1       GPIO_Pin_1     //12
#define Bur_CS2       GPIO_Pin_2     //12
#define Bur_CS3       GPIO_Pin_3     //12
#define Bur_CS4       GPIO_Pin_4     //12
#define Bur_CS5       GPIO_Pin_5     //12


#define Bur_DATA_PORT         GPIOE
#define Bur_RST_PORT          GPIOB
#define Bur_CLK_PORT          GPIOB
#define Bur_CS_PORT           GPIOA
#define Bur_CSALL_PORT        GPIOC



#define SET_BUR_CLK()          Bur_CLK_PORT->BSRR = Bur_CLK
#define CLR_BUR_CLK()          Bur_CLK_PORT->BRR  = Bur_CLK

#define SET_BUR_RST()          Bur_RST_PORT->BSRR = Bur_RST
#define CLR_BUR_RST()          Bur_RST_PORT->BRR  = Bur_RST


/* only for test */
#define CMD_Test05 0x00550055
#define CMD_Test55 0x55555555
#define CMD_Test0A 0x00AA00AA
#define CMD_TestAA 0xAAAAAAAA

#define RECVCRCOK   0
#define RECVCRCERR  1


enum StatusEnum
{
    Default = 0x55,

    EverythingOk,
    ErrorExist,

    /* receive data part */
    ReceiveOk,
    ReceiveError,

    /* function burEraseChip */
    EraseFlashOk,
    EraseFlashError,

    ProgramDataOk,
    /* function burProgramSeries101 */
    ProgramSeries101Error0,
    ProgramSeries101Error1,
    ProgramSeries101Error2,
    ProgramSeries101Error3,

    /* function burFlashDataSeries101 */
    FlashDataSeries101Error0,
    FlashDataSeries101Error1,
    FlashDataSeries101Error2,
    FlashDataSeries101Error3,
    FlashDataSeries101Error4,
    FlashDataSeries101Error5,
    FlashDataSeries101Error6,
    FlashDataSeries101Error7,
    FlashDataSeries101Error8,

    /* function CheckCrcConfig */
    CheckCrcOk,
    CheckCrcError,

    /* function SecurityCodeConfig */
    SecurityCodeOk,
    SecurityCodeError,

    /* function ClearSecurity */
    ClearSecurityOk,
    ClearSecurityError,

    SlaveBoardBusy,

    /* test part */
    TestOk,
    TestError0,
    TestError1,
    TestError2,
    TestError3,
};


extern u16 MainSendDataLen;      //主机发送给从机的数据长度，包括CRC的长度，4Byte命令位 + 2Byte长度位(命令位+长度位+数据总长度)


void MainSlaveGpioInit(void);
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
void MainDataOut(void);

void MainDataIn(void);

void MainSendDataByte(u8 burData8Bit);

void MainSendDataWord(u8 *p_burData8Bit);

void MainReDataWord(u8* burData8Bit);

void Set_CS(u16 SlaveNum);

void SlaveBoardDisconnect(void);

void ResetSlaveChip(void);

void MainSendData(u8 slavenum, u8* DataSrc, u16 DataLen);

void SendCommandData(u8 slavenum, u32 Command_Code, u8* DataSrc, u16 DataLen);

void Speak_init(void);

void Speak(u8 SpeakCount);

void SetHighVoltage(void);

void SetLowVoltage(void);

u8 MainReceiveData(u8 slavenum, u8 *redatabuf);


u16 MainReceiveSN(u8 slavenum );

void GetReplyResult(u8 * Resultbuf);

#endif
