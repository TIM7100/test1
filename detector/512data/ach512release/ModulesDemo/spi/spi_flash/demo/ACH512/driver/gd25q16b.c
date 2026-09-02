/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : gd25q16b.c
 * Description : gd25q16b driver source file
 * Author(s)   : Eric
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#include "gd25q16b.h"
#include "spi.h"

static UINT8 CmdBuffer[16];

void spi_io_config(UINT8 spi_index, UINT8 x_mode, UINT8 enout)
{
    x_mode &= 0x03;
    enout &= 0x0f;
    REG_SPI_CTL(spi_index) = (REG_SPI_CTL(spi_index) & ~(0x03 << 5)) | (x_mode << 5);
    REG_SPI_OUT_EN(spi_index) =  enout;
}

UINT16 read_id(void)
{
    UINT8 id[2];

    CmdBuffer[0] = READ_ID;
    CmdBuffer[1] = 0;
    CmdBuffer[2] = 0;
    CmdBuffer[3] = 0;

    spi_tx_bytes(SPI_MEM_COM, CmdBuffer, 4);

    spi_rx_bytes(SPI_MEM_COM, id, 2);
    chip_disable(SPI_MEM_COM);

    return 	(id[0] << 8 | id[1]);
}

UINT8 read_status_low(void)
{
    CmdBuffer[0] = READ_STATUS_L;

    spi_tx_bytes(SPI_MEM_COM, CmdBuffer, 1);
    spi_rx_bytes(SPI_MEM_COM, CmdBuffer, 1);

    chip_disable(SPI_MEM_COM);
    return CmdBuffer[0];
}

UINT8 read_status_high(void)
{
    CmdBuffer[0] = READ_STATUS_H;

    spi_tx_bytes(SPI_MEM_COM, CmdBuffer, 1);
    spi_rx_bytes(SPI_MEM_COM, CmdBuffer, 1);

    chip_disable(SPI_MEM_COM);
    return CmdBuffer[0];
}

UINT16 read_whole_status(void)
{
    volatile UINT8 temp_status[2];

    temp_status[0] = read_status_high();
    temp_status[1] = read_status_low();

    chip_disable(SPI_MEM_COM);
    return ((temp_status[0] << 8) | temp_status[1]);
}

void write_enable(void)
{
    CmdBuffer[0] = WRITE_EN;
    spi_tx_bytes(SPI_MEM_COM, CmdBuffer, 1);

    chip_disable(SPI_MEM_COM);
}

void wait_flash_ready(void)
{
    while(read_status_low() & 0x01);
}


void write_status(UINT16 status)
{
    CmdBuffer[0] = WRITE_STATUS;
    CmdBuffer[1] = (status & 0xff);
    CmdBuffer[2] = ((status >> 8) & 0xff);

    spi_tx_bytes(SPI_MEM_COM, CmdBuffer, 3);
    chip_disable(SPI_MEM_COM);

    wait_flash_ready();
}

void flash_qe_set(void)
{
    UINT16 temp_status;
    write_enable();
    temp_status = read_whole_status();
    temp_status |= (0x01 << 9);
    write_status(temp_status);

}

void flash_qe_clear(void)
{
    UINT16 temp_status;
    write_enable();
    temp_status = read_whole_status();
    temp_status &= ~(0x01 << 9);
    write_status(temp_status);
}

/***********************************************************************/
void flash_erase_sector(UINT32 addr)
{
    write_enable();
    CmdBuffer[0] = ERASE_SECTOR; //4K
    CmdBuffer[1] = ((addr >> 16) & (0xff));
    CmdBuffer[2] = ((addr >> 8) & (0xff));
    CmdBuffer[3] = ((addr)&(0xff));
    spi_tx_bytes(SPI_MEM_COM, CmdBuffer, 4);
    chip_disable(SPI_MEM_COM);

    wait_flash_ready();
    chip_disable(SPI_MEM_COM);
}

void flash_page_program(UINT8 *pBuf, UINT32 addr, UINT32 lenth)
{
    write_enable();

    CmdBuffer[0] = PAGE_PROGRAM;
    CmdBuffer[1] = ((addr >> 16) & (0xff));
    CmdBuffer[2] = ((addr >> 8) & (0xff));
    CmdBuffer[3] = ((addr)&(0xff));

    spi_tx_bytes(SPI_MEM_COM, CmdBuffer, 4);
    spi_tx_bytes(SPI_MEM_COM, pBuf, lenth);
    chip_disable(SPI_MEM_COM);

    wait_flash_ready();
    chip_disable(SPI_MEM_COM);

}

void flash_quad_page_program(UINT8 *pbuf, UINT32 page_addr, UINT32 lenth)
{
    flash_qe_set();

    write_enable();
    CmdBuffer[0] = QUAD_PAGE_PROGRAM;
    CmdBuffer[1] = (page_addr >> 16) & 0xff;
    CmdBuffer[2] = (page_addr >> 8) & 0xff;
    CmdBuffer[3] = (page_addr)&0xff;
    spi_tx_bytes(SPI_MEM_COM, CmdBuffer, 4);

    spi_io_config(SPI_MEM_COM, 2, 0x0f);
    spi_tx_bytes(SPI_MEM_COM, pbuf, lenth);
    chip_disable(SPI_MEM_COM);


    spi_io_config(SPI_MEM_COM, 0, 0x01);
    wait_flash_ready();

    flash_qe_clear();

}

/****************************************************************/
void flash_read(UINT8 *pbuf, UINT32 addr, UINT32 length)
{
    CmdBuffer[0] = READ;
    CmdBuffer[1] = (UINT8)(addr >> 16);
    CmdBuffer[2] = (UINT8)(addr >> 8);
    CmdBuffer[3] = (UINT8)(addr);
    spi_tx_bytes(SPI_MEM_COM, CmdBuffer, 4);
    spi_rx_bytes(SPI_MEM_COM, pbuf, length);
    chip_disable(SPI_MEM_COM);
}

void flash_read_fast(UINT8 *pbuf, UINT32 addr, UINT32 length)
{

    CmdBuffer[0] = READ_FAST;
    CmdBuffer[1] = (UINT8)(addr >> 16);
    CmdBuffer[2] = (UINT8)(addr >> 8);
    CmdBuffer[3] = (UINT8)(addr);
    spi_tx_bytes(SPI_MEM_COM, CmdBuffer, 4);
    spi_rx_bytes(SPI_MEM_COM, pbuf, 1);
    spi_rx_bytes(SPI_MEM_COM, pbuf, length);
    chip_disable(SPI_MEM_COM);
}

void dual_output_fast_read(UINT8 *pbuf, UINT32 addr, UINT32 length)
{
    CmdBuffer[0] = DUAL_OUT_FAST_READ;
    CmdBuffer[1] = (UINT8)(addr >> 16);
    CmdBuffer[2] = (UINT8)(addr >> 8);
    CmdBuffer[3] = (UINT8)(addr);
    spi_tx_bytes(SPI_MEM_COM, CmdBuffer, 4);


    spi_io_config(SPI_MEM_COM, 1, 0x00);

    spi_rx_bytes(SPI_MEM_COM, pbuf, 2);
    spi_rx_bytes(SPI_MEM_COM, pbuf, length);

    chip_disable(SPI_MEM_COM);

    spi_io_config(SPI_MEM_COM, 0, 0x01);
}

void dual_io_fast_read(UINT8 *pbuf, UINT32 addr, UINT32 length)
{
    CmdBuffer[0] = HIGH_PER_MODE;
    CmdBuffer[1] = (UINT8)(addr >> 16);
    CmdBuffer[2] = (UINT8)(addr >> 8);
    CmdBuffer[3] = (UINT8)(addr);
    spi_tx_bytes(SPI_MEM_COM, CmdBuffer, 4);
    chip_disable(SPI_MEM_COM);

    CmdBuffer[0] = DUAL_IO_FAST_READ;
    spi_tx_bytes(SPI_MEM_COM, CmdBuffer, 1);

    spi_io_config(SPI_MEM_COM, 1, 0x03);

    CmdBuffer[0] = (UINT8)(addr >> 16);
    CmdBuffer[1] = (UINT8)(addr >> 8);
    CmdBuffer[2] = (UINT8)(addr);
    CmdBuffer[3] = 0x05;
    spi_tx_bytes(SPI_MEM_COM, CmdBuffer, 4);

    spi_io_config(SPI_MEM_COM, 1, 0x00);

    spi_rx_bytes(SPI_MEM_COM, pbuf, length);

    chip_disable(SPI_MEM_COM);

    spi_io_config(SPI_MEM_COM, 0, 0x01);
}

void quad_output_fast_read(UINT8 *pbuf, UINT32 addr, UINT32 length)
{
    flash_qe_set();
    CmdBuffer[0] = QUAD_OUT_FAST_READ;
    CmdBuffer[1] = (UINT8)(addr >> 16);
    CmdBuffer[2] = (UINT8)(addr >> 8);
    CmdBuffer[3] = (UINT8)(addr);
    spi_tx_bytes(SPI_MEM_COM, CmdBuffer, 4);
    spi_io_config(SPI_MEM_COM, 2, 0x00);

    spi_rx_bytes(SPI_MEM_COM, pbuf, 4);
    spi_rx_bytes(SPI_MEM_COM, pbuf, length);

    chip_disable(SPI_MEM_COM);
    spi_io_config(SPI_MEM_COM, 0, 0x01);

    flash_qe_clear();
}



void quad_io_fast_read(UINT8 *pbuf, UINT32 addr, UINT32 length)
{
    flash_qe_set();

    CmdBuffer[0] = HIGH_PER_MODE;
    CmdBuffer[1] = (UINT8)(addr >> 16);
    CmdBuffer[2] = (UINT8)(addr >> 8);
    CmdBuffer[3] = (UINT8)(addr);
    spi_tx_bytes(SPI_MEM_COM, CmdBuffer, 4);
    chip_disable(SPI_MEM_COM);

    CmdBuffer[0] = QUAD_IO_FAST_READ;
    spi_tx_bytes(SPI_MEM_COM, CmdBuffer, 1);

    spi_io_config(SPI_MEM_COM, 2, 0x0f);

    CmdBuffer[0] = (UINT8)(addr >> 16);
    CmdBuffer[1] = (UINT8)(addr >> 8);
    CmdBuffer[2] = (UINT8)(addr);
    CmdBuffer[3] = 0x0c;
    spi_tx_bytes(SPI_MEM_COM, CmdBuffer, 4);

    spi_io_config(SPI_MEM_COM, 2, 0x00);

    spi_rx_bytes(SPI_MEM_COM, pbuf, 2);
    spi_rx_bytes(SPI_MEM_COM, pbuf, length);

    chip_disable(SPI_MEM_COM);

    spi_io_config(SPI_MEM_COM, 0, 0x01);

    flash_qe_clear();
}

void quad_io_word_fast_read(UINT8 *pbuf, UINT32 addr, UINT32 length)
{

    flash_qe_set();

    CmdBuffer[0] = HIGH_PER_MODE;
    CmdBuffer[1] = (UINT8)(addr >> 16);
    CmdBuffer[2] = (UINT8)(addr >> 8);
    CmdBuffer[3] = (UINT8)(addr);
    spi_tx_bytes(SPI_MEM_COM, CmdBuffer, 4);
    chip_disable(SPI_MEM_COM);

    CmdBuffer[0] = QUAD_IO_WORD_FAST_READ;
    spi_tx_bytes(SPI_MEM_COM, CmdBuffer, 1);

    spi_io_config(SPI_MEM_COM, 2, 0x0f);

    CmdBuffer[0] = (UINT8)(addr >> 16);
    CmdBuffer[1] = (UINT8)(addr >> 8);
    CmdBuffer[2] = (UINT8)(addr);
    CmdBuffer[3] = 0x0c;
    spi_tx_bytes(SPI_MEM_COM, CmdBuffer, 4);

    spi_io_config(SPI_MEM_COM, 2, 0x00);

    spi_rx_bytes(SPI_MEM_COM, pbuf, 1);
    spi_rx_bytes(SPI_MEM_COM, pbuf, length);

    chip_disable(SPI_MEM_COM);

    spi_io_config(SPI_MEM_COM, 0, 0x01);

    flash_qe_clear();
}

/*************************************************************************/
void flash_page_read_X1(UINT8 *pBuf, UINT32 addr, UINT32 length)
{
    flash_read(pBuf, addr, length);
    //flash_read_fast(pBuf, addr, length);
}

void flash_page_read_X2(UINT8 *pBuf, UINT32 addr, UINT32 length)
{
    dual_output_fast_read(pBuf, addr, length);
    //dual_io_fast_read(pBuf, addr, length);
}

void flash_page_read_X4(UINT8 *pBuf, UINT32 addr, UINT32 length)
{
    quad_output_fast_read(pBuf, addr, length);
    //quad_io_fast_read(pBuf, addr, length);
    //quad_io_word_fast_read(pBuf, addr, length);
}

void flash_page_program_X1(UINT8 *pBuf, UINT32 addr, UINT32 length)
{
    flash_page_program(pBuf, addr, length);
}

void flash_page_program_X4(UINT8 *pBuf, UINT32 addr, UINT32 length)
{
    flash_quad_page_program(pBuf, addr, length);
}
