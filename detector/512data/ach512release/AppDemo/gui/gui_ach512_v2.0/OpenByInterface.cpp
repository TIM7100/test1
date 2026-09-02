/**************************************************************************
Filename : OpenByIntf.cpp
Language : Cpp
Description : open device by device interface.
Author(s) :   
Company  : ANCH Ltd.
version  : 1.0
Change Log : 2007-3-2 
******************************************************************************/


#include "stdafx.h"
#define NOCRYPT			// prevent attempt to include missing files
#define _INC_EXCPT		// prevent excpt.h from being included

#include <setupapi.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
// OpenByInterface
//
// Opens the nth device found with the given interface class

HANDLE OpenByInterface(
		GUID* pClassGuid,	// points to the GUID that identifies the interface class
		DWORD instance,		// specifies which instance of the enumerated devices to open
		PDWORD pError		// address of variable to receive error status
		)
{
	HANDLE hDev;
	HDEVINFO hDevInfoSet;
	BOOL status;
	SP_DEVICE_INTERFACE_DATA ifdata;
	PSP_DEVICE_INTERFACE_DETAIL_DATA pDetail;
	DWORD ReqLen;

	hDevInfoSet = SetupDiGetClassDevs(
					pClassGuid, 							// class GUID 
					NULL,									// 无关键字 
					NULL,									// 不指定父窗口句柄 
					DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);	// 目前存在的设备

	// 失败...
	if(hDevInfoSet == INVALID_HANDLE_VALUE)
	{
		*pError = GetLastError();
		return INVALID_HANDLE_VALUE;
	}

	ifdata.cbSize=sizeof(ifdata);
	status = SetupDiEnumDeviceInterfaces(
				hDevInfoSet,			// 设备信息集句柄
				NULL,					// 不需额外的设备描述
				pClassGuid,				// GUID
				instance,				// 设备信息集里的设备序号
				&ifdata);				// 设备接口信息
	if (!status)
	{
		*pError = GetLastError();
		return INVALID_HANDLE_VALUE;
	}

	status = SetupDiGetDeviceInterfaceDetail(
				hDevInfoSet,			// 设备信息集句柄
				&ifdata,				// 设备接口信息
				NULL,					// 设备接口细节(设备路径)
				0,						// 输出缓冲区大小
				&ReqLen,				// 输出缓冲区大小
				NULL);					// 不需额外的设备描述
	if (!status)
	{
		*pError = GetLastError();
		if (*pError != ERROR_INSUFFICIENT_BUFFER)
			return INVALID_HANDLE_VALUE;
	}
				
	pDetail = PSP_INTERFACE_DEVICE_DETAIL_DATA(new char[ReqLen]);
	pDetail->cbSize = sizeof SP_INTERFACE_DEVICE_DETAIL_DATA;

	status = SetupDiGetInterfaceDeviceDetail (
				hDevInfoSet,			// 设备信息集句柄
				&ifdata,				// 设备接口信息
				pDetail,				// 设备接口细节(设备路径)
				ReqLen,					// 输出缓冲区大小
				&ReqLen,				// 输出缓冲区大小
				NULL);					// 不需额外的设备描述
	if (!status)
	{
		*pError = GetLastError();
		return INVALID_HANDLE_VALUE;
	}

	hDev = CreateFile(
			pDetail->DevicePath,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

	if (hDev == INVALID_HANDLE_VALUE)
		*pError = GetLastError();

	return hDev;
}