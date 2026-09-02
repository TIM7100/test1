// UsbDevice.h: interface for the CUsbDevice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USBDEVICE_H__EF977808_91B8_4C1D_A72B_47470B327FFC__INCLUDED_)
#define AFX_USBDEVICE_H__EF977808_91B8_4C1D_A72B_47470B327FFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// for ums protocol
#include "Function.H"
// end

#include "crypt.h"

#define USB_PROTOCOL_UMS           0x01
#define USB_PROTOCOL_AISINOCHIP    0x02

// for ums protocol
#pragma pack(1)    // 1 bytes¶ÔÆë
typedef struct _scsi_cmd
{
	BYTE OpCode;
	CRYPT_REQUEST crypt_req;
	BYTE reserved[7];
} SCSI_CMD;
#pragma pack()
// end

class CUsbDevice  
{
public:
	CUsbDevice();

	virtual ~CUsbDevice();

	DWORD IniDevice(DWORD instance = 0);

	DWORD CloseDevice();

	DWORD WriteDevice(BYTE operation, void *pCmdBuf, UINT nBytesOutLength, void *pOutBuf, UINT *nBytesWritten);

	DWORD ReadDevice(BYTE operation, void *pCmdBuf, UINT nBytesInLength, void *pInBuf, UINT *nBytesRead);	

protected:
	BYTE bRecvBuffer[8192];
	BYTE bSendBuffer[8192];

	BYTE usb_protocol;
	
	// for ums protocol
	HDISK hDisk;
	char DiskNum;
	SCSI_CMD scsi_cmd;
	// end

	// for aisinochip private protocol
	HANDLE hDevice;
	// end
};

// for ums protocol

#ifndef IOCTL_STORAGE_QUERY_PROPERTY
// IOCTL control code
#define IOCTL_STORAGE_QUERY_PROPERTY   CTL_CODE(IOCTL_STORAGE_BASE, 0x0500, METHOD_BUFFERED, FILE_ANY_ACCESS)

//// The following structures all can find at MSDN.
// enumeration type specifies the various types of storage buses
typedef enum _STORAGE_BUS_TYPE {
    BusTypeUnknown = 0x00,
    BusTypeScsi,
    BusTypeAtapi,
    BusTypeAta,
    BusType1394,
    BusTypeSsa,
    BusTypeFibre,
    BusTypeUsb,
    BusTypeRAID,
    BusTypeMaxReserved = 0x7F
} STORAGE_BUS_TYPE, *PSTORAGE_BUS_TYPE;

// retrieve the storage device descriptor data for a device. 
typedef struct _STORAGE_DEVICE_DESCRIPTOR {
  ULONG  Version;
  ULONG  Size;
  UCHAR  DeviceType;
  UCHAR  DeviceTypeModifier;
  BOOLEAN  RemovableMedia;
  BOOLEAN  CommandQueueing;
  ULONG  VendorIdOffset;
  ULONG  ProductIdOffset;
  ULONG  ProductRevisionOffset;
  ULONG  SerialNumberOffset;
  STORAGE_BUS_TYPE  BusType;
  ULONG  RawPropertiesLength;
  UCHAR  RawDeviceProperties[1];

} STORAGE_DEVICE_DESCRIPTOR, *PSTORAGE_DEVICE_DESCRIPTOR;

// retrieve the properties of a storage device or adapter. 
typedef enum _STORAGE_QUERY_TYPE {
  PropertyStandardQuery = 0,
  PropertyExistsQuery,
  PropertyMaskQuery,
  PropertyQueryMaxDefined

} STORAGE_QUERY_TYPE, *PSTORAGE_QUERY_TYPE;

// retrieve the properties of a storage device or adapter. 
typedef enum _STORAGE_PROPERTY_ID {
  StorageDeviceProperty = 0,
  StorageAdapterProperty,
  StorageDeviceIdProperty

} STORAGE_PROPERTY_ID, *PSTORAGE_PROPERTY_ID;

// retrieve the properties of a storage device or adapter. 
typedef struct _STORAGE_PROPERTY_QUERY {
  STORAGE_PROPERTY_ID  PropertyId;
  STORAGE_QUERY_TYPE  QueryType;
  UCHAR  AdditionalParameters[1];

} STORAGE_PROPERTY_QUERY, *PSTORAGE_PROPERTY_QUERY;
#endif

// end

#endif // !defined(AFX_USBDEVICE_H__EF977808_91B8_4C1D_A72B_47470B327FFC__INCLUDED_)
