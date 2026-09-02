#include "bsp_soft_spi.h"

#define SPI_CS_Select()             GPIO_ResetBits(GPIOB, GPIO_PIN_12)
#define SPI_CS_Release()            GPIO_SetBits(GPIOB, GPIO_PIN_12)

#define SPI_CLK_HIGH()              GPIO_SetBits(GPIOB, GPIO_PIN_13)
#define SPI_CLK_LOW()               GPIO_ResetBits(GPIOB, GPIO_PIN_13)

#define SPI_MOSI_HIGH()             GPIO_SetBits(GPIOB, GPIO_PIN_15)
#define SPI_MOSI_LOW()              GPIO_ResetBits(GPIOB, GPIO_PIN_15)

#define SPI_MISO()                  GPIO_ReadInputDataBit(GPIOB, GPIO_PIN_14)

#define DELAY_US    5

void SPI_GPIO_Config(void)
{
    GPIO_InitTypeDef SPI_GpioInit;

    /* SPI2 CS   PortB Pin12 */
    /* SPI2 CLK  PortB Pin13 */
    /* SPI2 MISO PortB Pin14 */
    /* SPI2 MOSI PortB Pin15 */
    SPI_GpioInit.Pin            = GPIO_PIN_12;
    SPI_GpioInit.Mode           = GPIO_MODE_OUTPUT_PP;
    SPI_GpioInit.Pull           = GPIO_PULLUP;
    SPI_GpioInit.Alternate      = GPIO_FUNCTION_0;
    GPIO_Init(GPIOB, &SPI_GpioInit);

    SPI_GpioInit.Pin            = GPIO_PIN_13;
    GPIO_Init(GPIOB, &SPI_GpioInit);

    /*MISO GPIO Config*/
    SPI_GpioInit.Pin            = GPIO_PIN_14;
    SPI_GpioInit.Mode           = GPIO_MODE_INPUT;
    SPI_GpioInit.Pull           = GPIO_PULLUP;
    GPIO_Init(GPIOB, &SPI_GpioInit);

    /*MOSI GPIO Config*/
    SPI_GpioInit.Pin            = GPIO_PIN_15;
    SPI_GpioInit.Mode           = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(GPIOB, &SPI_GpioInit);

    SPI_CS_Release();
    GPIO_SetBits(GPIOB, GPIO_PIN_13);
}


static void SPI_Delay(__IO uint32_t count)
{
    uint32_t i;
    for (i = 0; i < count; i++)
    {
        uint8_t uc = 12;
        while (uc--);
    }
}


//void SPI_SendByte(u8 Data)
//{
//  u8 Cnt;
//
//  for (Cnt = 0; Cnt < 8; Cnt++)
//  {
//      SPI_CS_Select();//拉低CLK
//      SPI_Delay(DELAY_US);//这个延时时间任意，但要大于芯片数据手册上的(纳秒级的)
//      if (Data & 0x80)
//      {
//          SPI_MOSI_HIGH();
//      }
//      else
//      {
//          SPI_MOSI_LOW();
//      }
//      Data <<= 1;
//      SPI_Delay(DELAY_US);
//      SPI_CLK_HIGH();//拉高CLK
//      SPI_Delay(DELAY_US);
//  }
//}

//u8 SPI_ReadByte(void)
//{
//  u8 i = 0;
//  u8 Value=0;
//
//  for (i = 0; i < 8; i++)
//  {
//      SPI_CLK_LOW();
//      SPI_Delay(DELAY_US);
//
//      SPI_CLK_HIGH();
//      SPI_Delay(DELAY_US);
//        Value <<= 1;
//      if(SPI_MISO())
//      {
//          Value |= 0x01;
//      }
//  }
//  return Value;
//}

u8 SPI_ReadWriteByte(u8 Data)
{
    u8 i = 0;
    u8 Value = 0;

    for (i = 0; i < 8; i++)
    {
        SPI_CLK_LOW();
        SPI_Delay(DELAY_US);

        if (Data & 0x80)
        {
            SPI_MOSI_HIGH();
        }
        else
        {
            SPI_MOSI_LOW();
        }
        Data <<= 1;

        SPI_Delay(DELAY_US);
        SPI_CLK_HIGH();

        Value <<= 1;
        if (SPI_MISO())
        {
            Value |= 0x01;
        }
        SPI_Delay(DELAY_US);
    }
    return Value;
}
