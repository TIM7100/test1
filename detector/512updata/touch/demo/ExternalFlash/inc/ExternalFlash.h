#ifndef __EXTERNAL_FLASH_H__
#define __EXTERNAL_FLASH_H__

#include "ach512.h"
#include "XxySpi.h"


#define W25Q_SUCC		0x00
#define W25Q_FAIL		0x01
#define W25Q_TIMEOUT 	500	





//W25X系列/Q系列芯片列表	   
//W25Q80  ID  0XEF13
//W25Q16  ID  0XEF14
//W25Q32  ID  0XEF15
//W25Q64  ID  0XEF16	
//W25Q128 ID  0XEF17	
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17

//指令表
#define W25X_WriteEnable        0x06
#define W25X_WriteDisable       0x04
#define W25X_ReadStatusReg      0x05
#define W25X_WriteStatusReg     0x01
#define W25X_ReadData           0x03
#define W25X_FastReadData       0x0B
#define W25X_FastReadDual       0x3B
#define W25X_PageProgram        0x02
#define W25X_BlockErase         0xD8
#define W25X_SectorErase        0x20
#define W25X_ChipErase          0xC7
#define W25X_PowerDown          0xB9
#define W25X_ReleasePowerDown   0xAB
#define W25X_DeviceID           0xAB
#define W25X_ManufactDeviceID   0x90
#define W25X_JedecDeviceID      0x9F

#define EXFLASH_MAX_LEN					0x800000		//暂定8MB
#define	EXFLASH_ADDR_START			0x00
#define	EXFLASH_SECTOR_SIZE			4096
#define EXFLASH_BLOCK_SIZE			65536
#define EXFLASH_PAGE_SIZE				256
#define SPI_W25Q								SPIA

//初始化SPI FLASH的IO口
void ExFlashInit(void);

//读取芯片ID
u16 ExFlashReadID(void);

//读取SPI FLASH
//在指定地址开始读取指定长度的数据
//pBuffer:数据存储区
//ReadAddr:开始读取的地址(24bit)
//NumByteToRead:要读取的字节数(最大65535)
void ExFlashRead(u8* pBuffer, u32 ReadAddr, u32 NumByteToRead);

//W25QXX写使能
void ExFlashWriteEnable(void);

//W25QXX写禁止
void ExFlashWriteDisable(void);

//读取W25QXX的状态寄存器
u8 ExFlashReadSR(void);

//等待空闲
u8 ExFlashWaitBusy(void);

//擦除一个扇区
u8 ExFlashEraseSector(u32 Dst_Addr);

//擦除一个块
u8 ExFlashEraseBlock(u32 Dst_Addr);

//擦除整个芯片
u8 ExFlashEraseChip(void);

//SPI在一页(0~65535)内写入少于256个字节的数据
u8 ExFlashWritePage(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);

//无检验写SPI FLASH
u8 ExFlashWriteNoCheck(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);

//写SPI FLASH
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
u8 ExFlashWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);

#endif
