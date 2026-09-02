#include "Timer0.h"
#include "usart_lcd.h"
#include "CheckFunction.h"
#include "CommonFunction.h"
#include "commandlist_NPG83_10P.h"
#include "MainSlaveInterface.h"

extern u8 MainCount[7];

//参数分别是             检测命令
void CheckChipInterface(u32 command_code)
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
            WaitingCheckInterface(); //提前进入等待检测界面
            CheckAcl16(command_code);
        }
        else if (keyDataBasic == Key_Esc)          /* 返回按键 */
        {
            break;
        }
    }
}


//参数分别是          检测命令
void CheckAcl16(u32 command_code)
{
    u8 slavenum;
    u16 SendDataLen;
    u8 SendDataBuf[0xA00];
    u8 CheckResult[40];

    for (slavenum = 0; slavenum < 40; slavenum++)
    {
        SendDataLen = 0;     
        SendCommandData(slavenum, command_code, SendDataBuf, SendDataLen);
    }

    WaitingSurfaceCheck(2, 0);

    /* 接收检测结果 */
    memset_data(CheckResult, 0, 40);

    GetReplyResult(CheckResult);  //获取功能板返回状态

    ShowResult_40pcs_r(CheckResult);
}
