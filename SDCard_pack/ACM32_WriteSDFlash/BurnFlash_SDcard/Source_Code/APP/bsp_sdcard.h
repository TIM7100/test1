#ifndef __BSP_SDCARD_H__
#define __BSP_SDCARD_H__

#include "ACM32Fxx_HAL.h"

/* Data token start byte */
#define SD_START_DATA_SINGLE_BLOCK_READ    0xFE
#define SD_START_DATA_MULTIPLE_BLOCK_READ  0xFE
#define SD_START_DATA_SINGLE_BLOCK_WRITE   0xFE
#define SD_START_DATA_MULTIPLE_BLOCK_WRITE 0xFC
#define SD_STOP_DATA_MULTIPLE_BLOCK_WRITE  0xFD

typedef enum {
    SD_RESPONSE_NO_ERROR       = (0x00),
    SD_IN_IDLE_STATE           = (0x01),
    SD_ERASE_RESET             = (0x02),
    SD_ILLEGAL_COMMAND         = (0x04),
    SD_COM_CRC_ERROR           = (0x08),
    SD_ERASE_SEQUENCE_ERROR    = (0x10),
    SD_ADDRESS_ERROR           = (0x20),
    SD_PARAMETER_ERROR         = (0x40),
    SD_RESPONSE_FAILURE        = (0xFF),

    SD_DATA_OK                 = (0x05),
    SD_DATA_CRC_ERROR          = (0x0B),
    SD_DATA_WRITE_ERROR        = (0x0D),
    SD_DATA_OTHER_ERROR        = (0xFF)
} SD_Error;

/* SD commands */
#define SD_CMD_GO_IDLE_STATE          0
#define SD_CMD_SEND_OP_COND           1
#define SD_CMD_SEND_IF_COND           8
#define SD_CMD_SEND_CSD               9
#define SD_CMD_SEND_CID               10
#define SD_CMD_STOP_TRANSMISSION      12
#define SD_CMD_SEND_STATUS            13
#define SD_CMD_SET_BLOCKLEN           16
#define SD_CMD_READ_SINGLE_BLOCK      17
#define SD_CMD_READ_MULT_BLOCK        18
#define SD_CMD_SET_BLOCK_COUNT        23
#define SD_CMD_WRITE_SINGLE_BLOCK     24
#define SD_CMD_WRITE_MULT_BLOCK       25
#define SD_CMD_PROG_CSD               27
#define SD_CMD_SET_WRITE_PROT         28
#define SD_CMD_CLR_WRITE_PROT         29
#define SD_CMD_SEND_WRITE_PROT        30
#define SD_CMD_SD_ERASE_GRP_START     32
#define SD_CMD_SD_ERASE_GRP_END       33
#define SD_CMD_UNTAG_SECTOR           34
#define SD_CMD_ERASE_GRP_START        35
#define SD_CMD_ERASE_GRP_END          36
#define SD_CMD_UNTAG_ERASE_GROUP      37
#define SD_CMD_ERASE                  38
#define SD_CMD_READ_OCR               58
#define SD_CMD_APP_CMD                55
#define SD_ACMD_SD_SEND_OP_COND       41

/* SD card type */
#define SD_TYPE_NOT_SD   0
#define SD_TYPE_V1       1
#define SD_TYPE_V2       2
#define SD_TYPE_V2HC     4

uint8_t SD_Initialize(void);
uint8_t SD_ReadDisk(uint8_t* RxBuf, uint32_t StartSector, uint8_t Cnt);
uint8_t SD_WriteDisk(uint8_t* TxBuf, uint32_t StartSector, uint8_t Cnt);

#endif