#ifndef __BSP_SDCARD_H__
#define __BSP_SDCARD_H__


#include "fxx_std.h"

/**
* @brief  Start Data tokens:
*    Tokens (necessary because at nop/idle (and CS active) only 0xff is
*         on the data/command line)
*/
/*!< Data token start byte, 单块读起始Token */
#define SD_START_DATA_SINGLE_BLOCK_READ    0xFE

/*!< Data token start byte, 多块读起始Token */
#define SD_START_DATA_MULTIPLE_BLOCK_READ  0xFE

/*!< Data token start byte, 单块写起始Token */
#define SD_START_DATA_SINGLE_BLOCK_WRITE   0xFE

/*!< Data token start byte, 多块写起始Token */
#define SD_START_DATA_MULTIPLE_BLOCK_WRITE 0xFC

/*!< Data toke stop byte, 多块写停止Token */
#define SD_STOP_DATA_MULTIPLE_BLOCK_WRITE  0xFD


typedef enum {
    /**
    * @brief  SD卡对命令的响应
    */
    SD_RESPONSE_NO_ERROR       = (0x00),		//命令成功响应
    SD_IN_IDLE_STATE           = (0x01),
    SD_ERASE_RESET             = (0x02),
    SD_ILLEGAL_COMMAND         = (0x04),
    SD_COM_CRC_ERROR           = (0x08),
    SD_ERASE_SEQUENCE_ERROR    = (0x10),
    SD_ADDRESS_ERROR           = (0x20),
    SD_PARAMETER_ERROR         = (0x40),
    SD_RESPONSE_FAILURE        = (0xFF),

    /*
    * @brief  主机向从机写入一个数据块后，会收到SD卡的数据响应称为TOKEN
    */
    SD_DATA_OK                 = (0x05),		//主机写入SD卡的数据被接收
    SD_DATA_CRC_ERROR          = (0x0B),		//因为CRC校验失败，数据被拒收
    SD_DATA_WRITE_ERROR        = (0x0D),		//写入错误，数据被拒收
    SD_DATA_OTHER_ERROR        = (0xFF)		
} SD_Error;


/**
  * @brief  SD卡的命令宏定义，Commands: CMDxx = CMD-number | 0x40
  */
#define SD_CMD_GO_IDLE_STATE          0   	/*!< CMD0 = 0x40 */
#define SD_CMD_SEND_OP_COND           1   	/*!< CMD1 = 0x41 */
#define SD_CMD_SEND_IF_COND			  8	  	/*!< CMD8 = 0x48 */
#define SD_CMD_SEND_CSD               9   	/*!< CMD9 = 0x49 */
#define SD_CMD_SEND_CID               10  	/*!< CMD10 = 0x4A */
#define SD_CMD_STOP_TRANSMISSION      12  	/*!< CMD12 = 0x4C */
#define SD_CMD_SEND_STATUS            13  	/*!< CMD13 = 0x4D */
#define SD_CMD_SET_BLOCKLEN           16  	/*!< CMD16 = 0x50 */
#define SD_CMD_READ_SINGLE_BLOCK      17  	/*!< CMD17 = 0x51 */
#define SD_CMD_READ_MULT_BLOCK        18  	/*!< CMD18 = 0x52 */
#define SD_CMD_SET_BLOCK_COUNT        23  	/*!< CMD23 = 0x57 */
#define SD_CMD_WRITE_SINGLE_BLOCK     24  	/*!< CMD24 = 0x58 */
#define SD_CMD_WRITE_MULT_BLOCK       25  	/*!< CMD25 = 0x59 */
#define SD_CMD_PROG_CSD               27  	/*!< CMD27 = 0x5B */
#define SD_CMD_SET_WRITE_PROT         28  	/*!< CMD28 = 0x5C */
#define SD_CMD_CLR_WRITE_PROT         29  	/*!< CMD29 = 0x5D */
#define SD_CMD_SEND_WRITE_PROT        30  	/*!< CMD30 = 0x5E */
#define SD_CMD_SD_ERASE_GRP_START     32  	/*!< CMD32 = 0x60 */
#define SD_CMD_SD_ERASE_GRP_END       33  	/*!< CMD33 = 0x61 */
#define SD_CMD_UNTAG_SECTOR           34  	/*!< CMD34 = 0x62 */
#define SD_CMD_ERASE_GRP_START        35  	/*!< CMD35 = 0x63 */
#define SD_CMD_ERASE_GRP_END          36  	/*!< CMD36 = 0x64 */
#define SD_CMD_UNTAG_ERASE_GROUP      37  	/*!< CMD37 = 0x65 */
#define SD_CMD_ERASE                  38  	/*!< CMD38 = 0x66 */

#define SD_CMD_READ_OCR				  58    /*!< CMD58 */
#define SD_CMD_APP_CMD				  55    /*!< CMD55 返回0x01*/
#define SD_ACMD_SD_SEND_OP_COND		  41    /*!< ACMD41  返回0x00*/

//SD卡的类型
#define SD_TYPE_NOT_SD   0	   //非SD卡
#define SD_TYPE_V1       1	  
#define SD_TYPE_V2       2    
#define SD_TYPE_V2HC     4	  





u8 SD_Initialize(void);
u8 SD_ReadDisk(u8* RxBuf, u32 StartSector, u8 Cnt);
u8 SD_WriteDisk(u8* TxBuf, u32 StartSector, u8 Cnt);

#endif
