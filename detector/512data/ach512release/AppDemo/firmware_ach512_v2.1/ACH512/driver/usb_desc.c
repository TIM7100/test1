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

//ums relative descriptor
const USB_DEVICE_DESCRIPTOR device_descr_ums =
{
	0x12,
	USB_DEVICE_DESCRIPTOR_TYPE, //bDescriptorType: 0x01
	SWAP(0x0200),               //bcdUSB: usb 2.0

	0,                          //bDeviceClass: independent interfaces
	0,                          //bDeviceSubClass: 0
	0,                          //bDeviceProtocol: class specific protocols NOT used on device basis
	EP0_MAX_PACKET_SIZE,        //bMaxPacketSize0: maximum packet size for endpoint zero
	0xabcd,	                    //idVendor: 0xabcd
	0x1001,	                    //idProduct
	0x0100,	                    //bcdDevice
	INDEX_MANUFACTURER,        	//iManufacturer: index of string
	INDEX_PRODUCT,              //iProduct: index of manufacturer string
	0,                          //iSerialNumber: index of product string
	0x01                        //bNumConfigurations: 1 configuration
};

UINT8 config_descr_ums[32] =
{
	0x09, USB_CONFIGURATION_DESCRIPTOR_TYPE, 0x20, 0x00, 0x01, 0x01, 0x00, 0x80, 0x64,                                              //config descriptor
	0x09, USB_INTERFACE_DESCRIPTOR_TYPE, 0x00, 0x00, 0x02, 0x08, 0x06, 0x50, 0x00,                                                  //interface descriptor
	0x07, USB_ENDPOINT_DESCRIPTOR_TYPE, (EP_DIR_IN | USB_EP1), 0x02, EPX_MAX_PACKET_SIZE_FS&0xFF, EPX_MAX_PACKET_SIZE_FS>>8, 0x00,  //endpoint in descriptor
	0x07, USB_ENDPOINT_DESCRIPTOR_TYPE, (EP_DIR_OUT | USB_EP1), 0x02, EPX_MAX_PACKET_SIZE_FS&0xFF, EPX_MAX_PACKET_SIZE_FS>>8, 0x00, //endpoint out descriptor
};

const UINT8 StrDesc_LanguageID_ums[4] = 
{
	4,				                   //bLength: 0x04 bytes
	USB_STRING_DESCRIPTOR_TYPE,		   //bDescriptorType: 0x03
	0x09,			                   //Language ID LSB
	0x04			                   //Language ID	
};

const UINT8 StrDesc_Manufacturer_ums[22] =
{
	22,				                   //bLength: 22 bytes
	USB_STRING_DESCRIPTOR_TYPE,		   //bDescriptorType: 0x03
	'A',	0,
	'i',	0,
	's',	0,
	'i',	0,
	'n',	0,
	'o',	0,
	'C',	0,
	'h',	0,
	'i',	0,
	'p',	0
};

const UINT8 StrDesc_Product_ums[30] =
{
	30,				                   //bLength: 30 bytes
	USB_STRING_DESCRIPTOR_TYPE,		   //bDescriptorType: 0x03
	'U',	0,
	'S',	0,
	'B',	0,
	' ',	0,
	'F',	0,
	'l',	0,
	'a',	0,
	's',	0,
	'h',	0,
	' ',	0,
	'D',	0,
	'i',	0,
	's',	0,
	'k',	0
};

const UINT8 StrDesc_SerialNumber_ums[34] =
{
	34,			                       //bLength: 34 bytes
	USB_STRING_DESCRIPTOR_TYPE,		   //bDescriptorType: 0x03
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
	'1',	0,
	'2',	0
};

//aisino relative descriptor
const USB_DEVICE_DESCRIPTOR device_descr_aisino =
{
	0x12,
	USB_DEVICE_DESCRIPTOR_TYPE,	//bDescriptorType: DEVICE
	0x0200,	                    //bcdUSB: usb 2.0
	
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

UINT8 config_descr_aisino[32] =
{
	0x09, USB_CONFIGURATION_DESCRIPTOR_TYPE, 0x20, 0x00, 0x01, 0x01, 0x00, 0x80, 0xFA,                                                                         //config descriptor
	0x09, USB_INTERFACE_DESCRIPTOR_TYPE, 0x00, 0x00, 0x02, USB_CLASS_CODE_TEST_CLASS_DEVICE, USB_SUBCLASS_CODE_TEST_CLASS, USB_PROTOCOL_CODE_TEST_CLASS, 0x00, //interface descriptor
	0x07, USB_ENDPOINT_DESCRIPTOR_TYPE, (EP_DIR_IN | USB_EP1), 0x02, EPX_MAX_PACKET_SIZE_FS&0xFF, EPX_MAX_PACKET_SIZE_FS>>8, 0x00,                             //endpoint in descriptor
	0x07, USB_ENDPOINT_DESCRIPTOR_TYPE, (EP_DIR_OUT | USB_EP1), 0x02, EPX_MAX_PACKET_SIZE_FS&0xFF, EPX_MAX_PACKET_SIZE_FS>>8, 0x00,                            //endpoint out descriptor
};

const UINT8 StrDesc_LanguageID_aisino[4] =
{
	4,				                   //bLength: 0x04 bytes
	USB_STRING_DESCRIPTOR_TYPE,		   //bDescriptorType: 0x03
	0x09,			                   //Language ID LSB
	0x04			                   //Language ID
};

const UINT8 StrDesc_Manufacturer_aisino[16] =
{
	16,                                //bLength: 16 bytes
	USB_STRING_DESCRIPTOR_TYPE,		   //bDescriptorType: 0x03
	'A',	0,
	'i',	0,
	's',	0,
	'i',	0,
	'n',	0,
	'o',	0,
	' ',	0,
};

const UINT8 StrDesc_Product_aisino[30] =
{
	30,				                   //bLength: 30 bytes
	USB_STRING_DESCRIPTOR_TYPE,		   //bDescriptorType: 0x03
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

const UINT8 StrDesc_SerialNumber_aisino[34]=
{
	34,			                       //bLength: 34 bytes
	USB_STRING_DESCRIPTOR_TYPE,		   //bDescriptorType: 0x03
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

const USB_DEVICE_QUALIFIER_DESCRIPTOR device_qualifier_descr =
{
	0x0a,
	USB_DEVICE_QUALIFIER_DESCRIPTOR_TYPE, //bDescriptorType: DEVICE
	SWAP(0x0200),               //bcdUSB: usb 2.0

	0,                          //Mass Storage Class
	0,                          //bDeviceSubClass: 0
	0,                          //bDeviceProtocol: class specific protocols NOT used on device basis
	EP0_MAX_PACKET_SIZE,        //bMaxPacketSize0: maximum packet size for endpoint zero
	0x01,                       //bNumConfigurations: 1 configuration
	0
};
