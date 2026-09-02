#include "XxySoftSpi.h"

/*函数名: XxySoftSpiInit                             */
/*函数功能: SPI接口的初始化函数                      */
/*返回值: 无                                         */
/*参数  : 无                                         */
void XxySoftSpiInit(void)
{
    REG_SCU_MUXCTRLB |= ~(0xFF << 14);      //设置管脚复用为GPIO23 24 25 26
    REG_GPIO_DIR(GPIOA) |= 0x6800000;       //设置管脚23 25 26为输出管脚
    REG_GPIO_DIR(GPIOA) &= ~(0x1000000);    //设置管脚24为输入管脚

    SOFT_SPI_CS_DISABLE;                    //SPI FLASH不选中
    SCK_HIGH;
    MOSI_HIGH;
}

/*函数名: SoftSpiWriteByte                           */
/*函数功能: SPI发送一个字节数据                      */
/*返回值: 无                                         */
/*参数  :                                            */
/*ByteData     输入参数,SPI将要发送的一个字节数据    */
void SoftSpiWriteByte(u8 ByteData)
{
    u8 BitCount;
    for (BitCount = 0; BitCount < 8; BitCount++)
    {
        SCK_LOW;                        //CLK拉低
//      Timer0DelayUs(SPI_DELAY);
        if (ByteData & 0x80)
        {
            MOSI_HIGH;                  //MOSI拉高
        }
        else
        {
            MOSI_LOW;                   //MOSI拉低
        }
        ByteData = ByteData << 1;       //发送数据左移一位
//      Timer0DelayUs(SPI_DELAY);
        SCK_HIGH;                       //CLK拉高
//      Timer0DelayUs(SPI_DELAY);
    }
}

/*函数名: SoftSpiReadByte                            */
/*函数功能: SPI接收一个字节数据                      */
/*返回值: 无                                         */
/*RecvByte    返回值,SPI接收到的一个字节数据         */
/*参数  : 无                                         */
u8 SoftSpiReadByte(void)
{
    u8 BitCount = 0;
    u8 RecvByte = 0;

    for (BitCount = 0; BitCount < 8; BitCount++)
    {
        SCK_LOW;                        //CLK拉低
//      Timer0DelayUs(SPI_DELAY);
        RecvByte = RecvByte << 1;       //读的时候 高位在前
        if (READ_SOFT_SPI_MISO)
        {
            RecvByte |= 0x01;
        }
//      Timer0DelayUs(SPI_DELAY);
        SCK_HIGH;                       //CLK拉高
//      Timer0DelayUs(SPI_DELAY);
    }
    return RecvByte;
}

/*函数名: SoftSpiSendBytes                           */
/*函数功能: SPI连续发送数据                          */
/*返回值: 无                                         */
/*参数  :                                            */
/**rx_data   输入参数,指向发送数据数组指针           */
/*Length     输入参数,发送数据的长度                 */
void SoftSpiSendBytes(u8 *tx_data, u32 Length)
{
    int i = 0;

    for (i = 0; i < Length; i++)
    {
        SoftSpiWriteByte(tx_data[i]);
    }
}

/*函数名: SoftSpiReadBytes                           */
/*函数功能: SPI连续读取数据                          */
/*返回值: 无                                         */
/*参数  :                                            */
/**tx_data   输入参数,指向读取数据的数组指针         */
/*Length    输入参数,读取数据的长度                  */
void SoftSpiReadBytes(u8 *rx_data, u32 Length)
{
    int i = 0;

    for (i = 0; i < Length; i++)
    {
        rx_data[i] = SoftSpiReadByte();
    }
}
