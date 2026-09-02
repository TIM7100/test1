/*
  ******************************************************************************
  * @file    fxx_can.h
  * @version V1.0.0
  * @date    2020
  * @brief   Header file of CAN HAL module.
  ******************************************************************************
*/
#ifndef _CAN_H_
#define _CAN_H_

#include "f4_f3.h"

/**
  * @}
  */
	
/******************************************************************************/
/*                    Peripheral Registers Bits Definition                    */
/******************************************************************************/
/******************************************************************************/
/*                                    (CAN)                                   */
/******************************************************************************/

/****************  Bit definition for CAN MOD register  ***********************/
#define CAN_MOD_RM                      BIT0
#define CAN_MOD_LOM                     BIT1
#define CAN_MOD_STM                     BIT2
#define CAN_MOD_AFM                     BIT3
#define CAN_MOD_SM                      BIT4
/****************  Bit definition for CAN CMR register  ***********************/
#define CAN_CMR_TR                      BIT0
#define CAN_CMR_AT                      BIT1
#define CAN_CMR_RRB                     BIT2
#define CAN_CMR_CDO                     BIT3
#define CAN_CMR_SRR                     BIT4
/****************  Bit definition for CAN SR register  ***********************/
#define CAN_SR_RBS                      BIT0
#define CAN_SR_DOS                      BIT1
#define CAN_SR_TBS                      BIT2
#define CAN_SR_TCS                      BIT3
#define CAN_SR_RS                       BIT4
#define CAN_SR_TS                       BIT5
#define CAN_SR_ES                       BIT6
#define CAN_SR_BS                       BIT7
/****************  Bit definition for CAN IR register  ***********************/
#define CAN_IR_RI                       BIT0  
#define CAN_IR_TI                       BIT1 
#define CAN_IR_EI                       BIT2 
#define CAN_IR_DOI                      BIT3 
#define CAN_IR_WUI                      BIT4 
#define CAN_IR_EPI                      BIT5 
#define CAN_IR_ALI                      BIT6 
#define CAN_IR_BEI                      BIT7 
/****************  Bit definition for CAN IER register  ***********************/
#define CAN_IER_RIE                     BIT0  
#define CAN_IER_TIE                     BIT1 
#define CAN_IER_EIE                     BIT2 
#define CAN_IER_DOIE                    BIT3 
#define CAN_IER_WUIE                    BIT4 
#define CAN_IER_EPIE                    BIT5 
#define CAN_IER_ALIE                    BIT6 
#define CAN_IER_BEIE                    BIT7 

/** 
  * @brief  CAN  init structure definition
  */
typedef struct
{
    uint32_t CAN_Mode;         /*!< Specifies the CAN operating mode.
                                This parameter can be a value of 
                                @ref CAN_mode  e.g:CAN_Mode_Normal   CAN_Mode_Normal*/ 
    
    uint32_t CAN_SJW;          /*!< Specifies the maximum number of time quanta 
                                the CAN hardware is allowed to lengthen or 
                                shorten a bit to perform resynchronization.
                                This parameter can be a value of 
                                @ref CAN_SJW  e.g:CAN_SJW_1tq--CAN_SJW_4tq*/
    
    uint32_t CAN_BRP ;          /*!< Specifies the number of time quanta in Bit 
                                Segment 1. This parameter can be a value between 0 and 63 */
    
    uint32_t CAN_TSEG1;          /*!< Specifies the number of time quanta in Bit 
                                Segment 1. This parameter can be a value of 
                                @ref CAN_TSEG1  e.g: CAN_TSEG1_1tq-CAN_TSEG1_16tq*/
    
    uint32_t CAN_TSEG2;          /*!< Specifies the number of time quanta in Bit 
                                Segment 2.This parameter can be a value of 
                                @ref CAN_TSEG2  e.g:CAN_TSEG1_1tq-CAN_TSEG16_tq*/
    
    uint32_t CAN_SAM ;           /*!< Specifies the CAN operating mode.
                                This parameter can be a value of 
                                @ref CAN_SAM e.g:CAN_SAM_1time  CAN_SAM_3time*/
                                
    uint32_t CAN_ABOM ;          /*!< CAN BUF OFF ERROR RECOVER
                                @ref CAN_ABOM e.g:CAN_ABOM_ENABLE  CAN_ABOM_DISABLE*/                          
} CAN_InitTypeDef;


/** 
  * @brief  CAN filter init structure definition
  */

typedef struct
{
    uint32_t CAN_FilterMode;            /*!< Specifies the filter mode to be initialized.
                                        This parameter can be a value of 
                                        @ref CAN_filter_mode  e.g:CAN_FilterMode_Dual    CAN_FilterMode_Single*/
    
    uint32_t CAN_FilterId1;             /*!< Specifies the filter identification number 
                                        This parameter can be a value between 0x0000 and 0x1FFFFFFF */
    
    uint32_t CAN_FilterId2;             /*!< Specifies the filter identification number 
                                        only CAN_FilterMode=CAN_FilterMode_Dual Enable
                                        This parameter can be a value between 0x0000 and 0x1FFFFFFF */
    
    uint32_t CAN_FilterMaskId1;         /*!< Specifies the filter identification mask number 
                                        This parameter can be a value between 0x0000 and 0x1FFFFFFF */
    
    uint32_t CAN_FilterMaskId2;         /*!< Specifies the filter identification mask number 
                                        only CAN_FilterMode=CAN_FilterMode_Dual Enable
                                        This parameter can be a value between 0x0000 and 0x1FFFFFFF */
} CAN_FilterInitTypeDef;



/** 
  * @brief  CAN RxTxMessege structure definition
  */
typedef struct
{
    uint32_t StdId;                     /*!< Specifies the standard identifier.
                                        This parameter can be a value between 0 to 0x7FF. */
    
    uint32_t ExtId;                     /*!< Specifies the extended identifier.
                                        This parameter can be a value between 0 to 0x1FFFFFFF. */
    
    uint32_t IDE;                        /*!< Specifies the type of identifier for the message that 
                                        will be transmitted. This parameter can be a value 
                                        of @ref CAN_identifier_type  e.g: CAN_Id_Standard  CAN_Id_Extended*/
    
    uint32_t RTR;                        /*!< Specifies the type of frame for the message that will 
                                        be transmitted. This parameter can be a value of 
                                        @ref CAN_remote_transmission    e.g: CAN_RTR_Data  CAN_RTR_Remote */
    
    uint32_t DLC;                        /*!< Specifies the length of the frame that will be 
                                        transmitted. This parameter can be a value between 0 to 8 */
    
    uint8_t Data[8];                    /*!< Contains the data to be transmitted. It ranges from 0 to 0xFF. */
} CanTxRxMsg;


/**
  * @brief  CAN handle Structure definition
  */
typedef struct __CAN_HandleTypeDef
{
    CAN_TypeDef         *Instance;                 /*!< Register base address */
    
    CAN_InitTypeDef      Init;                      /*!< CAN required parameters */
    
    CanTxRxMsg          *RxMessage;                 /*!< CAN RxMessage */
    
    void (*CAN_ReceiveIT_Callback)(struct __CAN_HandleTypeDef *hcan);     /* CAN  ReceiveIT complete callback */ 
    
    void (*CAN_TransmitIT_Callback)(struct __CAN_HandleTypeDef *hcan);     /* CAN TransmitIT complete callback */ 
    
} CAN_HandleTypeDef;


#define CAN_InitStatus_Failed              ((uint8_t)0x00) /*!< CAN initialization failed */
#define CAN_InitStatus_Success             ((uint8_t)0x01) /*!< CAN initialization OK */

#define CAN_Status_Failed    ((uint8_t)0x00)                /*!< CAN entering the specific mode failed */
#define CAN_Status_Success   ((uint8_t)!CAN_Status_Failed)   /*!< CAN entering the specific mode Succeed */


//typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;

#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

#define IS_CAN_ALL_PERIPH(PERIPH) (((PERIPH) == CAN1) || \
                                   ((PERIPH) == CAN2))

/** @defgroup CAN_identifier_type 
  * @{
  */

#define CAN_Id_Standard             ((uint32_t)0x00000000)  /*!< Standard Id */
#define CAN_Id_Extended             ((uint32_t)0x00000001)  /*!< Extended Id */
#define IS_CAN_IDTYPE(IDTYPE) (((IDTYPE) == CAN_Id_Standard) || \
                               ((IDTYPE) == CAN_Id_Extended))
/**
  * @}
  */
/** @defgroup CAN_remote_transmission
  * @{
  */

#define CAN_RTR_Data                ((uint32_t)0x00000000)  /*!< Data frame */
#define CAN_RTR_Remote              ((uint32_t)0x00000001)  /*!< Remote frame */
#define IS_CAN_RTR(RTR) (((RTR) == CAN_RTR_Data) || ((RTR) == CAN_RTR_Remote))

/**
  * @}
  */
/** @defgroup CAN_TxRxMessege 
  * @{
  */

#define IS_CAN_STDID(STDID)   ((STDID) <= ((uint32_t)0x7FF))
#define IS_CAN_EXTID(EXTID)   ((EXTID) <= ((uint32_t)0x1FFFFFFF))
#define IS_CAN_DLC(DLC)       ((DLC) <=   ((uint8_t)0x08))


#define IS_CAN_GET_FLAG(FLAG) (((FLAG) == CAN_SR_RBS)  || ((FLAG) == CAN_SR_DOS)   || \
                               ((FLAG) == CAN_SR_TBS)  || ((FLAG) == CAN_SR_TCS)   || \
                               ((FLAG) == CAN_SR_RS)  || ((FLAG) == CAN_SR_TS)  || \
                               ((FLAG) == CAN_SR_ES)  || ((FLAG) == CAN_SR_BS))	

#define IS_CAN_BRP(BRP) 	(((BRP) >= 0) && ((BRP) <= 63))

/**
  * @defgroup CAN_Mode 
  * @{
  */  
#define CAN_Mode_Normal         ((uint8_t)0x00) /*!< Normal mode */
#define CAN_Mode_SlefTest       ((uint8_t)0x01) /*!< SlefTest mode */

#define IS_CAN_MODE(MODE) (((MODE) == CAN_Mode_Normal) ||\
                            ((MODE) == CAN_Mode_SlefTest))
/**
  * @}
  */

/**
  * @defgroup CAN_ABOM 
  * @{
  */  
#define CAN_ABOM_ENABLE         ((uint8_t)0x00) /*!< Normal mode */
#define CAN_ABOM_DISABLE        ((uint8_t)0x01) /*!< SlefTest mode */
/**
  * @}
  */


/**
  * @defgroup CAN_Operating_Mode 
  * @{
  */  
#define CAN_OperatingMode_Normal           ((uint8_t)0x00) /*!< Initialization mode */
#define CAN_OperatingMode_Initialization   ((uint8_t)0x01) /*!< Normal mode */
#define CAN_OperatingMode_Listen           ((uint8_t)0x02) /*!< Listen mode */
#define CAN_OperatingMode_SelfTest           ((uint8_t)0x04) /*!< Listen mode */
#define CAN_OperatingMode_Sleep            ((uint8_t)0x10) /*!< sleep mode */


#define IS_CAN_OPERATING_MODE(MODE) (((MODE) == CAN_OperatingMode_Initialization) ||\
                                    ((MODE) == CAN_OperatingMode_Normal)|| \
                                    ((MODE) == CAN_OperatingMode_Sleep)|| \
                                    ((MODE) == CAN_OperatingMode_SelfTest)|| \
                                    ((MODE) == CAN_OperatingMode_Listen))
/**
  * @}
  */

/** @defgroup CAN_SAM 
  * @{
  */

#define CAN_SAM_1time       ((uint8_t)0x00)  /*!< 1 time quantum */
#define CAN_SAM_3time       ((uint8_t)0x01)  /*!< 2 time quantum */

#define IS_CAN_SAM(SAM)     (((SAM) == CAN_SAM_1time) || ((SAM) == CAN_SAM_3time))
/**
  * @}
  */


/** @defgroup CAN_synchronisation_jump_width 
  * @{
  */

#define CAN_SJW_1tq         ((uint8_t)0x00)  /*!< 1 time quantum */
#define CAN_SJW_2tq         ((uint8_t)0x01)  /*!< 2 time quantum */
#define CAN_SJW_3tq         ((uint8_t)0x02)  /*!< 3 time quantum */
#define CAN_SJW_4tq         ((uint8_t)0x03)  /*!< 4 time quantum */

#define IS_CAN_SJW(SJW) (((SJW) == CAN_SJW_1tq) || ((SJW) == CAN_SJW_2tq)|| \
                         ((SJW) == CAN_SJW_3tq) || ((SJW) == CAN_SJW_4tq))
/**
  * @}
  */
	
	/** @defgroup CAN_time_quantum_in_bit_segment_1 
  * @{
  */

#define CAN_TSEG1_1tq       ((uint8_t)0x00)  /*!< 1 time quantum */
#define CAN_TSEG1_2tq       ((uint8_t)0x01)  /*!< 2 time quantum */
#define CAN_TSEG1_3tq       ((uint8_t)0x02)  /*!< 3 time quantum */
#define CAN_TSEG1_4tq       ((uint8_t)0x03)  /*!< 4 time quantum */
#define CAN_TSEG1_5tq       ((uint8_t)0x04)  /*!< 5 time quantum */
#define CAN_TSEG1_6tq       ((uint8_t)0x05)  /*!< 6 time quantum */
#define CAN_TSEG1_7tq       ((uint8_t)0x06)  /*!< 7 time quantum */
#define CAN_TSEG1_8tq       ((uint8_t)0x07)  /*!< 8 time quantum */
#define CAN_TSEG1_9tq       ((uint8_t)0x08)  /*!< 9 time quantum */
#define CAN_TSEG1_10tq      ((uint8_t)0x09)  /*!< 10 time quantum */
#define CAN_TSEG1_11tq      ((uint8_t)0x0A)  /*!< 11 time quantum */
#define CAN_TSEG1_12tq      ((uint8_t)0x0B)  /*!< 12 time quantum */
#define CAN_TSEG1_13tq      ((uint8_t)0x0C)  /*!< 13 time quantum */
#define CAN_TSEG1_14tq      ((uint8_t)0x0D)  /*!< 14 time quantum */
#define CAN_TSEG1_15tq      ((uint8_t)0x0E)  /*!< 15 time quantum */
#define CAN_TSEG1_16tq      ((uint8_t)0x0F)  /*!< 16 time quantum */

#define IS_CAN_TSEG1(TSEG1) ((TSEG1) <= CAN_TSEG1_16tq)
/**
  * @}
  */

/** @defgroup CAN_time_quantum_in_bit_segment_2 
  * @{
  */

#define CAN_TSEG2_1tq       ((uint8_t)0x00)  /*!< 1 time quantum */
#define CAN_TSEG2_2tq       ((uint8_t)0x01)  /*!< 2 time quantum */
#define CAN_TSEG2_3tq       ((uint8_t)0x02)  /*!< 3 time quantum */
#define CAN_TSEG2_4tq       ((uint8_t)0x03)  /*!< 4 time quantum */
#define CAN_TSEG2_5tq       ((uint8_t)0x04)  /*!< 5 time quantum */
#define CAN_TSEG2_6tq       ((uint8_t)0x05)  /*!< 6 time quantum */
#define CAN_TSEG2_7tq       ((uint8_t)0x06)  /*!< 7 time quantum */
#define CAN_TSEG2_8tq       ((uint8_t)0x07)  /*!< 8 time quantum */
#define IS_CAN_TSEG2(TSEG) ((TSEG) <= CAN_TSEG2_8tq)
/**
  * @}
  */

	  
/** @defgroup CAN_filter_mode 
  * @{
  */
#define CAN_FilterMode_Dual         ((uint8_t)0x00)  /*!< identifier list mode */
#define CAN_FilterMode_Single       ((uint8_t)0x01)  /*!< identifier/mask mode */
#define IS_CAN_FILTER_MODE(MODE) (((MODE) == CAN_FilterMode_Dual) || \
                                  ((MODE) == CAN_FilterMode_Single))

	/**
  * @}
  */

/** @defgroup CAN_ErrorCode 
  * @{
  */
#define CAN_ErrorType_ErrCode         ((uint8_t)0xC0)  /*!< identifier list mode */
#define CAN_ErrorType_Direction       ((uint8_t)0x20)  /*!< identifier/mask mode */
#define CAN_ErrorType_SegCode      		 ((uint8_t)0x1F)  /*!< identifier/mask mode */
#define IS_CAN_ErrorType(ErrorType) (((ErrorType) == CAN_ErrorType_ErrCode) || \
                                    ((ErrorType) == CAN_ErrorType_Direction)|| \
                                    ((ErrorType) == CAN_ErrorType_SegCode))
/**
  * @}
  */
/* Initialization and Configuration functions *********************************/
uint8_t CAN_Init(CAN_TypeDef* CANx, CAN_InitTypeDef* CAN_InitStruct);

void CAN_DeInit(CAN_TypeDef* CANx);

void CAN_FilterInit(CAN_TypeDef* CANx,CAN_FilterInitTypeDef* CAN_FilterInitStruct);

///* Transmit functions *********************************************************/
uint8_t CAN_Transmit(CAN_TypeDef* CANx, CanTxRxMsg* TxMessage);

void CAN_CancelTransmit(CAN_TypeDef* CANx);

///* Receive functions **********************************************************/
void CAN_Receive(CAN_TypeDef* CANx, CanTxRxMsg* RxMessage);

int8_t CAN_GetReceiveFiFoCounter(CAN_TypeDef* CANx);

int8_t CAN_GetReceiveFiFoAddr(CAN_TypeDef* CANx);

void CAN_ReleaseReceiveFIFO(CAN_TypeDef* CANx);

void CAN_GetRxMessage(CAN_TypeDef* CANx, CanTxRxMsg* RxMessage);

///* Operation modes functions **************************************************/
uint8_t CAN_OperatingModeRequest(CAN_TypeDef* CANx, uint8_t CAN_OperatingMode);

void CAN_ClearOverload(CAN_TypeDef* CANx);

void CAN_SelfReceive(CAN_TypeDef* CANx);

uint8_t CAN_Sleep(CAN_TypeDef* CANx);

uint8_t CAN_WakeUp(CAN_TypeDef* CANx);

///* Error Code management functions **********************************/
int8_t CAN_GetErrorCode(CAN_TypeDef* CANx,uint32_t Error_Type);

int8_t CAN_GetErrorAlarmCounter(CAN_TypeDef* CANx);

int8_t CAN_GetArbitrationErrorPosition(CAN_TypeDef* CANx);

int8_t CAN_GetReceiveFiFoCounter(CAN_TypeDef* CANx);

int8_t CAN_GetTransmitErrorCounter(CAN_TypeDef* CANx);

///* Interrupts and flags management functions **********************************/
void CAN_ITConfig(CAN_TypeDef* CANx, uint32_t CAN_IT, FunctionalState NewState);
	
ITStatus CAN_GetITStatus(CAN_TypeDef* CANx, uint32_t CAN_IT);

FlagStatus CAN_GetFlagStatus(CAN_TypeDef* CANx, uint32_t CAN_FLAG);

#endif
