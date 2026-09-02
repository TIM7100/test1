#ifndef __BSP_SPI_H__
#define __BSP_SPI_H__

#include "ACM32Fxx_HAL.h"

typedef enum
{
    SPI_OK       = 0x00U,
    SPI_ERROR    = 0x01U,
    SPI_BUSY     = 0x02U,
    SPI_TIMEOUT  = 0x03U
}SPI_StatusTypeDef;

#define SPIx_WriteByte(SPIx, TxData)    SPI_Transmit(SPIx, (uint8_t*)(&TxData), 1, 0)
#define SPIx_ReadByte(SPIx, RxData)     SPI_Receive(SPIx, RxData, 1, 0)

void SPI_Configuration(SPI_TypeDef* SPIx);
uint8_t SPIx_WriteReadByte(SPI_TypeDef* SPIx, uint8_t TxData);

SPI_StatusTypeDef SPI_Transmit(SPI_TypeDef* SPIx, uint8_t *pData, uint32_t Size, uint32_t Timeout);
SPI_StatusTypeDef SPI_Receive(SPI_TypeDef* SPIx, uint8_t *pData, uint32_t Size, uint32_t Timeout);
SPI_StatusTypeDef SPI_TransmitReceive(SPI_TypeDef* SPIx, uint8_t *pTxData, uint8_t *pRxData, uint32_t Size, uint32_t Timeout);

void SPIx_SetSpeed(SPI_TypeDef* SPIx, uint8_t speed);

#endif