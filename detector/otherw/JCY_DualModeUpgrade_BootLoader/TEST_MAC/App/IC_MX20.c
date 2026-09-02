#include "IC_MX20.h"
#include "delay.h"
#include "iic.h"


/************************************************************************
 * function   : IIC_Transmit
 * Description: 写入多个字节
 * input : uint8_t DevAddress
 *         uint8_t* order: 写入地址和命令
 *         uint8_t Num: 要写入的个数
 * return: none
 ************************************************************************/
uint8_t IIC_Transmit(uint8_t DevAddress, uint8_t *pData, uint16_t Size)
{
	uint8_t i;
	IIC_Start();
	

	if (IIC_Write_Byte(DevAddress) == 0)
	{
		printfS("%02x ", DevAddress);
		for (i = 0; i < Size; i++)
		{
			/*接收到应答，返回0*/
			if (IIC_Write_Byte(pData[i]) != 0) 		
			{
				delay_us(8);
				printf("NACK\r\n");
				IIC_Stop();
				while(1);
				return 1;
			}
			printfS("%02x ", pData[i]);
		}
		
		/* 成功发送完指定的数据,发送停止信号 */
		delay_us(8);
		IIC_Stop();
		printf("\r\n");
		return	0;
	}	
	IIC_Stop();
	printf("error\r\n");
	while(1);
	return	1;
}


/************************************************************************
 * function   : IIC_Transmit
 * Description: 写入多个字节
 * input : uint8_t DevAddress
 *         uint8_t* order: 写入地址和命令
 *         uint8_t Num: 要写入的个数
 * return: none
 ************************************************************************/
uint8_t IIC_S_Transmit(uint8_t DevAddress, uint8_t *pData, uint16_t Size)
{
	uint8_t i;
	//printf("\r\nSend: ");
	IIC_Start();
	
	//printfS("%02x ", DevAddress);
	if (IIC_Write_Byte(DevAddress) == 0)
	{
		for (i = 0; i < Size; i++)
		{
			if(i == 7)
			{
				if (IIC_Write_Byte_flag(pData[i], 1) != 0) 		
				{
					delay_us(8);
					printf("NACK\r\n");
					IIC_Stop();
					while(1);
					return 1;
				}
			}
			/*接收到应答，返回0*/
			if (IIC_Write_Byte(pData[i]) != 0) 		
			{
				delay_us(8);
				printf("NACK\r\n");
				IIC_Stop();
				while(1);
				return 1;
			}
		//	printfS("%02x ", pData[i]);
		//	printfS("%d ", pData[i]);
		}
		
		/* 成功发送完指定的数据,发送停止信号 */
		delay_us(8);
		IIC_Stop();
		printf("\r\n");
		return	0;
	}	
	IIC_Stop();
	printf("error\r\n");
	while(1);
	return	1;
}


/************************************************************************
 * function   : IIC_Transmit
 * Description: 写入命令并读取数据
 * input : uint8_t DevAddress	设备的地址
 *         uint8_t *pWData      写入的命令
 *         uint16_t wSize       命令的长度
 *         uint8_t *pRData      读出的数据的存储区
 *         uint16_t rSize       读出的长度
 *         uint8_t  Delay_flag  跳过停止信号标志位          
 * return: none
 ************************************************************************/
uint8_t IIC_Receive(uint8_t DevAddress, uint8_t *pWData, uint16_t wSize, uint8_t *pRData, uint16_t rSize, uint8_t Delay_flag)
{
	uint8_t i;
	IIC_Start();
//	printf("\r\nSend: ");
	if (IIC_Write_Byte(DevAddress) == 0)			//发送设备地址
	{
		printfS("%02x ", DevAddress);
		for (i = 0; i < wSize; i++)
		{
			if (IIC_Write_Byte(pWData[i]) != 0) return 1;			//发送读取的寄存器
			printfS("%02x ", pWData[i]);
			//printfS("%c ", pWData[i]);
		}
		
		
		/* 寄存器地址写入成功，准备读取数据 */
		delay_us(500);
		IIC_Start();
	//	printf("\r\nReceive: ");
		IIC_Read(DevAddress, pRData, rSize);
		
		if (Delay_flag == 1)
		{
			
			printf("\r\n");
			return	0;
		}
		IIC_Stop();
	//	printf("\r\n");
		return	0;
	}	
	

	/* 读取失败 */
	IIC_Stop();
	return	1;
}



