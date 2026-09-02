/*
  ******************************************************************************
  * @file    fxx_uart.h
  * @version V1.0.0
  * @date    2021
  * @brief   Header file of UART HAL module.
  ******************************************************************************
*/
#ifndef __FXX_UART_H__
#define __FXX_UART_H__

#ifdef __cplusplus
 extern "C" {
#endif
  
#include "fxx_std.h"

/******************************************************************************/
/*                    Peripheral Registers Bits Definition                    */
/******************************************************************************/

/****************  Bit definition for UART FR register  ***********************/
#define UART_FR_TXFE                    BIT7
#define UART_FR_RXFF                    BIT6
#define UART_FR_TXFF                    BIT5
#define UART_FR_RXFE                    BIT4
#define UART_FR_BUSY                    BIT3
#define UART_FR_CTS                     BIT0
/***************  Bit definition for UART LCRH register  **********************/
#define UART_LCRH_SPS                   BIT7
#define UART_LCRH_WLEN                 (BIT6|BIT5)
#define UART_LCRH_FEN                   BIT4
#define UART_LCRH_STP2                  BIT3
#define UART_LCRH_EPS                   BIT2
#define UART_LCRH_PEN                   BIT1
#define UART_LCRH_BRK                   BIT0
/****************  Bit definition for UART CR register  ***********************/
#define UART_CR_CTSEN                   BIT15
#define UART_CR_RTSEN                   BIT14
#define UART_CR_RTS                     BIT11
#define UART_CR_RXE                     BIT9
#define UART_CR_TXE                     BIT8
#define UART_CR_UARTEN                  BIT0
/***************  Bit definition for UART IFLS register  **********************/
#define UART_IFLS_RXIFLSEL             (BIT3|BIT4|BIT5)
#define UART_IFLS_TXIFLSEL             (BIT0|BIT1|BIT2)
/****************  Bit definition for UART IE register  ***********************/
#define UART_IE_OEI                     BIT10
#define UART_IE_BEI                     BIT9
#define UART_IE_PEI                     BIT8
#define UART_IE_FEI                     BIT7
#define UART_IE_RTI                     BIT6
#define UART_IE_TXI                     BIT5
#define UART_IE_RXI                     BIT4
/****************  Bit definition for UART RIS register  ***********************/
#define UART_RIS_OEI                    BIT10
#define UART_RIS_BEI                    BIT9
#define UART_RIS_PEI                    BIT8
#define UART_RIS_FEI                    BIT7
#define UART_RIS_RTI                    BIT6
#define UART_RIS_TXI                    BIT5
#define UART_RIS_RXI                    BIT4
/****************  Bit definition for UART ICR register  ***********************/
#define UART_ICR_OEI                    BIT10
#define UART_ICR_BEI                    BIT9
#define UART_ICR_PEI                    BIT8
#define UART_ICR_FEI                    BIT7
#define UART_ICR_RTI                    BIT6
#define UART_ICR_TXI                    BIT5
#define UART_ICR_RXI                    BIT4
/*****************  Bit definition for UART DMACR register  ***********************/
#define UART_DMACR_DMAONERR             BIT2
#define UART_DMACR_TXDMAE               BIT1
#define UART_DMACR_RXDMAE               BIT0
/*****************  Bit definition for UART CR2 register  ***********************/
#define UART_CR2_TXOE_SEL               BIT1
#define UART_CR2_RX_SEL                 BIT0



/** @defgroup UARTEx_Word_Length UARTEx Word Length
  * @{
  */
#define UART_WORDLENGTH_5B                  (0x00000000U)    /*!< 5-bit Word Length */
#define UART_WORDLENGTH_6B                  (0x00000020U)    /*!< 6-bit Word Length */
#define UART_WORDLENGTH_7B                  (0x00000040U)    /*!< 7-bit Word Length */
#define UART_WORDLENGTH_8B                  (0x00000060U)    /*!< 8-bit Word Length */
/**
  * @}
  */


/** @defgroup UART_Parity  UART Parity
  * @{
  */
#define UART_PARITY_NONE                    (0x00000000U)    /*!< No parity   */
#define UART_PARITY_EVEN                    (0x00000006U)    /*!< Even parity */
#define UART_PARITY_ODD                     (0x00000002U)    /*!< Odd parity  */
#define UART_PARITY_0                       (0x00000086U)    /*!< 0 parity  */
#define UART_PARITY_1                       (0x00000082U)    /*!< 1 parity  */
/**
  * @}
  */


/** @defgroup UART_Stop_Bits   UART Number of Stop Bits
  * @{
  */
#define UART_STOPBITS_1                     (0x00000000U)    /*!< UART frame with 1 stop bit    */
#define UART_STOPBITS_2                     (0x00008000U)    /*!< UART frame with 2 stop bits   */
/**
  * @}
  */


/** @defgroup UART_Hardware_Flow_Control UART Hardware Flow Control
  * @{
  */
#define UART_HWCONTROL_NONE                  (0x00000000U)        /*!< No hardware control       */
#define UART_HWCONTROL_CTS                   (0x00008000U)        /*!< Clear To Send             */
#define UART_HWCONTROL_RTS                   (0x00004000U)        /*!< Request To Send           */
#define UART_HWCONTROL_CTS_RTS               (UART_HWCONTROL_CTS | UART_HWCONTROL_RTS)    /*!< Request and Clear To Send */
/**
  * @}
  */


/** @defgroup UART_Mode UART Transfer Mode
  * @{
  */
#define UART_MODE_RX                        (0x00000200)        /*!< RX mode        */
#define UART_MODE_TX                        (0x00000100)        /*!< TX mode        */
#define UART_MODE_TX_RX                     (0x00000300)        /*!< RX and TX mode */
#define UART_MODE_TX_RX_DEBUG               (0x10000300)        /*!< RX¡¢TX mode and Debug use this uart */
#define UART_MODE_HALF_DUPLEX               (0x20000300)        /*!< Single half duplex */
/**
  * @}
  */


/** @defgroup FIFO interrupt Config
  * @{
  */
#define UART_TX_FIFO_1_16             (0x00000005)        /*!< Transfer 1 Data  */
#define UART_TX_FIFO_1_8              (0x00000000)        /*!< Transfer 2 Data  */
#define UART_TX_FIFO_1_4              (0x00000001)        /*!< Transfer 4 Data  */
#define UART_TX_FIFO_1_2              (0x00000002)        /*!< Transfer 8 Data  */
#define UART_TX_FIFO_3_4              (0x00000003)        /*!< Transfer 12 Data */
#define UART_TX_FIFO_7_8              (0x00000004)        /*!< Transfer 14 Data */

#define UART_RX_FIFO_1_16             (0x00000028)        /*!< Receive  1 Data  */
#define UART_RX_FIFO_1_8              (0x00000000)        /*!< Receive  2 Data  */
#define UART_RX_FIFO_1_4              (0x00000008)        /*!< Receive  4 Data  */
#define UART_RX_FIFO_1_2              (0x00000010)        /*!< Receive  8 Data  */
#define UART_RX_FIFO_3_4              (0x00000018)        /*!< Receive  12 Data */
#define UART_RX_FIFO_7_8              (0x00000020)        /*!< Receive  14 Data */
/**
  * @}
  */

/** @defgroup UART_Error_Code UART Error Code
  * @{
  */
#define UART_ERROR_NONE              0x00000000U   /*!< No error            */
#define UART_ERROR_PE                0x00000001U   /*!< Parity error        */
#define UART_ERROR_NE                0x00000002U   /*!< Noise error         */
#define UART_ERROR_FE                0x00000004U   /*!< Frame error         */
#define UART_ERROR_ORE               0x00000008U   /*!< Overrun error       */
#define UART_ERROR_DMA               0x00000010U   /*!< DMA transfer error  */
/**
  * @}
  */

/** @defgroup USART_Interrupt_definition 
  * @{
  */
#define UART_IT_BCNTI                       ((uint16_t)0x000C)
#define UART_IT_LBD                         ((uint16_t)0x000B)
#define UART_IT_ORE                         ((uint16_t)0x000A)
#define UART_IT_BE                          ((uint16_t)0x0009)
#define UART_IT_PE                          ((uint16_t)0x0008)
#define UART_IT_FE                          ((uint16_t)0x0007)
#define UART_IT_RTI                         ((uint16_t)0x0006)
#define UART_IT_TXE                         ((uint16_t)0x0005)
#define UART_IT_RXNE                        ((uint16_t)0x0004)
#define IT_MASK                              0x00FF
#define IS_UART_CONFIG_IT(IT) (((IT) == UART_IT_BCNTI) || ((IT) == UART_IT_LBD) || \
                               ((IT) == UART_IT_ORE) || ((IT) == UART_IT_BE) || \
                               ((IT) == UART_IT_PE) || ((IT) == UART_IT_FE) || \
                               ((IT) == UART_IT_RTI) || ((IT) == UART_IT_TXE))||\
                               ((IT) == UART_IT_RXNE)
#define IS_UART_GET_IT(IT) (((IT) == UART_IT_BCNTI) || ((IT) == UART_IT_LBD) || \
                               ((IT) == UART_IT_ORE) || ((IT) == UART_IT_BE) || \
                               ((IT) == UART_IT_PE) || ((IT) == UART_IT_FE) || \
                               ((IT) == UART_IT_RTI) || ((IT) == UART_IT_TXE))||\
                               ((IT) == UART_IT_RXNE)
#define IS_UART_CLEAR_IT(IT) (((IT) == UART_IT_BCNTI) || ((IT) == UART_IT_LBD) || \
                               ((IT) == UART_IT_ORE) || ((IT) == UART_IT_BE) || \
                               ((IT) == UART_IT_PE) || ((IT) == UART_IT_FE) || \
                               ((IT) == UART_IT_RTI) || ((IT) == UART_IT_TXE))||\
                               ((IT) == UART_IT_RXNE)

/**
  * @}
  */

/** @defgroup USART_Flags 
  * @{
  */
#define UART_FLAG_BCNTIC                    ((uint16_t)0x1000)
#define UART_FLAG_LBD                       ((uint16_t)0x0800)
#define UART_FLAG_ORE                       ((uint16_t)0x0400)
#define UART_FLAG_BE                        ((uint16_t)0x0200)
#define UART_FLAG_PE                        ((uint16_t)0x0100)
#define UART_FLAG_FE                        ((uint16_t)0x0080)
#define UART_FLAG_RTI                       ((uint16_t)0x0040)
#define UART_FLAG_TXE                       ((uint16_t)0x0020)
#define UART_FLAG_RXE                       ((uint16_t)0x0010)

/**
  * @}
  */


/*
 * @brief UART Init Structure definition
 */
typedef struct
{
    uint32_t UART_BaudRate;                  /*!< This member configures the UART communication baud rate. */

    uint32_t UART_WordLength;                /*!< Specifies the number of data bits transmitted or received in a frame.
                                             This parameter can be a value of @ref UARTEx_Word_Length. */

    uint32_t UART_StopBits;                  /*!< Specifies the number of stop bits transmitted.
                                             This parameter can be a value of @ref UART_Stop_Bits. */

    uint32_t UART_Parity;                    /*!< Specifies the parity mode. 
                                             This parameter can be a value of @ref UART_Parity. */

    uint32_t UART_Mode;                      /*!< Specifies whether the Receive or Transmit mode is enabled or disabled.
                                             This parameter can be a value of @ref UART_Mode. */

    uint32_t UART_HardwareFlowControl;       /*!< Specifies whether the hardware flow control mode is enabled or disabled.
                                             This parameter can be a value of @ref UART_Hardware_Flow_Control. */

}UART_InitTypeDef;



/** @defgroup   UART_Exported_Constants
  * @{
  */
#define IS_UART_ALL_INSTANCE(INSTANCE)    (((INSTANCE) == UART1) || \
                                           ((INSTANCE) == UART2) || \
                                           ((INSTANCE) == UART3) || \
                                           ((INSTANCE) == UART4))

#define IS_UART_WORD_LENGTH(__WORDLENGTH__)    (((__WORDLENGTH__) == UART_WORDLENGTH_5B) || \
                                               ((__WORDLENGTH__) == UART_WORDLENGTH_6B) || \
                                               ((__WORDLENGTH__) == UART_WORDLENGTH_7B) || \
                                               ((__WORDLENGTH__) == UART_WORDLENGTH_8B))

#define IS_UART_STOPBITS(__STOPBITS__)    (((__STOPBITS__) == UART_STOPBITS_1) || \
                                           ((__STOPBITS__) == UART_STOPBITS_2))

#define IS_UART_PARITY(__PARITY__)        (((__PARITY__) == UART_PARITY_NONE) || \
                                           ((__PARITY__) == UART_PARITY_EVEN) || \
                                           ((__PARITY__) == UART_PARITY_ODD))

#define IS_UART_MODE(__MODE__)            (((__MODE__) == UART_MODE_RX)          || \
                                           ((__MODE__) == UART_MODE_TX)          || \
                                           ((__MODE__) == UART_MODE_TX_RX)       || \
                                           ((__MODE__) == UART_MODE_TX_RX_DEBUG) || \
                                           ((__MODE__) == UART_MODE_HALF_DUPLEX))
                                           
#define IS_UART_HARDWARE_FLOW_CONTROL(__CONTROL__)    (((__CONTROL__) == UART_HWCONTROL_NONE) || \
                                                       ((__CONTROL__) == UART_HWCONTROL_RTS)  || \
                                                       ((__CONTROL__) == UART_HWCONTROL_CTS)  || \
                                                       ((__CONTROL__) == UART_HWCONTROL_CTS_RTS))
/**
  * @}
  */


void UART_Cmd(UART_TypeDef* UARTx, FunctionalState NewState);

void UART_Init(UART_TypeDef* UARTx, UART_InitTypeDef* UART_InitStruct);

void UART_DeInit(UART_TypeDef* UARTx);

void UART_StructInit(UART_InitTypeDef* UART_InitStruct);

void UART_ITConfig(UART_TypeDef* UARTx, uint16_t UART_IT, FunctionalState NewState);

void UART_ClearITPendingBit(UART_TypeDef* UARTx, uint32_t UART_IT);

void UART_SendData(UART_TypeDef* UARTx, uint8_t Data);

uint8_t UART_ReceiveData(UART_TypeDef* UARTx);

ITStatus UART_GetITStatus(UART_TypeDef* UARTx, uint16_t UART_IT);

void UART_DMACmd(UART_TypeDef* UARTx, uint32_t UART_DMAReq, FunctionalState NewState);

#ifdef __cplusplus
}
#endif

#endif
