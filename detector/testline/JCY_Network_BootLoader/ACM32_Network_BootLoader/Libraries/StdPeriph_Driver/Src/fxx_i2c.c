/*
  ******************************************************************************
  * @file    fxx_i2c.c
  * @version V1.0.0
  * @date    2021
  * @brief   I2C  module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Inter Integrated Circuit (I2C) peripheral:
  *           @ Initialization and de-initialization functions
  *           @ IO operation functions
  *           @ Peripheral Control functions
  ******************************************************************************
*/

#include "fxx_std.h"

/************************************************************************
 * function   : I2C_IRQHandler
 * Description: This function handles I2C interrupt request.
 * input      : I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * input      : Slave_Comm : Slave Communication Structure
 ************************************************************************/
__weak void _I2Cx_IRQHandler(I2C_TypeDef* I2Cx,Slave_Comm_TypeDef* Slave_Comm)
{    
    uint32_t i;
    /* Slave ADDR1 Interrupt */
    if (I2C_GetFlagStatus(I2Cx, I2C_SR_RX_ADDR1))
    {		
        /* Clear ADDR1 Interrupt Flag */
		I2C_ClearFlag(I2Cx,I2C_SR_RX_ADDR1);

        /* Slave Transmit */
        if (I2C_GetFlagStatus(I2Cx, I2C_SR_SRW))
        {
            i = 1;

            /* Wait for transmission End*/
            while(!I2C_GetFlagStatus(I2Cx, I2C_SR_MTF));
            /* Clear MTF */
			I2C_ClearFlag(I2Cx,I2C_SR_MTF);
     
            /* BUS BUSY */
            while(I2C_GetFlagStatus(I2Cx, I2C_SR_BUS_BUSY))
            {
                if (i >= Slave_Comm->Tx_Size && Slave_Comm->Tx_Size != 0) 
                {
                    break;
                }

                if (I2C_GetFlagStatus(I2Cx, I2C_SR_MTF)) 
                {
                    /* Clear MTF */
					I2C_ClearFlag(I2Cx,I2C_SR_MTF);
                }

                if (I2C_GetFlagStatus(I2Cx, I2C_SR_TXE))
                {
                    I2Cx->DR = Slave_Comm->Tx_Buffer[i++];
                    Slave_Comm->Tx_Count++;
                }
            }
            /* Set Slave machine is DILE */
            Slave_Comm->Slave_TxState = SLAVE_TX_STATE_IDLE;			
        }
        /* Slave Receive */
        else 
        {
            i = 0;

            /* Wait for transmission End*/
            while(!I2C_GetFlagStatus(I2Cx, I2C_SR_MTF));
            /* Clear MTF */
			I2C_ClearFlag(I2Cx,I2C_SR_MTF);

            /* BUS BUSY */
            while(I2C_GetFlagStatus(I2Cx, I2C_SR_BUS_BUSY))
            {
                /* Receive Data */
                if (I2C_GetFlagStatus(I2Cx, I2C_SR_RXNE))
                {
                    Slave_Comm->Rx_Buffer[i++] = I2Cx->DR;

                    /* Wait for transmission End*/
                    while(!I2C_GetFlagStatus(I2Cx, I2C_SR_MTF));
                    /* Clear MTF */
                    I2C_ClearFlag(I2Cx,I2C_SR_MTF);

                    Slave_Comm->Rx_Count++;

                    if (Slave_Comm->Rx_Size != 0) 
                   {
                        if (i >= Slave_Comm->Rx_Size) 
                        {
                            break;
                        }
                    }
                }
            }       
            /* Set Slave machine is DILE */
            Slave_Comm->Slave_RxState = SLAVE_RX_STATE_IDLE;
        }

        if (Slave_Comm->Slave_RxState == SLAVE_RX_STATE_IDLE && Slave_Comm->Slave_TxState == SLAVE_TX_STATE_IDLE) 
        {
            /* Disable RX_ADDR1_INT_EN */
            I2C_ITConfig(I2Cx,I2C_CR_RX_ADDR1_INT_EN,DISABLE);
        }
    }  
	/* STOP Flag Interrupt */
    if (READ_BIT(I2Cx->SR, I2C_SR_STOPF))
    {
        /* Clear STOPF Interrupt Flag */
        I2Cx->SR = READ_BIT(I2Cx->SR, I2C_SR_STOPF);
        
        /* Clear STOPF */
        CLEAR_BIT(I2Cx->CR, I2C_CR_STOPF_INTEN);
        
        if (Slave_Comm->I2C_STOPF_Callback != NULL) 
        {
            Slave_Comm->I2C_STOPF_Callback();
        }
    }
}

/************************************************************************
 * function   : I2C_Set_Clock_Speed
 * Description: Set I2C Clock Speed
 * input      : I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 *              ClockSpeed: I2C Clock Speed
 ************************************************************************/
void I2C_Set_Clock_Speed(I2C_TypeDef *hi2c, uint32_t ClockSpeed)
{
    uint32_t APB_Clock;

    APB_Clock = System_Get_APBClock();

    hi2c->CLK_DIV = APB_Clock / (4 * ClockSpeed) - 1;
}

/************************************************************************
 * function   : I2C_Init
 * Description: I2c initial with parameters. 
 * input      : I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * input      : I2C_InitStruct: pointer to a I2C_InitTypeDef structure that
 *        contains the configuration information for the specified I2C peripheral.
 ************************************************************************/
void I2C_Init(I2C_TypeDef* I2Cx, I2C_InitTypeDef* I2C_InitStruct)
{
#if (USE_FULL_ASSERT == 1)
	 /* Check the parameters */
	  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	  assert_param(IS_I2C_CLOCK_SPEED(I2C_InitStruct->I2C_ClockSpeed));
	  assert_param(IS_I2C_MODE(I2C_InitStruct->I2C_Mode));
#endif
	
	/* Disable the selected I2C peripheral */
    CLEAR_BIT(I2Cx->CR, I2C_CR_MEN);
	
	switch(I2C_InitStruct->I2C_Mode)
    {
        /* Master Mode */
        case I2C_MODE_MASTER:
        {
            /* Set Master Mode */
            SET_BIT(I2Cx->CR, I2C_CR_MASTER);
            
            /* Set Clock Speed */
            I2C_Set_Clock_Speed(I2Cx, I2C_InitStruct->I2C_ClockSpeed);
            
            /* Set SDA auto change the direction */
            if (I2C_InitStruct->I2C_Tx_Auto_En == TX_AUTO_EN_ENABLE)
                SET_BIT(I2Cx->CR, I2C_CR_TX_AUTO_EN);
            else
                CLEAR_BIT(I2Cx->CR, I2C_CR_TX_AUTO_EN);
            
        }break;
        
        /* Slave Mode */
        case I2C_MODE_SLAVE: 
        {
            SET_BIT(I2Cx->CR, I2C_CR_TXE_SEL);

            /* Set SDA auto change the direction */
            if (I2C_InitStruct->I2C_Tx_Auto_En == TX_AUTO_EN_ENABLE)
                SET_BIT(I2Cx->CR, I2C_CR_TX_AUTO_EN);
            else
                CLEAR_BIT(I2Cx->CR, I2C_CR_TX_AUTO_EN);
            
            /* Set Clock Stretch Mode */
            if (I2C_InitStruct->I2C_No_Stretch_Mode == NO_STRETCH_MODE_NOSTRETCH)
                SET_BIT(I2Cx->CR, I2C_CR_NOSTRETCH);
            else
                CLEAR_BIT(I2Cx->CR, I2C_CR_NOSTRETCH);

            /* Set Address 1 */
            I2Cx->SLAVE_ADDR1 = I2C_InitStruct->I2C_Own_Address;
            
        }break;
        
        default: break; 
    }
}

/************************************************************************
 * function   : I2C_DeInit
 * Description: Deinitializes the I2Cx peripheral registers to their default reset values. 
 * input      : I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * retval     : None
 ************************************************************************/
void I2C_DeInit(I2C_TypeDef* I2Cx)
{
#if (USE_FULL_ASSERT == 1)
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
#endif

	if (I2Cx == I2C1)
	{
	/* Enable I2C1 reset state */
	System_Module_Disable(EN_I2C1);   
	}
	else
	{
	/* Enable I2C2 reset state */
	System_Module_Disable(EN_I2C2);
	}
}

/************************************************************************
 * function   : I2C_Cmd
 * Description: Enables or disables the specified I2C peripheral. 
 * input      : I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * input      : NewState: new state of the I2C START condition generation.
 *       This parameter can be: ENABLE or DISABLE.
 * retval     : None
 ************************************************************************/
void I2C_Cmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
#if (USE_FULL_ASSERT == 1)
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
#endif
	
	if (NewState != DISABLE)
	{
		/* Enable the selected I2C peripheral */
		I2Cx->CR |= I2C_CR_MEN;
	}
	else
	{
		/* Disable the selected I2C peripheral */
		I2Cx->CR &= ~I2C_CR_MEN;
	}
}

/************************************************************************
 * function   : I2C_DMACmd
 * Description: Enables or disables the specified I2C DMA requests.
 * input      : I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * input      : NewState: new state of the I2C START condition generation.
 *       This parameter can be: ENABLE or DISABLE.
 * retval     : None
 ************************************************************************/
void I2C_DMACmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
#if (USE_FULL_ASSERT == 1)
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
#endif
	if (NewState != DISABLE)
	{
		/* Enable the selected I2C DMA requests */
		I2Cx->CR |= I2C_CR_DMA_EN;
	}
	else
	{
		/* Disable the selected I2C DMA requests */
		I2Cx->CR &= ~I2C_CR_DMA_EN;
	}
}

/************************************************************************
 * function   : I2C_GetFlagStatus
 * Description: Checks whether the specified I2C flag is set or not.
 * input      : I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * input      : I2C_FLAG: specifies the flag to check. 
 *       This parameter can be: I2C_SR_TXE  I2C_SR_MTF ...
 * retval     : None
 ************************************************************************/
FlagStatus I2C_GetFlagStatus(I2C_TypeDef* I2Cx, uint32_t I2C_FLAG)
{
	FlagStatus bitstatus = RESET;
	
#if (USE_FULL_ASSERT == 1)
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
#endif
	
	if(READ_BIT(I2Cx->SR,I2C_FLAG))
	{
		bitstatus =SET;
	}
		
	return bitstatus;	
}

/************************************************************************
 * function   : I2C_ClearFlag
 * Description: Checks whether the specified I2C flag is set or not.
 * input      : I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * input      : I2C_FLAG: specifies the flag to clear. 
 *       This parameter can be: I2C_SR_TXE  I2C_SR_MTF ...
 * retval     : None
 ************************************************************************/
void I2C_ClearFlag(I2C_TypeDef* I2Cx, uint32_t I2C_FLAG)
{
  uint32_t flagpos = 0;
	
#if (USE_FULL_ASSERT == 1)
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
#endif
	
  /* Clear the selected I2C flag  */
	I2Cx->SR = READ_BIT(I2Cx->SR, I2C_FLAG);
}

/************************************************************************
 * function   : I2C_ClearFlag
 * Description: Checks whether the specified I2C flag is set or not.
 * input      : I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * input      :I2C_IT: specifies the I2C interrupts sources to be enabled or disabled. 
 *       This parameter can be:I2C_CR_TXE_INT_EN  I2C_CR_TXE_INT_EN ...
 * input      : NewState: new state of the I2C START condition generation.
 *       This parameter can be: ENABLE or DISABLE.
 * retval     : None
 ************************************************************************/
void I2C_ITConfig(I2C_TypeDef* I2Cx, uint16_t I2C_IT, FunctionalState NewState)
{
#if (USE_FULL_ASSERT == 1)
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
#endif
  
	if (NewState != DISABLE)
	{
	/* Enable the selected I2C interrupts */
	I2Cx->CR |= I2C_IT;
	}
	else
	{
	/* Disable the selected I2C interrupts */
	I2Cx->CR &= (uint16_t)~I2C_IT;
	}
}

 /************************************************************************
 * function   : I2C_GenerateSTART
 * Description: Generates I2Cx communication START condition.
 * input      : I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * input      : NewState: new state of the I2C START condition generation.
 *       This parameter can be: ENABLE or DISABLE.
 * retval     : None.
 ************************************************************************/
void I2C_GenerateSTART(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
#if (USE_FULL_ASSERT == 1)
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
#endif
	
	if (NewState != DISABLE)
	{
	/* Generate a START condition */
	I2Cx->CR |= I2C_CR_START;
	}
	else
	{
	/* Disable the START condition generation */
	I2Cx->CR &= ~I2C_CR_START;
	}
}

 /************************************************************************
 * function   : I2C_GenerateSTOP
 * Description: Generates I2Cx communication STOP condition.
 * input      : I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * input      : NewState: new state of the I2C START condition generation.
 *       This parameter can be: ENABLE or DISABLE.
 * retval     : None.
 ************************************************************************/
void I2C_GenerateSTOP(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
#if (USE_FULL_ASSERT == 1)
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
#endif
	
  if (NewState != DISABLE)
  {
    /* Generate a STOP condition */
    I2Cx->CR |= I2C_CR_STOP;
  }
  else
  {
    /* Disable the STOP condition generation */
    I2Cx->CR &= ~I2C_CR_STOP;
  }
}

 /************************************************************************
 * function   : I2C_Send7bitAddress
 * Description: Transmits the address byte to select the slave device.
 * input      : I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * input      : Address: specifies the slave address which will be transmitted
 * input      : I2C_Direction: specifies whether the I2C device will be a
 *     Transmitter or a Receiver. This parameter can be one of the following values
 *     @arg I2C_Direction_Transmitter: Transmitter mode
 *     @arg I2C_Direction_Receiver: Receiver mode
 * retval     : None.
 ************************************************************************/
void I2C_Send7bitAddress(I2C_TypeDef* I2Cx, uint8_t Address, uint8_t I2C_Direction)
{
#if (USE_FULL_ASSERT == 1)	
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	assert_param(IS_I2C_DIRECTION(I2C_Direction));
#endif
	
	/* Test on the direction to set/reset the read/write bit */
	if (I2C_Direction != I2C_Direction_Transmitter)
	{
		/* Set the address bit0 for read */
		Address |= 0x01;
	}
	else
	{
		/* Reset the address bit0 for write */
		Address &= 0xFE;
	}
	/* Send the address */
	I2Cx->DR = Address;	
}

/*********************************************************************************
* Function    : I2C_StretchClockCmd
* Description : Enables or disables the specified I2C Clock stretching.
* Input       : I2Cx: where x can be 1 or 2 to select the I2C peripheral.
* Input       : NewState: new state of the I2Cx Clock stretching.
*               This parameter can be: ENABLE or DISABLE. 
* Output      : None
**********************************************************************************/
void I2C_StretchClockCmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
#if (USE_FULL_ASSERT == 1)
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
#endif
	
	if (NewState == DISABLE)
	{
		/* Enable the selected I2C Clock stretching */
		I2Cx->CR &= ~I2C_CR_NOSTRETCH;
	}
	else
	{
		/* Disable the selected I2C Clock stretching */
		I2Cx->CR |= I2C_CR_NOSTRETCH;
	}
}

/*********************************************************************************
* Function    : I2C_ReadRegister
* Description :  Reads the specified I2C register and returns its value.
* Input       : I2Cx: where x can be 1 or 2 to select the I2C peripheral.
* Input       : I2C_Register: specifies the register to read.
*               This parameter can be: I2C_Register_ADDR1 I2C_Register_CR ...
* retval      : None
**********************************************************************************/
uint32_t I2C_ReadRegister(I2C_TypeDef* I2Cx, uint8_t I2C_Register)
{
  __IO uint32_t tmp = 0;
	
#if (USE_FULL_ASSERT == 1)
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	assert_param(IS_I2C_REGISTER(I2C_Register));
#endif
	
	tmp = (uint32_t) I2Cx;
	tmp += I2C_Register;

	/* Return the selected register value */
	return (*(__IO uint32_t *) tmp);
}

/*********************************************************************************
* Function    : I2C_SendData
* Description : Sends a data byte through the I2Cx peripheral.
* Input       : I2Cx: where x can be 1 or 2 to select the I2C peripheral.
* Input       : Data: Byte to be transmitted..
* Output      : None
**********************************************************************************/
void I2C_SendData(I2C_TypeDef* I2Cx, uint8_t Data)
{
#if (USE_FULL_ASSERT == 1)
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
#endif
	
	/* Write in the DR register the data to be sent */
	I2Cx->DR = Data;
}

/*********************************************************************************
* Function    : I2C_ReceiveData
* Description : Returns the most recent received data by the I2Cx peripheral.
* Input       : I2Cx: where x can be 1 or 2 to select the I2C peripheral.
* Input       : Data: Byte to be transmitted..
* Output      : None
**********************************************************************************/
uint8_t I2C_ReceiveData(I2C_TypeDef* I2Cx)
{
#if (USE_FULL_ASSERT == 1)
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
#endif
	
	/* Return the data in the DR register */
	return (uint8_t)I2Cx->DR;
}

/*********************************************************************************
* Function    : I2C_WaitOnFlagUntilTimeout
* Description : This function handles I2C Communication Timeout.
* Input       : I2Cx: where x can be 1 or 2 to select the I2C peripheral.
* Input       : Flag: Flag specifies the I2C flag to check.
* Input       : Status: The new Flag status (SET or RESET).
* Input       : Timeout: Timeout duration.
* Output      : None
**********************************************************************************/
static uint8_t I2C_WaitOnFlagUntilTimeout(I2C_TypeDef* I2Cx, uint32_t Flag, FlagStatus Status, uint32_t Timeout)
{
    __IO uint32_t lu32_Timeout;
    /* have no timeout */
    if (Timeout == 0) 
    {
        while(_I2C_GET_FLAG(I2Cx, Flag)==Status);
    }
    else 
    {
        lu32_Timeout = Timeout * 256;
            
        while(_I2C_GET_FLAG(I2Cx, Flag)==Status)
        {
            if (lu32_Timeout-- == 0) 
            {
                return I2C_ERROR;
            }
        }
    }
    return I2C_OK;
}

/************************************************************************
 * function   : I2C_Master_Request_Write
 * Description: I2C Write Access Request
 * input      : I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 *              DevAddress: Device address
 *              Timeout: Timeout value
 ************************************************************************/
static uint8_t I2C_Master_Request_Write(I2C_TypeDef* I2Cx, uint8_t DevAddress, uint32_t Timeout)
{
    /* Generate Start */
	I2C_GenerateSTART(I2Cx,ENABLE);

    /* Clear MTF, To Prevent Errors */
	I2C_ClearFlag(I2Cx,I2C_SR_MTF);

    /* Send Device Address */
	I2C_Send7bitAddress(I2Cx,DevAddress,I2C_Direction_Transmitter);

    /* Wait for transmission End*/
    if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_MTF, RESET, Timeout) != I2C_OK) return I2C_ERROR;
    /* Clear MTF */
	I2C_ClearFlag(I2Cx,I2C_SR_MTF);

    /* Get NACK */
    if (I2C_GetFlagStatus(I2Cx, I2C_SR_RACK))
    {
        /* Generate Stop */
		I2C_GenerateSTOP(I2Cx,ENABLE);
        
        /* Wait for the bus to idle */
        if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_BUS_BUSY, SET, Timeout) != I2C_OK) return I2C_ERROR;
        
        return I2C_ERROR;
    }
    /* Get ACK */
    else 
    {
        return I2C_OK;
    }
}

/************************************************************************
 * function   : I2C_Master_Request_Read
 * Description: I2C Read Access Request
 * input      : I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 *              DevAddress: Device address
 *              Timeout: Timeout value
 ************************************************************************/
static uint8_t I2C_Master_Request_Read(I2C_TypeDef* I2Cx, uint8_t DevAddress, uint32_t Timeout)
{
    /* Generate Start */
    I2C_GenerateSTART(I2Cx,ENABLE);

    /* Clear MTF, To Prevent Errors */
    I2C_ClearFlag(I2Cx,I2C_SR_MTF);
    
    /* Send Device Address */
	I2C_Send7bitAddress(I2Cx,DevAddress,I2C_Direction_Receiver);
    
    /* Wait for transmission End */
    if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_MTF, RESET, Timeout) != I2C_OK) return I2C_ERROR;
    /* Clear MTF */
    I2C_ClearFlag(I2Cx,I2C_SR_MTF);
    
    /* Get NACK */
    if (I2C_GetFlagStatus(I2Cx, I2C_SR_RACK))
    {
        /* Generate Stop */
        I2C_GenerateSTOP(I2Cx,ENABLE);
        
        /* Wait for the bus to idle */
        if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_BUS_BUSY, SET, Timeout) != I2C_OK) return I2C_ERROR;
        
        return I2C_ERROR;
    }
    /* Get ACK */
    else 
    {
        return I2C_OK;
    }
}

/************************************************************************
 * function   : I2C_Check_Device_Ready
 * Description: Check Device Ready
 * input      : I2Cx: where x can be 1 or 2 to select the I2C peripheral. 
 *              DevAddress: Device address
 *              Timeout: Timeout value
 ************************************************************************/
static uint8_t I2C_Check_Device_Ready(I2C_TypeDef* I2Cx, uint8_t DevAddress, uint32_t Timeout)
{
    /* Bus Busy */
    if (I2C_GetFlagStatus(I2Cx, I2C_SR_BUS_BUSY)) 
        return I2C_ERROR;

    /* Generate Start */
    I2C_GenerateSTART(I2Cx,ENABLE);
    
    /* Send Device Address */
    I2Cx->DR = DevAddress;
    
    /* Wait for transmission End */
    if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_MTF, RESET, Timeout) != I2C_OK) return I2C_ERROR;
    /* Clear MTF */
    I2C_ClearFlag(I2Cx,I2C_SR_MTF);
    
    /* Get NACK */
    if (I2C_GetFlagStatus(I2Cx, I2C_SR_RACK))
    {
        /* Generate Stop */
		I2C_GenerateSTOP(I2Cx,ENABLE);
        
        /* Wait for the bus to idle */
        if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_BUS_BUSY, SET, Timeout) != I2C_OK) return I2C_ERROR;
        
        return I2C_ERROR;
    }
    /* Get ACK */
    else 
    {
        /* Generate Stop */
        I2C_GenerateSTOP(I2Cx,ENABLE);
        
        /* Wait for the bus to idle */
        if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_BUS_BUSY, SET, Timeout) != I2C_OK) return I2C_ERROR;
        
        return I2C_OK;
    }
}
/************************************************************************
 * function   : I2C_Mem_Write
 * Description: Write an amount of data in blocking mode to a specific memory address
 * input      : I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 *              DevAddress : Target device address
 *              MemAddress : MemAddress Internal memory address
 *              MemAddSize : MemAddSize Size of internal memory address
 *              pData      : Pointer to data buffer
 *              Size       : Amount of data to be sent
 *              Timeout    : Timeout value
 ************************************************************************/
uint8_t I2C_Mem_Write(I2C_TypeDef* I2Cx,uint8_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint32_t Size, uint32_t Timeout)
{
    uint32_t i;
    
#if (USE_FULL_ASSERT == 1)
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
#endif
   
    /* Bus Busy */
    if (I2C_GetFlagStatus(I2Cx, I2C_SR_BUS_BUSY)) 
        return I2C_BUSY;
    
    /* Send Write Access Request */
    if (I2C_Master_Request_Write(I2Cx, DevAddress,0) == I2C_OK)
    {
        /* If Memory address size is 8Bit */
        if (MemAddSize == I2C_MEMADD_SIZE_8BIT)
        {
            /* Send Memory Address */
			I2C_SendData(I2Cx,I2C_MEM_ADD_LSB(MemAddress));
        }
        /* If Memory address size is 16Bit */ 
        else
        {
            /* Send Memory Address MSB*/
            I2C_SendData(I2Cx,I2C_MEM_ADD_MSB(MemAddress));
            /* Wait for transmission End*/
            if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_MTF, RESET, Timeout) != I2C_OK)  return I2C_ERROR;
            /* Clear MTF */
            I2C_ClearFlag(I2Cx,I2C_SR_MTF);
            /* Send Memory Address LSB*/
			I2C_SendData(I2Cx,I2C_MEM_ADD_LSB(MemAddress));
        }
        
        /* Wait for transmission End*/
        if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_MTF, RESET, Timeout) != I2C_OK)  return I2C_ERROR;
        /* Clear MTF */
        I2C_ClearFlag(I2Cx,I2C_SR_MTF);
        
        /* Get NACK */
        if (I2C_GetFlagStatus(I2Cx, I2C_SR_RACK))
        {
            /* Generate Stop */
			I2C_GenerateSTOP(I2Cx,ENABLE);
            
            /* Wait for the bus to idle */
            if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_BUS_BUSY, SET, Timeout) != I2C_OK)  return I2C_ERROR;
            
            return I2C_ERROR;
        }
        /* Get ACK */
        else 
        {
            for (i = 0; i < Size; i++)
            {
                /* Wait TXE Flag */
                if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_TXE, RESET, Timeout)!= I2C_OK)  return I2C_ERROR;
                
                /* Send Data */
				I2C_SendData(I2Cx,pData[i]);
                
                /* Wait for transmission End*/
                 if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_MTF, RESET, Timeout) != I2C_OK)  return I2C_ERROR;
                /* Clear MTF */
                I2C_ClearFlag(I2Cx,I2C_SR_MTF);
                
                /* Get NACK */
                if (I2C_GetFlagStatus(I2Cx, I2C_SR_RACK))
                {
                    /* Generate Stop */
                    I2C_GenerateSTOP(I2Cx,ENABLE);
                    
                    /* Wait for the bus to idle */
                    if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_BUS_BUSY, SET, Timeout) != I2C_OK)  return I2C_ERROR;
                    return I2C_ERROR;
                }
            }
            
            /* Generate Stop */
            I2C_GenerateSTOP(I2Cx,ENABLE);
            
            /* Wait for the bus to idle */
            if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_BUS_BUSY, SET, Timeout) != I2C_OK)  return I2C_ERROR;
            /* Check Device Ready */
            while(I2C_Check_Device_Ready(I2Cx, DevAddress, Timeout) != I2C_OK);
        }
    }
    else
    {
        return I2C_ERROR;
    }

    return I2C_OK;
}

/************************************************************************
 * function   : I2C_Mem_Read
 * Description: Read an amount of data in blocking mode to a specific memory address
 * input      : I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 *              DevAddress : Target device address
 *              MemAddress : MemAddress Internal memory address
 *              MemAddSize : MemAddSize Size of internal memory address
 *              pData      : Pointer to data buffer
 *              Size       : Amount of data to be sent
 *              Timeout    : Timeout value
 ************************************************************************/
uint8_t I2C_Mem_Read(I2C_TypeDef* I2Cx, uint8_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    uint32_t i;
    
#if (USE_FULL_ASSERT == 1)
	 /* Check the parameters */
	 assert_param(IS_I2C_ALL_PERIPH(I2Cx));
#endif
   
    /* Bus Busy */
    if (READ_BIT(I2Cx->SR, I2C_SR_BUS_BUSY)) 
        return I2C_ERROR;
    
    /* Send Write Access Request */
    if (I2C_Master_Request_Write(I2Cx, DevAddress,0) == I2C_OK)
    {
        /* If Memory address size is 8Bit */
        if (MemAddSize == I2C_MEMADD_SIZE_8BIT)
        {
            /* Send Memory Address */
            I2Cx->DR = I2C_MEM_ADD_LSB(MemAddress);
        }
        /* If Memory address size is 16Bit */ 
        else
        {
            /* Send Memory Address MSB*/
            I2Cx->DR = I2C_MEM_ADD_MSB(MemAddress);
            /* Wait for transmission End*/
            if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_MTF, RESET, Timeout) != I2C_OK)  return I2C_ERROR;
            /* Clear MTF */
            I2Cx->SR = READ_BIT(I2Cx->SR, I2C_SR_MTF);
            /* Send Memory Address LSB*/
            I2Cx->DR = I2C_MEM_ADD_LSB(MemAddress);
        }
        
        /* Wait for transmission End*/
        if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_MTF, RESET, Timeout) != I2C_OK)  return I2C_ERROR;
        /* Clear MTF */
        I2Cx->SR = READ_BIT(I2Cx->SR, I2C_SR_MTF);
        
        /* Get NACK */
        if (READ_BIT(I2Cx->SR, I2C_SR_RACK))
        {
            /* Generate Stop */
            SET_BIT(I2Cx->CR, I2C_CR_STOP);
            
            /* Wait for the bus to idle */
            if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_BUS_BUSY, SET, Timeout) != I2C_OK)  return I2C_ERROR;
            
            return I2C_ERROR;
        }
        /* Get ACK */
        else
        {
            /* Send Write Read Request */
            if (I2C_Master_Request_Read(I2Cx, DevAddress, Timeout) == I2C_OK)
            {
                /* Wait Master Transition receiving state */
                if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_TX_RX_FLAG, RESET, Timeout) != I2C_OK)  return I2C_ERROR;
                /* Clear TX_RX_FLAG */
                I2Cx->SR = READ_BIT(I2Cx->SR, I2C_SR_TX_RX_FLAG);

                /* Generate ACK */
                CLEAR_BIT(I2Cx->CR, I2C_CR_TACK);

                for (i = 0; i <Size - 1; i++)
                {
                    /* Wait RXNE Flag */
                    if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_RXNE, RESET, Timeout) != I2C_OK)  return I2C_ERROR;
                    /* Read Data */
                    pData[i] = I2Cx->DR;
                    /* Wait for transmission End*/
                    if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_MTF, RESET, Timeout) != I2C_OK)  return I2C_ERROR;
                    /* Clear MTF */
                    I2Cx->SR = READ_BIT(I2Cx->SR, I2C_SR_MTF);
                }

                /* Prepare for Generate NACK */
                SET_BIT(I2Cx->CR, I2C_CR_TACK);
                /* Prepare for Generate STOP */
                SET_BIT(I2Cx->CR, I2C_CR_STOP);

                /* Wait RXNE Flag */
                if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_RXNE, RESET, Timeout) != I2C_OK)  return I2C_ERROR;
                /* Read Data */
                pData[i] = I2Cx->DR;
                /* Wait for transmission End*/
                if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_MTF, RESET, Timeout) != I2C_OK)  return I2C_ERROR;
                /* Clear MTF */
                I2Cx->SR = READ_BIT(I2Cx->SR, I2C_SR_MTF);

                /* Wait for the bus to idle */
                if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_BUS_BUSY, SET, Timeout) != I2C_OK)  return I2C_ERROR;
                /* Generate ACK */
                CLEAR_BIT(I2Cx->CR, I2C_CR_TACK);
            }
            else 
            {
				/* Get NACK */
				return I2C_ERROR;
            }
        }
    }
    else
    {
		return I2C_OK;
    }

    return I2C_OK;
}


/************************************************************************
 * function   : I2C_Master_Transmit
 * Description: Transmits in master mode an amount of data in blocking mode.
 * input      : I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 *              DevAddress : Target device address
 *              pData      : Pointer to data buffer
 *              Size       : Amount of data to be sent
 *              Timeout    : Timeout value
************************************************************************/
uint8_t I2C_Master_Transmit(I2C_TypeDef* I2Cx, uint16_t DevAddress, uint8_t *pData, uint32_t Size, uint32_t Timeout)
{
    uint32_t i;
    
#if (USE_FULL_ASSERT == 1)
	  /* Check the parameters */
	  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
#endif
       
    /* Send Write Access Request */
    if (I2C_Master_Request_Write(I2Cx, DevAddress, Timeout) == I2C_OK)
    {
        for (i = 0; i < Size; i++)
        {
            /* Wait TXE Flag */
            if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_TXE, RESET, Timeout) != I2C_OK) return I2C_ERROR;
            
            /* Send Data */
			I2C_SendData(I2Cx,pData[i]);
            
            /* Wait for transmission End*/
            if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_MTF, RESET, Timeout) != I2C_OK) return I2C_ERROR;
            
            /* Clear MTF */
            I2C_ClearFlag(I2Cx,I2C_SR_MTF);
            /* Get NACK */
            if (I2C_GetFlagStatus(I2Cx, I2C_SR_RACK))
            {
                /* Generate Stop */
				I2C_GenerateSTOP(I2Cx,ENABLE);
                
                /* Wait for the bus to idle */
                if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_BUS_BUSY, SET, Timeout) != I2C_OK) return I2C_ERROR;
                
                return I2C_ERROR;
            }
        }
        
        /* Generate Stop */
        I2C_GenerateSTOP(I2Cx,ENABLE);
        
        /* Wait for the bus to idle */
         if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_BUS_BUSY, SET, Timeout) != I2C_OK) return I2C_ERROR;
    }
    else
    {
        return I2C_ERROR;
    }

    return I2C_OK;
}

/************************************************************************
 * function   : I2C_Master_Receive
 * Description: Transmits in master mode an amount of data in blocking mode.
 * input      : I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 *              DevAddress : Target device address
 *              pData      : Pointer to data buffer
 *              Size       : Amount of data to be Receive
 *              Timeout    : Timeout value
 ************************************************************************/
uint8_t I2C_Master_Receive(I2C_TypeDef* I2Cx, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    uint32_t i;

#if (USE_FULL_ASSERT == 1)
	  /* Check the parameters */
	  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
#endif
       
    /* Send Read Access Request */
    if (I2C_Master_Request_Read(I2Cx, DevAddress, Timeout) == I2C_OK)
    {
        /* Wait Master Transition receiving state */
        if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_TX_RX_FLAG, RESET, Timeout) != I2C_OK) return I2C_ERROR;
        /* Clear TX_RX_FLAG */
		I2C_ClearFlag(I2Cx,I2C_SR_TX_RX_FLAG);
        /* Generate ACK */
        CLEAR_BIT(I2Cx->CR, I2C_CR_TACK);

        for (i = 0; i < Size - 1; i++)
        {
            /* Wait RXNE Flag */
            if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_RXNE, RESET, Timeout) != I2C_OK) return I2C_ERROR;
            /* Read Data */
            pData[i] = I2C_ReceiveData(I2Cx);
            /* Wait for transmission End*/
            if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_MTF, RESET, Timeout) != I2C_OK) return I2C_ERROR;
            /* Clear MTF */
            I2Cx->SR = READ_BIT(I2Cx->SR, I2C_SR_MTF);
        }

        /* Prepare for Generate NACK */
        SET_BIT(I2Cx->CR, I2C_CR_TACK);
        /* Prepare for Generate STOP */
        SET_BIT(I2Cx->CR, I2C_CR_STOP);

        /* Wait RXNE Flag */
        if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_RXNE, RESET, Timeout) != I2C_OK) return I2C_ERROR;
        /* Read Data */
        pData[i] = I2C_ReceiveData(I2Cx);
        /* Wait for transmission End*/
        if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_MTF, RESET, Timeout) != I2C_OK) return I2C_ERROR;
        /* Clear MTF */
        I2C_ClearFlag(I2Cx,I2C_SR_MTF);
        
        /* Wait for the bus to idle */
        if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_BUS_BUSY, SET, Timeout) != I2C_OK) return I2C_ERROR;

        /* Generate ACK */
        CLEAR_BIT(I2Cx->CR, I2C_CR_TACK);
    }
    else
    {
        return I2C_ERROR;
    }

    return I2C_OK;
}

/************************************************************************
 * function   : I2C_Slave_Transmit
 * Description: Transmits in Slave mode an amount of data in blocking mode.
 * input      : I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 *              pData      : Pointer to data buffer
 *              Size       : Amount of data to be sent
 *              Timeout    : Timeout value
 ************************************************************************/
uint8_t I2C_Slave_Transmit(I2C_TypeDef* I2Cx, uint8_t *pData, uint32_t Size, uint32_t Timeout)
{
    uint32_t i = 0;
    
#if (USE_FULL_ASSERT == 1)
	  /* Check the parameters */
	  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
#endif
    
    /* Clear RX_ADDR1 Flag */ 
    I2C_ClearFlag(I2Cx,I2C_SR_RX_ADDR1);	  
    /* Match the Address 1 */
    if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_RX_ADDR1, RESET, Timeout) != I2C_OK) return I2C_ERROR;
    /* Clear RX_ADDR1 Flag */
	I2C_ClearFlag(I2Cx,I2C_SR_RX_ADDR1);

    /* Slave Transmit */
    if (I2C_GetFlagStatus(I2Cx, I2C_SR_SRW))
    {
        /* BUS BUSY */
        while(I2C_GetFlagStatus(I2Cx, I2C_SR_BUS_BUSY))
        {
            if (I2C_GetFlagStatus(I2Cx, I2C_SR_MTF)) 
            {
                /* Clear MTF */
				I2C_ClearFlag(I2Cx,I2C_SR_MTF);				
            }

            if (I2C_GetFlagStatus(I2Cx, I2C_SR_TXE))
            {
                if (i < Size || Size == 0) 
                {
					I2C_SendData(I2Cx,pData[i]);
					i++;
                }
            }
        }
		//Clear Flags
        I2Cx->SR = READ_REG(I2Cx->SR);
    }
    else
    {
        return I2C_ERROR;
    }
   
    return I2C_OK;
}

/************************************************************************
 * function   : I2C_Slave_Receive
 * Description: Receive in Slave mode an amount of data in blocking mode.
 * input      : I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 *              pData : Pointer to data buffer
 *              Size  : Amount of data to be sent
 *              Timeout    : Timeout value
 * retval     : I2C_OK or I2C_ERROR
 ************************************************************************/
uint8_t I2C_Slave_Receive(I2C_TypeDef* I2Cx, uint8_t *pData, uint32_t Size, uint32_t Timeout)
{
	uint32_t Rx_Count =0;
    
#if (USE_FULL_ASSERT == 1)
	  /* Check the parameters */
	  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
#endif
    
 
    /* Match the Address 1 */
    if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_RX_ADDR1, RESET, Timeout) != I2C_OK) return I2C_ERROR;
    /* Clear RX_ADDR1 Flag */
	I2C_ClearFlag(I2Cx,I2C_SR_RX_ADDR1);
    
    /* Slave Receive */
    if (!I2C_GetFlagStatus(I2Cx, I2C_SR_SRW))
    {
        /* Wait for transmission End*/
        if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_MTF, RESET, Timeout) != I2C_OK)  return I2C_ERROR;
        /* Clear MTF */
		I2C_ClearFlag(I2Cx,I2C_SR_MTF);

        /* BUS BUSY */
        while(I2C_GetFlagStatus(I2Cx, I2C_SR_BUS_BUSY))
        {
            /* Receive Data */
            if (I2C_GetFlagStatus(I2Cx, I2C_SR_RXNE))
            {
                pData[Rx_Count++] = I2C_ReceiveData(I2Cx);
              
                /* Wait for transmission End*/
                if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_SR_MTF, RESET, Timeout) != I2C_OK)  return I2C_ERROR;
                /* Clear MTF */
                I2C_ClearFlag(I2Cx,I2C_SR_MTF);

                if (Size != 0) 
                {
                    if (Rx_Count >= Size) 
                    {
                        break;
                    }
                }
            }
        }       
        /* Clear ACK */
        CLEAR_BIT(I2Cx->CR, I2C_CR_TACK);
        
        I2Cx->SR = READ_REG(I2Cx->SR);
    }
    /* Slave Transmit */
    else 
    {
       return I2C_ERROR;
    }

    return I2C_OK;
}

/************************************************************************
 * function   : I2C_Slave_Transmit_IT
 * Description: Transmit in slave mode an amount of data in non-blocking mode with Interrupt
 * input      : hi2c : pointer to a I2C_HandleTypeDef structure that contains
 *                     the configuration information for I2C module
 *              pData      : Pointer to data buffer
 *              Size       : Amount of data to be sent
 * return     : HAL_StatusTypeDef
 ************************************************************************/
uint8_t I2C_Slave_Transmit_IT(I2C_TypeDef* I2Cx, Slave_Comm_TypeDef* Slave_Comm, uint8_t *pData, uint32_t Size)
{
	
#if (USE_FULL_ASSERT == 1)
	  /* Check the parameters */
	  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
#endif

    /* Slave machine is running */
    if (Slave_Comm->Slave_TxState != SLAVE_TX_STATE_IDLE)
        return I2C_ERROR;

    /* Set Slave machine is sending */
    Slave_Comm->Slave_TxState = SLAVE_TX_STATE_SENDING;

    Slave_Comm->Tx_Buffer = pData;
    Slave_Comm->Tx_Size   = Size;
    Slave_Comm->Tx_Count = 0;

    CLEAR_BIT(I2Cx->CR, I2C_CR_TXE_SEL);

    I2Cx->DR = Slave_Comm->Tx_Buffer[0];

    Slave_Comm->Tx_Count++;

    /* Clear RX ADDR1 Flag */
	I2C_ClearFlag(I2Cx,I2C_SR_RX_ADDR1);
    /* RX ADDR1 Interrupt Enable */
	I2C_ITConfig(I2Cx,I2C_CR_RX_ADDR1_INT_EN,ENABLE);
    
    return I2C_OK;
}

/************************************************************************
 * function   : I2C_Slave_Receive_IT
 * Description: Receive in slave mode an amount of data in non-blocking mode with Interrupt
 * input      : hi2c : pointer to a I2C_HandleTypeDef structure that contains
 *                     the configuration information for I2C module
 *              pData      : Pointer to data buffer
 *              Size       : Amount of data to be sent
 ************************************************************************/
uint8_t I2C_Slave_Receive_IT(I2C_TypeDef* I2Cx, Slave_Comm_TypeDef* Slave_Comm,uint8_t *pData, uint32_t Size)
{
	
#if (USE_FULL_ASSERT == 1)
	  /* Check the parameters */
	  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
#endif

    /* Rx machine is running */
    if (Slave_Comm->Slave_RxState != SLAVE_RX_STATE_IDLE)
        return I2C_ERROR;

    /* Set Slave machine is receiving */
    Slave_Comm->Slave_RxState = SLAVE_RX_STATE_RECEIVING;
    
    Slave_Comm->Rx_Buffer = pData;
    Slave_Comm->Rx_Size   = Size;
    Slave_Comm->Rx_Count = 0;
    
    /* Clear RX ADDR1 Flag */
	I2C_ClearFlag(I2Cx,I2C_SR_RX_ADDR1);
    /* RX ADDR1 Interrupt Enable */
	I2C_ITConfig(I2Cx,I2C_CR_RX_ADDR1_INT_EN,ENABLE);
    
    return I2C_OK;
}
