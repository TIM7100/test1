/***********************************************************************
 * Copyright (c)  2009,Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : usb.c
 * Description : usb driver
 * Author(s)   : eric   
 * version     : 1.0
 * Modify date : 2012-02-02
 ***********************************************************************/

#include  "common.h"
#include  "usb.h"


static UINT16 ep1_stall[2]= {0};    // EP1 stall状态
static UINT16 ep2_stall[2]= {0};    // EP2 stall状态
static UINT16 ep3_stall[2]= {0};    // EP3 stall状态
static UINT16 ep4_stall[2]= {0};    // EP3 stall状态

volatile UINT8 flag_clear_stall=0;  
UINT8 speed_mode=0;

//ep_index表示端点编号
void ep0_send_empty_packet(void)
{
	REG_USBC_Eindex=0;
	REG_USBC_E0CSR |= 0x4A;	// clear RxPktRdy, set DataEnd and TxPktRdy
	//while( !(REG_USBC_IntrTx& 0x01) );	
	while( REG_USBC_E0CSR& 0x02 );
}


UINT16 usb_get_fifo_length(UINT8 ep_index)
{
    REG_USBC_Eindex=ep_index;	
	if(ep_index==0)	 
	{
		return	REG_USBC_E0COUNTR;
	}
	else
	{		 
		return	REG_USBC_RXCOUNT;
	}	
}

void usb_clear_fifo(UINT8 ep_index, UINT8 ep_dir)
{
    REG_USBC_Eindex=ep_index;	
    if(ep_index==0)	 //EPO 功能暂不支持
	{
	}
	else
	{
		if(ep_dir == EP_DIR_IN)	 REG_USBC_TxCSR |= (1<<3);
		else 					 REG_USBC_RxCSR |= (1<<4);		
	}	
}

  
void usb_ep0_send_stall(void)
{
	REG_USBC_Eindex=0;	
	REG_USBC_E0CSR |= 1<<5;
	while(REG_USBC_E0CSR&0x20);
	REG_USBC_E0CSR &= ~(1<<2);  //clear stall status
}

UINT16 get_stall_status(UINT8 ep_index, UINT8 ep_dir)
{
	switch(ep_index)
	{
		case USB_EP1: 
		{    
			if(ep_dir == EP_DIR_IN)		return ep1_stall[0];	  //in
			else			  			return ep1_stall[1];	  //out	    
		}
		case USB_EP2: 
		{    
			if(ep_dir == EP_DIR_IN)		return ep2_stall[0];	  //in
			else			  			return ep2_stall[1];	  //out	    
		}
		case USB_EP3: 
		{    
			if(ep_dir == EP_DIR_IN)	  	return ep3_stall[0];	  //in
			else			  			return ep3_stall[1];	  //out	    
		}
		case USB_EP4: 
		{    
			if(ep_dir == EP_DIR_IN)	  	return ep4_stall[0];	  //in
			else			  			return ep4_stall[1];	  //out	    
		}

		default:    	 	  return 0xff;
	}	
}

void usb_clear_stall(UINT8 ep_index, UINT8 ep_dir)
{
	if(ep_index==0)	return;

	REG_USBC_Eindex=ep_index;	
	if(ep_dir == EP_DIR_IN)	  	
	{
		ep1_stall[0]=0x0000;	  		//in	
		REG_USBC_TxCSR |= 1<<6;         //clear toggle
		REG_USBC_TxCSR &= ~(1<<4);		//clear stall
	}
	else			  			
	{
		ep1_stall[1]=0x0000;	  		//out	 
		REG_USBC_RxCSR |= 1<<7;   		//clear toggle 
		REG_USBC_RxCSR &= ~(1<<5);     	//clear stall
	}
	
	flag_clear_stall=0;  			 
}



void usb_send_stall(UINT8 ep_index, UINT8 ep_dir)
{
	REG_USBC_Eindex=ep_index;	
	 
	if(ep_dir == EP_DIR_IN)	 	
	{
		ep1_stall[0]=0x0001;	  	//in
		REG_USBC_TxCSR |= 1<<4;    	//set stall
	}
	else			  			
	{
		ep1_stall[1]=0x0001;	  	//out	
		REG_USBC_RxCSR |= 1<<5;    	//set stall
	}
}


#ifndef ROM_DRIVER_USB

//读端点FIFO, ep_index表示端点编号
void usb_read_ep_fifo(UINT8 *dst,UINT32 length,UINT8 ep_index)
{
	UINT32 i;
	
	for(i=0;i<length;i++)  
	{
		*dst++ = REG_USBC_FIFO_ENTRY(ep_index);
	}	 

}

//写端点FIFO, ep_index表示端点编号
void usb_write_ep_fifo(UINT8 * src, UINT32 length, UINT8 ep_index)
{
	UINT32 i;

	for(i=0;i<length;i++)  
	{
		REG_USBC_FIFO_ENTRY(ep_index) = *src++;
	} 

}

//ep_index表示端点编号
void start_ep_transfer(UINT32 length,UINT8 ep_index)
{
	REG_USBC_Eindex=ep_index;
 	if(ep_index==USB_EP0)
	{		
      	REG_USBC_E0CSR |= 0x02;	   //set the TxPktRdy 
      	while( REG_USBC_E0CSR & 0x02 );
	}
    else
	{
		REG_USBC_TX_PTR = length;
		REG_USBC_TxCSR |=0x01;  //set the TxPktRdy 
		while(REG_USBC_TxCSR &0x01);	
	}
}

void usb_send_data(UINT8 *buffer,UINT32 length,UINT8 ep_index)
{
	UINT16 max_packet_size;  

   	if(ep_index==USB_EP0)
	{
		max_packet_size = EP0_MAX_PACKET_SIZE;	
	}
	else
	{
		if (speed_mode)	max_packet_size = EPX_MAX_PACKET_SIZE_HS;
		else 			max_packet_size = EPX_MAX_PACKET_SIZE_FS;		
	}

	while(length>=max_packet_size)
	{    
		usb_write_ep_fifo(buffer,max_packet_size, ep_index);  		   
		start_ep_transfer(max_packet_size, ep_index);
	 	length -= max_packet_size;
	  	buffer += max_packet_size;
	}
	if(length>0)  
	{	
		usb_write_ep_fifo(buffer,length, ep_index);  
		start_ep_transfer(length,ep_index);
	}
}

void usb_receive_data(UINT8 *buffer,UINT32 length,UINT8 ep_index)  
{
	UINT32 len;

 	REG_USBC_Eindex=ep_index; 

	if(ep_index == USB_EP0)
	{
		while(length>0)	
		{	
			while( !(REG_USBC_E0CSR & 0x01) );  
			
			len	= REG_USBC_E0COUNTR;	
			usb_read_ep_fifo(buffer, len,ep_index);	
			
			REG_USBC_E0CSR |= 0x40;   //clear RxReady
			
			length -= len;
			buffer += len;	
			
		}
		return;
	}

	while(length>0)	
	{	
		REG_USBC_RxCSR &= (~0x01);   
		while( !(REG_USBC_RxCSR & 0x01) );  
	
		len	=REG_USBC_RXCOUNT;	
		usb_read_ep_fifo(buffer, len,ep_index);	
		length -= len;
		buffer += len;
	}	
	
//	REG_USBC_RxCSR &= (~0x01);     //所有数据接收结束后，再释放   
} 

//注意：该函数针对端点EP1/2/3/4 的大批量数据传输,动态fifo模式，必须保证buffer起始地址8字节对齐
void usb_send_data_dynamic(UINT8 *buffer,UINT32 length,UINT8 ep_index)	
{
	UINT32 backup_TXFIFOADD;
	UINT16 max_packet_size;  

	if (speed_mode)	max_packet_size = EPX_MAX_PACKET_SIZE_HS;
	else 			max_packet_size = EPX_MAX_PACKET_SIZE_FS;

   	REG_USBC_Eindex=ep_index; 
	backup_TXFIFOADD = REG_USBC_TxFIFO_ADD;  

	while(length>=max_packet_size)
	{    
		REG_USBC_TxFIFO_ADD = (UINT32)buffer>>3;    	   
		start_ep_transfer(max_packet_size, ep_index);
	 	length -= max_packet_size;
	  	buffer += max_packet_size;
	}
	if(length>0)  
	{
	 	REG_USBC_TxFIFO_ADD = (UINT32)buffer>>3; 		     
		start_ep_transfer(length,ep_index);
	}

	REG_USBC_TxFIFO_ADD = backup_TXFIFOADD;   
}

//注意：该函数针对端点EP1/2/3/4 的大批量数据传输,动态fifo模式，必须保证buffer起始地址8字节对齐
void usb_receive_data_dynamic(UINT8 *buffer,UINT32 length,UINT8 ep_index) 
{
	UINT32 backup_RXFIFOADD, backup_RXPSZR, backup_RXFIFOSIZE;
	UINT32 max_packet_size,packet_num,operation_size;  
	UINT32 len;

   	if (speed_mode)	
	{
		max_packet_size = EPX_MAX_PACKET_SIZE_HS;
		packet_num = USB_PACKET_NUM_HS;
		operation_size = EPX_MAX_PACKET_SIZE_HS * USB_PACKET_NUM_HS;
	}
	else 
	{
		max_packet_size = EPX_MAX_PACKET_SIZE_FS;
		packet_num = USB_PACKET_NUM_FS;
		operation_size = EPX_MAX_PACKET_SIZE_FS * USB_PACKET_NUM_FS ;
	}	

 	REG_USBC_Eindex = ep_index; 
	backup_RXFIFOADD = REG_USBC_RxFIFO_ADD;  
	backup_RXPSZR = REG_USBC_RXPSZR;
	backup_RXFIFOSIZE = REG_USBC_RxFIFO_SIZE;	
	REG_USBC_RxFIFO_SIZE = USB_FIFO_SIZE;  

	while( length>= operation_size )
	{
		REG_USBC_RxFIFO_ADD = (UINT32)buffer>>3;   
		REG_USBC_RXPSZR = max_packet_size|((packet_num-1)<<11);	
		REG_USBC_RxCSR &= (~0x01);   
		while( !(REG_USBC_RxCSR & 0x01) );  
		length -= operation_size;
		buffer += operation_size;
	}

	while(length>0)	
	{
		REG_USBC_RxFIFO_ADD = (UINT32)buffer>>3;   	
		REG_USBC_RXPSZR = max_packet_size;		//一次一个packet
		REG_USBC_RxCSR &= (~0x01);   
		while( !(REG_USBC_RxCSR & 0x01) );  

		len	=REG_USBC_RXCOUNT;	
		length -= len;
		buffer += len;
	}	
	REG_USBC_RxFIFO_ADD = backup_RXFIFOADD;  
	REG_USBC_RXPSZR = backup_RXPSZR;
	REG_USBC_RxFIFO_SIZE = backup_RXFIFOSIZE;

//	REG_USBC_RxCSR &= (~0x01);     //所有数据接收结束后，再释放
} 


#endif


