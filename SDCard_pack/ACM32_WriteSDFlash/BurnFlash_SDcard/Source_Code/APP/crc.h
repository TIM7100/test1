#ifndef __CRC16_H__
#define __CRC16_H__

#include "ACM32Fxx_HAL.h"

uint16_t get_crc_16(const uint8_t* pData, uint32_t nLength);
uint16_t crc16_ccitt_continue(uint16_t crc, const uint8_t* pData, uint32_t nLength);

#endif