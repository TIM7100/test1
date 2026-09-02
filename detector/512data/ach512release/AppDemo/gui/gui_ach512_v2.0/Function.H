#ifndef             FUNCTION_H
#define             FUNCTION_H

#include <Windows.H>
#include <WINDOWS.H>
#include <WINUSER.H>
#include <WINBASE.H>
#include <WINIOCTL.H>
#include <STDDEF.H>


#define    OUT
#define    IN

//所有返回值定义为BOOL型的函数，返回TRUE表示功能执行成功，返回FLASE表示执行失败；
#define			 LOG  	                       0xff
//#define          PHY 	                       0

#define          SCSI_IOCTL_DATA_OUT		    0
#define  	     SCSI_IOCTL_DATA_IN				1

typedef          void         *HDISK;

extern "C" 
{
	void  __stdcall InitDisks(HDISK  phDisk[], int   InhDiskLength, PINT OuthDiskLength);
	HDISK __stdcall InitDiskByName(char  DriverName);
	void   __stdcall   CloseDevcHandle(HDISK   IN   hDriver);	
	BOOL    __stdcall SendDeviceCmd( 
		HDISK    IN     hDriver,        
		PVOID    IN     pCBD = NULL,	
		DWORD    IN     CBDLen = 0,					  
		DWORD    IN     DataTransferLength = 0, 
		PVOID    IN OUT pBuff = NULL,		  					  			
		BYTE     IN     DataIn = SCSI_IOCTL_DATA_IN, 													  
		PDWORD   OUT    RetLength = NULL 
		);
}

#endif 