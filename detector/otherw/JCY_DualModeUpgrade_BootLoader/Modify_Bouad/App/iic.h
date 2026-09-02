#ifndef __IIC_H__
#define __IIC_H__

#include "fxx_std.h"

#define SDA_READ()  			GPIO_ReadInputDataBit(GPIOB, GPIO_PIN_9)       //读取SDA

#define SCL_H()					GPIO_SetBits(GPIOB, GPIO_PIN_8)	
#define SCL_L() 			    GPIO_ResetBits(GPIOB, GPIO_PIN_8)

#define SDA_H()					GPIO_SetBits(GPIOB, GPIO_PIN_9)	
#define SDA_L() 			    GPIO_ResetBits(GPIOB, GPIO_PIN_9)

#define SCL_OUTPUT()		    


#define IIC_ACK			  0
#define IIC_NACK		  1




#define SDA_INPUT()             GPIOAB->DIR &=  ~(1<<25)   					//开漏模式，拉高可以切换成输出，但需要上拉电阻
#define SDA_OUTPUT()    				GPIOAB->DIR |=  (1<<25)   		 		


void User_IIC_Init(void);

void IIC_Start(void);

void IIC_Stop(void);
	
uint8_t IIC_Write_Byte(uint8_t IIC_byte);

uint8_t IIC_Send_Array(uint8_t DevAddress, const uint8_t *pData, uint16_t Size);

void IIC_Read_Bytes(uint8_t DevAddress, uint8_t* byte, uint16_t NumByteToRead);

uint8_t IIC_Receive_Array(uint8_t DevAddress, const uint8_t *pTxData, uint16_t TxSize, uint8_t *pRxData, uint16_t RxSize);

void IIC_Send_Ack(void);

void IIC_Send_NAck(void);


void IIC_SDA_OUTPUT(void);

void IIC_SDA_INPUT(void);
#endif
