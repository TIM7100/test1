#ifndef _OTA_FIRMWARE_HANDLE_H
#define _OTA_FIRMWARE_HANDLE_H

#include "common.h"
#include "ota_firmware_update.h"

#define ONCE_HANDLE_SIZE     (512) 
//内部FLASH 共享数据区的起始地址（包括影子计数，中英文标志位）
#define DATA_AREA_ADDR       (0x2FE00)
//内部FLASH 中英文标志位地址
#define DeviceLanguageAddr   (DATA_AREA_ADDR + 0x10)

//内部FLASH 切换flash标志位地址
#define FLASHSWITCHAddr   (0x00070200)

//内部FLASH APP区域的起始地址
#define APP_START_ADDR       (0x30000)
//W25Q中固件包的存放地址
#define FIRMWARE_AREA_1_START_ADDR   (0x200)
// #define FIRMWARE_AREA_2_START_ADDR   (0x800000+FIRMWARE_AREA_1_START_ADDR)   //0x800000(8M)
#define FIRMWARE_AREA_2_START_ADDR   (0x200) //第二颗flash
#define FIRMWARE_AREA_1              (1U)
#define FIRMWARE_AREA_2              (2U)
#define GET_NEXT_FIRMWARE_WRITE_ADDRESS(flag) ( ( (flag) == 0 || (flag) == 2 ) ? FIRMWARE_AREA_1_START_ADDR : FIRMWARE_AREA_2_START_ADDR )
#define GET_NEXT_FIRMWARE_AREA(flag) ( ( (flag) == 0 || (flag) == 2 ) ? FIRMWARE_AREA_1 : FIRMWARE_AREA_2 )

/***********************************************************************************************
 *                                 本地固件包信息的数据结构体                                    *
************************************************************************************************/
//当前该结构体占用的空间为512位， 请确保
#pragma pack(1)
typedef struct partition_header_info{
	char partition_version[PARTITION_VERSION_SIZE];  //文件名
	u32 partition_flash_start_address;              //w25q中（代码区/数据区）的起始地址
	u32 partition_flash_end_address;				//w25q中（代码区/数据区）的结束地址
}partition_header_info_t;

typedef struct firmware_info{
	partition_header_info_t code;     //代码区分区信息
	partition_header_info_t data;     //数据区分区信息
	u8  FirmwareAreaFlag;			  // 0:没有固件包； 1: 当前可执行固件包在区域1  2: 当前可执行固件包在区域2
	u16 KeyFlag;                      //如果不是3435，则会进行一次恢复出厂设置
	//*********断电重传记录*****************//
	u8  FirmwareDownloadState;		// 0:没有在进行下载或更新操作；1: 进行下载操作 
	u32 FirmwareSize;				// 本次要更新的固件包的大小（https下发的固件包真实大小）
	HTTP_PACKET_t HttpPacketInfo;   // 目前使用在线重传，外部HTTP_PACKET_t替换该结构体内HTTP_PACKET_t，可使用断点重传
	//*************************************//
	char firmware_version[FIRMWARE_VERSION_SIZE];			  //记录最近一次接收到的云端固件版本
	char local_firmware_version[FIRMWARE_VERSION_SIZE];       //记录了当前设备使用的云端固件版本
	u8 reserve[404];                //保留
	u8 crc[2];
}firmware_info_t;
#pragma pack()

typedef  void (*iapfun)(void); 

void IapLoadApp(u32 Addr);
u8 VerifyFirmwareHandle(void);
u8 JumpAppAfterDecrypt(void);
#endif


