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
#include  "eflash.h"

#define SN_BASE_ADDR     	( EFlashNVR2BaseAddr + 0x08 )  //SN base addr

#define LOT_ID_ADDR     	( EFlashNVR2BaseAddr + 0x08 )  //LOT ID
#define WAFER_ID_ADDR   	( EFlashNVR2BaseAddr + 0x0C )  //Wafer ID
#define DIE_LOC_Y_ADDR    	( EFlashNVR2BaseAddr + 0x10 )  //DIE LOC Y
#define DIE_LOC_X_ADDR    	( EFlashNVR2BaseAddr + 0x12 )  //DIE LOC X
#define DATE_DAY_ADDR       ( EFlashNVR2BaseAddr + 0x14 )  //DATE_DAY
#define DATE_MONTH_ADDR     ( EFlashNVR2BaseAddr + 0x15 )  //DATE_MONTH
#define DATE_YEAR_ADDR      ( EFlashNVR2BaseAddr + 0x16 )  //DATE_YEAR
#define SN_CRC_ADDR     	( EFlashNVR2BaseAddr + 0x18 )  //SN CRC value


/*----------------------------SN--------------------------------*/
//CRC16-CCITT校验算法
#define CRC_INIT	0xffff	  //在CRC16-CCITT标准中reg_init = 0xffff
UINT16 do_crc(UINT32 addr, UINT32 len, UINT16 crc_init)
{
    UINT32 i, j;
    UINT16 crc_reg;      //reg for calculate CRC value
    UINT16 current;

    crc_reg = crc_init;  //initial value for CRC16-CCITT, GOOD_CRC = 0xf0b8
    for(i = 0; i < len; i++)
    {
        current = *(volatile UINT8 *)(addr + i);
        for(j = 0; j < 8; j++)
        {
            if((crc_reg ^ current) & 0x0001) crc_reg = (crc_reg >> 1) ^ 0x8408;  //CRC16-CCITT的生成多项式是	0x1021,将0x1021按位颠倒后为：0x8408
            else crc_reg >>= 1;
            current >>= 1;
        }
    }
    return crc_reg;
}

UINT16 check_crc_sn(void)
{
    UINT16 crc_value = 0;
    UINT16 crc_count = 0;
    UINT32 temp;
    temp = (*(volatile UINT32 *)(SN_CRC_ADDR));
    crc_value = temp & 0xFFFF;
    temp  = (~temp) >> 16;
    if(temp != crc_value)
    {
        return 0xFFFF;   //CRC 未写入
    }
    else
    {
        crc_count = do_crc(SN_BASE_ADDR, 16, CRC_INIT);
        if(crc_count == crc_value)    return 0;             //CRC OK
        else    return crc_count;     //CRC fail
    }

}

/************************************************************************
 * function   : read_sequence
 * Description: read unique SN（16bytes）
 * input :
 *         UINT8 * buff：SN buff pointer
                        buff[0:3]:   LOT ID  
                        buff[4:7]:   WaferID
                        buff[8:9]:   DieLocY 
                        buff[10:11]: DieLocX 
                        buff[12:13]: Day
                        buff[14]:    Month
                        buff[15]:    Year      
 * return: 0：CRC pass， 
           1：CRC fail
 ************************************************************************/
UINT16 read_sequence(UINT8 *buff)
{
    UINT32 i = 0;
    UINT32 temp;
    UINT8 *p;
    p = (UINT8 *)SN_BASE_ADDR;
    for(i = 0; i < 16; i++)
    {
        *buff++ = *p++;
    }
    //check SN crc
    temp = check_crc_sn();
    return temp;
}

/************************************************************************
 * function   : read_UID
 * Description: read unique SN（8bytes）
 * input :
 *         UINT8 * buff：SN buff pointer
                        buff[0:3]:   LOT ID  BYTE0~3 
                        buff[4]:     WaferID BYTE0
                        buff[5]:	 DieLocY BYTE0
                        buff[6]:	 DieLocX BYTE0
                        buff[7]:	 Month       
 * return: 0：CRC pass， 
           1：CRC fail
 ************************************************************************/
UINT16 read_UID(UINT8 *buff)
{
    UINT32 temp = 0;
    temp =  (*(volatile UINT32 *)(LOT_ID_ADDR));
    buff[0] = temp;
    buff[1] = temp >> 8;
    buff[2] = temp >> 16;
    buff[3] = temp >> 24;

    temp =  (*(volatile UINT8 *)(WAFER_ID_ADDR));
    buff[4] = temp;

    temp =  (*(volatile UINT16 *)(DIE_LOC_Y_ADDR));
    buff[5] = temp;
    temp =  (*(volatile UINT16 *)(DIE_LOC_X_ADDR));
    buff[6] = temp;

    temp =  (*(volatile UINT8 *)(DATE_MONTH_ADDR));
    buff[7] = temp;

    //check SN crc
    temp = check_crc_sn();
    return temp;
}

// test code

void SN_test(void)
{
	UINT8 chip_sn[16];
	UINT8 chip_UID[8];
	UINT32 temp=0;

	printfS("--------read chip sn for 16 bytes-------\n");
	temp=read_sequence(chip_sn);   
	if(temp) printfS("SN CRC fail\n");

	printfS("SN(16bytes): \n");
	printfB8(chip_sn,16);

	printfS("--------end-------\n");

	
	printfS("--------read chip UID for 8 bytes-------\n");
	temp=read_UID(chip_UID);
	if(temp) printfS("SN CRC fail\n");

	printfS("UID(8bytes): \n");
	printfB8(chip_UID,8);

	printfS("--------end-------\n");


	printfS("LOT_ID  = A%d \n", chip_sn[0]+(chip_sn[1]<<8)+(chip_sn[2]<<16)+(chip_sn[3]<<24) );	
	printfS("WAFER_ID= %d \n", chip_sn[4]);
	printfS("DIE ADDR= (%d,%d) \n", chip_sn[10]+(chip_sn[11]<<8),chip_sn[8]+(chip_sn[9]<<8));
	printfS("DATE= %d.%d.%d \n", (chip_sn[14]+(chip_sn[15]<<8)),chip_sn[13], chip_sn[12] );
}


void  eflash_page_erase_test(UINT32 base_addr)
{
    UINT32 page, start_page, end_page, addr, i, result;

    start_page = 100;
    end_page = 110;

    for(page = start_page; page < end_page; page++)
    {
        addr = base_addr + page * PAGE_SIZE;

#ifdef ROM_DRIVER_FLASH
        eflash_erase_page(addr);//rom里面的页擦除没有返回值
#else
        if(eflash_erase_page(addr))//如果返回1，则擦除校验错误
        {
            printfS("page Erase Verify Error\n");
            return;
        }
#endif

        for(i = 0; i < (PAGE_SIZE / 4); i++)
        {
            result = eflash_read_word(addr);
            if(result != (*(volatile UINT32 *)(SM_FLASH_FF_VALUE_ADDR)))
			//if(result != 0xffffffff)
            {
                printfS("page Erase Error: addr= 0x%x, result= 0x%x \r\n", addr, result);
                return;
            }
     
            addr += 4;
        }
		 printfS("page Erase Test Pass, page = %d\n", page);
     }
}       


void eflash_write_read_test(UINT32 base_addr, UINT32 value)
{
    UINT32 page, start_page, end_page, addr, i, result;

    printfS("write/read test value= 0x%x \n", value);

    start_page = 100;
    end_page = 110;

    for(page = start_page; page < end_page; page++)
    {
        addr = base_addr + page * PAGE_SIZE;
        eflash_erase_page(addr);
        for(i = 0; i < (PAGE_SIZE / 4); i++)
        {

#ifdef ROM_DRIVER_FLASH
            eflash_write_word(addr, value);
#else 
            if(eflash_write_word(addr, value))
            {
                printfS("write word verify Error\n");
                return; 
            }        
#endif

            result = eflash_read_word(addr);
            if(result != value)
            {
                printfS("write/word read error: addr= 0x%x, result= 0x%x \n", addr, result);
                return;
            }
            result = eflash_read_halfword(addr);
            if(result != (value & 0xffff))
            {
                printfS("write/halfword read error: addr= 0x%x, result= 0x%x \n", addr, result);
                return;
            }
            result = eflash_read_byte(addr);
            if(result != (value & 0xff))
            {
                printfS("write/byte read error: addr= 0x%x, result= 0x%x \n", addr, result);
                return;
            }
            addr += 4;
        }
        printfS("Write/Read Test Pass, page = %d\n", page);
    }
}


void eflash_test(void)
{
	printfS("-----start eflash test-------\n");

	printfS("------main area: page erase test------\n");
	eflash_page_erase_test(EFlashMainBaseAddr);

	printfS("------main area: word write->word|halfword|byte read test------\n");
	eflash_write_read_test(EFlashMainBaseAddr, 0x13223344);
	
	printfS("-----eflash test over-------\n");
	
}

