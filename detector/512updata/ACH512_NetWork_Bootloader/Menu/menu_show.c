#include "menu_show.h"
#include "lcd.h"
#include "gpio.h"
#include "cloud_platform_para.h"
#include "delay.h"
#include "ota_firmware_handle.h"
#include "eflash.h"
#include "lcd_show.h"
#include "hmi_driver.h"
#include "use_lcd.h"

extern firmware_info_t firmware_info;



/*******************************************************/
/*                  界面显示                            */
/*******************************************************/


void ConnectTencentCloudShow(void)
{
    Send_SetText(0x0001, str_net_check, sizeof(str_net_check)); // 设备状态异常
	  Timer0DelayMs(30);
  	Send_SetText(0x0004, str_net_check_en, sizeof(str_net_check_en)); 
}



void DeviveAbnormalShow(void)
{
    Send_SetText(0x0001, str_dev_err, sizeof(str_dev_err)); // 设备状态异常
	  Timer0DelayMs(30);
	  Send_SetText(0x0004, str_dev_err_en, sizeof(str_dev_err_en)); // 设备状态异常
   	Timer0DelayMs(30);
    Send_SetText(0x0002, str_restore_factory, sizeof(str_restore_factory)); // 请恢复出厂设置并重新下载固件
	  Timer0DelayMs(30);
	  Send_SetText(0x0005, str_restore_factory_en, sizeof(str_restore_factory_en)); 
}


void GetOTAInfoShow(u8 Result)
{
    char ErrCode[15] = {0};
    sprintf(ErrCode, "Error Code %d", Result);
//     OTAfail();  //跳转到下载失败页显示
    Send_SetText(0x0001, str_get_fw_info_fail, sizeof(str_get_fw_info_fail)); //获取固件信息失败
    Timer0DelayMs(25);
    Send_SetText(0x0004, str_get_fw_info_fail_en, sizeof(str_get_fw_info_fail_en)); 
    Timer0DelayMs(25);
    Send_SetText(0x0002, (u8*)ErrCode, sizeof(ErrCode));

}

void DownloadFirmwareShow(u8 Result)
{
    char ErrCode[15] = {0};
    sprintf(ErrCode, "Error Code %d", Result);
    Send_SetText(0x0001, str_download_fw_fail, sizeof(str_download_fw_fail)); //下载固件失败
    Timer0DelayMs(30);
    Send_SetText(0x0006, str_download_fw_fail_en, sizeof(str_download_fw_fail_en));
    Timer0DelayMs(30);
    Send_SetText(0x0007, (u8*)ErrCode, sizeof(ErrCode));

}



void VerifyFirmwareShow(u8 Result)
{
    char ErrCode[15] = {0};

    sprintf(ErrCode, "Error Code %d", Result);
    OTAfail();
    Send_SetText(0x0001, str_verify_fw_fail, sizeof(str_verify_fw_fail)); //验证固件失败
    Timer0DelayMs(30);
    Send_SetText(0x0006, str_verify_fw_fail_en, sizeof(str_verify_fw_fail_en));
    Timer0DelayMs(30);
    Send_SetText(0x0007, (u8*)ErrCode, sizeof(ErrCode));
}




void ConnectShow(void)   //连接页面显示
{ 
    Send_SetText(0x0001, textConnecting, sizeof(textConnecting));
	  Timer0DelayMs(40);
	  Send_SetText(0x0004, textConnecting_en, sizeof(textConnecting_en));
}

void ResultShow(u8 Action)
{
    if (Action == Download_Firmware)
    {
        Send_SetText(0x0001, str_download_ok, sizeof(str_download_ok));
			  Timer0DelayMs(40);
			  Send_SetText(0x0006, str_download_ok_en, sizeof(str_download_ok_en));
    }
    else if (Action == Factory_Reset)
    {
        Send_SetText(0x0001, str_set_ok, sizeof(str_set_ok));
			  Timer0DelayMs(30);
			  Send_SetText(0x0004, str_set_ok_en, sizeof(str_set_ok_en));
    }
}

void JumpAppShow(u8 Result)
{

    if (Result == OTA_FIRMWARE_EMPTY)
    {
        Send_SetText(0x0001, str_no_firmware, sizeof(str_no_firmware));// 未检测到固件
			  Timer0DelayMs(30);
			  Send_SetText(0x0004, str_no_firmware_en, sizeof(str_no_firmware_en));
    }
    else if (Result == OTA_JUMP_APP_ERROR)
    {
        Send_SetText(0x0001, str_jump_fail, sizeof(str_jump_fail));// 跳转失败
			  Timer0DelayMs(30);
			  Send_SetText(0x0004, str_jump_fail_en, sizeof(str_jump_fail_en));
    }
    else if (Result == OTA_WRITE_FLASH_ERROR)
    {
        Send_SetText(0x0001, str_write_fail, sizeof(str_write_fail));// 写入失败
			  Timer0DelayMs(30);
			  Send_SetText(0x0004, str_write_fail_en, sizeof(str_write_fail_en));
    }
}


void ShadowCountShow(u8 Result)
{
    char ErrCode[15] = {0};

    sprintf(ErrCode, "Error Code %d", Result);
    if (Result == SHADOW_SET_SUCCESS)
    {
        Send_SetText(0x0001, textUpdatePointOk, sizeof(textUpdatePointOk));
			  Timer0DelayMs(30);
			  Send_SetText(0x0004, textUpdatePointOk_en, sizeof(textUpdatePointOk_en));
    }
    else
    {
        Send_SetText(0x0001, textUpdatePointFail, sizeof(textUpdatePointFail));
			  Timer0DelayMs(30);
			  Send_SetText(0x0004, textUpdatePointFail_en, sizeof(textUpdatePointFail_en));
			  Timer0DelayMs(30);
        Send_SetText(0x0002, (u8*)ErrCode, sizeof(ErrCode));
    }

}


void RunningShow(u8 Action)          //加载、下载页面显示
{
    if (Action == DOWNLOAD_RUNNING)
    {
        Send_SetText(0x0001, str_downloading, sizeof(str_downloading));
			  Timer0DelayMs(40);
			  Send_SetText(0x0006, str_downloading_en, sizeof(str_downloading_en));
			  Timer0DelayMs(30);
    }
    else if (Action == UPDATE_RUNNING)
    {
        Send_SetText(0x0001, str_checking, sizeof(str_checking));
			  Timer0DelayMs(30);
			  Send_SetText(0x0006, str_checking_en, sizeof(str_checking_en));
    }
    else if (Action == JUMP_APP_RUNNING)
    {
        Send_SetText(0x0001, str_waiting, sizeof(str_waiting));
			  Timer0DelayMs(30);
			  Send_SetText(0x0004, str_waiting_en, sizeof(str_waiting_en));
			  Timer0DelayMs(30);
    }

}


void WIFI_ConnectShow(void)
{
    Send_SetText(0x0001, textConnecting, sizeof(textConnecting));
	  Timer0DelayMs(30);
	  Send_SetText(0x0004, textConnecting_en, sizeof(textConnecting_en));
}

void WIFI_ConnectSuccesShow(void)
{
    Send_SetText(0x0001, textWIFIOk, sizeof(textWIFIOk));
	  Timer0DelayMs(30);
  	Send_SetText(0x0004, textWIFIOk_en, sizeof(textWIFIOk_en));
}


void SmartConfig_SuccessShow(void)
{
    Send_SetText(0x0001, textConfigNetOk, sizeof(textConfigNetOk));
	  Timer0DelayMs(30);
	  Send_SetText(0x0004, textConfigNetOk_en, sizeof(textConfigNetOk_en));
	
    Send_SetText(0x0002, text_wifi_connected, sizeof(text_wifi_connected));
	  Timer0DelayMs(30);
	  Send_SetText(0x0005, text_wifi_connected_en, sizeof(text_wifi_connected_en));

}

void SmartConfig_ErrorShow(void)
{
    Send_SetText(0x0001, textConfigNetFail, sizeof(textConfigNetFail));
	  Timer0DelayMs(30);
	  Send_SetText(0x0004, textConfigNetFail_en, sizeof(textConfigNetFail_en));
}


void APPcomming(void)
{
	Timer0DelayMs(30);
	Send_SetText(0x0001, text_coming_in, sizeof(text_coming_in));
	Timer0DelayMs(30);
	Send_SetText(0x0004, text_coming_in_en, sizeof(text_coming_in_en));
}

