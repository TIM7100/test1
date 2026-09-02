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

//UINT8 tx_data[512];
UINT8 rx_data[512];

void spis_comm_test(void)
{

	UINT32 i;
	
	printfS("---------SPIS comm test is start!-------\r\n");
	spi_init(SPIB, WORK_MODE_0);

	REG_SPI_TX_CTL(SPIB) = (REG_SPI_TX_CTL(SPIB) & ~0xff00) | 0x55 << 8;

	if (REG_SPI_CS(SPIB) == 0x00)
	{
		while(REG_SPI_CS(SPIB) == 0x00);
	}
    // 从机不存在分频问题

	while (1)
	{
		for (i = 0; i < 4; i++)
		{

			while (REG_SPI_CS(SPIB) == 0x00);//1x状态
			REG_SPI_CTL(SPIB) = 0 << 5 | 0 << 4 | i << 2 | 0 << 0;
			REG_SPI_OUT_EN(SPIB) = 0x00;
			spi_rx_bytes(SPIB, rx_data, 256);
			REG_SPI_OUT_EN(SPIB) = 0x02;
			spi_tx_bytes(SPIB, rx_data, 256);

		    while (REG_SPI_CS(SPIB) == 0x00);//2x状态
			REG_SPI_CTL(SPIB) = 1 << 5 | 0 << 4 | i << 2 | 0 << 0;
			REG_SPI_OUT_EN(SPIB) = 0x00;
			spi_rx_bytes(SPIB, rx_data, 256);
			REG_SPI_OUT_EN(SPIB) = 0x03;
			spi_tx_bytes(SPIB, rx_data, 256);


			while (REG_SPI_CS(SPIB) == 0x00);//4x状态
			REG_SPI_CTL(SPIB) = 2 << 5 | 0 << 4 | i << 2 | 0 << 0;
			REG_SPI_OUT_EN(SPIB) = 0x00;
			spi_rx_bytes(SPIB, rx_data, 256);
			REG_SPI_OUT_EN(SPIB) = 0x0f;
			spi_tx_bytes(SPIB, rx_data, 256);
		}
	}
}


