/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : usb_desc.h
 * Description : usb driver header file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/

#ifndef __USBDESC_H__
#define __USBDESC_H__

#include  "common.h"

/**********************************************************
*	definitions
**********************************************************/

/**********************************************************
*	include files
**********************************************************/

/**********************************************************
*	structure
**********************************************************/
typedef struct _USB_DEVICE_DESCRIPTOR {
    UINT8 bLength;
    UINT8 bDescriptorType;
    UINT16 bcdUSB;
    UINT8 bDeviceClass;
    UINT8 bDeviceSubClass;
    UINT8 bDeviceProtocol;
    UINT8 bMaxPacketSize0;
    UINT16 idVendor;
    UINT16 idProduct;
    UINT16 bcdDevice;
    UINT8 iManufacturer;
    UINT8 iProduct;
    UINT8 iSerialNumber;
    UINT8 bNumConfigurations;
} USB_DEVICE_DESCRIPTOR;
typedef struct _USB_CONFIGURATION_DESCRIPTOR {
    UINT8 bLength;
    UINT8 bDescriptorType;
    UINT16 wTotalLength;
    UINT8 bNumInterfaces;
    UINT8 bConfigurationValue;
    UINT8 iConfiguration;
    UINT8 bmAttributes;
    UINT8 MaxPower;
} USB_CONFIGURATION_DESCRIPTOR, *PUSB_CONFIGURATION_DESCRIPTOR;

typedef struct _USB_INTERFACE_DESCRIPTOR {
    UINT8 bLength;
    UINT8 bDescriptorType;
    UINT8 bInterfaceNumber;
    UINT8 bAlternateSetting;
    UINT8 bNumEndpoints;
    UINT8 bInterfaceClass;
    UINT8 bInterfaceSubClass;
    UINT8 bInterfaceProtocol;
    UINT8 iInterface;
} USB_INTERFACE_DESCRIPTOR, *PUSB_INTERFACE_DESCRIPTOR;

typedef struct _USB_CCID_DESCRIPTOR {
	UINT8 bLength;			
	UINT8 bDescriptorType;		
	UINT16 bcdCCID;                 
	UINT8 bMaxSlotIndex;		
	UINT8 bVoltageSupport;		
	UINT8 dwProtocols0;
	UINT8 dwProtocols1;
	UINT8 dwProtocols2;
	UINT8 dwProtocols3;

	UINT8 dwDefaultClock0;
	UINT8 dwDefaultClock1;
	UINT8 dwDefaultClock2;
	UINT8 dwDefaultClock3;
			
	UINT8 dwMaximumClock0;
	UINT8 dwMaximumClock1;
	UINT8 dwMaximumClock2;
	UINT8 dwMaximumClock3;
			
	UINT8 bNumClockSupporte; 
	    
	UINT8 dwDataRate0;
	UINT8 dwDataRate1;
	UINT8 dwDataRate2;
	UINT8 dwDataRate3;	
		
	UINT8 dwMaxDataRate0;
	UINT8 dwMaxDataRate1;
	UINT8 dwMaxDataRate2;
	UINT8 dwMaxDataRate3;

	UINT8 bNumDataRatesSupp;
	      
	UINT32 dwMaxIFSD;	
	UINT32 dwSynchProtocols;
	UINT32 dwMechanical;		
	UINT32 dwFeatures;		
	UINT32 dwMaxCCIDMessageL;
	       
	UINT8 bClassGetResponse;       
	UINT8 bClassEnvelope;		
	UINT16 wLcdLayout;		
	UINT8 bPINSupport;		
	UINT8 bMaxCCIDBusySlots;  
} USB_CCID_DESCRIPTOR,*PUSB_CCID_DESCRIPTOR;

typedef struct _USB_ENDPOINT_DESCRIPTOR {
    UINT8 bLength;
    UINT8 bDescriptorType;
    UINT8 bEndpointAddress;
    UINT8 bmAttributes;
    UINT16 wMaxPacketSize;
    UINT8 bInterval;
} USB_ENDPOINT_DESCRIPTOR,*PUSB_ENDPOINT_DESCRIPTOR;

typedef struct USB_DEVICE_QUALIFIER_DESCRIPTOR {
    BYTE bLength;
    BYTE bDescriptorType;
    WORD bcdUSB;
    BYTE bDeviceClass;
    BYTE bDeviceSubClass;
    BYTE bDeviceProtocol;
    BYTE bMaxPacketSize0;
    BYTE bNumConfigurations;
    BYTE bReserved;
} USB_DEVICE_QUALIFIER_DESCRIPTOR, *PUSB_DEVICE_QUALIFIER_DESCRIPTOR;
// Done!

/*typedef struct __packed _USB_CONFIGURATION_DESCRIPTOR {
    UINT8 bLength;
    UINT8 bDescriptorType;
    UINT16 wTotalLength;
    UINT8 bNumInterfaces;
    UINT8 bConfigurationValue;
    UINT8 iConfiguration;
    UINT8 bmAttributes;
    UINT8 MaxPower;
} USB_CONFIGURATION_DESCRIPTOR;	 */

/*
typedef struct __packed _USB_INTERFACE_DESCRIPTOR {
    UINT8 bLength;
    UINT8 bDescriptorType;
    UINT8 bInterfaceNumber;
    UINT8 bAlternateSetting;
    UINT8 bNumEndpoints;
    UINT8 bInterfaceClass;
    UINT8 bInterfaceSubClass;
    UINT8 bInterfaceProtocol;
    UINT8 iInterface;
} USB_INTERFACE_DESCRIPTOR;

typedef struct __packed _USB_ENDPOINT_DESCRIPTOR {
    UINT8 bLength;
    UINT8 bDescriptorType;
    UINT8 bEndpointAddress;
    UINT8 bmAttributes;
    UINT16 wMaxPacketSize;
    UINT8 bInterval;
} USB_ENDPOINT_DESCRIPTOR; */


/**********************************************************
*	extern variables
**********************************************************/
//for ums protocol
extern const USB_DEVICE_DESCRIPTOR device_descr_ums;
extern UINT8 config_descr_ums[];
extern const UINT8 StrDesc_LanguageID_ums[];
extern const UINT8 StrDesc_Manufacturer_ums[];
extern const UINT8 StrDesc_Product_ums[];
extern const UINT8 StrDesc_SerialNumber_ums[];

//for aisinochip private protocol
extern const USB_DEVICE_DESCRIPTOR device_descr_aisino;
extern UINT8 config_descr_aisino[];
extern const UINT8 StrDesc_LanguageID_aisino[];
extern const UINT8 StrDesc_Manufacturer_aisino[];
extern const UINT8 StrDesc_Product_aisino[];
extern const UINT8 StrDesc_SerialNumber_aisino[];

extern const USB_DEVICE_QUALIFIER_DESCRIPTOR device_qualifier_descr;

#endif

