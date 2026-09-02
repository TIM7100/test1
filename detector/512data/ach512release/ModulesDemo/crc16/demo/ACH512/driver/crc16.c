/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : crc16.c
 * Description : crc16 driver source file
 * Author(s)   : Eric
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#include  "crc16.h"

/************************************************************************
 * function   : crc16_ccitt
 * Description: CRC-CCITT=X16+X12+X5+1
 * input :
 *         UINT8[] crc_data: crc16 indata
 *         UINT32 len: data length
 *         UINT16 init_data: init data
 * return: UINT16 -- data
 ************************************************************************/
UINT16 crc16_ccitt(UINT8 crc_data[], UINT32 len, UINT16 init_data) {
    UINT32 i;
#ifdef LOW_POWER
    enable_module(BIT_CRC16 | BIT_UAC );
#endif		
    REG_CRC16_CTRL = 0x07; //normal:input, out, init all no reverse
    REG_CRC16_INIT = init_data;
    for (i = 0; i < len; i++) {
        REG_CRC16_DATA = crc_data[i];
    }
    return REG_CRC16_DATA;
}


