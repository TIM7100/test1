#ifndef _BSP_SOFT_SPI_H_
#define _BSP_SOFT_SPI_H_

#include "fxx_std.h"

void SPI_GPIO_Config(void);
//void SPI_SendByte(u8 Data);
//u8 SPI_ReadByte(void);
u8 SPI_ReadWriteByte(u8 Data);

#endif
