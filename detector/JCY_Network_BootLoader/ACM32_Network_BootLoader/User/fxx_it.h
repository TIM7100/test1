/*
  ******************************************************************************
  * @file    fxx_it.h
  * @brief   fxx_it source header.
  ******************************************************************************
*/


#ifndef __FXX_IT_H
#define __FXX_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "f4_f3.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void GPIOAB_IRQHandler(void);
void GPIOCD_IRQHandler(void); 

#ifdef __cplusplus
}
#endif

#endif


