/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : usb_desc.c
 * Description : usb driver source file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#include  "usb.h"
#include  "usb_app.h"
#include  "usb_desc.h"

USB_DEVICE_DESCRIPTOR device_descr=
{
	0x12,
	USB_DEVICE_DESCRIPTOR_TYPE,	//bDescriptorType: DEVICE
	0x0200,	                    //bcdUSB: usb 2.0

	//USB_CLASS_CODE_TEST_CLASS_DEVICE,		//bDeviceClass: independent interfaces
	0,
	0,				            //bDeviceSubClass: 0
	0,				            //bDeviceProtocol: class specific protocols NOT used on device basis
	EP0_MAX_PACKET_SIZE,	    //bMaxPacketSize0: maximum packet size for endpoint zero
	0x1234,	                    //idVendor	//0x101d
	0xABCD,	                    //idProduct
	0x0200,	                    //bcdDevice
	INDEX_MANUFACTURER,         //iManufacturer: index of string
	INDEX_PRODUCT,             	//iProduct: index of manufacturer string
	INDEX_SERIALNUMBER,         //iSerialNumber: index of product string
	0x01                        //bNumConfigurations: 1 configuration
};

USB_DEVICE_QUALIFIER_DESCRIPTOR device_qualifier_descr=
{
	0x0a,
	USB_DEVICE_QUALIFIER_DESCRIPTOR_TYPE,	//bDescriptorType: DEVICE
	SWAP(0x0200),	//bcdUSB: usb 2.0

	//USB_CLASS_CODE_TEST_CLASS_DEVICE,		//bDeviceClass: independent interfaces
	0,				// Mass Storage Class

	0,				//bDeviceSubClass: 0
	0,				//bDeviceProtocol: class specific protocols NOT used on device basis
	EP0_MAX_PACKET_SIZE,		//bMaxPacketSize0: maximum packet size for endpoint zero
	0x01,               //bNumConfigurations: 1 configuration
	0
};

UINT8 usb_descriptor_config[32]=
{
	0x09, 0x02, 0x20, 0x00, 0x01, 0x01, 0x00, 0x80, 0xfa,
  	//0x09, 0x02, 0x20, 0x00, 0x01, 0x01, 0x00, USB_REMOTE_WAKEUP, 0xfa,
  	0x09, 0x04, 0x00, 0x00, 0x02, USB_CLASS_CODE_TEST_CLASS_DEVICE, USB_SUBCLASS_CODE_TEST_CLASS, USB_PROTOCOL_CODE_TEST_CLASS, 0x00,
  	0x07, 0x05, (EP_DIR_IN+USB_EP_IN), 0x02, EPX_MAX_PACKET_SIZE_FS&0xff, EPX_MAX_PACKET_SIZE_FS>>8, 0x00,
  	0x07, 0x05, (EP_DIR_OUT+USB_EP_OUT), 0x02, EPX_MAX_PACKET_SIZE_FS&0xff, EPX_MAX_PACKET_SIZE_FS>>8, 0x00
};


UINT8 StrDesc_LanguageID[4] =
{
	4,				// Num bytes of this descriptor
	3,				// String descriptor
	0x09,			// Language ID LSB
	0x04			// Language ID
};

UINT8 StrDesc_Manufacturer[16]=
{
	16,				// Num bytes of this descriptor
	3,				// String descriptor
	'A',	0,
	'i',	0,
	's',	0,
	'i',	0,
	'n',	0,
	'o',	0,
	' ',	0,
};

UINT8 StrDesc_Product[30]=
{
	30,				// Num bytes of this descriptor
	3,				// String descriptor
	'U',	0,
	'S',	0,
	'B',	0,
	' ',	0,
	'D',	0,
	'e',	0,
	'v',	0,
	'i',	0,
	'c',	0,
	'e',	0,
	' ',	0,
	' ',	0,
	' ',	0,
	' ',	0
};

UINT8 StrDesc_SerialNumber[34]=
{
	34,			// Num bytes of this descriptor
	3,			// String descriptor
	'1',	0,
	'2',	0,
	'3',	0,
	'4',	0,
	'5',	0,
	'6',	0,
	'7',	0,
	'8',	0,
	'9',	0,
	'0',	0,
	'A',	0,
	'B',	0,
	'C',	0,
	'D',	0,
	'E',	0,
	'F',	0
};



