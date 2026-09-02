/*
  ******************************************************************************
  * @file    fxx_std.h
  * @version V1.0.0
  * @date    2020
  * @brief   Config header file.
  ******************************************************************************
*/
#ifndef __FXX_STD_H__
#define __FXX_STD_H__


/** @addtogroup Device_Included
  * @{
  */

#include "f4_f3.h"
#include "system_f4_f3.h"
#include "system_accelerate.h"
/**
  * @}
  */



#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050) /* ARM Compiler V6 */
  #ifndef __weak
    #define __weak  __attribute__((weak))
  #endif
#endif

/* Standard integer type definitions (mapped to stdint.h types) */
typedef int8_t      int8;
typedef int16_t     int16;
typedef int32_t     int32;
typedef int64_t     int64;


#define STD_I2C_MODULE_ENABLED
#define STD_UART_MODULE_ENABLED
#define STD_DMA_MODULE_ENABLED
#define STD_GPIO_MODULE_ENABLED
#define STD_RTC_MODULE_ENABLED
#define STD_EFLASH_MODULE_ENABLED
#define STD_SYSTICK_ENABLED
#define STD_SPI_MODULE_ENABLED
#define STD_EXTI_MODULE_ENABLED
#define STD_CAN_MODULE_ENABLED

#define STD_ADC_MODULE_ENABLED       
#define STD_TIM_MODULE_ENABLED    

#define STD_FSUSB_MODULE_ENABLED   


#ifdef STD_I2C_MODULE_ENABLED
    #include "fxx_i2c.h"
#endif

#ifdef STD_UART_MODULE_ENABLED
    #include "fxx_uart.h"
#endif

#ifdef STD_DMA_MODULE_ENABLED
    #include "fxx_dma.h"
#endif

#ifdef STD_RTC_MODULE_ENABLED
    #include "fxx_rtc.h"
#endif

#ifdef STD_GPIO_MODULE_ENABLED
    #include "fxx_gpio.h"
#endif

#ifdef STD_EFLASH_MODULE_ENABLED
    #include "fxx_eflash.h"
    #include "fxx_eflash_ex.h"
#endif

#ifdef STD_SPI_MODULE_ENABLED
    #include "fxx_spi.h"
#endif


#ifdef STD_EXTI_MODULE_ENABLED
    #include "fxx_exti.h"
#endif

#ifdef STD_ADC_MODULE_ENABLED
    #include "fxx_adc.h"
#endif


#ifdef STD_TIM_MODULE_ENABLED
    #include "fxx_tim.h"
#endif

#ifdef STD_CAN_MODULE_ENABLED
    #include "fxx_can.h"
#endif

#ifdef STD_FSUSB_MODULE_ENABLED    
    #include "fxx_fsusb.h"  
#endif  

/* USE FULL ASSERT */
#define USE_FULL_ASSERT             (0)

#ifdef  USE_FULL_ASSERT


/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function
  *   which reports the name of the source file and the source
  *   line number of the call that failed. 
  *   If expr is true, it returns no value.
  * @retval None
  */
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */
  
  
  
  
  
#endif
