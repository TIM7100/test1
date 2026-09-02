/*
  ******************************************************************************
  * @file    GPIO.h
  * @version V1.0.0
  * @date    2020
  * @brief   Header file of GPIO  module.
  ******************************************************************************
*/
#ifndef __EXTI_H__
#define __EXTI_H__

#include "f4_f3.h"

/** @defgroup EXTI_Line  EXTI Line
  * @{
  */
#define EXTI_LINE_0                (0x000001)
#define EXTI_LINE_1                (0x000002)
#define EXTI_LINE_2                (0x000004)
#define EXTI_LINE_3                (0x000008)
#define EXTI_LINE_4                (0x000010)
#define EXTI_LINE_5                (0x000020)
#define EXTI_LINE_6                (0x000040)
#define EXTI_LINE_7                (0x000080)
#define EXTI_LINE_8                (0x000100)
#define EXTI_LINE_9                (0x000200)
#define EXTI_LINE_10               (0x000300)
#define EXTI_LINE_11               (0x000400)
#define EXTI_LINE_12               (0x001000)
#define EXTI_LINE_13               (0x002000)
#define EXTI_LINE_14               (0x004000)
#define EXTI_LINE_15               (0x008000)
#define EXTI_LINE_16               (0x010000)
#define EXTI_LINE_17               (0x020000)
#define EXTI_LINE_18               (0x040000)
#define EXTI_LINE_19               (0x080000)
#define EXTI_LINE_20               (0x100000)
#define EXTI_LINE_21               (0x200000)
#define EXTI_LINE_22               (0x400000)
#define EXTI_LINE_23               (0x800000) 
#define EXTI_LINE_NONE             (0x000000)  /* No interrupt selected */
#define EXTI_LINE_MASK             (0xFFFFFFU)

/**
  * @}
  */
  
  
/** @defgroup EXTI_Line  EXTI Line
  * @{
  */
#define GPIO_PinSource0                (0x000001)
#define GPIO_PinSource1                (0x000002)
#define GPIO_PinSource2                (0x000004)
#define GPIO_PinSource3                (0x000008)
#define GPIO_PinSource4                (0x000010)
#define GPIO_PinSource5                (0x000020)
#define GPIO_PinSource6                (0x000040)
#define GPIO_PinSource7                (0x000080)
#define GPIO_PinSource8                (0x000100)
#define GPIO_PinSource9                (0x000200)
#define GPIO_PinSource10               (0x000300)
#define GPIO_PinSource11               (0x000400)
#define GPIO_PinSource12               (0x001000)
#define GPIO_PinSource13               (0x002000)
#define GPIO_PinSource14               (0x004000)
#define GPIO_PinSource15               (0x008000)
/**
  * @}
  */





/** @defgroup EXTI_GPIOSel  EXTI GPIOSel
  * @brief
  * @{
  */
#define EXTI_GPIOA                          0x00000000u
#define EXTI_GPIOB                          0x00000001u
#define EXTI_GPIOC                          0x00000002u
#define EXTI_GPIOD                          0x00000003u
#define EXTI_GPIOE                          0x00000004u
#define EXTI_GPIOF                          0x00000005u
/**
  * @}
  */





/** @defgroup  EXTI Private Macros
  * @{
  */
#define IS_EXTI_ALL_LINE(LINE)    ( (LINE) | (EXTI_LINE_MASK) )      


/** 
  * @brief  EXTI mode enumeration  
  */

typedef enum
{
 EXTI_MODE_INTERRUPT = 0x00000001,
 
 EXTI_MODE_EVENT    = 0x00000002

}EXTIMode_TypeDef;

#define IS_EXTI_MODE(__MODE__)     (((__MODE__) == EXTI_MODE_INTERRUPT) || \
                                    ((__MODE__) == EXTI_MODE_EVENT))



/** @defgroup EXTI_Trigger  EXTI Trigger
  * @{
  */
typedef enum
{
 EXTI_TRIGGER_RISING  = 0x00000001,

 EXTI_TRIGGER_FALLING = 0x00000002,

 EXTI_TRIGGER_RISING_FALLING =(EXTI_TRIGGER_RISING | EXTI_TRIGGER_FALLING)

}EXTITrigger_TypeDef;

#define IS_EXTI_TRIGGER(__TRIGGER__)     (((__TRIGGER__) == EXTI_TRIGGER_RISING)  || \
                                          ((__TRIGGER__) == EXTI_TRIGGER_FALLING) || \
                                          ((__TRIGGER__) == EXTI_TRIGGER_RISING_FALLING))


#define IS_EXTI_GPIOSEL(__GPIOSEL__)     (((__GPIOSEL__) == EXTI_GPIOA) || \
                                          ((__GPIOSEL__) == EXTI_GPIOB) || \
                                          ((__GPIOSEL__) == EXTI_GPIOC) || \
                                          ((__GPIOSEL__) == EXTI_GPIOD) || \
                                          ((__GPIOSEL__) == EXTI_GPIOE) || \
                                          ((__GPIOSEL__) == EXTI_GPIOF))

#define IS_GPIO_PIN_SOURCE(PINSOURCE) (((PINSOURCE) == GPIO_PinSource0) || \
                                       ((PINSOURCE) == GPIO_PinSource1) || \
                                       ((PINSOURCE) == GPIO_PinSource2) || \
                                       ((PINSOURCE) == GPIO_PinSource3) || \
                                       ((PINSOURCE) == GPIO_PinSource4) || \
                                       ((PINSOURCE) == GPIO_PinSource5) || \
                                       ((PINSOURCE) == GPIO_PinSource6) || \
                                       ((PINSOURCE) == GPIO_PinSource7) || \
                                       ((PINSOURCE) == GPIO_PinSource8) || \
                                       ((PINSOURCE) == GPIO_PinSource9) || \
                                       ((PINSOURCE) == GPIO_PinSource10) || \
                                       ((PINSOURCE) == GPIO_PinSource11) || \
                                       ((PINSOURCE) == GPIO_PinSource12) || \
                                       ((PINSOURCE) == GPIO_PinSource13) || \
                                       ((PINSOURCE) == GPIO_PinSource14) || \
                                       ((PINSOURCE) == GPIO_PinSource15))

/**
  * @brief  EXTI Configuration structure definition
  */
typedef struct
{
    uint32_t EXTI_Line;        /*!< The Exti line to be configured. This parameter
                                   can be a value of @ref EXTI_Line */
    
    EXTIMode_TypeDef EXTI_Mode;        /*!< The Exit Mode to be configured for a core.
                                   This parameter can be a combination of @ref EXTI_Mode */
    
    EXTITrigger_TypeDef EXTI_Trigger;     /*!< The Exti Trigger to be configured. This parameter
                                   can be a value of @ref EXTI_Trigger */
    
    FunctionalState EXTI_LineCmd;     /*!< Specifies the new state of the selected EXTI lines.
                                         This parameter can be set either to ENABLE or DISABLE */  
}EXTI_InitTypeDef;

/**
  * @}
  */

/** @brief  __HAL_EXTI_LINE_IT_ENABLE 
  * @param  __LINE__: EXTI line.
  *         This parameter can be a value of @ref EXTI_Line
  */
#define __HAL_EXTI_LINE_IT_ENABLE(__LINE__)    (EXTI->IENR |= (__LINE__))

/** @brief  __HAL_EXTI_LINE_IT_DISABLE 
  * @param  __LINE__: EXTI line.
  *         This parameter can be a value of @ref EXTI_Line
  */
#define __HAL_EXTI_LINE_IT_DISABLE(__LINE__)    (EXTI->IENR &= ~(__LINE__))

/** @brief  __HAL_EXTI_LINE_EVENT_ENABLE 
  * @param  __LINE__: EXTI line.
  *         This parameter can be a value of @ref EXTI_Line
  */
#define __HAL_EXTI_LINE_EVENT_ENABLE(__LINE__)    (EXTI->EENR |= (__LINE__))

/** @brief  __HAL_EXTI_LINE_EVENT_DISABLE 
  * @param  __LINE__: EXTI line.
  *         This parameter can be a value of @ref EXTI_Line
  */
#define __HAL_EXTI_LINE_EVENT_DISABLE(__LINE__)    (EXTI->EENR &= ~(__LINE__))





void EXTI_Init(EXTI_InitTypeDef* EXTI_InitStruct);

void EXTI_DeInit(void);

void EXTI_StructInit(EXTI_InitTypeDef* EXTI_InitStruct);

void EXTI_LineConfig(uint32_t GPIO_PortSource, uint32_t GPIO_PinSource);

void EXTI_GenerateSWInterrupt(uint32_t EXTI_Line);

FlagStatus EXTI_GetFlagStatus(uint32_t EXTI_Line);

void EXTI_ClearFlag(uint32_t EXTI_Line);

ITStatus EXTI_GetITStatus(uint32_t EXTI_Line);

void EXTI_ClearITPendingBit(uint32_t EXTI_Line);



#endif
