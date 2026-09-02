/* ======================== SD Card + UART Communication ======================== */
#include "BurnSD.h"
#include "bsp_sdcard.h"
#include "crc.h"
#include "aes_app.h"
#include  "app.h"
#include <string.h>

#define NotFoundSDCard   0x01
#define ConnectUartError 0x02

uint8_t TestData[0x125] = { 0x12, 0x13, 0x14, 0x50, 0x55 };
uint8_t TestReadDat[0x400];
uint32_t g_plainLen = 0;
static uint8_t g_isEncrypted = 1;  // 1=加密模式, 0=明文模式 

/* Blocking UART receive */
static void UartRecvData(uint8_t* Data, uint32_t Len)
{
    HAL_UART_Receive(&UART1_Handle, Data, Len, 0xFFFF);
}

/* Blocking UART send */
static void UartSendData(uint8_t* Data, uint32_t Len)
{
    HAL_UART_Transmit(&UART1_Handle, Data, Len, 0xFFFF);
}

/* Check if SD card is connected */
static uint8_t CheckSDCardConnect(void)
{
    if (SD_RESPONSE_NO_ERROR == SD_Initialize())
    {
        return 0;
    }
    return 1;
}

/* Wait for UART start command from host PC */
static uint8_t WaitUartConnect(void)
{
    uint8_t ConnectTime = 3;
    uint8_t GetStartCmd[8];
    uint8_t calcXor;
    uint16_t i;
    uint8_t ACK[6] = {0x55, 0x00, 0x00, 0x00, 0x55, 0x00};

WAITSTART:
    memset(GetStartCmd, 0, 8);
    UartRecvData(GetStartCmd, 8);
    if (GetStartCmd[0] == 0xAA && (GetStartCmd[1] == 0x00 || GetStartCmd[1] == 0x04) && GetStartCmd[7] == 0x55)
    {
        g_isEncrypted = (GetStartCmd[1] == 0x00) ? 1 : 0;  //获取固件包加密标志
        calcXor = (uint8_t)(0xAA ^ GetStartCmd[1]);
        for (i = 2; i < 6; i++)
        {
            calcXor ^= GetStartCmd[i];
        }
        if (GetStartCmd[6] == calcXor)
        {
            g_plainLen = ((uint32_t)GetStartCmd[2] << 24) | ((uint32_t)GetStartCmd[3] << 16) |
                         ((uint32_t)GetStartCmd[4] << 8)  | (uint32_t)GetStartCmd[5];
            UartSendData(ACK, 6);
            return 0;
        }
    }
    ConnectTime--;
    if (ConnectTime == 0)
    {
        return 1;
    }
    goto WAITSTART;
}

/* Receive data from UART, decrypt with hardware AES, write to SD card */
static uint8_t WriteSDCardData(uint8_t* WriteBuf)
{
    static uint8_t buf[16384];
    static uint8_t plainBuf[16384];
    uint8_t  ACK[6] = {0x55, 0x00, 0x00, 0x00, 0x55, 0x00};
    uint8_t  dummy[4];
    uint8_t  seq_l, seq_h, calcXor;
    uint16_t len, i, recvCrc, calcCrc;
    uint16_t totalCrc = 0xFFFF;
    uint16_t innerCrcAcc = 0xFFFF;
    uint16_t recvTotalCrc;
    uint16_t expInner;
    uint16_t j;
    uint32_t sector = 0;
    uint32_t secs;
    uint32_t plainPos = 0;
    uint32_t take;
    uint8_t  prevIv[16];
    uint8_t  expInnerHi = 0, expInnerLo = 0, gotInner = 0;

    /* Copy AES_Iv (big-endian, matches AES_SWAP_ENABLE) */
    for (i = 0; i < 16; i++)
    {
        prevIv[i] = AES_Iv[i];
    }

    while (1)
    {
        UartRecvData(buf, 2);      /* [AA] [CMD] */
        if (buf[0] != 0xAA)
        {
            continue;
        }

        if (buf[1] == 0xFF)        /* END: [AA FF CRC_H CRC_L XOR FOOTER] */
        {
            UartRecvData(dummy, 4);
            recvTotalCrc = (uint16_t)((dummy[0] << 8) | dummy[1]);
            totalCrc = (uint16_t)~totalCrc;
            innerCrcAcc = (uint16_t)~innerCrcAcc;
            expInner = (uint16_t)((expInnerHi << 8) | expInnerLo);

            printfS("\r\n[DBG] recvTotalCrc=0x%04X totalCrc=0x%04X\r\n", recvTotalCrc, totalCrc);
            printfS("[DBG] gotInner=%d innerCrcAcc=0x%04X expInner=0x%04X\r\n", gotInner, innerCrcAcc, expInner);

            if (dummy[2] != (uint8_t)(0xAA ^ 0xFF ^ dummy[0] ^ dummy[1]))
            {
                ACK[3] = 0x01;
            }
            else if (totalCrc != recvTotalCrc)
            {
                ACK[3] = 0x01;
            }
            else if (g_isEncrypted)  //数据包加密判断
            {
                if (gotInner == 0 || innerCrcAcc != expInner)
                {
                    ACK[3] = 0x02;
                }
                else
                {
                    ACK[3] = 0x00;
                }
            }
            else
            {
                ACK[3] = 0x00;
            }

            ACK[1] = 0x00;
            ACK[2] = 0x00;
            ACK[5] = (uint8_t)(0x55 ^ ACK[1] ^ ACK[2] ^ ACK[3] ^ 0x55);
            UartSendData(ACK, 6);

            printfS("\r\n=== BURN END ===\r\n");
            break;
        }

        UartRecvData(buf, 4);      /* [SEQ_L] [SEQ_H] [LEN_H] [LEN_L] */
        seq_l = buf[0];
        seq_h = buf[1];
        len = (buf[2] << 8) | buf[3];

        UartRecvData(buf, len);    /* data */
        UartRecvData(dummy, 4);    /* [CRC_H] [CRC_L] [XOR] [FOOTER] */

        calcXor = 0xAA ^ 0x01 ^ seq_l ^ seq_h ^ (uint8_t)(len >> 8) ^ (uint8_t)len;
        for (i = 0; i < len; i++)
        {
            calcXor ^= buf[i];
        }

        recvCrc = (uint16_t)((dummy[0] << 8) | dummy[1]);
        calcCrc = get_crc_16(buf, len);

        if ((calcXor != dummy[2]) || (calcCrc != recvCrc))
        {
            ACK[1] = seq_l;
            ACK[2] = seq_h;
            ACK[3] = 0x01;
            ACK[5] = (uint8_t)(seq_l ^ seq_h ^ 0x01);
            UartSendData(ACK, 6);
            continue;
        }

        totalCrc = crc16_ccitt_continue(totalCrc, buf, len);

        if (g_isEncrypted)
        {
            /* Hardware AES-128-CBC decrypt */
            AES128_CBC_Decrypt(plainBuf, buf, len, AES_Key, prevIv);
            /* Copy last 16 bytes of ciphertext as next IV (big-endian, matches AES_SWAP_ENABLE). */
            for (i = 0; i < 16; i++)
            {
                prevIv[i] = buf[len - 16 + i];
            }

            take = g_plainLen - plainPos;
            if (take > len)
            {
                take = len;
            }
            if (take > 0)
            {
                innerCrcAcc = crc16_ccitt_continue(innerCrcAcc, plainBuf, take);
                plainPos += take;
            }
            if (gotInner == 0 && take < len)
            {
                expInnerHi = plainBuf[take];
                expInnerLo = plainBuf[take + 1];
                gotInner = 1;
            }
        }

        /* Zero-pad to full sector boundary */
        secs = ((uint32_t)len + 511u) / 512u;
        for (j = len; j < (uint16_t)(secs * 512u); j++)
        {
            buf[j] = 0;
        }

        /* Write to SD card */
        SD_WriteDisk(buf, sector, (uint8_t)secs);
        sector += secs;

        ACK[1] = seq_l;
        ACK[2] = seq_h;
        ACK[3] = 0x00;
        ACK[5] = (uint8_t)(seq_l ^ seq_h);
        UartSendData(ACK, 6);
    }

    return 0;
}

/* Main SD card write flow */
uint8_t Write_SDCard(void)
{
    uint8_t SD_Status;

    printfS("SD Card check...\r\n");
    SD_Status = CheckSDCardConnect();
    if (SD_Status)
    {
        printfS("SD Card not found!\r\n");
        return NotFoundSDCard;
    }
    printfS("SD Card OK.\r\n");

    printfS("Wait UART connect...\r\n");
    SD_Status = WaitUartConnect();
    if (SD_Status)
    {
        printfS("UART connect timeout!\r\n");
        return ConnectUartError;
    }
    printfS("UART connected, start burning...\r\n");

    WriteSDCardData(TestData);

    printfS("Burn complete.\r\n");
    return 0;
}




