#include "ExternalFlash.h"
#include "delay.h"
#include "ota_firmware_handle.h"


static u8 CmdBuffer[16];
extern firmware_info_t firmware_info;

// #define W25Q_ENABLE()   (REG_GPIO_CLR(GPIOA) |= (1 << 3))
// #define W25Q_DISABLE()  (REG_GPIO_SET(GPIOA) |= (1 << 3))

#define W25Q1_ENABLE()   (REG_GPIO_CLR(GPIOA) |= (1 << 3))
#define W25Q1_DISABLE()  (REG_GPIO_SET(GPIOA) |= (1 << 3))

#define W25Q2_ENABLE()   (REG_GPIO_CLR(GPIOA) |= (1 << 7))
#define W25Q2_DISABLE()  (REG_GPIO_SET(GPIOA) |= (1 << 7))
  
	
//根据对应的标志位进行两个外部flash的切换
#define W25Q_ENABLE() \
    do { \
        if (firmware_info.FirmwareAreaFlag == 0 || firmware_info.FirmwareAreaFlag == 2) { \
            W25Q1_ENABLE(); \
            W25Q2_DISABLE(); \
        } else { \
            W25Q2_ENABLE(); \
            W25Q1_DISABLE(); \
        } \
    } while(0)

#define W25Q_DISABLE()  do { W25Q1_DISABLE(); W25Q2_DISABLE(); } while(0)

//初始化SPI FLASH的IO口
void ExFlashInit(void)
{
    spi_init(SPI_W25Q, WORK_MODE_0);
    //wp1
    REG_SCU_MUXCTRLA &= ~(0x03 << 12);   //设置管脚复用为GPIO6
    REG_GPIO_DIR(GPIOA) |= (1 << 6);    //RES为输出
    REG_GPIO_CLR(GPIOA) |= (1 << 6);
    //cs1
    REG_SCU_MUXCTRLA &= ~(0x03 << 6);    //设置管脚复用为GPIO3
    REG_GPIO_DIR(GPIOA) |= (1 << 3);     //RES为输出
    REG_GPIO_SET(GPIOA) |= (1 << 3);
    //cs2
    REG_SCU_MUXCTRLA &= ~(0x03 << 14);   //设置管脚复用为GPIO7
    REG_GPIO_DIR(GPIOA) |= (1 << 7);     //RES为输出
    REG_GPIO_SET(GPIOA) |= (1 << 7);
}

//读取芯片ID
u16 ExFlashReadID(void)
{
    UINT8 Id[2];

    W25Q_ENABLE();
//  REG_GPIO_CLR(GPIOA) |= (1 << 3);
    CmdBuffer[0] = 0x90;
    CmdBuffer[1] = 0x00;
    CmdBuffer[2] = 0x00;
    CmdBuffer[3] = 0x00;
    spi_tx_bytes(SPI_W25Q, CmdBuffer, 4);

    spi_rx_bytes(SPI_W25Q, Id, 2);
    W25Q_DISABLE();

    return (Id[0] << 8 | Id[1]);
}

//读取SPI FLASH
//在指定地址开始读取指定长度的数据
//pBuffer:数据存储区
//ReadAddr:开始读取的地址(24bit)
//NumByteToRead:要读取的字节数(最大65535)
void ExFlashRead(u8* pBuffer, u32 ReadAddr, u32 NumByteToRead)
{
    u16 ReadI;
    u32 ReadCnt;

    W25Q_ENABLE();
    CmdBuffer[0] = W25X_ReadData;           //发送读取命令
    CmdBuffer[1] = (u8)((ReadAddr) >> 16);  //发送24bit地址
    CmdBuffer[2] = (u8)((ReadAddr) >> 8);
    CmdBuffer[3] = (u8)ReadAddr;
    spi_tx_bytes(SPI_W25Q, CmdBuffer, 4);

    ReadCnt = NumByteToRead / EXFLASH_PAGE_SIZE;
    for (ReadI = 0; ReadI < ReadCnt; ReadI++)   //整数页读取，一次读取1页数据
    {
        spi_rx_bytes(SPI_W25Q, pBuffer + (ReadI * EXFLASH_PAGE_SIZE), EXFLASH_PAGE_SIZE);
    }

    if (NumByteToRead % EXFLASH_PAGE_SIZE)      //剩下不足一页读取
    {
        spi_rx_bytes(SPI_W25Q, pBuffer + (ReadI * EXFLASH_PAGE_SIZE), (NumByteToRead % EXFLASH_PAGE_SIZE));
    }

    W25Q_DISABLE();
}

//W25QXX写使能
//将WEL置位
void ExFlashWriteEnable(void)
{
    W25Q_ENABLE();

    CmdBuffer[0] = W25X_WriteEnable;          //发送写使能
    spi_tx_bytes(SPI_W25Q, CmdBuffer, 1);

    W25Q_DISABLE();
}

//W25QXX写禁止
//将WEL清零
void ExFlashWriteDisable(void)
{
    W25Q_ENABLE();

    CmdBuffer[0] = W25X_WriteDisable;         //发送写禁止指令
    spi_tx_bytes(SPI_W25Q, CmdBuffer, 1);

    W25Q_DISABLE();
}

//读取W25QXX的状态寄存器
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00
u8 ExFlashReadSR(void)
{
    W25Q_ENABLE();

    CmdBuffer[0] = W25X_ReadStatusReg;        //发送读取状态寄存器命令
    spi_tx_bytes(SPI_W25Q, CmdBuffer, 1);
    spi_rx_bytes(SPI_W25Q, CmdBuffer, 1);     //读取一个字节

    W25Q_DISABLE();
    return CmdBuffer[0];
}

//等待空闲
u8 ExFlashWaitBusy(void)
{
    u16 Timeout = W25Q_TIMEOUT;
    while (Timeout--)
    {
        Timer0DelayMs(1);
        if ((ExFlashReadSR() & 0x01) == 0x00)
        {
            return W25Q_SUCC;
        }

    }

    return W25Q_FAIL;
}

//擦除一个扇区
//Dst_Addr:扇区地址 根据实际容量设置
//擦除一个扇区的最少时间:150ms
u8 ExFlashEraseSector(u32 Dst_Addr)
{
    //监视falsh擦除情况,测试用
    //printf("fe:%x\r\n",Dst_Addr);
    Dst_Addr *= 4096;
    ExFlashWriteEnable();                     //SET WEL

    if (ExFlashWaitBusy())
    {
        return W25Q_FAIL;
    }

    W25Q_ENABLE();
    CmdBuffer[0] = W25X_SectorErase;          //发送扇区擦除指令
    CmdBuffer[1] = (u8)((Dst_Addr) >> 16);    //发送24bit地址
    CmdBuffer[2] = (u8)((Dst_Addr) >> 8);
    CmdBuffer[3] = (u8)Dst_Addr;
    spi_tx_bytes(SPI_W25Q, CmdBuffer, 4);
    W25Q_DISABLE();

    if (ExFlashWaitBusy())                       //等待擦除完成
    {
        return W25Q_FAIL;
    }
    return W25Q_SUCC;
}

//擦除一个块
//Dst_Addr:块地址 根据实际容量设置
//擦除一个块的最少时间:150ms
u8 ExFlashEraseBlock(u32 Dst_Addr)
{
    //监视falsh擦除情况,测试用
    //printf("fe:%x\r\n",Dst_Addr);
    Dst_Addr *= 65536;
    ExFlashWriteEnable();                     //SET WEL

    if (ExFlashWaitBusy())
    {
        return W25Q_FAIL;
    }

    W25Q_ENABLE();
    CmdBuffer[0] = W25X_BlockErase;           //发送块擦除指令
    CmdBuffer[1] = (u8)((Dst_Addr) >> 16);    //发送24bit地址
    CmdBuffer[2] = (u8)((Dst_Addr) >> 8);
    CmdBuffer[3] = (u8)Dst_Addr;
    spi_tx_bytes(SPI_W25Q, CmdBuffer, 4);
    W25Q_DISABLE();

    if (ExFlashWaitBusy())                       //等待擦除完成
    {
        return W25Q_FAIL;
    }
    return W25Q_SUCC;
}

//擦除整个芯片
//等待时间超长...
u8 ExFlashEraseChip(void)
{
    ExFlashWriteEnable();                     //SET WEL

    if (ExFlashWaitBusy())
    {
        return W25Q_FAIL;
    }

    W25Q_ENABLE();
    CmdBuffer[0] = W25X_ChipErase;            //发送片擦除命令
    spi_tx_bytes(SPI_W25Q, CmdBuffer, 1);
    W25Q_DISABLE();

    if (ExFlashWaitBusy())                       //等待擦除完成
    {
        return W25Q_FAIL;
    }
    return W25Q_SUCC;
}

//SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!
u8 ExFlashWritePage(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
    ExFlashWriteEnable();                      //SET WEL
    W25Q_ENABLE();
    CmdBuffer[0] = W25X_PageProgram;           //发送写页命令
    CmdBuffer[1] = (u8)((WriteAddr) >> 16);    //发送24bit地址
    CmdBuffer[2] = (u8)((WriteAddr) >> 8);
    CmdBuffer[3] = (u8)WriteAddr;
    spi_tx_bytes(SPI_W25Q, CmdBuffer, 4);

    spi_tx_bytes(SPI_W25Q, pBuffer, NumByteToWrite);    //写入
    W25Q_DISABLE();
    if (ExFlashWaitBusy())                                 //等待写入结束
    {
        return W25Q_FAIL;
    }
    return W25Q_SUCC;
}



//无检验写SPI FLASH
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
u8 ExFlashWriteNoCheck(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
    u16 Page_Remain;


    Page_Remain = 256 - WriteAddr % 256; //单页剩余的字节数
    if (NumByteToWrite <= Page_Remain)
    {
        Page_Remain = NumByteToWrite;    //不大于256个字节
    }
    while (1)
    {
        if (ExFlashWritePage(pBuffer, WriteAddr, Page_Remain))
        {
            return W25Q_FAIL;
        }
        if (NumByteToWrite == Page_Remain)
        {
            break;    //写入结束了
        }
        else //NumByteToWrite>pageremain
        {
            pBuffer += Page_Remain;
            WriteAddr += Page_Remain;

            NumByteToWrite -= Page_Remain;          //减去已经写入了的字节数
            if (NumByteToWrite > 256)
            {
                Page_Remain = 256;               //一次可以写入256个字节
            }
            else
            {
                Page_Remain = NumByteToWrite;    //不够256个字节了
            }
        }
    }
    return W25Q_SUCC;

}

//写SPI FLASH
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
u8 W25QXX_BUFFER[4096];
u8 ExFlashWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
    u32 secpos;
    u16 secoff;
    u16 secremain;
    u16 i;
    u8 * W25QXX_BUF;
    W25QXX_BUF = W25QXX_BUFFER;
    secpos = WriteAddr / 4096; //扇区地址
    secoff = WriteAddr % 4096; //在扇区内的偏移
    secremain = 4096 - secoff; //扇区剩余空间大小


    if (NumByteToWrite <= secremain)
    {
        secremain = NumByteToWrite;    //不大于4096个字节
    }
    while (1)
    {
        ExFlashRead(W25QXX_BUF, secpos * 4096, 4096); //读出整个扇区的内容
        for (i = 0; i < secremain; i++) //校验数据
        {
            if (W25QXX_BUF[secoff + i] != 0XFF)
            {
                break;    //需要擦除
            }
        }
        if (i < secremain) //需要擦除
        {
            if (ExFlashEraseSector(secpos) != W25Q_SUCC)      //擦除这个扇区
            {
                ExFlashInit();
                if (ExFlashEraseSector(secpos) != W25Q_SUCC)
                {
                    return W25Q_FAIL;
                }
            }

            for (i = 0; i < secremain; i++)    //复制
            {
                W25QXX_BUF[i + secoff] = pBuffer[i];
            }
            if (ExFlashWriteNoCheck(W25QXX_BUF, secpos * 4096, 4096) != W25Q_SUCC)
            {
                ExFlashInit();
                if (ExFlashWriteNoCheck(W25QXX_BUF, secpos * 4096, 4096) != W25Q_SUCC)
                {
                    return W25Q_FAIL;
                }
            }

        }
        else
        {
            if (ExFlashWriteNoCheck(pBuffer, WriteAddr, secremain) != W25Q_SUCC)   //写已经擦除了的,直接写入扇区剩余区间.
            {
                ExFlashInit();
                if (ExFlashWriteNoCheck(pBuffer, WriteAddr, secremain) != W25Q_SUCC)
                {
                    return W25Q_FAIL;
                }
            }
        }
        if (NumByteToWrite == secremain)
        {
            break;    //写入结束了
        }
        else//写入未结束
        {
            secpos++;//扇区地址增1
            secoff = 0; //偏移位置为0

            pBuffer += secremain;               //指针偏移
            WriteAddr += secremain;             //写地址偏移
            NumByteToWrite -= secremain;        //字节数递减
            if (NumByteToWrite > 4096)
            {
                secremain = 4096;    //下一个扇区还是写不完
            }
            else
            {
                secremain = NumByteToWrite;    //下一个扇区可以写完了
            }
        }
    };
    return W25Q_SUCC;
}
