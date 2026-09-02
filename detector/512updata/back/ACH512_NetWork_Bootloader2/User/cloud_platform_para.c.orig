#include "cloud_platform_para.h"
#include "eflash.h"

static char *PRODUCT_ID = NULL;
static char *DEVICE_NAME = NULL;
static char *DEVICE_SECRET = NULL;
static char ErrorString[] = "ERROR";
/***********************************************************************************************
 *                                      订阅主题添加区                                           *
************************************************************************************************/
const char SubTopicBuff[SUB_TOPIC_COUNT][64] = { "$ota/update/%s/%s", 
												 "$thing/down/property/%s/%s",
                                                 "$sys/operation/result/%s/%s",
												 "$shadow/operation/result/%s/%s"
									         };


void LoadDeviceCloudParameter(void)
{
	char DeviceNameHeader[32];
	
	strncpy(DeviceNameHeader, DEVICE_NAME_HEADER, sizeof(DEVICE_NAME_HEADER));
	
	if(((*(char*)(DEVICE_NAME_ADDR)) == DeviceNameHeader[0]) && ((*(char*)(DEVICE_NAME_ADDR+ 1)) == DeviceNameHeader[1]) \
		&& ((*(char*)(DEVICE_NAME_ADDR + 2)) == DeviceNameHeader[2]))
	{
		PRODUCT_ID = (char*)(PRODUCT_ID_ADDR);
		DEVICE_NAME = (char*)(DEVICE_NAME_ADDR);
		DEVICE_SECRET = (char*)(DEVICE_SECRET_ADDR);
	}
	else
	{
		PRODUCT_ID = ErrorString;
		DEVICE_NAME = ErrorString;
		DEVICE_SECRET = ErrorString;
	}
}

char* GetDeviceName(void)
{
	return DEVICE_NAME;
}

void ServerParameterInit(SERVER_INFOR *ServerInfor)
{
	memcpy(ServerInfor->DomainName, SERVER_NAME, sizeof(SERVER_NAME));
	memset(ServerInfor->ServerIP, 0, sizeof(ServerInfor->ServerIP));
	ServerInfor->ServerPort = SERVERPORT;
}

void CloudParameterInit(CLOUD_INFOR *CloudInfor)
{
	strncpy(CloudInfor->ProductKey, PRODUCT_ID, strlen(PRODUCT_ID) + 1);
	strncpy(CloudInfor->DeviceName, DEVICE_NAME, strlen(DEVICE_NAME) + 1);
	strncpy(CloudInfor->DeviceSecret, DEVICE_SECRET, strlen(DEVICE_SECRET) + 1);
}

void MqttParameterInit(MQTT_INFOR *MqttInfor)
{
	memset(MqttInfor->ClientID, 0, sizeof(MqttInfor->ClientID));
	memset(MqttInfor->Username, 0, sizeof(MqttInfor->Username));
	memset(MqttInfor->Passward, 0, sizeof(MqttInfor->Passward));
}

void MqttPackerParameterInit(MQTT_PACKET_INFOR *MqttPacket)
{
	u8 i = 0;
	char SubTopic[64] = {0};
	
	memcpy(MqttPacket->PubTopic, OTA_PUB_TOPIC, sizeof(OTA_PUB_TOPIC));
/* 添加新的订阅主题后，需要在这里添加一行初始化 */
	for (i = 0; i < SUB_TOPIC_COUNT; i++)
	{
		memset(SubTopic, 0, sizeof(SubTopic));
		sprintf(SubTopic, SubTopicBuff[i], PRODUCT_ID, DEVICE_NAME);
		memcpy(MqttPacket->SubTopic[i], SubTopic, sizeof(SubTopic));
	}
	MqttPacket->Qos = 0;
	MqttPacket->KeepAlive = KEEP_ALIVE;
	MqttPacket->CleanSession = CLEAN_SESSION;
/* 报文数据缓存区 */
	memset(MqttPacket->DataPacket, 0, DATA_PACKET_LEN);
	MqttPacket->DataPacketLen = DATA_PACKET_LEN;
/* 有效载荷数据缓存区 */	
	memset(MqttPacket->UploadTemplate, 0, UPLOAD_TEMPLATE_LEN);	
//	MqttPacket->UploadTemplateLen = 0;	
}

void MqttPubTopicInit(MQTT_PACKET_INFOR *MqttPacket, const char *PubTopic)
{
	char PubTopicBuf[64] = {0};
	if (PubTopic != NULL)
	{
		sprintf(PubTopicBuf, PubTopic, PRODUCT_ID, DEVICE_NAME);
		memset(MqttPacket->PubTopic, 0, 64);
		memcpy(MqttPacket->PubTopic, PubTopicBuf, strlen(PubTopicBuf));
	}
}




