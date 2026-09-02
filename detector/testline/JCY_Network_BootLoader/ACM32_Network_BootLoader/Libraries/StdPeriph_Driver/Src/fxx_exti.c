
#include "fxx_std.h"
/*********************************************************************************
* Function    : HAL_EXTI_SetConfigLine
* Description : 
* Input       : 
* Output       : 
**********************************************************************************/
void EXTI_Init(EXTI_InitTypeDef* EXTI_InitStruct)
{
    uint32_t lu32_IndexLine;

    /* Check the parameters */
    assert_param(IS_EXTI_MODE(EXTI_InitStruct->EXTI_Mode));
    assert_param(IS_EXTI_TRIGGER(EXTI_InitStruct->EXTI_Trigger));
    assert_param(IS_EXTI_ALL_LINE(EXTI_InitStruct->EXTI_Line));  
    assert_param(IS_FUNCTIONAL_STATE(EXTI_InitStruct->EXTI_LineCmd));

    lu32_IndexLine = EXTI_InitStruct->EXTI_Line;
    if (EXTI_InitStruct->EXTI_LineCmd != DISABLE)
    {
        /* Interrupt Mode */
        if (EXTI_InitStruct->EXTI_Mode == EXTI_MODE_INTERRUPT)
        {
            EXTI->IENR |=  lu32_IndexLine;
            EXTI->EENR &= ~lu32_IndexLine;
        }
        /* Event Mode */
        else if (EXTI_InitStruct->EXTI_Mode == EXTI_MODE_EVENT)
        {
            EXTI->EENR |=  lu32_IndexLine;
            EXTI->IENR &= ~lu32_IndexLine;
        }


        if (EXTI_InitStruct->EXTI_Trigger == EXTI_TRIGGER_RISING) 
        {
            EXTI->RTENR |=  lu32_IndexLine;
            EXTI->FTENR &= ~lu32_IndexLine;
        }
        else if (EXTI_InitStruct->EXTI_Trigger == EXTI_TRIGGER_FALLING)
        {
            EXTI->FTENR |=  lu32_IndexLine;
            EXTI->RTENR &= ~lu32_IndexLine;
        }
        else 
        {
            EXTI->FTENR |= lu32_IndexLine;
            EXTI->RTENR |= lu32_IndexLine;
        }
    }
    else
    {
        EXTI->IENR &= ~lu32_IndexLine;
        EXTI->EENR &= ~lu32_IndexLine;
    }
    
}

/*********************************************************************************
* Function    : EXTI_DeInit
* Description :  Deinitializes the EXTI peripheral registers to their default reset values.
* Input       : 
* Output       : 
**********************************************************************************/
void EXTI_DeInit(void)
{
    EXTI->FTENR   = 0x00000000;
    EXTI->EENR    = 0x00000000;
    EXTI->RTENR   = 0x00000000; 
    EXTI->FTENR   = 0x00000000; 
    EXTI->SWIER   = 0x00000000;
    EXTI->PDR     = 0x00000000;
    EXTI->EXTICR1 = 0x00000000;
    EXTI->EXTICR2 = 0x00000000;
}

/*********************************************************************************
* Function    : EXTI_StructInit
* Description : Fills each EXTI_InitStruct member with its reset value.
* Input       : EXTI_InitStruct: pointer to a EXTI_InitTypeDef structure which will
*               be initialized.
* Output       : 
**********************************************************************************/
void EXTI_StructInit(EXTI_InitTypeDef* EXTI_InitStruct)
{
    EXTI_InitStruct->EXTI_Line = EXTI_LINE_NONE;
    EXTI_InitStruct->EXTI_Mode = EXTI_MODE_INTERRUPT;
    EXTI_InitStruct->EXTI_Trigger = EXTI_TRIGGER_RISING;
    EXTI_InitStruct->EXTI_LineCmd = DISABLE;
}

/*********************************************************************************
* Function    : EXTI_LineConfig
* Description : Selects the GPIO pin used as EXTI Line.
* Input       :  GPIO_PortSource: selects the GPIO port to be used as source for EXTI lines.
*                                 This parameter can be EXTI_GPIOx where x can be (A..F).
*                GPIO_PinSource: specifies the EXTI line to be configured.
*                                This parameter can be GPIO_PinSourcex where x can be (0..15).
* Output       : 
**********************************************************************************/
void EXTI_LineConfig(uint32_t GPIO_PortSource, uint32_t GPIO_PinSource)
{

    uint32_t lu32_IndexLine;   
    /* Check the parameters */
    assert_param(IS_EXTI_GPIOSEL(GPIO_PortSource));
    assert_param(IS_GPIO_PIN_SOURCE(GPIO_PinSource));

    /* PIN0 ~ 15 trigger from GPIO */

    lu32_IndexLine = 0;

    while(GPIO_PinSource >> lu32_IndexLine != 0x01)
    {
        lu32_IndexLine++;
    }

    /* PIN0 ~ 7 */
    if (lu32_IndexLine < 8) 
    {
        EXTI->EXTICR1 = (EXTI->EXTICR1 & ~(0x0F << (lu32_IndexLine * 4))) | GPIO_PortSource << (lu32_IndexLine * 4);
    }
    /* PIN8 ~ 15 */
    else 
    {
        lu32_IndexLine -= 8;
        
        EXTI->EXTICR2 = (EXTI->EXTICR2 & ~(0x0F << (lu32_IndexLine * 4))) | GPIO_PortSource << (lu32_IndexLine * 4);
    }
}


/*********************************************************************************
* Function    : EXTI_GenerateSWInterrupt
* Description : Generates a Software interrupt.
* Input       : EXTI_Line: specifies the EXTI lines to be enabled or disabled.
*               This parameter can be any combination of EXTI_Linex where x can be (0..23).
* Output       : 
**********************************************************************************/
void EXTI_GenerateSWInterrupt(uint32_t EXTI_Line)
{

    assert_param(IS_EXTI_ALL_LINE(EXTI_Line)) ;
    /* Set SWIER BIT */
    EXTI->SWIER |= EXTI_Line;
}


/*********************************************************************************
* Function    : EXTI_GetFlagStatus
* Description : Checks whether the specified EXTI line flag is set or not.
* Input       : EXTI_Line: specifies the EXTI lines to be enabled or disabled.
*               This parameter can be any combination of EXTI_Linex where x can be (0..23).
* Output       : FlagStatus
**********************************************************************************/
FlagStatus EXTI_GetFlagStatus(uint32_t EXTI_Line)
{
    FlagStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_EXTI_ALL_LINE(EXTI_Line)) ;
  
    if ((EXTI_Line& EXTI->PDR) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }
    return bitstatus;
}

/*********************************************************************************
* Function    : EXTI_ClearFlag
* Description : Clears the EXTI's line pending flags.
* Input       : EXTI_Line: specifies the EXTI lines to be enabled or disabled.
*               This parameter can be any combination of EXTI_Linex where x can be (0..23).
* Output       : 
**********************************************************************************/
void EXTI_ClearFlag(uint32_t EXTI_Line)
{
    /* Check the parameters */
    assert_param(IS_EXTI_ALL_LINE(EXTI_Line)) ;
  
    EXTI->PDR |=  EXTI_Line;
}


/*********************************************************************************
* Function    : EXTI_GetITStatus
* Description : Checks whether the specified EXTI line is asserted or not.
* Input       : EXTI_Line: specifies the EXTI lines to be enabled or disabled.
*               This parameter can be any combination of EXTI_Linex where x can be (0..23).
* Output       : ITStatus
**********************************************************************************/
ITStatus EXTI_GetITStatus(uint32_t EXTI_Line)
{
    ITStatus bitstatus = RESET;
    uint32_t enablestatus = 0;
    /* Check the parameters */
    assert_param(IS_EXTI_ALL_LINE(EXTI_Line)) ;
  
    enablestatus =  EXTI->IENR & EXTI_Line;
    if (((EXTI->PDR & EXTI_Line) != (uint32_t)RESET) && (enablestatus != (uint32_t)RESET))
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }
    return bitstatus;
}

/*********************************************************************************
* Function    : EXTI_ClearITPendingBit
* Description : Clears the EXTI's line pending bits.
* Input       : EXTI_Line: specifies the EXTI lines to be enabled or disabled.
*               This parameter can be any combination of EXTI_Linex where x can be (0..23).
* Output       : 
**********************************************************************************/
void EXTI_ClearITPendingBit(uint32_t EXTI_Line)
{
    /* Check the parameters */
    assert_param(IS_EXTI_ALL_LINE(EXTI_Line)) ;
  
    EXTI->PDR |=  EXTI_Line;
}





