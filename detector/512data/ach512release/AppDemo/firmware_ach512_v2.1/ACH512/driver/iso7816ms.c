/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : iso7816ms.c
 * Description : iso7816 master/slave driver source file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#include "iso7816ms.h"

volatile UINT8 ISO7816_buff;
volatile UINT8 ISO7816_RX_flag = 0;
volatile UINT8 ISO7816_atr_flag = 0;
volatile UINT8 ISO7816_rst_flag = 0;


UINT8 T0_atr_buffer[20] = { 0x3b, 0x6e, 0x00, 0x00, 0x11, 0x41, 0x43, 0x48, 0x41, 0x69, 0x73, 0x69, 0x6E, 0x6F, 0x43, 0x68, 0x69, 0x70, 0x00, 0x00 };
//UINT8 T1_atr_buffer[24] = {0x3b,0xbc,0x94,0x00,0x81,0x31,0xde,0x65,0x50,0x4f,0x57,0x45,0x52,0x43,0x4f,0x53,0x32,0x31,0x90,0x00,0xb0,0x00,0x00,0x00 };


void  ISO7816MS_IRQHandler(void)
{
	if(REG_7816_ISR & 0x40)     //接收FIFO非空引发中断
	{
		ISO7816_RX_flag = 1;
		ISO7816_buff = REG_7816_DR;
		//REG_7816_ISR &= (~0x40);
	}

	NVIC_ClearPendingIRQ(MS7816_IRQn);
}

void  MS7816RST_IRQHandler(void) //从模式下卡RST复位产生中断
{
	NVIC_DisableIRQ(MS7816RST_IRQn);
	NVIC_ClearPendingIRQ(MS7816RST_IRQn);

	while(1)
	{
		if(!(REG_SCU_SCICTRL & (1 << 4)))  //去抖动
		{
			delay(1000); //delay 不能太久
			if(!(REG_SCU_SCICTRL & (1 << 4))) 	break;
		}
	}
	while(!(REG_SCU_SCICTRL & (1 << 4))); //等待RST释放变高

	ISO7816_rst_flag = 1;

	REG_7816_BPR = 0x174;

	ISO7816_delay_cck_num(400);

	ISO7816_t0_tx_poll(T0_atr_buffer, 18); //ATR

	NVIC_EnableIRQ(MS7816RST_IRQn);
}


/************************************************************************
 * function   : ISO7816_delay_etu_num (only useful in rcv direction)
 * Description: delay etu number time
 * input :
 *         etu_num: delay etu number
 * return: none
 ************************************************************************/
void ISO7816_delay_etu_num(UINT16 etu_num)
{
	REG_7816_ISR &= (~0x100);
	REG_7816_ETU |= (etu_num << 16);
	REG_7816_CTRL |= 0x400; // enable etu count
	while(!(REG_7816_ISR & 0x100));  //等待ETU计数器溢出
	REG_7816_CTRL &= (~0x400); // disable etu count
	REG_7816_ISR &= (~0x100);
}

/*******************************************************************************
* Function Name  : ISO7816S_delay_cck_num
* Description    : delay CCK number time
* Input          : - cck_num: delay CCK number.
* Output         : None
* Return         : None
*******************************************************************************/
void ISO7816_delay_cck_num(UINT32 cck_num)
{
	REG_7816_ISR &= (~0x200);
	REG_7816_CCKCNT = cck_num;
	REG_7816_CTRL |= 0x200; // enable cck count
	while(!(REG_7816_ISR & 0x200));  //等待ETU计数器溢出
	REG_7816_CTRL &= (~0x200); // disable etu count
	REG_7816_ISR &= (~0x200);
}

/************************************************************************
 * function   : ISO7816_rcv_to_send
 * Description: change receive to send mode
 * input : none
 * return: none
 ************************************************************************/
void ISO7816_rcv_to_send(void)
{
	//ISO7816_delay_etu_num(2);
	while(!(REG_7816_ISR & 0x2000));  //等待接收彻底完成(收完stop)
	REG_7816_CTRL |= 0x08;   //enable send mode
}

/************************************************************************
 * function   : ISO7816_send_to_rcv
 * Description: change receive to send mode
 * input : none
 * return: none
 ************************************************************************/
void ISO7816_send_to_rcv(void)
{
	REG_7816_CTRL &= 0xffff7;    //enable receive mode
}


/************************************************************************
 * function   : ISO7816_t0_tx_poll
 * Description: Transmits data through the SCI7816MS.
 * input :
 *         tx_data: the data to transmit.
 *         length: the length of data
 * return: none
 ************************************************************************/
void ISO7816_t0_tx_poll(UINT8 *tx_data, UINT32 length)
{
	UINT32 i;

	ISO7816_rcv_to_send();
	for(i = 0; i < length; i++)
	{
		REG_7816_DR = tx_data[i];
		while(!(REG_7816_ISR & 0x04));        //等待发送完成 //还可以检查重传错误flag
		REG_7816_ISR &= (~0x04);
#if	T==1
		while(!(REG_7816_ISR & 0x100));         //等待ETU计数器溢出
		REG_7816_ISR &= (~0x100);
#endif
	}
	ISO7816_send_to_rcv();
}


/************************************************************************
 * function   : ISO7816_t0_rx_poll
 * Description: Receives data through the SCI7816MS.
 * input :
 *         rx_data: the address of received data.
 *         length: the length of data
 * return: none
 ************************************************************************/
UINT8 ISO7816_t0_rx_poll(UINT8 *rx_data, UINT32 length)
{
	UINT32 i;

	for(i = 0; i < length; i++)
	{
		while(!(REG_7816_ISR & 0x40));  //等待接收FIFO非空
		rx_data[i] = REG_7816_DR;
	}
	//REG_7816_ISR &= (~0x40);
	return i;
}


/************************************************************************
 * function   : ISO7816_change_baud_rate
 * Description: change baud rate according to para of PPS
 * input :
 *         UINT8 para: the parameter of change baud rate
 * return: none
 ************************************************************************/
void ISO7816_change_baud_rate(UINT8 para)
{
	switch(para)
	{
		case 0x11:              //	3.57M  9600bps
		{
			REG_7816_BPR = 372;
			break;
		}
		case 0x12:              //	3.57M  19200bps
		{
			REG_7816_BPR = 186;
			break;
		}
		case 0x13:              //	3.57M  38400bps
		{
			REG_7816_BPR = 93;
			break;
		}
		case 0x94:              //	3.57M  57600bps,55781
		{
			REG_7816_BPR = 64;
			break;
		}
		case 0x14:              //	3.57M  76800bps
		{
			REG_7816_BPR = 47;
			break;
		}
		case 0x18:              //	3.57M  115200bps
		{
			REG_7816_BPR = 31;
			break;
		}
		case 0x96:              //	3.57M  223200bps
		{
			REG_7816_BPR = 16;
			break;
		}
		default:
		{
			REG_7816_BPR = 372;
			break;
		}
	}
}


/************************************************************************
 * function   : ISO7816_slave_rst_int_en
 * Description: enable the rst int of slave
 * input : none
 * return: none
 ************************************************************************/
void ISO7816_slave_int_en(void)
{
	NVIC_ClearPendingIRQ(MS7816_IRQn);
	NVIC_EnableIRQ(MS7816_IRQn);
	REG_7816_IER |= (1 << 6);    //rx int en
}

/************************************************************************
 * function   : ISO7816_slave_init
 * Description: ISO7816 slvae initial
 * input : none
 * return: none
 ************************************************************************/
void ISO7816_slave_init(void)
{
#ifdef LOW_POWER
	enable_module(BIT_7816MS); //enable 7816MS
#endif

//rst used as gpio to read status / generate interrupt
	REG_SCU_MUXCTRLD = (REG_SCU_MUXCTRLD & ~(0xfc000000)) | PAD_SCC_MS_RST_EN | PAD_SCC_MS_CLK_EN | PAD_SCC_MS_CIO_EN;
	REG_7816_CTRL &= (~0x10000); //salve mode
	REG_7816_IER = 0;            //禁止所有ISO7816中断
#if	T==1
	REG_7816_CTRL |= 0x2c;   //选择T=1,硬件自动重发,偶校验,正向模式(LSB在前),发送模式打开
	REG_7816_ETU = 0x01;     //发送数据增加额外一个ETU
#else
	REG_7816_CTRL |= 0x0c;   //选择T=0,硬件自动重发,偶校验,正向模式(LSB在前),发送模式打开
	REG_7816_ETU = 0x00;
#endif

	//接收前清空接收FIFO
	REG_7816_CTRL |= 0x10;
	REG_7816_CTRL &= (~0x10);

	REG_7816_ISR = 0;           //清除所有状态位
	REG_7816_BPR = 0x0174;      //恢复到默认波特率9600bps/3.57M

	REG_7816_IER |= (1 << 11);      //使能7816卡插入中断

}

/************************************************************************
 * function   : ISO7816_master_init
 * Description: ISO7816 master initial
 * input : none
 * return: none
 ************************************************************************/
void ISO7816_master_init(void)
{
#ifdef LOW_POWER
	enable_module(BIT_7816MS); //enable UARTA
#endif
	REG_SCU_MUXCTRLD = (REG_SCU_MUXCTRLD & ~(0xffc00000)) | PAD_SCC_MS_RST_EN | PAD_SCC_MS_CLK_EN | PAD_SCC_MS_DET_EN | PAD_SCC_MS_VCC_EN;

	REG_7816_CTRL |= (1 << 16);    //master mode
	REG_7816_MCTRL |= (0x06 << 4); //7816clk = sys_clk/12; 1M?
	REG_7816_RSTT = 8000;        //7816 clk，复位时间设置

	REG_7816_IER = 0;            //禁止所有ISO7816中断
#if	T==1
	REG_7816_CTRL |= 0x2c;   //选择T=1,硬件自动重发,偶校验,正向模式(LSB在前),发送模式打开
	REG_7816_ETU = 0x01;     //发送数据增加额外一个ETU
#else
	REG_7816_CTRL |= 0x0c;   //选择T=0,硬件自动重发,偶校验,正向模式(LSB在前),发送模式打开
	REG_7816_ETU = 0x00;
#endif

	//接收前清空接收FIFO
	REG_7816_CTRL |= 0x10;
	REG_7816_CTRL &= (~0x10);

	REG_7816_ISR = 0;        //清除所有状态位
	REG_7816_BPR = 0x0174;   //恢复到默认波特率9600bps/3.57M
	ISO7816_send_to_rcv();
	REG_SCU_SCICTRL |= (1 << 2); //7816M 低电平卡到位
	REG_SCU_SCICTRL |= (1 << 3); //7816M VCCEN 低电平使能，该电平信号用来控制外围电路使能VCC

}

/************************************************************************
 * function   : ISO7816_master_warm_rst
 * Description: ISO7816 master warm reset
 * input : none
 * return: none
 ************************************************************************/
void ISO7816_master_warm_rst(void)
{
	REG_7816_MCTRL |= (1 << 1);   //warm rst
}
/************************************************************************
 * function   : ISO7816_master_cold_rst
 * Description: ISO7816 master cold reset
 * input : none
 * return: none
 ************************************************************************/
void ISO7816_master_cold_rst(void)
{
	REG_7816_MCTRL |= (1 << 2);   //cold rst
	while(1)
	{
		if(!(REG_SCU_SCICTRL & (1 << 4)))  //去抖动，变低
		{
			delay(100);
			if(!(REG_SCU_SCICTRL & (1 << 4)))
			{
				break; //还是低
			}
		}
	}
	while(!(REG_SCU_SCICTRL & (1 << 4))); //等待RST释放变高
}


