#ifndef _CLOUDPLATFORM_H_
#define _CLOUDPLATFORM_H_
#include "common.h"


/***********************************************************************************************
 *                               云平台域名和端口的更改位置                                      *
************************************************************************************************/
#define SERVER_NAME    "G0C7J0Z3BO.iotcloud.tencentdevices.com"       //"iotcloud-mqtt.gz.tencentdevices.com"          
#define SERVERPORT     8883                           		          //8883   // 配置远程服务器端口


/***********************************************************************************************
 *                               设备版本             										*
************************************************************************************************/
#define DEVICE_VERSION "V1_1"								//设备版本

/***********************************************************************************************
 *                            云平台三元组内部FLASH存储地址             									*
************************************************************************************************/
#define PRODUCT_ID_ADDR       0x2FC00
#define DEVICE_NAME_ADDR      0x2FC20
#define DEVICE_SECRET_ADDR	  0x2FC40
#define DEVICE_NAME_HEADER    "SJY"                       //新产品使用其他的设备名称，需要修改此处

/***********************************************************************************************
 *                                     发布主题添加区                                           *
************************************************************************************************/
#define OTA_PUB_TOPIC                     "$ota/report/%s/%s"
#define THING_PUB_TOPIC                   "$thing/up/property/%s/%s"
#define SYS_PUB_TOPIC                  	  "$sys/operation/%s/%s"    
#define SHADOW_PUB_TOPIC                  "$shadow/operation/%s/%s"            

//订阅主题在二维数组中的位置
typedef enum
{
    SUB_OTA_NUM = 0,
    SUB_THING_NUM,
    SUB_SYS_NUM,
	SUB_SHADOW_NUM,
    SUB_TOPIC_COUNT             //当前订阅主题的数量
}SUB_TOPIC_MAP_t;

/***********************************************************************************************
 *                                 MQTT连接报文参数更改区                                       *
************************************************************************************************/
#define KEEP_ALIVE			1000				//保活计时器，定义了服务器收到客户端消息的最大时间间隔		
#define CLEAN_SESSION        1				    //1：创建一个短期会话，客户端断开连接时会话销毁； 0：创建长期会话，客户端断开连接，仍会保持会话


#define PRODUCTKEY_LEN              32                     //产品密钥长度 ProductKey     字符串长度        
#define DEVICENAME_LEN              32                     //设备名字长度 DeviceName     字符串长度        
#define DEVICESECRET_LEN            64                     //设备密钥长度 DeviceSecret   字符串长度   
#define UPLOAD_TEMPLATE_LEN         2048
#define DATA_PACKET_LEN             (UPLOAD_TEMPLATE_LEN + 100)


/***********************************************************************************************
 *                         云平台服务器的域名或IP 和 端口参数                                    *
************************************************************************************************/
typedef struct _SERVERPARA_{
/********  连接云平台服务器参数  ********/
	char   DomainName[64];					   //域名
	u8   ServerIP[4];                        //存放云平台的服务器IP或是域名
	u16  ServerPort;                           //存放服务器的端口号	
}SERVER_INFOR;


/***********************************************************************************************
 *                            云平台生成MQTT三元组所需要的参数                                   *
************************************************************************************************/
typedef struct _CLOUDPARA_{
	char  ProductKey[PRODUCTKEY_LEN];                                //保存ProductKey的缓冲区
    char  DeviceName[DEVICENAME_LEN];                                //保存DeviceName的缓冲区
    char  DeviceSecret[DEVICESECRET_LEN];                            //保存DeviceSecret的缓冲区
	
}CLOUD_INFOR;



/***********************************************************************************************
 *                               MQTT连接云平台所需元素的数据集合                                *
************************************************************************************************/
typedef struct _MQTTPARA_{
		
/**********  连接MQTT三元组  **********/
	u8 ClientID[64];                         //存放MQTT客户端ID的缓冲区
    char Username[64];                       //存放MQTT用户名的缓冲区
    u8 Passward[80];                         //存放MQTT密码的缓冲区	
}MQTT_INFOR;



#pragma pack(1)
typedef struct _MQTT_PACKET_{
/*********  发布和订阅的主题  *********/	
	char PubTopic[64];
	char SubTopic[SUB_TOPIC_COUNT][64];
/*********  MQTT报文的可变参数  *********/	
	u8 Qos;						//发布消息的服务质量等级, 可选 0 or 1
	u8 CleanSession;			//1：创建一个短期会话，客户端断开连接时会话销毁； 0：创建长期会话，客户端断开连接，仍会保持会话
	u16 KeepAlive;				//保活计时器，定义了服务器收到客户端消息的最大时间间隔
	u16 DataPacketLen;
	char UploadTemplate[UPLOAD_TEMPLATE_LEN];	//组成报文的有效载荷区
	u8 DataPacket[DATA_PACKET_LEN];				//待发布的数据报文缓存区
}MQTT_PACKET_INFOR;
#pragma pack()

/********* 连接云平台服务器（域名、IP、端口号）参数初始化 *********/
void ServerParameterInit(SERVER_INFOR *ServerInfor);

/********* 连接云平台三要素（产品ID、设备名、设备密钥）参数初始化 *********/
void CloudParameterInit(CLOUD_INFOR *CloudInfor);

/********* 连接云平台三元组（客户端ID、用户名、用户密码）参数初始化 *********/
void MqttParameterInit(MQTT_INFOR *MqttInfor);

/********* 初始化连接云平台时，需要使用的结构体（数据格式缓存区、订阅主题等）*********/
void MqttPackerParameterInit(MQTT_PACKET_INFOR *MqttPacket);

/********* 发布主题参数初始化 *********/
void MqttPubTopicInit(MQTT_PACKET_INFOR *MqttPacket, const char *PubTopic);

/********* 初始化设备三元组信息 *********/
void LoadDeviceCloudParameter(void);

/********* 获取当前三元组设备名称 *********/
char* GetDeviceName(void);

#endif

