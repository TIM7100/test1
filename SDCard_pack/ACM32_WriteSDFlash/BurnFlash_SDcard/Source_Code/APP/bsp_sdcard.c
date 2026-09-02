#include "bsp_sdcard.h"
#include "bsp_spi.h"

#define Debug_SD
#ifdef Debug_SD
    #define Debug_Error_Inform(x)   printfS("%s_%d.SD_Error:%X", __FILE__, __LINE__, x);
#else
    #define Debug_Error_Inform(x)   ((void)0);
#endif

#define SD_DUMMY_BYTE   0xFF
static __IO uint8_t SD_Type = 0;

#define SD_BSP_SPIx         SPI2

#define SPI_CS_Select()     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_CLEAR)
#define SPI_CS_Release()    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET)

static uint8_t SD_ReadByte(void)
{
    uint8_t RxData;
    if (SPIx_ReadByte(SD_BSP_SPIx, &RxData) != SPI_OK)
    {
        Debug_Error_Inform(SPI_ERROR);
        return SPI_ERROR;
    }
    return RxData;
}

static uint8_t SD_WriteByte(uint8_t TxData)
{
    return SPIx_WriteByte(SD_BSP_SPIx, TxData);
}

static uint8_t SD_WriteReadByte(uint8_t TxData)
{
    return SPIx_WriteReadByte(SD_BSP_SPIx, TxData);
}

static void SD_SPI_SpeedLow(void)
{
    SPIx_SetSpeed(SD_BSP_SPIx, SPI_BAUDRATE_PRESCALER_254);
}

static void SD_SPI_SpeedHigh(void)
{
    SPIx_SetSpeed(SD_BSP_SPIx, SPI_BAUDRATE_PRESCALER_8);
}

static void SD_IO_Init(void)
{
    SPI_Configuration(SD_BSP_SPIx);
}

static void SD_DisSelect(void)
{
    SPI_CS_Release();
    SD_WriteByte(SD_DUMMY_BYTE);
}

static uint8_t SD_WaitReady(void)
{
    uint32_t t = 0;
    do
    {
        if (SD_WriteReadByte(SD_DUMMY_BYTE) == SD_DUMMY_BYTE)
        {
            return 0;
        }
        System_Delay(10);
        t++;
    }
    while (t < 0x1FFFFFF);
    return 1;
}

static uint8_t SD_Select(void)
{
    SPI_CS_Select();
    if (SD_WaitReady() == 0)
    {
        return 0;
    }
    SPI_CS_Release();
    return 1;
}

static uint8_t SD_GetResponse(uint8_t Response)
{
    uint32_t Count = 0xFFF;
    uint8_t Return = 0;

    while ((SD_WriteReadByte(0xFF) != Response) && Count) {
        Count--;
    }
    if (Count == 0) {
        return SD_RESPONSE_FAILURE;
    } else {
        return SD_RESPONSE_NO_ERROR;
    }
}

void SD_SendCmd(uint8_t Cmd, uint32_t Arg, uint8_t Crc)
{
    SD_WriteByte(Cmd | 0x40);
    SD_WriteByte(Arg >> 24);
    SD_WriteByte(Arg >> 16);
    SD_WriteByte(Arg >> 8);
    SD_WriteByte(Arg);
    SD_WriteByte(Crc);
}

uint8_t SD_GoIdleState(void)
{
    uint8_t Count = 3;

    SD_Select();
    SD_SendCmd(SD_CMD_GO_IDLE_STATE, 0, 0x95);

    while (SD_GetResponse(SD_IN_IDLE_STATE) && Count--)
    {
        if (Count == 0)
        {
            return SD_RESPONSE_FAILURE;
        }
    }
    return SD_RESPONSE_NO_ERROR;
}

SD_Error SD_GetCardType(void)
{
    uint32_t i = 0;
    uint32_t Count = 0xFFF;
    uint8_t R7R3_Resp[4];
    __IO uint8_t R1_Resp = 0;

    SD_DisSelect();
    SD_Select();
    SD_SendCmd(SD_CMD_SEND_IF_COND, 0x1AA, 0x87);

    do {
        R1_Resp = SD_WriteReadByte(0xFF);
    } while ((R1_Resp & 0x80) && Count--);
    if (Count == 0) {
        return SD_RESPONSE_FAILURE;
    }

    if (R1_Resp == (SD_IN_IDLE_STATE|SD_ILLEGAL_COMMAND)) {
        do {
            SD_DisSelect();
            SD_Select();
            SD_SendCmd(SD_CMD_SEND_OP_COND, 0, 0xFF);
        } while (SD_GetResponse(SD_RESPONSE_NO_ERROR));
        SD_Type = SD_TYPE_V1;
    }
    else if (R1_Resp == SD_IN_IDLE_STATE) {
        for (i = 0; i < 4; i++) {
            R7R3_Resp[i] = SD_ReadByte();
        }

        SD_DisSelect();
        SD_Select();

        if (R7R3_Resp[2]==0x01 && R7R3_Resp[3]==0xAA) {
            Count = 200;
            do {
                SD_SendCmd(SD_CMD_APP_CMD, 0, 0xFF);
                if (!SD_GetResponse(SD_RESPONSE_NO_ERROR))
                    return SD_RESPONSE_FAILURE;
                SD_SendCmd(SD_ACMD_SD_SEND_OP_COND, 0x40000000, 0xFF);
                if (Count-- == 0)
                    return SD_RESPONSE_FAILURE;
            } while (SD_GetResponse(SD_RESPONSE_NO_ERROR));

            Count = 200;
            do {
                SD_DisSelect();
                SD_Select();
                SD_SendCmd(SD_CMD_READ_OCR, 0, 0xFF);
            } while (SD_GetResponse(SD_RESPONSE_NO_ERROR) || Count-- == 0);

            if (Count == 0)
                return SD_RESPONSE_FAILURE;

            for (i = 0; i < 4; i++) {
                R7R3_Resp[i] = SD_ReadByte();
            }

            if (R7R3_Resp[0]&0x40) {
                SD_Type = SD_TYPE_V2HC;
            } else {
                SD_Type = SD_TYPE_V2;
            }
        }
    }

    SD_DisSelect();
    return SD_RESPONSE_NO_ERROR;
}

uint8_t SD_Initialize(void)
{
    __IO uint32_t i = 0;
    __IO uint8_t R1_Response = 0;

    SD_IO_Init();
    SD_SPI_SpeedLow();

    for (i = 0; i < 10; i++) {
        SD_WriteByte(SD_DUMMY_BYTE);
    }

    i = 10;
    do
    {
        R1_Response = SD_GoIdleState();
        if (i == 0)
            return SD_RESPONSE_FAILURE;
    } while ((R1_Response == SD_RESPONSE_FAILURE) && (i-- > 0));

    i = 10;
    do
    {
        SD_GetCardType();
    } while ((SD_Type == SD_TYPE_NOT_SD) && (i-- > 0));

    if (SD_Type == SD_TYPE_NOT_SD)
    {
        return SD_RESPONSE_FAILURE;
    }
    SD_SPI_SpeedHigh();
    return SD_RESPONSE_NO_ERROR;
}

static uint8_t SD_RecvData(uint8_t Token, uint8_t *RxBuf, uint16_t Size)
{
    uint32_t i = 0;

    if (!SD_GetResponse(Token)) {
        for (i = 0; i < Size; i++) {
            *RxBuf = SD_ReadByte();
            RxBuf++;
        }
        SD_ReadByte();
        SD_ReadByte();
        return SD_DATA_OK;
    }
    return SD_RESPONSE_FAILURE;
}

uint8_t SD_ReadDisk(uint8_t* DataBuff, uint32_t StartSector, uint8_t Cnt)
{
    __IO uint8_t R1_Response = SD_RESPONSE_FAILURE;
    uint8_t* RxBuf = DataBuff;
    uint8_t TryCount = 5;

    if (SD_Type != SD_TYPE_V2HC)
    {
        StartSector <<= 9;
    }
    while (TryCount--)
    {
        SD_Select();

        if (Cnt == 1)
        {
            SD_SendCmd(SD_CMD_READ_SINGLE_BLOCK, StartSector, 0x01);
            if (!SD_GetResponse(SD_RESPONSE_NO_ERROR)) {
                R1_Response = SD_RecvData(SD_START_DATA_SINGLE_BLOCK_READ, RxBuf, 512);
            }
        }
        else
        {
            SD_SendCmd(SD_CMD_READ_MULT_BLOCK, StartSector, 0x01);
            if (!SD_GetResponse(SD_RESPONSE_NO_ERROR)) {
                do
                {
                    R1_Response = SD_RecvData(SD_START_DATA_MULTIPLE_BLOCK_READ, RxBuf, 512);
                    RxBuf += 512;
                }
                while (--Cnt && (R1_Response == SD_DATA_OK));
                SD_SendCmd(SD_CMD_STOP_TRANSMISSION, 0, 0x01);
            }
        }
        SD_DisSelect();
        if (R1_Response != SD_DATA_OK)
        {
            if (SD_Initialize() != SD_RESPONSE_NO_ERROR)
            {
                TryCount--;
                return R1_Response;
            }
        }
        else
        {
            return R1_Response;
        }
    }
    return R1_Response;
}

static uint8_t SD_GetDataResponse(void)
{
    uint32_t i = 0;
    uint8_t response, rvalue = SD_DATA_OTHER_ERROR;

    while (i <= 64) {
        response = SD_ReadByte();
        response &= 0x1F;
        switch (response) {
        case SD_DATA_OK:
            rvalue = SD_DATA_OK;
            break;
        case SD_DATA_CRC_ERROR:
            return SD_DATA_CRC_ERROR;
        case SD_DATA_WRITE_ERROR:
            return SD_DATA_WRITE_ERROR;
        default:
            rvalue = SD_DATA_OTHER_ERROR;
            break;
        }
        if (rvalue == SD_DATA_OK)
            break;
        i++;
    }

    while (SD_ReadByte() == 0);
    return response;
}

static uint8_t SD_WriteBlock(uint8_t *TxBuf, uint8_t Token)
{
    uint16_t i;

    SD_WriteByte(SD_DUMMY_BYTE);
    SD_WriteByte(Token);

    if (TxBuf != 0)
    {
        for (i = 0; i < 512; i++) {
            SD_WriteByte(*TxBuf);
            TxBuf++;
        }
        SD_ReadByte();
        SD_ReadByte();
        if (SD_GetDataResponse() != SD_DATA_OK) {
            return SD_RESPONSE_FAILURE;
        }
    }
    return SD_DATA_OK;
}

uint8_t SD_WriteDisk(uint8_t* TxBuf, uint32_t StartSector, uint8_t Cnt) 
{
    uint8_t i;

    if (SD_Type != SD_TYPE_V2HC)
    {
        StartSector *= 512;
    }

    for (i = 0; i < Cnt; i++)
    {
        __IO uint8_t R1_Response = SD_RESPONSE_FAILURE;
        uint8_t TryCount = 3;

        while (TryCount--)
        {
            SD_Select();
            SD_SendCmd(SD_CMD_WRITE_SINGLE_BLOCK, StartSector + i, 0x01);
            if (!SD_GetResponse(SD_RESPONSE_NO_ERROR)) {
                R1_Response = SD_WriteBlock(TxBuf + i * 512, SD_START_DATA_SINGLE_BLOCK_WRITE);
            }
            SD_DisSelect();
            if (R1_Response == SD_DATA_OK)
                break;
//            SD_Initialize();
        }
        if (R1_Response != SD_DATA_OK)
            return R1_Response;
    }
    return SD_DATA_OK;
}