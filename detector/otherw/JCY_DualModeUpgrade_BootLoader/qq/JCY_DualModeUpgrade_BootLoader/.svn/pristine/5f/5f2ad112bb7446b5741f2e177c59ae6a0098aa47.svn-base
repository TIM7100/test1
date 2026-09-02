/***************************************************************************************************
 *                               读取SD卡数据并进行加解密后，再放入SD卡                               *
****************************************************************************************************/
#include "handle_firmware.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bsp_sdcard.h"
#include "bsp_eflash.h"
#include "w5500_user_conf.h"
#include "menu_show.h"
#include "crc.h"
#include "my_aes_cbc.h"

extern FIRMWARE_INFO_t LocalFirmwareInfo;                    //存放本地的固件消息，包括固件版本和支持的系列
static MODULE_ADDRESS_t ModuleAddr = {0};                    //存放了每个APP起始地址和结束地址的集合

u8 AppNum = 0;          //记录总的App数量（去除menu）
u8 ReplaceVerCount = 0;	//记录当前实际获取到的系列数
u8 MenuFlag = 0;        //记录当前是否获取了menu表头信息

/***********************************************************************************************
 *                                 执行IAP跳转的必要参数                                        *
************************************************************************************************/
typedef  void (*IAPFun)(void);                              //定义一个函数类型的参数.
typedef __IO u32  vu32;
IAPFun JumpToMenu;


enum
{
    HANDLE_SUCCESS = 0,
//  Padding_ERR = 1,
//  SET_KEY_ERR = 2,
//  ENC_CBC_ERR,
//  DEC_CBC_ERR,
    HEAD_CRC_ERR
};


static void InstallData(u32 HandleCount);
static u8 Head_Handle(u8 *HeadBuff, u8* HandleSeriesCount, u8 HandleNum);


/**********************************************************************
 * @brief   获取存储在SD卡中的本地固件包信息，
 *          从SD卡更新参数LocalFirmwareInfo，用于存放本地固件包信息的集合体
 * @param   无
 * @return  SUCCESS: 读取SD卡成功
            ERROR: 读取SD卡失败
************************************************************************/
u8 GetLocalFirmwareInfo(void)
{
    u8 i = 0;
    u8 IV[16] = {0};

//    SD_Initialize();
    memset(&LocalFirmwareInfo, 0, 1024);
    if (SD_ReadDisk((u8 *)&LocalFirmwareInfo, LOCAL_VERSION_SECTOR, sizeof(LocalFirmwareInfo) / 512) != SD_DATA_OK)
    {
        printfS("Get Current Version Error\r\n");
        return ERROR;
    }

    //PrintfHex(&LocalFirmwareInfo, sizeof(LocalFirmwareInfo));
    if (LocalFirmwareInfo.KeyFlag != 0x3435)
    {
        DecryptDataByAesCBC((u8 *)&LocalFirmwareInfo, 1024, (u8 *)&LocalFirmwareInfo, IV, SV_KEY);          //解密
        //新机开始时，加密解密时都没有这个标志位，因此需要进行一次出厂设置
        if (LocalFirmwareInfo.KeyFlag != 0x3435)
        {
            FactoryReset();
        }
    }
    printfS("User Flash Area: %d\r\n", LocalFirmwareInfo.FirmwareAreaFlag);
    printfS("Current Series：%d\r\n", LocalFirmwareInfo.SeriesCount);
//  for (i = 0; i < LocalFirmwareInfo.SeriesCount; i++)
//  {
//      printfS("%d.", i);
//      if (i < FIRST_BUFF_SERIES_COUNT)
//      {
//          printfS("Series Name:%s; Series Version:", LocalFirmwareInfo.SeriesBuff[i].SeriesName);
//          PrintfString(LocalFirmwareInfo.SeriesBuff[i].SeriesVersion, 4);
//      }
//      else
//      {
//          printfS("Series Name:%s; Series Version:", LocalFirmwareInfo.SeriesBuff2[i - FIRST_BUFF_SERIES_COUNT].SeriesName);
//          PrintfString(LocalFirmwareInfo.SeriesBuff2[i - FIRST_BUFF_SERIES_COUNT].SeriesVersion, 4);
//      }
//  }
    return SUCCESS;
}

/**********************************************************************
 * @brief   更新固件包下载状态
 * @param   State: 0: 无固件包在下载；
 *                 1：固件包在下载
 *                 2：进行固件包解密-处理-加密操作
 * @return  SUCCESS: 读取SD卡成功
            ERROR: 读取SD卡失败
************************************************************************/
void UpdateFirmwareDownloadState(u8 State)
{
    u8 i = 0;
    u8 ModuleIV[16] = {0};                      //对固件包中各个模块单独加密的IV值
    u8 EncryptData[512];
    u32 InfoWriteArea = 0;

    if (LocalFirmwareInfo.FirmwareState != State)
    {
        LocalFirmwareInfo.FirmwareState = State;
        HandleLocalFirmwareInfo();
    }
}

u8 GetFirmwareDownloadState(void)
{
    return LocalFirmwareInfo.FirmwareState;
}

void ChangeFirmwareSaveArea(void)
{
    if (LocalFirmwareInfo.FirmwareAreaFlag == 1)
    {
        LocalFirmwareInfo.FirmwareAreaFlag = 2;
    }
    else
    {
        LocalFirmwareInfo.FirmwareAreaFlag = 1;
    }
}
/*******************************************************
 * @brief    处理本机存储的固件信息
 * @param    无
 * @return   无
********************************************************/
void HandleLocalFirmwareInfo(void)
{
    u8 i = 0;
    u8 ModuleIV[16] = {0};                      //对固件包中各个模块单独加密的IV值
    u16 CrcValue = 0;
    u8 EncryptData[512];
    u8 SectorCount = 1;


    /* 本地固件信息处理 0x0 -- 0x400 */
    memset(ModuleIV, 0, 16);

    CrcValue = GetCrc16((char *)&LocalFirmwareInfo, 510);
    LocalFirmwareInfo.Crc1[0] = CrcValue >> 8;
    LocalFirmwareInfo.Crc1[1] = (u8)CrcValue;
    CrcValue = GetCrc16((char *)&LocalFirmwareInfo + 512, 510);
    LocalFirmwareInfo.Crc2[0] = CrcValue >> 8;
    LocalFirmwareInfo.Crc2[1] = (u8)CrcValue;

//  SD_Initialize();
    /* 对本地Firmware信息进行加密后，放进SD卡 */
    for (i = 0; i < 2; i++)
    {
        EncryptDataByAesCBC((u8*)&LocalFirmwareInfo + (i * 512), ONCE_HANDLE_SIZE, EncryptData, ModuleIV);     //进行加密
        SD_WriteDisk(EncryptData, i, SectorCount);                                     //再写入SD卡
    }
}

//void Test_SD(void)
//{
//    __IO u8 EncryptData[ONCE_HANDLE_SIZE] = {0};
//    __IO u32 ReadSector = 0;
//
//    ReadSector = (WRITE_SD_START_ADDR) / 512;
//    SD_ReadDisk(EncryptData, ReadSector, 1);
//
//    PrintfHex(EncryptData, 512);
//}

/*********************************************************************
 * @brief    处理云平台下载的固件包数据
 * @param    无
 * @return   FIRMWARE_CRC_ERR  固件包整体CRC校验出错，需要重新下载固件包
 *           WriteSector       处理完成的固件包占用SD卡的扇区数量
**********************************************************************/
u32 HandleFirmwareData(void)
{
    u8 EncryptData[ONCE_HANDLE_SIZE] = {0};
    u8 DecryptData[ONCE_HANDLE_SIZE] = {0};
    __IO u32 ReadSector = 0;
    u32 WriteStartAddress = FIEMWARE_HANDLE_AREA(LocalFirmwareInfo.FirmwareAreaFlag);
    u32 WriteSector =  WriteStartAddress /  512;    //固件包存放在SD卡的起始扇区地址
    __IO u8 SectorCount = ONCE_HANDLE_SIZE / 512;       //单次读取SD卡扇区的数量

    u32 i = 0;
    __IO u32 HandleCount = 0;
    u16 CrcValue = 0;
    u8 FirmwareIV[16] = {0};                        //固件包整体解密的IV值
    u8 ModuleIV[16] = {0};                          //对固件包中各个模块单独加密的IV值
    __IO u32 PointFirmware = 0;
    __IO u8 HandleSeriesCount = 0;
    u8 CopyData[2048];

    printfS("Handle Firmware Data\r\n");
    HandleCount = LocalFirmwareInfo.FirmwareSize / ONCE_HANDLE_SIZE;
    if (LocalFirmwareInfo.FirmwareSize % ONCE_HANDLE_SIZE)              //计算待处理的数据在SD卡中占用的扇区数
    {
        HandleCount += 1;
    }
    printfS("Head Address :%08X\r\n", WriteSector * 512);
    /* 分段读取下载的加密固件包的数据，进行数据处理后，再分模块进行加密后放入固件备份区 */
    for (i = 0; i < HandleCount; i++)
    {
        ProgressBarShow(HandleCount, i);                                         //加载进度条
        ReadSector = (u32)((WRITE_SD_START_ADDR + i * (ONCE_HANDLE_SIZE)) / 512);        //读取SD中的地址
        WriteSector += ONCE_HANDLE_SIZE / 512;                                   //数据处理完成重新加密后，写入SD卡的位置
        PointFirmware = i * ONCE_HANDLE_SIZE;                                    //指向加密固件包的相对处理地址

        /* 读取ONCE_HANDLE_SIZE字节的存放在SD卡的固件包数据, 进行解密后再放入SD卡 */
        if (SD_ReadDisk(EncryptData, ReadSector, SectorCount) != SD_DATA_OK)
        {
            return OTA_WRITE_ERROR;
        }
        DecryptDataByAesCBC(EncryptData, ONCE_HANDLE_SIZE, DecryptData, FirmwareIV, APP_KEY);       //对读出的数据进行解密
        /* 整体CRC校验 */
        if (i == (HandleCount - 1))
        {
            CrcValue = GetTolCrc16(~CrcValue, (char *)DecryptData, ModuleAddr.AppAddr[ModuleAddr.AppCount - 1].End - PointFirmware);
            if (CrcValue != (DecryptData[ModuleAddr.AppAddr[ModuleAddr.AppCount - 1].End - PointFirmware] << 8 | DecryptData[ModuleAddr.AppAddr[ModuleAddr.AppCount - 1].End - PointFirmware + 1]))
            {
                printfS("Crc: %X%X\r\n", CrcValue >> 8, CrcValue & 0xFF);
                printfS("Source Crc: %X%X\r\n", DecryptData[ModuleAddr.AppAddr[ModuleAddr.AppCount - 1].End - PointFirmware], DecryptData[ModuleAddr.AppAddr[ModuleAddr.AppCount - 1].End - PointFirmware + 1]);
                return OTA_FIRM_CRC_ERR;
            }
        }
        else
        {
            CrcValue = GetTolCrc16(~CrcValue, (char *)DecryptData, ONCE_HANDLE_SIZE);
        }
        /* 表头处理 */
        if (PointFirmware < SD_HEAD_SIZE)
        {
            if (Head_Handle(DecryptData, (u8 *)&HandleSeriesCount, i) == HEAD_CRC_ERR)      //处理表头数据，并验证单个表头数据CRC是否正确
            {
                return OTA_FIRM_CRC_ERR;
            }
            EncryptDataByAesCBC(DecryptData, ONCE_HANDLE_SIZE, EncryptData, ModuleIV);      //对读出的数据进行加密
            SD_WriteDisk(EncryptData, WriteSector - 1, SectorCount);                        //加密后的数据, 再写入SD卡
            continue;
        }
        //MenuFlag = 0;
				AppNum = 0;         
				ReplaceVerCount = 0;
				MenuFlag = 0;       
				
        /* Menu处理 */
        if (PointFirmware < ModuleAddr.AppAddr[0].Start)
        {
            /* MENU文件, 需要进行IV处理, 指定写入的扇区位置（Menu的存储位置的偏移值为 SD_MENU_ADDRESS -- 0x1400） */
            if (PointFirmware == ModuleAddr.MenuAddr.Start)
            {
                //计算待处理的数据在SD卡中占用的扇区数
                HandleCount = ModuleAddr.AppAddr[ModuleAddr.AppCount - 1].End / ONCE_HANDLE_SIZE;     //重新计算待下载的次数
                if (ModuleAddr.AppAddr[ModuleAddr.AppCount - 1].End % ONCE_HANDLE_SIZE)
                {
                    HandleCount += 1;
                }
                memset(ModuleIV, 0, 16);
                //重新指向MENU的写入地址
                WriteSector = (WriteStartAddress + SD_HEAD_SIZE) / 512;
                printfS("Menu Address :%08X\r\n", WriteSector * 512);
            }
            EncryptDataByAesCBC(DecryptData, ONCE_HANDLE_SIZE, EncryptData, ModuleIV);      //对读出的数据进行加密
            SD_WriteDisk(EncryptData, WriteSector, SectorCount);                            //加密后的数据, 再写入SD卡
            continue;
        }
        /* APP处理 */
        if (HandleSeriesCount <= ModuleAddr.AppCount)
        {
            /* 每个新的APP文件, 都需要进行IV处理, 指定写入的扇区位置(APP的存储区的偏移值为SD_APP_START_ADDR -- 0x11400)
            （每个APP文件的存储区大小为 SD_SINGLE_APP_SIZE -- 0x57800） */
            if (PointFirmware == ModuleAddr.AppAddr[HandleSeriesCount].Start)
            {
                //重新指向每个APP的起始地址
                WriteSector = (WriteStartAddress + SD_HEAD_SIZE + SD_MENU_SIZE + (SD_SINGLE_APP_SIZE * HandleSeriesCount)) / 512;
                printfS("APP Address :%08X\r\n", WriteSector * 512);
                memset(ModuleIV, 0, 16);
            }
            if (PointFirmware < ModuleAddr.AppAddr[HandleSeriesCount].End)
            {
                if (PointFirmware >= (ModuleAddr.AppAddr[HandleSeriesCount].End - 512))
                {
                    HandleSeriesCount += 1;
                }
                EncryptDataByAesCBC(DecryptData, ONCE_HANDLE_SIZE, EncryptData, ModuleIV);      //对读出的数据进行加密
                SD_WriteDisk(EncryptData, WriteSector, SectorCount);                            //加密后的数据, 再写入SD卡
                continue;
            }
        }
    }
    return (WriteSector - SD_APP_END_ADDR / 512 + 1);
}


/************************************************************************************************************
 * @brief    固件包表头数据处理函数, 通过处理下载的固件包中的表头数据
 *           获得固件包中 bin文件的系列名、版本和存放地址
 * @param    HeadBuff: 待处理的数据指针
 *           HandleSeriesCount: 当前固件包中待处理的bin文件表头的数量
 *           HandleNum: 当前处理SD中，表头数据的第num块, 为0时, 需要读取前32Byte中, 存放的bin文件数量数
 * @return   表头CRC校验成功
 *           HEAD_CRC_ERR  表头CRC校验出错
************************************************************************************************************/
static u8 Head_Handle(u8 *HeadBuff, u8* HandleSeriesCount, u8 HandleNum)
{
    u8 i = 0;
    u16 BuffLen = ONCE_HANDLE_SIZE;
    u16 CrcValue = 0xffff;
    SERIES_HEAD_t SeriesInfo[16] = {0};

    u8 *VersionPoint = NULL;

    if (HandleNum == 0)                             //处理第0块时，需要拿出头部包含系列数的数量
    {
        *HandleSeriesCount = *HeadBuff;             //获取接收的固件包中头部记录的数量(包含Menu)
        ModuleAddr.AppCount = (*HeadBuff) - 1;      //获取接收的固件包中头部记录的APP的数量
        HeadBuff += 32;
        BuffLen -= 32;
    }
    if (*HandleSeriesCount > 0)
    {
        memcpy((u8*)SeriesInfo, HeadBuff, BuffLen);
        for (i = 0; i < BuffLen / 32; i++)
        {
            if (*HandleSeriesCount == 0)
            {
                MenuFlag = 0;
                break;
            }
            /* 校验CRC、获取系列版本号、拿取系列对应的bin地址 */
            CrcValue = GetCrc16((char *)&SeriesInfo[i], 28);
            if (((SeriesInfo[i].Crc[0] << 8) | SeriesInfo[i].Crc[1]) != CrcValue)
            {
                printfS("Head Crc Err\r\n");
                return HEAD_CRC_ERR;
            }
            /* 处理表头数据，需要划分成菜单表头和APP表头 */
            if ((MenuFlag == 0) && (strncmp((const char*)SeriesInfo[i].SeriesName, "MENU", 4) == 0))//(const char*)LocalFirmwareInfo.SeriesBuff[0].SeriesName) == 0))
            {
                //处理菜单表头，获取Menu代码块在固件包中对应的地址和版本号
                MenuFlag = 1;
                strncpy(LocalFirmwareInfo.SeriesBuff[0].SeriesVersion, (const char*)SeriesInfo[i].Version, SERIES_VERSION_SIZE);
                ModuleAddr.MenuAddr.Start = SeriesInfo[i].StartAddress;
                ModuleAddr.MenuAddr.End = SeriesInfo[i].EndAddress + 2;
                SeriesInfo[i].StartAddress += SD_START_ADDR;                                 //计算放入SD卡时，Menu代码块的存储地址
                SeriesInfo[i].EndAddress += SD_START_ADDR;

                CrcValue = GetCrc16((char *)&SeriesInfo[i], 28);                            //重新计算CRC值
                SeriesInfo[i].Crc[0] = CrcValue >> 8;
                SeriesInfo[i].Crc[1] = CrcValue;


                printfS("Menu Address:0x%X -- 0x%X\r\n", SeriesInfo[i].StartAddress, SeriesInfo[i].EndAddress);
            }
            else if (MenuFlag)
            {
                //处理APP表头，获取APP代码块在固件包中的位置信息
                ModuleAddr.AppAddr[AppNum].Start = SeriesInfo[i].StartAddress;
                ModuleAddr.AppAddr[AppNum].End = SeriesInfo[i].EndAddress + 2;
                SeriesInfo[i].EndAddress -= SeriesInfo[i].StartAddress;
                SeriesInfo[i].StartAddress = SD_APP_START_ADDR + SD_SINGLE_APP_SIZE * AppNum;    //计算放入SD卡时，APP代码块的存储地址
                SeriesInfo[i].EndAddress += SeriesInfo[i].StartAddress;
                CrcValue = GetCrc16((char *)&SeriesInfo[i], 28);                                //重新计算CRC值
                SeriesInfo[i].Crc[0] = CrcValue >> 8;
                SeriesInfo[i].Crc[1] = CrcValue;
                PrintfHex(&SeriesInfo[i], 32);
                printfS("APP %d Address:0x%X -- 0x%X\r\n", AppNum, SeriesInfo[i].StartAddress, SeriesInfo[i].EndAddress);
                AppNum++;                   //当前已经处理的表头数量
            }
            (*HandleSeriesCount)--;
        }
        memcpy(HeadBuff, (u8 *)SeriesInfo, BuffLen);                        //将原来的APP代码地址替换成重新处理好的APP代码地址

        /* 遍历本地支持的系列, 并更新支持的系列 */
        for (i = 0; i < LocalFirmwareInfo.SeriesCount; i++)
        {
            if (AppNum == (ReplaceVerCount - 1))
            {
                break;
            }
            if (i < FIRST_BUFF_SERIES_COUNT)
            {
                VersionPoint = HeadBuff;
                while ((VersionPoint = memchr(VersionPoint, LocalFirmwareInfo.SeriesBuff[i].SeriesName[0], BuffLen)) != NULL)
                {
                    if (strcmp((const char*)LocalFirmwareInfo.SeriesBuff[i].SeriesName, (const char*)VersionPoint) == 0)                         //找到对应的系列名
                    {
                        strncpy(LocalFirmwareInfo.SeriesBuff[i].SeriesVersion, (const char*)(VersionPoint - SERIES_VERSION_SIZE), SERIES_VERSION_SIZE);
                        ReplaceVerCount++;
                        break;
                    }
                    VersionPoint += 1;//strlen((const char*)VersionPoint);
                }

            }
            else if (i >= FIRST_BUFF_SERIES_COUNT)
            {
                VersionPoint = HeadBuff;
                while ((VersionPoint = memchr(VersionPoint, LocalFirmwareInfo.SeriesBuff2[i - FIRST_BUFF_SERIES_COUNT].SeriesName[0], BuffLen)) != NULL)
                {
                    if (strcmp((const char*)LocalFirmwareInfo.SeriesBuff2[i - FIRST_BUFF_SERIES_COUNT].SeriesName, (const char*)VersionPoint) == 0)                      //找到对应的系列名
                    {
                        strncpy(LocalFirmwareInfo.SeriesBuff2[i - FIRST_BUFF_SERIES_COUNT].SeriesVersion, (const char*)(VersionPoint - SERIES_VERSION_SIZE), SERIES_VERSION_SIZE);
                        ReplaceVerCount++;
                        break;
                    }
                    VersionPoint += 1;//VersionPoint += strlen((const char*)VersionPoint);
                }
            }
            if (ReplaceVerCount == LocalFirmwareInfo.SeriesCount)
            {
                printfS("APP Version Updata SUCCESS：%d\r\n", ReplaceVerCount);
                break;
            }
        }
    }
    return HANDLE_SUCCESS;
}



/*******************************************************
 * @brief    加载SD卡的数据到指定的芯片Flash区
 * @param    xxx:xxx
 * @return   ERROR: SD卡读取失败
             SUCCESS: 读取成功，并跳转
********************************************************/
u8 LoadCodeIntoFlash(void)
{
    u16 i;
    u8 SourceBuf[512] = {0};
    u8 DecryptData[512] = {0};
    u32 Size = 0;
    u16 LoadCount = 0;                                       //加载代码的次数
    __IO u32 SDReadSector = SD_MENU_ADDRESS(LocalFirmwareInfo.FirmwareAreaFlag) / 512;
    u32 LoadAddrToFlash = FLASH_MENU_ADDRESS;
    u8  IV[16] = {0};

    printfS("%08lX\r\n", SD_MENU_ADDRESS(LocalFirmwareInfo.FirmwareAreaFlag));
    SD_Initialize();
    //判断SD中是否固件包可供跳转
    if ((LocalFirmwareInfo.FirmwareAreaFlag != 1) && (LocalFirmwareInfo.FirmwareAreaFlag != 2))
    {
        return JUMP_FIRMWARE_EMPTY;
    }


    if (Size == 0)
    {
        Size = 28800;
    }
    /* 计算分段下载的次数 */
    LoadCount = Size / 512;
    if (Size % 512)
    {
        LoadCount += 1;
    }
    /* 分段从SD卡加载Menu的代码到Flash */
    for (i = 0; i < LoadCount; i++)
    {
        ProgressBarShow(LoadCount, i);                                 // 显示加载进度条
        /* 每次从SD卡读取512字节写入芯片Flash */
        if (SD_ReadDisk(SourceBuf, SDReadSector, 1) != SD_DATA_OK)
        {
            return JUMP_READ_SD_FAIL;
        }
        DecryptDataByAesCBC(SourceBuf, 512, DecryptData, IV, SV_KEY);      // 对读出的数据进行解密
        PrintfHex(DecryptData, 512);
        EflashEraseArea(LoadAddrToFlash, 512);                         // 擦除待写入的Flash区域
        EflashWritePage(LoadAddrToFlash, 512, DecryptData);              // 写入Menu指定的存取区域
        SDReadSector += 1;                                             // 下次读取SD卡的扇区位置加一
        LoadAddrToFlash += 512;                                        // 写入Flash的地址加512
    }
    ProgressBarShow(LoadCount, LoadCount);                             //进度条填充满

    if (LocalFirmwareInfo.FirmwareState != 0)
    {
        return JUMP_FIRMWARE_NEW;
    }
    return JUMP_SUCCESS;
}


/*******************************************************
 * @brief    跳转到指定的Flash区域，以运行Menu代码
 * @param    xxx:xxx
 * @return
********************************************************/
void JumpMenuCode(void)
{
    /*
     *   跳转之前，需要先关闭所有中断（NMI和硬Fault除外），1为关闭所有中断  *
     *   跳转之后，需要先开启所有中断（NMI和硬Fault除外），0为使能所有中断  *
    */
    __set_PRIMASK(1);
    if (((*(vu32*)FLASH_MENU_ADDRESS) & 0x2FFE0000) == 0x20000000) //检查栈顶地址是否合法.
    {
        printfS("jump\r\n");
        JumpToMenu = (IAPFun) * (vu32*)(FLASH_MENU_ADDRESS + 4);  //用户代码区第二个字为程序开始地址(复位地址),解引用后获得复位函数入口地址
        __set_MSP(*(volatile uint32_t*)FLASH_MENU_ADDRESS);     //初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)

        JumpToMenu();                                   //跳转到APP的复位函数入口地址.
    }
}

