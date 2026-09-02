#ifndef  __soft_iic_H__
#define  __soft_iic_H__


#include "filtering_iic.h"
#include "fxx_tim.h"
#include "fxx_std.h"
#include "crc.h"
#include "delay.h"
#include  "iic.h"

#define  I2C_POWER_STATUS   1

#if I2C_POWER_STATUS 
#define I2C_POWER_PORT GPIOA
#define I2C_POWER_PORT1 GPIOB
#define I2C_POWER_PIN  GPIO_PIN_1
#define I2C_POWER_PIN1  GPIO_PIN_7
#define I2C_POWER_ON()   GPIO_SetBits(I2C_POWER_PORT, I2C_POWER_PIN)
#define I2C_POWER_OFF()  GPIO_ResetBits(I2C_POWER_PORT, I2C_POWER_PIN)
#define I2C_POWER_ON1()   GPIO_SetBits(I2C_POWER_PORT1, I2C_POWER_PIN1)
#define I2C_POWER_OFF1()  GPIO_ResetBits(I2C_POWER_PORT1, I2C_POWER_PIN1)
#endif


#define SCL_1   GPIOAB->ODATA |= 0x01000000
#define SCL_0   GPIOAB->ODATA &= ~(0x01000000)
#define SDA_1   GPIOAB->ODATA |= 0x02000000
#define SDA_0   GPIOAB->ODATA &= ~(0x02000000)
#define R_SDA   ((GPIOAB-> IDATA) & 0x02000000)
#define SDA_OUT GPIOAB->DIR |=0x02000000
#define SDA_IN  GPIOAB->DIR &=~0x02000000






void Uart_Init(uint32_t Baudrate);
/* iic_interface_Test */

void soft_iic_init();
void FreeTime(u32 CountNum);
void IIC_filtering_write(UINT8 slave_addr,  const uint8_t *txdata, UINT8 datalen);
void IIC_filtering_read(UINT8 slave_addr, UINT8 *txdata, UINT16 datalen);
void FreeTime1(u32 CountNum);
void Clk_Turn(UINT16 len);
void Data_Turn(UINT16 len);
void IIC_filtering_stop(int del1, int del2, int del3, int turn_num);
void IIC_filtering_start();
UINT8 IIC_r_ac(void);
void IIC_w_ac(UINT8 bit);
void IIC_Start_0(void);

#endif