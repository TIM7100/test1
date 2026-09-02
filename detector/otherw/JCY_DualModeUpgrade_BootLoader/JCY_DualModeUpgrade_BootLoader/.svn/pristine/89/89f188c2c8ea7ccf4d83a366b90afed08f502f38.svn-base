/*
  ******************************************************************************
  * @file    fxx_spi.h
  * @version V1.0.0
  * @date    2021
  * @brief   Header file of SPI module.
  ******************************************************************************
*/
#ifndef __FXX_SPI_H__
#define __FXX_SPI_H__

#include  "fxx_std.h" 

/****************  Bit definition for SPI_CTL register  **************************/
#define SPI_CTL_CS_TIME                       (BIT11|BIT12|BIT13|BIT14|BIT15|BIT16|BIT17|BIT18)
#define SPI_CTL_CS_FILTER                      BIT10
#define SPI_CTL_CS_RST                         BIT9
#define SPI_CTL_SLAVE_EN                       BIT8
#define SPI_CTL_IO_MODE                        BIT7
#define SPI_CTL_X_MODE                        (BIT6|BIT5)
#define SPI_CTL_LSB_FIRST                      BIT4
#define SPI_CTL_CPOL                           BIT3
#define SPI_CTL_CPHA                           BIT2
#define SPI_CTL_SFILTER                        BIT1
#define SPI_CTL_MST_MODE                       BIT0

/****************  Bit definition for SPI_TX_CTL register  ***********************/
#define SPI_TX_CTL_DMA_LEVEL                  (BIT4|BIT5|BIT6|BIT7)
#define SPI_TX_CTL_DMA_LEVEL_3                 BIT7
#define SPI_TX_CTL_DMA_LEVEL_2                 BIT6
#define SPI_TX_CTL_DMA_LEVEL_1                 BIT5
#define SPI_TX_CTL_DMA_LEVEL_0                 BIT4
#define SPI_TX_CTL_DMA_REQ_EN                  BIT3
#define SPI_TX_CTL_MODE                        BIT2
#define SPI_TX_CTL_FIFO_RESET                  BIT1
#define SPI_TX_CTL_EN                          BIT0

/****************  Bit definition for SPI_RX_CTL register  ***********************/
#define SPI_RX_CTL_DMA_LEVEL                  (BIT4|BIT5|BIT6|BIT7)
#define SPI_RX_CTL_DMA_LEVEL_3                 BIT7
#define SPI_RX_CTL_DMA_LEVEL_2                 BIT6
#define SPI_RX_CTL_DMA_LEVEL_1                 BIT5
#define SPI_RX_CTL_DMA_LEVEL_0                 BIT4
#define SPI_RX_CTL_DMA_REQ_EN                  BIT3
#define SPI_RX_CTL_FIFO_RESET                  BIT1
#define SPI_RX_CTL_EN                          BIT0

/****************  Bit definition for SPI_IE register  ***************************/
#define SPI_IE_RX_BATCH_DONE_EN                BIT15
#define SPI_IE_TX_BATCH_DONE_EN                BIT14
#define SPI_IE_RX_FIFO_FULL_OV_EN              BIT13
#define SPI_IE_RX_FIFO_EMPTY_OV_EN             BIT12
#define SPI_IE_RX_NOT_EMPTY_EN                 BIT11
#define SPI_IE_CS_POS_EN                       BIT10
#define SPI_IE_RX_FIFO_HALF_FULL_EN            BIT9
#define SPI_IE_RX_FIFO_HALF_EMPTY_EN           BIT8
#define SPI_IE_TX_FIFO_HALF_FULL_EN            BIT7
#define SPI_IE_TX_FIFO_HALF_EMPTY_EN           BIT6
#define SPI_IE_RX_FIFO_FULL_EN                 BIT5
#define SPI_IE_RX_FIFO_EMPTY_EN                BIT4
#define SPI_IE_TX_FIFO_FULL_EN                 BIT3
#define SPI_IE_TX_FIFO_EMPTY_EN                BIT2
#define SPI_IE_BATCH_DONE_EN                   BIT1
#define IS_SPI_CONFIG_IT(IT)  (((IT) <= BIT16))                          

/****************  Bit definition for SPI_STATUS register  ***********************/
#define SPI_STATUS_RX_BATCH_DONE               BIT15
#define SPI_STATUS_TX_BATCH_DONE               BIT14
#define SPI_STATUS_RX_FIFO_FULL_OV             BIT13
#define SPI_STATUS_RX_FIFO_EMPTY_OV            BIT12
#define SPI_STATUS_RX_NOT_EMPTY                BIT11
#define SPI_STATUS_CS_POS                      BIT10
#define SPI_STATUS_RX_FIFO_HALF_FULL           BIT9
#define SPI_STATUS_RX_FIFO_HALF_EMPTY          BIT8
#define SPI_STATUS_TX_FIFO_HALF_FULL           BIT7
#define SPI_STATUS_TX_FIFO_HALF_EMPTY          BIT6
#define SPI_STATUS_RX_FIFO_FULL                BIT5
#define SPI_STATUS_RX_FIFO_EMPTY               BIT4
#define SPI_STATUS_TX_FIFO_FULL                BIT3
#define SPI_STATUS_TX_FIFO_EMPTY               BIT2
#define SPI_STATUS_BATCH_DONE                  BIT1
#define SPI_STATUS_TX_BUSY                     BIT0
#define IS_SPI_GET_FLAG(FLAG)  (((FLAG) <= BIT16))                          

/****************  Bit definition for SPI_CS register  ***************************/
#define SPI_CS_CSX                             BIT1
#define SPI_CS_CS0                             BIT0

/****************  Bit definition for SPI_OUT_EN register  ***********************/
#define SPI_HOLD_EN                            BIT3
#define SPI_HOLD_WP_EN                         BIT2
#define SPI_HOLD_MISO_EN                       BIT1
#define SPI_HOLD_MOSI_EN                       BIT0

/****************  Bit definition for SPI_MEMO_ACC register  ***********************/
#define SPI_ADDR_WIDTH                        (BIT14|BIT15|BIT16|BIT17|BIT18)
#define SPI_PARA_NO2                          (BIT9|BIT10|BIT11|BIT12|BIT13)
#define SPI_PARA_NO1                          (BIT5|BIT6|BIT7|BIT8)
#define SPI_CON_RD_EN                          BIT3
#define SPI_PARA_ORD2                          BIT2
#define SPI_PARA_ORD1                          BIT1
#define SPI_ACC_EN                             BIT0

/** @defgroup SLAVE State machine
 *  @{
 */
#define    SPI_RX_STATE_IDLE         (0U)
#define    SPI_RX_STATE_RECEIVING    (1U)
#define    SPI_TX_STATE_IDLE         (0U)
#define    SPI_TX_STATE_SENDING      (1U)
/**
  * @}
  */


/** @defgroup SPI_MODE
 *  @{
 */
#define SPI_MODE_SLAVE     (0x00000000)
#define SPI_MODE_MASTER    (0x00000001)
#define IS_SPI_ALL_MODE(SPI_Mode) (((SPI_Mode) == SPI_MODE_SLAVE) || \
                                   ((SPI_Mode) == SPI_MODE_MASTER))
/**
  * @}
  */
  
/** @defgroup SPI_Clock_Polarity 
  * @{
  */
#define SPI_CPOL_LOW      (0x00000000)
#define SPI_CPOL_HIGH     (0x00000008)
#define IS_SPI_CPOL(CPOL) (((CPOL) == SPI_CPOL_LOW) || \
                           ((CPOL) == SPI_CPOL_HIGH))
/**
  * @}
  */

/** @defgroup SPI_Clock_Phase SPI Clock Phase
  * @{
  */
#define SPI_CPHA_1EDGE     (0x00000000)
#define SPI_CPHA_2EDGE     (0x00000004)
#define IS_SPI_CPHA(CPHA) (((CPHA) == SPI_CPHA_1EDGE) || \
                           ((CPHA) == SPI_CPHA_2EDGE))                          
/**
  * @}
  */


/** @defgroup SPI_X_MODE SPI line mode 1/2/4 lines
  * @{
  */
#define SPI_1X_MODE    (0x00000000)
#define SPI_2X_MODE    (0x00000020)
#define SPI_4X_MODE    (0x00000040)
#define IS_SPI_X_MODE(X_MODE) (((X_MODE) == SPI_1X_MODE) || \
                               ((X_MODE) == SPI_2X_MODE) || \
                               ((X_MODE) == SPI_4X_MODE))
/**
  * @}
  */


/** @defgroup SPI_MSB_LSB_FIRST
  * @{
  */
#define SPI_FIRSTBIT_MSB    (0x00000000)
#define SPI_FIRSTBIT_LSB    (0x00000010)
#define IS_SPI_FIRST_BIT(FIRST_BIT)        (((FIRST_BIT) == SPI_FIRSTBIT_MSB) || \
                                            ((FIRST_BIT) == SPI_FIRSTBIT_LSB))
/**
  * @}
  */


/** @defgroup SPI_BAUDRATE_PRESCALER
  * @{
  */
#define SPI_BAUDRATE_PRESCALER_2      (2U)
#define SPI_BAUDRATE_PRESCALER_4      (4U)
#define SPI_BAUDRATE_PRESCALER_8      (8U)
#define SPI_BAUDRATE_PRESCALER_16     (16U)
#define SPI_BAUDRATE_PRESCALER_32     (32U)
#define SPI_BAUDRATE_PRESCALER_64     (64U)
#define SPI_BAUDRATE_PRESCALER_128    (128U)
#define SPI_BAUDRATE_PRESCALER_254    (254U)
#define IS_SPI_BAUDRATE_PRESCALER(BAUDRATE)    (((BAUDRATE) == SPI_BAUDRATE_PRESCALER_2)   || \
                                                ((BAUDRATE) == SPI_BAUDRATE_PRESCALER_4)   || \
                                                ((BAUDRATE) == SPI_BAUDRATE_PRESCALER_8)   || \
                                                ((BAUDRATE) == SPI_BAUDRATE_PRESCALER_16)  || \
                                                ((BAUDRATE) == SPI_BAUDRATE_PRESCALER_32)  || \
                                                ((BAUDRATE) == SPI_BAUDRATE_PRESCALER_64)  || \
                                                ((BAUDRATE) == SPI_BAUDRATE_PRESCALER_128) || \
                                                ((BAUDRATE) == SPI_BAUDRATE_PRESCALER_254))
/**
  * @}
  */

/** @defgroup SPI_DMA_transfer_requests 
  * @{
  */
#define SPI_DMAReq_TX               (BIT0)
#define SPI_DMAReq_RX               (BIT1)
#define IS_SPI_DMAREQ(DMAREQ)   ((DMAREQ) < BIT2)
/**
  * @}
  */
  
/** @defgroup SPI_Clear_Flag 
  * @{
  */
#define SPI_CLEAR_CS_POS                      BIT10
#define SPI_CLEAR_BATCH_DONE                  BIT1
#define IS_SPI_CLEAR_FLAG(FLAG) (((FLAG) == SPI_CLEAR_CS_POS)   || \
                                 ((FLAG) == SPI_CLEAR_BATCH_DONE))

/**
  * @brief  SPI Configuration Structure definition
  */
typedef struct
{
    uint32_t  SPI_Mode;              /*!< Specifies the SPI operating mode.
                                         This parameter can be a value of @ref SPI_MODE */
    
    uint32_t  SPI_CPOL;              /*!< Specifies the serial clock steady state.
                                         This parameter can be a value of @ref SPI_Clock_Polarity */

    uint32_t  SPI_CPHA;              /*!< Specifies the clock active edge for the bit capture.
                                         This parameter can be a value of @ref SPI_Clock_Phase */
    
    uint32_t  SPI_X_Mode;            /*!< Specifies line mode 1/2/4 lines. 
                                         This parameter can be a value of @ref SPI_X_MODE */
    
    uint32_t  SPI_FirstBit;          /*!< Specifies whether data transfers start from MSB or LSB bit.
                                         This parameter can be a value of @ref SPI_MSB_LSB_FIRST */
    
    uint32_t  SPI_BaudRatePrescaler; /*!< Specifies the Baud Rate prescaler value which will be
                                         used to configure the transmit and receive SCK clock.
                                         This parameter can be a value of @ref SPI_BAUDRATE_PRESCALER */
}SPI_InitTypeDef;
/**
  * @}
  */

/**
  * @brief  SPI Periph Check
  */
#define IS_SPI_ALL_PERIPH(PERIPH) (((PERIPH) == SPI1) || \
                                   ((PERIPH) == SPI2) || \
                                   ((PERIPH) == SPI3) || \
                                   ((PERIPH) == SPI4))
/**
  * @}
  */

/** @defgroup SPI_Exported_Functions
  * @{
  */
void SPI_DeInit(SPI_TypeDef* SPIx);
void SPI_Init(SPI_TypeDef* SPIx, SPI_InitTypeDef* SPI_InitStruct);
void SPI_StructInit(SPI_InitTypeDef* SPI_InitStruct);
void SPI_TxCmd(SPI_TypeDef* SPIx, FunctionalState NewState);
void SPI_RxCmd(SPI_TypeDef* SPIx, FunctionalState NewState);
void SPI_ITConfig(SPI_TypeDef* SPIx, uint32_t SPI_IT, FunctionalState NewState);
FlagStatus SPI_ITGet(SPI_TypeDef* SPIx, uint32_t SPI_IT);
void SPI_WireDmaFifo(SPI_TypeDef* SPIx, uint32_t Fifo_level);
void SPI_DMACmd(SPI_TypeDef* SPIx, uint16_t SPI_DMAReq, FunctionalState NewState);
void SPI_SendData(SPI_TypeDef* SPIx, uint8_t Data);
uint8_t SPI_ReceiveData(SPI_TypeDef* SPIx);
void SPI_SSOutputCmd(SPI_TypeDef* SPIx, FunctionalState NewState);
void SPI_Wire_Config(SPI_TypeDef* SPIx, uint32_t X_Mode);
void SPI_WriteBatch(SPI_TypeDef* SPIx, uint32_t number);
uint32_t SPI_ReadBatch(SPI_TypeDef* SPIx);
void SPI_ClearTxFifo(SPI_TypeDef* SPIx);
void SPI_ClearRxFifo(SPI_TypeDef* SPIx);
FlagStatus SPI_GetFlagStatus(SPI_TypeDef* SPIx, uint32_t SPI_FLAG);
void SPI_ClearFlag(SPI_TypeDef* SPIx, uint32_t SPI_FLAG);
ITStatus SPI_GetITStatus(SPI_TypeDef* SPIx, uint32_t SPI_IT);
void SPI_ClearITPendingBit(SPI_TypeDef* SPIx, uint32_t SPI_IT);

#endif





