#include "w5500_user_conf.h"
#include "bsp_spi.h"
#include "dhcp.h"

#define ChipSnAddress  0x00080208        //芯片的SN号在Flash的地址

static wiz_NetInfo WizNetInfo;			 //存放了配置W5500网络所需要的参数
/***********************************************************************************************
 *                                     静态函数声明区                                           *
************************************************************************************************/
static void W5500_GpioConfig(void);
static void ChipIDToMac(uint8_t *MacBuff);
//static void W5500Reset(void);
static void CrisEnter(void);
static void CrisExit(void);
static void CsSelect(void);
static void CsDeselect(void);
static void SPI_WriteByte(u8 Byte);
static u8 SPI_ReadByte(void);
void DhcpSuccessAssign(void);
void DhcpErrorAssign(void);

#ifdef UART_DEBUG_ENABLE
void PrintfHex(u8 *RxBuff, u16 Len)
{
	u16 i;
	for (i = 1; i <= Len; i++)
	{
		printfS("%02X ", RxBuff[i - 1]);
		if (i % 16 == 0)
		{
			printfS("\r\n");
		}		
	}
    printfS("\r\n");
}

void PrintfString(const char *Buff, u16 Len)
{
	u16 i;
	for (i = 0; i < Len; i++)
	{
		printfS("%c", Buff[i]);
	}
	printfS("\r\n");
}

void PrintfDecs(u8 *Buff, u16 Len)
{
	u16 i;
	for (i = 0; i < Len; i++)
	{
		printfS("%d.", Buff[i]);
	}
	printfS("\r\n");
}


void PrintfNetInfoPara(wiz_NetInfo *WizNetInfo)
{
//	printfS("打印W5500网络配置信息\r\n");
//	printfS("Mac: ");
//	PrintfDecs(WizNetInfo->mac, sizeof(WizNetInfo->mac));
//	printfS("IP: ");
//	PrintfDecs(WizNetInfo->ip, sizeof(WizNetInfo->ip));
//	printfS("Subnet Mask: ");
//	PrintfDecs(WizNetInfo->sn, sizeof(WizNetInfo->sn));
//	printfS("Gateway IP Address: ");
//	PrintfDecs(WizNetInfo->gw, sizeof(WizNetInfo->gw));
//	printfS("Dns IP: ");
//	PrintfDecs(WizNetInfo->dns, sizeof(WizNetInfo->dns));
}
#endif
/*******************************************************
 * @brief    初始化W5500的IO 
 * @param    无
 * @return   无
********************************************************/
void W5500Config(void)
{
    W5500_GpioConfig();                                            //初始化SPI2接口
    reg_wizchip_cris_cbfunc(CrisEnter, CrisExit);                  //注册临界区函数
    reg_wizchip_cs_cbfunc(CsSelect, CsDeselect);                  //注册SPI片选信号函数
    reg_wizchip_spi_cbfunc(SPI_ReadByte, SPI_WriteByte);           //注册读写函数
    ChipIDToMac(WizNetInfo.mac);                                   //根据芯片ID号合成MAC地址
//    DNS_flag = 0;                                                  //DNS解析标志=
//    W5500Reset();                                                    //硬件复位
}

/*******************************************************
 * @brief    配置W5500 Socket的接收缓存区为 2K
 * @param    无
 * @return   SUCCESS： 配置成功；   ERROR：配置失败
********************************************************/
u8 W5500_SocketBuffConfig(void)
{
	//W5500收发内存分区，收发缓冲区各自总的空间是16K，（0-7）每个端口的收发缓冲区我们分配 2K
    u8 MemSize[2][8] = {{2, 2, 2, 2, 2, 2, 2, 2}, {2, 4, 2, 2, 2, 2, 1, 1}};			//[0]为发送缓存区		[1]为接收缓存区
	if(ctlwizchip(CW_INIT_WIZCHIP, (void*)MemSize) == -1)
	{
		return ERROR;
	}
	return SUCCESS;
}

void DhcpIint(void)
{
	static u8 DHCP_Buff[512];                 //DHCPBuff 
	DHCP_init(SOCK_DHCP, DHCP_Buff);
	reg_dhcp_cbfunc(DhcpSuccessAssign, DhcpSuccessAssign, DhcpErrorAssign);       //注册DHCP回调函数
}

/*-------------------------------------------------*/
/*函数名：W5500网线连接检测                        */
/*参  数：无                                       */
/*返回值：PHY_LINK_OFF  PHY_LINK_ON                */
/*-------------------------------------------------*/
u8 W5500PhyLinkCheck(void)
{
    u8 tmp;

    if (ctlwizchip(CW_GET_PHYLINK, (void*)&tmp) == -1)             //如果if成立，表示未知错误
    {
        return PHY_LINK_OFF;
    }

    return tmp;
}

/*******************************************************
 * @brief    配置W5500的网络参数
 * @param    xxx:xxx
 * @return   
********************************************************/
u8 NetworkConfig(wiz_NetInfo *NetworkInfo)
{
    u8 tmpstr[6] = {0};
    

    ctlnetwork(CN_SET_NETINFO, (void*)&WizNetInfo);     //设置网络参数
    ctlnetwork(CN_GET_NETINFO, (void*)NetworkInfo);     //读取网络参数
	
	//    ctlwizchip(CW_GET_ID, (void*)tmpstr);         //读取芯片ID
	if(memcmp((u8 *)&WizNetInfo, (u8 *)NetworkInfo, sizeof(NetworkInfo)) != 0)
	{
		return ERROR;
	}
	PrintfNetInfoPara(NetworkInfo);	  //打印
	return SUCCESS;

}



/*******************************************************
 * @brief    DHCP获取任务成功时的指定任务函数
 * @param    xxx:xxx
 * @return   
********************************************************/
void DhcpSuccessAssign(void)
{
    getIPfromDHCP(WizNetInfo.ip);     //把获取到的ip参数，记录到机构体中
    getGWfromDHCP(WizNetInfo.gw);     //把获取到的网关参数，记录到机构体中
    getSNfromDHCP(WizNetInfo.sn);     //把获取到的子网掩码参数，记录到机构体中
    getDNSfromDHCP(WizNetInfo.dns);   //把获取到的DNS服务器参数，记录到机构体中
    WizNetInfo.dhcp = NETINFO_DHCP;   //标记使用的是DHCP方式	

}

/*******************************************************
 * @brief    DHCP获取任务失败时的指定任务函数
 * @param    xxx:xxx
 * @return   
********************************************************/
void DhcpErrorAssign(void)
{
	printfS("Get Dhcp ERROR\r\n");
}


/*-------------------------------------------------*/
/*函数名：根据唯一芯片ID得到大概与唯一的MAC地址    */
/*参  数：MacBuff：存储返回的MAC地址               */
/*返回值：无                                       */
/*-------------------------------------------------*/
void ChipIDToMac(uint8_t *MacBuff)
{
    u32 McuId[3]; //存储96bit唯一芯片ID

    //使用ACM32的SN号作为W5500的MAC地址
    McuId[0] = *(u32*)(ChipSnAddress);
    McuId[1] = *(u32*)(ChipSnAddress + 4);
    McuId[2] = *(u32*)(ChipSnAddress + 8);

    //芯片ID太长,每隔两位相加,组合成一个字节，12Byte->6Byte
    MacBuff[0] = (McuId[0] & 0x000000FF) + ((McuId[0] & 0x0000FF00) >> 8);
    MacBuff[1] = ((McuId[0] & 0x00FF0000) >> 16) + ((McuId[0] & 0xFF000000) >> 24);
    MacBuff[2] = (McuId[1] & 0x000000FF) + ((McuId[1] & 0x0000FF00) >> 8);
    MacBuff[3] = ((McuId[1] & 0x00FF0000) >> 16) + ((McuId[1] & 0xFF000000) >> 24);
    MacBuff[4] = (McuId[2] & 0x000000FF) + ((McuId[2] & 0x0000FF00) >> 8);
    MacBuff[5] = ((McuId[2] & 0x00FF0000) >> 16) + ((McuId[2] & 0xFF000000) >> 24);

	
	//芯片ID太长,每隔两位相加,组合成一个字节，12Byte->6Byte
    MacBuff[0] = (McuId[0] & 0x000000FF) + ((McuId[0] & 0x0000FF00) >> 8);
    MacBuff[1] = ((McuId[0] & 0x00FF0000) >> 16) + ((McuId[0] & 0xFF000000) >> 24);
    MacBuff[2] = (McuId[1] & 0x000000FF) + ((McuId[1] & 0x0000FF00) >> 8);
    MacBuff[3] = ((McuId[1] & 0x00FF0000) >> 16) + ((McuId[1] & 0xFF000000) >> 24);
    MacBuff[4] = (McuId[2] & 0x000000FF) + ((McuId[2] & 0x0000FF00) >> 8);
    MacBuff[5] = ((McuId[2] & 0x00FF0000) >> 16) + ((McuId[2] & 0xFF000000) >> 24);
    //MAC地址开头必须是偶数
    if ((MacBuff[0] % 2) != 0)
    {
        MacBuff[0] = MacBuff[0] + 1;
    }
}



/*-------------------------------------------------*/
/*函数名：进入临界区                               */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void CrisEnter(void)
{
    __set_PRIMASK(1);                                           //禁止全局中断
}
/*-------------------------------------------------*/
/*函数名：退出临界区                               */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void CrisExit(void)
{
    __set_PRIMASK(0);                                          //开全局中断
}
/*-------------------------------------------------*/
/*函数名：片选信号输出低电平                       */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void CsSelect(void)
{
    GPIO_ResetBits(W5500_SPIx_CS_PORT, W5500_SPIx_CS);     //拉低CS片选
}

/*-------------------------------------------------*/
/*函数名：片选信号输出高电平                       */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void CsDeselect(void)
{
    GPIO_SetBits(W5500_SPIx_CS_PORT, W5500_SPIx_CS);      //拉高CS片选
}


/*******************************************************
 * @brief    W5500复位函数
 * @param    xxx:xxx
 * @return   
********************************************************/
void W5500Reset(void)
{
    GPIO_ResetBits(W5500_SPIx_RESET_PORT, W5500_RESET);     //拉低硬件复位引脚
    System_Delay_MS(1);                                             //延时，至少500us以上低电平
    GPIO_SetBits(W5500_SPIx_RESET_PORT, W5500_RESET);       //拉高硬件复位引脚
    System_Delay_MS(1800);                                           //延时
}

/*******************************************************
 * @brief    配置W5500的GPIO接口
 * @param    无
 * @return   无
********************************************************/
void W5500_GpioConfig(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
//    System_Module_Enable(EN_GPIOCD);
//    System_Module_Enable(EN_GPIOAB);

    SPI_Configuration(W5500_SPIx);
	CsDeselect();			//释放片选脚
    SPIx_SetSpeed(W5500_SPIx, SPI_BAUDRATE_PRESCALER_254);//设置到低速模式
    /*定义RESET引脚*/
    GPIO_InitStructure.Pin  = W5500_RESET;                          /*选择要控制的GPIO引脚*/
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;            	    /*设置引脚模式为通用推挽输出*/
    GPIO_InitStructure.Pull = GPIO_PULLUP;           				/*设置引脚模式为Upput*/
    GPIO_InitStructure.Alternate =  GPIO_FUNCTION_0;
    GPIO_Init(W5500_SPIx_RESET_PORT, &GPIO_InitStructure);          /*调用库函数，初始化GPIO*/
    GPIO_SetBits(W5500_SPIx_RESET_PORT, W5500_RESET);
	
	/*定义CS引脚*/
//    GPIO_InitStructure.Pin = W5500_SPIx_CS;                           
//    GPIO_InitStructure.Alternate = GPIO_FUNCTION_0;
//    GPIO_InitStructure.Pull = GPIO_PULLUP;           
//    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
//    GPIO_Init(W5500_SPIx_CS_PORT, &GPIO_InitStructure);             /*调用库函数，初始化GPIO*/
	
    /*定义INT引脚*/
    GPIO_InitStructure.Pin = W5500_INT;                           
    GPIO_InitStructure.Alternate = GPIO_FUNCTION_0;
    GPIO_InitStructure.Pull = GPIO_PULLUP;           
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(W5500_SPIx_INT_PORT, &GPIO_InitStructure);             /*调用库函数，初始化GPIO*/
    
    
}


/*******************************************************
 * @brief    写一个字节
 * @param    xxx:xxx
 * @return   无
********************************************************/
void SPI_WriteByte(u8 Byte)
{
    SPI_Transmit(W5500_SPIx, &Byte, 1, 0);
}


/*******************************************************
 * @brief    读一个字节
 * @param    xxx:xxx
 * @return   
********************************************************/
u8 SPI_ReadByte(void)
{
	u8 Byte = 0;
    SPI_Receive(W5500_SPIx, &Byte, 1, 0);
	return Byte;
}


/*******************************************************
 * @brief    W5500发送数据, 用于SSL自定义的发送函数
 * @param    TxData: 发送的数据缓存区
 *           DataLen: 数据的发送长度
 * @return   发送的长度
********************************************************/
int32 W5500_TxData(u8 *TxData, u32 DataLen)
{
    return send(SOCK_TCPS, TxData, DataLen);
}

/*-------------------------------------------------*/
/*函数名：W5500接收数据                            */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
u16 W5500_RxData(u8 *RxData, u32 DataLen)
{
    u16 W5500_RxLen = 0;

//    W5500_RxLen = getSn_RX_RSR(SOCK_TCPS);
//    //有接收到数据
//    if ((W5500_RxLen > 0))     //&& (rbCanWrite(&rb_t_ssl) >= W5500_RxCnt) && (SysCB.SysEventFlag & CONNECT_EVENT)
//    {
    W5500_RxLen = (u16)recv(SOCK_TCPS, (u8 *)RxData, DataLen); //接收来自W5500的数据
//    }
	return W5500_RxLen;
}


