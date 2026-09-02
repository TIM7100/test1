/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : usb_app.c
 * Description : usb driver source file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#include  "usb.h"
#include  "usb_app.h"
#include  "usb_desc.h"
#include  "app.h"

static DEVICE_REQUEST dev_req;
static UINT8 config_value = 0;
static UINT8 AltSetting = 0;
static UINT16 usb_device_status = 0;  // 远程唤醒和自供电状态

volatile UINT8 flag_usb_int = 0;
volatile UINT8 flag_usb_ep1_int = 0;
volatile UINT8 flag_usb_ep2_int = 0;
volatile UINT8 flag_usb_ep3_int = 0;
volatile UINT8 flag_usb_ep4_int = 0;
UINT8 MaxLun = 0;

void USB_IRQHandler(void)
{
	UINT8	temp=0;
	UINT8 backup_index;

	backup_index = REG_USBC_Eindex;

	temp = REG_USBC_IntrUSB;

	if(temp&0x01)	  //suspend
	{		
		flag_usb_int |=USB_SUSPEND;
	}
	if(temp&0x02)	  //resume
	{
		flag_usb_int |=USB_RESUME;
	}
	if(temp&0x04)	  	//bus reset	  
	{		
		flag_usb_int |=USB_BUS_RESET;
	}	

    temp = REG_USBC_IntrTx;
	//printfS("IntrTx=0x%x", temp); //for test EP_IN int
	
	if(temp &0x01)	//EP0
	{
		flag_usb_int |=USB_EP0_SETUP_PACKET; 
	}
	else
   	{
		temp = REG_USBC_IntrRx;

		if(temp &0x02)	//EP1
		{   
			flag_usb_ep1_int = USB_EP1_OUT_PACKET;
		}  
		if(temp &0x04)	//EP2
		{   
			flag_usb_ep2_int = USB_EP2_OUT_PACKET;
		}  
		if(temp &0x08)	//EP3
		{   
			flag_usb_ep3_int = USB_EP3_OUT_PACKET;
		}      		
		if(temp &0x10)	//EP4
		{   
			flag_usb_ep4_int = USB_EP4_OUT_PACKET;
		}   
	}

	usb_monitor(); 

	REG_USBC_Eindex = backup_index;
}

/*----------------- 标准请求--------------------*/
void usb_get_status(void)
{  
	UINT8 recipient=0;
	UINT16 status=0;
	UINT8 bEpIndex=0;
	UINT8 bEpDir=0;
	UINT16 ep_status=0;

	printfS("get_status!\n");
   
	recipient = dev_req.bmRequestType & USB_RECIPIENT_MASK;
	if(recipient==USB_RECIPIENT_DEVICE)
	{
		usb_write_ep_fifo((UINT8 *)(&usb_device_status),2,USB_EP0);
		start_ep_transfer(2,USB_EP0);
		REG_USBC_E0CSR |= 0x08;	 //set DataEnd	
	}
	else if(recipient==USB_RECIPIENT_INTERFACE)
   	{
		usb_write_ep_fifo((UINT8 *)(&status),2,USB_EP0);
		start_ep_transfer(2,USB_EP0);
		REG_USBC_E0CSR |= 0x08;	 //set DataEnd	
   	}
   	else if(recipient==USB_RECIPIENT_ENDPOINT)
   	{
		bEpIndex = dev_req.wIndex & 0x0f;
		bEpDir = dev_req.wIndex & 0x80;
		ep_status=get_stall_status(bEpIndex, bEpDir);
	   	
		usb_write_ep_fifo((UINT8 *)(&ep_status),2,USB_EP0);
	   	start_ep_transfer(2,USB_EP0);
		REG_USBC_E0CSR |= 0x08;	 //set DataEnd	
   	}
   	else
      	usb_ep0_send_stall();	
}

void usb_clear_feature(void)
{
  
	UINT8 recipient=0;
	UINT8 bEpIndex=0;
	UINT8 bEpDir=0;

	printfS("clear_feature!\n");

	recipient = dev_req.bmRequestType & USB_RECIPIENT_MASK;
	if(recipient==USB_RECIPIENT_DEVICE)
	{
		if(dev_req.wValue==USB_FEATURE_REMOTE_WAKEUP)
		{
			usb_device_status &= (~USB_GETSTATUS_REMOTE_WAKEUP);
		}
		//ep0_send_empty_packet();
		REG_USBC_E0CSR |= 0x08;
	}
   	else if(recipient==USB_RECIPIENT_ENDPOINT)
   	{
		bEpIndex = dev_req.wIndex & 0x0f;
		bEpDir = dev_req.wIndex & 0x80;
    	usb_clear_stall(bEpIndex, bEpDir);
		//ep0_send_empty_packet();
		REG_USBC_E0CSR |= 0x08;	//set DataEnd bit

		flag_clear_stall=1;
   	}
   	else
		usb_ep0_send_stall();
}

void usb_set_feature(void)
{
	UINT8 recipient=0;
	UINT8 bEpIndex=0;
	UINT8 bEpDir=0;
  	UINT8 test_packet[53] = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
      0x00, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 
      0xAA, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 
      0xEE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xBF, 0xDF, 
      0xEF, 0xF7, 0xFB, 0xFD, 0xFC, 0x7E, 0xBF, 0xDF, 
      0xEF, 0xF7, 0xFB, 0xFD, 0x7E};	

	printfS("set_feature!\n");
   
	recipient = dev_req.bmRequestType & USB_RECIPIENT_MASK;
	if(recipient==USB_RECIPIENT_DEVICE)
	{
		if(dev_req.wValue==USB_FEATURE_REMOTE_WAKEUP)
		{  
       		usb_device_status |= USB_GETSTATUS_REMOTE_WAKEUP;
		}
	    if(dev_req.wValue==0x02) ////test mode for 2.0 high speed 
		{
         	switch(dev_req.wIndex>>8)
         	{
         	case 0x01:  //test_J
         		printfS("test_J \n");
         
         		REG_USBC_Testmode =0x02;
           		break;
       		case 0x02:  //test_K
       		 	printfS("test_K \n");
       		 	
         		REG_USBC_Testmode =0x4;
         		break;
       		case 0x03:  //test_SE0_NAK
				printfS("test_SE0_NAK \n");
      		 	       		 
         		REG_USBC_Testmode =0x1;
         		break;  
       		case 0x04:  //test_Packet
       		 	printfS("test_Packet \n");
         		
         		usb_write_ep_fifo(test_packet,53,USB_EP0);
				REG_USBC_E0CSR |= 0x02;	   //set the TxPktRdy 
      		 	
         		REG_USBC_Testmode =0x08;
         		break;  
       		 case 0x05:  //test_Force_HS_Enable
				printfS("test_Force_HS_Enable \n");
                		
         		REG_USBC_Testmode =0x10;
         		break;         		        		       		       
         	}
		}
		REG_USBC_E0CSR |= 0x08;	 //set DataEnd
	}
	else if(recipient==USB_RECIPIENT_ENDPOINT)
	{
		bEpIndex = dev_req.wIndex & 0x0f;
		bEpDir = dev_req.wIndex & 0x80;
    	usb_send_stall(bEpIndex, bEpDir);
		//ep0_send_empty_packet();
		REG_USBC_E0CSR |= 0x08;	 //set DataEnd
   	}
   	else
      	usb_ep0_send_stall();
}

void usb_set_address(void)
{
	printfS("set_address,addr=0x%x\n",dev_req.wValue);

	REG_USBC_FADDRR = dev_req.wValue ;

	REG_USBC_E0CSR |= 0x08;	//set DataEnd 	
	//ep0_send_empty_packet();

	printfS("REG_USB_ADDR=0x%x\n",REG_USBC_FADDRR);
}


void usb_get_configuration(void)
{
	printfS("get_configuration!\n");

	usb_send_data(&config_value,1,USB_EP0);

	REG_USBC_E0CSR |= 0x08;	 //set DataEnd
}

void usb_set_configuration(void)
{
	printfS("set_configuration!\n");

	if( (dev_req.wValue==0)||(dev_req.wValue==1) )
   	{
	  	config_value=dev_req.wValue;
		//ep0_send_empty_packet();
		REG_USBC_E0CSR |= 0x08;	 //set DataEnd
   	}
   	else
	  	usb_ep0_send_stall();	
}

void usb_get_interface(void)
{
	UINT8 inter_value=0;
	UINT8 recipient=0;

	printfS("get_interface!\n");

	recipient = dev_req.bmRequestType & USB_RECIPIENT_MASK;
	if(recipient==USB_RECIPIENT_INTERFACE)
	{
		inter_value=AltSetting+dev_req.wIndex;	 
		usb_send_data(&inter_value,1,USB_EP0);
		REG_USBC_E0CSR |= 0x08;	 //set DataEnd	
	}
	else	
		usb_ep0_send_stall();

}

void usb_set_interface(void)
{
	UINT8  alt,inter_num;
	UINT8 recipient=0;

	printfS("set_interface!\n");

	recipient = dev_req.bmRequestType & USB_RECIPIENT_MASK;
	if(recipient==USB_RECIPIENT_INTERFACE)
	{
		inter_num = usb_descriptor_config[11]; //bInterfaceNumber
		alt = usb_descriptor_config[12]; //bAlternateSetting
	
		if( (dev_req.wIndex==inter_num)&&(dev_req.wValue==alt))
		{
		 	AltSetting=alt;	
			//ep0_send_empty_packet();
			REG_USBC_E0CSR |= 0x08;	 //set DataEnd	
		}
		else
			usb_ep0_send_stall();

	}
	else	
		usb_ep0_send_stall();
    
}

void usb_get_description(void)
{
	UINT16 wValue;
	UINT16 wLength;
	UINT8 wValue_bIndex;

	printfS("get_description->\n");

	wValue = (dev_req.wValue >> 8);
	wValue_bIndex = dev_req.wValue & 0xff;
	wLength = dev_req.wLength;

	if(wValue == USB_DEVICE_DESCRIPTOR_TYPE)
	{
		printfS("DEV_DESCRIPTOR!\n");

		if(wLength > device_descr.bLength)
		{
			wLength = device_descr.bLength;
		}
		usb_send_data((UINT8 *)(&device_descr), wLength, USB_EP0);
		REG_USBC_E0CSR |= 0x08;  //set DataEnd
	}

	else if(wValue == USB_DEVICE_QUALIFIER_DESCRIPTOR_TYPE)
	{
		printfS("QUALIFIER_DESCRIPTOR!\n");

		if(wLength > device_qualifier_descr.bLength)
		{
			wLength = device_qualifier_descr.bLength;
		}
		usb_send_data((UINT8 *)(&device_qualifier_descr), wLength, USB_EP0);
		REG_USBC_E0CSR |= 0x08;  //set DataEnd
	}

	else if(wValue == USB_CONFIGURATION_DESCRIPTOR_TYPE)
	{
		printfS("CONFIG_DESCRIPTOR!\n");

		if(wLength > sizeof(usb_descriptor_config))
		{
			wLength = sizeof(usb_descriptor_config);
		}

		if(speed_mode)
		{
			usb_descriptor_config[22] = EPX_MAX_PACKET_SIZE_HS & 0xff;
			usb_descriptor_config[23] = EPX_MAX_PACKET_SIZE_HS >> 8;
			usb_descriptor_config[29] = EPX_MAX_PACKET_SIZE_HS & 0xff;
			usb_descriptor_config[30] = EPX_MAX_PACKET_SIZE_HS >> 8;
		}
		else
		{
			usb_descriptor_config[22] = EPX_MAX_PACKET_SIZE_FS & 0xff;
			usb_descriptor_config[23] = EPX_MAX_PACKET_SIZE_FS >> 8;
			usb_descriptor_config[29] = EPX_MAX_PACKET_SIZE_FS & 0xff;
			usb_descriptor_config[30] = EPX_MAX_PACKET_SIZE_FS >> 8;
		}
		//usb_write_ep_fifo((UINT8 *)(&usb_descriptor_config), wLength, USB_EP0);
		//start_ep_transfer(wLength, USB_EP0);
		usb_send_data( (UINT8 *)(&usb_descriptor_config),wLength, USB_EP0 );
		REG_USBC_E0CSR |= 0x08;  //set DataEnd
	}
	else if(wValue == USB_STRING_DESCRIPTOR_TYPE)
	{
		if(wValue_bIndex == INDEX_LANGUAGE_ID)       //LANGID string
		{
			printfS("STRING_DESCRIPTOR__LANGID!\n");

			if(wLength > StrDesc_LanguageID[0])
			{
				wLength = StrDesc_LanguageID[0];
			}
			usb_send_data(StrDesc_LanguageID, wLength, USB_EP0);
			REG_USBC_E0CSR |= 0x08;  //set DataEnd
		}
		else if(wValue_bIndex == INDEX_MANUFACTURER) //vender string
		{
			printfS("STRING_DESCRIPTOR__vender!\n");

			if(wLength > StrDesc_Manufacturer[0])
			{
				wLength = StrDesc_Manufacturer[0];
			}
			usb_send_data(StrDesc_Manufacturer, wLength, USB_EP0);
			REG_USBC_E0CSR |= 0x08;  //set DataEnd
		}
		else if(wValue_bIndex == INDEX_PRODUCT)      //product string
		{
			printfS("STRING_DESCRIPTOR__product!\n");

			if(wLength > StrDesc_Product[0])
			{
				wLength = StrDesc_Product[0];
			}
			usb_send_data(StrDesc_Product, wLength, USB_EP0);
			REG_USBC_E0CSR |= 0x08;  //set DataEnd
		}
		else if(wValue_bIndex == INDEX_SERIALNUMBER) //SerialNumber	string
		{
			printfS("STRING_DESCRIPTOR__SerialNumber!\n");

			if(wLength > StrDesc_SerialNumber[0])
			{
				wLength = StrDesc_SerialNumber[0];
			}
			usb_send_data(StrDesc_SerialNumber, wLength, USB_EP0);
			REG_USBC_E0CSR |= 0x08;  //set DataEnd
		}
		else
		{
			usb_ep0_send_stall();
		}
	}
	else
	{
		usb_ep0_send_stall();
	}
}

void reserved(void)
{
	printfS("reserved!\n");

	usb_ep0_send_stall();
}



/*************************************************************************
 * USB标准设备请求入口地址指针表
 * 请求代码值由4个bit位组成，共16种可能，所以有16个元素
 *************************************************************************/
void (*StandardDeviceRequest[])(void) =
{
	usb_get_status,         //0x00
	usb_clear_feature,      //0x01
	reserved,
	usb_set_feature,        //0x03
	reserved,
	usb_set_address,        //0x05
	usb_get_description,    //0x06
	reserved,
	usb_get_configuration,  //0x08
	usb_set_configuration,  //0x09
	usb_get_interface,      //0x0a
	usb_set_interface,      //0x0b
	reserved,
	reserved,
	reserved,
	reserved
};


void usb_control_transfer(void)
{
	UINT8 type, req;

	REG_USBC_Eindex=0;

	printfS("REG_USBC_E0COUNTR=0x%x\n", REG_USBC_E0COUNTR);

	if(REG_USBC_E0COUNTR == 8)
	{
		usb_read_ep_fifo((UINT8 *)(&dev_req), 8, USB_EP0);
		REG_USBC_E0CSR |= 0x40;	//set the ServicedRxPktRdy
		
		type = dev_req.bmRequestType & USB_REQUEST_TYPE_MASK;

		if(type == USB_STANDARD_REQUEST)       //标准请求
		{
			printfS("standard request-->");

			req  = dev_req.bRequest & USB_REQUEST_MASK;
			(*StandardDeviceRequest[req])();
		}
		else usb_ep0_send_stall();
	}

}

void usb_bus_reset(void)
{
	REG_USBC_INTRTXE = 0x01;    //disable TX interrupt,enable EP0 interrupt
	//REG_USBC_INTRTXE = 0x1F;   	//enable TX int
	REG_USBC_INTRRXE = 0x1E;    //enable EP1/4 RX interrupt

	REG_USBC_Eindex = USB_EP_OUT;
	REG_USBC_RxCSR |= 0x1000;       //close NYET
	REG_USBC_RxFIFO_ADD = (UINT32)USBBUF_OUT >> 3;

	REG_USBC_Eindex = USB_EP_IN;
	//REG_USBC_TxCSR |= 1<<13;  //set the mode:dir IN
	REG_USBC_TxFIFO_ADD = (UINT32)USBBUF_IN >> 3;

	if(REG_USBC_UCSR & 0x10)
	{
		printfS("high speed\n");

		speed_mode = HIGH_SPEED;

		REG_USBC_Eindex = USB_EP_OUT;
		REG_USBC_RxFIFO_SIZE = 0x06;      //Rx FIFO size:512 byte
		REG_USBC_RXPSZR = EPX_MAX_PACKET_SIZE_HS;

		REG_USBC_Eindex = USB_EP_IN;
		REG_USBC_TxFIFO_SIZE = 0x06;      //Tx FIFO size:512 byte
		REG_USBC_TXPSZR = EPX_MAX_PACKET_SIZE_HS;
	}
	else
	{
		printfS("full speed\n");
		speed_mode = FULL_SPEED;

		REG_USBC_Eindex = USB_EP_OUT;
		REG_USBC_RxFIFO_SIZE = 0x03;      //Rx FIFO size:64byte
		REG_USBC_RXPSZR = EPX_MAX_PACKET_SIZE_FS;

		REG_USBC_Eindex = USB_EP_IN;
		REG_USBC_TxFIFO_SIZE = 0x03;   //Tx FIFO size:64byte
		REG_USBC_TXPSZR = EPX_MAX_PACKET_SIZE_FS;
	}

	REG_USBC_Eindex = 0;
}

void usb_resume(void)
{
	REG_USBC_UCSR &= ~0x01; // clear suspend mode enable bit
}

void usb_suspend(void)
{
	REG_USBC_UCSR |= 0x01; // set into suspend mode

	//low power
}

void usb_monitor(void)
{
	if((flag_usb_int & USB_BUS_RESET))  //bus reset
	{
		flag_usb_int &= (~USB_BUS_RESET);

		usb_bus_reset();

		printfS("bus reset \n");
	}

	if((flag_usb_int & USB_SUSPEND))    //suspend
	{
		flag_usb_int &= (~USB_SUSPEND);

		usb_suspend();

		printfS("suspend \n");
	}

	if((flag_usb_int & USB_RESUME))     //resume
	{
		flag_usb_int &= (~USB_RESUME);

		usb_resume();

		printfS("resume \n");
	}

	if((flag_usb_int & USB_EP0_SETUP_PACKET))   //EP0 setup packet received
	{
		printfS("setup packet received !\n");

		flag_usb_int &= (~USB_EP0_SETUP_PACKET);
		//REG_USBC_INTRTXE &= ~(1<<0); //disable EP0 interrupt
		usb_control_transfer();
		//REG_USBC_INTRTXE |= (1<<0); //enable EP0 interrupt

		printfS("---------control transfer end---------------------\n");
	}
}
void usb_transfer_monitor(void)
{
    if(flag_usb_ep1_int == USB_EP1_OUT_PACKET)  //EP1 OUT packet received
    {
        printfS("EP1 OUT packet received !\n");

        REG_USBC_Eindex = 1;
        out_ep_index = USB_EP1;
        in_ep_index = USB_EP1;

        USB_CMD_proc();

        printfS("---------EP1 transfer end---------------------\n");
    }

    if(flag_usb_ep2_int == USB_EP2_OUT_PACKET)  //EP2 OUT packet received
    {
        printfS("EP2 OUT packet received !\n");

        REG_USBC_Eindex = 2;
        out_ep_index = USB_EP2;
        in_ep_index = USB_EP2;

        USB_CMD_proc();

        printfS("---------EP2 transfer end---------------------\n");
    }

    if(flag_usb_ep3_int == USB_EP3_OUT_PACKET)  //EP3 OUT packet received
    {
        printfS("EP3 OUT packet received !\n");

        REG_USBC_Eindex = 3;
        out_ep_index = USB_EP3;
        in_ep_index = USB_EP3;

        USB_CMD_proc();

        printfS("---------EP3 transfer end---------------------\n");
    }

    if(flag_usb_ep4_int == USB_EP4_OUT_PACKET)  //EP4 OUT packet received
    {
        printfS("EP4 OUT packet received !\n");

        REG_USBC_Eindex = 4;
        out_ep_index = USB_EP4;
        in_ep_index = USB_EP4;

        USB_CMD_proc();

        printfS("---------EP4 transfer end---------------------\n");
    }
}

void usb_initial(void)
{
#ifdef LOW_POWER  
	enable_module(BIT_USB); //enable USB
#endif	
	
	NVIC_ClearPendingIRQ(USB_IRQn);
	//NVIC_SetPriority(USB_IRQn, 1);
	NVIC_EnableIRQ(USB_IRQn);
	
	REG_SCU_USBPHYCSR = (REG_SCU_USBPHYCSR&0xffffe1ff) | (0x01<<9); //提高眼图的幅值

	REG_USBC_UCSR &= (~0x40);   //disconnect usb
	delay(1000);
	
	REG_USBC_Testmode |=1<<7; //toggle error deal 

	REG_USBC_IntrUSBE = 0x07;   // enable Reset,Resume, Suspend interrupt
	REG_USBC_UCSR = 0x60;       //bit6 Soft Conn 5 HS enable
	//REG_USBC_UCSR = 0x40;		//bit6 Soft Conn 5 FS enable
}

void usb_remote_wakeup(void)
{
	REG_USBC_UCSR |= 1 << 2; // set into remote waleup mode
	delay(3870 * 8);     //2~15ms
	REG_USBC_UCSR &= ~(1 << 2); // set into remote waleup mode
}

