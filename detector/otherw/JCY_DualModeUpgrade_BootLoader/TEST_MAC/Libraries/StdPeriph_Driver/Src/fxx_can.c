/*
  ******************************************************************************
  * @file    fxx_can.c
  * @version V1.0.0
  * @date    2020
  * @brief   CAN HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Universal Asynchronous Receiver Transmitter Peripheral (CAN).
  *           @ Initialization and de-initialization functions
  *           @ IO operation functions
  *           @ Peripheral Control functions
  ******************************************************************************
*/
#include "fxx_std.h"

/*********************************************************************************
* Function    : CAN_OperatingModeRequest
* Description : Select the CAN Operation mode.
* Input       : CANx : CAN_TypeDef
* Input       : CAN_OperatingMode:CAN Operating Mode. This parameter can be one  of @ref CAN_OperatingMode enumeration.
* Output      : status
* Author      : CWT                         Data : 2020
**********************************************************************************/
uint8_t CAN_OperatingModeRequest(CAN_TypeDef* CANx, uint8_t CAN_OperatingMode)
{
    uint8_t status = CAN_Status_Failed ;
	
    /* Check the parameters */
    assert_param(IS_CAN_ALL_PERIPH(CANx));
    assert_param(IS_CAN_OPERATING_MODE(CAN_OperatingMode));
    if (CAN_OperatingMode == CAN_OperatingMode_Initialization)
    {
        CANx->MOD |= CAN_OperatingMode_Initialization;  // enter Initialization 
        if ((CANx->MOD & CAN_MOD_RM) != CAN_OperatingMode_Initialization)
        {
            status = CAN_Status_Failed;
        }
        else
        {
            status = CAN_Status_Success;
        }
    }
    else if(CAN_OperatingMode == CAN_OperatingMode_Normal)
    {
        CANx->MOD &=~ CAN_OperatingMode_Initialization;   //1-->0 enter Normal 
        if ((CANx->MOD & CAN_MOD_RM) != CAN_OperatingMode_Normal)
        {
            status = CAN_Status_Failed;
        }
        else
        {
            status = CAN_Status_Success;
        }
    }
    else if (CAN_OperatingMode == CAN_OperatingMode_Sleep)
    {
        CANx->MOD |= CAN_OperatingMode_Sleep;  // enter Normal 
        if ((CANx->MOD & CAN_MOD_SM) != CAN_OperatingMode_Sleep)
        {
            status = CAN_Status_Failed;
        }
        else
        {
            status = CAN_Status_Success;
        }
    }
    else if(CAN_OperatingMode == CAN_OperatingMode_Listen)
    {
        CANx->MOD |= CAN_OperatingMode_Listen;  // enter Normal 
        if((CANx->MOD & CAN_MOD_LOM) != CAN_OperatingMode_Listen)
        {
            status = CAN_Status_Failed;
        }
        else
        {
            status = CAN_Status_Success;
        }
    }
		else if(CAN_OperatingMode == CAN_OperatingMode_SelfTest)
    {
        CANx->MOD |= CAN_OperatingMode_SelfTest;  // enter Normal 
        if((CANx->MOD & CAN_MOD_STM) != CAN_OperatingMode_SelfTest)
        {
            status = CAN_Status_Failed;
        }
        else
        {
            status = CAN_Status_Success;
        }
    }
    else
    {
        status = CAN_Status_Failed;
    }
    return  (uint8_t) status;
}

/*********************************************************************************
* Function    : CAN_Init
* Description :	Initializes the CAN peripheral according to the specified  parameters in the CAN_HandleTypeDef..
* Input       : CANx : CAN_TypeDef   CAN_InitStruct:CAN_InitTypeDef
* Output      : status
* Author      : CWT                         Data : 2020
**********************************************************************************/
uint8_t CAN_Init(CAN_TypeDef* CANx, CAN_InitTypeDef* CAN_InitStruct)
{
    uint8_t InitStatus = CAN_InitStatus_Failed;
    /* Check the parameters */
    assert_param(IS_CAN_ALL_PERIPH(CANx)) ;
    assert_param(IS_CAN_MODE(CAN_InitStruct->CAN_Mode)) ;
    assert_param(IS_CAN_SJW(CAN_InitStruct->CAN_SJW)) ;
    assert_param(IS_CAN_TSEG1(CAN_InitStruct->CAN_TSEG1));
    assert_param(IS_CAN_TSEG2(CAN_InitStruct->CAN_TSEG2)) ;
    assert_param(IS_CAN_BRP(CAN_InitStruct->CAN_BRP));
    assert_param(IS_CAN_SAM(CAN_InitStruct->CAN_SAM));
    /* Reset the CANx */
    if(CANx==CAN1)
    {
        System_Module_Reset(RST_CAN1);
		System_Module_Enable(EN_CAN1);
    }
    else
    {
        System_Module_Reset(RST_CAN2);
		System_Module_Enable(EN_CAN2);
    }

    CAN_OperatingModeRequest(CANx,CAN_OperatingMode_Initialization);//enter CAN_OperatingMode_Initialization
    if(CAN_InitStruct->CAN_ABOM==CAN_ABOM_ENABLE)
    {
        /* Enable the CAN BUS OFF ERROR interrupt */
        CANx->IER |= CAN_IER_BEIE;
    }
    CANx->BTR0=0xff;
    CANx->BTR0=(CAN_InitStruct->CAN_SJW<<6)|(CAN_InitStruct->CAN_BRP);
    CANx->BTR1=(CAN_InitStruct->CAN_SAM<<7)|(CAN_InitStruct->CAN_TSEG2<<4)|(CAN_InitStruct->CAN_TSEG1);
    CAN_OperatingModeRequest(CANx,CAN_OperatingMode_Normal);//enter CAN_OperatingMode_Normal
    return CAN_InitStatus_Success;
}

/*********************************************************************************
* Function    : CAN_DeInit
* Description :	Deinitializes the CAN peripheral registers to their default
*               reset values.
* Input       : CANx : CAN_TypeDef
* Output      : none
* Author      : CWT                         Data : 2020
**********************************************************************************/
void CAN_DeInit(CAN_TypeDef* CANx)
{
		if(CANx==CAN1)
		{
				/* Reset CAN clock */
				System_Module_Disable(EN_CAN1);
				}
		else if(CANx==CAN2)
				{
				/* Reset CAN clock */
				System_Module_Disable(EN_CAN2);
		}
    
    /* Reset the CAN peripheral */
    SET_BIT(CANx->MOD, CAN_MOD_RM);
    
}
/*********************************************************************************
* Function    : CAN_Transmit
* Description :	Initiates the transmission of a message.
* Input       : CANx : CAN_TypeDef
* Input       : TxMessage : ppointer to a structure which contains CAN Id, CAN
  *                         DLC and CAN data.
* Output      : Status
* Author      : CWT                         Data : 2020
**********************************************************************************/
uint8_t CAN_Transmit(CAN_TypeDef* CANx, CanTxRxMsg* TxMessage)
{
    uint8_t i = 0;
    uint8_t can_id[4];  
    uint32_t frame_header;  
    /* Check the parameters */
    assert_param(IS_CAN_ALL_PERIPH(CANx));
    assert_param(IS_CAN_IDTYPE(TxMessage->IDE));
    assert_param(IS_CAN_RTR(TxMessage->RTR));
    assert_param(IS_CAN_DLC(TxMessage->DLC));
    /* Set up the DLC */
    frame_header =TxMessage->DLC & 0x0F;  // standard data frame 
    /* Set up the Id */
    if(TxMessage->IDE==CAN_Id_Standard)//Standard ID
    {
        can_id[0] = TxMessage->StdId >>3; 
        can_id[1] = (TxMessage->StdId&0x07)<<5;   
        for(i=0;i<2;i++)
        {
            CANx->DF.DATABUF[1+i] = can_id[i];  
        }
    }
    else//Id_Extended
    {
        can_id[0] = TxMessage->ExtId>>21; 
        can_id[1] = (TxMessage->ExtId&0x1FE000)>>13;   
        can_id[2] = (TxMessage->ExtId&0x1FE0)>>5; 
        can_id[3] = (TxMessage->ExtId&0x1F)<<3;
        frame_header |= (CAN_Id_Extended<<7);  // extended data frame 
        for(i=0;i<4;i++)
        {
            CANx->DF.DATABUF[1+i] = can_id[i];  
        }
    }
    if(TxMessage->RTR==CAN_RTR_Data)//CAN_RTR_Data
    {
        frame_header&=~(CAN_RTR_Remote<<6);
        for(i=0; i<TxMessage->DLC; i++)  
        {
            CANx->DF.DATABUF[3+(TxMessage->IDE*2)+i] = TxMessage->Data[i];      
        }
    }
    else//CAN_RTR_Remote
    {
        frame_header|=(CAN_RTR_Remote<<6);
    }
    CANx->DF.DATABUF[0]=frame_header;
    CANx->CMR = CAN_CMR_SRR;  // transfer request
    while((CANx->SR & CAN_SR_TCS)==0x00); //wait for send ok	
    return CAN_Status_Success;
}

/*********************************************************************************
* Function    : CAN_CancelTransmit
* Description :	Cancels a transmit request.
* Input       : CANx : CAN_TypeDef
* Output      : 
* Author      : CWT                         Data : 2020
**********************************************************************************/
void CAN_CancelTransmit(CAN_TypeDef* CANx)
{
    /* Check the parameters */
    assert_param(IS_CAN_ALL_PERIPH(CANx)) ;
    /* abort transmission */
    CANx->CMR |= CAN_CMR_AT; //Abort Transmission 
}

/*********************************************************************************
* Function    : CAN_Receive
* Description :	Receives a message.
* Input       : CANx : CAN_TypeDef
* Input       : RxMessage : pointer to a structure receive message which contains 
*                           CAN Id, CAN DLC, CAN datas  .
* Output      : 
* Author      : CWT                         Data : 2020
**********************************************************************************/
void CAN_Receive(CAN_TypeDef* CANx, CanTxRxMsg* RxMessage)
{
    /* Check the parameters */
    assert_param(IS_CAN_ALL_PERIPH(CANx));
    while(!(CANx->SR & CAN_SR_RBS));
    CAN_GetRxMessage(CANx, RxMessage);
}

/*********************************************************************************
* Function    : CAN_GetRxMessage
* Description :	Receives a message.
* Input       : CANx : CAN_TypeDef
* Input       : RxMessage : pointer to a structure receive message which contains 
*                           CAN Id, CAN DLC, CAN datas  .
* Output      : 
* Author      : CWT                         Data : 2020
**********************************************************************************/
void CAN_GetRxMessage(CAN_TypeDef* CANx, CanTxRxMsg* RxMessage)
{
    uint8_t i=0;
    /* Check the parameters */
    assert_param(IS_CAN_ALL_PERIPH(CANx)) ;
    if(0 == (CANx->SR & CAN_SR_RBS) ) return;  // receive fifo not empty  
    /* Get the IDE */
    RxMessage->IDE = (uint8_t)(0x80 & CANx->DF.DATABUF[0])>>7;
    /* Get the RTR */
    RxMessage->RTR = (uint8_t)(0x40 & CANx->DF.DATABUF[0])>>6;
    /* Get the DLC */
    RxMessage->DLC = (uint8_t)0x0F & CANx->DF.DATABUF[0];
    if (RxMessage->IDE == CAN_Id_Standard)
    {
        RxMessage->StdId = (uint32_t)(( CANx->DF.DATABUF[1]<<8) |  CANx->DF.DATABUF[2])>>5;;
        for(i=0; i<RxMessage->DLC; i++)  
        {
            RxMessage->Data[i] = CANx->DF.DATABUF[3+i];       
        }
    }
    else
    {
        RxMessage->ExtId = (uint32_t)(( CANx->DF.DATABUF[1]<<24) | ( CANx->DF.DATABUF[2]<<16) | ( CANx->DF.DATABUF[3]<<8) | (CANx->DF.DATABUF[4] ))>>3;;
        for(i=0; i<RxMessage->DLC; i++)  
        {
            RxMessage->Data[i] = CANx->DF.DATABUF[5+i];       
        }
    }
    /* Release the FIFO */
    CANx->CMR |= CAN_CMR_RRB; //Release Receive Buffer
}

/*********************************************************************************
* Function    : CAN_FilterInit
* Description :	Initializes the CAN peripheral according to the specified  parameters in the CAN_FilterInitStruct.
* Input       : CANx : CAN_TypeDef
* Input       : CAN_FilterInitStruct : pointer to a CAN_FilterInitTypeDef structure that contains the configuration 
*                                       information.
* Output      : 
* Author      : CWT                         Data : 2020
**********************************************************************************/
void CAN_FilterInit(CAN_TypeDef* CANx,CAN_FilterInitTypeDef* CAN_FilterInitStruct)
{
    CAN_OperatingModeRequest(CANx,CAN_OperatingMode_Initialization);//enter CAN_OperatingMode_Initialization
    /* Filter Mode */
    if (CAN_FilterInitStruct->CAN_FilterMode ==CAN_FilterMode_Dual) /*Dual mode*/
    {
        CANx->MOD &= ~CAN_MOD_AFM;
        /*Dual mode ACR set*/    
        CANx->DF.FILTER.ACR[0] = (CAN_FilterInitStruct->CAN_FilterId1&0x1FE00000)>>21;	/*Dual mode ACR0=ID28...ID21 of ID1*/
        CANx->DF.FILTER.ACR[1] = (CAN_FilterInitStruct->CAN_FilterId1&0x1FE000)>>13; 		/*Dual mode ACR0=ID20...ID13 of ID1*/
        CANx->DF.FILTER.ACR[2] = (CAN_FilterInitStruct->CAN_FilterId2&0x1FE00000)>>21;  /*Dual mode ACR0=ID28...ID21 of ID2*/
        CANx->DF.FILTER.ACR[3] = (CAN_FilterInitStruct->CAN_FilterId2&0x1FE000)>>13;     /*Dual mode ACR0=ID20...ID13 of ID2*/
        /*Dual mode AMR set*/   
        CANx->DF.FILTER.AMR[0] = (CAN_FilterInitStruct->CAN_FilterMaskId1)>>24; 
        CANx->DF.FILTER.AMR[1] = (CAN_FilterInitStruct->CAN_FilterMaskId1&0xFF0000)>>16; 
        CANx->DF.FILTER.AMR[2] = (CAN_FilterInitStruct->CAN_FilterMaskId2)>>24; 
        CANx->DF.FILTER.AMR[3] = (CAN_FilterInitStruct->CAN_FilterMaskId2&0xFF0000)>>16; 
    }
    else /*Single mode*/
    {
        CANx->MOD |= CAN_MOD_AFM;
        /*Single mode ACR set*/    
        CANx->DF.FILTER.ACR[0] = (CAN_FilterInitStruct->CAN_FilterId1&0x1FE00000)>>21;	/*Single mode ACR0=ID28...ID21*/
        CANx->DF.FILTER.ACR[1] = (CAN_FilterInitStruct->CAN_FilterId1&0x1FE000)>>13; 		/*Single mode ACR1=ID20...ID13*/
        CANx->DF.FILTER.ACR[2] = (CAN_FilterInitStruct->CAN_FilterId1&0x1FE0)>>5; 			/*Single mode ACR2=ID12...ID5*/
        CANx->DF.FILTER.ACR[3] = (CAN_FilterInitStruct->CAN_FilterId1&0x1F)<<3;  				/*Single mode ACR3=ID4...ID0*/
        /*Single mode AMR set*/   
        CANx->DF.FILTER.AMR[0] = (CAN_FilterInitStruct->CAN_FilterMaskId1)>>24; 
        CANx->DF.FILTER.AMR[1] = (CAN_FilterInitStruct->CAN_FilterMaskId1&0xFF0000)>>16; 
        CANx->DF.FILTER.AMR[2] = (CAN_FilterInitStruct->CAN_FilterMaskId1&0xFF00)>>8; 
        CANx->DF.FILTER.AMR[3] = (CAN_FilterInitStruct->CAN_FilterMaskId1&0xFF);   
    }

    CAN_OperatingModeRequest(CANx,CAN_OperatingMode_Normal);//enter CAN_OperatingMode_Initialization
}


/*********************************************************************************
* Function    : CAN_Sleep
* Description :	Enters the sleep mode.
* Input       : CANx :CAN_TypeDef
* Output      : status
* Author      : CWT                         Data : 2020
**********************************************************************************/
uint8_t CAN_Sleep(CAN_TypeDef* CANx)
{
    uint8_t status;
    /* Check the parameters */
    assert_param(IS_CAN_ALL_PERIPH(CANx));
    /* Request Sleep mode */
    CANx->MOD |= CAN_MOD_SM; //Enter Sleep Mode
   
    /* Sleep mode status */
    if ((CANx->MOD & CAN_MOD_SM) == CAN_MOD_SM)
    {
    /* Sleep mode entered */
        status= CAN_Status_Success;
    }else
    {
        status=CAN_Status_Failed;
    }
    /* return sleep mode status */
    return status;
}

/*********************************************************************************
* Function    : CAN_WakeUp
* Description :	Wakes the CAN up.
* Input       : CANx :CAN_TypeDef
* Output      : status
* Author      : CWT                         Data : 2020
**********************************************************************************/
uint8_t CAN_WakeUp(CAN_TypeDef* CANx)
{
    uint8_t status;
    /* Check the parameters */
    assert_param(IS_CAN_ALL_PERIPH(CANx));
    /* sleep wake mode */
    CANx->MOD &=~ CAN_MOD_SM; //Enter Sleep Mode
   
    /* sleep wake status */
    if ((CANx->MOD & CAN_MOD_SM)== CAN_MOD_SM)
    {
        /* sleep wake not entered */
        status= CAN_Status_Failed;
    }else
    {
        status=CAN_Status_Success;
    }
    /* return sleep mode status */
    return status;
}

/*********************************************************************************
* Function    : CAN_GetTransmitErrorCounter
* Description :	Returns the CANx Transmit Error Counter(TXERR).
* Input       : CANx : CAN_TypeDef
* Output      : counter
* Author      : CWT                         Data : 2020
**********************************************************************************/
int8_t CAN_GetTransmitErrorCounter(CAN_TypeDef* CANx)
{
    uint8_t counter=0;
    /* Check the parameters */
    assert_param(IS_CAN_ALL_PERIPH(CANx));
    /* Get the  CANx Transmit Error Counter(TXERR) */
    counter = (uint8_t)(CANx->TXERR);
    /* Return the CANx Transmit Error Counter(TXERR) */
    return counter;
}


/*********************************************************************************
* Function    : CAN_GetReceiveErrorCounter
* Description :	Returns the  CANx Receive Error Counter(RXERR).
* Input       : CANx : CAN_TypeDef
* Output      : counter
* Author      : CWT                         Data : 2020
**********************************************************************************/
int8_t CAN_GetReceiveErrorCounter(CAN_TypeDef* CANx)
{
    uint8_t counter=0;
    /* Check the parameters */
    assert_param(IS_CAN_ALL_PERIPH(CANx));
    /* Get the  CANx Receive Error Counter(RXERR) */
    counter = (uint8_t)(CANx->RXERR);
    /* Return the CANx Receive Error Counter(RXERR) */
    return counter;
}


/*********************************************************************************
* Function    : CAN_GetErrorCode
* Description :	Returns the CANx's  error code (ECC).
* Input       : CANx : CAN_TypeDef
* Input       : Error_Type:This parameter can be one of the following flags: 
*														CAN_ErrorType_SegCode
*														CAN_ErrorType_Direction
*														CAN_ErrorType_ErrCode
* Output      : ErrorCode
* Author      : CWT                         Data : 2020
**********************************************************************************/
int8_t CAN_GetErrorCode(CAN_TypeDef* CANx,uint32_t Error_Type)
{
    uint8_t ErrorCode=0;
    assert_param(IS_CAN_ALL_PERIPH(CANx));
    assert_param(IS_CAN_ErrorType(Error_Type));
    /* Get the CANx  Error SegCode */
    if(Error_Type==CAN_ErrorType_SegCode)
    {
        ErrorCode= (uint8_t)(CANx->ECC & CAN_ErrorType_SegCode);
    }
    /* Get the CANx  Error Direction */
    else if(Error_Type==CAN_ErrorType_Direction)
    {
        ErrorCode= (uint8_t)((CANx->ECC & CAN_ErrorType_Direction)>>5);
    }
    /* Get the CANx  Error ErrCode */
    else
    {
        ErrorCode= (uint8_t)((CANx->ECC & CAN_ErrorType_ErrCode)>>6);
    }
    return ErrorCode;
}

/*********************************************************************************
* Function    : CAN_GetErrorAlarmCounter
* Description :	Returns the  CANx Error  Alarm Counter(EWLR).
* Input       : CANx : CAN_TypeDef
* Output      : counter
* Author      : CWT                         Data : 2020
**********************************************************************************/
int8_t CAN_GetErrorAlarmCounter(CAN_TypeDef* CANx)
{
    uint8_t counter=0;
    /* Check the parameters */
    assert_param(IS_CAN_ALL_PERIPH(CANx));
    /* Get the  CANx Error Alarm Counter(EWLR) */
    counter = (uint8_t)(CANx->EWLR);
    /* Return the CANx Error Alarm Counter(EWLR) */
    return counter;
}

/*********************************************************************************
* Function    : CAN_GetArbitrationErrorPosition
* Description :	Returns the  CANx Arbitration Error Position(ALC).
* Input       : CANx : CAN_TypeDef
* Output      : position
* Author      : CWT                         Data : 2020
**********************************************************************************/
int8_t CAN_GetArbitrationErrorPosition(CAN_TypeDef* CANx)
{
    uint8_t position=0;
    /* Check the parameters */
    assert_param(IS_CAN_ALL_PERIPH(CANx));
    /* Get the  CANx Arbitration Error Counter(ALC) */
    position = (uint8_t)((CANx->ALC)+1);
    /* Return the CANx Arbitration Error Counter(ALC) */
    return position;
}


/*********************************************************************************
* Function    : CAN_GetReceiveFiFoCounter
* Description :	Returns the  CANx Receive FiFo Counter(RMC).
* Input       : CANx : CAN_TypeDef
* Output      : counter
* Author      : CWT                         Data : 2020
**********************************************************************************/
int8_t CAN_GetReceiveFiFoCounter(CAN_TypeDef* CANx)
{
    uint8_t counter=0;
    /* Check the parameters */
    assert_param(IS_CAN_ALL_PERIPH(CANx));
    /* Get the  CANx Receive FiFo Counter(RMC) */
    counter = (uint8_t)(CANx->RMC);
    /* Return the CANx Receive FiFo Counter(RMC) */
    return counter;
}


/*********************************************************************************
* Function    : CAN_GetReceiveFiFoAddr
* Description :	 Returns the  CANx Receive FiFo start address(RBSA).
* Input       : CANx : CAN_TypeDef
* Output      : addr
* Author      : CWT                         Data : 2020
**********************************************************************************/
int8_t CAN_GetReceiveFiFoAddr(CAN_TypeDef* CANx)
{
    uint8_t addr=0;
    /* Check the parameters */
    assert_param(IS_CAN_ALL_PERIPH(CANx));
    /* Get the  CANx Receive FiFo start address(RBSA) */
    addr = (uint8_t)(CANx->RBSA);
    /* Return the CANx Receive FiFo start address(RBSA) */
    return addr;
}


/*********************************************************************************
* Function    : CAN_ReleaseReceiveFIFO
* Description :	Releases the Receive FIFO.
* Input       : CANx : CAN_TypeDef
* Output      : 
* Author      : CWT                         Data : 2020
**********************************************************************************/
void CAN_ReleaseReceiveFIFO(CAN_TypeDef* CANx)
{
    /* Check the parameters */
    assert_param(IS_CAN_ALL_PERIPH(CANx));
    /* Releases the Receive FIFO. */
    CANx->CMR|=CAN_CMR_RRB;	
}


/*********************************************************************************
* Function    : CAN_ClearOverload
* Description : Clear Overload
* Input       : CANx : CAN_TypeDef
* Output      : 
* Author      : CWT                         Data : 2020
**********************************************************************************/
void CAN_ClearOverload(CAN_TypeDef* CANx)
{
    /* Check the parameters */
    assert_param(IS_CAN_ALL_PERIPH(CANx));
    /* Clear Overload. */
    CANx->CMR|=CAN_CMR_CDO;	
}


/*********************************************************************************
* Function    : CAN_SelfReceive
* Description : Slef Receive
* Input       : CANx : CAN_TypeDef
* Output      : 
* Author      : CWT                         Data : 2020
**********************************************************************************/
void CAN_SelfReceive(CAN_TypeDef* CANx)
{
    /* Check the parameters */
    assert_param(IS_CAN_ALL_PERIPH(CANx));
    /* Slef Receive. */
    CANx->CMR|=CAN_CMR_SRR;	
    while((CANx->SR & CAN_SR_TCS)==0x00); //wait for send ok	
}


/*********************************************************************************
* Function    : CAN_ITConfig
* Description : CAN_ITConfig
* Input       : CANx : CAN_TypeDef
*      				  CAN_IT : CAN_IER_RIE  CAN_IER_TIE CAN_IER_EIE CAN_IER_DOIE CAN_IER_WUIE CAN_IER_EPIE CAN_IER_ALIE CAN_IER_BEIE	
*								NewState£ºENABLE   DISABLE
* Output      : 
* Author      : CWT                         Data : 2020
**********************************************************************************/
void CAN_ITConfig(CAN_TypeDef* CANx, uint32_t CAN_IT, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_CAN_ALL_PERIPH(CANx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected CANx interrupt */
    CANx->IER |= CAN_IT;
  }
  else
  {
    /* Disable the selected CANx interrupt */
    CANx->IER &= ~CAN_IT;
  }
}

static ITStatus CheckITStatus(uint32_t CAN_Reg, uint32_t It_Bit)
{
  ITStatus pendingbitstatus = RESET;
  
  if ((CAN_Reg & It_Bit) != (uint32_t)RESET)
  {
    /* CAN_IT is set */
    pendingbitstatus = SET;
  }
  else
  {
    /* CAN_IT is reset */
    pendingbitstatus = RESET;
  }
  return pendingbitstatus;
}

/*********************************************************************************
* Function    : CAN_GetITStatus
* Description : CAN_GetITStatus
* Input       : CANx : CAN_TypeDef
*      				  CAN_IT : CAN_IER_RIE  CAN_IER_TIE CAN_IER_EIE CAN_IER_DOIE CAN_IER_WUIE CAN_IER_EPIE CAN_IER_ALIE CAN_IER_BEIE	
*								NewState£ºENABLE   DISABLE
* Output      : 
* Author      : CWT                         Data : 2020
**********************************************************************************/
ITStatus CAN_GetITStatus(CAN_TypeDef* CANx, uint32_t CAN_IT)
{
		ITStatus itstatus = RESET;
		/* Check the parameters */
		assert_param(IS_CAN_ALL_PERIPH(CANx));
		
		/* check the enable interrupt bit */
	 if((CANx->IER & CAN_IT) != RESET)
	 {
		 /* in case the Interrupt is enabled, .... */
			switch (CAN_IT)
			{
				case CAN_IER_RIE:
								 /* Check CAN_TSR_RQCPx bits */
								 itstatus = CheckITStatus(CANx->IR, CAN_IR_RI);  
					break;
				case CAN_IER_TIE:
								 /* Check CAN_RF0R_FMP0 bit */
								 itstatus = CheckITStatus(CANx->IR, CAN_IR_TI);  
					break;
				case CAN_IER_EIE:
								 /* Check CAN_RF0R_FULL0 bit */
								 itstatus = CheckITStatus(CANx->IR, CAN_IR_EI);  
					break;
				case CAN_IER_DOIE:
								 /* Check CAN_RF0R_FOVR0 bit */
								 itstatus = CheckITStatus(CANx->IR, CAN_IR_DOI);  
					break;
				case CAN_IER_WUIE:
								 /* Check CAN_RF1R_FMP1 bit */
								 itstatus = CheckITStatus(CANx->IR, CAN_IR_WUI);  
					break;
				case CAN_IER_EPIE:
								 /* Check CAN_RF1R_FULL1 bit */
								 itstatus = CheckITStatus(CANx->IR, CAN_IR_EPI);  
					break;
				case CAN_IER_ALIE:
								 /* Check CAN_RF1R_FOVR1 bit */
								 itstatus = CheckITStatus(CANx->IR, CAN_IR_ALI);  
					break;
				case CAN_IER_BEIE:
								 /* Check CAN_MSR_WKUI bit */
								 itstatus = CheckITStatus(CANx->IR, CAN_IR_BEI);  
					break;
				default :
								 /* in case of error, return RESET */
								itstatus = RESET;
								break;
			}
		}
		else
		{
		 /* in case the Interrupt is not enabled, return RESET */
			itstatus  = RESET;
		}
		
		/* Return the CAN_IT status */
		return  itstatus;
}

/*********************************************************************************
* Function    : CAN_GetFlagStatus
* Description : CAN_GetFlagStatus
* Input       : CANx : CAN_TypeDef
*      				  CAN_IT : CAN_SR_RBS  CAN_SR_DOS  CAN_SR_TBS  CAN_SR_TCS CAN_SR_RS CAN_SR_TS CAN_SR_ES CAN_SR_BS
*								NewState£ºENABLE   DISABLE
* Output      : 
* Author      : CWT                         Data : 2020
**********************************************************************************/
FlagStatus CAN_GetFlagStatus(CAN_TypeDef* CANx, uint32_t CAN_FLAG)
{
		FlagStatus bitstatus = RESET;
		
		/* Check the parameters */
		assert_param(IS_CAN_ALL_PERIPH(CANx));
		assert_param(IS_CAN_GET_FLAG(CAN_FLAG));
	 
			/* Check the status of the specified CAN flag */
		 if ((CANx->SR & CAN_FLAG ) != (uint32_t)RESET)
			{ 
				/* CAN_FLAG is set */
				bitstatus = SET;
			}
			else
			{ 
				/* CAN_FLAG is reset */
				bitstatus = RESET;
			}
	 
		/* Return the CAN_FLAG status */
		return  bitstatus;
}
