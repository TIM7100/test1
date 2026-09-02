#include "use_lcd.h"
#include "esp8266_config.h"
#include "w5500_user_conf.h"
#include "lcd_show.h"
#include "ota_firmware_update.h"
#include "cloud_platform_para.h"
#include "UserFunctionInterface.h"
#include "ota_firmware_handle.h"

extern firmware_info_t firmware_info;

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
char user_name_buf[32] = {0}; // 存放用户名文本
char user_name_buf1[32] = {0}; // 存放用户名文本
__IO u8 NetWork_Flag = ESP8266;

void User_App(void)
{
    qsize size = 0;
    size = queue_find_cmd(cmd_buffer, CMD_MAX_SIZE, CMD_HEAD); //从缓冲区中获
    if (size > 0) //接收到指令
    {
        cmd_head = cmd_buffer[0]; //获取数据头
        cmd_type = Get_Buff_8(cmd_type_pos);   //获取指令类型


        switch (cmd_head)
        {

        case NOTIFY_HEAD_TOUCH: //触摸指令头
            Process_Touch((PCTRL_MSG)cmd_buffer, size);  //触摸指令处理
            break;
        case NOTIFY_HEAD_RES:  //回发指令头
            Process_Postback((PCTRL_MSG)cmd_buffer, size);   //回发指令处理
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
    u8 status;
    char buf[64];
    cmd_page_id = Get_Buff_16(cmd_type_pos + 1); //获取页面ID
    cmd_ctrol_id = Get_Buff_16(cmd_type_pos + 3); //获取控件ID
    cmd_ctrol_value = Get_Buff_16(cmd_type_pos + 9); //获取控件值

//==========================================
    switch (cmd_type) //指令类型
    {
    case 0x06:  //按钮控件
        if (cmd_page_id == 0x0000) //页面ID
        {
            switch (cmd_ctrol_id) //控件ID
            {
            case 0x0001:
                OTA_Event_Handle();

                break;
            case 0x0002:
                Jump_App_Handle();
                break;
            case 0x0003:

                break;
            case 0x0004:


                break;


            }
        }

        if (cmd_page_id == 0x0001) //页面ID   重置设备
        {
            switch (cmd_ctrol_id) //控件ID
            {
            case 0x0002:

                NetWork_Flag = W5500;

                Send_SetText(0x0001, textConnecting, sizeof(textConnecting));

                status = Factory_Reset_Event_Handle();
                if (status == 0)
                {
                    Send_SetText(0x0001, textRestoreFactoryOk, sizeof(textRestoreFactoryOk));
                }
                else
                {
                    Send_SetText(0x0001, textNetFailed, sizeof(textNetFailed));
                }
                break;

            case 0x0004:
                NetWork_Flag = ESP8266;
                Factory_Reset_Event_Handle();
                break;
            }
        }
        if (cmd_page_id == 0x0003) //页面ID
        {
        case 0x0005:
//          Send_GetText(0x0001);
            Send_GetText(0x0002);
            CMD_HEAD = NOTIFY_HEAD_RES;
            break;
        }

        if (cmd_page_id == 0x0005)  //更新点数
        {
            switch (cmd_ctrol_id) //控件ID
            {
            case 0x0002:
                NetWork_Flag = W5500;

                Send_SetText(0x0001, textConnecting, sizeof(textConnecting));

                status = CountEventHandle();
                if (status == SHADOW_SET_SUCCESS)
                {
                    Send_SetText(0x0001, textUpdatePointOk, sizeof(textUpdatePointOk));
                }
                else
                {

//                     sprintf(buf, "%s%d", textUpdatePointFail, status);
//                     Send_SetText(0x0001, (u8*)buf, strlen(buf));
                    Send_SetText(0x0001, textUpdatePointFail, sizeof(textUpdatePointFail));
                }

                break;
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
    case 0x82:  //主指令码为0x82
        switch (cmd_type_sub) //从指令码
        {
        case 0x05:  //获取控件的文本内容
            switch (cmd_ctrol_id) //控件ID
            {
            case 0x0001:  //控件ID
                strcpy(user_name_buf, (char*)p_t);


                break;
            case 0x0002:  //控件ID
                strcpy(user_name_buf1, (char*)p_t);


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
    CMD_HEAD = 0xCC;
}

void LcdMainpage(void)          //主页面显示
{
    Send_SetPage(6, 0); //跳转页面
    Timer0DelayMs(10);
    Send_SetText(0x0006, (u8*)GetDeviceName(), sizeof((u8*)GetDeviceName() + 3));
    Timer0DelayMs(10);
    Send_SetText(0x0007, DEVICE_VERSION, sizeof(DEVICE_VERSION));
    Timer0DelayMs(10);
    Send_SetText(0x0008, (u8*)firmware_info.local_firmware_version, sizeof((u8*)firmware_info.local_firmware_version));
}
