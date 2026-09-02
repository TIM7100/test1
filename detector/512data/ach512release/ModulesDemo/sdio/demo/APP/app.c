/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : app.c
 * Description : application example source file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/

#include  "app.h"
#include  "sdio.h"

__align(8) UINT32 g_data_buf[1024];


UINT32 erase_test(void)
{
	UINT32 result, i, blk_num ;

	result=sdio_CMD38_erase(0, 50, EMMC_CARD);  
	if(result)
	{
		 return	1;
	}

	for(blk_num=0; blk_num<50; blk_num++)	 //10KB
	{
		for(i=0;i<(SDIO_BLOCK_SIZE/4);i++)	 
		{
			read_buff[i]=0x12345678;  //init buff
		}
		
		result= sdio_CMD17_read_single(blk_num, read_buff, EMMC_CARD);
		if(result)
		{
			return	2;
		}
		
		for(i=0;i<(SDIO_BLOCK_SIZE/4);i++)	 
		{
			if(read_buff[i] != 0x00)  
			{
				printfS("erase/read error \n");
				return	3;
			}
		}

		printfS("write/read OK: blk_num= %d \n", blk_num);	
	}

	return	0;
}

UINT32 single_write_read_test(void)
{
	UINT32 result, i, blk_num ;

	//sdio_CMD38_erase(0, 10, EMMC_CARD);   //erase 10个block 

	for(blk_num=0; blk_num<10; blk_num++)    //一个循环测试1个block
	{

		for(i=0;i<128;i++)	  //init buff
		{
			//write_buff[i]=0x55AA5A00+i;  
			write_buff[i]=0x05050505;  
			read_buff[i]=0;  
		}

		result= sdio_CMD24_write_single(blk_num, write_buff, EMMC_CARD);
		if(result)
		{
			return 1;
		}

		result= sdio_CMD17_read_single(blk_num, read_buff, EMMC_CARD);
		if(result)
		{
			return 2;
		}

		result=memcmp((UINT8 *)write_buff, (UINT8 *)read_buff, SDIO_BLOCK_SIZE);
		if(result)
		{
		  	printfS("write/read error \n");
			return 3;
		}

		printfS("write/read OK: blk_num= %d \n", blk_num);	

	}

	return 0;
}

UINT32 multiple_write_read_test(void)
{
	UINT32 result, i, blk_num ;
	UINT32 byte_cnt;  //单位：block size	

	for(byte_cnt=1;byte_cnt<=4;byte_cnt++) 	// 一次multiple操作中block的个数
	{   
		printfS("byte cnt = %d blocks \n" , byte_cnt);

		//sdio_CMD38_erase(0, 48, EMMC_CARD);    //erase 49个block 
		for(blk_num=0; blk_num<48; blk_num +=byte_cnt)	  //一个循环测试byte_cnt个block size
		{
			for(i=0;i<512;i++)	   //init buff
			{
				write_buff[i]=0xA5A5A5A5;  
				read_buff[i]=0;  
			}
	
			result= sdio_CMD25_write_multiple(blk_num, byte_cnt, write_buff, EMMC_CARD);
			if(result)
			{
				return 1;
			}
	
			result= sdio_CMD18_read_multiple(blk_num, byte_cnt, read_buff, EMMC_CARD);
			if(result)
			{
				return 2;
			}

			result=memcmp((UINT8 *)write_buff, (UINT8 *)read_buff, byte_cnt*SDIO_BLOCK_SIZE);
			if(result)
			{
			  	printfS("write/read error, (blk_num: (%d,%d))\n", blk_num, blk_num+ (byte_cnt-1) );
				return 3;
			}
			
			printfS("write/read OK: (%d,%d) \n", blk_num, blk_num+ (byte_cnt-1) );	

		}
	}
		
	return 0;	
}


void emmc_test(void)
{
	UINT32 result;
//	UINT32 div;
	
	printfS("---------EMMC test start------- \n");

	sdio_init(EMMC_CARD);
	
	printfS("\n---------EMMC enumerate test------- \n");
	result=0;
	result= emmc_enumerate();

	if(result) 	printfS("enumerate result---> fail \n" );
	else		printfS("enumerate result---> pass \n" );
	
	sdio_CMD9_get_csd(EMMC_CARD);
    
	sdio_CMD7_select(EMMC_CARD);	 
	
	printfS("switch to high speed mode \n" );	
	sdio_CMD6_switch(EXT_CSD_ARG_HS_TIMING);

	printfS("set block length \n" );	
	sdio_CMD16_set_blocklen(SDIO_BLOCK_SIZE);

	printfS("\n---------single write/read test ------- \n");
	result=0;
	result= single_write_read_test();
	if(result) 	printfS("result---> fail \n" );
	else		printfS("result---> pass \n" );

	printfS("\n---------multiple write/read test for width=1 ------- \n");
	result=0;
	result= multiple_write_read_test();
	if(result) 	printfS("result---> fail \n" );
	else		printfS("result---> pass \n" );

 	printfS("\n---------multiple write/read test for width=4 ------- \n");
	sdio_CMD6_switch_bus_width(BUS_BIT4);
	result=0;
	result= multiple_write_read_test();
	if(result) 	printfS("result---> fail \n" );
	else		printfS("result---> pass \n" );

 	printfS("\n---------multiple write/read test for width=8 ------- \n");
	sdio_CMD6_switch_bus_width(BUS_BIT8);
	result=0;
	result= multiple_write_read_test();
	if(result) 	printfS("result---> fail \n" );
	else		printfS("result---> pass \n" );
	
	printfS("\n---------erase test ------- \n");
	result=0;
	result= erase_test();
	if(result) 	printfS("result---> fail \n" );
	else		printfS("result---> pass \n" );

	printfS("\n---------EMMC test end-------- \n");				
}




UINT32 sd_erase_test(void)
{
	UINT32 result, i, blk_num ;

	result=0;
	result=sdio_CMD38_erase(0,100, SD_CARD);   
	if(result)
	{
		 return	1;
	}

	for(blk_num=0; blk_num<100; blk_num++)	 
	{
		for(i=0;i<(SDIO_BLOCK_SIZE/4);i++)	 
		{
			read_buff[i]=0x12345678;  //init buff
		}
		
		result= sdio_CMD17_read_single(blk_num, read_buff, SD_CARD);
		if(result)
		{
			return	2;
		}
		
		for(i=0;i<(SDIO_BLOCK_SIZE/4);i++)	 
		{
			if(read_buff[i] != 0x00)  
			{
				return	3;
			}
		}
	}

	return	0;
}

UINT32 sd_single_write_read_test(void)
{
	UINT32 result, i, blk_num ;

	//sdio_CMD38_erase(0, 10, SD_CARD); 

	for(blk_num=0; blk_num<10; blk_num++)    //一个循环测试1个block
	{
		printfS("blk_num= %d \n", blk_num);

		for(i=0;i<128;i++)	  //init buff
		{
			write_buff[i]=0x05050505;  
			read_buff[i]=0;  
		}

		result= sdio_CMD24_write_single(blk_num, write_buff, SD_CARD);
		if(result)
		{
			return 1;
		}

		result= sdio_CMD17_read_single(blk_num, read_buff, SD_CARD);
		if(result)
		{
			return 2;
		}

		result=memcmp((UINT8 *)write_buff, (UINT8 *)read_buff, SDIO_BLOCK_SIZE);
		if(result)
		{
			return 3;
		}
	}

	return 0;
}

UINT32 sd_multiple_write_read_test(void)
{
	UINT32 result, i, blk_num ;
	UINT32 byte_cnt;  //单位：block size	

	for(byte_cnt=1;byte_cnt<=4;byte_cnt++) 	// 一次multiple操作中block的个数
	{   
		printfS("byte cnt = %d blocks \n" , byte_cnt);

		//sdio_CMD38_erase(0, 48, SD_CARD);    
		for(blk_num=0; blk_num<48; blk_num +=byte_cnt)	  //一个循环测试byte_cnt个block size
		{
			printfS("blk_num: (%d,%d) \n", blk_num, blk_num+ (byte_cnt-1) );

			for(i=0;i<512;i++)	   //init buff
			{
				write_buff[i]=0xA5A5A5A5;  
				read_buff[i]=0;  
			}
	
			result= sdio_CMD25_write_multiple(blk_num, byte_cnt, write_buff, SD_CARD);
			if(result)
			{
				return 1;
			}
	
			result= sdio_CMD18_read_multiple(blk_num, byte_cnt, read_buff, SD_CARD);
			if(result)
			{
				return 2;
			}

			result=memcmp((UINT8 *)write_buff, (UINT8 *)read_buff, byte_cnt*SDIO_BLOCK_SIZE);
			if(result)
			{
			  	printfS("write/read error, (blk_num: (%d,%d))\n", blk_num, blk_num+ (byte_cnt-1) );
				return 3;
			}
		}
	}
		
	return 0;	
}



void sd_test(void)
{
	UINT32 result;

	sdio_init(SD_CARD);
	
	printfS("\n---------SD enumerate test------- \n");
	result=0;
	result= sd_enumerate();

	if(result)
	{
		printfS("result=%d---> fail \n" ,result );
		return;
	} 	
	printfS("result---> pass \n" );	
	
	sdio_CMD9_get_csd(SD_CARD);

	sdio_CMD7_select(SD_CARD);	 

	sdio_CMD16_set_blocklen(SDIO_BLOCK_SIZE);	
		
	sdio_ACMD6_set_bus_width(BUS_BIT1, SD_CARD);

	printfS("\n---------single write/read test ------- \n");
	result=0;
	result= sd_single_write_read_test();
	if(result) 	printfS("result---> fail \n" );
	else		printfS("result---> pass \n" );
   

	printfS("\n---------multiple write/read test for width=1 ------- \n");
	result=0;
	result= sd_multiple_write_read_test();
	if(result) 	printfS("result---> fail \n" );
	else		printfS("result---> pass \n" );

 	printfS("\n---------multiple write/read test for width=4 ------- \n");
	sdio_ACMD6_set_bus_width(BUS_BIT4, SD_CARD);
	result=0;
	result= sd_multiple_write_read_test();

	if(result) 	printfS("result---> fail \n" );
	else		printfS("result---> pass \n" );

	printfS("\n---------erase test ------- \n");
	result=0;
	result= sd_erase_test();
	if(result) 	printfS("result---> fail \n" );
	else		printfS("result---> pass \n" );


	printfS("\n---------SD test end-------- \n");				

}
