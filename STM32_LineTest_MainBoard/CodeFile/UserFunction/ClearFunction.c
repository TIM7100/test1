#include "Timer0.h"
#include "usart_lcd.h"
#include "ClearFunction.h"
#include "CommonFunction.h"
#include "commandlist_NPG83_10P.h"
#include "MainSlaveInterface.h"

void ModifClearAcl16Dfn8(u8 ShowFlag)
{
    int slavenum;
    u8 ClearResult[10];
    u8 SendDataBuf[256];
    u16 SendDataLen;

    TimerDelayMS(200);

    for (slavenum = 0; slavenum < 10; slavenum++)
    {
        SendDataLen = 0;    //总发送长度，包括命令位与2Bytes CRC校验位
        SendCommandData(slavenum, REMOVE_SECURITY_CODE, SendDataBuf, SendDataLen);

    }

    TimerDelayMS(2000);

    memset_data(ClearResult, 0, 10);   //清空接收小板返回值的数组

    GetReplyResult(ClearResult);			

		ShowResult_10pcs_r(ClearResult);
}



//清除函数接口
void ClearSecurityCodeInterface(u8 SreccnShow)
{
    u16 PressKeyData;

    while (1)
    {
        PressKeyData =  0x0000;
        PressKeyData = AutoKeyScan();

        if (PressKeyData == Key_Enter)
        {
            ResetSlaveChip();  //复位小板
            delay_ms(100); 

            UartSend("CLR(0);\r\n");
            CheckBusy();

            UartSend(T_DC32(112, 40, "正在执行解锁命令", 15));
            CheckBusy();
						UartSend(T_DC32(130, 100, MenuPoint[UserChoose].DisplayString, 5));
            CheckBusy();

            UartSend(T_DC32(144, 160, "解锁中...", 4));
            CheckBusy();

            ModifClearAcl16Dfn8(SreccnShow); 

        }
        else if (PressKeyData == Key_Esc)          /* 退出底层用户界面，然后返回显示目录 */
        {
            UserChoose = 0;
            DisplayStart = 0;
            break;
        }
    }
}
