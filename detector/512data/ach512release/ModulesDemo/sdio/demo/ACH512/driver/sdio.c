#include  "common.h"
#include  "sdio.h"

SDIO_REG_INFO emmc_reg_info;
SDIO_REG_INFO sd_reg_info;

volatile UINT32 flag_int_sdio=0;

void SDIO_IRQHandler(void)
{
	flag_int_sdio = REG_SDIO_RINTSTS;

	REG_SDIO_RINTSTS = flag_int_sdio;
}

void sdio_init(UINT8 card_type)
{
#ifdef LOW_POWER
		enable_module(BIT_SDIO); 
#endif

	
//SDIO0配置
    REG_SCU_MUXCTRLB = (REG_SCU_MUXCTRLB & (~0xf0ffff00)) | 0xA0555500; 	//CLK/DETECT/D7~D0	
    REG_SCU_MUXCTRLC = (REG_SCU_MUXCTRLC & (~0x000000ff)) | 0x0000002A;     //CMD/POWER_EN/WP		

	REG_SDIO_CTRL |= 0x07; 				//reset SDIOC/FIFO/DMA
	while(REG_SDIO_CTRL&0x07);

	//REG_SDIO_POWEN = 0;        //power off
	REG_SDIO_POWEN |= SDIO_POWER_ON(SDIO_CH0);

	REG_SDIO_CLK_EN |= SDIO_LOW_POWER(SDIO_CH0); //low power mode
    //REG_SDIO_CLK_EN = 0; 	  //非low power mode

	REG_SDIO_RINTSTS =0x3FFFF; 	//clear all int status
#ifdef	INT_MODE
	NVIC_ClearPendingIRQ(SDIO_IRQn);
    NVIC_EnableIRQ(SDIO_IRQn);
	REG_SDIO_INT_MASK = 0x3FFFF; 		//enable all int src
   	REG_SDIO_CTRL |= SDIO_INT_EN;
#endif

	REG_SDIO_WIDTH = SDIO_WIDTH(BUS_BIT1<<SDIO_CH0)  ;//1线模式

	REG_SDIO_TIMEOUT = SDIO_TIMEOUT_RW | SDIO_TIMEOUT_RESP ; //设置响应和读写超时参数
	REG_SDIO_BLK_SIZE = SDIO_BLOCK_SIZE;
	REG_SDIO_FIFOTH = SDIO_RX_LEVEL | SDIO_TX_LEVEL ;
	REG_SDIO_DEBOUNCE = SDIO_DEBOUNCE_COUNT;   

	if(card_type ==EMMC_CARD)
	{
		emmc_reg_info.rca = EMMC_RCA_VALUE;
		emmc_reg_info.hcs = EMMC_HCS;
	}
	else
	{
		sd_reg_info.rca = 0;
		sd_reg_info.hcs = SD_HCS;
	}
}

void sdio_set_clock(UINT32 div, UINT32 clk_src, UINT8 sdio_index)
{

	if(sdio_index ==SDIO_CH0) REG_SDIO_CLK_EN &= ~0x01;   	//stop CCK
	else					  REG_SDIO_CLK_EN &= ~0x02;   	//stop CCK
	REG_SDIO_CMD = SDIO_START_CMD | SDIO_UPDATE_CLK_ONLY | SDIO_WAIT_PRVDATA_END;
	while(REG_SDIO_CMD & SDIO_START_CMD);

	if(sdio_index==SDIO_CH0)  	
	{
		REG_SDIO_CLK_SRC = (REG_SDIO_CLK_SRC & 0x0C) | clk_src ;	
	}
	else
	{
		REG_SDIO_CLK_SRC = (REG_SDIO_CLK_SRC & 0x03) | (clk_src<<2) ;	
	}

	REG_SDIO_CLK_DIV = (REG_SDIO_CLK_DIV&(~(0xFF<<(clk_src*8))) )|(div<<(clk_src*8)) ;
	REG_SDIO_CMD = SDIO_START_CMD | SDIO_UPDATE_CLK_ONLY | SDIO_WAIT_PRVDATA_END;
	while(REG_SDIO_CMD & SDIO_START_CMD);

	REG_SDIO_CLK_EN |= 1<<sdio_index; 	//enable CCK
	REG_SDIO_CMD = SDIO_START_CMD | SDIO_UPDATE_CLK_ONLY | SDIO_WAIT_PRVDATA_END;
	while(REG_SDIO_CMD & SDIO_START_CMD);
}


UINT8 sdio_cmd_nodata(UINT32 cmd, UINT32 cmd_arg, UINT8 sdio_index)
{
	while(REG_SDIO_STATUS&0x200); 	//wait idel	
	REG_SDIO_RINTSTS = 0x3FFFF;	//clear int	
	
	REG_SDIO_CMD_ARG = cmd_arg;	
	REG_SDIO_CMD = cmd | (sdio_index<<16);

#ifdef INT_MODE	
	while(!(flag_int_sdio&0x04)); 
	flag_int_sdio &= ~0x04;
#else
	while(!(REG_SDIO_RINTSTS&0x04)); 	//wait cmd done	
	REG_SDIO_RINTSTS = 0x04;			//clear int
#endif

	if(cmd == CMD1_MATCH_VCC)  //R3
	{
		if(REG_SDIO_RINTSTS & 0x100)     //error:ack_timeout
		{
			REG_SDIO_RINTSTS = 0x100;  //clear int
			return	0x01;
		}
	}
	else
	{
		if(REG_SDIO_RINTSTS & 0x142)     //error:ack/ack_crc,ack_timeout
		{
			REG_SDIO_RINTSTS = 0x142;  //clear int

			return	0x02;
		}
	}

	return 0;
}


UINT32 sdio_cmd_read_data(UINT32 cmd, UINT32 cmd_arg, UINT32 length, UINT32 *buff, UINT8 sdio_index)
{
	UINT32 i;
	UINT32 mask;

	while(REG_SDIO_STATUS&0x200); 	//wait idel    
	REG_SDIO_RINTSTS = 0x3FFFF;	//clear int	

	REG_SDIO_BYTE_CNT = length;
	REG_SDIO_CMD_ARG = cmd_arg;	
    REG_SDIO_CMD = cmd | (sdio_index<<16);
	
#ifdef INT_MODE	
	while(!(flag_int_sdio&0x04)); 
	flag_int_sdio &= ~0x04;
#else
	while(!(REG_SDIO_RINTSTS&0x04)); 	//wait cmd done	
	REG_SDIO_RINTSTS = 0x04;			//clear int
#endif

 	if(REG_SDIO_RINTSTS&0x142) 	//error:ack/ack_crc,ack_timeout
	{
		REG_SDIO_RINTSTS = 0x142;	//clear int	

		return	0x01;
	}  

	for(i=0;i<(length/4);i++)	  
	{
		while(REG_SDIO_STATUS & 0x04);	 //fifo empty ?
		buff[i] = REG_SDIO_DATA;						
	}
	if(length%4)
	{
		while(REG_SDIO_STATUS & 0x04);	 //fifo empty ?
		buff[i]=REG_SDIO_DATA;	
	}

#ifdef INT_MODE	
	while(!(flag_int_sdio&0x08)); 
	flag_int_sdio &= ~0x08;
#else
	while(!(REG_SDIO_RINTSTS&0x08)); 	//wait data transfer over	
	REG_SDIO_RINTSTS=0x08;
#endif

	while(REG_SDIO_STATUS&0x200);    	//wait card idle

   	if( REG_SDIO_STATUS&0x3FFE0000 ) // fifo 非空
   	{
		REG_SDIO_CTRL = 0x02; 			//reset fifo 
		while(REG_SDIO_CTRL&0x02);
   	}

	if(cmd ==CMD14_BUSTEST_R)	mask = 0xA200;
	else						mask = 0xA280;

	if(REG_SDIO_RINTSTS&mask) 	   //error: data crc,timeout,start/end bit
	{
		REG_SDIO_RINTSTS = mask;	//clear int	
		
		return	0x02;
	}

	return	0;
}

UINT32 sdio_cmd_write_data(UINT32 cmd, UINT32 cmd_arg, UINT32 length, UINT32 *buff, UINT8 sdio_index)
{
	UINT32 i;

	while(REG_SDIO_STATUS&0x200); 	//wait idel	
	REG_SDIO_RINTSTS = 0x3FFFF;	//clear int	

	REG_SDIO_BYTE_CNT = length;
	REG_SDIO_CMD_ARG = cmd_arg;	
    REG_SDIO_CMD = cmd | (sdio_index<<16);
	
#ifdef INT_MODE	
	while(!(flag_int_sdio&0x04)); 
	flag_int_sdio &= ~0x04;
#else
	while(!(REG_SDIO_RINTSTS&0x04)); 	//wait cmd done	
	REG_SDIO_RINTSTS = 0x04;			//clear int
#endif	 

 	if(REG_SDIO_RINTSTS&0x142) 	//error:ack/ack_crc,ack_timeout
	{
		REG_SDIO_RINTSTS = 0x142;	//clear int	

		return	0x01;
	}  

	for(i=0;i<(length/4);i++)	  
	{
		while(REG_SDIO_STATUS & 0x08);	 //fifo full ?
		REG_SDIO_DATA = buff[i];			
	}
	if(length%4)
	{
		while(REG_SDIO_STATUS & 0x08);	 //fifo full ?
		REG_SDIO_DATA = buff[i];	
	}


#ifdef INT_MODE	
	while(!(flag_int_sdio&0x08)); 
	flag_int_sdio &= ~0x08;
#else
	while(!(REG_SDIO_RINTSTS&0x08)); 	//wait data transfer over	
	REG_SDIO_RINTSTS=0x08;
#endif
	REG_SDIO_CTRL = 0x02;  //clear fifo
	while(REG_SDIO_STATUS&0x200);    	//wait card idle

	if(cmd !=CMD19_BUSTEST_W)
	{
		if(REG_SDIO_RINTSTS&0x8080) 	   	//error:data crc, end bit
		{
			REG_SDIO_RINTSTS = 0x8080;	//clear int	
	
			return	0x02;
		}
	}

	return	0;
}


//----------------------SD_EMMC CMD-------------------------------//

UINT8 sdio_CMD0_go_idle(UINT8 card_type)
{
	UINT32 result=0;
	UINT32 cmd_arg;

	if(card_type==EMMC_CARD)	cmd_arg=0xF0F0F0F0;
	else						cmd_arg=0;  //stuff bits

	result=sdio_cmd_nodata(CMD0_GO_IDLE, cmd_arg , SDIO_CH0);	 //enter pre idle state
	if(result)
	{
		return	0x01;
	}

	return 0;
}

UINT8 sdio_CMD1_send_op(UINT8 card_type)
{
	UINT32 result=0;
	UINT32 cmd_arg;
	
	if(card_type != EMMC_CARD) return 0xFF;

	cmd_arg= (emmc_reg_info.hcs<<30)|EMMC_VDD_WINDOW; 

	while(1)
	{
		result=sdio_cmd_nodata(CMD1_MATCH_VCC, cmd_arg, SDIO_CH0);
		if(result)
		{
			return	3;
		}
		if(REG_SDIO_RESP0 & 0x80000000)	break;
	}


	if(((REG_SDIO_RESP0>>30)&0x01) != emmc_reg_info.hcs)	 //容量类型不匹配
	{
		emmc_reg_info.hcs = ((REG_SDIO_RESP0>>30)&0x01);
	}

	emmc_reg_info.ocr = REG_SDIO_RESP0;

	return 0;
}

UINT8 sdio_CMD2_get_cid(UINT8 card_type)
{
	UINT32 result=0;

	result=sdio_cmd_nodata(CMD2_CID, 0x00, SDIO_CH0);	 //enter pre idle state
	if(result)
	{
		return	0x01;
	}

	if(card_type==EMMC_CARD) 
	{	
		emmc_reg_info.cid[0]=REG_SDIO_RESP0;
		emmc_reg_info.cid[1]=REG_SDIO_RESP1;
		emmc_reg_info.cid[2]=REG_SDIO_RESP2;
		emmc_reg_info.cid[3]=REG_SDIO_RESP3;
	}
	else
	{
		sd_reg_info.cid[0]=REG_SDIO_RESP0;
		sd_reg_info.cid[1]=REG_SDIO_RESP1;
		sd_reg_info.cid[2]=REG_SDIO_RESP2;
		sd_reg_info.cid[3]=REG_SDIO_RESP3;
	}

	return 0;
}

UINT8 sdio_CMD3_rca(UINT8 card_type)
{
	UINT32 result=0;
	UINT32 cmd_arg;

	if(card_type==EMMC_CARD)	cmd_arg= emmc_reg_info.rca <<16;
	else						cmd_arg= 0;   	//stuff bits

	result=sdio_cmd_nodata(CMD3_RCA, cmd_arg, SDIO_CH0);	 //enter pre idle state
	if(result)
	{
		return	0x01;
	}

	if(card_type==SD_CARD)	 sd_reg_info.rca = REG_SDIO_RESP0>>16;

	return 0;
}

UINT8 sdio_CMD6_switch(UINT32 cmd_arg)
{
	UINT32 result=0;

	result=sdio_cmd_nodata(CMD6_SWTICH, cmd_arg , SDIO_CH0);
	if(result)
	{
		return	0x01;
	}
	return 0;
}


UINT8 sdio_CMD6_switch_bus_width(UINT32 bus_width)
{
	UINT32 result=0;
	UINT32 cmd_arg;
	UINT32 temp;

	if(bus_width==BUS_BIT4)	 		cmd_arg = 0x03B70100;
	else if (bus_width==BUS_BIT8)	cmd_arg = 0x03B70200;
	else 							return 0x01;

	result=sdio_cmd_nodata(CMD6_SWTICH, cmd_arg , SDIO_CH0);
	if(result)
	{
		return	0x02;
	}	 
	
	//	if(sdio_index==SDIO_CH0) 	temp=REG_SDIO_WIDTH&0x20002;  //mask:保留CARD_CH1的位宽设置
	//	else						temp=REG_SDIO_WIDTH&0x10001;  //mask:保留SDIO_CH0的位宽设置
	temp=REG_SDIO_WIDTH&0x20002;  //mask:保留CARD_CH1的位宽设置	
	REG_SDIO_WIDTH = temp | SDIO_WIDTH(bus_width<<SDIO_CH0) ;

	return	0;
}


UINT8 sdio_CMD7_select(UINT8 card_type)
{
	UINT32 result=0;
	UINT32 cmd_arg;

	if(card_type==EMMC_CARD)	cmd_arg= emmc_reg_info.rca<<16 ;
	else						cmd_arg= sd_reg_info.rca<<16 ;

	result=sdio_cmd_nodata(CMD7_SELECT_CARD, cmd_arg, SDIO_CH0);
	if(result)
	{
		return	0x01;
	}

	return 0;
}

UINT8 sdio_CMD8_get_ext_csd(UINT32 *buff, UINT8 card_type)
{
	UINT32 result=0;
	UINT32 cmd_arg;

	if(card_type==EMMC_CARD)	
	{
		cmd_arg= 0; //stuff bits
		result=sdio_cmd_read_data(CMD8_EXT_CSD_EMMC, cmd_arg, 512, buff, SDIO_CH0);
	}
	else						
	{
		cmd_arg= (1<<8)|0xAA;  //2.7V~3.3V
		result=sdio_cmd_nodata(CMD8_EXT_CSD_SD, cmd_arg, SDIO_CH0);

		if(REG_SDIO_RESP0 != cmd_arg)	return	0x02;   //VCC范围不支持
	}

	if(result)
	{
		return	0x01;
	}

	return 0;
}

UINT8 sdio_CMD9_get_csd(UINT8 card_type)
{
	UINT32 result=0;
	UINT32 cmd_arg;

	if(card_type==EMMC_CARD)	cmd_arg= emmc_reg_info.rca<<16 ;
	else						cmd_arg= sd_reg_info.rca<<16 ;

	result=sdio_cmd_nodata(CMD9_CSD, cmd_arg, SDIO_CH0);
	if(result)
	{
		return	0x01;
	}

	if(card_type==EMMC_CARD) 
	{	
		emmc_reg_info.csd[0]=REG_SDIO_RESP0;
		emmc_reg_info.csd[1]=REG_SDIO_RESP1;
		emmc_reg_info.csd[2]=REG_SDIO_RESP2;
		emmc_reg_info.csd[3]=REG_SDIO_RESP3;
	}
	else
	{
		sd_reg_info.csd[0]=REG_SDIO_RESP0;
		sd_reg_info.csd[1]=REG_SDIO_RESP1;
		sd_reg_info.csd[2]=REG_SDIO_RESP2;
		sd_reg_info.csd[3]=REG_SDIO_RESP3;
	}	 	

	return 0;
}


UINT8 sdio_CMD16_set_blocklen(UINT32 block_size)
{
	UINT32 result=0;

	result=sdio_cmd_nodata(CMD16_SET_BLOCKLEN, block_size, SDIO_CH0);	 //enter pre idle state
	if(result)
	{
		return	0x01;
	}

	return 0;
}

//blk_addr:逻辑起始块地址
UINT8 sdio_CMD17_read_single(UINT32 blk_addr, UINT32 *buff, UINT8 card_type) 
{
	UINT32 result=0;
	UINT32 addr;
	UINT32 length;

	if(card_type==EMMC_CARD) 
	{
		if(emmc_reg_info.hcs==STANDRAD_CAPACITY)  	addr = blk_addr*SDIO_BLOCK_SIZE;
		else										addr = blk_addr;
	}
	else
	{
		if(sd_reg_info.hcs==STANDRAD_CAPACITY)  	addr = blk_addr*SDIO_BLOCK_SIZE;
		else										addr = blk_addr;
	}
 	
	length = SDIO_BLOCK_SIZE;
	result=sdio_cmd_read_data(CMD17_READ_SINGLE, addr, length, buff, SDIO_CH0);

	if(result)
	{
		return	0x01;
	}
	 	
	return 0;
}

 //blk_addr:逻辑起始块地址,
UINT8 sdio_CMD18_read_multiple(UINT32 blk_addr, UINT32 blk_num, UINT32 *buff, UINT8 card_type)
{
	UINT32 result=0;
	UINT32 addr;
	UINT32 length;

	if(card_type==EMMC_CARD) 
	{
		if(emmc_reg_info.hcs==STANDRAD_CAPACITY)  	addr = blk_addr*SDIO_BLOCK_SIZE;
		else										addr = blk_addr;
	}
	else
	{
		if(sd_reg_info.hcs==STANDRAD_CAPACITY)  	addr = blk_addr*SDIO_BLOCK_SIZE;
		else										addr = blk_addr;
	}

	length = blk_num*SDIO_BLOCK_SIZE;

	result=sdio_cmd_read_data(CMD18_READ_MUL, addr, length, buff, SDIO_CH0);
	
	if(result)
	{
		return	0x01;
	}
	 	
	return 0;
}

//blk_addr:逻辑起始块地址
UINT8 sdio_CMD24_write_single(UINT32 blk_addr, UINT32 *buff, UINT8 card_type)	 
{
	UINT32 result=0;
	UINT32 addr;
	UINT32 length;

	if(card_type==EMMC_CARD) 
	{
		if(emmc_reg_info.hcs==STANDRAD_CAPACITY)  	addr = blk_addr*SDIO_BLOCK_SIZE;
		else										addr = blk_addr;
	}
	else
	{
		if(sd_reg_info.hcs==STANDRAD_CAPACITY)  	addr = blk_addr*SDIO_BLOCK_SIZE;
		else										addr = blk_addr;
	}

	length = SDIO_BLOCK_SIZE;

	result=sdio_cmd_write_data(CMD24_WRITE_SINGLE, addr, length, buff, SDIO_CH0);

	if(result)
	{
		return	0x01;
	}

	return 0;
}

 //blk_addr:逻辑起始块地址,
UINT8 sdio_CMD25_write_multiple(UINT32 blk_addr, UINT32 blk_num, UINT32 *buff, UINT8 card_type)
{
	UINT32 result=0;
	UINT32 addr;
	UINT32 length;

	if(card_type==EMMC_CARD) 
	{
		if(emmc_reg_info.hcs==STANDRAD_CAPACITY)  	addr = blk_addr*SDIO_BLOCK_SIZE;
		else										addr = blk_addr;
	}
	else
	{
		if(sd_reg_info.hcs==STANDRAD_CAPACITY)  	addr = blk_addr*SDIO_BLOCK_SIZE;
		else										addr = blk_addr;
	}

	length = blk_num*SDIO_BLOCK_SIZE;

	result=sdio_cmd_write_data(CMD25_WRITE_MUL, addr, length, buff, SDIO_CH0);
	
	if(result)
	{
		return	0x01;
	}

	return 0;
}

UINT8 sdio_CMD38_erase(UINT32 blk_start_addr, UINT32 blk_num, UINT8 card_type)
{
	UINT32 result=0;
	UINT32 first_blk_addr;
	UINT32 last_blk_addr;
	UINT32 cmd;

	
	if(card_type==EMMC_CARD)	
	{
		cmd= CMD35_ERASESD_START ;
		if(emmc_reg_info.hcs==STANDRAD_CAPACITY) 
		{
			first_blk_addr = blk_start_addr*SDIO_BLOCK_SIZE;
		}
		else
		{
		 	first_blk_addr = blk_start_addr;
		}
	}
	else						
	{
		cmd= CMD32_ERASESD_START ;
		if(sd_reg_info.hcs==STANDRAD_CAPACITY) 
		{
			first_blk_addr = blk_start_addr*SDIO_BLOCK_SIZE;
		}
		else
		{
		 	first_blk_addr = blk_start_addr;
		}

	}
	result=sdio_cmd_nodata(cmd, first_blk_addr, SDIO_CH0);	
	if(result)
	{
		return	0x01;
	}

 	if(card_type==EMMC_CARD)
	{
		cmd= CMD36_ERASESD_END ;
		if(emmc_reg_info.hcs==STANDRAD_CAPACITY) 
		{
			last_blk_addr = ((blk_start_addr+ (blk_num-1))*SDIO_BLOCK_SIZE);
		}
		else
		{
		 	last_blk_addr = blk_start_addr+ (blk_num-1);
		}
	}
	else						
	{
		cmd= CMD33_ERASESD_END ;
		if(sd_reg_info.hcs==STANDRAD_CAPACITY) 
		{
			last_blk_addr = ((blk_start_addr+ (blk_num-1))*SDIO_BLOCK_SIZE);
		}
		else
		{
		 	last_blk_addr = blk_start_addr+ (blk_num-1);
		}
	}
	result=sdio_cmd_nodata(cmd, last_blk_addr, SDIO_CH0);	
	if(result)
	{
		return	0x02;
	}

	result=sdio_cmd_nodata(CMD38_ERASE, 0x00, SDIO_CH0);	
	if(result)
	{
		return	0x03;
	}
	 	
	return 0;
}



UINT8 sdio_ACMD6_set_bus_width(UINT32 bus_width, UINT8 card_type)
{
	UINT32 result=0;
	UINT32 cmd_arg;
	UINT32 temp;

	result=sdio_CMD55_app(card_type);
	if(result)
	{
		return	0x01;
	}

	if(bus_width==BUS_BIT1)	 		cmd_arg = 0x00;
	else if (bus_width==BUS_BIT4)	cmd_arg = 0x02;
	else 							return 0x02;

	result=sdio_cmd_nodata(ACMD6_SET_BUS_WIDTH, cmd_arg , SDIO_CH0);
	if(result)
	{
		return	0x03;
	} 
	
	//	if(sdio_index==SDIO_CH0) 	temp=REG_SDIO_WIDTH&0x20002;  //mask:保留CARD_CH1的位宽设置
	//	else						temp=REG_SDIO_WIDTH&0x10001;  //mask:保留SDIO_CH0的位宽设置
	temp=REG_SDIO_WIDTH&0x20002;  //mask:保留CARD_CH1的位宽设置
	REG_SDIO_WIDTH = temp | SDIO_WIDTH(bus_width<<SDIO_CH0) ;

	return	0;
}

UINT8 sdio_ACMD41_send_cond(UINT8 card_type)
{
	UINT32 result=0;
	UINT32 cmd_arg;

	while(1)
	{
		result=sdio_CMD55_app(card_type);
		if(result)
		{
			return	0x01;
		}

		cmd_arg= (sd_reg_info.hcs<<30)|SD_VDD_WINDOW; 

		result=sdio_cmd_nodata(ACMD41_SEND_COND, cmd_arg, SDIO_CH0);
		if(result)
		{
			return	3;
		}
		if(REG_SDIO_RESP0 & 0x80000000)	break;
	}

	if(((REG_SDIO_RESP0>>30)&0x01) != sd_reg_info.hcs)	 //容量类型不匹配
	{
		sd_reg_info.hcs = ((REG_SDIO_RESP0>>30)&0x01);
	}

	sd_reg_info.ocr=REG_SDIO_RESP0; 

	return 0;
}

UINT8 sdio_CMD55_app(UINT8 card_type)
{
	UINT32 result=0;
	UINT32 cmd_arg;

	if(card_type==EMMC_CARD)	cmd_arg= emmc_reg_info.rca<<16 ;
	else						cmd_arg= sd_reg_info.rca<<16 ;

	result=sdio_cmd_nodata(CMD55_APP, cmd_arg, SDIO_CH0);
	if(result)
	{
		return	0x01;
	}

	return 0;
}


UINT8 emmc_enumerate(void)
{
	UINT32 result=0;
	UINT32 div;
	
	div = ((REG_SCU_CLKDIV>>12)&0x0F) +1;  //SDIO_CLK 分频
	div=(( (SRCClock/div)/1000) /400)+4;			//大约400Khz  (0~400KHz)
	div=div/2;
	sdio_set_clock(div, SDIO_CLK_SRC_DIV0, SDIO_CH0);

	result=sdio_CMD0_go_idle(EMMC_CARD);
	if(result)
	{
		return	2;
	}

	result=sdio_CMD1_send_op(EMMC_CARD);
	if(result)
	{
		return	3;
	}

	result=sdio_CMD2_get_cid(EMMC_CARD);
	if(result)
	{
		return	4;
	}

	result=sdio_CMD3_rca(EMMC_CARD);
	if(result)
	{
		return	5;
	}

	div= SRCClock/(EMMC_CLK*1000000);
	if(SRCClock%(EMMC_CLK*1000000))	div++;
	printfS("SDIO CLK= %d Mhz\n", (SRCClock/div)/1000000);	
	REG_SCU_CLKDIV = (REG_SCU_CLKDIV&(~(0x0F<<12)))|((div-1)<<12); 
	
	div=0;//能支持的最高速率	
	sdio_set_clock(div, SDIO_CLK_SRC_DIV0, SDIO_CH0 ); // DIV0作为时钟源	 

	return	0;
}

UINT8 sd_enumerate(void)
{
	static UINT32 result=0;
	UINT32 div;


	if(REG_SDIO_CARD_DETECT &(1<<SDIO_CH0))	
	{
		return	0x01;
	}
	
//	if(REG_SDIO_WRTPRT &(1<<SDIO_CH0))	
//	{	
//		return	0x02;
//	}	
				
	div = ((REG_SCU_CLKDIV>>12)&0x0F) +1;  //SDIO_CLK 分频
	div=(( (SRCClock/div)/1000) /400)+4;			//大约400Khz  (0~400KHz)
	div=div/2;
	sdio_set_clock(div, SDIO_CLK_SRC_DIV1, SDIO_CH0);

	//卡复位
	result=sdio_CMD0_go_idle(SD_CARD);
	if(result)
	{
		return	3;
	}

	result=sdio_CMD8_get_ext_csd(NULL, SD_CARD);
 	if(result)
	{
		return	4;
	}

	//卡初始化
	result=sdio_ACMD41_send_cond(SD_CARD);
	
 	if(result)
	{
		return	5;
	}


	//卡识别
	result=sdio_CMD2_get_cid(SD_CARD);
	if(result)
	{
		return	6;
	}

	result=sdio_CMD3_rca(SD_CARD);
	if(result)
	{
		return	7;
	}
	
    
	div= SRCClock/(SD_CLK*1000000);
	if(SRCClock%(SD_CLK*1000000))	div++;
	printfS("SDIO CLK= %d Mhz\n", (SRCClock/div)/1000000);	
	
	REG_SCU_CLKDIV = (REG_SCU_CLKDIV&(~(0x0F<<12)))|((div-1)<<12); 
	div=0;
	sdio_set_clock(div, SDIO_CLK_SRC_DIV1, SDIO_CH0);	// DIV1作为时钟源  
	

	return	0;
}














