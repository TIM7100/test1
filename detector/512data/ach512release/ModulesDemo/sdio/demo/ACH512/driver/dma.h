
#ifndef __DMA_H__
#define __DMA_H__

#include  "common.h"

//#define DMA_INT



/*----------------------DMA BIT------------------------*/

#define CH0                     0
#define CH1         	        1
#define CH2         			2
#define CH3                     3
#define CH4         			4
#define CH5         			5
#define CH6         			6
#define CH7         			7

#define TRICOUNT                (1<<31)
#define DEST_INC                (1<<27)
#define SRC_INC		    		(1<<26)

#define WIDTH_8BIT      		0
#define WIDTH_16BIT   			1
#define WIDTH_32BIT      		2
#define DEST_WIDTH(x)  			(x<<21)
#define SRC_WIDTH(x)     		(x<<18)

#define BURST_1                   0x00
#define BURST_4                   0x01
#define BURST_8                   0x02
#define BURST_16				  0x03
#define BURST_32				  0x04
#define BURST_64				  0x05
#define BURST_128				  0x06
#define BURST_256				  0x07
#define DEST_BURST_SIZE(x)       (x<<15)
#define SRC_BURST_SIZE(x)        (x<<12)

#define DMA_TYPE                  0
#define MEM_TO_MEM                0
#define MEM_TO_PER                1
#define PER_TO_MEM                2
#define TRANS_TYPE(x)            (x<<11)
#define START_DMA               (1<<0)
#define DMACC_ITCM              (1<<15)
//中断号待确认
#define DMAC_IRQn                18

#define DMA_NUMBER               2


extern volatile UINT8 flag_dma_end_ch[8];
extern volatile UINT8 flag_dma_err_ch[8];

typedef struct dma_nextlink {
    UINT32 src_addr;
    UINT32 dest_addr;
    struct dma_nextlink *next;
    UINT32 control;
} DMA_NeLli_InitTypeDef;


typedef struct {
//  UINT8 channel_index;
    UINT32 src_addr;
    UINT32 dest_addr;
    UINT32 link_addr;

    UINT8 tricount;
    UINT8 dest_addr_inc;
    UINT8 src_addr_inc;
    UINT8 dest_master;
    UINT8 src_master;
    UINT8 mode_with;
    UINT8 mode_size;
    UINT32 length;

    UINT8 inten;
    UINT8 interror;
    UINT8 flowctrl;

} DMA_InitTypeDef;



/*大端小端互换
#define BigtoLittle32(A)    ((((UINT32)(A) & 0xff000000) >> 24) | (((UINT32)(A)& 0x00ff0000)>> 8)| \
(((UINT32)(A)& 0x0000ff00)<< 8)| (((UINT32)(A)& 0x000000ff)<< 24))
#define BigtoLittle16(A)    ((((UINT16)(A) & 0xff00) >> 8)|(((UINT16)(A) & 0x00ff) << 8))
*/
UINT8 IsBigEndian(void);
void dma_init(void);
void dma_startch(UINT8 channel_index, UINT8 flowctrl, UINT8 inten);
void dma_transfer(UINT8 channel_index, UINT32 dest_addr, UINT32 src_addr, UINT32 length);
void dma_stopch(UINT8 channel_index);

//test
void dma_struct_init(DMA_InitTypeDef *DMA_Chnx);
void dma_app_init(UINT8 channel_index, DMA_InitTypeDef *DMA_Chnx);
void dma_app_cmd(UINT8 channel_index, UINT8 new_state);
void dma_app_itconfig(UINT8 channel_index, UINT32 my_int, UINT8 new_state);

#endif
