#include "Timer0.h"
#include "usart_lcd.h"
#include "CheckFunction.h"
#include "CommonFunction.h"
#include "commandlist_HP388.h"
#include "MainSlaveInterface.h"

extern u8 MainCount[7];

void CheckChipInterface(TypeInfrom Type_sel, u32 command_code, u8 EEDataSum)
{
    u16 keyDataBasic;

    //AutoStatus = RELEASE;
    while (1)
    {
        keyDataBasic =  0x0000;
        keyDataBasic = AutoKeyScan();

        if (keyDataBasic == Key_Enter)
        {
            ResetSlaveChip();    //复位小板
            delay_ms(100);
            ModifCheckAcl16(Type_sel, command_code, EEDataSum);
        }
        else if (keyDataBasic == Key_Esc)
        {
            break;
        }
    }
}

void ModifCheckAcl16(TypeInfrom Type_sel, u32 command_code, u8 EEDataSum)
{
    u8 slavenum;
    u8 SendDataBuf[0x100];
    u8 CheckResult[50];
    u16 SendDataLen;

    memset_data(SendDataBuf, 0, 0x100);
    for (slavenum = 0; slavenum < 50; slavenum++)
    {
        SendCommandData(slavenum, command_code, SendDataBuf, SendDataLen);
    }

    delay_ms(10);

    WaittingCheckTime(EEDataSum);

    /* 接收检测结果 */
    memset_data(CheckResult, 0, 50);

    GetReplyResult(CheckResult);

    ShowResult_50pcs(CheckResult);
}
