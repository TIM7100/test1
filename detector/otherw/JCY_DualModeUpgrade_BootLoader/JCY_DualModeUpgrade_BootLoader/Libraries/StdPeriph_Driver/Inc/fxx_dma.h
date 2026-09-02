/*
  ******************************************************************************
  * @file    fxx_dma.h
  * @version V1.0.0
  * @date    2020
  * @brief   Header file of DMA module.
  ******************************************************************************
*/
#ifndef __FXX_DMA_H__
#define __FXX_DMA_H__

#include "fxx_std.h"

#define DMA_CHANNEL_NUM     (8)

/******************************************************************************/
/*                    Peripheral Registers Bits Definition                    */
/******************************************************************************/

/****************  Bit definition for DMA CONFIG register  ***********************/
#define DMA_CONFIG_M2ENDIAN             BIT2
#define DMA_CONFIG_M1ENDIAN             BIT1
#define DMA_CONFIG_EN                   BIT0

/****************  Bit definition for DMA Channel CTRL register  ***********************/
#define DMA_CHANNEL_CTRL_ITC            BIT31
#define DMA_CHANNEL_CTRL_DI             BIT27
#define DMA_CHANNEL_CTRL_SI             BIT26


/****************  Bit definition for DMA Channel CONFIG register  ***********************/
#define DMA_CHANNEL_CONFIG_DEST_PERIPH         (BIT19|BIT20|BIT21|BIT22|BIT23|BIT24)
#define DMA_CHANNEL_CONFIG_DEST_PERIPH_POS     (19)
#define DMA_CHANNEL_CONFIG_HALT                 BIT18
#define DMA_CHANNEL_CONFIG_ACTIVE               BIT17
#define DMA_CHANNEL_CONFIG_LOCK                 BIT16
#define DMA_CHANNEL_CONFIG_ITC                  BIT15
#define DMA_CHANNEL_CONFIG_IE                   BIT14
#define DMA_CHANNEL_CONFIG_FLOW_CTRL           (BIT11|BIT12|BIT13)
#define DMA_CHANNEL_CONFIG_SRC_PERIPH          (BIT1|BIT2|BIT3|BIT4|BIT5|BIT6)
#define DMA_CHANNEL_CONFIG_SRC_PERIPH_POS      (1)
#define DMA_CHANNEL_CONFIG_EN                   BIT0

/** @defgroup DMA_DATA_FLOW
 *  @{
 */
#define    DMA_DATA_FLOW_M2M    (0x00000000)
#define    DMA_DATA_FLOW_M2P    (0x00000800)
#define    DMA_DATA_FLOW_P2M    (0x00001000)
/**
  * @}
  */


/** @defgroup REQUEST_ID
 *  @{
 */
#define    REG_M2M                        (0)

#define    REQ0_ADC                       (0)
#define    REQ1_SPI1_SEND                 (1)
#define    REQ2_SPI1_RECV                 (2)
#define    REQ3_SPI2_SEND                 (3)
#define    REQ4_SPI2_RECV                 (4)
#define    REQ5_UART1_SEND                (5)
#define    REQ6_UART1_RECV                (6)
#define    REQ7_UART2_SEND                (7)
#define    REQ8_UART2_RECV                (8)
#define    REQ9_I2C1_SEND                 (9)
#define    REQ10_I2C1_RECV                (10)
#define    REQ11_I2C2_SEND                (11)
#define    REQ12_I2C2_RECV                (12)
#define    REQ13_TIM1_CH1                 (13)
#define    REQ14_TIM1_CH2                 (14)
#define    REQ15_TIM1_CH3                 (15)
#define    REQ16_TIM1_CH4                 (16)
#define    REQ17_TIM1_UP                  (17)
#define    REQ18_TIM1_TRIG_COM            (18)
#define    REQ19_TIM3_CH3                 (19)
#define    REQ20_TIM3_CH4_UP              (20)
#define    REQ21_TIM3_CH1_TRIG            (21)
#define    REQ22_TIM3_CH2                 (22)
#define    REQ23_TIM6_UP                  (23)
#define    REQ24_TIM15_CH1_UP_TRIG_COM    (24)
#define    REQ25_TIM15_CH2                (25)
#define    REQ26_TIM16_CH1_UP             (26)
#define    REQ27_UART3_SEND               (27)
#define    REQ28_TIM17_CH1_UP             (28)
#define    REQ29_UART3_RECV               (29)
#define    REQ30_LPUART_SEND              (30)
#define    REQ31_LPUART_RECV              (31)
#define    REQ32_TIM2_CH3                 (32)
#define    REQ33_TIM2_CH4_UP              (33)
#define    REQ34_TIM2_CH1_TRIG            (34)
#define    REQ35_TIM2_CH2                 (35)
#define    REQ36_TIM7_UP                  (36)
#define    REQ37_I2S1_TX                  (37)
#define    REQ38_I2S1_RX                  (38)
#define    REQ39_DAC1_CH1                 (39)
#define    REQ40_DAC1_CH2                 (40)
#define    REQ41_TIM4_CH3                 (41)
#define    REQ42_TIM4_CH4_UP              (42)
#define    REQ43_TIM4_CH1_TRIG            (43)
#define    REQ44_TIM4_CH2                 (44)
#define    REQ45_UART4_SEND               (45)
#define    REQ46_UART4_RECV               (46)
#define    REQ47_SPI3_SEND                (47)
#define    REQ48_SPI3_RECV                (48)
#define    REQ49_SPI4_SEND                (49)
#define    REQ50_SPI4_RECV                (50)

#define    REQ_MAX_LIMIT                  (51)

/** @defgroup IT_FLAG
 *  @{
 */
#define    DMA_IT_FLAG_TC0                (BIT0)
#define    DMA_IT_FLAG_TC1                (BIT1)
#define    DMA_IT_FLAG_TC2                (BIT2)
#define    DMA_IT_FLAG_TC3                (BIT3)
#define    DMA_IT_FLAG_TC4                (BIT4)
#define    DMA_IT_FLAG_TC5                (BIT5)
#define    DMA_IT_FLAG_TC6                (BIT6)
#define    DMA_IT_FLAG_TC7                (BIT7)
#define    DMA_IT_FLAG_TE0                (BIT16)
#define    DMA_IT_FLAG_TE1                (BIT17)
#define    DMA_IT_FLAG_TE2                (BIT18)
#define    DMA_IT_FLAG_TE3                (BIT19)
#define    DMA_IT_FLAG_TE4                (BIT20)
#define    DMA_IT_FLAG_TE5                (BIT21)
#define    DMA_IT_FLAG_TE6                (BIT22)
#define    DMA_IT_FLAG_TE7                (BIT23)
/**
  * @}
  */


/** @defgroup DMA_SOURCE_ADDR_INCREASE
 *  @{
 */
#define    DMA_SOURCE_ADDR_INCREASE_DISABLE    (0x00000000)
#define    DMA_SOURCE_ADDR_INCREASE_ENABLE     (0x04000000)
/**
  * @}
  */


/** @defgroup DMA_DST_ADDR_INCREASE
 *  @{
 */
#define    DMA_DST_ADDR_INCREASE_DISABLE    (0x00000000)
#define    DMA_DST_ADDR_INCREASE_ENABLE     (0x08000000)
/**
  * @}
  */


/** @defgroup DMA_SRC_WIDTH
 *  @{
 */
#define    DMA_SRC_WIDTH_BYTE         (0x00000000)    /*  8bit */
#define    DMA_SRC_WIDTH_HALF_WORD    (0x00040000)    /* 16bit */
#define    DMA_SRC_WIDTH_WORD         (0x00080000)    /* 32bit */
/**
  * @}
  */

/** @defgroup DMA_DST_WIDTH
 *  @{
 */
#define    DMA_DST_WIDTH_BYTE         (0x00000000)    /*  8bit */
#define    DMA_DST_WIDTH_HALF_WORD    (0x00200000)    /* 16bit */
#define    DMA_DST_WIDTH_WORD         (0x00400000)    /* 32bit */
/**
  * @}
  */
  
 /** @defgroup DMA_MODE DMA MODE
 *  @{
 */
#define DMA_NORMAL                    0x00000000U                  /*!< Normal mode                  */
#define DMA_CIRCULAR                  0x00000001U                  /*!< Circular mode                */
/**
  * @}
  */

/**
  * @brief  DMA burst length Structure definition
  */
typedef enum
{
    DMA_BURST_LENGTH_1   = 0,
    DMA_BURST_LENGTH_4   = 1,
    DMA_BURST_LENGTH_8   = 2,
    DMA_BURST_LENGTH_16  = 3,
    DMA_BURST_LENGTH_32  = 4,
    DMA_BURST_LENGTH_64  = 5,
    DMA_BURST_LENGTH_128 = 6,
    DMA_BURST_LENGTH_256 = 7,
}DMA_BURST_LENGTH;


/**
  * @brief  DMA Configuration Structure definition
  */
typedef struct 
{
    uint32_t SrcAddr;                /* source address */

    uint32_t DstAddr;                /* desination address */

    uint32_t Data_Flow;              /* This parameter can be a value of @ref DMA_DATA_FLOW */
    
    uint32_t Request_ID;             /* This parameter can be a value of @ref REQUEST_ID */
    
    uint32_t DMA_BufferSize;         /*!< Specifies the buffer size, in data unit, of the specified Channel. 
                                        The data unit is equal to the configuration set in DMA_PeripheralDataSize
                                        or DMA_MemoryDataSize members depending in the transfer direction. */

    uint32_t Source_Inc;             /* This parameter can be a value of @ref DMA_SOURCE_ADDR_INCREASE */

    uint32_t Desination_Inc;         /* This parameter can be a value of @ref DMA_DST_ADDR_INCREASE */

    uint32_t Source_Width;           /* This parameter can be a value of @ref DMA_SRC_WIDTH */

    uint32_t Desination_Width;       /* This parameter can be a value of @ref DMA_DST_WIDTH */

    uint32_t DMA_Mode;               /* This parameter can be a value of @ref DMA_MODE */
    
}DMA_InitTypeDef;

/**
  * @brief  DMA Link List Item Structure
  */
typedef struct DMA_NextLink
{
    uint32_t SrcAddr;               /* source address */

    uint32_t DstAddr;               /* desination address */

    struct DMA_NextLink *Next;      /* Next Link */

    uint32_t Control;               /* Control */

}DMA_LLI_InitTypeDef;


/**
  * @}
  */

/** @defgroup DMA_Exported_Constants
  * @{
  */
#define IS_DMA_MODE(MODE) (((MODE) == DMA_NORMAL )  || \
                           ((MODE) == DMA_CIRCULAR)) 

#define IS_DMA_ALL_PERIPH(PERIPH)    (((PERIPH) == DMA_Channel0) || \
                                      ((PERIPH) == DMA_Channel1) || \
                                      ((PERIPH) == DMA_Channel2) || \
                                      ((PERIPH) == DMA_Channel3) || \
                                      ((PERIPH) == DMA_Channel4) || \
                                      ((PERIPH) == DMA_Channel5) || \
                                      ((PERIPH) == DMA_Channel6) || \
                                      ((PERIPH) == DMA_Channel7))


#define IS_DMA_DATA_FLOW(DATA_FLOW)     (((DATA_FLOW) == DMA_DATA_FLOW_M2M) || \
                                         ((DATA_FLOW) == DMA_DATA_FLOW_M2P) || \
                                         ((DATA_FLOW) == DMA_DATA_FLOW_P2M))

#define IS_DMA_REQUEST_ID(REQUEST_ID)    ((REQUEST_ID < REQ_MAX_LIMIT) ? true : false)

#define IS_DMA_SRC_WIDTH(WIDTH)     (((WIDTH) == DMA_SRC_WIDTH_BYTE)      || \
                                     ((WIDTH) == DMA_SRC_WIDTH_HALF_WORD) || \
                                     ((WIDTH) == DMA_SRC_WIDTH_WORD))

#define IS_DMA_DST_WIDTH(WIDTH)     (((WIDTH) == DMA_DST_WIDTH_BYTE)      || \
                                     ((WIDTH) == DMA_DST_WIDTH_HALF_WORD) || \
                                     ((WIDTH) == DMA_DST_WIDTH_WORD))

#define IS_DMA_CONFIG_IT(DMA_IT)    (((DMA_IT) == DMA_CHANNEL_CONFIG_ITC) || \
                                     ((DMA_IT) == DMA_CHANNEL_CONFIG_IE))

#define IS_DMA_IT_FLAG(DMA_IT_FLAG) (((DMA_IT_FLAG) == DMA_IT_FLAG_TC0) || \
                                     ((DMA_IT_FLAG) == DMA_IT_FLAG_TC1) || \
                                     ((DMA_IT_FLAG) == DMA_IT_FLAG_TC2) || \
                                     ((DMA_IT_FLAG) == DMA_IT_FLAG_TC3) || \
                                     ((DMA_IT_FLAG) == DMA_IT_FLAG_TC4) || \
                                     ((DMA_IT_FLAG) == DMA_IT_FLAG_TC5) || \
                                     ((DMA_IT_FLAG) == DMA_IT_FLAG_TC6) || \
                                     ((DMA_IT_FLAG) == DMA_IT_FLAG_TC7) || \
                                     ((DMA_IT_FLAG) == DMA_IT_FLAG_TE0) || \
                                     ((DMA_IT_FLAG) == DMA_IT_FLAG_TE1) || \
                                     ((DMA_IT_FLAG) == DMA_IT_FLAG_TE2) || \
                                     ((DMA_IT_FLAG) == DMA_IT_FLAG_TE3) || \
                                     ((DMA_IT_FLAG) == DMA_IT_FLAG_TE4) || \
                                     ((DMA_IT_FLAG) == DMA_IT_FLAG_TE5) || \
                                     ((DMA_IT_FLAG) == DMA_IT_FLAG_TE6) || \
                                     ((DMA_IT_FLAG) == DMA_IT_FLAG_TE7))
                                     
/**
  * @}
  */

/** @defgroup DMA_Exported_Functions
  * @{
  */

void DMA_DeInit(DMA_Channel_TypeDef* DMA_Channelx);
void DMA_Init(DMA_Channel_TypeDef* DMA_Channelx, DMA_InitTypeDef* DMA_InitStruct);
void DMA_StructInit(DMA_InitTypeDef* DMA_InitStruct);
void DMA_Cmd(DMA_Channel_TypeDef* DMA_Channelx, FunctionalState NewState);
void DMA_ITConfig(DMA_Channel_TypeDef* DMA_Channelx, uint32_t DMA_IT, FunctionalState NewState);
void DMA_SetCurrDataCounter(DMA_Channel_TypeDef* DMA_Channelx, uint16_t DataNumber); 
uint16_t DMA_GetCurrDataCounter(DMA_Channel_TypeDef* DMA_Channelx);
FlagStatus DMA_GetFlagStatus(uint32_t DMA_FLAG);
void DMA_ClearFlag(uint32_t DMA_FLAG);
ITStatus DMA_GetITStatus(uint32_t DMA_IT);
void DMA_ClearITPendingBit(uint32_t DMA_IT);

#endif
