/***********************************************************************
 * Filename    : hal_lpuart.c
 * Description : lpuart driver source file
 * Author(s)   : xwl  
 * version     : V1.0
 * Modify date : 2019-11-19
 ***********************************************************************/
#include "fxx_std.h"  

static uint16_t ep1_stall[2]= {0};    // EP1 stall״̬
static uint16_t ep2_stall[2]= {0};    // EP2 stall״̬
static uint16_t ep3_stall[2]= {0};    // EP3 stall״̬
static uint16_t ep4_stall[2]= {0};    // EP4 stall״̬     
     
/*============================================================================================
  * @brief  Config IO, enable USB PHY 
  * @param  none 
  * @retval None
==============================================================================================*/
uint32_t FSUSB_MSP_Init(void)  
{
    GPIO_InitTypeDef    GPIO_init_para;  

    System_Module_Reset(RST_USB); 
    System_Module_Enable(EN_USB);  
           
    System_USB_PHY_Config();   
 
    
    SCU->PABADS = (SCU->PABADS | ( (0x3 << 11)) );       
//    GPIO_init_para.Pin       = GPIO_PIN_11 | GPIO_PIN_12;
//    GPIO_init_para.Mode      = GPIO_MODE_ANALOG;
//    GPIO_init_para.Pull      = GPIO_NOPULL;
//    GPIO_init_para.Alternate = GPIO_FUNCTION_0; 
//    GPIO_Init(GPIOA, &GPIO_init_para);  
    
    NVIC_ClearPendingIRQ(USB_IRQn);  
	NVIC_EnableIRQ(USB_IRQn); 
    
    return 0;    
}   

/*=======================================================================================================
  * @brief  Config IO, enable USB PHY, enable EP, pullup USB PHY internal resistor to connect with host   
  * @param  none 
  * @retval success 
===========================================================================================================*/
uint32_t FSUSB_Init(void)  
{
    FSUSB_MSP_Init();       
    
    System_Delay(10);    
    
    USBCTRL->WORKING_MODE = 0x04;  //disconnect usb/ reset USBC    
	System_Delay(3000);   
	USBCTRL->WORKING_MODE = 0x09 ;  //auto reset, fullspeed  
    
    USBCTRL->EPxCSR[0] |= 1<<8;	//enable EP0  
    USBCTRL->EPxCSR[1] |= 1<<8;	//enable EP1  
    USBCTRL->EPxCSR[2] |= 1<<8;	//enable EP2  
    USBCTRL->EPxCSR[3] |= 1<<8;	//enable EP3  
    USBCTRL->EPxCSR[4] |= 1<<8;	//enable EP4  
    USBCTRL->EPADDR_CFG = 0x4321;  
    
    USBINT->INT_EN = 0x92427;   // enable Reset,Resume,Suspend,setup, EP1/2/3/4 OUT interrupt	  
     
	USBCTRL->WORKING_MODE  |= (1<<6)|(1<<4);	//connect   
    
    return 0;   
}  

/*=======================================================================================================
  * @brief  Get data length in FIFO
  * @param  ep_index, index of endpoint
  * @retval length of data received in FIFO 
===========================================================================================================*/  
uint16_t FSUSB_Get_FIFO_Length(uint8_t ep_index)
{
	return USBCTRL->EPxCSR[ep_index]&0xff;             	
}
/*=======================================================================================================
  * @brief  reset fifo 
  * @param  ep_index, index of endpoint
  * @retval none 
===========================================================================================================*/  
void FSUSB_Clear_FIFO(uint8_t ep_index)  
{
	USBCTRL->EPxCSR[ep_index] |= 1<<9;	  
} 

/*=======================================================================================================
  * @brief  check fifo stall or not 
  * @param  ep_index, index of endpoint
  * @param  ep_dir, in or out 
  * @retval ep stall status  
===========================================================================================================*/  
uint16_t FSUSB_Get_Stall_Status(uint8_t ep_index, uint8_t ep_dir) 
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

/*=======================================================================================================
  * @brief  check fifo stall or not 
  * @param  ep_index, index of endpoint
  * @param  ep_dir, in or out 
  * @retval ep stall status  
===========================================================================================================*/  
void FSUSB_Clear_Stall(uint8_t ep_index, uint8_t ep_dir)
{
	switch(ep_index)
	{
		case USB_EP1: 
		{    
			if(ep_dir == EP_DIR_IN)	 	ep1_stall[0]=0x0000;	  //in
			else			  			ep1_stall[1]=0x0000;	  //out	
			break;  
		}
		case USB_EP2: 
		{    
			if(ep_dir == EP_DIR_IN)	  	ep2_stall[0]=0x0000;	  //in
			else			  			ep2_stall[1]=0x0000;	  //out				  
			break;   
		}
		case USB_EP3: 
		{    
			if(ep_dir == EP_DIR_IN)	  	ep3_stall[0]=0x0000;	  //in
			else			  			ep3_stall[1]=0x0000;	  //out		  
			break;   
		}
		case USB_EP4: 
		{    
			if(ep_dir == EP_DIR_IN)	  	ep4_stall[0]=0x0000;	  //in
			else			  			ep4_stall[1]=0x0000;	  //out					  
			break;    
		}

		default: return;
	}

	USBCTRL->EPxCSR[ep_index] = 0x02100;		   //clear in/out toggle,stall,stall status
	USBCTRL->EPxCSR[ep_index] |= (1<<18)|(1<<15);  //enable change
//	flag_clear_stall=0;  		 
}

/*=======================================================================================================
  * @brief  send stall response 
  * @param  ep_index, index of endpoint
  * @param  ep_dir, in or out 
  * @retval none  
===========================================================================================================*/  
void FSUSB_Send_Stall(uint8_t ep_index, uint8_t ep_dir)
{
	switch(ep_index)
	{
		case USB_EP1: 
		{    
			if(ep_dir == EP_DIR_IN)	 	ep1_stall[0]=0x0001;	  //in
			else			  			ep1_stall[1]=0x0001;	  //out	
			break;  
		}
		case USB_EP2: 
		{    
			if(ep_dir == EP_DIR_IN)	  	ep2_stall[0]=0x0001;	  //in
			else			  			ep2_stall[1]=0x0001;	  //out				  
			break;   
		}
		case USB_EP3: 
		{    
			if(ep_dir == EP_DIR_IN)	  	ep3_stall[0]=0x0001;	  //in
			else			  			ep3_stall[1]=0x0001;	  //out		  
			break;   
		}
		case USB_EP4: 
		{    
			if(ep_dir == EP_DIR_IN)	  	ep4_stall[0]=0x0001;	  //in
			else			  			ep4_stall[1]=0x0001;	  //out					  
			break;    
		}

		default: return;
	}

	USBCTRL->EPxCSR[ep_index] |= (1<<12);    
}

/*=======================================================================================================
  * @brief  read received data byte by byte 
  * @param  dst, addr of destination
  * @param  length, length of data to read
  * @param  fifo_offset, fifo offset   
  * @param  index of endpoint 
  * @retval none  
===========================================================================================================*/  
void FSUSB_Read_EP_MEM8(uint8_t *dst, uint32_t length, uint32_t fifo_offset, uint8_t ep_index)
{
	uint8_t *src;

	src = (uint8_t *)(USB_BASE+0x200+(ep_index<<6)+fifo_offset);       
	while(length--)
	{
		*dst++ = *src++;
	}		 
}

/*=======================================================================================================
  * @brief  write data into fifo to transmit  
  * @param  src, addr of source
  * @param  length, length of data to write
  * @param  fifo_offset, fifo offset   
  * @param  index of endpoint 
  * @retval none  
===========================================================================================================*/    
void FSUSB_Write_EP_MEM8(uint8_t *src, uint32_t length, uint32_t fifo_offset, uint8_t ep_index)
{

	uint8_t *dst;

	dst = (uint8_t *)(USB_BASE+0x200+(ep_index<<6)+fifo_offset); 

	while(length--)
	{
		*dst++ = *src++;
	}
}

/*=======================================================================================================
  * @brief  set endpoint ready to transmit, data will be sent after receiving in token packet   
  * @param  length, length of data to transmit    
  * @param  index of endpoint 
  * @retval none  
===========================================================================================================*/ 
uint8_t FSUSB_Start_EP_Transfer(uint32_t length, uint8_t ep_index)
{
	uint8_t intoken_cnt;
	
	USBCTRL->EPxSENDBN[ep_index]= length;
	
	while(1)
	{
		// if a new out data packet received, return error to caller  
	    if( (USBINT->INT_STAT_RAW & MASK_EPX_OUT(ep_index)) && (USBINT->INT_STAT_RAW & MASK_EPX_ACK(ep_index)) )
		{
		    USBINT->INT_CLR  = MASK_EPX_OUT(ep_index);	
	        USBINT->INT_CLR  = MASK_EPX_ACK(ep_index);
			return ERROR_OUT_OUT;
		}
		// wait for IN token to start transfer 
        if(USBINT->INT_STAT_RAW & MASK_EPX_IN(ep_index) )
        {
		    USBINT->INT_CLR  = MASK_EPX_IN(ep_index);
			USBCTRL->WORKING_MODE  |= (1<<11);//return NAK when timeout
			USBCTRL->EPxCSR[ep_index] |= (1<<10);//data is ready for tx
			break;
        }			
	}
	

	while(1)
	{
		if( USBCTRL->EPxCSR[ep_index]&0x1000000 ) //received ACK from host 
		{
			USBINT->INT_CLR  = MASK_EPX_ACK(ep_index);
			USBINT->INT_CLR  = MASK_EPX_IN(ep_index);
			return 0;//pass
		}
		
		if(USBINT->INT_STAT_RAW & (1<<21) ) // timeout occurs when wait ACK 
		{
            USBINT->INT_CLR  = (1<<21);
			intoken_cnt = 4;
			while(intoken_cnt) // wait 3 SOF frame for bad signal, during this time, device will send NACK when IN token received  
			{               
                if(USBINT->INT_STAT_RAW & (1<<3))				
				{
				    intoken_cnt --;
					USBINT->INT_CLR  = (1<<3);
				}
			}
			USBINT->INT_CLR  = MASK_EPX_TIMEOUT(ep_index); // device recover to send data packet after IN token received  		
		}	
		
		if(USBINT->INT_STAT_RAW & MASK_EPX_OUT(ep_index))
		{
			return ERROR_IN_OUT; 
		}		
	}	
}

/*=======================================================================================================
  * @brief  send length of data from buffer by ep_index 
  * @param  buffer, address of source data      
  * @param  length, length of data to transmit    
  * @param  index of endpoint 
  * @retval 0, success; other value, failed    
===========================================================================================================*/       
uint8_t FSUSB_Send_Data(uint8_t *buffer,uint32_t length,uint8_t ep_index)
{
	uint8_t ret;
	
	while(length>=EPX_MAX_PACKET_SIZE)
	{    		
		FSUSB_Write_EP_MEM8(buffer,EPX_MAX_PACKET_SIZE,0, ep_index);  		   
		ret = FSUSB_Start_EP_Transfer(EPX_MAX_PACKET_SIZE, ep_index);
		if(ret == ERROR_OUT_OUT)
		{
		    if( USBCTRL->EPxCSR[ep_index] & ( 1<< 19) )//Toggle error
		    {
		        USBCTRL->EPxCSR[ep_index] ^= (1<<17);  //out toggle want
			    USBCTRL->EPxCSR[ep_index] |= (1<<18);  //update want toggle;			 			             			
		    }
			USBCTRL->EPxCSR[ep_index] |= 1<<11; //set rx ready  	
            continue;	// received a same packet, has processed this packet, just fill respoonse to fifo and send it to host 		
		}
		else if(ret != 0)
		{
			return 1; // send data fail, exit with error code to let caller know 
		}
	 	length -= EPX_MAX_PACKET_SIZE;
	  	buffer += EPX_MAX_PACKET_SIZE;		
	}
	// remaining data, less than EPX_MAX_PACKET_SIZE  
	while(length>0)  
	{
		FSUSB_Write_EP_MEM8(buffer,length,0,ep_index);  	     
		ret = FSUSB_Start_EP_Transfer(length,ep_index);
		if(ret == ERROR_OUT_OUT)
		{
		    if( USBCTRL->EPxCSR[ep_index] & ( 1<< 19) )//Toggle error
		    {
		        USBCTRL->EPxCSR[ep_index] ^= (1<<17); //out toggle want
			    USBCTRL->EPxCSR[ep_index] |= (1<<18); //update want toggle;			 			             			
		    }
			USBCTRL->EPxCSR[ep_index] |= 1<<11; //set rx ready  
            continue;				
		}
		else if(ret != 0)
		{
			return 1; // send data fail, exit with error code to let caller know   
		}
	 	length -= length;
	  	buffer += length;
	}
	
	return 0;  
}


 

/*=================================================================================================================================
  * @brief  receive length of data and save them into buffer by ep_index 
  * @param  buffer, address of destination      
  * @param  length, length of data to receive, if you don't know the specific value, you can use max vuale of one packet-64     
  * @param  ep_index, index of endpoint 
  * @param  single_packet, receive one packet or more data 
  * @retval actual length of readout; if the high 16 bits is greater than low 16 bits, it indicates error.     
=====================================================================================================================================*/        
uint32_t FSUSB_Receive_Data(uint8_t *buffer, uint32_t length, uint8_t ep_index, uint8_t single_packet)  
{
	volatile uint32_t fifo_len;
    volatile uint32_t read_length;  
    
    read_length = length;  
    
	while(read_length > 0)	 
	{			        
        while (0 == (USBCTRL->EPxCSR[ep_index] & BIT20))
        {
            // wait 
        }
		USBINT->INT_CLR  = MASK_EPX_OUT(ep_index);	
	    USBINT->INT_CLR  = MASK_EPX_ACK(ep_index);		
		USBCTRL->EPxCSR[ep_index] |= BIT20;
        
		if( USBCTRL->EPxCSR[ep_index] & ( 1<< 19) )//Toggle error 
		{
		    USBCTRL->EPxCSR[ep_index] ^= (1<<17); //out toggle want
			USBCTRL->EPxCSR[ep_index] |= (1<<18); //update want toggle; 
            
			USBCTRL->EPxCSR[ep_index] |= 1<<11;   //set rx ready, wait for a new packet 
			continue;  //discard this packet          			
		}		
	
		fifo_len = FSUSB_Get_FIFO_Length(ep_index);  
        
        if (read_length < fifo_len)
        {
            return ((fifo_len << 16) | read_length);     // error, app should read all data saved in fifo    
        }           
		FSUSB_Read_EP_MEM8(buffer, fifo_len, 0, ep_index);	  // read all data out    		
		read_length -= fifo_len;
		buffer += fifo_len;     
        USBCTRL->EPxCSR[ep_index] |= 1<<11; //set rx ready to wait next packet    
        
        if (single_packet)
        {
            return fifo_len;   
        }
        
	}	

    return length;   
} 


/*=======================================================================================================
  * @brief  send 0 packet by EP0 
  * @retval none    
===========================================================================================================*/     
void FSUSB_EP0_Send_Empty_Packet(void)
{
	FSUSB_Start_EP_Transfer(0,USB_EP0);    
} 

/*=======================================================================================================
  * @brief  send stall packet by EP0   
  * @retval none    
===========================================================================================================*/  
void FSUSB_EP0_Send_Stall(void)
{
	USBCTRL->EPxCSR[0] |= 1<<12;
	while(!(USBCTRL->EPxCSR[0] &0x2000));
	USBCTRL->EPxCSR[0] |= 0x2000;    
}         

/*=======================================================================================================
  * @brief  enable or disable interrupt 
  * @param  usb_interrupt, USB_EP1_OUT_PACKET/USB_EP2_OUT_PACKET, ...   
  * @param  enable_disable, enable or disable   
  * @retval none    
===========================================================================================================*/       
void FSUSB_Enable_Disable_INT(uint32_t usb_interrupt, uint32_t enable_disable)
{
    if (0 == enable_disable)  
    {
        USBINT->INT_EN &= (~(usb_interrupt));  
    }
    else 
    {
        USBINT->INT_EN |= ((usb_interrupt));      
    }
}







