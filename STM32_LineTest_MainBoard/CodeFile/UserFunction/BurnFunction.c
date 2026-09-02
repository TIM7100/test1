// #include "Timer0.h"
// #include "usart_lcd.h"
// #include "BurnFunction.h"
// #include "CommonFunction.h"
// #include "commandlist_NPG83_10P.h"
// #include "MainSlaveInterface.h"
// #include "rtc.h"
// #include "crc.h"
// #include "flash.h"
// #include "string.h"
// #include "user_OS.h"

// extern u8 SlaveVersion_Backdoor[16];
// extern u8 Main_Board_Version[16];
// extern u32 MainCount_Hex;           //主板烧录次数
// extern u32 MainCount_Hex_back;
// u32 MainCount_Hex_back;

// u8 MainCount[7] = {0x30, 0x30, 0x30, 0x30, 0x30, 0x30};

// // 后门数据    从机烧录个数   存放后门数据数组   烧录型号，     区域编号
// u16 BackdoorData(u8 slavenum, u8* SendDataBuf, u32 SNLen, u16 type_code, u8 area_code);

// //参数分别是              烧录型号，     烧录命令，     
// void BurnInterface( u16 type_code, u32 command_code, u8 AreaCode)  //ModifBurnInterface
// {
//     u16 keyDataBasic;

//     //AutoStatus = RELEASE;
//     while (1)
//     {
//         keyDataBasic =  0x0000;
//         keyDataBasic = AutoKeyScan();

//         if (keyDataBasic == Key_Enter)
//         {
//             ResetSlaveChip();    //复位小板
//             delay_ms(100);
//             WaitingBurnInterface();
//             BurnAcl16( type_code, command_code, AreaCode);  /* 烧录实现函数，传进参数分别为 flash的12位结构体，217系列的型号，烧录命令，惠普或非 */
//         }
//         else if (keyDataBasic == Key_Esc)          /* 返回按键 */
//         {
//             break;
//         }
//     }
// }


// //参数分别是          烧录型号，     烧录命令，    
// void BurnAcl16( u16 type_code, u32 command_code , u8 AreaCode)
// {
//     u8 slavenum;
//     u8 SendDataBuf[0xA00];
//     u8 BurnResult[10];
//     u8 LCD_maincount[6];
//     u8 BackdoorBuf[0x80];
//     u16 CalBackdoorCRC;
//     u32 SendDataLen;

//     if (MainCount_Hex >= 300000)
//     {
//         UartSend("CLR(0);\r\n");
//         CheckBusy();
//         UartSend(T_DC32(160, 160, "超过烧录计数", 1));
//         CheckBusy();
//         while (1);
//     }
// 		
//     RTC_Get();//更新时间
//     MainCount_Hex_back = MainCount_Hex;  //备份主板计数初值
// 		
//     for (slavenum = 0; slavenum < 10; slavenum++)
//     {
//         SendDataLen = 0;             //清0发送长度

//         transform_MainCount(MainCount_Hex, MainCount);
//         memcpy_data(SendDataBuf + SendDataLen, MainCount, 4); //十进制烧录总数（显示屏显示的总数）
//         SendDataLen += 4;

// 				memcpy_data(SendDataBuf + SendDataLen, &AreaCode, 1); //区域码
// 				SendDataLen += 1;
// 			
//         //-------------------------------
//         memset_data(BackdoorBuf, 0, 0x80);              //清零Backdoor数据buffer


//         transform_MainCount(MainCount_Hex, MainCount);  //  [13,16]
//         memcpy_data(BackdoorBuf + 0x0D, MainCount, 4); //   [13,16]十进制烧录总数（显示屏显示的总数）

//         BackdoorBuf[0x11] = (u8)(type_code >> 8);
//         BackdoorBuf[0x12] = (u8)(type_code);                       //型号代码typecode [17,18]

// 					//无型号序列号总数 

//         Tran_Time_To_Array(BackdoorBuf + 0x15);            //RTC时间 [21,39]

//         TranVersionToArray(BackdoorBuf + 0x28);     //主板版本号[40,55]

//         TranSlaveVersionToArray(BackdoorBuf + 0x38); //功能板版本号 [56,71]
// 				
// 				BackdoorBuf[0x48] = AreaCode;											//区域码[72]
// 						
// 				memset_data(BackdoorBuf + 0x49, 0xFF, 0x35);		//预留53位 [73,125]

//         CalBackdoorCRC = GetCrc16(BackdoorBuf, 0x7E);    

//         BackdoorBuf[0x7E] = (CalBackdoorCRC >> 8) & 0xFF;   //CRC  [126,127]
//         BackdoorBuf[0x7F] = CalBackdoorCRC & 0xFF;

//         //----------- 生成发送数组 结束------------------------------------

//         memcpy_data(SendDataBuf + SendDataLen, BackdoorBuf, 0x80);
//         SendDataLen += 0x80;

//         
// 				//发送烧录命令
//         SendCommandData(slavenum, command_code, SendDataBuf, SendDataLen);

//         MainCount_Hex++;
//     }

//     memset_data(BurnResult, 0, 10);     //清空接收小板返回值的数组

//     MainCount_Hex = MainCount_Hex_back;  //大板烧录总数

//     WaitingSurfaceBurn(3);              //烧录等待界面

//     GetReplyResult(BurnResult);

//     Train_MainCount_to_Array(LCD_maincount);

//     //更新Count的值
//     MainCount_Hex += 10;    //烧录次数值
//     Write_Flash_u32(0x20000, &MainCount_Hex, 1);

// 		ShowResult_10pcs_r(BurnResult);
// 		

// }


