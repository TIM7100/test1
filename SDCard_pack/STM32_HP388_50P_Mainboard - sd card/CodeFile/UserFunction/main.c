#include "user_OS.h"
#include "user_system_init.h"
#include "crc.h"
#include "MainSlaveInterface.h"
#include "SD.h"
#include "aes.h"

const u8 Main_Board_Version[17] = {"2412100955HP388M"}; 

u8 Rebuf[256];
u8 Slave_Burn_Times;


u8 TEMP[1024];
	u8 OUTput[1024];


void AEStest(void)
{
	SD_Initialize();
	SD_ReadDisk(TEMP, 0, 2);
	
	AES128_CBC_Decrypt(OUTput, TEMP,1024, AES_Key, AES_Iv);
	
}


int main(void)
{
    SystemDrvInit();
	
	
//     SystemInformationInit();
// 	
// 	  ShowSystemStartupInformation();
  	AEStest();


    /* 主循环,每检测到key按下循环一次，相当于是事件触发 */
    while (1)
    {
		    SystemInformationInit();
//         OperatingSystem();
    }
}

