/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : app.c
 * Description : application example source file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#include  "app.h"
#include  "iso7816ms.h"
#define USE_7816MSRST_INT

UINT32 g_data_buf[1024];


//状态码返回值
UINT8 SW1, SW2;
//卡需接受数据的个数
UINT8 LC_DATA_LEN = 0;
UINT8 LE_DATA_LEN = 0;
//指令
UINT8 CLA, INS, P1, P2, P3;


/************************************************************************
 * function   : CalBitNum
 * Description: calculate the number of 1 in *A
 * input :
 *         UINT8 *A   : data to be calculated
		   UINT8 Alen :	the len to be calculated
 * return: number of 1 in *A
 ************************************************************************/
UINT8 CalBitNum(UINT8 *A, UINT8 Alen)
{
	int	i, j;
	UINT8 Scan;

	j = 0;
	Scan = 0x80;
	for(i = 8; i > 0; i--)
	{
		if(A[Alen - 1] & Scan)
		{
			j++;
		}
		Scan = Scan >> 1;

	}
	return (j);
}
/************************************************************************
 * function   : iso7816m_atr_rec
 * Description: SCIM atr receive
 * input :
 *         UINT8* data_rec  : start address of received ATR
 * return: len of ATR
 ************************************************************************/
UINT8 iso7816m_atr_rec(UINT8 *data_rec)
{
	UINT8 td, t0;
	UINT8 i = 0;
	UINT8 flag = 1;
	UINT8 l;
	UINT8 tck_flag = 0;

	ISO7816_t0_rx_poll(&data_rec[i], 1);
	i++;
	
	while(flag)
	{
		ISO7816_t0_rx_poll(&td,1);
		//if only T=0 is indicated, TCK shall not be sent
		if((i != 1) && ((td & 0x0f) != 0x00))//判断T=1变没变
		{
			tck_flag = 1;//第二轮开始检查，T=1有TCK
		}
		data_rec[i] = td;
		i++;
		
		if((td & 0x10) == 0x10)
		{
			ISO7816_t0_rx_poll(&data_rec[i],1);
			i++;
		}

		if((td & 0x20) == 0x20)
		{
			ISO7816_t0_rx_poll(&data_rec[i],1);
			i++;
		}

		if((td & 0x40) == 0x40)
		{
			ISO7816_t0_rx_poll(&data_rec[i],1);
			i++;
		}

		if((td & 0x80) == 0x80)
		{
			flag = 1;//TD =1表示还有
		}
		else
		{
			flag = 0;//表示收完头
		}
	}
//以下开始接收历史字节
	t0 = data_rec[1];
	l = (t0 & 0x0f) + i;
	
	ISO7816_t0_rx_poll(&data_rec[i],l-i);	//store bytes

	if(tck_flag == 1)   // TCK exist，多收一个字节
	{
		ISO7816_t0_rx_poll(&data_rec[l],1);
		l++;
	}
	return l;
}

void iso7816m_test(void)
{
	int i;
	UINT8 flag = 0;
	UINT8 atr_rec_buffer[32] = { 0 };
	UINT8 uci_sent_buffer[256] = { 0 };
	UINT8 uci_rec_buffer[256] = { 0 };
	UINT8 atr_length = 0;
	UINT8 T_flag = 0;
	UINT8 TA1, TD1, TA2;
	UINT8 temp;
	UINT8 rcv_length = 0;
	UINT32 comm_count = 0;	
	
	ISO7816_master_init();

	printfS("7816sm master test start, wait card insert---> \n");
	while((REG_7816_ISR & (1 << 12)) == 0);    //wait card present
	
	printfS("begin to receive atr \n");	
	
	ISO7816_master_cold_rst();
	REG_SCU_MUXCTRLD |=	PAD_SCC_MS_CIO_EN;//再配成SCC_MS_SIO
	
	atr_length = iso7816m_atr_rec(atr_rec_buffer);//收取ATR,返回值为ATR的长度

	if(atr_length == 0)
	{
		for(i = 0; i < 3; i++)
		{
			ISO7816_master_warm_rst();
			atr_length = iso7816m_atr_rec(atr_rec_buffer);
			if(atr_length != 0)
			{
				flag = 0;
				break;
			}
			else
			{
				flag = 1;
			}
		}
	}
	if(flag == 1)
	{
		flag = 0;
		printfS("atr isn't be received!\n\n");
	}
	else
	{
		printfS("ATR: ");
		printfB8(atr_rec_buffer, atr_length);
	}
	//判断格式字符T0的高4bit
	if((atr_rec_buffer[1] & 0x80) == 0x00)
	{ //不存在TD1，则也就不存在TA2,因此通信方式为T=0
		T_flag = 0;
	}
	else
	{ //存在TD1
		atr_rec_buffer[1] = (atr_rec_buffer[1] & 0xf0);
		temp = CalBitNum(atr_rec_buffer, 2);
		//判断TD1
		TD1 = 	atr_rec_buffer[temp + 1];
		if((TD1 & 0x10) == 0x10)
		{ //存在TA2
			TA2 = atr_rec_buffer[temp + 2];
			; //专用模式
			if((TA2 & 0x10) == 0x00)
			{ //使用TA1指定的F和D值,否则使用默认值
			  //存在TA1
				if((atr_rec_buffer[1] & 0x10) == 0x10)
				{
					TA1 = atr_rec_buffer[2];
					ISO7816_change_baud_rate(TA1);
				}
			}
		}
		if((TD1 & 0x0f) == 0x00)
		{
			T_flag = 0;
		}
		else
		{
			T_flag = 1;
		}
	}

	//根据复位应答选择通信协议
	if(T_flag == 0x00) //T=0通信
	{
//		uci_sent_buffer[0] = 0xFF;
//		uci_sent_buffer[1] = 0x10;
//		uci_sent_buffer[2] = 0x12;    //19200
//		uci_sent_buffer[3] = 0xFC;
//		ISO7816_t0_tx_poll(uci_sent_buffer, 4);   // set PPS

//		rcv_length = ISO7816_t0_rx_poll(uci_rec_buffer, 4);
//		printfS("PPS: ");
//		printfB8(uci_rec_buffer, rcv_length);
//		ISO7816_change_baud_rate(uci_rec_buffer[2]);

		comm_count = 0;
		while(1)
		{
			comm_count++;
			printfS("\ncomm_count: %d\n", comm_count);

			uci_sent_buffer[0] = 0x00;
			uci_sent_buffer[1] = 0x84;
			uci_sent_buffer[2] = 0x00;
			uci_sent_buffer[3] = 0x00;
			uci_sent_buffer[4] = 0x04;
			ISO7816_t0_tx_poll(uci_sent_buffer, 5); // transfer the command bytes
			
			printfS("send test command: ");
			printfB8(uci_sent_buffer, 5);
			
			rcv_length = ISO7816_t0_rx_poll(uci_rec_buffer, 1);
			if(uci_rec_buffer[0]==0x6D)
			{
				ISO7816_t0_rx_poll(&uci_rec_buffer[1], 1);
				printfS("SW1SW2: ");
				printfB8(uci_rec_buffer, 2);
				delay(4000000);
				continue;				
			}			
			
			printfS("ACK INS: ");
			printfB8(uci_rec_buffer, rcv_length);
			
			rcv_length = ISO7816_t0_rx_poll(uci_rec_buffer, 2);
			
			printfS("LE DATA: ");
			printfB8(uci_rec_buffer, rcv_length);
			
			rcv_length = ISO7816_t0_rx_poll(uci_rec_buffer, 2);
			
			printfS("SW1SW2: ");
			printfB8(uci_rec_buffer, rcv_length);
			
			delay(4000000);			
	
		}
	}
}

void iso7816s_test(void)
{
	UINT8 PPS = 1;
	UINT8 buff[2];
	UINT8 rec_data_num = 0;
	UINT8 test_data[8] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88 };

	ISO7816_slave_init();

#ifdef USE_7816MSRST_INT	//slave 复位允许中断
	NVIC_EnableIRQ(MS7816RST_IRQn);
	while(!(ISO7816_rst_flag));//等待RST释放变高	
	if(ISO7816_rst_flag)
	{
		ISO7816_rst_flag = 0;
		printfS("slave RST_inatila_int happened\n");
	}
#else						//slave 复位不允许中断
	while(1)
	{
		if(!(REG_SCU_SCICTRL&(1<<4)))  //去抖动
		{
			delay(1000);
			if(!(REG_SCU_SCICTRL&(1<<4))) 	break;	
		}
	}
    while(!(REG_SCU_SCICTRL&(1<<4)));//等待RST释放变高
	ISO7816_delay_cck_num(400);	  //wait ATR
	ISO7816_t0_tx_poll(T0_atr_buffer, 18);
#endif		


	
	printfS("7816sm slave test start\n");
	printfS("send ATR OK2\n");

	ISO7816_slave_int_en();
	ISO7816_atr_flag = 1;//该标志指示刚刚发完ATR，允许调整PPS
	
	while(1)
	{
#ifdef USE_7816MSRST_INT		
		if(ISO7816_rst_flag)
		{
			ISO7816_rst_flag = 0;
			printfS("slave RST_int happened\n");
		}		
#endif			
		if(ISO7816_atr_flag == 1)
		{
			ISO7816_atr_flag = 0;
			rec_data_num = 0;
			PPS = 1;
			//printfS("receive atr int\n");
		}

		if(ISO7816_RX_flag == 1)
		{
			ISO7816_RX_flag = 0;
			DATABUF[rec_data_num++] = ISO7816_buff;			
			
			//PPSS为0xFF的处理
			if(DATABUF[0] != 0xff)
			{
				PPS = 0;
			}
			else
			{
				if((PPS == 1) && (rec_data_num == 4))
				{
					PPS = 0;
					rec_data_num = 0;

					ISO7816_t0_tx_poll(DATABUF, 4);
					ISO7816_change_baud_rate(DATABUF[2]);
					printfS("REG_7816_BPR %d\n",REG_7816_BPR);
					printfS("set pps OK\n");
				}
			}

			//指令解析
			if(rec_data_num == 5)
			{	
				CLA = DATABUF[0];
				INS = DATABUF[1];
				P1 = DATABUF[2];
				P2 = DATABUF[3];
				P3 = DATABUF[4];

				//判断接收或发送数据长度
				if((INS == 0xC0) || (INS == 0xB0))
				{
					LC_DATA_LEN = 0;
					LE_DATA_LEN = P3;
					//返回确认信息
					ISO7816_t0_tx_poll(&INS, 1);					
					//printfS("INS:%x\n", INS);
				}
				else if(INS == 0xd0)
				{
					LC_DATA_LEN = P3;
					LE_DATA_LEN = 0;
					//返回确认信息
					ISO7816_t0_tx_poll(&INS, 1);					
					//printfS("INS:%x\n", INS);
				}
				else
				{
					LC_DATA_LEN = 0;
					LE_DATA_LEN = 0;
				}
			}		
			
			//开始命令处理
			if(rec_data_num == (5 + LC_DATA_LEN))
			{
				switch(INS)
				{
					case 0xc0:
					{							
						ISO7816_t0_tx_poll(test_data, LE_DATA_LEN);
						SW1 = 0x90;
						SW2 = 0x00;
						break;
					}
					case 0xb0:
					{
						ISO7816_t0_tx_poll(test_data, LE_DATA_LEN);
						SW1 = 0x90;
						SW2 = 0x00;
						break;
					}
					case 0xd0:
					{
						memcpy(test_data, DATABUF+5, LC_DATA_LEN);						
						SW1 = 0x90;
						SW2 = 0x00;
						break;
					}
					default:
					{
						SW1 = 0x6d;
						SW2 = 0x00;
					}
				}
				rec_data_num = 0;

				buff[0] = SW1;
				buff[1] = SW2;
				ISO7816_t0_tx_poll(buff, 2);
			}
		}
	}
}

