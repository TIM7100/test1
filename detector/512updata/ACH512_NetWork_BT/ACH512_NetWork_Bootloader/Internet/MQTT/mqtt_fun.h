#ifndef  __MQTT_FUN_H_
#define  __MQTT_FUN_H_

#include "common.h"
#include "cloud_platform_para.h"
#include "w5500_user_conf.h"


enum MQTT_STATE{
	PACKET_SUCCESS = 0, CONNECT_MQTT_CLIENT_ERR = 1, SUB_TOPIC_ERR, 
	PUB_TOPIC_ERR, KEEP_ALIVE_ERR
};


u8 ConnectMqttClientPacket(MQTT_PACKET_INFOR *MqttPacket, MQTT_INFOR *MqttInfor);
int32 DisconnectMqttClientPacket(void);

u8 SubscribeTopicPacket(MQTT_PACKET_INFOR *MqttPacket);
u8 PublishTopicPacket(MQTT_PACKET_INFOR *MqttPacket);

u8 KeppAlivePacket(void);

u16 WaitReceiveMsg(MQTT_PACKET_INFOR *MqttPacket);
#endif
