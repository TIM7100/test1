#ifndef _ENC_FIRM_H
#define _ENC_FIRM_H

#include "fxx_std.h"
#include "ota_firmware_updata.h"
/***********************************************************************************************
 *                           固件包加解密处理时，各参数的大小                                  *
************************************************************************************************/
#define ONCE_HANDLE_SIZE     512                                        
#define APP_COUNT            207

#define SD_START_ADDR        0x400												 //表头的起始地址
#define SD_HEAD_SIZE         0x1A00                                              //表头的大小
#define SD_MENU_SIZE		 0xA600                                              //菜单的大小

#define SD_APP_START_ADDR    (SD_MENU_SIZE + SD_HEAD_SIZE + SD_START_ADDR)       //APP数据的起始地址
#define SD_SINGLE_APP_SIZE   0x52800UL						                     //单个APP的存储空间大小
#define SD_APP_END_ADDR      (SD_APP_START_ADDR + SD_SINGLE_APP_SIZE * APP_COUNT)//APP的存储空间的结束地址，为0xC400 + 0x52800 * 207 = 0x42C1C00 `` 66M

#define FIRST_APP_AREA		  SD_START_ADDR
#define SECOND_APP_AREA		  SD_APP_END_ADDR
/***********************************************************************************************
 * 解释： 当 x == 1时，既代表 FirmwareAreaFlag 为1，表示固件包存放在 区域1 -- SD_START_ADDR，
 *					   在这情况下，更新的新固件包需要写入 区域2 -- SD_APP_END_ADDR
 *        当 x == 2时，既代表 FirmwareAreaFlag 为2，表示固件包存放在 区域2 -- SD_APP_END_ADDR，
 *						在这情况下，更新的新固件包需要写入 区域1 --  SD_START_ADDR
 *        当 x == 其他值时，此时表示 SD卡中没有固件包，需要更新固件包，
 *						在这情况下，更新的新固件包需要写入 区域1 --  SD_START_ADDR
************************************************************************************************/
#define FIEMWARE_HANDLE_AREA(x)    ((x == 1) ? (SECOND_APP_AREA) : (FIRST_APP_AREA))  

#define SD_MENU_ADDRESS(x)		   ((x == 1) ? (FIRST_APP_AREA + SD_HEAD_SIZE) : (SECOND_APP_AREA + SD_HEAD_SIZE))               //菜单程序的起始地址

#define FLASH_MENU_ADDRESS   0x23200U						   // Menu程序在Flash的位置： 起始地址 
#define LANGUAGE_ADDRESS     (FLASH_MENU_ADDRESS - 0x200)      //语言标志位在Flash中的存储地址

/*******************************************************************************************************
 * 固件包状态标志位，根据不同的情况，升级固件包时会执行不同的操作
 * 解释： (1)FS_IDLE：空闲中可以下载新固件包；
 *        (2)FS_DOWNLOADING: 下载固件包时，出现意外中断，需要重新下载
 *        (3)FS_ANALYSIS：解密标志。加密固件包已下载完成，需要解密后校验固件包的完整性再加密存放进SD卡
*********************************************************************************************************/
#define FS_IDLE              0
#define FS_DOWNLOADING       1
#define FS_ANALYSIS          2

/***********************************************************************************************
 *                                 本地固件包信息的数据结构体                                    *
************************************************************************************************/
//当前该结构体占用的空间为1024位， 请确保
#pragma pack(1)
typedef struct SERIES_INFO{							    
	char SeriesName[SERIES_NAME_SIZE];
	char SeriesVersion[SERIES_VERSION_SIZE];
}SERIES_INFO_t;

typedef struct FIRMWARE_INFO{
	char DeviceName[DEVICE_NAME_SIZE];				
	char FirmwareVersion[FIRMWARE_VERSION_SIZE];		//记录最近一次接收到的固件版本
	char LocalFirmVersion[FIRMWARE_VERSION_SIZE];       //记录了当前设备使用的固件版本
	u32 SeriesCount;
	SERIES_INFO_t SeriesBuff[39];
	u8 Crc1[2];
	/*---------512Byte--------*/
	SERIES_INFO_t SeriesBuff2[41];
	
	u32 FirmwareSize;				// 本次更新的固件包的大小（除去CRC）
	u16 KeyFlag;					// 是否加密标志位, 若没加密, 则能直接读取到12345
	u8  EmptyBlock[10];
	
	u8  FirmwareState;				// 0:没有在进行下载或更新操作； 1: 进行下载操作  2: 进行固件包解密-处理-加密操作
	u8  FirmwareAreaFlag;			// 0:没有固件包； 1: 固件包在区域1  2: 固件包在区域2
	u8  Crc2[2];
	/*---------1024Byte--------*/
}FIRMWARE_INFO_t;
#pragma pack()


/* 接收的固件包表头构成 */
#pragma pack(1)
typedef struct _SERIES_HEAD_{
	u8 Version[4];
	u8 SeriesName[16];
	u32 StartAddress;
	u32 EndAddress;
	u8 EmptyBlock[2];
	u8 Crc[2];
}SERIES_HEAD_t;
#pragma pack()


#pragma pack(1)
typedef struct _ADDRESS_BUFF
{
	u32 Start;
	u32 End;
}ADDRESS_BUFF_t;
typedef struct _MODULE_ADDRESS_
{
	u8 AppCount;
	ADDRESS_BUFF_t AppAddr[APP_COUNT];
	ADDRESS_BUFF_t MenuAddr;
}MODULE_ADDRESS_t;
#pragma pack()


enum{
	JUMP_SUCCESS = 0,
	JUMP_FIRMWARE_EMPTY = 1,
	JUMP_FIRMWARE_NEW = 2,
	JUMP_READ_SD_FAIL
};

//void Test_SD(void);
u8 GetLocalFirmwareInfo(void);				   //获取本地固件包信息
void UpdateFirmwareDownloadState(u8 State);    //
u8 GetFirmwareDownloadState(void);
void ChangeFirmwareSaveArea(void);
void HandleLocalFirmwareInfo(void); 

u32 HandleFirmwareData(void);
u8 LoadCodeIntoFlash(void);
void JumpMenuCode(void);
#endif 

