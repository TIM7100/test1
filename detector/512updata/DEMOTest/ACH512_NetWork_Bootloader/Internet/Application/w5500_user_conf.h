#ifndef _USER_CONF_H
#define _USER_CONF_H

#include "common.h"
#include "socket.h"

#define SPI_W5500								SPIB

#define W5500   0
#define ESP8266 1

#define SOCK_DHCP             0                  //DHCP功能用的端口，W5500有8个，0-7都行
#define SOCK_TCPS             1                  //TCP连接MQTT用的端口， W5500有8个，0-7都行
#define SOCKET_DNS            2                  //DNS功能用的端口， W5500有8个，0-7都行
#define SOCK_HTTP             3

/***********************************************************************************************
 *                                  W5500硬件引脚更改处                                         *
************************************************************************************************/

void W5500Config(void);
void W5500Reset(void);
u8 W5500_SocketBuffConfig(void);
void DhcpInit(void);
u8 NetworkConfig(wiz_NetInfo *NetworkInfo);
u8 W5500PhyLinkCheck(void);
void ChipIDToMac(uint8_t *MacBuff);
/* SSL底层使用的发送接收函数 */
int32 W5500_TxData(u8 *TxData, u32 DataLen);
u16 W5500_RxData(u8 *RxData, u32 DataLen);

/* 调试时使用的打印函数 */
#ifdef UART_DEBUG_ENABLE
	void PrintfHex(u8 *RxBuff, u16 Len);
	void PrintfString(const char *Buff, u16 Len);
	void PrintfDecs(u8 *Buff, u16 Len);
	void PrintfNetInfoPara(wiz_NetInfo *WizNetInfo);
#else
	#define PrintfHex(x, y)    ((void)0)
	#define PrintfString(x, y)    ((void)0)
	#define PrintfDecs(x, y)    ((void)0)
	#define PrintfNetInfoPara(x)    ((void)0)
#endif
	
	
#endif
