/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : app.c
 * Description : app source file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#include  "app.h"
#include  "sm1.h"

UINT8 sm1_ecb_test(void)
{
    UINT32 mode;
    UINT32 i;
    UINT32 blk_len;

    __align(4) UINT8 key_ecb[48] = {
        0x40,0xBB,0x12,0xDD,0x6A,0x82,0x73,0x86,0x7F,0x35,0x29,0xD3,0x54,0xB4,0xA0,0x26, // EK = 0x40BB12DD6A8273867F3529D354B4A026
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,   // AK = 0
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,   // SK = 0
    };
    __align(4) UINT8  iv_ecb[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };              //iv_ecb    = 0x0;
    __align(4) UINT8	 plain_ecb[16] = { 0x12,0x15,0x35,0x24,0xC0,0x89,0x5E,0x81,0x84,0x84,0xD6,0x09,0xB1,0xF0,0x56,0x63 }; //plain_ecb = 0x12153524C0895E818484D609B1F05663;
    __align(4) UINT8  crypt_ecb[16] = { 0x20,0x6d,0xc8,0xdc,0xbe,0x92,0x33,0x32,0xba,0xd1,0x8b,0x21,0xcd,0xad,0xe6,0xb5 }; //crypt_ecb = 0x206dc8dcbe923332bad18b21cdade6b5;
    __align(4) UINT8  result[16];

    blk_len = sizeof(plain_ecb) / 16;
    //-----------------------    ECB mode (no swap)   ---------------------
    mode = SM1_NORMAL_MODE;
//	mode = SM1_SECURITY_MODE;

    sm1_set_key((UINT32*)key_ecb, SM1_INTERPRAR,  SM1_SWAP_ENABLE);
    sm1_crypt((UINT32*)plain_ecb, (UINT32*)result, blk_len, SM1_ENCRYPTION, SM1_ECB_MODE, (UINT32*)iv_ecb, mode);    // sm1 encrypt
    for(i = 0; i < (blk_len * 16); i++)
    {
        if(result[i] != crypt_ecb[i])
        {
            return 1;
        }
    }

    sm1_crypt((UINT32*)crypt_ecb, (UINT32*)result, blk_len, SM1_DECRYPTION, SM1_ECB_MODE, (UINT32*)iv_ecb, mode);    // sm1 decrypt
    for(i = 0; i < (blk_len * 16); i++)
    {
        if(result[i] != plain_ecb[i])
        {
            return 1;
        }
    }

    //-----------------------   End of  ECB mode   ---------------------
    return 0;
}

UINT8 sm1_cbc_test(void)
{
    UINT32 mode;
    UINT32 i;
    UINT32 blk_len;

    __align(4) UINT8 key_cbc[48] = {
        0x01,0x1f,0x09,0xa0,0x09,0xa0,0x34,0xd8,0x2f,0xbf,0x82,0x60,0x41,0xcc,0xf1,0xa4, // EK = 0x011f09a009a034d82fbf826041ccf1a4
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  // AK = 0                                  // AK = 0
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  // SK = 0                                    // SK = 0
    };
    __align(4) UINT8 iv_cbc[16] = { 0xad,0x6a,0x2d,0xe8,0xdd,0xbe,0x64,0xab,0xdc,0x43,0xe5,0x31,0x7c,0x4a,0x13,0x3b };  // iv_cbc     = 0xad6a2de8ddbe64abdc43e5317c4a133b;
    __align(4) UINT8 plain_cbc[16] = { 0x3c,0xcf,0x00,0xfb,0xb6,0xbc,0x1c,0x00,0x64,0x0e,0x2a,0x52,0x73,0xfe,0x94,0xb7 };  // plain_cbc  = 0x3ccf00fbb6bc1c00640e2a5273fe94b7;
    __align(4) UINT8 cipher_cbc[16] = { 0x77,0xde,0xb3,0xe5,0x8b,0x75,0x00,0xb9,0xf2,0x9d,0xf4,0x35,0xba,0x0e,0xd5,0xca };  // cipher_cbc = 0x77deb3e58b7500b9f29df435ba0ed5ca;
    __align(4) UINT8 result[16];

    blk_len = sizeof(plain_cbc) / 16;
    //-----------------------    CBC mode (no swap)    ---------------------
//	mode = SM1_NORMAL_MODE;
    mode = SM1_SECURITY_MODE;

    sm1_set_key((UINT32*)key_cbc, SM1_INTERPRAR,  SM1_SWAP_ENABLE);
    sm1_crypt((UINT32*)plain_cbc, (UINT32*)result, blk_len, SM1_ENCRYPTION, SM1_CBC_MODE, (UINT32*)iv_cbc, mode);    // sm1 CBC encrypt
    for(i = 0; i < (blk_len * 16); i++)
    {
        if(result[i] != cipher_cbc[i])
        {
            return 1;
        }
    }

    sm1_crypt((UINT32*)cipher_cbc, (UINT32*)result, blk_len, SM1_DECRYPTION, SM1_CBC_MODE, (UINT32*)iv_cbc, mode);   // sm1 CBC decrypt
    for(i = 0; i < (blk_len * 16); i++)
    {
        if(result[i] != plain_cbc[i])
        {
            return 1;
        }
    }
    //-----------------------  End of  CBC mode    ---------------------
    return 0;
}
void sm1_test(void)
{
    printfS("SM1 test start\n");
  
    enable_module(BIT_UAC | BIT_EMW | BIT_HRNG | BIT_SM1 | BIT_HRNGS);  //UAC,EMW,hrng,SM1,hrng_sclk
    if(sm1_ecb_test()) printfS(" SM1 ECB test fail\n");
    else printfS(" SM1 ECB test pass\n");

    if(sm1_cbc_test()) printfS(" SM1 CBC test fail\n");
    else printfS(" SM1 CBC test pass\n");

    printfS("SM1 test over\n");

}




