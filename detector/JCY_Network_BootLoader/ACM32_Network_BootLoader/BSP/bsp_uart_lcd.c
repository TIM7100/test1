#include "bsp_uart_lcd.h"
//#include "crc.h"



/***   串口屏外设接口设置   ***/
#define UARTx     2				//1:使用Uart1和串口屏通信  2:使用Uart2和串口屏通信

#if   (UARTx == 1)
	#define LCD_UART     UART1
#elif (UARTx == 2)
	#define LCD_UART     UART2
#endif

/**** 数据结束标志\r\n, 只有都接收到才判断成功接收到数据；不然，需要重新接收 ****/
#define DATAEND_FLAG1      0x0D
#define DATAEND_FLAG2      0x0A


/*** 全局变量，Uart中断中修改的值  ***/
static u8 RxBuf[10]; 
static u8 UartRxFinished = 0;
static u8 RxCount = 0;
static char CmdBuf[64];
static volatile UART_RxState RxState = UART_RX_IDLE;  
static u32 RxTimeout = 0;                          
//static char CmdBuf[64];

void LCD_UartInit(uint32_t Baudrate)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	UART_InitTypeDef UART_InitStruc = {0};
#if (UARTx == 1)
	
	
    //Enable GPIO and UART Clock
    System_Module_Enable(EN_UART1);
//    System_Module_Enable(EN_GPIOAB);
    //Config GPIO
    /* A2:Tx  A3:Rx */
    GPIO_InitStruct.Pin       = GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Alternate = GPIO_FUNCTION_2;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    //UART Config
    UART_InitStruc.UART_BaudRate    = Baudrate;
    UART_InitStruc.UART_WordLength  = UART_WORDLENGTH_8B;
    UART_InitStruc.UART_StopBits    = UART_STOPBITS_1;
    UART_InitStruc.UART_Parity      = UART_PARITY_NONE;
    UART_InitStruc.UART_Mode        = UART_MODE_TX_RX;
    UART_InitStruc.UART_HardwareFlowControl = UART_HWCONTROL_NONE;
    UART_Init(UART1, &UART_InitStruc);
    /* NVIC Config */
    NVIC_ClearPendingIRQ(UART1_IRQn);
    NVIC_SetPriority(UART1_IRQn, 5);
    NVIC_EnableIRQ(UART1_IRQn);

    UART_ITConfig(UART1, UART_IT_RXNE, ENABLE);   //中断初始化

    UART_Cmd(UART1, ENABLE);
    System_Delay_MS(1000);

#elif (UARTx == 2)
    //Enable GPIO and UART Clock
    System_Module_Enable(EN_UART2);
    System_Module_Enable(EN_GPIOAB);
    //Config GPIO
    /* A2:Tx  A3:Rx */
    GPIO_InitStruct.Pin       = GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Alternate = GPIO_FUNCTION_2;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    //UART Config
    UART_InitStruc.UART_BaudRate    = Baudrate;
    UART_InitStruc.UART_WordLength  = UART_WORDLENGTH_8B;
    UART_InitStruc.UART_StopBits    = UART_STOPBITS_1;
    UART_InitStruc.UART_Parity      = UART_PARITY_NONE;
    UART_InitStruc.UART_Mode        = UART_MODE_TX_RX;
    UART_InitStruc.UART_HardwareFlowControl = UART_HWCONTROL_NONE;
    UART_Init(UART2, &UART_InitStruc);
    /* NVIC Config */
    NVIC_ClearPendingIRQ(UART2_IRQn);
    NVIC_SetPriority(UART2_IRQn, 5);
    NVIC_EnableIRQ(UART2_IRQn);

    UART_ITConfig(UART2, UART_IT_RXNE, ENABLE);   //中断初始化

    UART_Cmd(UART2, ENABLE);
    System_Delay_MS(1000);
		
#endif

	LCD_UartSend("CLR(0);\r\n");
//	CheckBusy();
	LCD_UartSend("DIR(1);\r\n");
//	CheckBusy();
}


void UartSend(char * data_buf)
{
    while (1)
    {
        if ((*data_buf) != 0) //直到数据都发送完成
        {
            UART_SendData(LCD_UART, (uint8_t)*data_buf); //发送一个字节数据
            data_buf++;
        }
        else
        {
            return;
        }
    }
}

u8 CheckBusy2(void)					//忙检测, 既等待数据发送成功
{
    u32 TimeOut = 0;
    for (;;)
    {
        if (UartRxFinished == 0xC0)
        {
            if ((RxBuf[0] == 'O') && (RxBuf[1] == 'K'))
            {
                RxBuf[0] = 0;
                RxBuf[1] = 0;
                UartRxFinished = 0;
                RxCount = 0;
                return 0;
            }
        }
        if (TimeOut++ >= 10000)
        {
            UartRxFinished = 0;
            RxCount = 0;
            return 1;
        }
    }
}
#if 0
void LCD_UartSend(const char *DataBuf)			
{
	u8 BusyFlag = 1;			//1代表处于忙状态
	u16 Index = 0;
	u8 ShowCount = 10;			//显示失败时，允许重新显示的次数
	
	while(BusyFlag && ShowCount)
	{
		if (*(DataBuf + Index) == 0)
		{
			BusyFlag = CheckBusy2();
			Index = 0;
			ShowCount--;
			System_Delay_MS(1);
			continue;
		}
		UART_SendData(LCD_UART, *(DataBuf + Index)); //发送一个字节数据
		while (LCD_UART->FR & UART_FR_BUSY); //用于检查串口UART2是否发送完成，完成时，TC中断标志置位，
		Index++;
	}
}
#else 
void LCD_UartSend(const char *DataBuf)	
{
	u8 ShowCount;			//显示失败时，允许重新显示的次数
	
	for(ShowCount = 0; ShowCount < 10; ShowCount++)
	{
		UartSend((char*)DataBuf);
		while(1)
		{
			if (RxState == UART_RX_COMPLETE) 
			{
				RxState = UART_RX_IDLE;
				RxCount = 0;
				if (strncmp((char*)RxBuf, "OK", 2) == 0) 
				{
					return;         
				}
				break;
			}	
			
			if (RxState != UART_RX_IDLE && RxTimeout > 0) 
			{
				RxTimeout--;  
				if (RxTimeout == 0) 
				{
					RxState = UART_RX_IDLE;
					RxCount = 0;
					break;
				}
			}
		}
	}
}

#endif
		
		
//void LCD_UartSend(const char *DataBuf)	
//{
//	UartSend((char*)DataBuf);
//	System_Delay_MS(1);

//}
		
		
#if 0
void UART2_IRQ(void)                
{
    u8 Res = 1;
    if (UART_GetITStatus(UART2, UART_IT_RXNE) != RESET) //0x0d 0x0a  串口传输状态获取  中断产生
    {
		/* 数据格式：OK\r\n */
        Res = UART_ReceiveData(UART2); 				
		
        if (UartRxFinished != 0xC0) 					
        {
            if (UartRxFinished & 0x40) 		//\r是否已经接收到
            {
				/* 接收到 0x0A -- '\n',表示数据接收完成，此时UartRxFinished == 0xC0  */
                if (Res == DATAEND_FLAG2)
                {
                    UartRxFinished |= 0x80; 		
                    RxBuf[UartRxCount] = '\0'; 	
                }
                else
                {
					/* 数据接收失败，重新接收 */
                    UartRxFinished = 0;			 
                    UartRxCount = 0;
                }
            }
            else
            {
				/* 接收到 0x0D -- '\r',表示数据部分接收完毕，此时UartRxFinished == 0x40 */
                if (Res != DATAEND_FLAG1)				
                {
                    RxBuf[UartRxCount++] = Res ;
                }
                else
                {
                    UartRxFinished |= 0x40;			
                }
            }
        }
		UART_ClearITPendingBit(UART2, UART_IT_RXNE);
    }
}
#else
void UART2_IRQ(void) 
{
    if (UART_GetITStatus(UART2, UART_IT_RXNE) != RESET) 
	{  
        u8 Res = UART_ReceiveData(UART2);  
        switch (RxState) 
		{
		case UART_RX_IDLE:
			if (Res != DATAEND_FLAG1 && Res != DATAEND_FLAG2) 
			{
				RxBuf[RxCount++] = Res;         
				RxState = UART_RX_RECEIVING;     // 切换状态
				RxTimeout = 10000;                 
			}
			break;

		case UART_RX_RECEIVING:
			if (Res == DATAEND_FLAG1) 
			{ 
				RxState = UART_RX_CR;
			} 
			else 
			{
				if (RxCount < sizeof(RxBuf)-1) 
				{ 
					RxBuf[RxCount++] = Res;     
				} 
				else 
				{
					RxCount = 0;
					RxState = UART_RX_RECEIVING;    //出现错误，全部接收完，等待超时
				}
			}
			RxTimeout = 10000;  
			break;

		case UART_RX_CR:
			if (Res == DATAEND_FLAG2) 
			{       
				RxBuf[RxCount] = '\0';           
				RxState = UART_RX_COMPLETE;   
			} 
			else 
			{    
				RxState = UART_RX_RECEIVING;   //出现错误，全部接收完，等待超时
			}
			RxTimeout = 10000;  
			break;

		default:
			RxState = UART_RX_IDLE;
			RxCount = 0;
			break;
		}
        UART_ClearITPendingBit(UART2, UART_IT_RXNE);  // 清除接收中断标志
    }
}
#endif



void StrMiddleShow(char *StrName, u8 Yaxis, u8 Color)      //让字符串在每行居中显示
{
    u8 Strlen = 0;
    u8 Xaxis = 0;

    Strlen = strlen(StrName);               //获取型号长度
    Xaxis = (160 - 8 * Strlen) / 2;          //计算型号要显示的位置的x轴
    LCD_UartSend(CMD_DC16(Xaxis, Yaxis, StrName, Color));
//    CheckBusy();

}


//"BOX(115,50,150,90,5)
char* CMD_BOX(u16 x1, u16 y1, u16 x2, u16 y2, u8 Color)
{
	  u8 i, j;
    u8 CmdStart[4] = {"BOX("};     //开始指令
    u8 CmdEnd[4] = {");\r\n"};      //结束指令

//    MemsetData(CmdBuf, 0, 64);

		memset(CmdBuf, 0, sizeof(CmdBuf));
    for (i = 0; i < 4; i++)
    {
        CmdBuf[i] = CmdStart[i];   
    }
		
		//传坐标X和Y 128X160像素点
    CmdBuf[i++] = x1 / 100 + 0x30;       //百位
    CmdBuf[i++] = x1 % 100 / 10 + 0x30;  //十位
    CmdBuf[i++] = x1 % 10 + 0x30;        //个位
    CmdBuf[i++] = ',';

    CmdBuf[i++] = y1 / 100 + 0x30;       //百位
    CmdBuf[i++] = y1 % 100 / 10 + 0x30;  //十位
    CmdBuf[i++] = y1 % 10 + 0x30;        //个位
    CmdBuf[i++] = ',';
		
		CmdBuf[i++] = x2 / 100 + 0x30;       //百位
    CmdBuf[i++] = x2 % 100 / 10 + 0x30;  //十位
    CmdBuf[i++] = x2 % 10 + 0x30;        //个位
    CmdBuf[i++] = ',';
		
		CmdBuf[i++] = y2 / 100 + 0x30;       //百位
    CmdBuf[i++] = y2 % 100 / 10 + 0x30;  //十位
    CmdBuf[i++] = y2 % 10 + 0x30;        //个位
    CmdBuf[i++] = ',';

		CmdBuf[i++] = Color / 10 + 0x30;     //十位
    CmdBuf[i++] = Color % 10 + 0x30;     //个位
		
		for (j = 0; j < 4; j++)
    {
        CmdBuf[i + j] = CmdEnd[j];
    }
		i += j;
		return (char*)CmdBuf;                   //返回命令指针
}

char* CMD_DC24(u16 x, u16 y, char *Str, u8 Color)
{
    u8 i, j;
    u16 Set_X;                          //X坐标
    u16 Set_Y;                          //Y坐标
    u8 CmdStart[6] = {"DCV24("};     //开始指令
    u8 CmdEnd[4] = {");\r\n"};      //结束指令

//    MemsetData(CmdBuf, 0, 64);

	memset(CmdBuf, 0, sizeof(CmdBuf));
    for (i = 0; i < 6; i++)
    {
        CmdBuf[i] = CmdStart[i];    
    }

//传坐标X和Y 128X160像素点
    Set_X = x;
    CmdBuf[i++] = Set_X / 100 + 0x30;       //百位
    CmdBuf[i++] = Set_X % 100 / 10 + 0x30;  //十位
    CmdBuf[i++] = Set_X % 10 + 0x30;        //个位
    CmdBuf[i++] = ',';

    Set_Y = y;
    CmdBuf[i++] = Set_Y / 100 + 0x30;       //百位
    CmdBuf[i++] = Set_Y % 100 / 10 + 0x30;  //十位
    CmdBuf[i++] = Set_Y % 10 + 0x30;        //个位
    CmdBuf[i++] = ',';
    CmdBuf[i++] = '\'';

//传显示字符串
    j = 0;
    while (Str[j] != '\0')
    {
        CmdBuf[i + j] = Str[j];
        j++;
    }
    i += j;

//传显示颜色
    CmdBuf[i++] = '\'';
    CmdBuf[i++] = ',';
    CmdBuf[i++] = Color / 10 + 0x30;     //十位
    CmdBuf[i++] = Color % 10 + 0x30;     //个位

//传输结束指令
    for (j = 0; j < 4; j++)
    {
        CmdBuf[i + j] = CmdEnd[j];
    }

    i += j;

    return (char*)CmdBuf;                   //返回命令指针
}

char* CMD_DC16(u16 x, u16 y, char *Str, u8 Color)
{
    u8 i, j;
    u16 Set_X;                          //X坐标
    u16 Set_Y;                          //Y坐标
    u8 CmdStart[5] = {"DC16("};     //开始指令
    u8 CmdEnd[4] = {");\r\n"};      //结束指令

//    MemsetData(CmdBuf, 0, 64);

	memset(CmdBuf, 0, sizeof(CmdBuf));
    for (i = 0; i < 5; i++)
    {
        CmdBuf[i] = CmdStart[i];    //传“DC16(”
    }

//传坐标X和Y 128X160像素点
    Set_X = x;
    CmdBuf[i++] = Set_X / 100 + 0x30;       //百位
    CmdBuf[i++] = Set_X % 100 / 10 + 0x30;  //十位
    CmdBuf[i++] = Set_X % 10 + 0x30;        //个位
    CmdBuf[i++] = ',';

    Set_Y = y;
    CmdBuf[i++] = Set_Y / 100 + 0x30;       //百位
    CmdBuf[i++] = Set_Y % 100 / 10 + 0x30;  //十位
    CmdBuf[i++] = Set_Y % 10 + 0x30;        //个位
    CmdBuf[i++] = ',';
    CmdBuf[i++] = '\'';

//传显示字符串
    j = 0;
    while (Str[j] != '\0')
    {
        CmdBuf[i + j] = Str[j];
        j++;
    }
    i += j;

//传显示颜色
    CmdBuf[i++] = '\'';
    CmdBuf[i++] = ',';
    CmdBuf[i++] = Color / 10 + 0x30;     //十位
    CmdBuf[i++] = Color % 10 + 0x30;     //个位

//传输结束指令
    for (j = 0; j < 4; j++)
    {
        CmdBuf[i + j] = CmdEnd[j];
    }

    i += j;

    return (char*)CmdBuf;                   //返回命令指针
}


char* CMD_DC16_Len(u16 x, u16 y, char *Str, u32 Length, u8 Color)
{
    u8 i, j;
    u16 Set_X;                          //X坐标
    u16 Set_Y;                          //Y坐标

    u8 CmdStart[5] = {"DC16("};     //开始指令
    u8 CmdEnd[4] = {");\r\n"};      //结束指令

//   MemsetData(CmdBuf, 0, 64);

    for (i = 0; i < 5; i++)
    {
        CmdBuf[i] = CmdStart[i];    //传“DC16(”
    }

//传坐标X和Y 128X160像素点
    Set_X = x;
    CmdBuf[i++] = Set_X / 100 + 0x30;       //百位
    CmdBuf[i++] = Set_X % 100 / 10 + 0x30;  //十位
    CmdBuf[i++] = Set_X % 10 + 0x30;        //个位
    CmdBuf[i++] = ',';

    Set_Y = y;
    CmdBuf[i++] = Set_Y / 100 + 0x30;       //百位
    CmdBuf[i++] = Set_Y % 100 / 10 + 0x30;  //十位
    CmdBuf[i++] = Set_Y % 10 + 0x30;        //个位
    CmdBuf[i++] = ',';
    CmdBuf[i++] = '\'';

//传显示字符串
    for (j = 0; j < Length; j++)
    {
        CmdBuf[i + j] = Str[j];
    }
    i += j;

//传显示颜色
    CmdBuf[i++] = '\'';
    CmdBuf[i++] = ',';
    CmdBuf[i++] = Color / 10 + 0x30;     //十位
    CmdBuf[i++] = Color % 10 + 0x30;     //个位

//传输结束指令
    for (j = 0; j < 4; j++)
    {
        CmdBuf[i + j] = CmdEnd[j];
    }

    i += j;

    return (char*)CmdBuf;                   //返回命令指针
}




//void GetSDVersion(void)
//{
//    SD_ReadDisk(IAP_Version, 3, 1);
//    IAP_Version[3] = '\0';

//    MainpageShow((char *)IAP_Version);
//}

void Interface_Display(void)
{
    char Interface_buf[32];
    LCD_UartSend("CLR(0);\r\n");        //清屏
    LCD_UartSend("DIR(1);\r\n");         //横屏

    LCD_UartSend("PL(35,30,110,30,2);\r\n");//线
    LCD_UartSend("PL(25,40,100,40,2);\r\n");

    LCD_UartSend("PL(35,30,25,40,2);\r\n");

    LCD_UartSend("PL(110,30,100,40,2);\r\n");


    LCD_UartSend("PL(25,40,25,100,2);\r\n");

    LCD_UartSend("PL(100,40,100,100,2);\r\n");

    LCD_UartSend("PL(25,100,100,100,2);\r\n");

    LCD_UartSend("PL(110,30,110,90,2);\r\n");

    LCD_UartSend("PL(100,100,110,90,2);\r\n");


    for (int i = 30; i < 90; i += 3)  //点
    {
        sprintf(Interface_buf, "PS(35,%d,2);\r\n", i);
        LCD_UartSend(Interface_buf);
    }

    for (int i = 35; i < 110; i += 3)
    {
        sprintf(Interface_buf, "PS(%d,90,2);\r\n", i);
        LCD_UartSend(Interface_buf);
    }


    LCD_UartSend("PS(27,98,2);\r\n");
    LCD_UartSend("PS(29,96,2);\r\n");
    LCD_UartSend("PS(31,94,2);\r\n");
    LCD_UartSend("PS(33,92,2);\r\n");



    LCD_UartSend("PL(110,40,100,50,4);\r\n");  //端口1

    LCD_UartSend("PL(110,50,100,60,4);\r\n");

    LCD_UartSend("PL(110,40,110,50,4);\r\n");
    LCD_UartSend("PL(100,50,100,60,4);\r\n");


    LCD_UartSend("PL(110,70,100,80,4);\r\n");//端口2

    LCD_UartSend("PL(110,80,100,90,4);\r\n");

    LCD_UartSend("PL(110,70,110,80,4);\r\n");
    LCD_UartSend("PL(100,80,100,90,4);\r\n");

    LCD_UartSend("PL(102,30,92,40,4);\r\n");//网口
    LCD_UartSend("PL(92,30,82,40,4);\r\n");

    LCD_UartSend("PL(102,30,92,30,4);\r\n");

    LCD_UartSend("PL(82,40,92,30,4);\r\n");


    LCD_UartSend("PL(50,30,40,40,4);\r\n");//电源
    LCD_UartSend("PL(40,30,30,40,4);\r\n");

    LCD_UartSend("PL(50,30,40,30,4);\r\n");

    LCD_UartSend("PL(30,40,40,30,4);\r\n");



    LCD_UartSend("PL(72,30,62,40,4);\r\n");//网口
    LCD_UartSend("PL(62,30,52,40,4);\r\n");

    LCD_UartSend("PL(72,30,62,30,4);\r\n");

    LCD_UartSend("PL(52,40,62,30,4);\r\n");


//    LCD_UartSend("DC16(10,110,'- 按 \"Enter\" 键 -',4);\r\n");

//    Uart_Send("DC16(0,5,'·根据提示接入端口·',4);\r\n");
//    Check_Busy();




//    LCD_UartSend("DC16(0, 0,'根',4);\r\n");
//    LCD_UartSend("DC16(0,20,'据',4);\r\n");
//    LCD_UartSend("DC16(0,40,'提',4);\r\n");
//    LCD_UartSend("DC16(0,60,'示',4);\r\n");
//    LCD_UartSend("DC16(0,80,'接',4);\r\n");
//    LCD_UartSend("DC16(0,100,'入',4);\r\n");
}
void Uart_Action(void) //串口插入动作
{
    char uart_buf[32];
    for (int i = 5; i < 25; i += 5)
    {
        sprintf(uart_buf, "SBC(0);DCV16(55,%d,'↓',2);\r\n", i);
        LCD_UartSend(uart_buf);
        System_Delay_MS(10);
    }

    LCD_UartSend("SBC(0);DCV16(55,5,'串口',2);\r\n");
}

void Power_Action(void) //电源插入动作
{
    char uart_buf[32];
    for (int i = 5; i < 25; i += 5)
    {
        sprintf(uart_buf, "SBC(0);DCV16(33,%d,'↓',2);\r\n", i);
        LCD_UartSend(uart_buf);
        System_Delay_MS(10);
    }

    LCD_UartSend("SBC(0);DCV16(23,5,'电源',2);\r\n");
}


void Ethernet_Action(void)//网口插入动作
{
    char Ethernet_buf[32];
    for (int i = 5; i < 25; i += 5)
    {

        sprintf(Ethernet_buf, "SBC(0);DCV16(85,%d,'↓',2);\r\n", i);
        LCD_UartSend(Ethernet_buf);

        System_Delay_MS(10);
    }

    LCD_UartSend("SBC(0);DCV16(85,5,'网口',2);\r\n");
}

void Port1_Action(void)//端口1插入动作
{
    char Port1_buf[32];
    for (int i = 125; i > 100; i -= 5)
    {
        sprintf(Port1_buf, "SBC(0);DCV16(%d,43,'←',2);\r\n", i);
        LCD_UartSend(Port1_buf);

        System_Delay_MS(10);
    }
    LCD_UartSend("SBC(0);DCV16(120,43,'端口1',2);\r\n");

}

void Port2_Action(void)//端口二插入动作
{
    char Port2_buf[32];
    for (int i = 125; i > 100; i -= 5)
    {
        sprintf(Port2_buf, "SBC(0);DCV16(%d,73,'←',2);\r\n", i);
        LCD_UartSend(Port2_buf);
        System_Delay_MS(10);
    }

    LCD_UartSend("SBC(0);DCV16(120,73,'端口2',2);\r\n");
}
