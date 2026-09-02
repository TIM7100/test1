#ifndef XXY_SPI_H
#define XXY_SPI_H

#include "ach512.h"

#define SPI_MASTER

//SPI模式
#define WORK_MODE_0  0
#define WORK_MODE_1  1
#define WORK_MODE_2  2
#define WORK_MODE_3  3

extern volatile UINT8 flag_spia_batchdone_int;
extern volatile UINT8 flag_spib_batchdone_int;

/*函数功能: 硬件SPI初始化函数					   */
void spi_init(UINT8 spi_index, UINT8 work_mode);

/*函数功能: SPI失能函数					 		   */
void chip_disable(UINT8 spi_index);

/*函数功能: SPI使能函数					 		   */
void chip_enable(UINT8 spi_index);

/*函数功能: SPI通用模式接收数据					   */
void spi_rx_bytes(UINT8 spi_index, UINT8 *rx_data, UINT32 len);

/*函数功能: SPI通用模式发送数据					   */
void spi_tx_bytes(UINT8 spi_index, UINT8 *tx_data, UINT32 len);

/*函数功能: SPI通用模式收发数据					   */
void spi_txrx_bytes(UINT8 spi_index, UINT8 *tx_data, UINT8 *rx_data, UINT32 len);

#endif
