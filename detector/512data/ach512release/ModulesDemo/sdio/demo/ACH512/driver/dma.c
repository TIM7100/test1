
/***********************************************************************
 * Copyright (c)  2015,Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.

 * Filename    : DMA.c
 * Description : DMA driver
 * Author(s)   : chenbailing   
 * version     : 1.0
 * Modify date : 2015-05-14
 ***********************************************************************/


#include  "dma.h"

volatile UINT8 flag_dma_end_ch[8] = { 0 };
volatile UINT8 flag_dma_err_ch[8] = { 0 };

void DMA_IRQHandler(void)
{
	UINT8 i;
	for(i = 0; i < DMA_NUMBER; i++)
	{
		if(REG_DMACIntStatus & (0x01 << i))  //channel
		{
			if(REG_DMACIntTCStatus & (0x01 << i))
			{
				REG_DMACIntTCClr |= (0x01 << i);
				flag_dma_end_ch[i] = 1;
			}
			if(REG_DMACIntErrStatus & (0x01 << i))
			{
				REG_DMACIntErrClr |= (0x01 << i);
				flag_dma_err_ch[i] = 1;
			}
		}
	}
}

void dma_init(void)
{
	UINT8 i;
	/*IO口配置*/

	/*中断配置*/
	NVIC_ClearPendingIRQ(DMAC_IRQn);
	//NVIC_SetPriority(DMAC_IRQn, 1);
	NVIC_EnableIRQ(DMAC_IRQn);

	REG_DMACConfiguration |= 0x01; //enable DMAC
	REG_DMACSync |= 0x01;          //disable synchronize request

	//关闭所有的DMA通道
	for(i = 0; i < DMA_NUMBER; i++)
	{
		if(REG_DMACEnChnStatus & (1 << i))
		{
			REG_DMACCConfiguration(i) &= ~0x01;
			while(REG_DMACEnChnStatus & (1 << i)); //等改关闭DMA通道完成
		}

		if(REG_DMACRawIntTCStatus & (0x01 << i))
		{
			REG_DMACIntTCClr |= 0x01 << i;
		}
	}

}

void dma_startch(UINT8 channel_index, UINT8 flowctrl, UINT8 inten)
{
	channel_index &= 0x07; //取低三位
	flowctrl &= 0x07;
	inten &= 0x01;
	if(inten == 0)
	{
		REG_DMACCConfiguration(channel_index) = (REG_DMACCConfiguration(channel_index) & ~((0x07 << 11) | DMACC_ITCM)) |
				TRANS_TYPE(flowctrl) | START_DMA;
		while(!(REG_DMACRawIntTCStatus & (0x01 << channel_index)));
		REG_DMACIntTCClr |= 0x01 << channel_index;

	} else if(inten == 1)
	{

		REG_DMACCConfiguration(channel_index) =  (REG_DMACCConfiguration(channel_index) & ~(0x07 << 11)) |
				DMACC_ITCM | TRANS_TYPE(flowctrl) | START_DMA;
		while(!(flag_dma_end_ch[channel_index]));
		flag_dma_end_ch[channel_index] = 0;
	}
}

void dma_transfer(UINT8 channel_index, UINT32 dest_addr, UINT32 src_addr, UINT32 length)
{
	channel_index &= 0x07; //取低三位
	REG_DMACSync |= 0x01;  //disable synchronize request
	REG_DMACConfiguration |= 0x01; //打开DMA模块

	REG_DMACCDestAddr(channel_index) = dest_addr;
	REG_DMACCSrcAddr(channel_index) = src_addr;
	REG_DMACCControl(channel_index) |= 0x80000000;
	REG_DMACCControl(channel_index) = (REG_DMACCControl(channel_index) & 0xF3000000) | DEST_INC | SRC_INC |
			DEST_WIDTH(WIDTH_32BIT) | SRC_WIDTH(WIDTH_32BIT) | DEST_BURST_SIZE(BURST_8) | SRC_BURST_SIZE(BURST_8) | (length >> WIDTH_32BIT);
}

void dma_stopch(UINT8 channel_index)
{
	REG_DMACCConfiguration(channel_index) &= ~0x01;
	while(REG_DMACEnChnStatus & (0x01 << channel_index)); //等改关闭DMA通道完成
}


/**********************************测试使用函数************************************************/
void dma_struct_init(DMA_InitTypeDef *DMA_Chnx)
{
	DMA_Chnx->dest_addr = (UINT32)(&g_data_buf[256]);
	DMA_Chnx->src_addr = (UINT32)(&g_data_buf[0]);
	DMA_Chnx->link_addr = NULL;

	DMA_Chnx->tricount = 1;
	DMA_Chnx->dest_addr_inc = 1;
	DMA_Chnx->src_addr_inc = 1;
	DMA_Chnx->dest_master = 0;
	DMA_Chnx->src_master = 0;
	DMA_Chnx->mode_with = 2;
	DMA_Chnx->mode_size = 2;
	DMA_Chnx->length = 1024;

	DMA_Chnx->inten = 0;
	DMA_Chnx->interror = 0;
	DMA_Chnx->flowctrl = 0;
}

void dma_app_init(UINT8 channel_index, DMA_InitTypeDef *DMA_Chnx)
{
	UINT32 mode, with, size, length;

	//整理数据格式
	channel_index &= 0x07; //取低三位

	DMA_Chnx->tricount &= 0x01;
	DMA_Chnx->dest_addr_inc &= 0x01;
	DMA_Chnx->src_addr_inc &= 0x01;
	DMA_Chnx->dest_master &= 0x01;
	DMA_Chnx->src_master &= 0x01;
	DMA_Chnx->mode_with &= 0x07;
	DMA_Chnx->mode_size &= 0x07;

	with = DMA_Chnx->mode_with | DMA_Chnx->mode_with << 3;
	size = DMA_Chnx->mode_size | DMA_Chnx->mode_size << 3;
	mode = with << 6 | size;

//	tmp = (0x01 << (with & 0x07)) & 0x07;
	length = DMA_Chnx->length >> DMA_Chnx->mode_with;
	length &= 0x0fff;

	DMA_Chnx->inten &= 0x01;
	DMA_Chnx->interror &= 0x01;
	DMA_Chnx->flowctrl &= 0x07;

	//给寄存器赋值
	REG_DMACCSrcAddr(channel_index) = DMA_Chnx->src_addr;
	REG_DMACCDestAddr(channel_index) = DMA_Chnx->dest_addr;
	REG_DMACCLinkList(channel_index) = DMA_Chnx->link_addr;
	REG_DMACCControl(channel_index) = (REG_DMACCControl(channel_index) & 0x00000000) | DMA_Chnx->tricount << 31 | DMA_Chnx->dest_addr_inc << 27 | DMA_Chnx->src_addr_inc << 26 |
			DMA_Chnx->dest_master << 25 | DMA_Chnx->src_master << 24 | mode << 12 | length;
	REG_DMACCConfiguration(channel_index) = (REG_DMACCConfiguration(channel_index) & 0xfffc0000) | DMA_Chnx->inten << 15 | DMA_Chnx->interror << 14 | DMA_Chnx->flowctrl << 11;

}

void dma_app_cmd(UINT8 channel_index, UINT8 new_state)
{
	new_state &= 0x01;
	if(new_state == 1)
	{
		REG_DMACCConfiguration(channel_index) |= START_DMA;
	} else
	{
		REG_DMACCConfiguration(channel_index) &= ~START_DMA;
		while((REG_DMACEnChnStatus & (0x01 << channel_index)) != 0x00); //等改DMA通道关闭完成
	}
}

void dma_app_itconfig(UINT8 channel_index, UINT32 my_int, UINT8 new_state)
{
	new_state &= 0x01;
	if(new_state == 1)
	{
		REG_DMACCConfiguration(channel_index) &= ~my_int;
	} else
	{
		REG_DMACCConfiguration(channel_index) |= my_int;
	}
}


