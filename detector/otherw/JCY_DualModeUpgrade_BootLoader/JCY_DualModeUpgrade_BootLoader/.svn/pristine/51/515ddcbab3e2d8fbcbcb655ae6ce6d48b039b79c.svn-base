#include "firmware_download.h"
#include "show.h"
#include "bsp_sdcard.h"
#include "bsp_eflash.h"


#define SD_WriteCount(x)		  (x / 512 + 1)	
#define RXBUFFSIZE      		  0x1000

/** 用于接收待更新固件的缓存区 **/
Type_UART_t UARTDataBuff;
static uint8_t RXFirmwareBuff[RXBUFFSIZE] = {0};





void Printf_HexData(u8 *Buffer, uint32_t Size)
{
	uint32_t i;
	for(i = 0; i < Size; i++)
	{
		printfS("%c", Buffer[i]);
	}
	printfS("\r\n");
}


/**********************************************  uart更新固件函数 **********************************************/
/**
 * @brief 使用uart更新固件，该方法需要占用非常大RAM空间，请确保RAM空间足够
 * @param xxx:xxx
 * @return 
 ***/
u8 UartUpdataFirmware(uint32_t UpdateSector)
{
	u8 Flag = 0;
	
	/* 清除接收缓存区 */
	memset(RXFirmwareBuff, 0, RXBUFFSIZE);
	
	/* 更新Uart接收参数 */
	UART1_ParaInit(&UARTDataBuff, RXFirmwareBuff, RXBUFFSIZE);
	
	
	/* 等待中断接收数据到缓存区，200ms没接收到数据，则表示数据接收成功 */
	UART1_WaitRxDataBeForeTimeOut(&UARTDataBuff);
	
	
//	/* 打印接收到的数据 */
//	Printf_HexData(RXFirmwareBuff, UARTDataBuff.RxLength);
	
	/* 需要把缓存区的数据写入SD卡 */
	if(UARTDataBuff.RxLength > 0)
	{
		printfS("Write\r\n");
		
		Flag = SD_WriteDisk(RXFirmwareBuff, UpdateSector, SD_WriteCount(UARTDataBuff.RxLength));
		if(Flag != SD_DATA_OK)
		{
			printfS("Write SD Error %X\r\n",Flag);
			return UpdataError;
		}	
	}
	return UpdataSuccess;
	
//	/* 测试用，读取SD刚写入的数据 */
//	SD_ReadDisk(Read_Flash_Buff, DownloadSector, SD_WriteCount(UARTDataBuff.RxLength));		//读512字节，一个数据包
//	Printf_HexData(Read_Flash_Buff, UARTDataBuff.RxLength);
}


	