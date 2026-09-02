/**************************************************************************
Filename : UsbDevice.cpp
Language : Cpp
Description : implementation of the CUsbDevice class.
Author(s) :   
Company  : ANCH Ltd.
version  : 1.0
Change Log : 2007-3-3
******************************************************************************/

#include "stdafx.h"
#include "UsbDevice.h"

// for ums protocol
#include "publicstruct.h"
#include "CmdDll.h"
// end

// for aisinochip private protocol
#include "interface.h"
#include <winioctl.h>
#include <initguid.h>
// end
	
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// for ums protocol
// lun index
#define ND_IDX    0
static const BYTE RBC_DISK_PROPERTY[18] = {0x55,0x53,0x42,0x20,0x20,0x20,0x20,0x20,0x00,0x46,0x6C,0x61,0x73,0x68,0x44,0x69,0x73,0x6B}; // = {"USB FlashDisk"};
// end

// for aisinochip private protocol
#define AnchUSB_GUID \
	{ 0xAB8AF19D, 0xF042, 0x4A5C, { 0xBE, 0xD3, 0x82, 0x49, 0x10, 0x97, 0x65, 0x4B } }
HANDLE OpenByInterface(GUID* pClassGuid, DWORD instance, PDWORD pError);
GUID ClassGuid = AnchUSB_GUID;
// end

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUsbDevice::CUsbDevice()
{

}

CUsbDevice::~CUsbDevice()
{

}

// for ums protocol
/****************************************************************************
*
*    FUNCTION: GetDisksProperty(HANDLE hDevice, PSTORAGE_DEVICE_DESCRIPTOR pDevDesc)
*
*    PURPOSE:  get the info of specified device
*
****************************************************************************/
BOOL GetDisksProperty(HANDLE hDevice, PSTORAGE_DEVICE_DESCRIPTOR pDevDesc)
{
	STORAGE_PROPERTY_QUERY	Query;	// input param for query
	DWORD dwOutBytes;				// IOCTL output length
	BOOL bResult;					// IOCTL return val

	// specify the query type
	Query.PropertyId = StorageDeviceProperty;
	Query.QueryType = PropertyStandardQuery;

	// Query using IOCTL_STORAGE_QUERY_PROPERTY 
	bResult = DeviceIoControl(hDevice,			// device handle
			IOCTL_STORAGE_QUERY_PROPERTY,			// info of device property
			&Query, sizeof(STORAGE_PROPERTY_QUERY),	// input data buffer
			pDevDesc, pDevDesc->Size,				// output data buffer
			&dwOutBytes,							// out's length
			(LPOVERLAPPED)NULL);					

	return bResult;
}

HDISK FindDisk(BYTE disk_lun, PBYTE property_buffer, UINT property_len, char *DiskNUM)
{
	char strdisk[] = "C:\\";
	UINT uDiskType;
	char DiskName[30][4];
	int nDisk;
	HDISK m_hDisk;
	HANDLE hDevice;
	TCHAR szBuf[MAX_PATH+1];
	PSTORAGE_DEVICE_DESCRIPTOR pDevDesc;
	BYTE DevDesc[1024];

	memset(DiskName, 0, sizeof(DiskName));
	GetLogicalDriveStrings(30*4, (PCHAR)DiskName);	
	
	//枚举盘符
	for(nDisk = 0; DiskName[nDisk][0]; nDisk++) 
	{		 
		DiskName[nDisk][0] = UpChar(DiskName[nDisk][0]);
		strdisk[0] = DiskName[nDisk][0];
		uDiskType = GetDriveType(strdisk);		
		
		if(DRIVE_REMOVABLE == uDiskType) 
		{
			sprintf(szBuf, "\\\\.\\%c:", strdisk[0]);
			hDevice = CreateFile(szBuf,
				0,
				FILE_SHARE_READ | FILE_SHARE_WRITE, 
				NULL,
				OPEN_EXISTING, 
				0, 
				NULL);
			
			if(hDevice != INVALID_HANDLE_VALUE)
			{
				pDevDesc = (PSTORAGE_DEVICE_DESCRIPTOR)DevDesc;
				pDevDesc->Size = sizeof(DevDesc);

				if(GetDisksProperty(hDevice, pDevDesc))
				{
					if(pDevDesc->BusType == BusTypeUsb)
					{
						if (0 != memcmp(&DevDesc[pDevDesc->VendorIdOffset], property_buffer, property_len)) 
						{			       
							CloseHandle(hDevice);
							continue;
						}
						CloseHandle(hDevice);
						
						m_hDisk = InitUSBDiskByName(DiskName[nDisk][0]);
						if(NULL == m_hDisk)
						{
							continue;
						}
						*DiskNUM = DiskName[nDisk][0];						
						return m_hDisk;
					}
				}

				CloseHandle(hDevice);
			}
		}
	}
	
	return NULL;
}

BOOL Write_bulk( 
					HDISK   hDriver,           //驱动器句柄；
					PVOID   pCDBbuff,          //cdb命令缓冲区
					DWORD   length,            //要发送数据的长度
					PVOID   pInBuff,           //写入数据的缓冲区
					BYTE    FlashNo,           //物理读写时的FLASH 片数； 为0XFF则表示逻辑读写										  
					BYTE    btDisk 
				)
{
	if (NULL == hDriver)
		return FALSE;
	
	if (LOG == FlashNo)
	{
		return SendDeviceCmd(hDriver, pCDBbuff, 16, length, pInBuff, SCSI_IOCTL_DATA_OUT);
	}

	return TRUE;
}

BOOL Read_bulk ( 
					HDISK   hDriver,            //驱动器句柄
					PVOID   pCDBbuff,           //cdb命令缓冲区
					DWORD   length,             //要接受数据的长度
					PVOID   pOutBuff,           //接收数据的缓冲区
					BYTE    btFlashNo,          //物理读写时的FLASH 片数； 为0XFF则表示逻辑读写										  
					BYTE    btDisk
				)
{
	if (NULL == hDriver)
		return FALSE;
	
	if (LOG == btFlashNo)
	{
		return SendDeviceCmd(hDriver, pCDBbuff, 16, length, pOutBuff);
	}

	return TRUE;
}
// end

// for aisinochip private protocol
DWORD Endpoint1WritePipes(HANDLE hdev, UINT Length, void* pBuffer, UINT *nBytesWritten)
{
	DWORD	nOutput;	
	DWORD	error;
	if (!DeviceIoControl(hdev,
						 IOCTL_ANCHUSB_BULK_OUT,
						 pBuffer,
						 Length,
						 NULL,
						 0,
						 &nOutput,
						 NULL)){	
		error = GetLastError();
		return(error);
	}

	*nBytesWritten = nOutput;

	return ERROR_SUCCESS;
}


DWORD Endpoint1ReadPipes(HANDLE hdev, UINT Length, void* pBuffer, UINT *nBytesRead)
{
	ULONG	nOutput;
	DWORD	error;

	if (!DeviceIoControl(hdev,
						 IOCTL_ANCHUSB_BULK_IN,
						 NULL,
						 0,
						 pBuffer,
						 Length,
						 &nOutput,
						 NULL)){	
		error = GetLastError();
		return(error);
	}

	*nBytesRead = nOutput;

	return ERROR_SUCCESS;
}
// end

DWORD CUsbDevice::IniDevice(DWORD instance)
{
	DWORD dError;

	if((hDisk = FindDisk(ND_IDX, (PBYTE)RBC_DISK_PROPERTY, sizeof(RBC_DISK_PROPERTY), &DiskNum)) != NULL)
	{
		usb_protocol = USB_PROTOCOL_UMS;

		// Init SCSI operation code
		memset((BYTE*)&scsi_cmd, 0x00, sizeof(scsi_cmd));

		return ERROR_SUCCESS;
	}
	else
	{
		if((hDevice = OpenByInterface(&ClassGuid, instance, &dError)) != INVALID_HANDLE_VALUE)
		{
			usb_protocol = USB_PROTOCOL_AISINOCHIP;
			
			return ERROR_SUCCESS;
		}
	}

	return (-1);
}

DWORD CUsbDevice::CloseDevice()
{
	if(usb_protocol == USB_PROTOCOL_UMS)
	{
		if(hDisk != NULL)
		{
			ClosePhyDeviceHandle(hDisk);
			return ERROR_SUCCESS;
		}
		else
		{
			return ERROR_INVALID_HANDLE;
		}
	}
	else
	{
		if(!CloseHandle(hDevice))
		{
			return GetLastError();
		}
		else
		{
			return ERROR_SUCCESS;
		}
	}
}

DWORD CUsbDevice::WriteDevice(BYTE operation, void *pCmdBuf, UINT nBytesOutLength, void *pOutBuf, UINT *nBytesWritten)
{
	if(usb_protocol == USB_PROTOCOL_UMS)
	{
		scsi_cmd.OpCode = operation;
		memcpy((BYTE*)&scsi_cmd.crypt_req, pCmdBuf, sizeof(CRYPT_REQUEST));
		
		if(TRUE != Write_bulk(hDisk, &scsi_cmd, nBytesOutLength, pOutBuf, 0xff, 0))
		{
			DWORD dError = GetLastError();
			if(dError == ERROR_SUCCESS)
			{
				dError = USB_STATUS_ERROR;
			}
			return dError;
		}

		*nBytesWritten = nBytesOutLength;
	}
	else
	{
		memcpy(bSendBuffer, pCmdBuf, sizeof(CRYPT_REQUEST));
		memcpy(bSendBuffer+sizeof(CRYPT_REQUEST), pOutBuf, nBytesOutLength);
		if(ERROR_SUCCESS != Endpoint1WritePipes(hDevice, sizeof(CRYPT_REQUEST)+nBytesOutLength, bSendBuffer, nBytesWritten))
		{
			return GetLastError();
		}
	}

	return ERROR_SUCCESS;
}

DWORD CUsbDevice::ReadDevice(BYTE operation, void *pCmdBuf, UINT nBytesInLength, void *pInBuf, UINT *nBytesRead)
{
	if(usb_protocol == USB_PROTOCOL_UMS)
	{
		scsi_cmd.OpCode = operation;
		memcpy((BYTE*)&scsi_cmd.crypt_req, pCmdBuf, sizeof(CRYPT_REQUEST));

		if(TRUE != Read_bulk(hDisk, &scsi_cmd, nBytesInLength, pInBuf, 0xff, 0))
		{
			DWORD dError = GetLastError();
			if(dError == ERROR_SUCCESS)
			{
				dError = USB_STATUS_ERROR;
			}
			return dError;
		}

		*nBytesRead = nBytesInLength;
	}
	else
	{
		if(ERROR_SUCCESS != Endpoint1ReadPipes(hDevice, nBytesInLength, pInBuf, nBytesRead))
		{
			return GetLastError();
		}
		else if(*nBytesRead < nBytesInLength)
		{
			return USB_STATUS_ERROR;
		}
	}

	return ERROR_SUCCESS;
}
