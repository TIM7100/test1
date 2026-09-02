/***********************************************************************************************
 *                                  影子设备功能函数集合
 *   注意：使用获取影子设备GetShadowInfo()函数前，需要先连接上云平台
 *         上报物模型时，也需要先确保连接上云平台
************************************************************************************************/
#include "shadow.h"
#include "bsp_eflash.h"
#include "mqtt_client.h"
#include "cloud_platform_para.h"

#define COUNT_SAVE_ADDRESS    (0x80000 - 0x200)
#define LOCK_FLAG              0xA55A8778
#define DEFAULT_COUNT          0
#define SHADOW_DATA_MODE      "{\"type\":\"get\",\"clientToken\":1}"
#define THING_UPDATA_COUNT    "{\"method\":\"report\",\"clientToken\":\"123\",\"timestamp\":1628646783,\"params\":{\"HP95_90Count\":%d,\"HP97Count\":%d}}"

UpdataCount_t UpdataCount;

void UpdataCountParameterInit(void)
{
    EflashReadPage(COUNT_SAVE_ADDRESS, sizeof(UpdataCount), (u32 *)&UpdataCount);
    if (UpdataCount.Lock != LOCK_FLAG)
    {
        printfS("Init updata count");
        UpdataCount.Lock = LOCK_FLAG;
        UpdataCount.Version = 0;
        UpdataCount.HP95_90Count = DEFAULT_COUNT;
        UpdataCount.HP97Count = DEFAULT_COUNT;
        EflashEraseArea(COUNT_SAVE_ADDRESS, 512);
        EflashWritePage(COUNT_SAVE_ADDRESS, sizeof(UpdataCount), (u8 *)&UpdataCount);
    }
    printfS(" Lock:%08X, Version:%d, HP95Count:%d, HP97Count:%d\r\n", 
             UpdataCount.Lock, UpdataCount.Version, UpdataCount.HP95_90Count, UpdataCount.HP97Count);
} 


u8 SaveUpdataCountParameter(u32 Version, u32 HP95_90Count, u32 HP97Count)
{
    if (Version == UpdataCount.Version)
    {
        printfS("Shadow Version \r\n");
        return SHADOW_VERSION_ALIKE;
    }
    else if(Version == (UpdataCount.Version + 1))
    {
        UpdataCount.Version = Version;
        UpdataCount.HP95_90Count += HP95_90Count;
        UpdataCount.HP97Count += HP97Count;
        EflashEraseArea(COUNT_SAVE_ADDRESS, 512);
        EflashWritePage(COUNT_SAVE_ADDRESS, sizeof(UpdataCount), (u8 *)&UpdataCount);
        printfS(" Lock:%08X, Version:%d, HP95Count:%d, HP97Count:%d\r\n", 
             UpdataCount.Lock, UpdataCount.Version, UpdataCount.HP95_90Count, UpdataCount.HP97Count);
        return SHADOW_SUCCESS;
    }
    return SHADOW_VERSION_ERR;
}


u8 GetShadowInfo(void)
{
    char RxBuff[400] = {0};
    u32 HP95_90Count = 0;
    u32 HP97Count = 0;
    u32 Version = 0;
    u32 Result = 0;
    u8 Len;
    //获取影子设备数据
    if (GetSubTopicInfo(SHADOW_PUB_TOPIC, SUB_SHADOW_NUM, SHADOW_DATA_MODE, RxBuff) != GET_SUB_TOPIC_DATA)
    {
        return SHADOW_GET_FAIL;
    }
   // {"desired":{"HP95_90Count":10,"HP97Count":10}}
    //处理获取的数据
    Len = sscanf(RxBuff, "%*[^{]{%*[^{]{%*[^{]{%*[^H]HP95_90Count\":%d,\"HP97Count\":%d%*[^v]version\":%d},\"result\":%d",&HP95_90Count, &HP97Count, &Version, &Result);
    if (Len != 4)
    {
        printfS("Len:%d, HP95_90Count:%d, HP97Count:%d, Version:%d, Result:%d\r\n", Len, HP95_90Count, HP97Count, Version, Result);
        return SHADOW_GET_FAIL;
    }
    
    //判断获取数据是否正确并保存数据
    if (Result != 0)
    {
        return SHADOW_GET_FAIL;
    }
    return  SaveUpdataCountParameter(Version, HP95_90Count, HP97Count);
}


void GetUpdataCount(u32* HP95_90Count, u32* HP97Count)
{
    *HP95_90Count = UpdataCount.HP95_90Count;
    *HP97Count = UpdataCount.HP97Count;
}

void ThingUploadUpdataCount(void)
{
    char ThingBuff[400];
    
    sprintf(ThingBuff, THING_UPDATA_COUNT, UpdataCount.HP95_90Count, UpdataCount.HP97Count);
    SendPubTopicInfo(THING_PUB_TOPIC, ThingBuff);
}
