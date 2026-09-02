#ifndef __TCP_CLIENT_H
#define __TCP_CLIENT_H

#include "fxx_std.h"

/* W5500连接云服务器的状态 */
enum CONNECT_SEVERIP_STATE
{
	CONNECT_SUCCESS = 0, NETWORK_CABLE_LINK_ERROR = 1, GET_LOCALIP_ERROR = 2, SET_LOCALIP_ERROR, 
	GET_SERVERIP_ERROR, SOCKET_CONNECT_ERROR, SOCK_OPEN_ERROR,NETWORK_CONNECT_ERROR,
};


u8 GetDnsServerIP(u8* DnsServerIP);
u8 EthernetConnectServer(const char *DomainName, u16 ServerPort);
u8 ConnectServerAgain();
#endif 
