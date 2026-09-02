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
#include  "dma.h"

UINT32 g_data_buf[1024];

#define SRC_ADDR 	 (UINT32)(&g_data_buf[0])
#define DEST_ADDR 	 (UINT32)(&g_data_buf[256])

typedef struct dma_nextlink
{
	UINT32 src_addr;
	UINT32 dest_addr;
	struct dma_nextlink *next;
	UINT32 control;
} DMA_NEXT_LINK;

void dma_normal_test(void)
{
	UINT8  result,i;
	/*
	i=0:test DMA channel_0
	i=1:test DMA channel_1
	i=2:test DMA channel_2
	i=3:test DMA channel_3
	*/
	for(i = 0; i < DMA_NUMBER;i++)
	{
		printfS("---------DMA channel_%d test-------\n",i);

		memset(g_data_buf, 0xaa, 1024);
		dma_init();
		dma_transfer(i, DEST_ADDR, SRC_ADDR, 1024);

		result = memcmp((UINT32 *)DEST_ADDR, (UINT32 *)SRC_ADDR, 1024);
		if(result)
		{
			printfS("dma test fail \n");
		}
		else
		{
			printfS("dma test pass \n");
		}

	}
	printfS("---------dma test test end-------\n\n");
}

void dma_link_test(void)
{
//测试链表,链表初始化
	UINT8 result;
	DMA_NEXT_LINK dma_link1;

	printfS("\n-----linklist test-----\n");
	memset(&g_data_buf[0], 0xcc, 256);
	memset(&g_data_buf[128], 0xdd, 256);
//DMA源地址,目标地址,链表地址初始化
	REG_DMAC_ChSrcAddr(DMACH0) =  (UINT32)(&g_data_buf[0]);
	REG_DMAC_ChDestAddr(DMACH0) = (UINT32)(&g_data_buf[256]);
	REG_DMAC_ChLinkList(DMACH0) = (UINT32)&dma_link1;
	REG_DMAC_ChCtrl(DMACH0) = 0x80000000 | DEST_INC | SRC_INC |
			DEST_WIDTH(WIDTH_32BIT) | SRC_WIDTH(WIDTH_32BIT) | DEST_BURST_SIZE(BURST_8) | SRC_BURST_SIZE(BURST_8) | (256 >> WIDTH_32BIT);

//链表初始化
	dma_link1.src_addr = (UINT32)&g_data_buf[128];
	dma_link1.dest_addr = (UINT32)&g_data_buf[384];
	dma_link1.next = NULL;
	dma_link1.control =  0x80000000 | DEST_INC | SRC_INC |
			DEST_WIDTH(WIDTH_32BIT) | SRC_WIDTH(WIDTH_32BIT) | DEST_BURST_SIZE(BURST_8) | SRC_BURST_SIZE(BURST_8) | (256 >> WIDTH_32BIT);

	REG_DMAC_ChConfig(DMACH0) =  DMACC_ITC | TRANS_TYPE(MEM_TO_MEM) | START_DMA;

//等待DMA传输完成
	while(REG_DMAC_RawIntTCStatus & (0x01 << DMACH0) == 0x00);
	REG_DMAC_IntTCClr = 0x01;

	result = memcmp((UINT32 *)DEST_ADDR, (UINT32 *)SRC_ADDR, 1024);
	if(result)
	{
		printfS("dma test fail \n");
	}
	else
	{
		printfS("dma test pass \n");
	}
	printfS("---------dma test test end-------\n");
}

void dma_test(void)
{
	dma_normal_test(); //dma mem to mem transfer data
	dma_link_test(); //dma mem to mem transfer data use link

}

