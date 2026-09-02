#ifndef __SDIO_H__
#define __SDIO_H__
#include  "common.h"

typedef struct
{
	UINT32 ocr;
	UINT32 cid[4];
	UINT32 csd[4];
	UINT32 rca;
	UINT8 hcs; //capacity staus, 1:high capacity ,0: standrad capacity
}SDIO_REG_INFO;


#define	EMMC_CARD	0	
#define SD_CARD		1	

#define	SDIO_CH0	0	
//#define	SDIO_CH1	1



#define EMMC_CLK  50	//单位M
#define SD_CLK    50	//单位M

//#define	INT_MODE		
#define	SDIO_INT_EN		(1<<4)	
#define	SDIO_OD_PULLUP_EN	(1<<24)
#define	SDIO_POWER_ON(x)	(1<<x)		  //(x:sdio_index )
			
#define	SDIO_DIV0			(0<<0)				//div0：不分频
#define	SDIO_DIV1			(1<<8)				//div1：2
#define	SDIO_DIV2			(1<<16)				//div2：2
#define	SDIO_DIV3			(1<<24)				//div3：2

#define	SDIO_CLK_SRC_DIV0	0			//clk src from DIV0
#define	SDIO_CLK_SRC_DIV1	1			//clk src from DIV1 
#define	SDIO_CLK_SRC_DIV2	2			//clk src from DIV2
#define	SDIO_CLK_SRC_DIV3	3			//clk src from DIV3

/*//#define	SDIO_C0_CLK_SRC		(0<<0)			//card 0 clk src from DIV0
//#define	SDIO_C0_CLK_SRC	(1<<0)			//card 0 clk src from DIV1
//#define	SDIO_C0_CLK_SRC	(2<<0)			//card 0 clk src from DIV2
//#define	SDIO_C0_CLK_SRC	(3<<0)			//card 0 clk src from DIV3

//#define	SDIO_C1_CLK_SRC		(0<<2)				//card 1 clk src from DIV0
//#define	SDIO_C1_CLK_SRC	(1<<2)			//card 1 clk src from DIV1
//#define	SDIO_C1_CLK_SRC	(2<<2)			//card 1 clk src from DIV2
//#define	SDIO_C1_CLK_SRC	(3<<2)			//card 1 clk src from DIV3	 */
#define	SDIO_LOW_POWER(x)		(0x10000<<x)	//lowpower mode	   (x:sdio_index )
#define	SDIO_ENABLE_CCK(x)		(1<<x)			//enable CCK out   (x:sdio_index )

#define	SDIO_TIMEOUT_RW		(0xFFFFFF00)//(0x700000<<8)		//timeout for RW
#define	SDIO_TIMEOUT_RESP		(0xFF<<0)			//timeout for response


#define	BUS_BIT8		0x10000						//8bit mode
#define	BUS_BIT4		0x01						//4bit mode
#define	BUS_BIT1		0x00						//1bit mode
#define	SDIO_WIDTH(x)			(x)					//card 0 width  (x:sdio_index )
#define	SDIO_BLOCK_SIZE		512		
#define	SDIO_FIFO_SIZE			64					
#define	SDIO_RX_LEVEL			(7<<16)				//Rx level:32 bytes	
#define	SDIO_TX_LEVEL			(8<<0)				//Tx level:32 bytes	
#define	SDIO_DEBOUNCE_COUNT	0xFFFFF;	
#define	SDIO_START_CMD			(0x80000000)		//start cmd	
#define	SDIO_UPDATE_CLK_ONLY	(1<<21)				//update clk only	
#define	SDIO_WAIT_PRVDATA_END	(1<<13)				//wait prv data finish	

#define EXT_CSD_ARG_HS_TIMING	0x03B90100

#define CMD0_GO_IDLE					0x8000C000     //go idle
#define CMD1_MATCH_VCC					0x80000041     //turn on for match vcc //only for EMMC
#define CMD2_CID						0x800001c2     //initial card get CID
#define CMD3_RCA						0x80000143     //comfire/read RCA to card
#define CMD6_SWTICH					    0x80002146     //switch function
#define CMD7_SELECT_CARD				0x80002147     //select card
#define CMD8_EXT_CSD_SD					0x80002148     //get EXT_CSD (value on CMD line )
#define CMD8_EXT_CSD_EMMC				0x80002348     //get EXT_CSD (value on DATA line )
#define CMD9_CSD						0x800021c9     //get CSD
//#define CMD10_CID						0x800021ca     //get CID at transfers
//#define CMD12_STOP_STEARM				0x8000614c     //stop block transfers 
#define CMD13_GET_STATUS				0x8000214D     //get card status
#define CMD14_BUSTEST_R				    0x8000234E     //bus test read
#define CMD15_INACTIVE					0x8000610F     //make card to inactive
#define CMD16_SET_BLOCKLEN				0x80002150     //set card block length
#define CMD17_READ_SINGLE				0x80002351     //single block read
#define CMD18_READ_MUL					0x80003352     //multipe block read
#define CMD19_BUSTEST_W					0x80002753     //bus test
//#define CMD23_PRE_ERASE					0x80002157     //pre erase for write
#define CMD24_WRITE_SINGLE				0x80002758     //single block write
#define CMD25_WRITE_MUL					0x80003759     //multipe block write
//#define CMD27_PROG_CSD					0x8000275b     //programme csd
//#define CMD28_SET_PROTECT				0x8000215c     //set protect
#define CMD29_CLR_PROTECT				0x8000215d     //clearn protect
#define CMD30_SEND_WRITE				0x8000235e     //get the status about protect
#define CMD32_ERASESD_START				0x80002160     //set start erase SD card address
#define CMD33_ERASESD_END				0x80002161     //set end erase SD card address
#define CMD35_ERASESD_START				0x80002163     //set start erase EMMC card address
#define CMD36_ERASESD_END				0x80002164     //set end erase EMMC card address
#define CMD38_ERASE						0x80002166     //confirm erase
//#define CMD40_INT_MODE					0x80002168     //into interrput mode ( Class 9命令，三星KLMxGxxEMx 不支持)

#define CMD55_APP						0x80002177     //APP CMD
#define ACMD6_SET_BUS_WIDTH			    0x80002146     //only for SD
#define ACMD13_GET_STATUS				0x8000234d     //get status 512 bit
#define ACMD41_SEND_COND				0x80002069     //FOR SD
#define ACMD42_DISCON_DATA3				0x8000216a     //make data3 disconnect
#define ACMD51_GET_CSR					0x80002373     //get card csr

#define	EMMC_RCA_VALUE					0x1234

#define	STANDRAD_CAPACITY		0  	//standard capacity
#define	HIGH_CAPACITY			1  	//high capacity
#define EMMC_HCS	HIGH_CAPACITY
#define SD_HCS		HIGH_CAPACITY
#define EMMC_VDD_WINDOW 0x00FF8000   //2.7~3.6v 
#define SD_VDD_WINDOW 	0x00FF8000   //2.7~3.6v 


void sdio_init(UINT8 card_type);
UINT8 sdio_CMD6_switch(UINT32 cmd_arg);
UINT8 sdio_CMD6_switch_bus_width(UINT32 bus_width);
UINT8 sdio_CMD7_select(UINT8 card_type);
UINT8 sdio_CMD8_get_ext_csd(UINT32 *buff, UINT8 card_type);
UINT8 sdio_CMD9_get_csd(UINT8 card_type);
UINT8 sdio_CMD16_set_blocklen(UINT32 block_size);
UINT8 sdio_CMD17_read_single(UINT32 blk_addr, UINT32 *buff, UINT8 card_type) ;
UINT8 sdio_CMD18_read_multiple(UINT32 blk_addr, UINT32 blk_num, UINT32 *buff, UINT8 card_type);
UINT8 sdio_CMD24_write_single(UINT32 blk_addr, UINT32 *buff, UINT8 card_type)	;
UINT8 sdio_CMD25_write_multiple(UINT32 blk_addr, UINT32 blk_num, UINT32 *buff, UINT8 card_type);
UINT8 sdio_CMD38_erase(UINT32 blk_start_addr, UINT32 blk_num, UINT8 card_type);
UINT8 sdio_ACMD6_set_bus_width(UINT32 bus_width, UINT8 card_type);
UINT8 sdio_ACMD41_send_cond(UINT8 card_type);
UINT8 sdio_CMD55_app(UINT8 card_type);
UINT8 sdio_cmd_nodata(UINT32 cmd, UINT32 cmd_arg, UINT8 sdio_index);
UINT32 sdio_cmd_read_data(UINT32 cmd, UINT32 cmd_arg, UINT32 length, UINT32 *buff, UINT8 sdio_index);
UINT32 sdio_cmd_write_data(UINT32 cmd, UINT32 cmd_arg, UINT32 length, UINT32 *buff, UINT8 sdio_index);
UINT8 emmc_enumerate(void);
UINT8 sd_enumerate(void);
void sdio_set_clock(UINT32 div, UINT32 clk_src, UINT8 sdio_index);


extern volatile UINT32 flag_int_sdio;
extern SDIO_REG_INFO emmc_reg_info;
extern SDIO_REG_INFO sd_reg_info;


#endif


