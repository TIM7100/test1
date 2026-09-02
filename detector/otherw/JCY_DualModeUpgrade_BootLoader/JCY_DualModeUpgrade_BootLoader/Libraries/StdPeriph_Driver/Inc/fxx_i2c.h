/*
  ******************************************************************************
  * @file    fxx_i2c.h
  * @version V1.0.0
  * @date    2021
  * @brief   Header file of I2C  module.
  ******************************************************************************
*/

#ifndef __FXX_I2C_H__
#define __FXX_I2C_H__

#include "fxx_std.h"
typedef signed char         int8;
typedef signed short        int16;
typedef signed int          int32;
typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
/****************  Bit definition for I2C CR register  ********************/
#define I2C_CR_STOPF_INTEN                  BIT20
#define I2C_CR_RX_ADDR3_INTEN               BIT19
#define I2C_CR_DMA_EN                       BIT18
#define I2C_CR_TXE_SEL                      BIT17
#define I2C_CR_MARLO_INTEN                  BIT16
#define I2C_CR_TX_AUTO_EN                   BIT15
#define I2C_CR_OD_MODE                      BIT14
#define I2C_CR_RX_ADDR2_INT_EN              BIT12
#define I2C_CR_OVR_INT_EN                   BIT11
#define I2C_CR_RXNE_INT_EN                  BIT10
#define I2C_CR_TXE_INT_EN                   BIT9
#define I2C_CR_RX_ADDR1_INT_EN              BIT8
#define I2C_CR_MTF_INT_EN                   BIT7
#define I2C_CR_TACK                         BIT6
#define I2C_CR_STOP                         BIT5
#define I2C_CR_START                        BIT4
#define I2C_CR_TX                           BIT3
#define I2C_CR_MASTER                       BIT2
#define I2C_CR_NOSTRETCH                    BIT1
#define I2C_CR_MEN                          BIT0

/****************  Bit definition for I2C SR register  ********************/
#define I2C_SR_TIMEOUTBF                    BIT16
#define I2C_SR_TIMEOUTAF                    BIT15
#define I2C_SR_RX_ADDR3                     BIT14
#define I2C_SR_RX_ADDR2                     BIT12
#define I2C_SR_OVR                          BIT11
#define I2C_SR_RXNE                         BIT10
#define I2C_SR_TXE                          BIT9
#define I2C_SR_RX_ADDR1                     BIT8
#define I2C_SR_MTF                          BIT7
#define I2C_SR_MARLO                        BIT6
#define I2C_SR_TX_RX_FLAG                   BIT5
#define I2C_SR_BUS_BUSY                     BIT4
#define I2C_SR_SRW                          BIT3
#define I2C_SR_STOPF                        BIT2
#define I2C_SR_STARTF                       BIT1
#define I2C_SR_RACK                         BIT0

/**************  Bit definition for I2C SLAVE ADDR2/3 register  **************/
#define I2C_ADDR3_EN                        BIT8
#define I2C_ADDR2_EN                        BIT0

/**************  Bit definition for I2C TIMEOUT register  **************/
#define I2C_TIMEOUT_EXTEN                   BIT31
#define I2C_TOUTB_INTEN                     BIT30
#define I2C_EXT_MODE                        BIT29
#define I2C_TIMEOUT_TIMOUTEN                BIT15
#define I2C_TOUTA_INTEN                     BIT14

/** @defgroup I2C_MODE
 *  @{
 */
#define    I2C_MODE_SLAVE     (0U)
#define    I2C_MODE_MASTER    (1U)
/**
  * @}
  */

/** @defgroup CLOCK_SPEED
 *  @{
 */
#define    CLOCK_SPEED_STANDARD     (100000U)
#define    CLOCK_SPEED_FAST         (400000U)
#define    CLOCK_SPEED_FAST_PLUS    (1000000U)
/**
  * @}
  */


/** @defgroup TX_AUTO_EN
 *  @{
 */
#define    TX_AUTO_EN_DISABLE    (0U)
#define    TX_AUTO_EN_ENABLE     (1U)
/**
  * @}
  */


/** @defgroup NO_STRETCH_MODE
 *  @{
 */
#define    NO_STRETCH_MODE_STRETCH      (0U)
#define    NO_STRETCH_MODE_NOSTRETCH    (1U)
/**
  * @}
  */

/** @defgroup SLAVE State machine
 *  @{
 */
#define    SLAVE_RX_STATE_IDLE         (0U)
#define    SLAVE_RX_STATE_RECEIVING    (1U)
#define    SLAVE_TX_STATE_IDLE         (0U)
#define    SLAVE_TX_STATE_SENDING      (1U)
/**
  * @}
  */
  
 /** @defgroup I2C_Memory_Address_Size I2C Memory Address Size
  * @{
  */
#define I2C_MEMADD_SIZE_8BIT            (0U)
#define I2C_MEMADD_SIZE_16BIT           (1U)
/**
  * @}
  */
 

/* Private macros ------------------------------------------------------------*/
/** @defgroup I2C_Private_Macros I2C Private Macros
  * @{
  */
#define I2C_MEM_ADD_MSB(__ADDRESS__)                       ((uint8_t)((uint16_t)(((uint16_t)((__ADDRESS__) & (uint16_t)0xFF00)) >> 8)))
#define I2C_MEM_ADD_LSB(__ADDRESS__)                       ((uint8_t)((uint16_t)((__ADDRESS__) & (uint16_t)0x00FF)))
 
/**
  * @brief  I2C Configuration Structure definition
  */
  
#define _I2C_GET_FLAG(__HANDLE__, __FLAG__)   ((((__HANDLE__->SR) & (__FLAG__))  == (__FLAG__) ) ? 1 : 0) 


typedef enum
{
    
	I2C_OK       = 0x00U,   
	I2C_ERROR    = 0x01U,    
	I2C_BUSY     = 0x02U,    
	I2C_TIMEOUT  = 0x03U
	
}I2C_StatusTypeDef;

typedef struct
{
    uint32_t  I2C_Mode;                   /* This parameter can be a value of @ref I2C_MODE */
        
    uint32_t  I2C_Own_Address;            /* This parameter can be a 7-bit address */
    
    uint32_t  I2C_ClockSpeed;             /* This parameter can be a value of @ref CLOCK_SPEED */
	
	uint32_t  I2C_Tx_Auto_En;             /* This parameter can be a value of @ref TX_AUTO_EN */
    
    uint32_t  I2C_No_Stretch_Mode;        /* This parameter can be a value of @ref NO_STRETCH_MODE */
} I2C_InitTypeDef;

/** @defgroup I2C_registers 
  * @{
  */

#define I2C_Register_ADDR1            ((uint8_t)0x00)
#define I2C_Register_CLKDIV           ((uint8_t)0x04)
#define I2C_Register_CR               ((uint8_t)0x08)
#define I2C_Register_SR               ((uint8_t)0x0C)
#define I2C_Register_DR               ((uint8_t)0x10)
#define I2C_Register_ADDR2_3          ((uint8_t)0x14)
#define I2C_Register_TIMEOUT          ((uint8_t)0x24)
#define IS_I2C_REGISTER(REGISTER) (((REGISTER) == I2C_Register_ADDR1) || \
                                   ((REGISTER) == I2C_Register_CLKDIV) || \
                                   ((REGISTER) == I2C_Register_CR) || \
                                   ((REGISTER) == I2C_Register_SR) || \
                                   ((REGISTER) == I2C_Register_DR) || \
                                   ((REGISTER) == I2C_Register_ADDR2_3) || \
                                   ((REGISTER) == I2C_Register_TIMEOUT))
/**
  * @}
  */



/** @defgroup I2C_transfer_direction 
  * @{
  */\
#define  I2C_Direction_Transmitter      ((uint8_t)0x00)
#define  I2C_Direction_Receiver         ((uint8_t)0x01)
#define IS_I2C_DIRECTION(DIRECTION) (((DIRECTION) == I2C_Direction_Transmitter) || ((DIRECTION) == I2C_Direction_Receiver))
									 
/**
  * @}
  */
  
  
/** 
  * @brief  I2C_Exported_Constants 
  */
#define IS_I2C_MODE(I2C_MODE)    (((I2C_MODE) == I2C_MODE_SLAVE) || \
                                      ((I2C_MODE) == I2C_MODE_MASTER))

#define IS_I2C_CLOCK_SPEED(CLOCK_SPEED)    (((CLOCK_SPEED) > 0U) && ((CLOCK_SPEED) <=1000000U))

#define IS_I2C_TX_AUTO_EN(TX_AUTO_EN)      (((TX_AUTO_EN) == TX_AUTO_EN_DISABLE) || \
                                            ((TX_AUTO_EN) == TX_AUTO_EN_ENABLE))

#define IS_I2C_STRETCH_EN(STRETCH_EN)    (((STRETCH_EN) == NO_STRETCH_MODE_STRETCH) || \
                                          ((STRETCH_EN) == NO_STRETCH_MODE_NOSTRETCH))

#define IS_I2C_ALL_PERIPH(PERIPH) (((PERIPH) == I2C1) || ((PERIPH) == I2C2))
/**
  * @}
  */

/**
  * @brief  I2C Slave Communication Structure definition
  */
typedef struct
{
   
    __IO uint32_t               Slave_RxState;    /* I2C Slave state machine */
    __IO uint32_t               Slave_TxState;    /* I2C Slave state machine */
      
    __IO uint32_t               Rx_Size;          /* I2C Rx Size */
    __IO uint32_t               Tx_Size;          /* I2C Tx Size */
    
    __IO uint32_t               Rx_Count;         /* I2C Rx Count */
    __IO uint32_t               Tx_Count;         /* I2C Tx Count */
	
	__IO uint8_t               *Rx_Buffer;        /* I2C Rx Buffer */
    __IO uint8_t               *Tx_Buffer;        /* I2C Tx Buffer */

//    DMA_HandleTypeDef   *HDMA_Rx;          /* I2C Rx DMA handle parameters */
//    DMA_HandleTypeDef   *HDMA_Tx;          /* I2C Tx DMA handle parameters */

    __IO void (*I2C_STOPF_Callback)(void);      /* I2C STOP flag interrupt callback */

}Slave_Comm_TypeDef;


void _I2Cx_IRQHandler(I2C_TypeDef* I2Cx,Slave_Comm_TypeDef* Slave_Comm);

void I2C_Init(I2C_TypeDef* I2Cx, I2C_InitTypeDef* I2C_InitStruct);

void I2C_DeInit(I2C_TypeDef* I2Cx);

void I2C_Cmd(I2C_TypeDef* I2Cx, FunctionalState NewState);

void I2C_DMACmd(I2C_TypeDef* I2Cx, FunctionalState NewState);

FlagStatus I2C_GetFlagStatus(I2C_TypeDef* I2Cx, uint32_t I2C_FLAG);

void I2C_ITConfig(I2C_TypeDef* I2Cx, uint16_t I2C_IT, FunctionalState NewState);

void I2C_ClearFlag(I2C_TypeDef* I2Cx, uint32_t I2C_FLAG);

void I2C_GenerateSTART(I2C_TypeDef* I2Cx, FunctionalState NewState);

void I2C_GenerateSTOP(I2C_TypeDef* I2Cx, FunctionalState NewState);

void I2C_Send7bitAddress(I2C_TypeDef* I2Cx, uint8_t Address, uint8_t I2C_Direction);

void I2C_StretchClockCmd(I2C_TypeDef* I2Cx, FunctionalState NewState);

uint32_t I2C_ReadRegister(I2C_TypeDef* I2Cx, uint8_t I2C_Register);

void I2C_SendData(I2C_TypeDef* I2Cx, uint8_t Data);

uint8_t I2C_ReceiveData(I2C_TypeDef* I2Cx);

uint8_t I2C_Mem_Write(I2C_TypeDef* I2Cx,uint8_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint32_t Size, uint32_t Timeout);

uint8_t I2C_Mem_Read(I2C_TypeDef* I2Cx, uint8_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);

uint8_t I2C_Master_Transmit(I2C_TypeDef* I2Cx, uint16_t DevAddress, uint8_t *pData, uint32_t Size, uint32_t Timeout);

uint8_t I2C_Master_Receive(I2C_TypeDef* I2Cx, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);

uint8_t I2C_Slave_Transmit(I2C_TypeDef* I2Cx, uint8_t *pData, uint32_t Size, uint32_t Timeout);

uint8_t I2C_Slave_Receive(I2C_TypeDef* I2Cx, uint8_t *pData, uint32_t Size, uint32_t Timeout);

uint8_t I2C_Slave_Transmit_IT(I2C_TypeDef* I2Cx, Slave_Comm_TypeDef* Slave_Comm, uint8_t *pData, uint32_t Size);

uint8_t I2C_Slave_Receive_IT(I2C_TypeDef* I2Cx, Slave_Comm_TypeDef* Slave_Comm, uint8_t *pData, uint32_t Size);

#endif
