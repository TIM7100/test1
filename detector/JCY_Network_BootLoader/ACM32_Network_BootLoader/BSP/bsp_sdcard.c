#include "bsp_sdcard.h"
//#include "bsp_spi.h"
#include "bsp_soft_spi.h"

//调试时，用于输出SD错误信息
#define Debug_SD 
#ifdef Debug_SD
	#define Debug_Error_Inform(x)		printfS("%s_%d.SD_Error:%X", __FILE__, __LINE__, x);
#else
	#define Debug_Error_Inform(x) 	((void)0);
	
#endif

#define SD_DUMMY_BYTE   0xFF		//空 字节数据
__IO static u8 SD_Type = 0; 			    //SD卡的类型


////////////////////////////////////移植修改区///////////////////////////////////
/**
 * @brief 选择使用的
 ***/
#define SD_BSP_SPIx			SPI2

/*
 * @brief SPI片选引脚选择，当前使用的是 PB12
 */
#define SPI_CS_Select()					 GPIO_ResetBits(GPIOB, GPIO_PIN_12) 
#define SPI_CS_Release()				 GPIO_SetBits(GPIOB, GPIO_PIN_12)


static u8 SD_ReadByte(void)
{
    u8 RxData;

    RxData = SPI_ReadWriteByte(0xFF);
//    if(SPIx_ReadByte(SD_BSP_SPIx, &RxData) != SPI_OK)
//    {
//        Debug_Error_Inform(SPI_ERROR);
//        return SPI_ERROR;
//    }
    return RxData;
}

static u8 SD_WriteByte(u8 TxData)
{
    SPI_ReadWriteByte(TxData);
    return 0;
//	return SPIx_WriteByte(SD_BSP_SPIx, TxData);
}

static u8 SD_WriteReadByte(u8 TxData)
{
    return SPI_ReadWriteByte(TxData);
//    return SPIx_WriteReadByte(SD_BSP_SPIx, TxData);
}


//SD卡初始化的时候,需要低速
static void SD_SPI_SpeedLow(void)
{
//    SPIx_SetSpeed(SD_BSP_SPIx, SPI_BAUDRATE_PRESCALER_254);//设置到低速模式
}

//SD卡正常工作的时候,可以高速了
static void SD_SPI_SpeedHigh(void)
{
//    SPIx_SetSpeed(SD_BSP_SPIx, SPI_BAUDRATE_PRESCALER_8);//设置到高速模式
}

//SPI硬件层初始化
static void SD_IO_Init(void)
{
    /* 设置硬件上与SD卡相关联的控制引脚输出 */
    SPI_GPIO_Config();
//    SPI_Configuration(SD_BSP_SPIx);
    SPI_CS_Release();			//释放片选脚	
}
///////////////////////////////////////////////////////////////////////////////////


/**
* @brief  取消选择,释放SPI总线
*/
static void SD_DisSelect(void)
{
    SPI_CS_Release();
	/*!< 发送 空 字节 0xFF, 提供额外的8个时钟 */
    SD_WriteByte(SD_DUMMY_BYTE);
}


/**
* @brief  等待卡准备好
* @return 0,成功; 1,失败
*/
static u8 SD_WaitReady(void)
{
    u32 t = 0;
    do
    {
        if (SD_WriteReadByte(SD_DUMMY_BYTE) == SD_DUMMY_BYTE)
        {
            return 0;    //OK
        }
        System_Delay(10);
        t++;
    }
    while (t < 0x1FFFFFF); //等待
    return 1;
}


/**
* @brief  选择sd卡,并且等待卡准备OK
* @return 0,成功; 1,失败
*/
static u8 SD_Select(void)
{
    SPI_CS_Select();
    if (SD_WaitReady() == 0)
    {
        return 0;    //等待成功
    }
    SPI_CS_Release();
    return 1;//等待失败
}


/**
* @brief  获取SD卡的的响应
* @param  要检查的响应类型
* @retval SD响应:
*         - SD_RESPONSE_FAILURE: 失败
*         - SD_RESPONSE_NO_ERROR: 成功
*/
static u8 SD_GetResponse(uint8_t Response)
{
    uint32_t Count = 0xFFF;
	u8 Return = 0;
    
	/*!< 检查是否接收到 response 表示的响应 */
    while ((SD_WriteReadByte(0xFF) != Response) && Count) {
        Count--;
    }
    if (Count == 0) {
        /*!< 检查超时 */
        return SD_RESPONSE_FAILURE;
    } else {
        /*!< 得到response表示的响应 */
        return SD_RESPONSE_NO_ERROR;
    }
}


/**
* @brief  发送SD命令
* @param  Cmd: 要发送的命令
* @param  Arg: 命令参数
* @param  Crc: CRC校验码.
* @return 
*/
void SD_SendCmd(uint8_t Cmd, uint32_t Arg, uint8_t Crc)
{
    uint32_t i = 0x00;
	u8 R1_Response;

//    if (SD_Select())
//    {
//        return ;    //片选失效
//    }
    
	SD_WriteByte(Cmd | 0x40); /*!< Construct byte 1 */
    SD_WriteByte(Arg >> 24); /*!< Construct byte 2 */
    SD_WriteByte(Arg >> 16); /*!< Construct byte 3 */
    SD_WriteByte(Arg >> 8); /*!< Construct byte 4 */
    SD_WriteByte(Arg); /*!< Construct byte 5 */
    SD_WriteByte(Crc); /*!< Construct CRC: byte 6 */
}

/**
* @brief  让SD卡进入空闲模式.
* @param  None
* @retval SD卡响应:
*         - SD_RESPONSE_FAILURE: 失败
*         - SD_RESPONSE_NO_ERROR: 成功
*/
u8 SD_GoIdleState(void)
{
    u8 Count = 3;
    u8 R1_Resp = 0;
    
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
//    Count = 0x1F;
//    do
//    {
//        R1_Resp = SD_WriteReadByte(0xFF);
//    }
//    while ((R1_Response & 0x80) && i--);
    /*!< 等待R1响应返回的状态为SD_IN_IDLE_STATE */
//    if () {
//        /*!< 响应不是空闲状态，失败返回 */
//        return SD_RESPONSE_FAILURE;
//    }

//    /*!< 片选CS高电平 */
//    SPI_CS_Release();
//    /*!< 发送 空 字节 0xFF */
//    SD_WriteByte(SD_DUMMY_BYTE);

    /*初始化成功返回*/
    return SD_RESPONSE_NO_ERROR;
}


///**
//* @brief  获取SD卡的版本类型，并区分SDSC和SDHC
//* @param  无
//* @retval SD响应:
//*         - SD_RESPONSE_FAILURE: 失败
//*         - SD_RESPONSE_NO_ERROR: 成功
//*/
//static void SD_GetCardType(void)
//{
//	__IO u8 Return = SD_RESPONSE_NO_ERROR;
//    uint32_t i = 0;
//    uint32_t Count = 0xFFF;
//    uint8_t R7R3_Resp[4];
//    uint8_t R1_Resp;

//	SD_Type = 0; //默认无卡
//    SD_DisSelect();
//    /*!< 片选信号CS低电平 */
//    SPI_CS_Select();
//	SD_SendCmd(SD_CMD_SEND_IF_COND, 0x1AA, 0x87);
//    Return = SD_ReadByte();
////	printfS("SD Card Type:0x%02X\r\n", Return);
//	/* 响应0x01, 则SD卡为 V2.0*/
//	if (Return == SD_IN_IDLE_STATE) 
//	{
//		for (i = 0; i < 4; i++)
//		{
//			R7R3_Resp[i] = SD_WriteReadByte(0xFF);    //Get trailing return value of R7 resp
//		}
//		if (R7R3_Resp[2] == 0x01 && R7R3_Resp[3] == 0xAA) //卡是否支持2.7~3.6V
//		{
//			Count = 0xFFFE;
//			/*发送主机支持的电压范围*/
//			do
//			{
//				SD_SendCmd(SD_CMD_APP_CMD, 0, 0x01); //发送CMD55
//				SD_SendCmd(SD_ACMD_SD_SEND_OP_COND, 0x40000000, 0x01); //发送CMD41
//                Return = SD_ReadByte();
//			}
//			while ((Return != SD_RESPONSE_NO_ERROR) && Count--);
//			
//			 //-----------鉴别SDSC SDHC卡类型开始-----------
//			
//			Count = 0xFFFE;
//			/*发送主机支持的电压范围*/
//			do
//			{
//                SD_SendCmd(SD_CMD_READ_OCR, 0, 0x01); ///*!< 发送CMD58 读取OCR寄存器 */
//                Return = SD_ReadByte();
//			}
//			while ((Return != SD_RESPONSE_NO_ERROR) && Count--);
//			if (Count == 0) 
//			{
//				SD_Type = SD_TYPE_NOT_SD;
//			}
//			
//			/*!< 读取CMD58的R3响应 */
//			for (i = 0; i < 4; i++)
//			{
//				R7R3_Resp[i] = SD_WriteReadByte(0xFF);    //得到OCR值
//			}
//			if (R7R3_Resp[0] & 0x40)
//			{
//				SD_Type = SD_TYPE_V2HC;    //检查CCS
//			}
//			else
//			{
//				SD_Type = SD_TYPE_V2;
//			}
//		}
//	}
//	//响应0x05, 非V2.0的卡
//	else if(Return == (SD_IN_IDLE_STATE | SD_ILLEGAL_COMMAND))
//	{
//		SD_SendCmd(SD_CMD_APP_CMD, 0, 0x01);     //发送CMD55
//		SD_SendCmd(SD_ACMD_SD_SEND_OP_COND, 0, 0x01); //发送CMD41
//        Return = SD_ReadByte();
//		if (Return <= 1)
//		{
//			SD_Type = SD_TYPE_V1;
//			Count = 0xFFFE;
//			/*发送主机支持的电压范围*/
//			do
//			{
//				SD_SendCmd(SD_CMD_APP_CMD, 0, 0x01); //发送CMD55
//				SD_SendCmd(SD_ACMD_SD_SEND_OP_COND, 0x40000000, 0x01); //发送CMD41
//                Return = SD_ReadByte();
//			}
//			while ((Return != SD_RESPONSE_NO_ERROR) && Count--);
//		}
//		else //MMC卡不支持CMD55+CMD41识别
//		{
//			SD_Type = SD_TYPE_NOT_SD; //MMC V3
//			Count = 0xFFFE;
//			do //等待退出IDLE模式
//			{
//				SD_SendCmd(SD_CMD_SEND_OP_COND, 0, 0x01); //发送CMD1
//                Return = SD_ReadByte();
//			}
//			while ((Return != SD_RESPONSE_NO_ERROR) && Count--);
//		}
//        SD_SendCmd(SD_CMD_SET_BLOCKLEN, 512, 0x01);
//		if (Count == 0 || SD_ReadByte() != 0)
//		{
//			SD_Type = SD_TYPE_NOT_SD;    //错误的卡
//		}
//	}

//    SD_DisSelect();//取消片选
//}
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
    do{
        R1_Resp = SD_WriteReadByte(0xFF);
    }while ((R1_Resp & 0x80) && Count--);
    if (Count == 0) {
        /*!< 等待超时 */
        return SD_RESPONSE_FAILURE;
    }

    //响应 = 0x05   非V2.0的卡
    if (R1_Resp == (SD_IN_IDLE_STATE|SD_ILLEGAL_COMMAND)) {
        /* 激活SD卡 */
        do {
            /*!< 片选信号CS高电平并发送空字节延时 0xFF */
            SD_DisSelect();
            /*!< 片选信号CS低电平 */
            SD_Select();
            /*!< 发送CMD1完成V1 版本卡的初始化 */
            SD_SendCmd(SD_CMD_SEND_OP_COND, 0, 0xFF);
            /*!< Wait for no error Response (R1 Format) equal to 0x00 */
        } while (SD_GetResponse(SD_RESPONSE_NO_ERROR));
        //V1版本的卡完成初始化

        SD_Type = SD_TYPE_V1;

        //不处理MMC卡

        //初始化正常

    }
    //响应 0x01   V2.0的卡
    else if (R1_Resp == SD_IN_IDLE_STATE) {
        /*!< 读取CMD8 的R7响应 */
        for (i = 0; i < 4; i++) {
            R7R3_Resp[i] = SD_ReadByte();
        }

        /*!< 片选信号CS高电平并发送空字节延时 0xFF */
        SD_DisSelect();
        /*!< 片选信号CS低电平 */
        SD_Select();

        //判断该卡是否支持2.7-3.6V电压
        if (R7R3_Resp[2]==0x01 && R7R3_Resp[3]==0xAA) {
            //支持电压范围，可以操作
            Count = 200;
            //发卡初始化指令CMD55+ACMD41
            do {
                //CMD55，以强调下面的是ACMD命令
                SD_SendCmd(SD_CMD_APP_CMD, 0, 0xFF);
                // SD_IN_IDLE_STATE
            if (!SD_GetResponse(SD_RESPONSE_NO_ERROR))
                    return SD_RESPONSE_FAILURE; //超时返回

                //ACMD41命令带HCS检查位
                SD_SendCmd(SD_ACMD_SD_SEND_OP_COND, 0x40000000, 0xFF);

                if (Count-- == 0)
                    return SD_RESPONSE_FAILURE; //重试次数超时
            } while (SD_GetResponse(SD_RESPONSE_NO_ERROR));

            //初始化指令完成，读取OCR信息，CMD58

            //-----------鉴别SDSC SDHC卡类型开始-----------

            Count = 200;
            do {
                /*!< 片选信号CS高电平并发送空字节延时 0xFF */
                SD_DisSelect();
                /*!< 片选信号CS低电平 */
                SD_Select();
                /*!< 发送CMD58 读取OCR寄存器 */
                SD_SendCmd(SD_CMD_READ_OCR, 0, 0xFF);
            } while (SD_GetResponse(SD_RESPONSE_NO_ERROR) || Count-- == 0);

            if (Count == 0)
                return SD_RESPONSE_FAILURE; //重试次数超时

            //响应正常，读取R3响应

            /*!< 读取CMD58的R3响应 */
            for (i = 0; i < 4; i++) {
                R7R3_Resp[i] = SD_ReadByte();
            }

            //检查接收到OCR中的bit30(CCS)
            //CCS = 0:SDSC       CCS = 1:SDHC
            if (R7R3_Resp[0]&0x40) { //检查CCS标志
                SD_Type = SD_TYPE_V2HC;
            } else {
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

/**
* @brief  初始化 SD/SD 卡
* @param  None
* @retval  SD 响应:
*         - SD_RESPONSE_FAILURE: 初始化失败
*         - SD_RESPONSE_NO_ERROR: 初始化成功
*/
u8 SD_Initialize(void)
{
    __IO uint32_t i = 0;
	__IO u8 R1_Response = 0;

    /*!< 初始化 SD_SPI 并拉高片选脚*/
    SD_IO_Init();
    SD_SPI_SpeedLow();
    
    /*!< CS高电平期间，发送 空 字节数据 0xFF, 10 次, 因为初始化SD卡需要生存至少74次的时钟信号*/
    for (i = 0; i < 10; i++) {
        /*!< 发送 空 字节数据 0xFF */
        SD_WriteByte(SD_DUMMY_BYTE);
    }

	/*!< 进入IDLE状态*/
    i = 10;
    do
    {
        R1_Response = SD_GoIdleState();
        if (i == 0)
            return SD_RESPONSE_FAILURE;
    } while ((R1_Response == SD_RESPONSE_FAILURE) && (i-- > 0));
	
    
    //获取卡的类型,最多尝试10次
    i=10;
    do
    {
        SD_GetCardType();
    } while ((SD_Type == SD_TYPE_NOT_SD) && (i-- > 0));

    //不支持的卡
    if (SD_Type == SD_TYPE_NOT_SD)
    {
        return SD_RESPONSE_FAILURE;
    }
    SD_SPI_SpeedHigh();
//    return SD_GetCardInfo(&SDCardInfo);    
	return SD_RESPONSE_NO_ERROR;
}



/**
* @brief  接收SD卡发送的数据
* @param  RxBuf: 接收缓存区
* @param  Size: 接收大小
* @return SD数据响应：
*		  - SD_DATA_OK：数据接收成功
*		  - SD_DATA_OTHER_ERROR：数据接收失败
*/
static u8 SD_RecvData(u8 Token, u8 *RxBuf, u16 Size)
{
    uint32_t i = 0;

    
    /*使用SPI读取SD的一扇数据前，需要判断数据块的头部是否有开始Token 0xFE*/
     if (!SD_GetResponse(Token)) {
            /*!< 读取一个数据块的数据 :  NumByteToRead 个数据 */
            for (i = 0; i < Size; i++) {
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

/**
* @brief  读SD卡
* @param  RxBuf:数据缓存区
* @param  StartSector:起始扇区
* @param  Cnt:读取的扇区数
* @retval  SD 响应:
*		  - SD_DATA_OK：数据写入成功
*		  - 其他：数据写入失败
*/
u8 SD_ReadDisk(u8* DataBuff, u32 StartSector, u8 Cnt)
{
    __IO u8 R1_Response = SD_RESPONSE_FAILURE;
    u8* RxBuf = DataBuff;
    u8 TryCount = 5;
    

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
            if (!SD_GetResponse(SD_RESPONSE_NO_ERROR)) {
                R1_Response = SD_RecvData(SD_START_DATA_SINGLE_BLOCK_READ, RxBuf, 512);
            }
        }
        /* 读多个扇区 */
        else
        {
            SD_SendCmd(SD_CMD_READ_MULT_BLOCK, StartSector, 0x01); //连续读命令  Crc0x01
            /*!< 检查R1响应 */
            if (!SD_GetResponse(SD_RESPONSE_NO_ERROR)) {
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

    while (i <= 64) {
        /*!< 读取响应 */
        response = SD_ReadByte();
        /*!< 屏蔽无关的数据位（前三位xxx） */
        response &= 0x1F;
        switch (response) {
        case SD_DATA_OK: {
            rvalue = SD_DATA_OK;
            break;
        }
        case SD_DATA_CRC_ERROR:
            return SD_DATA_CRC_ERROR;
        case SD_DATA_WRITE_ERROR:
            return SD_DATA_WRITE_ERROR;
        default: {
            rvalue = SD_DATA_OTHER_ERROR;
            break;
        }
        }
        /*!< 数据正常，退出循环 */
        if (rvalue == SD_DATA_OK)
            break;
        /*!< Increment loop counter */
        i++;
    }

    /*!< 等待空数据 */
    while (SD_ReadByte() == 0);

    /*!< 返回响应 */
    return response;
}

/**
* @brief  向SD卡写入一扇数据块，共512字节
* @param  TxBuf: 发送缓存区
* @param  Token: 写入数据块前，需要先发送的开始标志字节
* @return SD数据响应：
*		  - SD_DATA_OK：数据写入成功
*		  - 其他：数据写入失败
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
        for (i = 0; i < 512; i++) {
            /*!< 发送指针指向的字节 */
            SD_WriteByte(*TxBuf);
            /*!< 指针加1 */
            TxBuf++;
        }
        /*!< 两个字节的空CRC校验码，默认不验证 */
        SD_ReadByte();
        SD_ReadByte();
        /*!< 读取SD卡数据响应 */
        if (SD_GetDataResponse() != SD_DATA_OK) {
            return SD_RESPONSE_FAILURE;
        }
    }
    return SD_DATA_OK;
}


/**
* @brief  写SD卡
* @param  TxBuf:数据缓存区
* @param  StartSector:起始扇区
* @param  Cnt:写入的扇区数
* @retval  SD 响应:
*		  - SD_DATA_OK：数据写入成功
*		  - 其他：数据写入失败
*/
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
            if (!SD_GetResponse(SD_RESPONSE_NO_ERROR)) {
                R1_Response = SD_WriteBlock(TxBuf, SD_START_DATA_SINGLE_BLOCK_WRITE); //写512个字节
            }
        }
    /* 写多个扇区 */
        else
        {
    //		SD_SendCmd(SD_CMD_APP_CMD, 0, 0x01);
    //		SD_SendCmd(SD_CMD_SET_BLOCK_COUNT, Cnt, 0x01); //发送指令
            
            SD_SendCmd(SD_CMD_WRITE_MULT_BLOCK, StartSector, 0x01); //连续写命令
            if (!SD_GetResponse(SD_RESPONSE_NO_ERROR)) {
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
        }else
        {
            return R1_Response;
        }
    }
    return R1_Response;
}


