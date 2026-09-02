#include "tcp_client.h"
#include "socket.h"
#include "bsp_timer7.h"
#include "dhcp.h"
#include "dns.h"
#include "cloud_platform_para.h"
#include "w5500_user_conf.h"
#include "ssl_direct.h"
#include "stdlib.h"

//const u16 LocalPort = 5003;                                   //w5500网络端口号
static wiz_NetInfo NetworkInfo;                             // DHCP获取的IP、网关和DNS服务器地址
static SERVER_INFOR ServerInfor = {0};
extern u32 Timeus;

void PrintfDnsInfor(SERVER_INFOR *ServerInfor)
{
//  printfS("域名：");
//  PrintfString(ServerInfor->DomainName, sizeof(ServerInfor->DomainName));
//  printfS("端口号：");
//  printfS("%d\r\n", ServerInfor->ServerPort);
//  printfS("IP地址：");
//  PrintfDecs((u8 *)ServerInfor->ServerIP, sizeof(ServerInfor->ServerIP));
}
/*******************************************************
 * @brief
 * @param    xxx:xxx
 * @return
********************************************************/
u8 DHCP_WaitGetLocalIP(void)
{
    u8 TryCount = 4;
    u8 Timeout = 5;                     //超时时间为4s
    u8 DHCP_GetIPStatue = 0;
    u8 Ret = ERROR;


    /* 使用DHCP给网口获取动态IP地址，并配置该IP地址 */
    while (TryCount--)
    {
        DhcpIint();
        while (Timeout--)                            //等待超时时间到0
        {
            printfS("%drun DHCP\r\n", TryCount);
            if (DHCP_run() == DHCP_IP_LEASED)
            {
                TryCount = 0;
                Timeout = 0;
                Ret = SUCCESS;
            }
            System_Delay_MS(500);                           //延时1000ms
        }
        DHCP_stop();
        //close(SOCK_DHCP);
    }

    return Ret;
}

/*******************************************************
 * @brief    获取通过DHCP协议得到的DNS服务器地址
 * @param    DnsServerIP: 用于存放DNS服务器地址的缓存区
 * @return   获取成功: SUCCESS
             获取失败: ERROR
********************************************************/
u8 GetDnsServerIP(u8* DnsServerIP)
{
    u8 IPTest[4] = {0};
    if (memcmp(NetworkInfo.dns, IPTest, 4) == 0)
    {
        memcpy(DnsServerIP, NetworkInfo.dns, 4);
        return SUCCESS;
    }
    return ERROR;
}

/*******************************************************
 * @brief
 * @param    xxx:xxx
 * @return
********************************************************/
u8 DNS_GetServerIP(SERVER_INFOR *ServerInfor, u8 *DNSServerIP)
{
    u8 Ret = SUCCESS;
    u8 gDnsBuf[256];                                       //数据缓冲区
    u16 DNS_TryCount = 50;


    DNS_init(SOCKET_DNS, gDnsBuf);                         //DNS初始化
    printfS("DNS_run\r\n");
    /*开始使用DNS解析域名*/
    if (DNS_run(DNSServerIP, (u8*)ServerInfor->DomainName, (u8*)ServerInfor->ServerIP) != 1)
    {
        printfS("DNS Get ERROR");
        Ret = ERROR;
    }

    close(SOCKET_DNS);
    return Ret;
}


static u8 UserTCPConnectServer(u8 *ServerIP, u16 ServerPort, u16 LocalPort)
{
    u8 Ret;
    u16 Timeout = 3;


    /* 设备断开前，必须要先断开云平台的连接，否则因为MQTTKeepAlive机制，会导致无法重新连接上云平台服务器 */
    while (Timeout != 0)                                    //等待超时时间到0
    {
        switch (getSn_SR(SOCK_TCPS))                        //获取TCP链接端口的状态
        {
        case SOCK_INIT:
            Ret = connect(SOCK_TCPS, ServerIP, ServerPort); //链接服务器
            if (Ret == SOCK_ERROR)                          //如果连接超时，进入if
            {
                return SOCKET_CONNECT_ERROR;
            }
            break;

        case SOCK_ESTABLISHED:                  //处于连接状态
            return CONNECT_SUCCESS;

        case SOCK_CLOSE_WAIT:                   //处于等待关闭状态
            close(SOCK_TCPS);                   //端口关闭

            break;

        case SOCK_CLOSED:
            Ret = socket(SOCK_TCPS, Sn_MR_TCP, LocalPort, Sn_MR_ND);           //打开W5500的端口，用于建立TCP链接，本地TCP端口5050
            if (Ret == SOCK_ERROR)                                             //如果打开失败，进入if
            {
                return SOCK_OPEN_ERROR;
            }
            //  Timer7_Stop();
            break;

        }
        Timeout--;
        System_Delay_MS(2000);
    }
    return SOCK_OPEN_ERROR;
}


/*******************************************************
 * @brief    使用W5500连接云平台服务器, 使用该函数前需要先修改cloud_platform_para.c 文件的参数
 * @param    无
 * @return
********************************************************/
u8 EthernetConnectServer(const char *DomainName, u16 ServerPort)
{
    u8 Ret = 0;
//  u8 ServerIP[4] = {0};
    __IO u16 TcpPort = 5050;

    printfS("开始连接服务器  \r\n");

    /* 在使用DHCP动态获取本地IP地址前，需要W5500配置了本地的MAC地址 */
//  W5500Config();
    W5500Reset();
    W5500_SocketBuffConfig();

    if (W5500PhyLinkCheck() == 0)
    {
        return NETWORK_CABLE_LINK_ERROR;
    }

    /* 等待DHCP动态获取IP成功 */
    if ((NetworkInfo.ip[0] == 0) && (NetworkInfo.ip[1] == 0) && (NetworkInfo.ip[2] == 0) && (NetworkInfo.ip[3] == 0))
    {
        if (DHCP_WaitGetLocalIP() == ERROR)
        {
            printfS("DHCP Get IP ERROR\r\n");
            return GET_LOCALIP_ERROR;
        }
    }
    /* 将通过DHCP获取的IP地址，设置为W5500的IP地址 */
    if (NetworkConfig(&NetworkInfo) == ERROR)
    {
        return SET_LOCALIP_ERROR;
    }


    /* 使用DNS解析服务器的域名，获得对应的ServerIP */
    if (DomainName == NULL || ServerPort == 0)
    {
        ServerParameterInit(&ServerInfor);                     //获取服务器的域名和端口号
    }
    else
    {
        memset(ServerInfor.DomainName, 0, strlen(DomainName));
        memcpy(ServerInfor.DomainName, DomainName, strlen(DomainName));
        memset(ServerInfor.ServerIP, 0, sizeof(ServerInfor.ServerIP));
        ServerInfor.ServerPort = ServerPort;
    }
    if (DNS_GetServerIP(&ServerInfor, NetworkInfo.dns) == ERROR)
    {
        printfS("DNS Get ServerIP ERROR\r\n");
        return GET_SERVERIP_ERROR;
    }
    PrintfDnsInfor(&ServerInfor);                           //打印连接网络的参数
    printfS("4\r\n");

    /* 连接服务器 */
    srand(Timeus);
    TcpPort =  rand() % 1000 + 5000; //产生5000~5999的随机数;
    Ret = UserTCPConnectServer((u8 *)ServerInfor.ServerIP, ServerInfor.ServerPort, TcpPort);
    if (Ret != CONNECT_SUCCESS)
    {
        printfS("服务器 ERROR: %d\r\n", Ret);
        return SOCKET_CONNECT_ERROR;
    }
    return CONNECT_SUCCESS;
}


u8 ConnectServerAgain()
{
    u8 Ret = 0;
    uint16_t TcpPort = 5050;

    TcpPort =  rand() % 1000 + 5000;                                  //产生5000~5999的随机数;
    disconnect(SOCK_TCPS);                                            //连接服务器前，要确保断开连接其他的服务器
    System_Delay_MS(20);
    close(SOCK_TCPS);

    if (W5500PhyLinkCheck() == 0)
    {
        return NETWORK_CABLE_LINK_ERROR;
    }

    SSL_Init();
    /* 连接服务器 */
    Ret = UserTCPConnectServer((u8 *)ServerInfor.ServerIP, ServerInfor.ServerPort, TcpPort);
    if (Ret != CONNECT_SUCCESS)
    {
//      printfS("ERROR: %d\r\n", Ret);
        return SOCKET_CONNECT_ERROR;
    }
    //SSL轮询握手
    if (SSL_HandshakeAwait())
    {
//        SysCB.SysEventFlag &= ~CONNECT_EVENT;               //清除网络连接标志
        return SOCKET_CONNECT_ERROR;
    }
    return CONNECT_SUCCESS;
}
