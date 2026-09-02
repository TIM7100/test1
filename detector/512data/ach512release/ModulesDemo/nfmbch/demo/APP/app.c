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
#include  "nfm.h"
#include  "bch.h"

UINT32 g_data_buf[1024];

#define write_buffer_byte      ((UINT8 *)(g_data_buf))
#define read_buffer_byte       ((UINT8 *)(g_data_buf + 512))


void nandflash_test(void)
{
	UINT32 i;
	UINT8 nand_id[6];
	UINT32 page_addr;
	nfm_init();
	flash_reset();
	flash_get_id(nand_id, 6);
	printfS("Read ChipID:");
	printfB8(nand_id, 6);

	//erase a blcok
	printfS("\nTest Erase:\n");
	page_addr = 0;
	flash_erase(page_addr); //块擦除
	bch_flash_read(page_addr, 0, read_buffer_byte, 0); //非ecc模式只读512字节
	for(i = 0; i < 512; i++)
	{
		if(read_buffer_byte[i] != 0xff)
		{
			printf("Error at %d:%x.\n", i, read_buffer_byte[i]);
		}
	}
	printfS("Test Erase is over:\n\n");

	//Program withoutECC
	for(i = 0; i < 512; i++)
	{
		write_buffer_byte[i] = (UINT8)i;

	}
	printfS("Test Program withoutECC:\n");
	bch_flash_program(page_addr, 0, write_buffer_byte, 0); //wtihoutecc mode
	bch_flash_read(page_addr, 0, read_buffer_byte, 0);
	printfS("\nbch_flash_read_withoutecc:\n");
	printfB8(read_buffer_byte, BCH_SECTOR_SIZE);

	//Program withECC
	bch_flash_program(page_addr, 0, write_buffer_byte, 1); //ecc mode
	if(!bch_flash_read(page_addr, 0, read_buffer_byte, 1))
	{
		printfS("\nbch_flash_read_withecc:\n");
		printfB8(read_buffer_byte, BCH_SECTOR_SIZE );
	}
	else
	{
		printfS("\nbch_flash_read_withecc:ecc bch fail\n"); 
	}
	
	printfS("\nTypical_Nandflash_Process_Test is over\n");
}

