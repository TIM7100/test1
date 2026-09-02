/*
  ******************************************************************************
  * @file    IC_MX20.h
  * @author  PJ
  * @version V1.0.0
  * @date    2024
  * @brief   MX20 driver header file.
  ******************************************************************************
*/
#ifndef __IC_MX20_H__
#define __IC_MX20_H__

#include "fxx_std.h"

uint8_t IIC_Transmit(uint8_t DevAddress, uint8_t *pData, uint16_t Size);
uint8_t IIC_S_Transmit(uint8_t DevAddress, uint8_t *pData, uint16_t Size);
uint8_t IIC_Receive(uint8_t DevAddress, uint8_t *pWData, uint16_t wSize, uint8_t *pRData, uint16_t rSize, uint8_t Delay_flag);

#endif
