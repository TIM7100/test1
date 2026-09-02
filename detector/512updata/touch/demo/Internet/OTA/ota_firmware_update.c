#include "ota_firmware_update.h"
#include "ssl_direct.h"
#include "tcp_client.h"
#include "mqtt_client.h"
#include "cJSON.h"
#include "w5500_user_conf.h"
#include "ota_firmware_handle.h"
#include "ssl_direct.h"
#include "delay.h"
#include "ExternalFlash.h"
#include "crc.h"
#include "lcd.h"
// #include "menu_show.h"
#include "my_aes_ecb.h"
#include "rtc.h"
#include "w5500_user_conf.h"
#include "eflash.h"
#include "esp8266_config.h"

#define HEADER_LOCK_FLAG      0x3435
#define SHADOW_LOCK_FLAG      0x45464546

static OTA_INFO_t OtaInfo = {0};
static HTTP_PACKET_t HttpPacketInfo = {0};                          //???HTTP?????????
extern firmware_info_t firmware_info;
static SHADOW_INFO_t shadow_info = {0};

extern volatile u8 NetWork_Flag;

/*******************************************************
 * @brief    ???????????
 * @param    xxx:xxx
 * @return
********************************************************/
void InitialFirmwareInfo(void)
{
    memset(&firmware_info, 0, sizeof(firmware_info));
    strncpy(firmware_info.local_firmware_version, "000000000000", 12);
    firmware_info.KeyFlag = HEADER_LOCK_FLAG;
}

/*******************************************************
 * @brief    ???????õô????·Ú
 * @param    xxx:xxx
 * @return
********************************************************/
u8 UploadFirmwareVersion(void)
{
    char OTA_TxBuff[1024] = {0};
    u8 TimeOut = 3;

    while (ConnectTencentCloud() != OTA_SUCCESS)
    {
        if ((TimeOut--) == 0)
        {
            ConnectTencentCloudShow();
            return OTA_LINK_CLOUD_ERROR;
        }
    }
    sprintf(OTA_TxBuff, OTA_UPLOAD_VERSION_TEMPLATE, firmware_info.local_firmware_version);  //???????·Ú
    SendPubTopicInfo(OTA_PUB_TOPIC, OTA_TxBuff);
    UploadThingData();
    DisconnectTencentCloud();

    return OTA_SUCCESS;
}
/*******************************************************
 * @brief    ?????Firmware?????????§Þ???????W25Q
 * @param    xxx:xxx
 * @return
********************************************************/
void HandleFirmwareInfo(void)
{
    u16 cal_crc;
    u8 EncryptData[512];

    cal_crc = GetCrc16((char *)&firmware_info, 510);
    firmware_info.crc[0] = cal_crc >> 8;
    firmware_info.crc[1] = (u8)cal_crc;

    EncryptDataByAesECB((u8*)&firmware_info, ONCE_HANDLE_SIZE, EncryptData);
    ExFlashWrite(EncryptData, 0, ONCE_HANDLE_SIZE);
}

/*******************************************************
 * @brief    ????????????????????õô????·Ú
 * @param    xxx:xxx
 * @return
 *           OTA_CONNECT_SERVER_ERR     ????????????
 *           OTA_SUCCESS                ????????????
********************************************************/
u8 FactoryReset(void)
{
    /*?§Ø?????????????????·Ú???*/
    if (NetWork_Flag == W5500)
    {
        W5500Reset();
        if (W5500PhyLinkCheck() == 0)
        {
            return OTA_CONNECT_SERVER_ERR;
        }
    }
    InitialFirmwareInfo();
    HandleFirmwareInfo();
    if (UploadFirmwareVersion() != OTA_SUCCESS)
    {
        return OTA_CONNECT_SERVER_ERR;
    }

    return OTA_SUCCESS;
}

/*******************************************************
 * @brief    ??????????
 * @param    xxx:xxx
 * @return
********************************************************/
void UploadThingData(void)
{
    char OTA_TxBuff[1024] = {0};

    sprintf(OTA_TxBuff, (const char*)THING_UPLOAD_TEMPLATE, DEVICE_VERSION, shadow_info.ShadowCount);
    SendPubTopicInfo(THING_PUB_TOPIC, OTA_TxBuff);
}

/*******************************************************
 * @brief    ?????????
 * @param    xxx:xxx
 * @return
********************************************************/
u16 GetDeviceRemainCount(void)
{
    return shadow_info.ShadowCount;
}

/*******************************************************
 * @brief    ?????????????
 * @param    xxx:xxx
 * @return
********************************************************/
void DeviceShadowCountInit(void)
{
    u8 eflashBuffer[512];

    EflashReadPage(DATA_AREA_ADDR, 512, (u32*)eflashBuffer);
    memcpy(&shadow_info, eflashBuffer, sizeof(shadow_info));
    /*??????????????*/
    if (shadow_info.Lock != SHADOW_LOCK_FLAG)
    {
        memset(&shadow_info, 0, sizeof(shadow_info));
        shadow_info.Lock = SHADOW_LOCK_FLAG;
        memcpy(eflashBuffer, &shadow_info, sizeof(shadow_info));
        EflashWritePageHandle(DATA_AREA_ADDR, 512, eflashBuffer);
    }
}

/*******************************************************
 * @brief    ????????????
 * @param    xxx:xxx
 * @return
********************************************************/
u8 SetDeviceShadowCount(u32 Receive_Version, u32 Receive_Count)
{
    u8 eflashBuffer[512];

    /*??????????????·Ú?????????????·Ú??????§Ú???*/
    if (shadow_info.Version != Receive_Version)
    {
        if (shadow_info.CountValidFlag != 0)   /*¦Ä?????*/
        {
            shadow_info.ShadowCount +=  Receive_Count;
        }
        else                                   /*??????*/
        {
            shadow_info.ShadowCount =  Receive_Count;
        }
        shadow_info.Version = Receive_Version;      //????·Ú
        shadow_info.CountValidFlag = 1;

        EflashReadPage(DATA_AREA_ADDR, 512, (u32*)eflashBuffer);
        memcpy(eflashBuffer, &shadow_info, sizeof(shadow_info));
        EflashWritePageHandle(DATA_AREA_ADDR, 512, eflashBuffer);
        UploadThingData();
        return SHADOW_SET_SUCCESS;
    }
    return SHADOW_VERSION_ERROR;
}

/*******************************************************
 * @brief    ?????????????
 * @param    xxx:xxx
 * @return   OTA_CONNECT_SERVER_ERR??????????????
 *           OTA_SSL_ERR??    SSL???????
 *           OTA_SUCCESS??    ????????????????
********************************************************/
u8 ConnectTencentCloud(void)
{
    u8 Ret = 0;

    if (NetWork_Flag == W5500)
    {
        Ret = EthernetConnectServer(NULL, 0);                          //??????????????
        if (Ret != CONNECT_SUCCESS)
        {
            return OTA_CONNECT_SERVER_ERR;
        }
    }
    else
    {
        Ret = WiFi_Connect_IoTServer("iotcloud-mqtt.gz.tencentdevices.com", 8883, 100);
        if (Ret)
        {
            Ret = WiFi_Connect_IoTServer("iotcloud-mqtt.gz.tencentdevices.com", 8883, 100);
            if (Ret)
            {
                return OTA_CONNECT_SERVER_ERR;
            }
        }
    }
    SSL_Init();
    //SSL???????
    if (SSL_HandshakeAwait())
    {
        return OTA_SSL_ERR;
    }


		
    Ret = ConnectMqttClient();                                       //???????????????????????
    if (Ret != MQTT_SUCCESS)
    {
        return OTA_CONNECT_MQTT_ERR;
    }
    return OTA_SUCCESS;
}
/*******************************************************
 * @brief    ?????????????
 * @param    xxx:xxx
 * @return
********************************************************/
void DisconnectTencentCloud(void)
{
    //???????????
    if (DisconnectMqttClient() == DISCONNECT_ERROR)
    {
        //printfS("??????????, ??????: %d\r\n", Ret);
    }
    disconnect(SOCK_TCPS);                                              //?????????????
    close(SOCK_TCPS);                                                   //???Socket??
}

/*******************************************************
 * @brief    ???HTTP??????????????
 * @param    xxx:xxx
 * @return   WAIT_HTTP_ACK_TIMEOUT?? ???????????????§ß???????
 *           OTA_SUCCESS???????????????????????????????????
********************************************************/
static u8 WaitHttpAck(void)
{
    u8 Timeout = 20;
    u16 Len = 0;

    Timer0DelayMs(20);
    Len = getSn_RX_RSR(SOCK_TCPS);
    while (Len == 0)
    {
        Len = getSn_RX_RSR(SOCK_TCPS);
        Timeout--;
        if (Timeout == 0)
        {
            return OTA_HTTP_ERROR;
        }
        Timer0DelayMs(10);
    }
    return OTA_HTTP_SUCCESS;
}

/*******************************************************************************
 * @brief    ????NTP???????¡¤????????????????§Ö??????
 * @param    ??
 * @return   OTA_LOSE_INFO: ?????????????????
 *           OTA_DONT_UPDATA: ??§ß??????????????????????????
 *           OTA_SUCCESS: ?????????????????§Õ??
********************************************************************************/
u8 GetNTPTime(void)
{
    char OTA_RxBuff[1024] = {0};
    char OTA_TxBuff[200] = {0};
    char *ota_StartInfo;
    cJSON *Data, *ntptime2;
    char *bufferEnd;
    u64 NTPTime;

    sprintf(OTA_TxBuff, OTA_UPLOAD_NTP_TEMPLATE);    //???NTP?????????
    if (GetSubTopicInfo(SYS_PUB_TOPIC, SUB_SYS_NUM, OTA_TxBuff, OTA_RxBuff) != GET_SUB_TOPIC_DATA)
    {
        return OTA_DONT_UPDATA;
    }
    //???NTP JSON???
    ota_StartInfo = OTA_RxBuff;
    bufferEnd = ota_StartInfo + strlen(OTA_RxBuff);
    while ((ota_StartInfo < bufferEnd) && (*ota_StartInfo != '{'))
    {
        ota_StartInfo++;
    }
    if (ota_StartInfo >= bufferEnd)  //????????
    {
        return OTA_LOSE_INFO;
    }
    // cJSON????
    Data = cJSON_Parse(ota_StartInfo);
    if (Data == NULL)
    {
        return OTA_LOSE_INFO;
    }
    else
    {
        ntptime2 = cJSON_GetObjectItem(Data, "ntptime2");
        if (ntptime2 == NULL)
        {
            cJSON_Delete(Data);
            return OTA_LOSE_INFO;
        }
        NTPTime = (u64)ntptime2->valuedouble;
        RTC_Set(NTPTime / 1000);
        cJSON_Delete(Data);
        return OTA_SUCCESS;
    }
    return OTA_LOSE_INFO;
}

/*******************************************************************************
 * @brief    ????OTA?¡¤????????????????§Ö??????
 * @param    ??
 * @return   OTA_LOSE_INFO: ?????OTA??????????????
 *           OTA_DONT_UPDATA: ??§Û???????, ????????¡ã·Ú?????????????
 *           OTA_COMMAND_ERR: ????????????
 *           OTA_SUCCESS: ????????OTA????????????
********************************************************************************/
u8 GetOtaInfo(void)
{
    char OTA_RxBuff[1024] = {0};
    char OTA_TxBuff[200] = {0};
    char *ota_StartInfo;
    cJSON *Data, *fw_type, *file_size, *md5sum, *task_id, *type, *url, *version;
    char * url_index_start;
    char * url_index_end;
    char *bufferEnd;

    //1. ?????????????õô?????·Ú, ?????????????·Ú???
    sprintf(OTA_TxBuff, OTA_UPLOAD_VERSION_TEMPLATE, firmware_info.local_firmware_version);    //???????????
    if (GetSubTopicInfo(OTA_PUB_TOPIC, SUB_OTA_NUM, OTA_TxBuff, OTA_RxBuff) != GET_SUB_TOPIC_DATA)
    {
        return OTA_DONT_UPDATA;
    }
    //???OTA?????????????????????§³??URL?????????????·Ú??
    ota_StartInfo = OTA_RxBuff;
    bufferEnd = ota_StartInfo + strlen(OTA_RxBuff);
    while ((ota_StartInfo < bufferEnd) && (*ota_StartInfo != '{'))
    {
        ota_StartInfo++;
    }
    if (ota_StartInfo >= bufferEnd)  //????????
    {
        return OTA_LOSE_INFO;
    }
    // cJSON????
    Data = cJSON_Parse(ota_StartInfo);
    if (Data == NULL)
    {
        return OTA_LOSE_INFO;
    }
    else
    {
        file_size = cJSON_GetObjectItem(Data, "file_size");
        fw_type = cJSON_GetObjectItem(Data, "fw_type");
        md5sum = cJSON_GetObjectItem(Data, "md5sum");
        task_id = cJSON_GetObjectItem(Data, "task_id");
        type = cJSON_GetObjectItem(Data, "type");
        url = cJSON_GetObjectItem(Data, "url");
        version = cJSON_GetObjectItem(Data, "version");
        if (file_size == NULL || url == NULL || version == NULL)
        {
            cJSON_Delete(Data);
            return OTA_LOSE_INFO;
        }
        OtaInfo.FileSize = file_size->valueint;
        strncpy(OtaInfo.Md5Sum, md5sum->valuestring, sizeof(OtaInfo.Md5Sum));
        strncpy(OtaInfo.Version, version->valuestring, sizeof(OtaInfo.Version));

        url_index_start = url->valuestring;
        bufferEnd = url_index_start + strlen(url_index_start);
        while ((url_index_start < bufferEnd) && ((*url_index_start != '/') || (*(url_index_start + 1) != '/')))
        {
            url_index_start++;
        }
        if (url_index_start >= bufferEnd)  //????????
        {
            cJSON_Delete(Data);
            return OTA_COMMAND_ERR;
        }

        url_index_start += 2;   //????"//"

        url_index_end = url_index_start;
        bufferEnd = url_index_end + strlen(url_index_end);
        while ((url_index_end < bufferEnd) && (*url_index_end != '/'))
        {
            url_index_end++;
        }
        if (url_index_end >= bufferEnd)  //????????
        {
            cJSON_Delete(Data);
            return OTA_COMMAND_ERR;
        }

        strncpy(OtaInfo.Host, url_index_start, url_index_end - url_index_start);  //???????Host
        OtaInfo.Host[url_index_end - url_index_start] = '\0';
        strcpy(OtaInfo.URLABuff, url_index_end);                                  //???????URLABuff
        //???????????????·Ú
        strncpy(firmware_info.firmware_version, OtaInfo.Version, strlen(OtaInfo.Version));
        //???????
        cJSON_Delete(Data);
        return OTA_SUCCESS;
    }
    return OTA_LOSE_INFO;
}

/*******************************************************************************
 * @brief    ????????õô?¡¤????????????????§Ö??????
 * @param    ??
 * @return   OTA_LOSE_INFO: ?????????õô????????????
 *           OTA_DONT_UPDATA: ??§ß??????????????????????????
 *           SHADOW_VERSION_ERROR: ????õô????????·Ú????
 *           SHADOW_SET_SUCCESS: ??????????õô?·Ú????????????§Õ??
********************************************************************************/
u8 GetShadowInfo(void)
{
    char OTA_RxBuff[1024] = {0};
    char *ota_StartInfo;
    cJSON *Data, *payload, *version, *state, *desired, *SJY_COUNT;
    char *bufferEnd;

    //???????õô????
    if (GetSubTopicInfo(SHADOW_PUB_TOPIC, SUB_SHADOW_NUM, SHADOW_DATA_MODE, OTA_RxBuff) != GET_SUB_TOPIC_DATA)
    {
        return OTA_DONT_UPDATA;
    }
    //???????õôJSON???
    ota_StartInfo = OTA_RxBuff;
    bufferEnd = ota_StartInfo + strlen(OTA_RxBuff);
    while ((ota_StartInfo < bufferEnd) && (*ota_StartInfo != '{'))
    {
        ota_StartInfo++;
    }
    if (ota_StartInfo >= bufferEnd)  //????????
    {
        return OTA_LOSE_INFO;
    }
    Data = cJSON_Parse(ota_StartInfo);
    if (Data == NULL)
    {
        return OTA_LOSE_INFO;
    }
    else
    {
        payload = cJSON_GetObjectItem(Data, "payload");
        if (payload == NULL)
        {
            cJSON_Delete(Data);
            return OTA_LOSE_INFO;
        }
        version = cJSON_GetObjectItem(payload, "version");
        if (version == NULL)
        {
            cJSON_Delete(Data);
            return OTA_LOSE_INFO;
        }
        state = cJSON_GetObjectItem(payload, "state");
        if (state == NULL)
        {
            cJSON_Delete(Data);
            return OTA_LOSE_INFO;
        }
        desired = cJSON_GetObjectItem(state, "desired");
        if (desired == NULL)
        {
            cJSON_Delete(Data);
            return OTA_LOSE_INFO;
        }
        SJY_COUNT = cJSON_GetObjectItem(desired, "SJY_COUNT");
        if (SJY_COUNT == NULL)
        {
            cJSON_Delete(Data);
            return OTA_LOSE_INFO;
        }

        if (SetDeviceShadowCount(version->valueint, SJY_COUNT->valueint) != SHADOW_SET_SUCCESS)
        {
            cJSON_Delete(Data);
            return SHADOW_VERSION_ERROR;
        }
        cJSON_Delete(Data);
        return SHADOW_SET_SUCCESS;
    }
    return OTA_LOSE_INFO;
}
/******************************************************************
 * @brief    ???????????
 * @param    xxx:xxx
 * @return   OTA_SUCCESS?????????????
 *           OTA_CONNECT_SERVER_ERR: ????HTTP?????????
 *           OTA_SSL_ERR??   SSL???????
 *           OTA_HTTP_ERROR???????????????????HTTP??????????
 *           OTA_CRC_ERROR:  ?????????????????CRC§µ?????
 *           OTA_WRITE_ERROR???????????????????§Õ??SD?????
********************************************************************/
u8 StartDownloadFirmware(void)
{
    u8  DownloadState = OTA_COMMAND_UPDATA;                     //???????HTTP???????????????
    u16 DownloadCount = 0;                                      //???????¦É???????????????
    u32 StartAddress;                                           //??¦Ç????????????
    u32 EndAddress;                                             //??¦Ç?????????????
    u32 AckStartAddress = 0;                                    //HTTP??????????
    u32 AckEndAddress = 0;                                      //HTTP???????????
    u8  CrcErrCount = CRC_ERROR_COUNT;                          //??????¦Ç???????????CRC§µ??????????????
    u16 CurrentCRC = 0;                                         //????????????????????CRC
    char DataBuff[DOWNLOAD_SIZE_ONCE + 512];                    //?????????
    u8  *TxPoint = 0;
    u32 Write_W25q_Start_Addr;

    RunningShow(DOWNLOAD_RUNNING); //?????????

    //????HTTP??????????????????????????????
    SSL_Init();
    DownloadState = EthernetConnectServer((const char*)OtaInfo.Host, OTA_SERVER_PORT);          //????OTA?????????HTTP§¿??
    if (DownloadState != CONNECT_SUCCESS)
    {
        printfS("????????????, ??????: %d\r\n", DownloadState);
        return OTA_CONNECT_SERVER_ERR;
    }
    //SSL???????
    if (SSL_HandshakeAwait())
    {
        return OTA_SSL_ERR;
    }
    //???§Õ????
    Write_W25q_Start_Addr = GET_NEXT_FIRMWARE_WRITE_ADDRESS(firmware_info.FirmwareAreaFlag);
    //?????????????
    DownloadCount = OtaInfo.FileSize / DOWNLOAD_SIZE_ONCE;
    if (OtaInfo.FileSize % DOWNLOAD_SIZE_ONCE != 0)
    {
        DownloadCount += 1;
    }
    //?§Ø????????, ????????????????????¦Ä????????????
    if ((HttpPacketInfo.DownloadCount != DownloadCount) || (HttpPacketInfo.DownloadNum == 0))
    {
        /* ??¦È?????, ?????????????? */
        HttpPacketInfo.DownloadCount = DownloadCount;
        HttpPacketInfo.DownloadNum = 0;
    }
    else
    {
        /* ?????????????????¦Ë??, ????????¡¤?????? */
    }

    //??????????????????????
    for (; HttpPacketInfo.DownloadNum < HttpPacketInfo.DownloadCount; HttpPacketInfo.DownloadNum++)
    {
        CrcErrCount = CRC_ERROR_COUNT;
        DownloadState = OTA_COMMAND_UPDATA;
        /*????????????????????§Ö???????????????*/
        StartAddress = HttpPacketInfo.DownloadNum * DOWNLOAD_SIZE_ONCE;
        if (HttpPacketInfo.DownloadNum != HttpPacketInfo.DownloadCount - 1)
        {
            EndAddress = StartAddress + DOWNLOAD_SIZE_ONCE - 1;
        }
        else
        {
            //??????????????????????????????????????????????????§³
            EndAddress = OtaInfo.FileSize - 1;
        }
AgainSend:
        /* ???socket?????????, ????????????, ???HTTP??????, ???????HTTP?????? */
        //SSL_ReadData((u8 *)DataBuff, sizeof(DataBuff));
        memset(DataBuff, 0, sizeof(DataBuff));
        sprintf(DataBuff, HTTP_UPLOAD_TEMPLATE, OtaInfo.URLABuff, OtaInfo.Host, (int)StartAddress, (int)EndAddress);
        SSL_SendData((u8*)DataBuff, strlen((const char*)DataBuff));
        //???????Http???????, ??HTTP???????????????OTA_HTTP_ERROR
        if (WaitHttpAck() == OTA_HTTP_ERROR)
        {
            DownloadState = OTA_HTTP_ERROR;
            break;
        }
        /*Http???????????????????????????? */
        memset(DataBuff, 0, sizeof(DataBuff));
        SSL_ReadData((u8 *)DataBuff, sizeof(DataBuff));                //????????
        if (strstr(DataBuff, HTTP_ACK_PACKET) != NULL)                 //?§Ø???????????????
        {
            //Content-Range: bytes StartAddress-EndAddress/108616????????????????????????§Ö????¦Ë
            if ((sscanf(strstr(DataBuff, "Content-Range"), "Content-Range: bytes %d-%d/%*d", &AckStartAddress, &AckEndAddress) == 2))
            {
                /*?§Ø?HTTP?????????????????*/
                if (AckStartAddress != StartAddress || AckEndAddress != EndAddress)
                {
                    DownloadState = OTA_HTTP_ERROR;
                    break;
                }
                //?????????????
                TxPoint = (u8 *)strstr(DataBuff, "\r\n\r\n") + 4;           //???§Á????????????????????\r\n\r\n
                /* ????CRC */
                CurrentCRC = GetCrc16((char *)TxPoint, WRITE_W25Q_SIZE);
                if (CurrentCRC != ((TxPoint[EndAddress - StartAddress - 1] << 8) + TxPoint[EndAddress - StartAddress]))
                {
                    printfS("CRC §µ?????\r\n");
                    DownloadState = OTA_CRC_ERROR;
                    break;
                }
                //???§Õ??25q
                if (ExFlashWrite(TxPoint, Write_W25q_Start_Addr + HttpPacketInfo.DownloadNum * (WRITE_W25Q_SIZE), EndAddress - StartAddress - 1) != W25Q_SUCC)
                {
                    DownloadState = OTA_WRITE_ERROR;
                    break;
                }
                ProgressBarShow(HttpPacketInfo.DownloadCount, HttpPacketInfo.DownloadNum + 1);                      //?????????
            }
        }
        /* ????????????????????????OTA_HTTP_ERROR??????*/
        else
        {
            DownloadState = OTA_HTTP_ERROR;
            break;
        }
    }
    /* ????????????, ????????HTTP?????????, ???????????, ???????????????, ?????¦Í??????? */
    switch (DownloadState)
    {
    case OTA_HTTP_ERROR:                                       //HTTP??????????????????HTTP??????
        DownloadState = ConnectServerAgain();
        if (DownloadState == CONNECT_SUCCESS)                 //????????????????
        {
            DownloadState = OTA_CONNECT_TIMEOUT;
            goto AgainSend;
            break;
        }
        else if (DownloadState == SOCKET_CONNECT_ERROR)        //HTTP???????
        {
            DownloadState = OTA_HTTP_ERROR;
        }
        else if (DownloadState == NETWORK_CABLE_LINK_ERROR)    //???????????
        {
            DownloadState = OTA_LINK_ERR;
        }
        break;
    case OTA_CRC_ERROR:
        if (CrcErrCount-- != 0)
        {
            DownloadState = OTA_CRC_ERROR;
            goto AgainSend;
            break;
        }
        break;
    }
    /* ?????? */
    disconnect(SOCK_TCPS);              //???HTTPS??????????
    if (DownloadState == OTA_COMMAND_UPDATA)
    {
        DownloadState = OTA_COMMAND_ANALYSIS;    //???????????????????????
        printfS("Download Count:%d\r\nReality Download Count: %d\r\n", HttpPacketInfo.DownloadCount, HttpPacketInfo.DownloadNum);
        HttpPacketInfo.DownloadCount = 0;
        HttpPacketInfo.DownloadNum = 0;
    }
    return DownloadState;
}



