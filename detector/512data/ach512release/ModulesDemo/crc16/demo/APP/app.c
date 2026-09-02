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
#include  "crc16.h"

void crc16_test(void)
{
    UINT16 result;
    UINT16 i;
    UINT8 crc_data[256];

    for (i = 0; i < 256; i++)
    {
        crc_data[i] = i;
    }

    result = crc16_ccitt(crc_data, 128, 0x1234);
	
    printfS("result = %x\n", result);

    printfS("\nACH512 CRC16 Module Test Finished!\n\n");
}


