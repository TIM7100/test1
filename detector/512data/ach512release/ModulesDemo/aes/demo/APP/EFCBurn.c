#include "EFCBurn.h"
#include  "eflash.h"

u8 eflash_write_arr(UINT32 addr, UINT8 *SendData, UINT32 SendLen)
{
    int sendbuf;
    int page, endpage;
    static int i = 0;

    endpage = SendLen % 512;    //每一页512字节
    if (endpage != 0)
    {
        endpage++;  //写满一页
    }

    for (page = 0; page < endpage; page++)
    {
        for (; i < 512 * (page + 1); i = i + 4)
        {
            REG_EFC_CTRL |= EFC_WRITE_MODE;//写使能
            REG_EFC_SEC = 0x55AAAA55;   //解锁写安全保护
            sendbuf = (SendData[i] | SendData[i + 1] << 8 | SendData[i + 2] << 16 | SendData[i + 3] << 24); //每四个字节进行一次写入
            *((volatile UINT32 *)(addr + i)) = sendbuf;
            while (!(REG_EFC_STATUS & 0x01));
            REG_EFC_CTRL &= ~EFC_WRITE_MODE;
        }
    }
    printfS("-----eflash Burn over-------\n");
    return 0;
}
