#include "filtering_iic.h"
#include "delay.h"

//GPIO_InitTypeDef GPIO_InitStruct;
//UART_InitTypeDef UART1_InitStruc;

/************************************************************************
 * function   : 校验
 * Description: 打印调试
 ************************************************************************/
//u16 CRC_DATA16_CAL(u8 * data, u32 Length)
//{
//    u16 CRC_DATA = 0x6363;
//    u16 R1, R2, R3;
//    u32 i;

//    for (i = 0; i < Length; i = i + 1)
//    {
//        R1 = (u16)(*(data + i));
//        R2 = (u16)(CRC_DATA & 0xFF);
//        R1 = (u16)(R1 ^ R2);
//        R2 = R1;
//        R2 = (u16)(R2 << 4);
//        R2 = (u16)((R2 ^ R1) & 0xFF);
//        R1 = (u16)(R2 << 3);

//        R3 = (u16)(R2 >> 4);
//        R1 = (u16)(R1 ^ R3);

//        R3 = (u16)((CRC_DATA >> 8) & 0xFF);
//        R1 = (u16)(R1 ^ R3);

//        R2 = (u16)(R2 << 8);
//        CRC_DATA = (u16)(R1 ^ R2);
//    }
//    return CRC_DATA;
//}

/************************************************************************
 * function   : dayin
 * Description: 打印
 ************************************************************************/
void printf_data(u8* des, u16 len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        printf("%02X ", des[i]);
        if (des[i] == 0x90)
        {

            if (des[i + 1] == 0x00)
            {
                printf("%02x ", des[i + 1]);
                printf("%02X ", des[i + 2]);
                printf("%02X ", des[i + 3]);
                break;
            }
            else
            {
                continue;
            }
        }
    }
    printf("\r");
}

/************************************************************************
 * function   : soft_iic_init
 * Description: 端口初始化
 ************************************************************************/
void soft_iic_init()
{
    GPIO_InitTypeDef GPIO_InitStruct;
    //TIM6_Init();

    GPIO_InitStruct.Pin       = GPIO_PIN_8 ;
    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Alternate = GPIO_FUNCTION_0 ;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin       = GPIO_PIN_9;
    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull     =  GPIO_PULLUP ;
    GPIO_InitStruct.Alternate = GPIO_FUNCTION_0 ;
    GPIO_Init(GPIOB, &GPIO_InitStruct);


#if I2C_POWER_STATUS
    GPIO_InitStruct.Pin       = I2C_POWER_PIN | I2C_POWER_PIN1;
    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Alternate = GPIO_FUNCTION_0;
    GPIO_Init(I2C_POWER_PORT, &GPIO_InitStruct);
    GPIO_Init(I2C_POWER_PORT1, &GPIO_InitStruct);


#endif
    I2C_POWER_ON();
    I2C_POWER_OFF1();

    SCL_1;
    Delay_ms(6);
    SDA_1;
    Delay_ms(24);

}




void IIC_Start_0(void)
{
    IIC_SDA_OUTPUT();       //SDA输出

    SDA_1;            //SDA=1,准备数据
    TimeDelay(20);
    SCL_1;            //SCL=1,拉高时钟线通知读取
    TimeDelay(20);
    SDA_0;            //SDA=0，SCL高电平期间，拉低SDA，产生起始信号
    Delay_us(20);
    SCL_0;
}


/*****************************************************************************
*function   : soft_iic_start
*Descrii    : iic的起始时序信号
******************************************************************************/
void IIC_filtering_start()
{
    SDA_OUTPUT();
    SCL_1;
    TimeDelay(25);
    SCL_0;
    TimeDelay(5);
    SCL_1;
    SDA_0;
    TimeDelay(10);
    SCL_0;
    TimeDelay(10);
}

/*****************************************************************************
*function   : soft_iic_stop
*Descrii    : iic的起始时序信号
******************************************************************************/
void IIC_filtering_stop(int del1, int del2, int del3, int turn_num)
{
        SDA_OUTPUT();
        SDA_0;

        SCL_1;
        TimeDelay(del1);
        SDA_1;
				if(del2 < 60)
				{
						Delay_ms(del2);
				}
        else
				{
						TimeDelay(del2);
				}
        SCL_0;
        TimeDelay(del3);

        Clk_Turn(turn_num);
}
/*****************************************************************************
*function   : soft_iic_write
*Descrii    : iic的发送字节
******************************************************************************/
uint8_t IIC_write_byte(u8 byte)
{
    UINT8 i;

    SDA_OUTPUT();
    SCL_0;
    TimeDelay(10);
    for (i = 0; i < 8; i++)
    {
        if (byte & 0x80)
        {
            SDA_1;
        }
        else
        {
            SDA_0;
        }


        TimeDelay(120);
        SCL_1;
        byte <<= 1;
        TimeDelay(140);
        SCL_0;

    }
    SDA_0;
    if (IIC_r_ac())
    {
        return 1;    //没接收到应答
    }
    return 0;     //接收到应答
}
/*****************************************************************************
*function   : soft_iic_read
*Descrii    : iic的读时序信号
******************************************************************************/
UINT8 IIC_read_byte(uint8_t uAck)
{
    UINT8 i, byte = 0x00;
    SDA_INPUT();

    TimeDelay(100);
    for (i = 0; i < 8; i++)
    {

        //Delay_us(1);
        SCL_1;
        byte = byte << 1;
        if (R_SDA)
        {
            byte |= 0x01;
        }

        TimeDelay(100);
        SCL_0;
        TimeDelay(130);
    }

    //IIC_w_ac(uAck);
    if (uAck == 1) //ack=1时，发送ack，ack=0时，发送nack
    {
        IIC_w_ac(0);
    }
    else
    {
        IIC_w_ac(1);
    }


    return byte;
}


/*****************************************************************************
*function   : soft_iic_w_ac
*Descrii    : 主机写应答
******************************************************************************/
void IIC_w_ac(UINT8 bit)
{

    SDA_OUTPUT();

    SCL_0;

    if (bit)
    {
        SDA_0;

    }
    else
    {
        SDA_1;

    }

    SCL_1;
    Delay_us(7);
    SCL_0;
    Delay_us(1);

    SCL_1;
    Delay_us(1);
    SCL_0;

}





/*****************************************************************************
*function   : IIC_r_ac
*Descrii    : 接受从机应答
******************************************************************************/
UINT8 IIC_r_ac(void)
{
    u8 ack;
    SDA_INPUT();
    TimeDelay(80);
    SCL_1;
    ack = SDA_READ();
    TimeDelay(110);
    SCL_0;

    TimeDelay(10);
    SDA_OUTPUT();
    SCL_1;
    TimeDelay(10);
    SCL_0;

    //

    return ack;
}
/*****************************************************************************
*function   : 充电
*Descrii    : 翻转充电
******************************************************************************/
void FreeTime(u32 CountNum)
{
    u32 i;
    for (i = 0; i < CountNum; i++)
    {
        SCL_1;
        TimeDelay(58);
        SCL_0;
        TimeDelay(28); //低电平延时

    }
    SCL_1;

}
/*****************************************************************************
*function   : 上电时间
*Descrii    : 系统初始化
******************************************************************************/
void FreeTime1(u32 CountNum)
{
    u32 i;
    SCL_1;
    TimeDelay(120);
    SCL_0;

    SDA_1;
    TimeDelay(127);//高电平1us延时
    SDA_0;
    TimeDelay(25); //低电平延时1us
    for (i = 0; i < CountNum; i++)
    {
        SDA_1;
        TimeDelay(30);//高电平1us延时
        SDA_0;
        TimeDelay(20); //低电平延时1us
    }

    SDA_1;
    SCL_1;
}
/*****************************************************************************
*function   : IIC写字节
*Descrii    : IIC写字节
******************************************************************************/

void IIC_filtering_write(UINT8 slave_addr,  const uint8_t *txdata, UINT8 datalen)
{
    u8 ACK, i;
    //IIC_filtering_start();

    ACK = IIC_write_byte(slave_addr);


	
    if (ACK  == 0)
    {
        for (i = 0; i < datalen; i++)
        {
           ACK = IIC_write_byte(txdata[i]);

        }
        Clk_Turn(50);
        SDA_OUT;

    }


}
/*****************************************************************************
*function   : IIC读字节(带字长)
*Descrii    : IIC读字节
******************************************************************************/
void IIC_filtering_read(UINT8 slave_addr, UINT8 *txdata, UINT16 datalen)
{
    UINT16 i, ACK, M_Ack;
    M_Ack = IIC_ACK;

    ACK = IIC_write_byte(slave_addr | 0x01);

    if (ACK  == 0)
    {
        for (i = 0; i < datalen; i++)//主机接收数据后判断发送应答or非应答
        {
            if ((i == (datalen - 1)) && (txdata[0] == 0xA4))
            {
                M_Ack = IIC_NACK;
            }

            txdata[i] = IIC_read_byte(M_Ack);

        }

        Clk_Turn(50);

    }
    else
    {
        Clk_Turn(50);
    }



}

void Clk_Turn(UINT16 len)
{
    while (len--)
    {

        SCL_1;
        TimeDelay(40);
        SCL_0;
        TimeDelay(40);
    }

}


void Data_Turn(UINT16 len)
{
    SDA_OUT;

    while (len--)
    {

        SDA_0;
        Delay_us(2);
        SDA_1;
        Delay_us(2);
    }


}
