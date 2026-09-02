#ifndef             PUBLICSTRUCT_H
#define             PUBLICSTRUCT_H

#include <Windows.H>
#include <WINDOWS.H>
#include <WINUSER.H>
#include <WINBASE.H>
#include <WINIOCTL.H>
#include <STDDEF.H>

#define    OUT
#define    IN



typedef          void         *HDISK;



typedef struct  DEVINFO_T
{
	PVOID				m_pDev;                
	HANDLE				m_Handle;              
	char				m_cVidName[8+1];
	char				m_cPidName[16+1];
	char				m_cFirmwareVer[4+1];
	char				m_cDriveLetter;       
	WORD				m_wVid;
	WORD				m_wPid;
	DWORD				m_dwDeviceError;       
	BYTE				m_byDiskMode;	       
	DWORD               m_FlashNo;             
	DWORD               m_BlockOfFlash;            
	DWORD               m_SectorOfBlock;            
	DWORD               m_DiskMode;              
	BYTE                SenseBuf[32];          
	BYTE                m_Lib_Ver;               
	BYTE                reserved[128];
	bool                bActive;
	
} DEVINFO,*PDEVINFO;


struct ScsiInquiryResult
{
    unsigned char  deviceType    :5;
    unsigned char  reserved0     :3;
	
    unsigned char  reserved1     :7;
    unsigned char  rmb           :1;
	
    unsigned char  ansiVersion   :3;
    unsigned char  ecmaVersion   :3;
    unsigned char  isoVesion     :2;
	
    unsigned char  repDataFormat :4;
    unsigned char  reserved2     :4;
	
    unsigned char  additionalLen;
    unsigned char  reserved3[3];
	
    unsigned char  vendorInfo[8];        
    unsigned char  productID[16];        
    unsigned char  productRevisionlevel[4]; 
	
    unsigned char  vendorSpec[20];
    unsigned char  reserved4;
    unsigned char  vendorSpecPara[64];
};


typedef struct  PHYDEVINFO_T
{
	PVOID				m_pDev;                
	HANDLE				m_Handle;              
	char				m_cVidName[8+1];
	char				m_cPidName[16+1];
	char				m_cFirmwareVer[4+1];
	char				m_cDriveLetter;       
	WORD				m_wVid;
	WORD				m_wPid;
	DWORD				m_dwDeviceError;        
	BYTE				m_byDiskMode;	       
	DWORD               m_FlashNo;             
	DWORD               m_BlockOfFlash;            
	DWORD               m_ByteOfBlock;
	DWORD               m_DiskMode;               
	BYTE                SenseBuf[32];
	bool                bActive;
	BYTE                reserved[127];	
} PHYDEVINFO,*PPHYDEVINFO;

#endif 