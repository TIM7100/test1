#include "menu_show.h"
#include "lcd.h"
#include "gpio.h"
#include "cloud_platform_para.h"
#include "delay.h"
#include "ota_firmware_handle.h"
#include "eflash.h"

extern firmware_info_t firmware_info;

/*中英文标志FLAG*/
static u8 DeviceLanguageFlag;

/*******************************************************
 * @brief    设备语言初始化
 * @param    xxx:xxx
 * @return
********************************************************/
void DeviceLanguageInit(void)
{
	DeviceLanguageFlag = *(u8*)(DeviceLanguageAddr);
	
	if((DeviceLanguageFlag != CN) && (DeviceLanguageFlag != EN))
	{
		SetDeviceLanguage(EN);  //默认英文
	}
}

/*******************************************************
 * @brief    设置设备语言
 * @param    xxx:xxx
 * @return
********************************************************/
void SetDeviceLanguage(u8 Language)
{
	u8 eflashBuffer[512];
	
	EflashReadPage(DATA_AREA_ADDR, 512, (u32*)eflashBuffer);
	/*中英文标志位*/
	eflashBuffer[0x10] = Language;
	EflashWritePageHandle(DATA_AREA_ADDR, 512, eflashBuffer);
	DeviceLanguageFlag = *(u8*)(DeviceLanguageAddr);
}

/*******************************************************
 * @brief    获取当前设备语言
 * @param    xxx:xxx
 * @return
********************************************************/
u8 GetDeviceLanguage(void)
{
	return DeviceLanguageFlag;
}

/*******************************************************/
/*					界面显示						    */
/*******************************************************/
void LanguageSetResultShow(void)
{
    ClearShow();
    if (DeviceLanguageFlag == EN)
    {
		LCD_UartSend("DC24(192,97,'设置成功',2);\r\n");
        LCD_UartSend("DC24(156,151,'- \"Cancel\"返回 -',4);\r\n");
    }
    else if (DeviceLanguageFlag == CN)
    {
        LCD_UartSend("DC24(156,97,'Set Successfully',2);\r\n");
        LCD_UartSend("DC24(156,151,'- \"Cancel\"Return -',4);\r\n");
    }
}

void DeviceInitShow(void)
{
    ClearShow();LCD_UartSend("DC24(150,124,'设备初始化中...',2);\r\n");
    LCD_UartSend("DC24(156,164,'device init...',2);\r\n");
    
}

void LcdMainpage(void)          //主页面显示
{
	u32 KeyValue;
    //---------等待LCD上电-----------
    Timer0DelayMs(10);
    //-------------------------------
	
	ClearShow();
	if (DeviceLanguageFlag == EN)
	{
		LCD_UartSend("DC32(140,10,'Home Page',2);\r\n");
		//显示设备名
		LCD_UartSend("DC24(90,82,'Device:',4);\r\n");
 		LCD_UartSend(CMD_DC24(210, 82,(u8*)GetDeviceName(), 4));
		//显示设备版本
		LCD_UartSend("DC24(90,134,'DeviceVer:',4);\r\n");
		LCD_UartSend(CMD_DC24(210, 134, DEVICE_VERSION, 4));
		//显示固件版本
		LCD_UartSend("DC24(90,186,'Firmware:',4);\r\n");
		LCD_UartSend(CMD_DC24(210, 186, (u8*)firmware_info.local_firmware_version, 4));
		LCD_UartSend("DC24(160,220,'- \"Enter\"Next -',4);\r\n");
	}
	else
	{
		LCD_UartSend("DC32(204,10,'主页',2);\r\n");
		//显示设备名
		LCD_UartSend("DC24(90,82,'设备名:',4);\r\n");
		LCD_UartSend(CMD_DC24(210, 82, (u8*)GetDeviceName(), 4));
		//显示设备版本
		LCD_UartSend("DC24(90,134,'设备版本:',4);\r\n");
		LCD_UartSend(CMD_DC24(210, 134, DEVICE_VERSION, 4));
		//显示固件版本
		LCD_UartSend("DC24(90,186,'固件版本:',4);\r\n");
		LCD_UartSend(CMD_DC24(210, 186, (u8*)firmware_info.local_firmware_version, 4));
		LCD_UartSend("DC24(150,220,'- \"Enter\"Next -',4);\r\n");
	}

	while (1)
    {
		KeyValue = 0x0000;
        KeyValue =  wait_Key();
        if (KeyValue == Key_Enter)
        {
            break;
        }
    }
}

void ConnectTencentCloudShow(void)
{
    ClearShow();
	if (DeviceLanguageFlag == EN)
	{
		LCD_UartSend("DC24(126,88,'Please check network',1);\r\n");
		LCD_UartSend("DC24(144,136,'and network cable',1);\r\n");
		LCD_UartSend("DC24(150,184,'- \"Cancel\"Return -',4);\r\n");  
	}
	else
	{
        LCD_UartSend("DC24(144,88,'请检查网线是否连接',1);\r\n");
        LCD_UartSend("DC24(168,136,'并检查网络状态',1);\r\n");
        LCD_UartSend("DC24(150,184,'- \"Cancel\"返回 -',4);\r\n");
	}

}

void DeviveAbnormalShow(void)
{
	ClearShow();
	if (DeviceLanguageFlag == EN)
	{
		LCD_UartSend("DC24(156,94,'Device Abnormal',1);\r\n");
		LCD_UartSend("DC24(132,138,'Please Factory Reset',1);\r\n");
		LCD_UartSend("DC24(102,182,'And Download Firmware Again',1);\r\n");
		LCD_UartSend("DC24(150,226,'- \"Cancel\"Return -',4);\r\n");  
	}
	else
	{
		LCD_UartSend("DC24(168,94,'设备状态异常',1);\r\n");
		LCD_UartSend("DC24(156,138,'请恢复出厂设置',1);\r\n");
		LCD_UartSend("DC24(156,182,'并重新下载固件',1);\r\n");
		LCD_UartSend("DC24(150,226,'- \"Cancel\"返回 -',4);\r\n");
	}

}

void GetOTAInfoShow(u8 Result)
{
	char ErrCode[15] = {0};

	ClearShow();
	sprintf(ErrCode, "Error Code %d", Result);

	if (DeviceLanguageFlag == EN)
	{
		LCD_UartSend("DC24(144,115,'Get Firmware Err',1);\r\n");
		LCD_UartSend(CMD_DC24(150,157, (u8*)ErrCode, 1));
	}
	else
	{
		LCD_UartSend("DC24(156,115,'获取固件信息失败',1);\r\n");
		LCD_UartSend(CMD_DC24(150,157, (u8*)ErrCode, 1));
	}
}

void DownloadFirmwareShow(u8 Result)
{
	char ErrCode[15] = {0};

	ClearShow();
	sprintf(ErrCode, "Error Code %d", Result);
	if (DeviceLanguageFlag == EN)
	{
		LCD_UartSend("DC24(132,115,'Download Firmware Err',1);\r\n");
		LCD_UartSend(CMD_DC24(150, 157, (u8*)ErrCode, 1));
	}
	else
	{
		LCD_UartSend("DC24(192,115,'下载固件失败',1);\r\n");
		LCD_UartSend(CMD_DC24(150, 157, (u8*)ErrCode, 1));
	}
}

void VerifyFirmwareShow(u8 Result)
{
	char ErrCode[15] = {0};

	ClearShow();
	sprintf(ErrCode, "Error Code %d", Result);
	if (DeviceLanguageFlag == EN)
	{
		LCD_UartSend("DC24(144,115,'Verif Firmware Err',1);\r\n");
		LCD_UartSend(CMD_DC24(150, 157, (u8*)ErrCode, 1));
	}
	else
	{
		LCD_UartSend("DC24(192,115,'验证固件失败',1);\r\n");
		LCD_UartSend(CMD_DC24(150, 157, (u8*)ErrCode, 1));
	}
}


void ConnectShow(void)          //连接页面显示
{
	Interface_Display();
	Ethernet_Action();
	if (DeviceLanguageFlag == EN)
	{
		LCD_UartSend("DC24(168,124,'Connecting...',2);\r\n");
	}
	else
	{
		LCD_UartSend("DC24(174,124,'正在连接...',2);\r\n");
	}
}

void ResultShow(u8 Action)
{
    ClearShow();
	
	if (DeviceLanguageFlag == EN)
	{
		if(Action == Download_Firmware)
		{
			LCD_UartSend("DC24(192,97,'Download',2);\r\n");
			LCD_UartSend("DC24(174,141,'Successfully',2);\r\n");
		}
		else if(Action == Factory_Reset)
		{ 
			LCD_UartSend("DC24(156,119,'Set Successfully',2);\r\n");
		}
		LCD_UartSend("DC24(150,183,'- \"Cancel\"Return -',4);\r\n");
	}
	else
	{
		if(Action == Download_Firmware)
		{
			LCD_UartSend("DC24(192,119,'下载成功',2);\r\n");
		}
		else if(Action == Factory_Reset)
		{ 
			LCD_UartSend("DC24(192,119,'设置成功',2);\r\n");
		}
		LCD_UartSend("DC24(150,183,'- \"Cancel\"返回 -',4);\r\n");
	}
}

void JumpAppShow(u8 Result)
{
	ClearShow();
	
	if (DeviceLanguageFlag == EN)
	{
		if(Result == OTA_FIRMWARE_EMPTY)
		{
			LCD_UartSend("DC24(162,124,'Firmware Empty',1);\r\n");
		}
		else if(Result == OTA_JUMP_APP_ERROR)
		{
			LCD_UartSend("DC24(162,124,'Jump Error',1);\r\n");
		}
		else if(Result == OTA_WRITE_FLASH_ERROR)
		{
			LCD_UartSend("DC24(162,124,'Write Error',1);\r\n");
		}
		LCD_UartSend("DC24(150,154,'- \"Cancel\"Return -',4);\r\n");
	}
	else
	{
		if(Result == OTA_FIRMWARE_EMPTY)
		{
			LCD_UartSend("DC24(180,124,'未检测到固件',1);\r\n");
		}
		else if(Result == OTA_JUMP_APP_ERROR)
		{
			LCD_UartSend("DC24(192,124,'跳转失败',1);\r\n");
		}
		else if(Result == OTA_WRITE_FLASH_ERROR)
		{
			LCD_UartSend("DC24(192,124,'写入失败',1);\r\n");
		}
		LCD_UartSend("DC24(150,154,'- \"Cancel\"返回 -',4);\r\n");
	}
}


void ShadowCountShow(u8 Result)
{
	char ErrCode[15] = {0};

	ClearShow();
	sprintf(ErrCode, "Error Code %d", Result);

	if (DeviceLanguageFlag == EN)
	{
		if(Result == SHADOW_SET_SUCCESS)
		{
			LCD_UartSend("DC24(168,124,'Update Count Success',2);\r\n");
		}
		else
		{
			LCD_UartSend("DC24(168,124,'Update Count Err',1);\r\n");
			LCD_UartSend(CMD_DC24(194, 150, (u8*)ErrCode, 1));
		}
		LCD_UartSend("DC24(168,154,'- \"Cancel\"Return -',4);\r\n");
	}
	else
	{
		if(Result == SHADOW_SET_SUCCESS)
		{
			LCD_UartSend("DC24(168,124,'更新点数成功',2);\r\n");
		}
		else
		{
			LCD_UartSend("DC24(168,124,'更新点数失败',1);\r\n");
			LCD_UartSend(CMD_DC24(194, 150, (u8*)ErrCode, 1));
		}
		LCD_UartSend("DC24(168,154,'- \"Cancel\"返回 -',4);\r\n");
	}
}

u32 WaitingUserPress(u8 Action)
{
	u32 KeyValue;
	
	ClearShow();

	if (DeviceLanguageFlag == EN)
	{
		if(Action == Download_Firmware)
		{
			LCD_UartSend("DC24(144,88,'Download Firmware',2);\r\n");
		}
		else if(Action == Factory_Reset)
		{
			LCD_UartSend("DC24(144,88,'Restore Factory ?',2);\r\n");
		}
		else if(Action == JUMP_APPLICATION)
		{
			LCD_UartSend("DC24(144,88,'JUMP APP ?',2);\r\n");
		}
		LCD_UartSend("DC24(144,132,'- \"Enter\"Next -',4);\r\n");
		LCD_UartSend("DC24(144,166,'- \"Cancel\"Return -',4);\r\n");
	}
	else
	{
		if(Action == Download_Firmware)
		{
			LCD_UartSend("DC24(144,88,'新固件包，是否下载',2);\r\n");
		}
		else if(Action == Factory_Reset)
		{
			LCD_UartSend("DC24(144,88,'是否恢复出厂设置',2);\r\n");
		}
		else if(Action == JUMP_APPLICATION)
		{
			LCD_UartSend("DC24(144,88,'是否进入应用程序',2);\r\n");
		}
		LCD_UartSend("DC24(144,132,'- \"Enter\"继续 -',4);\r\n");
        LCD_UartSend("DC24(144,166,'- \"Cancel\"返回 -',4);\r\n");
	}
	
	while (1)
    {
		KeyValue = 0x0000;
        KeyValue =  wait_Key();
        if (KeyValue == Key_Enter || KeyValue == Key_Esc)
        {
            break;
        }
    }
	return KeyValue;
}

void RunningShow(u8 Action)          //加载、下载页面显示
{
    ClearShow();

	if (DeviceLanguageFlag == EN)
	{
		if (Action == DOWNLOAD_RUNNING)
		{
			LCD_UartSend("DC24(186,98,'Downloading...',5);\r\n");
		}
		else if (Action == UPDATE_RUNNING)
		{
			LCD_UartSend("DC24(186,98,'Updating...',5);\r\n");
		}
		else if(Action == JUMP_APP_RUNNING)
		{
			LCD_UartSend("DC24(186,98,'Watiting...',5);\r\n");
		}
	}
	else
	{
		if (Action == DOWNLOAD_RUNNING)
		{
			LCD_UartSend("DC24(186,98,'下载中...',5);\r\n");
		}
		else if (Action == UPDATE_RUNNING)
		{
			LCD_UartSend("DC24(186,98,'校验中...',5);\r\n");
		}
		else if(Action == JUMP_APP_RUNNING)
		{
			LCD_UartSend("DC24(186,98,'等待...',5);\r\n");
		}
	}
//     LCD_UartSend("BOX(166,158,150,90,16);\r\n");
	LCD_UartSend("BOX(100,160,380,180,16);\r\n");

}


void ProgressBarShow(u32 AllCut, u32 CurrentCut)
{
    char BoxBuf[32] = {0};
//     sprintf(BoxBuf, "BOXF(10,75,%02d,90,5);\r\n", ((140 * CurrentCut) / AllCut) + 10);
    sprintf(BoxBuf, "BOXF(100,160,%d,180,5);\r\n", 100 + (280 * CurrentCut) / AllCut);
    LCD_UartSend(BoxBuf);
}

/*
void Interface_Display(void)
{
	int i;
	
    char Interface_buf[32];
    LCD_UartSend("CLR(0);\r\n");        //清屏
    LCD_UartSend("DIR(1);\r\n");         //横屏

    LCD_UartSend("PL(35,30,110,30,2);\r\n");//线
    LCD_UartSend("PL(25,40,100,40,2);\r\n");

    LCD_UartSend("PL(35,30,25,40,2);\r\n");

    LCD_UartSend("PL(110,30,100,40,2);\r\n");


    LCD_UartSend("PL(25,40,25,100,2);\r\n");

    LCD_UartSend("PL(100,40,100,100,2);\r\n");

    LCD_UartSend("PL(25,100,100,100,2);\r\n");

    LCD_UartSend("PL(110,30,110,90,2);\r\n");

    LCD_UartSend("PL(100,100,110,90,2);\r\n");


    for (i = 30; i < 90; i += 3)  //点
    {
        sprintf(Interface_buf, "PS(35,%d,2);\r\n", i);
        LCD_UartSend(Interface_buf);
    }

    for (i = 35; i < 110; i += 3)
    {
        sprintf(Interface_buf, "PS(%d,90,2);\r\n", i);
        LCD_UartSend(Interface_buf);
    }


    LCD_UartSend("PS(27,98,2);\r\n");
    LCD_UartSend("PS(29,96,2);\r\n");
    LCD_UartSend("PS(31,94,2);\r\n");
    LCD_UartSend("PS(33,92,2);\r\n");



    LCD_UartSend("PL(110,40,100,50,4);\r\n");  //端口1

    LCD_UartSend("PL(110,50,100,60,4);\r\n");

    LCD_UartSend("PL(110,40,110,50,4);\r\n");
    LCD_UartSend("PL(100,50,100,60,4);\r\n");


    LCD_UartSend("PL(110,70,100,80,4);\r\n");//端口2

    LCD_UartSend("PL(110,80,100,90,4);\r\n");

    LCD_UartSend("PL(110,70,110,80,4);\r\n");
    LCD_UartSend("PL(100,80,100,90,4);\r\n");

    LCD_UartSend("PL(102,30,92,40,4);\r\n");//网口
    LCD_UartSend("PL(92,30,82,40,4);\r\n");

    LCD_UartSend("PL(102,30,92,30,4);\r\n");

    LCD_UartSend("PL(82,40,92,30,4);\r\n");


    LCD_UartSend("PL(50,30,40,40,4);\r\n");//电源
    LCD_UartSend("PL(40,30,30,40,4);\r\n");

    LCD_UartSend("PL(50,30,40,30,4);\r\n");

    LCD_UartSend("PL(30,40,40,30,4);\r\n");



    LCD_UartSend("PL(72,30,62,40,4);\r\n");//网口
    LCD_UartSend("PL(62,30,52,40,4);\r\n");

    LCD_UartSend("PL(72,30,62,30,4);\r\n");

    LCD_UartSend("PL(52,40,62,30,4);\r\n");

}
*/
void Interface_Display(void)
{
	int i;
	
    char Interface_buf[32];
    LCD_UartSend("CLR(0);\r\n");        //清屏
    LCD_UartSend("DIR(1);\r\n");         //横屏

    LCD_UartSend("PL(175,66,325,66,2);\r\n");//线
    LCD_UartSend("PL(155,86,305,86,2);\r\n");

    LCD_UartSend("PL(175,66,155,86,2);\r\n");

    LCD_UartSend("PL(325,66,305,86,2);\r\n");


    LCD_UartSend("PL(155,86,155,206,2);\r\n");

    LCD_UartSend("PL(305,86,305,206,2);\r\n");

    LCD_UartSend("PL(155,206,305,206,2);\r\n");

    LCD_UartSend("PL(325,66,325,186,2);\r\n");

    LCD_UartSend("PL(305,206,325,186,2);\r\n");


    for (i = 30; i < 90; i += 3)  //点
    {
        int new_y = (i - 30)*2 + 66;
        sprintf(Interface_buf, "PS(175,%d,2);\r\n", new_y);
        LCD_UartSend(Interface_buf);
    }

    for (i = 35; i < 110; i += 3)
    {
        int new_x = (i - 25)*2 + 155;
        sprintf(Interface_buf, "PS(%d,186,2);\r\n", new_x);
        LCD_UartSend(Interface_buf);
    }


    LCD_UartSend("PS(159,202,2);\r\n");
    LCD_UartSend("PS(163,198,2);\r\n");
    LCD_UartSend("PS(167,194,2);\r\n");
    LCD_UartSend("PS(171,190,2);\r\n");



    LCD_UartSend("PL(325,86,305,106,4);\r\n");  //端口1

    LCD_UartSend("PL(325,106,305,126,4);\r\n");

    LCD_UartSend("PL(325,86,325,106,4);\r\n");
    LCD_UartSend("PL(305,106,305,126,4);\r\n");


    LCD_UartSend("PL(325,146,305,166,4);\r\n");//端口2

    LCD_UartSend("PL(325,166,305,186,4);\r\n");

    LCD_UartSend("PL(325,146,325,166,4);\r\n");
    LCD_UartSend("PL(305,166,305,186,4);\r\n");

    LCD_UartSend("PL(269,66,249,86,4);\r\n");//网口
    LCD_UartSend("PL(249,66,229,86,4);\r\n");

    LCD_UartSend("PL(269,66,249,66,4);\r\n");

    LCD_UartSend("PL(229,86,249,66,4);\r\n");


    LCD_UartSend("PL(215,66,195,86,4);\r\n");//电源
    LCD_UartSend("PL(195,66,175,86,4);\r\n");

    LCD_UartSend("PL(215,66,195,66,4);\r\n");

    LCD_UartSend("PL(175,86,195,66,4);\r\n");



    LCD_UartSend("PL(259,66,239,86,4);\r\n");//网口
    LCD_UartSend("PL(239,66,219,86,4);\r\n");

    LCD_UartSend("PL(259,66,239,66,4);\r\n");

    LCD_UartSend("PL(219,86,239,66,4);\r\n");
}
/*
void Ethernet_Action(void)//网口插入动作
{
    char Ethernet_buf[32];
	int i;
	
    for (i = 0; i < 25; i += 5)
    {
        sprintf(Ethernet_buf, "SBC(0);DCV16(85,%d,'↓',2);\r\n", i);
        LCD_UartSend(Ethernet_buf);

        Timer0DelayMs(10);
    }

	if (DeviceLanguageFlag == EN)
	{
		LCD_UartSend("DC16(95,5,'Net',2);\r\n");
	}
	else
	{
		LCD_UartSend("DC16(95,5,'网口',2);\r\n");

	}
}
*/
void Ethernet_Action(void)//网口插入动作
{
    char Ethernet_buf[32];
	int i;
	
    for (i = 0; i < 25; i += 5)
    {
        sprintf(Ethernet_buf, "SBC(0);DC24(290,%d,'↓',2);\r\n", 40 + i);
        LCD_UartSend(Ethernet_buf);
        Timer0DelayMs(10);
    }

	if (DeviceLanguageFlag == EN)
	{
		LCD_UartSend("DC24(324,40,'Net',2);\r\n");
	}
	else
	{
		LCD_UartSend("DC24(304,40,'网口',2);\r\n");
	}
}

