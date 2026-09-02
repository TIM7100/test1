#include "esp8266_config.h"


#include  "uart.h"




WifiInfo_t Wifi_Info = 
{
	"XXY",
	"goodeveryday",

	SmartConfig_Uncompleted
};

void Get_WifiInfo(void)
{
// 	EflashReadByte(WIFI_SSID_Addr, 36, (uint32_t *)Wifi_Info.SSID);
// 	EflashReadByte(WIFI_Password_Addr, 36, (uint32_t *)Wifi_Info.Password);
}

/*-------------------------------------------------*/
/*º¯ÊýÃû£º³õÊ¼»¯WiFiµÄ¸´Î»IO                       */
/*²Î  Êý£ºÎÞ                                       */
/*·µ»ØÖµ£ºÎÞ                                       */
/*-------------------------------------------------*/
void WiFi_IO_Init(void)
{
    REG_SCU_MUXCTRLB &= ~(0x300000);
    REG_GPIO_DIR(GPIOA) |= 0x4000000;
	
	Get_WifiInfo();
}

/*-------------------------------------------------*/
/*º¯ÊýÃû£ºWiFi·¢ËÍÉèÖÃÖ¸Áî                         */
/*²Î  Êý£ºcmd£ºÖ¸Áî                                */
/*²Î  Êý£ºtimeout£º³¬Ê±Ê±¼ä£¨100msµÄ±¶Êý£©         */
/*·µ»ØÖµ£º0£ºÕýÈ·   ÆäËû£º´íÎó                     */
/*-------------------------------------------------*/
static uint8_t WiFi_SendCmd(uint8_t *cmd, int16_t timeout)
{
    uint8_t   Rx_Buffer[2048];
    uint16_t  Rx_Buffer_Index = 0;
    int rbCanReadLen;

    memset(Rx_Buffer, 0, 2048);
    WiFi_Send(cmd);                         //·¢ËÍÖ¸Áî
#ifdef DEBUG_ESP
    printf("%s\r\n", cmd);
#endif
    while (timeout--)                                   //µÈ´ý³¬Ê±Ê±¼äµ½0
    {
        Timer0DelayMs(10);                                   //ÑÓÊ±10ms

        //»ñÈ¡ÍøÂçÐ¾Æ¬·µ»ØµÄÊý¾Ý¸öÊý
        rbCanReadLen = rbCanRead(&rb_t_ssl);
        //ÓÐ»º´æµÄÊý¾Ý
        if (rbCanReadLen > 0)
        {
            //Ö»·µ»Øµ±Ç°ÓÐµÄ
            rbRead(&rb_t_ssl, &Rx_Buffer[Rx_Buffer_Index], rbCanReadLen);
            Rx_Buffer_Index += rbCanReadLen;

            if (strstr((char *)Rx_Buffer, "OK"))          //Èç¹û½ÓÊÕµ½OK±íÊ¾Ö¸Áî³É¹¦
            {

                break;                                      //Ö÷¶¯Ìø³öwhileÑ­»·
            }
        }

    }
    if (timeout <= 0)
    {
        return 1;                                       //Èç¹ûtimeout<=0£¬ËµÃ÷³¬Ê±Ê±¼äµ½ÁË£¬Ò²Ã»ÄÜÊÕµ½OK£¬·µ»Ø1
    }
    else
    {
        return 0;                                       //·´Ö®£¬±íÊ¾ÕýÈ·£¬ËµÃ÷ÊÕµ½OK£¬Í¨¹ýbreakÖ÷¶¯Ìø³öwhile
    }
}

/*-------------------------------------------------*/
/*º¯ÊýÃû£ºWiFi¼ÓÈëÂ·ÓÉÆ÷Ö¸Áî                       */
/*²Î  Êý£ºtimeout£º³¬Ê±Ê±¼ä£¨1sµÄ±¶Êý£©            */
/*·µ»ØÖµ£º0£ºÕýÈ·   ÆäËû£º´íÎó                     */
/*-------------------------------------------------*/
static uint8_t WiFi_JoinAP(int16_t timeout)
{
    char Tx_Buffer[200];
	
	if((Wifi_Info.SSID[0] == 0xFF) || (Wifi_Info.Password[0] == 0xFF))
	{
		return 1;
	}
	
	Wifi_Info.SSID[33] = '\0';
	Wifi_Info.Password[33] = '\0';
	
sprintf(Tx_Buffer, "AT+CWJAP=\"%s\",\"%s\"\r\n", Wifi_Info.SSID, Wifi_Info.Password);
	if (WiFi_SendCmd((uint8_t*)Tx_Buffer, timeout))       
	{
		return 2;
	}
	
    return 0;                                                     //ÕýÈ·£¬·µ»Ø0
}



/*-------------------------------------------------*/
/*º¯ÊýÃû£ºÒ»¼üÖÇÄÜÅäÍø                       	   */
/*²Î  Êý£ºNULL            						   */
/*·µ»ØÖµ£º0£ºÕýÈ·   ÆäËû£º´íÎó                      */
/*-------------------------------------------------*/
 uint8_t SmartConifg(void)
{
    uint16_t timeout = 3000 * 6;
    uint8_t   Rx_Buffer[512];
    uint16_t  Rx_Buffer_Index = 0;
    int rbCanReadLen;
	
    //ÅäÍø
    if (WiFi_SendCmd((uint8_t*)"AT\r\n", 50))                       //²âÊÔAT
    {
        return 1;
    }

    if (WiFi_SendCmd((uint8_t*)"ATE0\r\n", 50))                     //¹Ø±Õ»ØÏÔ
    {
        return 2;
    }

    if (WiFi_SendCmd((uint8_t*)"AT+CWMODE=1\r\n", 10))          //WIFIÄ£¿éÉèÖÃÎªSTAÄ£Ê½
    {
        return 3;
    }

    if (WiFi_SendCmd((uint8_t*)"AT+CWAUTOCONN=0\r\n", 50))           //È¡Ïû×Ô¶¯Á¬½Ó£¬10ms³¬Ê±µ¥Î»£¬×Ü¼Æ500ms³¬Ê±Ê±¼ä
    {
        return 4;                                      //·µ»Ø2£¬È¡Ïû×Ô¶¯Á¬½ÓÊ§°Ü
    }

// 	WIFI_ConnectShow();
	if (WiFi_JoinAP(1500) == 0)        
	{
// 		WIFI_ConnectSuccesShow();
		return 0;
	}
	
// 	SmartConfig_RequestShow();
// 	
	if (WiFi_SendCmd((uint8_t*)"AT+CWSTARTSMART=2", 50))
	{
		return 6;
	}
#ifdef DEBUG_ESP
	printf("Start SmartConfig:\r\n");
#endif
	memset(Rx_Buffer, 0, 512);
	//µÈ´ýÅäÍø£¬3·ÖÖÓ³¬Ê±ÍË³ö
	while (timeout)
	{
		timeout--;
		Timer0DelayMs(10);
		//»ñÈ¡ÍøÂçÐ¾Æ¬·µ»ØµÄÊý¾Ý¸öÊý
		rbCanReadLen = rbCanRead(&rb_t_ssl);
		//ÓÐ»º´æµÄÊý¾Ý
		if (rbCanReadLen > 0)
		{
			//Ö»·µ»Øµ±Ç°ÓÐµÄ
			rbRead(&rb_t_ssl, &Rx_Buffer[Rx_Buffer_Index], rbCanReadLen);
			Rx_Buffer_Index += rbCanReadLen;
#ifdef DEBUG_ESP
			//´òÓ¡ÐÅÏ¢
			printf("%s\r\n", Rx_Buffer);
#endif
			if (strstr((char *)Rx_Buffer, "Smart get wifi info") == NULL) //»ñÈ¡µ½wfiÐÅÏ¢
			{
				continue;
			}

			//ÅÐ¶ÏÁ¬½Ówifi
			if (strstr((const char*)Rx_Buffer, "connected wifi")) //³É¹¦Á¬½Óµ½wifi
			{
				if (WiFi_SendCmd((uint8_t*)"AT+CWSTOPSMART", 50)) //Í£Ö¹SmartConfig
				{
					return 5;
				}
#ifdef DEBUG_ESP
				printf("stop SmartConfig\r\n");
#endif
				if(Parse_SSID_Password(Rx_Buffer, &Wifi_Info))
				{
					return 6;
				}
				break;
			}
		}
	}
	if (timeout <= 0)
	{
// 		SmartConfig_ErrorShow();
		return 6;
	}
//     SmartConfig_SuccessShow();
    return 0;
}

/*-------------------------------------------------*/
/*º¯ÊýÃû£ºÁ¬½ÓTCP·þÎñÆ÷£¬²¢½øÈëÍ¸´«Ä£Ê½            */
/*²Î  Êý£ºWiFiServerIP£º ·þÎñÆ÷IP                  */
/*²Î  Êý£ºWiFiServerPort£º·þÎñÆ÷¶Ë¿ÚºÅ             */
/*²Î  Êý£ºtimeout£º ³¬Ê±Ê±¼ä£¨100msµÄ±¶Êý£©        */
/*·µ»ØÖµ£º0£ºÕýÈ·  ÆäËû£º´íÎó                      */
/*-------------------------------------------------*/
uint8_t WiFi_Connect_Server(char *WiFiServerIP, uint16_t WiFiServerPort, int16_t timeout)
{
	char cmd[64];
    int rbCanReadLen;
    uint8_t   Rx_Buffer[512];
    uint16_t  Rx_Buffer_Index = 0;

    memset(Rx_Buffer, 0, 512);

sprintf(cmd, "AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", WiFiServerIP, WiFiServerPort);//·¢ËÍÁ¬½Ó·þÎñÆ÷Ö¸Áî?
WiFi_Send(cmd);
//     WiFi_printf("AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", WiFiServerIP, WiFiServerPort); //·¢ËÍÁ¬½Ó·þÎñÆ÷Ö¸Áî¡

#ifdef DEBUG_ESP
    printf("AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", WiFiServerIP, WiFiServerPort);
#endif
    while (timeout--)                                           //µÈ´ý³¬Ê±Óë·ñ
    {
        Timer0DelayMs(100);

        //»ñÈ¡ÍøÂçÐ¾Æ¬·µ»ØµÄÊý¾Ý¸öÊý
        rbCanReadLen = rbCanRead(&rb_t_ssl);
        //ÓÐ»º´æµÄÊý¾Ý
        if (rbCanReadLen > 0)
        {
            //Ö»·µ»Øµ±Ç°ÓÐµÄ
            rbRead(&rb_t_ssl, &Rx_Buffer[Rx_Buffer_Index], rbCanReadLen);
            Rx_Buffer_Index += rbCanReadLen;
#ifdef DEBUG_ESP
			//´òÓ¡ÐÅÏ¢
            printf("%s\r\n", Rx_Buffer);
#endif
            if (strstr((char *)Rx_Buffer, "CONNECT"))             //Èç¹û½ÓÊÜµ½CONNECT±íÊ¾Á¬½Ó³É¹¦
            {
                break;                                              //Ìø³öwhileÑ­»·
            }
            if (strstr((char *)Rx_Buffer, "CLOSED"))              //Èç¹û½ÓÊÜµ½CLOSED±íÊ¾·þÎñÆ÷Î´¿ªÆô
            {
                return 1;                                           //·þÎñÆ÷Î´¿ªÆô·µ»Ø1
            }
            if (strstr((char *)Rx_Buffer, "ALREADY CONNECTED"))   //Èç¹û½ÓÊÜµ½ALREADY CONNECTEDÒÑ¾­½¨Á¢Á¬½Ó
            {
                return 2;                                           //ÒÑ¾­½¨Á¢Á¬½Ó·µ»Ø2
            }
        }

    }
    if (timeout <= 0)
    {
        return 3;                                               //³¬Ê±´íÎó£¬·µ»Ø3
    }
    else                                                        //Á¬½Ó³É¹¦£¬×¼±¸½øÈëÍ¸´«
    {
        if (WiFi_SendCmd((uint8_t*)"AT+CIPSEND\r\n", 5))
        {
            return 4;
        }
    }
    return 0;                                                   //³É¹¦·µ»Ø0
}


/*-------------------------------------------------*/
/*º¯ÊýÃû£ºWiFiÍË³öÍ¸´«Ä£Ê½                         */
/*²Î  Êý£ºÎÞ                                       */
/*·µ»ØÖµ£º0£ºÕýÈ·  ÆäËû£º´íÎó                      */
/*-------------------------------------------------*/
static uint8_t WiFi_QuitTrans(void)
{
    int16_t timeout = 5;

    while (timeout--)                                             //µÈ´ý³¬Ê±Ê±¼äµ½0
    {
        WiFi_Send("+++");
        Timer0DelayMs(1000);                                           //µÈ´ý500msÌ«ÉÙ Òª1000ms²Å¿ÉÒÔÍË³ö
        if (WiFi_SendCmd((uint8_t*)"AT", 50) == 0)                          //ÍË³öÍ¸´«ÅÐ¶Ï
        {
            break;
        }
    }

    if (timeout <= 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*-------------------------------------------------*/
/*º¯ÊýÃû£ºWiFiÁ¬½Ó·þÎñÆ÷                           */
/*²Î  Êý£ºWiFiIoTServerIP£º ·þÎñÆ÷IP               */
/*²Î  Êý£ºWiFiIoTServerPort£º·þÎñÆ÷¶Ë¿ÚºÅ          */
/*·µ»ØÖµ£º0£ºÕýÈ·   ÆäËû£º´íÎó                     */
/*-------------------------------------------------*/
uint8_t WiFi_Connect_IoTServer(char *WiFiServerIP, uint16_t WiFiServerPort, int16_t timeout)
{
	uint8_t   Rx_Buffer[512];
    uint16_t  Rx_Buffer_Index = 0;
    u8 ret;

    WIFI_RESET_RST_IO;                                //¸´Î»IOÀ­µÍµçÆ½
    Timer0DelayMs(50);                              //ÑÓÊ±£¬ÖÁÉÙ200usÒÔÉÏµÍµçÆ½
    WIFI_SET_RST_IO;                                  //¸´Î»IOÀ­¸ßµçÆ½
    Timer0DelayMs(1500);                             //ÑÓÊ±
    rbCreate(&rb_t_ssl, rb_t_ssl_buff, rb_t_ssl_buff_len);

	if(Wifi_Info.SmartConfig_Flag == SmartConfig_Uncompleted)
	{
		if(SmartConifg() == 0)
		{
			Wifi_Info.SmartConfig_Flag = SmartConfig_Completed;
			goto wifi_continue;
		}
		else
		{
			return 11;
		}
	}
	
    //ÅäÍø
    if (WiFi_SendCmd((uint8_t*)"AT\r\n", 10))                       //²âÊÔAT
    {
        return 1;
    }

    if (WiFi_SendCmd((uint8_t*)"ATE0\r\n", 50))                     //¹Ø±Õ»ØÏÔ
    {
        return 2;
    }

    if (WiFi_SendCmd((uint8_t*)"AT+CWMODE=1\r\n", 10))          //WIFIÄ£¿éÉèÖÃÎªSTAÄ£Ê½
    {
        return 3;
    }

    if (WiFi_SendCmd((uint8_t*)"AT+CWAUTOCONN=0\r\n", 50))           //È¡Ïû×Ô¶¯Á¬½Ó£¬10ms³¬Ê±µ¥Î»£¬×Ü¼Æ500ms³¬Ê±Ê±¼ä
    {
        return 4;                                      //·µ»Ø2£¬È¡Ïû×Ô¶¯Á¬½ÓÊ§°Ü
    }

	if (WiFi_JoinAP(3000))        
	{
		return 5;
	}
	
wifi_continue:
	if (WiFi_SendCmd((uint8_t*)"AT+CIPSNTPCFG=1,8,\"ntp1.aliyun.com\"\r\n", 500))       
	{
		return 6;
	}

	if (WiFi_SendCmd((uint8_t*)"AT+CIPSNTPTIME?\r\n", 50))       
	{
		return 7;
	}

    if (WiFi_SendCmd((uint8_t*)"AT+CIPMODE=1\r\n", 50))    //ÉèÖÃÍ¸´«£¬10ms³¬Ê±µ¥Î»£¬×Ü¼Æ500ms³¬Ê±Ê±¼ä
    {
        return 8;                                      //·µ»Ø4£¬ÉèÖÃÍ¸´«Ê§°Ü
    }

    if (WiFi_SendCmd((uint8_t*)"AT+CIPMUX=0\r\n", 50))     //¹Ø±Õ¶àÂ·Á¬½Ó£¬10ms³¬Ê±µ¥Î»£¬×Ü¼Æ500ms³¬Ê±Ê±¼ä
    {
        return 9;                                      //·µ»Ø5£¬¹Ø±Õ¶àÂ·Á¬½ÓÊ§°Ü
    }

    ret = WiFi_Connect_Server(WiFiServerIP, WiFiServerPort, timeout);
    if (ret)
    {
        return 10;
    }

    return 0;
}

u8 Parse_SSID_Password(u8* buffer, WifiInfo_t* wifi_info)
{
char *ssid_end_point = NULL;
char *pwd_end_point = NULL;
	 char *ssid_start_point = strstr((char*)buffer,  "ssid:");
     char *pwd_start_point = strstr((char*)buffer, "password:");
	 u8 len;
	
	 memset(wifi_info, '\0', sizeof(WifiInfo_t));
	
	 if (ssid_start_point == NULL || pwd_start_point == NULL) 
	 {
        return 1; 
     }
	
	 ssid_start_point += 5;
	 pwd_start_point += 9;
	 ssid_end_point = ssid_start_point;
	 pwd_end_point = pwd_start_point;
	
	 while(*ssid_end_point != '\0')
	 {
		 ssid_end_point++;
		 if(*(ssid_end_point) == 0x0D && *(ssid_end_point + 1) == 0x0A)
		 {
			 break;
		 }
	 }
	 len = ssid_end_point - ssid_start_point;
	 if(len > 32)
	 {
		 return 2;
	 }
	 memcpy(wifi_info->SSID, ssid_start_point, len);
	 
	 while(*pwd_end_point != '\0')
	 {
		 pwd_end_point++;
		 if(*(pwd_end_point) == 0x0D && *(pwd_end_point + 1) == 0x0A)
		 {
			 break;
		 }
	 }
	 len = pwd_end_point - pwd_start_point;
	 if(len > 32)
	 {
		 return 3;
	 }
	 memcpy(wifi_info->Password, pwd_start_point, len);
	 
// 	 EflashWriteByte(WIFI_SSID_Addr, 36, (uint8_t *)wifi_info->SSID);
// 	 EflashWriteByte(WIFI_Password_Addr, 36, (uint8_t *)wifi_info->Password);
	 
	 return 0;
}

