#include "SD.h"
#include "Timer0.h"

__IO static u8 SD_Type = 0;                 //SD卡的类型

volatile u8 Receive_SN[50][0x10F];
extern FlashTable_Type FlashTable;


u8 SD_SPI_ReadWriteByte(u8 data)
{
    return SPI2_ReadWriteByte(data);
}

void Get_TableHead(void)
{
    int i, j;
    u8 buffer[960] = {0};

    SD_ReadDisk(buffer, 0, 2);   //从0扇区开始读，读2个，一共是512*2

    for (i = 0; i < 80; i ++)
    {
        for (j = 0; j < 12; j++)
        {
            *((u8 *)&FlashTable + j + (12 * i)) =   buffer[j + (i * 12)];
        }
    }
}

//数据存放到 Receive_SN 中
u8 Get_SN_number(TypeInfrom TypeSel, u32 SN_Count, u8 ReadNum) //读取flash中的序列号 ，12位结构体， 大板烧录总数， 读取个数
{
    int i;
    u32 RealCount;
    u32 SNTotal;

    Get_TableHead();
    SNTotal = (TypeSel.SN_total[0] << 8) + TypeSel.SN_total[1];  //序列号总数
    RealCount = SN_Count % SNTotal; //读取的第几个

    if (SNTotal > 0)
    {
        for (i = 0; i < ReadNum; i++)
        {
            SD_ReadDisk((u8*)&Receive_SN[i][0], i, 1);  //读取需要个数的序列号存于2维数组Receive_SN中
            if (++RealCount >= SNTotal)
            {
                RealCount = 0;
            }
        }
    }
    return 0;
}

/*******************************************************************************************/
//初始化SPI FLASH的IO口
u8 SD_Initialize(void)
{

    u32 Retry = 0;
    u8 R1_Response = 0;

    SPI_GPIO_Config();
    SPI_CS_Release();           //释放片选脚

    for (Retry = 0; Retry < 10; Retry++)
    {
        SD_SPI_ReadWriteByte(0xFF);    //发送最少74个脉冲
    }
    Retry = 10;
    do
    {
        R1_Response = SD_GoIdleState();
        if (Retry == 0)
        {
            return SD_RESPONSE_FAILURE;
        }
    }
    while ((R1_Response == SD_RESPONSE_FAILURE) && (Retry-- > 0));

    Retry = 10;
    do
    {
        SD_GetCardType();
    }
    while ((SD_Type == SD_TYPE_NOT_SD) && (Retry-- > 0));
    //不支持的卡
    if (SD_Type == SD_TYPE_NOT_SD)
    {
        return SD_RESPONSE_FAILURE;
    }
    return SD_RESPONSE_NO_ERROR;

}

void SPI_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* SPI2 CS   PortB Pin12 */
    /* SPI2 CLK  PortB Pin13 */
    /* SPI2 MISO PortB Pin14 */
    /* SPI2 MOSI PortB Pin15 */
    GPIO_InitStructure.GPIO_Pin            = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode           = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed                    = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB

    GPIO_InitStructure.GPIO_Pin             = GPIO_Pin_13;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /*MISO GPIO Config*/
    GPIO_InitStructure.GPIO_Pin             = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode          = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed                    = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB

    /*MOSI GPIO Config*/
    GPIO_InitStructure.GPIO_Pin             = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode           = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB

    SPI_CS_Release();
    GPIO_SetBits(GPIOB, GPIO_Pin_13);
}

static void SPI_Delay(__IO uint32_t count)
{
    uint32_t i;
    uint8_t uc;
    for (i = 0; i < count; i++)
    {
        uc = 5;
        while (uc--);
    }
}

//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI2_ReadWriteByte(u8 TxData)
{
    u8 i = 0;
    u8 Value = 0;

    for (i = 0; i < 8; i++)
    {
        SPI_CLK_LOW();
        SPI_Delay(DELAY_US);

        if (TxData & 0x80)
        {
            SPI_MOSI_HIGH();
        }
        else
        {
            SPI_MOSI_LOW();
        }
        TxData <<= 1;

        SPI_Delay(DELAY_US);
        SPI_CLK_HIGH();

        Value <<= 1;
        if (SPI_MISO())
        {
            Value |= 0x01;
        }
        SPI_Delay(DELAY_US);
    }
    return Value;
}

//向SD卡发送一个命令
//输入: u8 cmd   命令
//      u32 arg  命令参数
//      u8 crc   crc校验值
//返回值:SD卡返回的响应
void SD_SendCmd(u8 cmd, u32 arg, u8 crc)
{
    SD_SPI_ReadWriteByte(cmd | 0x40); /*!< Construct byte 1 */
    SD_SPI_ReadWriteByte(arg >> 24); /*!< Construct byte 2 */
    SD_SPI_ReadWriteByte(arg >> 16); /*!< Construct byte 3 */
    SD_SPI_ReadWriteByte(arg >> 8); /*!< Construct byte 4 */
    SD_SPI_ReadWriteByte(arg); /*!< Construct byte 5 */
    SD_SPI_ReadWriteByte(crc); /*!< Construct CRC: byte 6 */
}

//取消选择,释放SPI总线
void SD_DisSelect(void)
{
    SDCard_CS = 1;
    SD_SPI_ReadWriteByte(0xff);//提供额外的8个时钟
}

u8 SD_Select(void)
{
    SDCard_CS = 0;
    if (SD_WaitReady() == 0)
    {
        return 0;    //等待成功
    }
    SDCard_CS = 1;
    return 1;//等待失败
}

//等待卡准备好
//返回值:0,准备好了;其他,错误代码
u8 SD_WaitReady(void)
{
    u32 t = 0;
    do
    {
        if (SD_SPI_ReadWriteByte(0xFF) == 0xFF)
        {
            return 0;    //OK
        }
        t++;
    }
    while (t < 0xFFFFFF); //等待
    return 1;
}

//读SD卡
//buf:数据缓存区
//sector:扇区
//cnt:扇区数
//返回值:0,ok;其他,失败.
u8 SD_ReadDisk(u8* DataBuff, u32 StartSector, u8 Cnt)
{
    __IO u8 R1_Response = SD_RESPONSE_FAILURE;
    u8* RxBuf = DataBuff;
    u8 TryCount = 3;


    if (SD_Type != SD_TYPE_V2HC)
    {
        StartSector <<= 9;    //转换为字节地址
    }
    while (TryCount--)
    {
        SD_Select();

        /* 读单个扇区 */
        if (Cnt == 1)
        {
            /*!< 发送 CMD17 (SD_CMD_READ_SINGLE_BLOCK) 以读取一个数据块 */
            SD_SendCmd(SD_CMD_READ_SINGLE_BLOCK, StartSector, 0x01);
            /*!< 检查R1响应 */
            if (!SD_GetResponse(SD_RESPONSE_NO_ERROR))
            {
                R1_Response = SD_RecvData(SD_START_DATA_SINGLE_BLOCK_READ, RxBuf, 512);
            }
        }
        /* 读多个扇区 */
        else
        {
            SD_SendCmd(SD_CMD_READ_MULT_BLOCK, StartSector, 0x01); //连续读命令  Crc0x01
            /*!< 检查R1响应 */
            if (!SD_GetResponse(SD_RESPONSE_NO_ERROR))
            {
                do
                {
                    R1_Response = SD_RecvData(SD_START_DATA_MULTIPLE_BLOCK_READ, RxBuf, 512); //接收512个字节
                    RxBuf += 512;
                }
                while (--Cnt && (R1_Response == SD_DATA_OK));
                SD_SendCmd(SD_CMD_STOP_TRANSMISSION, 0, 0x01); //发送停止命令
            }
        }
        SD_DisSelect();//取消片选
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
    return R1_Response;//
}
/**
* @brief  接收SD卡发送的数据
* @param  RxBuf: 接收缓存区
* @param  Size: 接收大小
* @return SD数据响应：
*         - SD_DATA_OK：数据接收成功
*         - SD_DATA_OTHER_ERROR：数据接收失败
*/
static u8 SD_RecvData(u8 Token, u8 *RxBuf, u16 Size)
{
    uint32_t i = 0;


    /*使用SPI读取SD的一扇数据前，需要判断数据块的头部是否有开始Token 0xFE*/
    if (!SD_GetResponse(Token))
    {
        /*!< 读取一个数据块的数据 :  NumByteToRead 个数据 */
        for (i = 0; i < Size; i++)
        {
            /*!< 接收一个字节到pBuffer */
            *RxBuf = SD_ReadByte();
            /*!< 指针加1*/
            RxBuf++;
        }
        /*!< 读取 CRC 校验字节 (此处不校验，但SD卡有该流程，需要接收) */
        SD_ReadByte();
        SD_ReadByte();
        return SD_DATA_OK;//读取成功
    }

    return SD_RESPONSE_FAILURE;
}


//等待SD卡回应
//Response:要得到的回应值
//返回值:0,成功得到了该回应值
//    其他,得到回应值失败
u8 SD_GetResponse(u8 Response)
{
    u16 Count = 0xFFF; //等待次数
    while ((SD_SPI_ReadWriteByte(0xFF) != Response) && Count)
    {
        Count--;    //等待得到准确的回应
    }
    if (Count == 0)
    {
        return MSD_RESPONSE_FAILURE;    //得到回应失败
    }
    else
    {
        return MSD_RESPONSE_NO_ERROR;    //正确回应
    }
}

u8 SD_WriteDisk(u8* TxBuf, u32 StartSector, u8 Cnt)
{
    __IO u8 R1_Response = SD_RESPONSE_FAILURE;
    u8 TryCount = 3;

    if (SD_Type != SD_TYPE_V2HC)
    {
        StartSector *= 512;    //转换为字节地址
    }
    while (TryCount--)
    {
        SD_Select();
        /* 写单个扇区 */
        if (Cnt == 1)
        {
            SD_SendCmd(SD_CMD_WRITE_SINGLE_BLOCK, StartSector, 0x01); //读命令
            /*!< 检查R1响应 */
            if (!SD_GetResponse(SD_RESPONSE_NO_ERROR))
            {
                R1_Response = SD_WriteBlock(TxBuf, SD_START_DATA_SINGLE_BLOCK_WRITE); //写512个字节
            }
        }
        /* 写多个扇区 */
        else
        {
            //      SD_SendCmd(SD_CMD_APP_CMD, 0, 0x01);
            //      SD_SendCmd(SD_CMD_SET_BLOCK_COUNT, Cnt, 0x01); //发送指令

            SD_SendCmd(SD_CMD_WRITE_MULT_BLOCK, StartSector, 0x01); //连续写命令
            if (!SD_GetResponse(SD_RESPONSE_NO_ERROR))
            {
                do
                {
                    R1_Response = SD_WriteBlock(TxBuf, SD_START_DATA_MULTIPLE_BLOCK_WRITE); //发送512个字节
                    TxBuf += 512;
                }
                while (--Cnt && (R1_Response == SD_DATA_OK));
                R1_Response = SD_WriteBlock(0, SD_STOP_DATA_MULTIPLE_BLOCK_WRITE); //发送512个字节
            }
        }
        SD_DisSelect();//取消片选

        if (R1_Response != SD_DATA_OK)
        {
            if (SD_Initialize() != SD_RESPONSE_NO_ERROR)
            {
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

/**
* @brief  向SD卡写入一扇数据块，共512字节
* @param  TxBuf: 发送缓存区
* @param  Token: 写入数据块前，需要先发送的开始标志字节
* @return SD数据响应：
*         - SD_DATA_OK：数据写入成功
*         - 其他：数据写入失败
*/
static u8 SD_WriteBlock(u8 *TxBuf, u8 Token)
{
    uint16_t i;

    /*!< 发送 dummy 空字节 */
    SD_WriteByte(SD_DUMMY_BYTE);

    /*!< 发送 一个数据块起始Token表示 开始传输数据 */
    SD_WriteByte(Token);

    if (TxBuf != 0)
    {
        /*!< 写入一个数据块的数据 */
        for (i = 0; i < 512; i++)
        {
            /*!< 发送指针指向的字节 */
            SD_WriteByte(*TxBuf);
            /*!< 指针加1 */
            TxBuf++;
        }
        /*!< 两个字节的空CRC校验码，默认不验证 */
        SD_ReadByte();
        SD_ReadByte();
        /*!< 读取SD卡数据响应 */
        if (SD_GetDataResponse() != SD_DATA_OK)
        {
            return SD_RESPONSE_FAILURE;
        }
    }
    return SD_DATA_OK;
}

u8 SD_GoIdleState(void)
{
    u8 Count = 3;

    /*!< 片选CS低电平 */
    SD_Select();

    /*!< 发送 CMD0 (SD_CMD_GO_IDLE_STATE) 让SD卡切换至SPI模式 */
    SD_SendCmd(SD_CMD_GO_IDLE_STATE, 0, 0x95);

    /*!< 等待R1响应 */
    while (SD_GetResponse(SD_IN_IDLE_STATE) && Count--)
    {
        if (Count == 0)
        {
            return SD_RESPONSE_FAILURE;
        }
    }

    /*初始化成功返回*/
    return SD_RESPONSE_NO_ERROR;
}


/**
* @brief  获取SD卡的版本类型，并区分SDSC和SDHC
* @param  无
* @retval SD响应:
*         - SD_RESPONSE_FAILURE: 失败
*         - SD_RESPONSE_NO_ERROR: 成功
*/
SD_Error SD_GetCardType(void)
{
    uint32_t i = 0;
    uint32_t Count = 0xFFF;

    uint8_t R7R3_Resp[4];
    __IO uint8_t R1_Resp = 0;

//    SD_Type = 0;
    /*!< 片选信号CS高电平并发送空字节延时 0xFF */
    SD_DisSelect();
    /*!< 片选信号CS低电平 */
    SD_Select();
    /*!< 发送 CMD8 命令，带0x1AA检查参数*/
    SD_SendCmd(SD_CMD_SEND_IF_COND, 0x1AA, 0x87);

    /*!< 等待R1响应 */
    do
    {
        R1_Resp = SD_WriteReadByte(0xFF);
    }
    while ((R1_Resp & 0x80) && Count--);
    if (Count == 0)
    {
        /*!< 等待超时 */
        return SD_RESPONSE_FAILURE;
    }

    //响应 = 0x05   非V2.0的卡
    if (R1_Resp == (SD_IN_IDLE_STATE | SD_ILLEGAL_COMMAND))
    {
        /* 激活SD卡 */
        do
        {
            /*!< 片选信号CS高电平并发送空字节延时 0xFF */
            SD_DisSelect();
            /*!< 片选信号CS低电平 */
            SD_Select();
            /*!< 发送CMD1完成V1 版本卡的初始化 */
            SD_SendCmd(SD_CMD_SEND_OP_COND, 0, 0xFF);
            /*!< Wait for no error Response (R1 Format) equal to 0x00 */
        }
        while (SD_GetResponse(SD_RESPONSE_NO_ERROR));
        //V1版本的卡完成初始化

        SD_Type = SD_TYPE_V1;

        //不处理MMC卡

        //初始化正常

    }
    //响应 0x01   V2.0的卡
    else if (R1_Resp == SD_IN_IDLE_STATE)
    {
        /*!< 读取CMD8 的R7响应 */
        for (i = 0; i < 4; i++)
        {
            R7R3_Resp[i] = SD_ReadByte();
        }

        /*!< 片选信号CS高电平并发送空字节延时 0xFF */
        SD_DisSelect();
        /*!< 片选信号CS低电平 */
        SD_Select();

        //判断该卡是否支持2.7-3.6V电压
        if (R7R3_Resp[2] == 0x01 && R7R3_Resp[3] == 0xAA)
        {
            //支持电压范围，可以操作
            Count = 200;
            //发卡初始化指令CMD55+ACMD41
            do
            {
                //CMD55，以强调下面的是ACMD命令
                SD_SendCmd(SD_CMD_APP_CMD, 0, 0xFF);
                // SD_IN_IDLE_STATE
                if (!SD_GetResponse(SD_RESPONSE_NO_ERROR))
                {
                    return SD_RESPONSE_FAILURE;    //超时返回
                }

                //ACMD41命令带HCS检查位
                SD_SendCmd(SD_ACMD_SD_SEND_OP_COND, 0x40000000, 0xFF);

                if (Count-- == 0)
                {
                    return SD_RESPONSE_FAILURE;    //重试次数超时
                }
            }
            while (SD_GetResponse(SD_RESPONSE_NO_ERROR));

            //初始化指令完成，读取OCR信息，CMD58

            //-----------鉴别SDSC SDHC卡类型开始-----------

            Count = 200;
            do
            {
                /*!< 片选信号CS高电平并发送空字节延时 0xFF */
                SD_DisSelect();
                /*!< 片选信号CS低电平 */
                SD_Select();
                /*!< 发送CMD58 读取OCR寄存器 */
                SD_SendCmd(SD_CMD_READ_OCR, 0, 0xFF);
            }
            while (SD_GetResponse(SD_RESPONSE_NO_ERROR) || Count-- == 0);

            if (Count == 0)
            {
                return SD_RESPONSE_FAILURE;    //重试次数超时
            }

            //响应正常，读取R3响应

            /*!< 读取CMD58的R3响应 */
            for (i = 0; i < 4; i++)
            {
                R7R3_Resp[i] = SD_ReadByte();
            }

            //检查接收到OCR中的bit30(CCS)
            //CCS = 0:SDSC       CCS = 1:SDHC
            if (R7R3_Resp[0] & 0x40) //检查CCS标志
            {
                SD_Type = SD_TYPE_V2HC;
            }
            else
            {
                SD_Type = SD_TYPE_V2;
            }
            //-----------鉴别SDSC SDHC版本卡的流程结束-----------
        }
    }

    /*!< 片选信号CS高电平并发送空字节延时 0xFF */
    SD_DisSelect();
    //初始化正常返回
    return SD_RESPONSE_NO_ERROR;
}

static u8 SD_ReadByte(void)
{
    u8 RxData;

    RxData = SPI_ReadWriteByte(0xFF);
    return RxData;
}

static u8 SD_WriteByte(u8 TxData)
{
    SPI_ReadWriteByte(TxData);
    return 0;
//  return SPIx_WriteByte(SD_BSP_SPIx, TxData);
}

/**
* @brief  获取SD卡的数据响应.
* @param  None
* @retval 返回响应状态 status: 读取到的数据响应 xxx0<status>1
*         - status 010: 接受数据
*         - status 101: CRC校验错误，拒绝数据
*         - status 110: 写入错误，拒绝数据
*         - status 111: 其它错误，拒绝数据
*/
uint8_t SD_GetDataResponse(void)
{
    uint32_t i = 0;
    uint8_t response, rvalue;

    while (i <= 64)
    {
        /*!< 读取响应 */
        response = SD_ReadByte();
        /*!< 屏蔽无关的数据位（前三位xxx） */
        response &= 0x1F;
        switch (response)
        {
        case SD_DATA_OK:
        {
            rvalue = SD_DATA_OK;
            break;
        }
        case SD_DATA_CRC_ERROR:
            return SD_DATA_CRC_ERROR;
        case SD_DATA_WRITE_ERROR:
            return SD_DATA_WRITE_ERROR;
        default:
        {
            rvalue = SD_DATA_OTHER_ERROR;
            break;
        }
        }
        /*!< 数据正常，退出循环 */
        if (rvalue == SD_DATA_OK)
        {
            break;
        }
        /*!< Increment loop counter */
        i++;
    }

    /*!< 等待空数据 */
    while (SD_ReadByte() == 0);

    /*!< 返回响应 */
    return response;
}

static u8 SD_WriteReadByte(u8 TxData)
{
    return SPI_ReadWriteByte(TxData);
//    return SPIx_WriteReadByte(SD_BSP_SPIx, TxData);
}

u8 SPI_ReadWriteByte(u8 Data)
{
    u8 i = 0;
    u8 Value = 0;

    for (i = 0; i < 8; i++)
    {
        SPI_CLK_LOW();
        SPI_Delay(DELAY_US);

        if (Data & 0x80)
        {
            SPI_MOSI_HIGH();
        }
        else
        {
            SPI_MOSI_LOW();
        }
        Data <<= 1;

        SPI_Delay(DELAY_US);
        SPI_CLK_HIGH();

        Value <<= 1;
        if (SPI_MISO())
        {
            Value |= 0x01;
        }
        SPI_Delay(DELAY_US);
    }
    return Value;
}
