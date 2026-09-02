#ifndef XXY_SOFT_SPI
#define XXY_SOFT_SPI

#include "ach512.h"
#include "Ach512Timer0Delay.h"

#define SPI_DELAY 1

//---------------------------说明---------------------------//
//SPI：MOSI：GPIO_23 MISO：GPIO_24 SCK：GPIO_25 SS：GPIO_26
#define MOSI_HIGH (REG_GPIO_SET(GPIOA) = 1<<23)					//SPI_MOSI-GPIO23 高 
#define MOSI_LOW  (REG_GPIO_CLR(GPIOA) = 1<<23)					//SPI_MOSI-GPIO23 低 

#define READ_SOFT_SPI_MISO (REG_GPIO_IDATA(GPIOA) &= (0x1<<24)) //读MISO-GPIO24

#define SCK_LOW (REG_GPIO_CLR(GPIOA) = 1<<25)						//SPI_SCK-GPIO25 低 
#define SCK_HIGH (REG_GPIO_SET(GPIOA) = 1<<25)					//SPI_SCK-GPIO25 高

#define SOFT_SPI_CS_ENBLE    (REG_GPIO_CLR(GPIOA) = 1<<26)		// 选中SPI_SS GPIO26
#define SOFT_SPI_CS_DISABLE  (REG_GPIO_SET(GPIOA) = 1<<26)		// 不选中SPI_SS GPIO26

/*函数功能: SPI接口的初始化函数                      */
void XxySoftSpiInit(void);

/*函数功能: SPI发送一个字节数据                      */
void SoftSpiWriteByte(u8 ByteData);

/*函数功能: SPI接收一个字节数据                      */
u8 SoftSpiReadByte(void);

/*函数功能: SPI连续发送数据	                     	 */
void SoftSpiSendBytes(u8 *tx_data, u32 Length);

/*函数功能: SPI连续读取数据	                     	 */
void SoftSpiReadBytes(u8 *rx_data, u32 Length);

#endif
