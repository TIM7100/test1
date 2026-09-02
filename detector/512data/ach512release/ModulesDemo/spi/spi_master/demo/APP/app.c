/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : app.c
 * Description : application example source file
 * Author(s)   : Eric
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#include  "app.h"
#include  "spi.h"


UINT8 tx_data[512];
UINT8 rx_data[512];



void spim_comm_test(void)
{
	UINT8 result;
	UINT32 i;

	printfS("---------SPIM comm test is start!-------\r\n");
	spi_init(SPIA, WORK_MODE_0);

	for (i = 0; i < 512; i++)
	{
		tx_data[i] = (UINT8)i;
		//tx_data[i] = 0xaa;
	}

	while (1)
	{
		for (i = 0; i < 4; i++)
		{
			REG_SPI_CTL(SPIA) = 0 << 5 | 0 << 4 | i << 2 | 1 << 0;
			REG_SPI_OUT_EN(SPIA) = 0x01;
			spi_tx_bytes(SPIA, tx_data, 256);
			chip_disable(SPIA);
			delay(1000000);
			REG_SPI_OUT_EN(SPIA) = 0x00;
			spi_rx_bytes(SPIA, rx_data, 257);
			chip_disable(SPIA);
			delay(1000000);
			result = memcmp(tx_data, rx_data + 1, 256);
			if (result)
			{
				printfS("spi 1x test fail \n");
				printfB8(rx_data,256);
				return;
			}
			else
			{
				printfS("spi 1x test success \n");
				//return;
			}

			REG_SPI_CTL(SPIA) = 1 << 5 | 0 << 4 | i << 2 | 1 << 0;
			REG_SPI_OUT_EN(SPIA) = 0x03;
			spi_tx_bytes(SPIA, tx_data, 256);
			chip_disable(SPIA);
			delay(1000000);
			REG_SPI_OUT_EN(SPIA) = 0x00;
			spi_rx_bytes(SPIA, rx_data, 257);
			chip_disable(SPIA);
			delay(1000000);
			result = memcmp(tx_data, rx_data + 1, 256);
			if (result)
			{
				printfS("spi 2x test fail \n");
				printfB8(rx_data,256);
				return;
			}
			else
			{
				printfS("spi 2x test success \n");
				//return;
			}

			REG_SPI_CTL(SPIA) = 2 << 5 | 0 << 4 | i << 2 | 1 << 0;
			REG_SPI_OUT_EN(SPIA) = 0x0f;
			spi_tx_bytes(SPIA, tx_data, 256);
			chip_disable(SPIA);
			delay(1000000);
			REG_SPI_OUT_EN(SPIA) = 0x00;
			spi_rx_bytes(SPIA, rx_data, 258);
			chip_disable(SPIA);
			delay(1000000);
			result = memcmp(tx_data, rx_data + 2, 256);
			if (result)
			{
				printfS("spi 4x test fail \n");
			    printfB8(rx_data,256);
				return;
			}
			else
			{
				printfS("spi 4x test success \n");
				//return;
			}
		}
	}
}




