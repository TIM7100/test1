#ifndef __BSP_SPI_H__
#define __BSP_SPI_H__

#include "fxx_std.h"


typedef enum
{
    SPI_OK       = 0x00U,
    SPI_ERROR    = 0x01U,
    SPI_BUSY     = 0x02U,
    SPI_TIMEOUT  = 0x03U
}SPI_StatusTypeDef;


/**
 * @brief 该数据类型用于存放SPI在使用过程中，需要使用的数据组
 *        一般使用于中断，若不需要使用SPI中断，可以删除该结构体类型
 ***/
typedef struct
{ 
    uint32_t               RxState;          /* SPI state machine */
    uint32_t               TxState;          /* SPI state machine */
    
    uint8_t               *Rx_Buffer;        /* SPI Rx Buffer */
    uint8_t               *Tx_Buffer;        /* SPI Tx Buffer */
    
    uint32_t               Rx_Size;          /* SPI Rx Size */
    uint32_t               Tx_Size;          /* SPI Tx Size */
    
    uint32_t               Rx_Count;         /* SPI RX Count */
    uint32_t               Tx_Count;         /* SPI TX Count */
}SPI_DataTypeDef;



#define SPIx_WriteByte(SPIx, TxData)  SPI_Transmit(SPIx, &TxData, 1, 0)
#define SPIx_ReadByte(SPIx, RxData)	  SPI_Receive(SPIx, RxData, 1, 0)

void SPI_Configuration(SPI_TypeDef* SPIx);

//SPI_StatusTypeDef SPIx_WriteByte(SPI_TypeDef* SPIx, u8 TxData);
//SPI_StatusTypeDef SPIx_ReadByte(SPI_TypeDef* SPIx, u8 *RxData);
u8 SPIx_WriteReadByte(SPI_TypeDef* SPIx, u8 TxData);


SPI_StatusTypeDef SPI_Transmit(SPI_TypeDef* SPIx, uint8_t *pData, uint32_t Size, uint32_t Timeout);
SPI_StatusTypeDef SPI_Receive(SPI_TypeDef* SPIx, uint8_t *pData, uint32_t Size, uint32_t Timeout);

SPI_StatusTypeDef SPI_TransmitReceive(SPI_TypeDef* SPIx, uint8_t *pTxData, uint8_t *pRxData, uint32_t Size, uint32_t Timeout);


void SPIx_SetSpeed(SPI_TypeDef* SPIx, u8 speed);


#endif

