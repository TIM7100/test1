#include "Timer0.h"
#include "usart_lcd.h"
#include "BurnFunction.h"
#include "CheckFunction.h"
#include "CommonFunction.h"
#include "commandlist_HP388.h"
#include "MainSlaveInterface.h"
#include "rtc.h"
#include "flash.h"

extern u32 MainCount_Hex;
extern u32 MainCount_Hex_back;
u32 MainCount_Hex_back;

u8 MainCount[7] = {0x30, 0x30, 0x30, 0x31, 0x30, 0x30};

void ModifBurnInterface(TypeInfrom Type_sel, u16 type_code, u32 command_code, u8 EEDataSum)
{
    u16 keyDataBasic;

    while (1)
    {
        keyDataBasic =  0x0000;
        keyDataBasic = AutoKeyScan();

        if (keyDataBasic == Key_Enter)
        {
            ResetSlaveChip();    //复位小板
            delay_ms(100);
            ModifBurnAcl16(Type_sel, type_code, command_code, EEDataSum);
        }
        else if (keyDataBasic == Key_Esc)          /* 返回按键 */
        {
            break;
        }
    }
}

void ModifBurnAcl16(TypeInfrom Type_sel, u16 type_code, u32 command_code, u8 EEDataSum)
{
    u8 slavenum;
    u8 SendDataBuf[0x100];
    static u8 BurnResult[50];
    u16 SendDataLen = 0;

    MainCount_Hex_back = MainCount_Hex;  //备份主板计数初值

    for (slavenum = 0; slavenum < 50; slavenum++)
    {
        SendDataLen = 0;             //清0发送长度
        memset_data(SendDataBuf, 0, 0x100);

        transform_MainCount(MainCount_Hex, MainCount);//[0:3]
        memcpy_data(SendDataBuf + SendDataLen, MainCount, 4); //十进制烧录总数（显示屏显示的总数）
        SendDataLen += 4;

        SendDataBuf[4] = (u8)(type_code >> 8);
        SendDataBuf[5] = (u8)(type_code);
        memcpy_data(SendDataBuf + SendDataLen, MainCount, 2); //Typecode
        SendDataLen += 2;

        //发送烧录命令
        SendCommandData(slavenum, command_code, SendDataBuf, SendDataLen);
        MainCount_Hex++;
    }

    memset_data(BurnResult, 0, 50); //清空接收小板返回值的数组

    MainCount_Hex = MainCount_Hex_back;  //大板烧录总数

		WaittingBurnTime(EEDataSum);	//烧录等待时间
		
    GetReplyResult(BurnResult);

    //更新Count的值
    MainCount_Hex += 50;
    Write_Flash_u32(0x20000, &MainCount_Hex, 1);

    ShowResult_50pcs(BurnResult);
}
