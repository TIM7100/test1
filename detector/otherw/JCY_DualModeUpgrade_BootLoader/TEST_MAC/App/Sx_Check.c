#include "Sx_Check.h"



u8 *Chip_Version;
u16 Data_Crc;
u8 Serial;
char Data_Day[8];
char Data_Type[9];
u8 ReadCMD_3B[0x19];
u8 ReadCMD_4A[0x104];
const u8 CmdA4CF03_101[] = {0xDA, 0x98, 0x89, 0xAD, 0x31, 0x31, 0x16};
const u8 CmdA4CF03[] = {0xA4, 0xCF, 0x03, 0x00, 0x00, 0x00, 0x00, 0x2A, 0xF9};
u8 CMD_C3B293[4] = {0xC3, 0xB2, 0x93, 0x00};
u8 DATA_C3B293[0x202];









//通过串口打印数据
void UartOutput(u8 *SendData, u32 Len, u8 NextLineFlag)
{
    u32 i;

    for (i = 0; i < Len; i++)
    {
        printf("0x");
        printf("%02X, ", SendData[i]);
    }
    if (NextLineFlag == 1)
    {
        printf("\r\n");
    }
}





void ErrorShow(void)    //检测失败
{

    UartSend("CLR(0);\r\n");
    CheckBusy();

    UartSend("DC24(10,40,'未检测到芯片',1);\r\n");
    CheckBusy();
    UartSend("BOXF(10,80,30,100,4);\r\n");
    CheckBusy();

    UartSend("BOXF(50,80,70,100,4);\r\n");
    CheckBusy();
    UartSend(CMD_DC16(8, 110, "继续", 4));
    CheckBusy();

    UartSend("BOXF(90,80,110,100,4);\r\n");
    CheckBusy();

    UartSend(CMD_DC16(48, 110, "取消", 4));
    CheckBusy();

    UartSend("BOXF(130,80,150,100,4);\r\n");
    CheckBusy();
}




void SuccessShow(void)
{
    char NameBuf[32] = {0};
    int i = 0;
    char VerBuf[32] = {0};
    int index = 0;

		
    UartSend("CLR(0);\r\n");
    CheckBusy();
    UartSend("DC16(26,10,'三星检测识别仪',2);\r\n");
    CheckBusy();


    if (((ReadCMD_3B[1] == 0x02) && (ReadCMD_3B[3] == 0x07)) || ((ReadCMD_3B[1] == 0x16) && (ReadCMD_3B[3] == 0x07) && (ReadCMD_3B[4] == 0x30) && (ReadCMD_3B[5] == 0x43) && (ReadCMD_3B[6] == 0x6F) && (ReadCMD_3B[7] == 0x70)))
    {
        UartSend("DC24(30,25,'104系列',2);\r\n");
        CheckBusy();
    }
    else if (ReadCMD_3B[1] == 0x16)
    {
        UartSend("DC24(30,25,'101系列',2);\r\n");
        CheckBusy();
    }
    else
    {
			
        UartSend("DC24(20,25,'404/111系列',2);\r\n");
        CheckBusy();




        UartSend("DC16(15,55,'版本：',4);");
        CheckBusy();
        if (ReadCMD_3B[1] == 0x06)
        {
            UartSend("DC16(60,55,'V2',1);\r\n");
            CheckBusy();
        }
        else if (ReadCMD_3B[1] == 0x07)
        {
            UartSend("DC16(60,55,'V3',1);\r\n");
            CheckBusy();;
        }
        else if (ReadCMD_3B[1] == 0x08)
        {
            UartSend("DC16(60,55,'V4',1);\r\n");
            CheckBusy();
        }





        if (ReadCMD_4A[104] == 0x00)
        {
            index = 112;
        }
        else
        {
            index = 104;
        }
        for (i = 0; i < 8; i++)
        {
            Data_Day[i] = ReadCMD_4A[5 + i] - '0';
        }

        for (i = 0; i < 9; i++)
        {
            Data_Type[i] = ReadCMD_4A[index + i];
        }
        if (Data_Type[0] == 0x00)
        {
            UartSend("DC16(15,75,'型号：',4);");
            CheckBusy();
            sprintf(VerBuf, "DC16(60,75,'NULL',1);\r\n");
            UartSend(VerBuf);
            CheckBusy();
        }
        else
        {
            UartSend("DC16(15,75,'型号：',4);");
            CheckBusy();
            sprintf(VerBuf, "DC16(60,75,'%c%c%c%c%c%c%c%c%c',1);\r\n", Data_Type[0], Data_Type[1], Data_Type[2], Data_Type[3], Data_Type[4], Data_Type[5], Data_Type[6], Data_Type[7], Data_Type[8]);
            UartSend(VerBuf);
            CheckBusy();
        }



        if (Data_Day[1] == 0)
        {
            UartSend("DC16(15,95,'日期：',4);\r\n");
            CheckBusy();
            sprintf(VerBuf, "DC16(60,95,'%d%d%d%d-%d%d-%d%d',1);\r\n", Data_Day[0], Data_Day[1], Data_Day[2], Data_Day[3], Data_Day[4], Data_Day[5], Data_Day[6], Data_Day[7]);
            UartSend(VerBuf);
            CheckBusy();

        }
        else
        {
            UartSend("DC16(15,95,'日期：',4);\r\n");
            CheckBusy();
            sprintf(VerBuf, "DC16(60,95,'20%d%d-%d%d-%d%d',1);\r\n", Data_Day[0], Data_Day[1], Data_Day[2], Data_Day[3], Data_Day[4], Data_Day[5]);
            UartSend(VerBuf);
            CheckBusy();
        }





    }










    UartSend(CMD_DC16(8, 110, "继续", 4));
    CheckBusy();

    UartSend(CMD_DC16(48, 110, "取消", 4));
    CheckBusy();


}


void Chip_Start(void)
{
    
    SCL_L();
		SDA_L();
		Delay_ms(10);
    SDA_H();
    SCL_H();

    Delay_ms(20);

    Clk_Turn(100);
}




void AutoTest(void)
{
    u8 i;
    u16 Crc;
    u16 Crc_Check = 0;



    MemsetData((char *)ReadCMD_3B, 0, 0x19);
    MemsetData((char *)ReadCMD_4A, 0, 0x104);
    
//        Chip_Start();

//        IIC_filtering_start();
//        IIC_filtering_read(0x51, ReadCMD_3B, 0x19);
//        if (ReadCMD_3B[0] == 0x3B)
//        {

//            IIC_filtering_stop(150, 1, 20, 100);
//        }
//        else
//        {


//            IIC_filtering_stop(100, 100, 0, 20);
//            IIC_Start_0();
//            IIC_filtering_read(0x51, ReadCMD_3B, 0x19);
//            IIC_filtering_stop(150, 1, 20, 100);

//        }


//        IIC_filtering_start();
//        IIC_filtering_write(0x50, CmdA4CF03, 9);
//        IIC_filtering_stop(150, 50, 20, 20);

//        //    SCL_1;
//        //    TimeDelay(45);
//        //    SCL_0;


//        IIC_filtering_start();
//        IIC_filtering_read(0x51, ReadCMD_4A, 0x104);
//        IIC_filtering_stop(150, 1, 20, 100);

//    



//    //获取数据
//    IIC_Start();
//    Delay_ms(5);
//    IIC_Read_Bytes(0x51, ReadCMD_3B, 0x19);
//    Delay_us(2);
//    IIC_Stop();

//    Delay_ms(1);


//    if (((ReadCMD_3B[1] == 0x02) && (ReadCMD_3B[3] == 0x07)) || ((ReadCMD_3B[1] == 0x16) && (ReadCMD_3B[3] == 0x07) && (ReadCMD_3B[4] == 0x30) && (ReadCMD_3B[5] == 0x43) && (ReadCMD_3B[6] == 0x6F) && (ReadCMD_3B[7] == 0x70)))
//    {
//        IIC_Receive_Array(0x50, CmdA4CF03, 9, ReadCMD_4A, 0x104);
//    }
//    else if (ReadCMD_3B[1] == 0x16)
//    {
//        IIC_Receive_Array(0x50, CmdA4CF03_101, 7, ReadCMD_4A, 0x104);
//    }
//    else
//    {
//        IIC_Receive_Array(0x50, CmdA4CF03, 9, ReadCMD_4A, 0x104);

//        Delay_ms(2);

//    }

//    Crc = CRC_DATA16_CAL(ReadCMD_4A, 0x102);
//    Crc_Check |= ReadCMD_4A[0x102] << 8;
//    Crc_Check |= ReadCMD_4A[0x103];



   // Delay_ms(2);

    if ((ReadCMD_4A[0] != 0x43) || (Crc_Check != Crc))
    {


        if (ReadCMD_3B[0] != 0x3B)
        {

            Crc = 0;
            Crc_Check = 0;
            MemsetData((char *)ReadCMD_3B, 0, 0x19);
            MemsetData((char *)ReadCMD_4A, 0, 0x104);
            Chip_Start();

            IIC_filtering_start();
            IIC_filtering_read(0x51, ReadCMD_3B, 0x19);
            if (ReadCMD_3B[0] == 0x3B)
            {

                IIC_filtering_stop(150, 1, 20, 100);
            }
            else
            {


                IIC_filtering_stop(100, 100, 0, 20);
                IIC_Start_0();
                IIC_filtering_read(0x51, ReadCMD_3B, 0x19);
                IIC_filtering_stop(150, 1, 20, 100);

            }


            IIC_filtering_start();
            IIC_filtering_write(0x50, CmdA4CF03, 9);
            IIC_filtering_stop(150, 50, 20, 20);

//    SCL_1;
//    TimeDelay(45);
//    SCL_0;


            IIC_filtering_start();
            IIC_filtering_read(0x51, ReadCMD_4A, 0x104);
            IIC_filtering_stop(150, 1, 20, 100);


            Crc = CRC_DATA16_CAL(ReadCMD_4A, 0x102);
            Crc_Check |= ReadCMD_4A[0x102] << 8;
            Crc_Check |= ReadCMD_4A[0x103];

            if ((ReadCMD_4A[0] != 0x43) || (Crc_Check != Crc))
            {
                if (ReadCMD_3B[0] != 0x3B)
                {
                    ErrorShow();
                    return;
                }
                //显示系列
                SuccessShow();
            }
        }

        //显示系列

        SuccessShow();

    }
    else
    {
        SuccessShow();
    }

}


















