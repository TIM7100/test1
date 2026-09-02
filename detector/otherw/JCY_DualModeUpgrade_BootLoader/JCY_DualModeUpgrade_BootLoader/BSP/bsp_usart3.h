#ifndef __USART3_H__
#define __USART3_H__

#include "fxx_std.h"
#include "LoopList.h"


#define  REG_UART3_DR         (*((volatile u32*)(0x40004800)))
#define  REG_UART3_IE         (*((volatile u32*)(0x40004838)))
#define  REG_GPIOAB_RIS       (*((volatile u32*)(0x4000483C)))
#define  REG_GPIOAB_MIS       (*((volatile u32*)(0x40004840)))
#define  REG_GPIOAB_ICR       (*((volatile u32*)(0x40004844)))

#define  READ_UART3_IE_RXI    ((REG_UART3_IE & 0x10) == 0x10)
#define  READ_UART3_IE_RIS    ((REG_UART3_IE & 0x10) == 0x10)


void UART3_Init(uint32_t Baudrate);
void UART_printf(UART_TypeDef* UARTx, const char *fmt, ...);

#endif
