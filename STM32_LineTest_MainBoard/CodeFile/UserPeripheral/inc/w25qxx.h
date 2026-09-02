#ifndef __W25QXX_H
#define __W25QXX_H

#include "sys.h"
//#include "lcd.h"



extern u8 Receive_SN[0xA6];

typedef struct
{
    u8 TypeCode[2];
    u8 DataLength[2];
    u8 FlashStartAddr[4];
    u8 SN_total[2];
    u8 FlashEndAddr[4];
} TypeInfrom;

typedef struct
{
		TypeInfrom SN_W1108A;
		TypeInfrom SN_W1109A;
} FlashTable_Type;

enum TypeEnmu
{
	NPG83_K_AS_71K       =     0x0000,
	NPG83_C_AS_60K       =     0x0001,
	NPG83_M_AS_60K       =     0x0002,
	NPG83_Y_AS_60K       =     0x0003,
	NPG83L_C_AS_26K      =     0x0004,
	NPG83L_M_AS_26K      =     0x0005,
	NPG83L_Y_AS_26K      =     0x0006,
	GPR61_BK_US_71K      =     0x0007,
	GPR61_C_US_60K       =     0x0008,
	GPR61_M_US_60K       =     0x0009,
	GPR61_Y_US_60K       =     0x000A,
	GPR61L_C_US_26K      =     0x000B,
	GPR61L_M_US_26K      =     0x000C,
	GPR61L_Y_US_26K      =     0x000D,
	CEXV58_K_EU_71K      =     0x000E,
	CEXV58_C_EU_60K      =     0x000F,
	CEXV58_M_EU_60K      =     0x0010,
	CEXV58_Y_EU_60K      =     0x0011,
	CEXV58L_C_EU_26K     =     0x0012,
	CEXV58L_M_EU_26K     =     0x0013,
	CEXV58L_Y_EU_26K     =     0x0014,
	NPG85_K_AS_71_5K     =     0x0015,
	GPR63_K_US_71_5K     =     0x0016,
	CEXV61_K_EU_71_5K    =     0x0017,


	CEXV65_BK_EU_17_5K   =     0x0018,
	CEXV65_C_EU_11K      =     0x0019,
	CEXV65_M_EU_11K      =     0x001A,	
	CEXV65_Y_EU_11K      =     0x001B,	

	CEXV64_BK_EU_38K     =     0x001C,	
	CEXV64_C_EU_25_5K    =     0x001D,	
	CEXV64_M_EU_25_5K    =     0x001E,	
	CEXV64_Y_EU_25_5K    =     0x001F,	
	CEXV66_BK_EU_44_5K   =     0x0020,


	GPR67_K_US_44_5K     =     0x0021,
	GPR66_BK_US_38K      =     0x0022,
	GPR66_C_US_25_5K     =     0x0023,
	GPR66_M_US_25_5K     =     0x0024,
	GPR66_Y_US_25_5K     =     0x0025,
	
	NPG83_DRUM_K_AS_477K =   	 0x0026,
	NPG83_DRUM_C_AS_404K =     0x0027,
	NPG83_DRUM_M_AS_404K = 		 0x0028,
	NPG83_DRUM_Y_AS_404K = 		 0x0029,
	NPG85_DRUM_K_AS_480K =     0x002A,
	
/*********9.27新增***************/
  CEXV67_K_EU_33K,             
  GPR66L_C_US_11K,             
  GPR66L_M_US_11K,             
  GPR66L_Y_US_11K,             
  NPG88_K_AS_38K,             
  NPG88_C_AS_25_5K,             
  NPG88_M_AS_25_5K,             
  NPG88_Y_AS_25_5K,             
  NPG88L_K_AS,             
  NPG88L_C_AS_4K,             
  NPG88L_M_AS_4K,             
  NPG88L_Y_AS_4K,             
  NPG89_K_AS_44_5K,             
  NPG90L_K_AS_6_5K,             
  NPG90_K_AS_33K,
  
	//20240719 add
  XT88K_U, 
  XT88C_U,
  XT88M_U,
  XT88Y_U,
  XT89K_U,
	
	//241226新增
	_3001K,
	_3001C,
	_3001M,
	_3001Y,
	
	CEXV1002,//20250908 add
	
	//251219新增
	CEXV1001_K_EU_63K,
	CEXV1001_C_EU_54_5K,
	CEXV1001_M_EU_54_5K,
	CEXV1001_Y_EU_54_5K,
	CEXV1001L_C_EU_24K,
	CEXV1001L_M_EU_24K,
	CEXV1001L_Y_EU_24K,
	
	//260227新增
	NPG1001_K_AS_63K,
	NPG1001_C_AS_54_5K,
	NPG1001_M_AS_54_5K,
	NPG1001_Y_AS_54_5K,
	
	
/************

***********/
};



#define W25Q80  0XEF13
#define W25Q16  0XEF14
#define W25Q32  0XEF15
#define W25Q64  0XEF16
#define W25Q128 0XEF17

extern u16 W25QXX_TYPE;                 //定义W25QXX芯片型号

#define W25QXX_CS       PBout(12)       //W25QXX的片选信号

////////////////////////////////////////////////////////////////////////////

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

void W25QXX_Init(void);
u16  W25QXX_ReadID(void);               //读取FLASH ID
u8   W25QXX_ReadSR(void);               //读取状态寄存器
void W25QXX_Write_SR(u8 sr);            //写状态寄存器
void W25QXX_Write_Enable(void);         //写使能
void W25QXX_Write_Disable(void);        //写保护
void W25QXX_Write_NoCheck(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void W25QXX_Read(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead); //读取flash
void W25QXX_Write(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite); //写入flash
void W25QXX_Erase_Chip(void);           //整片擦除
void W25QXX_Erase_Sector(u32 Dst_Addr); //扇区擦除
void W25QXX_Wait_Busy(void);            //等待空闲
void W25QXX_PowerDown(void);            //进入掉电模式
void W25QXX_WAKEUP(void);               //唤醒
u8 read_sn(u8 *sn_point, u16 Type_PrintEnum, u32 sn_num); //读需要读的0xD9个数据
void SPI2_Init(void);            //初始化SPI口
void SPI2_SetSpeed(u8 SpeedSet); //设置SPI速度
u8 SPI2_ReadWriteByte(u8 TxData);//SPI总线读写一个字节
//u8 read_sn(u8 *sn_point, u16 Type_PrintEnum, u16 sn_num);
void LCD_show_versions(void);
u16 get_crc_805C(u8* pData, u32 nLength);
u16 CRC_check(u8 *point);
u16 Read_data(void);
void Get_TableHead(void);
u8 Get_SN_number(TypeInfrom TypeSel, u32 SN_Count, u8 ReadNum);
u8 Check_data(u8 * pData);
void SPI_TEST_Burn(void);
#endif
















