/*
  ******************************************************************************
  * @file    GPIO.c
  * @version V1.0.0
  * @date    2020
  * @brief   GPIO module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the General Purpose Input/Output (GPIO) peripheral:
  *           @ Initialization  functions
  *           @ IO operation functions
  ******************************************************************************
*/

#include "fxx_std.h"

/*********************************************************************************
* Function    : GPIO_Init
* Description : Initializes the GPIOx peripheral according to the specified
*                parameters in the GPIO_InitStruct
* Input       : fe_GPIO:This parameter can be GPIOx where x can be (A..F).
*               GPIO_InitStruct:pointer to a GPIO_InitTypeDef structure that contains 
*                               the configuration information for the specified GPIO peripheral.
* Output      : None
**********************************************************************************/
void GPIO_Init(enum_GPIOx_t fe_GPIO, GPIO_InitTypeDef* GPIO_InitStruct)
{
    /* Check the parameters */
    assert_param (IS_GPIO_ALL_INSTANCE(fe_GPIO));
    assert_param (IS_GPIO_PIN(GPIO_InitStruct->Pin));
    assert_param (IS_GPIO_MODE(GPIO_InitStruct->Mode));  

    
    uint32_t lu32_Position = 0;
    uint32_t lu32_Current_Pin;
    uint32_t lu32_Position_Mask;
    
    volatile uint32_t *lu32_SEL1 = NULL;        // 指向 -> 管脚复用寄存器1
    volatile uint32_t *lu32_SEL2 = NULL;        // 指向 -> 管脚复用寄存器2
    volatile uint32_t *lu32_PollUP   = NULL;    // 指向 -> 上拉选择寄存器
    volatile uint32_t *lu32_PollDown = NULL;    // 指向 -> 下拉选择寄存器
    volatile uint32_t *lu32_ODEnable = NULL;    // 指向 -> 开漏使能寄存器
    volatile uint32_t *lu32_ADS      = NULL;    // 指向 -> 数字、模拟选择寄存器
    
    GPIO_TypeDef *GPIOx;
    
    switch (fe_GPIO)
    {
        case GPIOA: 
        case GPIOB: 
        {
            GPIOx = GPIOAB;
            
            lu32_PollUP   = &(SCU->PABPUR);
            lu32_PollDown = &(SCU->PABPDR);
            lu32_ODEnable = &(SCU->PABODR);
            lu32_ADS      = &(SCU->PABADS);
            
            if (fe_GPIO == GPIOB) 
            {
                GPIO_InitStruct->Pin <<= 16;
                
                lu32_SEL1 = &(SCU->PBSEL1);
                lu32_SEL2 = &(SCU->PBSEL2);
            }
            else 
            {
                lu32_SEL1 = &(SCU->PASEL1);
                lu32_SEL2 = &(SCU->PASEL2);
            }
        }break;

        case GPIOC: 
        case GPIOD: 
        {
            GPIOx = GPIOCD;
            
            lu32_PollUP   = &(SCU->PCDPUR);
            lu32_PollDown = &(SCU->PCDPDR);
            lu32_ODEnable = &(SCU->PCDODR);
            lu32_ADS      = &(SCU->PCDADS);
            
            if (fe_GPIO == GPIOD) 
            {
                GPIO_InitStruct->Pin <<= 16;
                
                lu32_SEL1 = &(SCU->PDSEL1);
                lu32_SEL2 = &(SCU->PDSEL2);
            }
            else 
            {
                lu32_SEL1 = &(SCU->PCSEL1);
                lu32_SEL2 = &(SCU->PCSEL2);
            }
        }break;

        case GPIOE: 
        case GPIOF: 
        {
            GPIOx = GPIOEF;
            
            lu32_PollUP   = &(SCU->PEFPUR);
            lu32_PollDown = &(SCU->PEFPDR);
            lu32_ODEnable = &(SCU->PEFODR);
            lu32_ADS      = &(SCU->PEFADS);
            
            if (fe_GPIO == GPIOF) 
            {
                GPIO_InitStruct->Pin <<= 16;
                
                lu32_SEL1 = &(SCU->PFSEL1);
            }
            else 
            {
                lu32_SEL1 = &(SCU->PESEL1);
                lu32_SEL2 = &(SCU->PESEL2);
            }
        }break;

        default: break; 
    }

    /* Configure Select pins */
    while ((GPIO_InitStruct->Pin) >> lu32_Position != 0) 
    {
        /* Get current pin position */
        lu32_Current_Pin = (GPIO_InitStruct->Pin) & (1uL << lu32_Position);
        
        if (lu32_Current_Pin) 
        {
            switch (GPIO_InitStruct->Mode)
            {
                /* GPIO IN Function */
                case GPIO_MODE_INPUT:
                {
                    GPIOx->DIR &= ~lu32_Current_Pin;
                }break;
                
                /* GPIO OUT Function */
                case GPIO_MODE_OUTPUT_PP: 
                case GPIO_MODE_OUTPUT_OD: 
                {
                    GPIOx->DIR |= lu32_Current_Pin;
                }break;

                /* Alternate Function */
                case GPIO_MODE_AF_PP:
                case GPIO_MODE_AF_OD:
                {
                    /* Get Position Mask */
                    if (lu32_Position < 16) 
                    {   /* GOIOA、GPIOC、GPIOE */
                        lu32_Position_Mask = lu32_Position;
                    }
                    else 
                    {   /* GPIOB、GPIOD、GPIOF */
                        lu32_Position_Mask = lu32_Position - 16;    
                    }
                    
                    /* SET GPIO Function */
                    if (lu32_Position_Mask < 8) 
                    {
                        *lu32_SEL1 = (*lu32_SEL1 & ~(0xF << (lu32_Position_Mask * 4))) | (GPIO_InitStruct->Alternate << (lu32_Position_Mask * 4));
                    }
                    else 
                    {
                        *lu32_SEL2 = (*lu32_SEL2 & ~(0xF << ((lu32_Position_Mask - 8) * 4))) | (GPIO_InitStruct->Alternate << ((lu32_Position_Mask - 8) * 4));
                    }
                }break;

                /* GPIO INT Function */
                case GPIO_MODE_IT_RISING:
                case GPIO_MODE_IT_FALLING:
                case GPIO_MODE_IT_RISING_FALLING:
                case GPIO_MODE_IT_HIGH_LEVEL:
                case GPIO_MODE_IT_LOW_LEVEL:
                {
                    /* Set direction Input、Enable INT */
                    GPIOx->DIR &= ~lu32_Current_Pin;
                    GPIOx->IEN |=  lu32_Current_Pin;

                    /* Single edge */
                    if (GPIO_InitStruct->Mode == GPIO_MODE_IT_RISING || GPIO_InitStruct->Mode == GPIO_MODE_IT_FALLING) 
                    {
                        /* edge trigger */
                        GPIOx->IS  &= ~lu32_Current_Pin;
                        /* Single trigger */
                        GPIOx->IBE &= ~lu32_Current_Pin;
                        
                        if (GPIO_InitStruct->Mode == GPIO_MODE_IT_RISING) 
                        {
                            GPIOx->IEV |= lu32_Current_Pin;
                        }
                        else 
                        {
                            GPIOx->IEV &= ~lu32_Current_Pin;
                        }
                    }

                    /* Double edge */
                    if (GPIO_InitStruct->Mode == GPIO_MODE_IT_RISING_FALLING) 
                    {
                        /* edge trigger */
                        GPIOx->IS  &= ~lu32_Current_Pin;
                        /* Double trigger */
                        GPIOx->IBE |=  lu32_Current_Pin;
                    }
                    
                    /* LEVEL trigger */
                    if (GPIO_InitStruct->Mode == GPIO_MODE_IT_HIGH_LEVEL || GPIO_InitStruct->Mode == GPIO_MODE_IT_LOW_LEVEL) 
                    {
                        /* LEVEL trigger */
                        GPIOx->IS |= lu32_Current_Pin;

                        if (GPIO_InitStruct->Mode == GPIO_MODE_IT_HIGH_LEVEL) 
                        {
                            GPIOx->IEV |= lu32_Current_Pin;
                        }
                        else 
                        {
                            GPIOx->IEV &= ~lu32_Current_Pin;
                        }
                    }
                }break;

                default: break; 
            }

            /* Set Pull UP or DOWN or NO */
            if (GPIO_InitStruct->Pull == GPIO_NOPULL) 
            {
                *lu32_PollUP   &= ~lu32_Current_Pin;
                *lu32_PollDown &= ~lu32_Current_Pin;
            }
            else if (GPIO_InitStruct->Pull == GPIO_PULLUP) 
            {
                *lu32_PollUP   |=  lu32_Current_Pin;
                *lu32_PollDown &= ~lu32_Current_Pin;
            }
            else if (GPIO_InitStruct->Pull == GPIO_PULLDOWN) 
            {
                *lu32_PollUP   &= ~lu32_Current_Pin;
                *lu32_PollDown |=  lu32_Current_Pin;
            }

            /* Set Open Drain Mode */
            if (GPIO_InitStruct->Mode & GPIO_MODE_OD_MASK) 
            {
                *lu32_ODEnable |= lu32_Current_Pin;
            }
            else 
            {
                *lu32_ODEnable &= ~lu32_Current_Pin;
            }
            
            /* GPIO Function */
            if (GPIO_InitStruct->Mode & GPIO_MODE_IO_MASK) 
            {
                /* Get Position Mask */
                if (lu32_Position < 16) 
                {   /* GOIOA、GPIOC、GPIOE */
                    lu32_Position_Mask = lu32_Position;
                }
                else 
                {   /* GPIOB、GPIOD、GPIOF */
                    lu32_Position_Mask = lu32_Position - 16;
                }
                
                /* SET GPIO Function */
                if (lu32_Position_Mask < 8) 
                {
                    *lu32_SEL1 = (*lu32_SEL1 & ~(0xF << (lu32_Position_Mask * 4))) | (GPIO_FUNCTION_0 << (lu32_Position_Mask * 4));
                }
                else 
                {
                    *lu32_SEL2 = (*lu32_SEL2 & ~(0xF << ((lu32_Position_Mask - 8) * 4))) | (GPIO_FUNCTION_0 << ((lu32_Position_Mask - 8) * 4));
                }
            }

            /* SET Digital or Analog */
            if (GPIO_InitStruct->Mode == GPIO_MODE_ANALOG) 
            {
                *lu32_ADS |= lu32_Current_Pin;
            }
            else 
            {
                *lu32_ADS &= ~lu32_Current_Pin;
            }
        }

        lu32_Position++;
    }
}

/*********************************************************************************
* Function    : GPIO_DeInit
* Description : Deinitializes the GPIOx peripheral registers to their default reset values.
* Input       : fe_GPIO:This parameter can be GPIOx where x can be (A..F).
* Output      : None
**********************************************************************************/
void GPIO_DeInit(enum_GPIOx_t fe_GPIO)
{
    /* Check the parameters */
    assert_param (IS_GPIO_ALL_INSTANCE(fe_GPIO));
    
    GPIO_TypeDef *GPIOx;
    
    switch (fe_GPIO)
    {
        case GPIOA: 
        case GPIOB: 
        {
            System_Module_Disable(EN_GPIOAB);
        }break;

        case GPIOC: 
        case GPIOD: 
        {
            System_Module_Disable(EN_GPIOCD);
        }break;

        case GPIOE: 
        case GPIOF: 
        {
            System_Module_Disable(EN_GPIOEF);
            
        }break;
        default: break; 
    } 
}

/*********************************************************************************
* Function    : GPIO_StructInit
* Description : Fills each GPIO_InitStruct member with its default value.
* Input       : GPIO_InitStruct: pointer to a GPIO_InitTypeDef structure which will
*                                be initialized.
* Output      : None
**********************************************************************************/
void GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct)
{
    /* Reset GPIO init structure parameters values */
    GPIO_InitStruct->Pin  = GPIO_Pin_All;
    GPIO_InitStruct->Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct->Pull = GPIO_NOPULL; 
    GPIO_InitStruct->Alternate = GPIO_FUNCTION_0;   
}

/*********************************************************************************
* Function    : GPIO_ReadInputDataBit
* Description : Reads the specified input port pin.
* Input       : fe_GPIO:This parameter can be GPIOx where x can be (A..F).
*               fu32_GPIO_Pin:specifies the port bit to read.
*                             This parameter can be GPIO_PIN_x where x can be (0..15).
* Output      : The input port pin value.
**********************************************************************************/
uint8_t GPIO_ReadInputDataBit(enum_GPIOx_t fe_GPIO, uint32_t fu32_GPIO_Pin)
{
    /* Check the parameters */
    assert_param (IS_GPIO_ALL_INSTANCE(fe_GPIO));
    assert_param (IS_GPIO_PIN(fu32_GPIO_Pin));
    
    GPIO_TypeDef *GPIOx;

    switch (fe_GPIO)
    {
        case GPIOA: 
        case GPIOB: 
        {
            GPIOx = GPIOAB;
        }break;

        case GPIOC: 
        case GPIOD: 
        {
            GPIOx = GPIOCD;
        }break;

        case GPIOE: 
        case GPIOF: 
        {
            GPIOx = GPIOEF;
        }break;

        default: break;
    }
    
    if (fe_GPIO == GPIOB || fe_GPIO == GPIOD || fe_GPIO == GPIOF)
    {
        fu32_GPIO_Pin <<= 16;
    }
    
    if (GPIOx->IDATA & fu32_GPIO_Pin) 
    {
        return Bit_SET;
    }
    else 
    {
        return Bit_RESET;
    }
}

/*********************************************************************************
* Function    : GPIO_ReadInputData
* Description : Reads the specified GPIO input data port.
* Input       : fe_GPIO:This parameter can be GPIOx where x can be (A..F).
* Output      : GPIO input data port value.
**********************************************************************************/
uint16_t GPIO_ReadInputData(enum_GPIOx_t fe_GPIO)
{
    /* Check the parameters */
    assert_param (IS_GPIO_ALL_INSTANCE(fe_GPIO));
    
    GPIO_TypeDef *GPIOx;
    uint32_t GPIO_MASK = 0;
    uint16_t InputData= 0;
    
    switch (fe_GPIO)
    {
        case GPIOA: 
        case GPIOB: 
        {
            GPIOx = GPIOAB;
        }break;

        case GPIOC: 
        case GPIOD: 
        {
            GPIOx = GPIOCD;
        }break;

        case GPIOE: 
        case GPIOF: 
        {
            GPIOx = GPIOEF;
        }break;

        default: break;
    }
    
    if (fe_GPIO == GPIOB || fe_GPIO == GPIOD || fe_GPIO == GPIOF)
    {
        GPIO_MASK = 0xFFFF0000;
        InputData=(GPIOx->IDATA & GPIO_MASK)>>16;
    }
    else
    {
        GPIO_MASK = 0x0000FFFF;
        InputData=(GPIOx->IDATA & GPIO_MASK);
    }
    
    return InputData;
}

/*********************************************************************************
* Function    : GPIO_ReadOutputDataBit
* Description : Reads the specified output data port bit.
* Input       : fe_GPIO:This parameter can be GPIOx where x can be (A..F).
*               fu32_GPIO_Pin:specifies the port bit to read.
*                             This parameter can be GPIO_PIN_x where x can be (0..15).
* Output      : The output port pin value.
**********************************************************************************/
uint8_t GPIO_ReadOutputDataBit(enum_GPIOx_t fe_GPIO, uint32_t fu32_GPIO_Pin)
{
    /* Check the parameters */
    assert_param (IS_GPIO_ALL_INSTANCE(fe_GPIO));
    assert_param (IS_GPIO_PIN(fu32_GPIO_Pin));
    
    GPIO_TypeDef *GPIOx;
    switch (fe_GPIO)
    {
        case GPIOA: 
        case GPIOB: 
        {
            GPIOx = GPIOAB;
        }break;

        case GPIOC: 
        case GPIOD: 
        {
            GPIOx = GPIOCD;
        }break;

        case GPIOE: 
        case GPIOF: 
        {
            GPIOx = GPIOEF;
        }break;

        default: break;
    }
    
    if (fe_GPIO == GPIOB || fe_GPIO == GPIOD || fe_GPIO == GPIOF)
    {
        fu32_GPIO_Pin <<= 16;
    }
    
    if (GPIOx->ODATA & fu32_GPIO_Pin) 
    {
        return Bit_SET;
    }
    else 
    {
        return Bit_RESET;
    }
}

/*********************************************************************************
* Function    : GPIO_ReadOutputData
* Description : Reads the specified GPIO output data port.
* Input       : fe_GPIO:This parameter can be GPIOx where x can be (A..F).
* Output      : GPIO output data port value.
**********************************************************************************/
uint16_t GPIO_ReadOutputData(enum_GPIOx_t fe_GPIO)
{
    /* Check the parameters */
    assert_param (IS_GPIO_ALL_INSTANCE(fe_GPIO));

    
    GPIO_TypeDef *GPIOx;
    uint32_t GPIO_MASK = 0;
    uint16_t OutputData= 0;
    switch (fe_GPIO)
    {
        case GPIOA: 
        case GPIOB: 
        {
            GPIOx = GPIOAB;
        }break;

        case GPIOC: 
        case GPIOD: 
        {
            GPIOx = GPIOCD;
        }break;

        case GPIOE: 
        case GPIOF: 
        {
            GPIOx = GPIOEF;
        }break;

        default: break;
    }
    
    if (fe_GPIO == GPIOB || fe_GPIO == GPIOD || fe_GPIO == GPIOF)
    {
        GPIO_MASK = 0xFFFF0000;
        OutputData=(GPIOx->ODATA & GPIO_MASK)>>16;
    }else
    {
        GPIO_MASK = 0x0000FFFF;
        OutputData=(GPIOx->ODATA & GPIO_MASK);
    }
    
    return OutputData;
}


/*********************************************************************************
* Function    : GPIO_SetBits
* Description : Sets the selected data port bits.
* Input       : fe_GPIO:This parameter can be GPIOx where x can be (A..F).
*               fu32_GPIO_Pin:specifies the port bit to read.
*                             This parameter can be GPIO_PIN_x where x can be (0..15).
* Output      : None
**********************************************************************************/
void GPIO_SetBits(enum_GPIOx_t fe_GPIO, uint32_t fu32_GPIO_Pin)
{
    /* Check the parameters */
    assert_param (IS_GPIO_ALL_INSTANCE(fe_GPIO));
    assert_param (IS_GPIO_PIN(fu32_GPIO_Pin));
    
    GPIO_TypeDef *GPIOx;

    switch (fe_GPIO)
    {
        case GPIOA: 
        case GPIOB: 
        {
            GPIOx = GPIOAB;
        }break;

        case GPIOC: 
        case GPIOD: 
        {
            GPIOx = GPIOCD;
        }break;

        case GPIOE: 
        case GPIOF: 
        {
            GPIOx = GPIOEF;
        }break;

        default: break;
    }

    if (fe_GPIO == GPIOB || fe_GPIO == GPIOD || fe_GPIO == GPIOF)
    {
        fu32_GPIO_Pin <<= 16;
    }
    GPIOx->ODATA |= fu32_GPIO_Pin;
}


/*********************************************************************************
* Function    : GPIO_ResetBits
* Description : Resets the selected data port bits.
* Input       : fe_GPIO:This parameter can be GPIOx where x can be (A..F).
*               fu32_GPIO_Pin:specifies the port bit to read.
*                             This parameter can be GPIO_PIN_x where x can be (0..15).
* Output      : None
**********************************************************************************/
void GPIO_ResetBits(enum_GPIOx_t fe_GPIO, uint32_t fu32_GPIO_Pin)
{
    /* Check the parameters */
    assert_param (IS_GPIO_ALL_INSTANCE(fe_GPIO));
    assert_param (IS_GPIO_PIN(fu32_GPIO_Pin));
    
    GPIO_TypeDef *GPIOx;

    switch (fe_GPIO)
    {
        case GPIOA: 
        case GPIOB: 
        {
            GPIOx = GPIOAB;
        }break;

        case GPIOC: 
        case GPIOD: 
        {
            GPIOx = GPIOCD;
        }break;

        case GPIOE: 
        case GPIOF: 
        {
            GPIOx = GPIOEF;
        }break;

        default: break;
    }

    if (fe_GPIO == GPIOB || fe_GPIO == GPIOD || fe_GPIO == GPIOF)
    {
        fu32_GPIO_Pin <<= 16;
    }
    
    GPIOx->ODATA &= ~fu32_GPIO_Pin;
}


/*********************************************************************************
* Function    : GPIO_WriteBit
* Description : Sets or clears the selected data port bit.
* Input       : fe_GPIO:This parameter can be GPIOx where x can be (A..F).
*               fu32_GPIO_Pin:specifies the port bit to read.
*                             This parameter can be GPIO_PIN_x where x can be (0..15).
*               BitVal: specifies the value to be written to the selected bit.
*                       This parameter can be one of the BitAction enum values:
*                       Bit_RESET: to clear the port pin
*                       Bit_SET: to set the port pin
* Output      : None
**********************************************************************************/
void GPIO_WriteBit(enum_GPIOx_t fe_GPIO, uint32_t fu32_GPIO_Pin, BitAction BitVal)
{
    GPIO_TypeDef *GPIOx;
    
    /* Check the parameters */
    assert_param (IS_GPIO_ALL_INSTANCE(fe_GPIO));
    assert_param (IS_GPIO_PIN(fu32_GPIO_Pin));
    assert_param (IS_GPIO_PIN_ACTION(BitVal));

    switch (fe_GPIO)
    {
        case GPIOA: 
        case GPIOB: 
        {
            GPIOx = GPIOAB;
        }break;

        case GPIOC: 
        case GPIOD: 
        {
            GPIOx = GPIOCD;
        }break;

        case GPIOE: 
        case GPIOF: 
        {
            GPIOx = GPIOEF;
        }break;

        default: break;
    }

    if (fe_GPIO == GPIOB || fe_GPIO == GPIOD || fe_GPIO == GPIOF)
    {
        fu32_GPIO_Pin <<= 16;
    }

    if (Bit_SET == BitVal) 
    {
        GPIOx->ODATA |= fu32_GPIO_Pin;
    }
    else 
    {
        GPIOx->ODATA &= ~fu32_GPIO_Pin;
    }
}


/*********************************************************************************
* Function    : GPIO_Write
* Description : Writes data to the specified GPIO data port.
* Input       : fe_GPIO:This parameter can be GPIOx where x can be (A..F).
*               PortVal: specifies the value to be written to the port output data register.
* Output      : None
**********************************************************************************/
void GPIO_Write(enum_GPIOx_t fe_GPIO, uint16_t PortVal)
{    
    /* Check the parameters */
    assert_param (IS_GPIO_ALL_INSTANCE(fe_GPIO));
    
    GPIO_TypeDef *GPIOx;

    switch (fe_GPIO)
    {
        case GPIOA: 
        case GPIOB: 
        {
            GPIOx = GPIOAB;
        }break;

        case GPIOC: 
        case GPIOD: 
        {
            GPIOx = GPIOCD;
        }break;

        case GPIOE: 
        case GPIOF: 
        {
            GPIOx = GPIOEF;
        }break;

        default: break;
    }

    if (fe_GPIO == GPIOB || fe_GPIO == GPIOD || fe_GPIO == GPIOF)
    {
        GPIOx->ODATA = (PortVal<<16);
    }else
    {
        GPIOx->ODATA = PortVal;
    }
}


/*********************************************************************************
* Function    : GPIO_ITConfig
* Description : GPIO IT Config.
* Input       : fe_GPIO:This parameter can be GPIOx where x can be (A..F).
*               fu32_GPIO_Pin:specifies the port bit to read.
*                             This parameter can be GPIO_PIN_x where x can be (0..15).
*               NewState:  new it state of the port pin remapping.
*                          This parameter can be: ENABLE or DISABLE.
* Output      : None
**********************************************************************************/
void GPIO_ITConfig(enum_GPIOx_t fe_GPIO, uint32_t fu32_GPIO_Pin, FunctionalState NewState)
{
    
    /* Check the parameters */
    assert_param (IS_GPIO_ALL_INSTANCE(fe_GPIO));
    assert_param (IS_GPIO_PIN(fu32_GPIO_Pin));

    GPIO_TypeDef *GPIOx;

    switch (fe_GPIO)
    {
        case GPIOA: 
        case GPIOB: 
        {
            GPIOx = GPIOAB;
        }break;

        case GPIOC: 
        case GPIOD: 
        {
            GPIOx = GPIOCD;
        }break;

        case GPIOE: 
        case GPIOF: 
        {
            GPIOx = GPIOEF;
        }break;

        default: break;
    }

    if (fe_GPIO == GPIOB || fe_GPIO == GPIOD || fe_GPIO == GPIOF)
    {
         fu32_GPIO_Pin <<= 16;
    }
    
    if (ENABLE == NewState) 
    {
        GPIOx->IEN |= fu32_GPIO_Pin;
    }
    else 
    {
        GPIOx->IEN &= ~fu32_GPIO_Pin;
    }  
}


/*********************************************************************************
* Function    : GPIO_ITClear
* Description : GPIO IT Clear.
* Input       : fe_GPIO:This parameter can be GPIOx where x can be (A..F).
*               fu32_GPIO_Pin:specifies the port bit to read.
*                             This parameter can be GPIO_PIN_x where x can be (0..15).
* Output      : None
**********************************************************************************/
void GPIO_ITClear(enum_GPIOx_t fe_GPIO, uint32_t fu32_GPIO_Pin)
{
        
    /* Check the parameters */
    assert_param (IS_GPIO_ALL_INSTANCE(fe_GPIO));
    assert_param (IS_GPIO_PIN(fu32_GPIO_Pin));

    GPIO_TypeDef *GPIOx;

    switch (fe_GPIO)
    {
        case GPIOA: 
        case GPIOB: 
        {
            GPIOx = GPIOAB;
        }break;

        case GPIOC: 
        case GPIOD: 
        {
            GPIOx = GPIOCD;
        }break;

        case GPIOE: 
        case GPIOF: 
        {
            GPIOx = GPIOEF;
        }break;

        default: break;
    }

    if (fe_GPIO == GPIOB || fe_GPIO == GPIOD || fe_GPIO == GPIOF)
    {
         fu32_GPIO_Pin <<= 16;
    }
    GPIOx->IC |= fu32_GPIO_Pin;
}



/*********************************************************************************
* Function    : GPIO_GetITStatus
* Description : Get GPIO Pin IT Status.
* Input       : fe_GPIO:This parameter can be GPIOx where x can be (A..F).
*               fu32_GPIO_Pin:specifies the port bit to read.
*                             This parameter can be GPIO_PIN_x where x can be (0..15).
* Output      : ITStatus
**********************************************************************************/
ITStatus GPIO_GetITStatus(enum_GPIOx_t fe_GPIO, uint32_t fu32_GPIO_Pin)
{
    /* Check the parameters */
    assert_param (IS_GPIO_ALL_INSTANCE(fe_GPIO));
    assert_param (IS_GPIO_PIN(fu32_GPIO_Pin));
    
    GPIO_TypeDef *GPIOx;
    ITStatus bitstatus = RESET;
    
    switch (fe_GPIO)
    {
        case GPIOA: 
        case GPIOB: 
        {
            GPIOx = GPIOAB;
        }break;

        case GPIOC: 
        case GPIOD: 
        {
            GPIOx = GPIOCD;
        }break;

        case GPIOE: 
        case GPIOF: 
        {
            GPIOx = GPIOEF;
        }break;

        default: break;
    }

    if (fe_GPIO == GPIOB || fe_GPIO == GPIOD || fe_GPIO == GPIOF)
    {
        fu32_GPIO_Pin <<= 16;
    }
    
    if (GPIOx->RIS & fu32_GPIO_Pin) 
    {
        bitstatus = SET;
    }
    else 
    {
        bitstatus = RESET;
    } 
    return bitstatus;
}
