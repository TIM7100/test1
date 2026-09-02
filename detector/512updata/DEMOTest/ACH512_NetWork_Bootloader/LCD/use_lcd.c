#include "use_lcd.h"
#include "esp8266_config.h"
#include "w5500_user_conf.h"
#include "lcd_show.h"
#include "ota_firmware_update.h"
#include "cloud_platform_para.h"
#include "UserFunctionInterface.h"
#include "ota_firmware_handle.h"
#include  "eflash.h"

extern firmware_info_t firmware_info;
extern volatile u8 NetWork_Flag;

uint8  cmd_buffer[CMD_MAX_SIZE];
static qdata CMD_HEAD = NOTIFY_HEAD_TOUCH;   //默认触摸帧头
static u8 cmd_head = 0;     //指令头
static u8 cmd_type = 0;     //指令类型码
static u8 cmd_type_sub = 0;     //子指令类型
static u16 cmd_page_id = 0; //页面ID
static u16 cmd_ctrol_id = 0; //控件ID
static u16 cmd_ctrol_value = 0; //控件值
static u8 cmd_type_pos = 1; //指令类型码位置
u8 temp_Str[50] = {0}; //文本字符buffer
char account[32] = {0}; // 存放用户名文本
char password[32] = {0}; // 存放用户名密码
char UserPassword[4] = {0x31, 0x32, 0x33, 0x34};//恢复出厂密码1234
char Resetdevice[4] = {0};
extern u8 light;
u8 Buzz = 30;
u8 BuzzFlag = 0;
extern uint32_t read_val;
extern u8 BeepFlag;
u32  LEVEL_cnt =  LEVEL_cnt3;
u32 levels[] = {LEVEL_cnt1, LEVEL_cnt2, LEVEL_cnt3, LEVEL_cnt4, LEVEL_cnt5};
u8 Beep_cnt[] = {1, 2, 3};


void User_App(void)
{
    static u32 debounce = 0;          // 消抖计数器

    qsize size = 0;

    if (debounce > 0)
    {
        debounce--;
    }

    size = queue_find_cmd(cmd_buffer, CMD_MAX_SIZE, CMD_HEAD);
    if (size > 0)
    {
        cmd_head = cmd_buffer[0];
        cmd_type = Get_Buff_8(cmd_type_pos);

        switch (cmd_head)
        {
        case NOTIFY_HEAD_TOUCH:
            if (debounce > 0)
            {
                break;    // 消抖期间跳过触摸
            }
            debounce = LEVEL_cnt;          //  触摸后设置消抖计数
            Process_Touch((PCTRL_MSG)cmd_buffer, size);
            break;
        case NOTIFY_HEAD_RES:
            Process_Postback((PCTRL_MSG)cmd_buffer, size);
            break;
        }
    }
}


void AddStr(u8* str1, u8* str2, u8 sp, u8 len)//添加字符
{
    u8 i = 0;
    while (i < len)
    {
        str1[sp++] = str2[i];
        i++;
    }
}
void Num2Str(u8* str, u16 num, u8 sp, u8 ep)  //数字转为字符
{
    unsigned char index[] = "0123456789";
    char i = ep;
    do
    {
        str[--i] = index[num % 10];
        num /= 10;
    }
    while (i == sp);
}
void Postback_Get_PageID(void) //获取当前页面ID
{
    u16 pageid = Get_Buff_16(cmd_type_pos + 1); //获取当前页面ID
    AddStr(temp_Str, (u8*)"Page", 0, 4); //添加字符
    Num2Str(temp_Str, pageid, 5, 6); //添加数字

    Send_SetText(0x0010, temp_Str, 6); //显示字符
}


void Process_Touch(PCTRL_MSG msg, uint16 size)  //触摸指令处理
{
    u8 eflashBuffer[512];

    cmd_page_id = Get_Buff_16(cmd_type_pos + 1); //获取页面ID
    cmd_ctrol_id = Get_Buff_16(cmd_type_pos + 3); //获取控件ID
    cmd_ctrol_value = Get_Buff_16(cmd_type_pos + 9); //获取控件值


//==========================================
    switch (cmd_type) //指令类型
    {
    case 0x06:  //按钮控件
        if (cmd_page_id == PAGE_MENU ) //页面ID Page 0 菜单页
        {
            switch (cmd_ctrol_id) //控件ID
            {
            case 0x0001:
               Send_SetPage(PAGE_JUMP_APP_CONFIRM, 0);

                break;
            case 0x0002:

             Send_SetPage(PAGE_UPDATE_POINT, 0);

                break;
            case 0x0003:  //设置
                Send_SetPage(PAGE_SETTING_MENU, 0); //设置界面


                break;
            case 0x0008:  //更新固件
                Send_SetPage(PAGE_FW_UPGRADE, 0);


                break;
            case 0x0009:     //主界面返回按键
                LcdMainpage();
                break;



            }
        }

        if (cmd_page_id == PAGE_RESET_FACTORY) //页面ID   重置设备  Page 1
        {
            switch (cmd_ctrol_id) //控件ID
            {
            case 0x0001:
                Send_GetText(0x0004);
                CMD_HEAD = NOTIFY_HEAD_RES;

                break;

            case 0x0002:

                break;
						
						case 0x0003:
							Send_SetPage(PAGE_SETTING_MENU, 0); //回到设置菜单
                break;
						
						

            case 0x0004:

                break;
            }
        }

        if (cmd_page_id == PAGE_UPDATE_POINT) //页面ID  Page 2 更新点数
        {
            switch (cmd_ctrol_id) //控件ID
            {
            case 0x0001:
                Send_SetPage(PAGE_UPDATE_CONFIRM, 0);//确认界面
                Timer0DelayMs(30);
                CountEventHandle();
                break;
						case 0x0003:
							  MenuMain();
                break;
						
            }
        }


        if (cmd_page_id == PAGE_WIFI_SETTING) //页面ID   Page 3 WIFI 设置
        {
            switch (cmd_ctrol_id)
            {
            case 0x0005:
                Send_GetText(0x0001);
//             Send_GetText(0x0002);
                CMD_HEAD = NOTIFY_HEAD_RES;
                break;
						case 0x0006:
							  Send_SetPage(PAGE_SETTING_MENU, 0); //回到设置菜单
                break;
            }
        }

        if (cmd_page_id == PAGE_RESET_DEVICE) //页面ID   Page 4 重置设备
        {
            switch (cmd_ctrol_id) //控件ID
            {
            case 0x0002:  //返回按键
                MenuMain();
                break;

            }
        }

        if (cmd_page_id == PAGE_BOOT_START)  // Page 5 开机动画开始页
        {
            switch (cmd_ctrol_id) //控件ID
            {
            case 0x0002:

                break;
            }
        }

        if (cmd_page_id == PAGE_BOOT_MAIN) //页面ID  Page 6
        {
            switch (cmd_ctrol_id) //控件ID
            {

            case 0x0009:
                Send_SetPage(PAGE_MENU, 0);//菜单
                break;

            }
        }

        if (cmd_page_id == PAGE_FW_UPGRADE) //页面ID  Page 7
        {
            switch (cmd_ctrol_id) //控件ID
            {
							
                 
            case 0x0003:
							  Send_SetPage(PAGE_FW_STATUS, 0);  
                OTA_Event_Handle(); //选择升级，出现固件包后跳转到Page 10
                break;
						

            }
        }

        if (cmd_page_id == PAGE_FW_UPDATING) //页面ID  Page 8 固件升级过程
        {
            switch (cmd_ctrol_id) //控件ID
            {
            case 0x0003:   //退出
                MenuMain();
                break;

            }
        }

        if (cmd_page_id == PAGE_FW_STATUS) //页面ID  Page 9 获取固件状态
        {
            switch (cmd_ctrol_id) //控件ID
            {
            case 0x0003:   //退出
                MenuMain();
                break;
						

            }
        }

        if (cmd_page_id == PAGE_FW_READY) //页面ID  Page 10 成功获取固件
        {
            switch (cmd_ctrol_id) //控件ID
            {
            case 0x0002:   //确认固件升级
							  Send_SetPage(PAGE_FW_UPDATING, 0); 
                OTA_Event();
                break;

            }
        }

        if (cmd_page_id == PAGE_JUMP_TRANSIT) //页面ID  Page 11
        {
            switch (cmd_ctrol_id) //控件ID
            {
            case 0x0003:   //返回菜单
                MenuMain();
                break;

            }
        }

        if (cmd_page_id == PAGE_JUMP_APP_CONFIRM) //页面ID  Page 12
        {
            switch (cmd_ctrol_id) //控件ID
            {
            case 0x0003:   //APP确认跳转
                Jump_App_Handle();
                break;

            }
        }

        if (cmd_page_id == PAGE_UPDATE_CONFIRM) //页面ID  Page 13  更新点数确认界面
        {
            switch (cmd_ctrol_id) //控件ID
            {
            case 0x0003:
                MenuMain();//返回菜单
                break;

            }
        }

        if (cmd_page_id == PAGE_SETTING_MENU) //页面ID  Page 14
        {
            switch (cmd_ctrol_id) //控件ID
            {
							case 0x0001:  
                Send_SetPage(PAGE_RESET_FACTORY, 0);
                break;
							case 0x0003:  //返回按键
                MenuMain();
                break;
            case 0x0006:
                Send_SetPage(PAGE_BACKLIGHT, 0);  //背光设置

                break;
						case 0x0007:
                Send_SetPage(PAGE_WIFI_SETTING, 0); 

                break;
            case 0x000E:
                Send_SetPage(PAGE_SETTING_2, 0);  //设置菜单页面2

                break;


            case 0x0000B:    //蜂鸣器
                EflashReadPage(DATA_AREA_ADDR, 512, (u32*)eflashBuffer);
                if (BuzzFlag == 0)
                {
                    Send_SetBeep(Buzz);
                    BuzzFlag = 1;
                    eflashBuffer[0x20] = BuzzFlag;
                    EflashWritePageHandle(DATA_AREA_ADDR, 512, eflashBuffer);
                    Send_Save();
                }
                else if (BuzzFlag == 1)
                {
                    Send_SetBeep(0);
                    BuzzFlag = 0;
                    eflashBuffer[0x20] = BuzzFlag;
                    EflashWritePageHandle(DATA_AREA_ADDR, 512, eflashBuffer);
                    Send_Save();
                }

                break;

            }
        }

        if (cmd_page_id == PAGE_JUMP_APP) //页面ID  Page 15
        {
            switch (cmd_ctrol_id) //控件ID
            {
            case 0x0003:
                MenuMain();//返回

                break;

            }
        }

        if (cmd_page_id == PAGE_BACKLIGHT) //页面ID  Page 16
        {
            switch (cmd_ctrol_id) //控件ID
            {
                EflashReadPage(DATA_AREA_ADDR, 512, (u32*)eflashBuffer);
                light = eflashBuffer[0x30];
            case 0x0001:
                if (light >= 0xFA) //背光增加
                {
                    light = 0xFA;
                }
                else if (light < 0xFA)
                {
                    light = light + 0x0A;
                }
                Send_SetBL(light);
                eflashBuffer[0x30] = light;
                eflashBuffer[0x31] = 0;
                EflashWritePageHandle(DATA_AREA_ADDR, 512, eflashBuffer);
                break;
            case 0x0004:

                if (light <= 10) //背光减少
                {
                    light = 10;
                }
                else if (light > 10)
                {
                    light = light - 0x0A;
                }
                Send_SetBL(light);
                eflashBuffer[0x30] = light;
                eflashBuffer[0x31] = 0;
                EflashWritePageHandle(DATA_AREA_ADDR, 512, eflashBuffer);

                break;

            }
        }

        if (cmd_page_id == PAGE_SETTING_2 ) //页面ID  Page 32
        {
            u8 i ;
            u8 disp;
					 EflashReadPage(DATA_AREA_ADDR, 512, (u32*)eflashBuffer);
            switch (cmd_ctrol_id) //控件ID
            {
            case 0x0001:
                Send_SetPage(PAGE_SETTING_MENU, 0);  //返回设置菜单1
                break;
            case 0x0002:
                Send_SetPage(PAGE_SENSITIVITY, 0);  //跳转灵敏度菜单
						    Timer0DelayMs(30);
                if (eflashBuffer[0x80 + 4] == 1)
                {
                    for (i = 0; i < 5; i++)
                    {
                        if (LEVEL_cnt == levels[i])
                        {
                            disp = '1' + i;
                            Send_SetText(0x000B, &disp, 1);
                            break;
                        }
                    }
                }
                else
                {
                    disp = '3';
                    Send_SetText(0x000B, &disp, 1);
									
                    break;
                }
                break;
								
								case 0x0005:
                Send_SetPage(PAGE_BUZZER , 0);  //跳转APP蜂鸣器设置
								Timer0DelayMs(30);
								 if (eflashBuffer[0xA1] == 1)
                {
									BeepFlag = eflashBuffer[0xA0];
									disp = 0x30 + BeepFlag;
									Send_SetText(0x000B, &disp, 1);
                }
								else
								{
									disp = '3';
									Send_SetText(0x000B, &disp, 1);
                }
                break;
            }
        }

        if (cmd_page_id == PAGE_SENSITIVITY)           //页面ID  Page 33   灵敏度设置菜单
        {
            u8 i;
            u8 disp;
            EflashReadPage(DATA_AREA_ADDR, 512, (u32*)eflashBuffer);
            switch (cmd_ctrol_id) //控件ID
            {
            case 0x0001:
                Send_SetPage(PAGE_SETTING_2, 0);  //返回设置菜单2
                break;
            
						}
            if (cmd_ctrol_id >= 0x0002 && cmd_ctrol_id <= 0x0006)
            {
                i = cmd_ctrol_id - 0x0002;
                LEVEL_cnt = levels[i];
                read_val = levels[i];
                disp = '1' + i;
                Send_SetText(0x000B, &disp, 1);
                eflashBuffer[0x80 + 0] = (read_val >> 0)  & 0xFF;   // 第0字节
                eflashBuffer[0x80 + 1] = (read_val >> 8)  & 0xFF;   // 第1字节
                eflashBuffer[0x80 + 2] = (read_val >> 16) & 0xFF;   // 第2字节
                eflashBuffer[0x80 + 3] = (read_val >> 24) & 0xFF;   // 第3字节
                eflashBuffer[0x80 + 4] = 1;                    //读写标志
                EflashWritePageHandle(DATA_AREA_ADDR, 512, (uint8_t *)eflashBuffer);
            }
        
			}
			
			
        if (cmd_page_id == PAGE_BUZZER) //页面ID  Page 34
        {
				  	u8 i;
            u8 disp;
            switch (cmd_ctrol_id) //控件ID
            {
            case 0x0001:
                Send_SetPage(PAGE_SETTING_2, 0);  //返回设置菜单2
						    break;
						}
						
						if (cmd_ctrol_id >= 0x0002 && cmd_ctrol_id <= 0x0004)
            {
							i = cmd_ctrol_id - 0x0002;
							disp = '1' + i;
							Send_SetText(0x000B, &disp, 1);
							eflashBuffer[0xA0] = i + 1;
							eflashBuffer[0xA1] = 1;                         //读写标志
							EflashWritePageHandle(DATA_AREA_ADDR, 512, (uint8_t *)eflashBuffer);
							
							Beeptest();
						}
           
				}

				
				

        break;
    default:
        break;
    }
}
//回发指令处理
void Process_Postback(PCTRL_MSG msg, uint16 size)  //回发指令处理
{
    u8 status;
//     char page_buf[64] = {0};
    u8 eflashBuffer[512];
    u8* p_t;
    cmd_type_sub = Get_Buff_8(cmd_type_pos + 1); //获取从指令类型
    cmd_ctrol_id = Get_Buff_16(cmd_type_pos + 2); //获取控件ID
    p_t = msg->param;
    p_t += cmd_type_pos + 3; //文本内容位置

    //=====================================
    switch (cmd_type) //指令类型
    {
    case 0x06:  //获取页面ID
        Postback_Get_PageID();
        break;

    case 0x0008:         //背光
        CMD_HEAD = 0xCC;
        break;//0x13






    case 0x82:  //主指令码为0x82
        switch (cmd_type_sub) //从指令码
        {
        case 0x05:  //获取控件的文本内容
            switch (cmd_ctrol_id) //控件ID
            {
            case 0x0001:  //控件ID
                strcpy(account, (char*)p_t);
                CMD_HEAD = 0xCC;
                Send_GetText(0x0002);
                CMD_HEAD = NOTIFY_HEAD_RES;
                break;
            case 0x0002:  //控件ID
                strcpy(password, (char*)p_t);
                EflashReadPage(DATA_AREA_ADDR, 512, (uint32_t *)eflashBuffer);
                strncpy((char *)(eflashBuffer + 0x40), account, 32);
                strncpy((char *)(eflashBuffer + 0x60), password, 32);
                EflashWritePageHandle(DATA_AREA_ADDR, 512, (uint8_t *)eflashBuffer);

                wifijump();//跳转
                WIFIconnect();

                CMD_HEAD = 0xCC;
                break;

            case 0x0004:
                strcpy(Resetdevice, (char*)p_t);

                Send_SetPage(PAGE_RESET_DEVICE, 0);

                if (memcmp(UserPassword, Resetdevice, 4) == 0)
                {
                     Factory_Reset_Event_Handle();
								}
                else
                {
                    Send_SetText(0x0001, text_pwd_error, sizeof(text_pwd_error));
                    Timer0DelayMs(30);
                    Send_SetText(0x0004, text_pwd_error_en, sizeof(text_pwd_error_en));
                }
                CMD_HEAD = 0xCC;
                break;

            }
            break;  //0x05 END
        default:
            break;
        }
        break;  //0x82 END
    default:
        break;
    }

    //回发指令处理完，帧头改为触摸通知
//     CMD_HEAD = 0xCC;
}



void LcdMainpage(void)          //主页面显示
{
    Send_SetPage(PAGE_BOOT_MAIN, 0); //跳转页面
    Timer0DelayMs(20);
    Send_SetText(0x0006, (u8*)GetDeviceName(), strlen(GetDeviceName()));
    Timer0DelayMs(30);
    Send_SetText(0x0007, (u8*)DEVICE_VERSION, sizeof(DEVICE_VERSION));
    Timer0DelayMs(30);
    Send_SetText(0x0008, (u8*)firmware_info.local_firmware_version, strlen(firmware_info.local_firmware_version));
}



void LcdMainpage1(void) //上电开机使用
{

    Timer0DelayMs(4000);
    Send_SetPage(PAGE_BOOT_MAIN, 0); //跳转页面
    Timer0DelayMs(20);
    Send_SetText(0x0006, (u8*)GetDeviceName(), strlen(GetDeviceName()));
    Timer0DelayMs(20);
    Send_SetText(0x0006, (u8*)GetDeviceName(), strlen(GetDeviceName()));
    Timer0DelayMs(30);
    Send_SetText(0x0007, (u8*)DEVICE_VERSION, sizeof(DEVICE_VERSION));
    Timer0DelayMs(30);
    Send_SetText(0x0008, (u8*)firmware_info.local_firmware_version, strlen(firmware_info.local_firmware_version));
}


void LcdMainpage0(void) 
{
	 u8 eflashBuffer[512];
	 u8 APP_Flag;
	 EflashReadPage(APPTOIAP_ADDR, 512, (u32*)eflashBuffer);
	 APP_Flag = eflashBuffer[0];
	 if (APP_Flag == 0x88)
	 {
		 LcdMainpage();
		 EflashEraseArea(APPTOIAP_ADDR, 512);
   }
	 else
	 {
		 LcdMainpage1();
   }
}



void MenuMain(void)
{
    Send_SetPage(PAGE_MENU , 0);
}

void OTAfail(void)
{
    Send_SetPage(PAGE_FW_STATUS, 2);
}


void OTAUPDATA(void)
{
    Send_SetPage(PAGE_FW_READY, 0);
}

void wifijump(void)
{
    Send_SetPage(PAGE_JUMP_TRANSIT, 2);
}

