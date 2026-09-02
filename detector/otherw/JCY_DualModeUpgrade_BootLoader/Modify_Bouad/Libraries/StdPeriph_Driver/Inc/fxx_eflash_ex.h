/*
  ******************************************************************************
  * @file    EFlash_EX.h
  * @version V1.0.0
  * @date    2021
  * @brief   Header file of EFlash extention module   
  ******************************************************************************
*/
#ifndef _EFlash_EX_H__
#define _EFlash_EX_H__

#include "stdint.h"

/* EFlash_Return_To_Boot */
void EFlash_Return_to_Boot(void);

/* EFlash_Init_Para */ 
void EFlash_Init_Para(uint32_t fu32_freq);   

/* EFlash_ErasePage_EX */   
void EFlash_ErasePage_EX(uint32_t fu32_Addr);  

/* EFlash_Program_Word_EX */     
void EFlash_Program_Word_EX(uint32_t fu32_Addr, uint32_t fu32_Data);  

#endif
