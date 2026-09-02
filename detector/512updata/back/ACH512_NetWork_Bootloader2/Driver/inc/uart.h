#ifndef __UART_H__
#define __UART_H__

#include "common.h"

#define  LCDPORT    UARTA  //调试打印的端口
#define  DEBUG_UART UARTB

extern volatile UINT8 tx_flag;
extern volatile UINT8 rx_flag;
extern volatile UINT8 uart_rx_buf[];
extern volatile UINT8 rx_count;

void uart_set_baud_rate(UINT32 uart_index, UINT32 cpu_mhz, UINT32 baud_rate);

void UartInit(UINT32 uart_index, UINT32 baud_rate);

void outbyte(UINT32 uart_index, char c);

void uart_send_bytes(UINT32 uart_index, UINT8 *buff, UINT32 length);

void printf_arraydata(u8 *arraydata, u32 data_len);

void UartSend(char *databuf);

void CheckBusy(void)  ;

#endif


