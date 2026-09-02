/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : iso7816ms.h
 * Description : iso7816 master/slave driver header file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/

#ifndef __ISO7816MS_H__
#define __ISO7816MS_H__

#include "common.h"
#define PAD_SCC_MS_RST   (1<<25) //GPIO25中断作为RST中断

#define PAD_SCC_MS_RST_EN  (1ul<<30)
#define PAD_SCC_MS_CLK_EN  (1<<28)
#define PAD_SCC_MS_CIO_EN  (1<<26)
#define PAD_SCC_MS_DET_EN  (1<<24)
#define PAD_SCC_MS_VCC_EN  (1<<22)

//变量声明
extern  volatile UINT8 ISO7816_buff;
extern  UINT8 T0_atr_buffer[20];

extern volatile UINT8 ISO7816_RX_flag;
extern volatile UINT8 ISO7816_atr_flag;
extern volatile UINT8 ISO7816_rst_flag;

//函数定义


/************************************************************************
 * function   : ISO7816_t0_tx_poll
 * Description: Transmits data through the SCI7816MS.
 * input :
 *         tx_data: the data to transmit.
 *         length: the length of data
 * return: none
 ************************************************************************/
void ISO7816_t0_tx_poll(UINT8 *tx_data, UINT32 length);


/************************************************************************
 * function   : ISO7816_t0_rx_poll
 * Description: Receives data through the SCI7816MS.
 * input :
 *         rx_data: the address of received data.
 *         length: the length of data
 * return: none
 ************************************************************************/
UINT8 ISO7816_t0_rx_poll(UINT8 *rx_data, UINT32 length);

/************************************************************************
 * function   : ISO7816_change_baud_rate
 * Description: change baud rate according to para of PPS
 * input :
 *         UINT8 para: the parameter of change baud rate
 * return: none
 ************************************************************************/
void ISO7816_change_baud_rate(UINT8 para);

/************************************************************************
 * function   : ISO7816_delay_etu_num (only useful in rcv direction)
 * Description: delay etu number time
 * input :
 *         etu_num: delay etu number
 * return: none
 ************************************************************************/
void ISO7816_delay_etu_num(UINT16 etu_num);

/*******************************************************************************
* Function Name  : ISO7816S_delay_cck_num
* Description    : delay CCK number time
* Input          : - cck_num: delay CCK number.
* Output         : None
* Return         : None
*******************************************************************************/
void ISO7816_delay_cck_num(UINT32 cck_num);

/************************************************************************
 * function   : ISO7816_rcv_to_send
 * Description: change receive to send mode
 * input : none
 * return: none
 ************************************************************************/
void ISO7816_rcv_to_send(void);

/************************************************************************
 * function   : ISO7816_send_to_rcv
 * Description: change receive to send mode
 * input : none
 * return: none
 ************************************************************************/
void ISO7816_send_to_rcv(void);

/************************************************************************
 * function   : ISO7816_slave_int_en
 * Description: enable the interrupt of slave
 * input : none
 * return: none
 ************************************************************************/
void ISO7816_slave_int_en(void);

/************************************************************************
 * function   : ISO7816_slave_init
 * Description: ISO7816 slvae initial
 * input : none
 * return: none
 ************************************************************************/
void ISO7816_slave_init(void);

/************************************************************************
 * function   : ISO7816_master_init
 * Description: ISO7816 master initial
 * input : none
 * return: none
 ************************************************************************/
void ISO7816_master_init(void);

/************************************************************************
 * function   : ISO7816_master_warm_rst
 * Description: ISO7816 master warm reset
 * input : none
 * return: none
 ************************************************************************/
void ISO7816_master_warm_rst(void);

/************************************************************************
 * function   : ISO7816_master_cold_rst
 * Description: ISO7816 master cold reset
 * input : none
 * return: none
 ************************************************************************/
void ISO7816_master_cold_rst(void);

#endif

