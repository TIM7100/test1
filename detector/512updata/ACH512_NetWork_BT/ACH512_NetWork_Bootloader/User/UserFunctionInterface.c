#include "UserFunctionInterface.h"
#include "ota_firmware_update.h"
#include "ota_firmware_handle.h"
#include "cloud_platform_para.h"
#include "ExternalFlash.h"
#include "delay.h"
#include "my_aes_ecb.h"
#include "menu_show.h"
#include "gpio.h"
#include "crc.h"
#include "eflash.h"
#include "w5500_user_conf.h"
#include "use_lcd.h"
#include "esp8266_config.h"
#include "lcd_show.h"


firmware_info_t firmware_info = {0};
__IO u8 NetWork_Flag = ESP8266;
uint32_t read_val;
extern u8 BuzzFlag;
extern u32 LEVEL_cnt;
u8 light ;
 /*通过读取W25Q表头信息判断设备状态，上电初始状态为正常*/
static u8 isDeviceNormalFlag = DEVICE_NORMAL;


/*******************************************************
 * @brief    灵敏度初始化
 * @param    xxx:xxx
 * @return
********************************************************/
void Sensitivity(void)
{ 
	u8 eflashBuffer[512];  
	EflashReadPage(DATA_AREA_ADDR, 512, (u32*)eflashBuffer); 
	if(eflashBuffer[0x80 + 4] == 1)
	{
  read_val  = eflashBuffer[0x80 + 0];
  read_val |= (uint32_t)eflashBuffer[0x80 + 1] << 8;
  read_val |= (uint32_t)eflashBuffer[0x80 + 2] << 16;
  read_val |= (uint32_t)eflashBuffer[0x80 + 3] << 24;
  LEVEL_cnt = read_val;
	}
	else
	{
		LEVEL_cnt = 200000;
  }
}



/*******************************************************
 * @brief    背光初始化
 * @param    xxx:xxx
 * @return
********************************************************/
void lightinit(void)
{
	  u8 eflashBuffer[512];
    EflashReadPage(DATA_AREA_ADDR, 512, (u32*)eflashBuffer);   
	  if(eflashBuffer[0x31] != 0)
		{
			eflashBuffer[0x31] = 0;
			eflashBuffer[0x30] = 0xFA;
			EflashWritePageHandle(DATA_AREA_ADDR, 512, eflashBuffer);
    }
    light = eflashBuffer[0x30];
		Send_SetBL(light); 
		Timer0DelayMs(20);
}


/*******************************************************
 * @brief    按键蜂鸣器声音初始化
 * @param    xxx:xxx
 * @return
********************************************************/
void Buzzinit(void)
{
	  u8 eflashBuffer[512];
	
    EflashReadPage(DATA_AREA_ADDR, 512, (u32*)eflashBuffer);   
	  if(eflashBuffer[0x20] != 0 && eflashBuffer[0x20] != 1)
		{
			
			eflashBuffer[0x20] = 0;
			EflashWritePageHandle(DATA_AREA_ADDR, 512, eflashBuffer);
    }
			 
    BuzzFlag = eflashBuffer[0x20]; 
    if 	(BuzzFlag == 0)
		{
			Send_SetBeep(0);
    }
		else if(BuzzFlag == 1)
		{
			Send_SetBeep(30);
    }
		
}
/*******************************************************
 * @brief    Flash初始选择
 * @param    xxx:xxx
 * @return
********************************************************/

void FLASH_readinit(void)
{
    u8 eflashBuffer[512];

	  EflashReadPage(DATA_AREA_ADDR, 512, (u32*)eflashBuffer);    //先读内部flash的flash切换标志位
	  firmware_info.FirmwareAreaFlag = eflashBuffer[0x10];
}



/*******************************************************
 * @brief    设备初始化
 * @param    xxx:xxx
 * @return
********************************************************/

void DeviceInit(void)
{
    u8 DecryBuffer[512];
    u16 CalCrc;
    u16 GetCrc;
    u8 found = 0;


    ExFlashRead((u8 *)&firmware_info, 0, 512);
    DecryptDataByAesECB((u8 *)&firmware_info, sizeof(firmware_info), DecryBuffer);
    memcpy(&firmware_info, DecryBuffer, 512);
    CalCrc = GetCrc16((char *)&firmware_info, 510);
    GetCrc = (firmware_info.crc[0] << 8) + firmware_info.crc[1];
    if ((firmware_info.KeyFlag == 0x3435) && (CalCrc == GetCrc))
    {
        found = 1;
    }

    if (!found)
    {
        InitialFirmwareInfo();                  //清空表头信息，避免显示乱码
        isDeviceNormalFlag = DEVICE_ABNORMAL;   //标记为异常
    }

    DeviceShadowCountInit();  //影子点数存储区初始化
    LoadDeviceCloudParameter(); //加载设备三元组
		lightinit();
    LcdMainpage0();  //显示首页
		Multi_LEDInit();
		Buzzinit();
		Beep_Init(); 
}

/*******************************************************
 * @brief    OTA固件升级
 * @param    xxx:xxx
 * @return
********************************************************/
u8 OTA_Event_Handle(void)
{
    u8 Ret;
    u8 GetPackage[] = "获取固件包中...\0" ;
	  u8 GetPackage_en[] = "Receiving Firmware Package...\0" ;

    /*操作前先判断设备状态是否正常*/
    if (isDeviceNormalFlag == DEVICE_ABNORMAL)
    {
        DeviveAbnormalShow();
        return OTA_DEVICE_ABNORMAL;
    }
    Timer0DelayMs(10);
    /*连接云服务器*/
    Connecttest();

    W5500Reset();
    if (W5500PhyLinkCheck())
    {
        NetWork_Flag = W5500;
    }
    else
    {
        NetWork_Flag = ESP8266;
    }
   ConnectShow();
    while (ConnectTencentCloud() != OTA_SUCCESS)
    {      
            ConnectTencentCloudShow();
            return OTA_LINK_CLOUD_ERROR;
    }


    /*获取时间*/
    GetNTPTime();

   Send_SetText(0x0001, GetPackage, sizeof(GetPackage)); 
	 Send_SetText(0x0004, GetPackage_en, sizeof(GetPackage_en)); 
    /*获取固件包信息*/
    Ret = GetOtaInfo();
    if (Ret != OTA_SUCCESS)
    {
        GetOTAInfoShow(Ret);
        return Ret;
    }
    else
    {
        OTAUPDATA(); //跳转到升级确认页面
    }
    return Ret;
}



u8 OTA_Event(void)
{
    u8 Ret;
    u8 eflashBuffer[512];
//     Send_SetText(0x0006, str_downloading_en, sizeof(str_downloading_en));
// 	  Timer0DelayMs(20);
    Ret = StartDownloadFirmware();
    if (Ret != OTA_COMMAND_ANALYSIS)
    {
        DownloadFirmwareShow(Ret);
        return Ret;
    }

    /*校验固件包*/
    Ret = VerifyFirmwareHandle();
		
    EflashReadPage(DATA_AREA_ADDR, 512, (u32*)eflashBuffer);
    eflashBuffer[0x10] = firmware_info.FirmwareAreaFlag;
    EflashWritePageHandle(DATA_AREA_ADDR, 512, eflashBuffer); //校验完成后把flash对应的标志写入内部flash


    if (Ret != OTA_FIRMWARE_VERIFY_SUCCES)
    {
        VerifyFirmwareShow(Ret);
        return Ret;
    }
    ResultShow(Download_Firmware);
    return Ret;
}




/*******************************************************
 * @brief    点数升级
 * @param    xxx:xxx
 * @return
********************************************************/
u8 CountEventHandle(void)
{
    u8 Ret;

    /*操作前先判断设备状态是否正常*/
    if (isDeviceNormalFlag == DEVICE_ABNORMAL)
    {
        DeviveAbnormalShow();
        return OTA_DEVICE_ABNORMAL;
    }

    /*连接云服务器*/
    Connecttest();
		W5500Reset();
    if (W5500PhyLinkCheck())
    {
        NetWork_Flag = W5500;
    }
    else
    {
        NetWork_Flag = ESP8266;
    }
    ConnectShow();
    while (ConnectTencentCloud() != OTA_SUCCESS)
    {
            ConnectTencentCloudShow();
            return OTA_LINK_CLOUD_ERROR;
    }
    Ret = GetShadowInfo();
    //断开连接
    DisconnectTencentCloud();
    ShadowCountShow(Ret);
    return Ret;
}


/*******************************************************
 * @brief    连接服务器，并恢复出厂设置
 * @param    xxx:xxx
 * @return
********************************************************/
u8 Factory_Reset_Event_Handle(void)
{

    Connecttest();
    if (FactoryReset() == OTA_CONNECT_SERVER_ERR)
    {
        ConnectTencentCloudShow();
        return OTA_LINK_CLOUD_ERROR;
    }
    ResultShow(Factory_Reset);
    isDeviceNormalFlag = DEVICE_NORMAL;   //标记为正常


    return OTA_SUCCESS;
}

/*******************************************************
 * @brief    搬运固件包至内部FLASH，并跳转APP
 * @param    xxx:xxx
 * @return
********************************************************/
u8 Jump_App_Handle(void)
{
    u8 Ret;
    u8 eflashBuffer[512];
    /*操作前先判断设备状态是否正常*/
    if (isDeviceNormalFlag == DEVICE_ABNORMAL)
    {
        DeviveAbnormalShow();
        return OTA_DEVICE_ABNORMAL;
    }
		
    EflashReadPage(DATA_AREA_ADDR, 512, (u32*)eflashBuffer);
		
		if ((eflashBuffer[0x10]) == 1)
		{
			firmware_info.FirmwareAreaFlag = 2;
    }
		else if ((eflashBuffer[0x10]) == 2)
		{
			firmware_info.FirmwareAreaFlag = 1;
    }
		else if ((eflashBuffer[0x10]) == 0)
		{
			firmware_info.FirmwareAreaFlag = 0;
    }
			

    Ret = JumpAppAfterDecrypt();
    JumpAppShow(Ret);
    return Ret;
}

/*******************************************************
 * @brief    Wifi设置显示使用
 * @param    xxx:xxx
 * @return
********************************************************/
void WIFIconnect(void)
{
    ConnectShow();
    if (ConnectTencentCloud() == OTA_SUCCESS)
    {
        Send_SetText(0x0001, textWIFIOk, sizeof(textWIFIOk));
			  Send_SetText(0x0004, textWIFIOk_en, sizeof(textWIFIOk_en));
			  Timer0DelayMs(25);
        Send_SetText(0x0002, textConfigNetOk, sizeof(textConfigNetOk));
			  Send_SetText(0x0005, textConfigNetOk_en, sizeof(textConfigNetOk_en));
    }
    else
    {
        Send_SetText(0x0001, textConfigNetFail, sizeof(textConfigNetFail));
			  Send_SetText(0x0004, textConfigNetFail_en, sizeof(textConfigNetFail_en));
			  Timer0DelayMs(25);
        Send_SetText(0x0002, text_please_retry, sizeof(text_please_retry));
			  Send_SetText(0x0005, text_please_retry_en, sizeof(text_please_retry_en));
    }
}



