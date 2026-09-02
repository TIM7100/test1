/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : iso7816_app.c
 * Description : iso7816 application demo
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#include  "iso7816_app.h"

//function description:calculate the bits of last word of A
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
 * function   : ISO7816_atr_rec
 * Description: receive the data of ATR from card,and set the CONV bit according to the TS byte
 * input : 
 *         UINT8* data_rec: data_rec: the pointer of data buffer to store receive data.
 * return: return atr length if success, else return 0
 ************************************************************************/
UINT8 ISO7816_atr_rec(UINT8 *data_rec)
{
	UINT8 td, t0;
	UINT8 i = 0;
	UINT8 flag = 1;
	UINT8 l;
	UINT8 tck_flag = 0;

	ISO7816_t0_rx_poll(&data_rec[i], 1);
	i++;
	printfS("TS = %x\n", data_rec[i - 1]);

	while(flag)
	{
		ISO7816_t0_rx_poll(&td, 1);

		//if only T=0 is indicated, TCK shall not be sent
		if((i != 1) && ((td & 0x0f) != 0x00)) tck_flag = 1;

		data_rec[i] = td;
		i++;

		if((td & 0x10) == 0x10)
		{
			ISO7816_t0_rx_poll(&data_rec[i], 1);
			i++;
		}

		if((td & 0x20) == 0x20)
		{
			ISO7816_t0_rx_poll(&data_rec[i], 1);
			i++;
		}

		if((td & 0x40) == 0x40)
		{
			ISO7816_t0_rx_poll(&data_rec[i], 1);
			i++;
		}

		if((td & 0x80) == 0x80) flag = 1;
		else flag = 0;
	}

	t0 = data_rec[1];
	l = (t0 & 0x0f) + i;
	ISO7816_t0_rx_poll(&data_rec[i], (l - i));

	if(tck_flag == 1)   // TCK exist
	{
		ISO7816_t0_rx_poll(&data_rec[i], 1); //TCK bytes
		i++;
	}

	return i;
}

UINT16 ISO7816_tpdu_data_sent_T0(SCARD_APDU *scard_apdu)
{
	UINT8 temp_s[1],temp_r[1];
	UINT8 cmd[5];
	UINT8 sw[2];
	UINT8 data_temp[256];
	UINT8 i = 0;
	UINT8 j;
	UINT8 lc = scard_apdu->P3;

	cmd[0] = scard_apdu->CLA;
	cmd[1] = scard_apdu->INS;
	cmd[2] = scard_apdu->P1;
	cmd[3] = scard_apdu->P2;
	cmd[4] = scard_apdu->P3;

	ISO7816_t0_tx_poll(cmd,5);
	if(lc == 0)
		goto L2;
L1:	
	ISO7816_t0_rx_poll(temp_r,1);
	if(temp_r[0] == 0x60)
		goto L1;
	if(((temp_r[0] ^ scard_apdu->INS) == 0xff)||((temp_r[0] ^ scard_apdu->INS) == 0xfe))
	{
		temp_s[0] = scard_apdu->data[i];
		i++;		
		ISO7816_t0_tx_poll(temp_s,1);
		if(i == lc)
			goto L2;
		else
			goto L1;		
	}
	if(((temp_r[0] ^ scard_apdu->INS) == 0x00)||((temp_r[0] ^ scard_apdu->INS) == 0x01))
	{
		for(j=0;j<lc-i;j++)
			data_temp[j] = scard_apdu->data[i+j];
		ISO7816_t0_tx_poll(data_temp,j);
	}
	else if((temp_r[0] & 0x60) == 0x60)
	{
		sw[0] = temp_r[0];
		ISO7816_t0_rx_poll(&sw[1],1);
		goto L3;
	}
		
L2:
	ISO7816_t0_rx_poll(sw,2);
L3:
	return sw[1]*256 + sw[0];
}

UINT16 ISO7816_tpdu_data_rec_T0(SCARD_APDU *scard_apdu)
{
	UINT8 temp_r[1];
	UINT8 cmd[5];
	UINT8 sw[2];
	UINT8 data_temp[256];
	UINT8 i = 0;
	UINT8 j;
	UINT8 le = scard_apdu->P3;

	cmd[0] = scard_apdu->CLA;
	cmd[1] = scard_apdu->INS;
	cmd[2] = scard_apdu->P1;
	cmd[3] = scard_apdu->P2;
	cmd[4] = scard_apdu->P3;

	ISO7816_t0_tx_poll(cmd,5);
	if(le == 0)
		goto L2;
L1:	
	ISO7816_t0_rx_poll(temp_r,1);
	if(temp_r[0] == 0x60)
		goto L1;
	if(((temp_r[0] ^ scard_apdu->INS) == 0xff)||((temp_r[0] ^ scard_apdu->INS) == 0xfe))
	{				
		ISO7816_t0_rx_poll(temp_r,1);
		scard_apdu->data[i] = temp_r[0];
		i++;
		if(i == le)
			goto L2;
		else
			goto L1;		
	}
	if(((temp_r[0] ^ scard_apdu->INS) == 0x00)||((temp_r[0] ^ scard_apdu->INS) == 0x01))
	{		
		ISO7816_t0_rx_poll(data_temp,le-i);
		for(j=i;j<le;j++)
			scard_apdu->data[j] = data_temp[j-i];
	}
	else if((temp_r[0] & 0x60) == 0x60)
	{
		sw[0] = temp_r[0];
		ISO7816_t0_rx_poll(&sw[1],1);
		goto L3;
	}
		
L2:
	ISO7816_t0_rx_poll(sw,2);
L3:
	return sw[1]*256 + sw[0];
}

UINT8 IP_ISO7816_event(CRYPT_ISO7816 *crypt_iso7816)
{
	SCARD_APDU scard_apdu;
	UINT8 TA1, TD1, TA2;
	UINT8 atr_len;
	UINT16 temp;
	int i;

	switch(crypt_iso7816->cmd & CMD_ISO7816_OPMODE_MASK)
	{
		case CMD_ISO7816_RESET:
			ISO7816_master_init();
			
			i = 0;
			while(1)
			{
				if(REG_7816_ISR & (1 << 12))
				{
					break;
				}
				else if(i >= 10000)
				{
					crypt_iso7816->respLen = 0;
					return ISO7816_SUCCEED;
				}
				i++;
			}
			
			ISO7816_master_cold_rst();
			REG_SCU_MUXCTRLD |=	PAD_SCC_MS_CIO_EN;//再配成SCC_MS_SIO
			
			atr_len = ISO7816_atr_rec(crypt_iso7816->respBuf);
			if(atr_len == 0)
			{
				for(i = 0; i < 3; i++)
				{
					ISO7816_master_warm_rst();
					atr_len = ISO7816_atr_rec(crypt_iso7816->respBuf);
					if(atr_len > 0)
					{
						break;
					}
				}
				if(i == 3)
				{
					crypt_iso7816->respLen = 0;
					return ISO7816_SUCCEED;
				}
			}

			//判断格式字符T0的高4bit
			if(crypt_iso7816->respBuf[1] & 0x80)
			{//存在TD1
				 crypt_iso7816->respBuf[1] = (crypt_iso7816->respBuf[1] & 0xF0);
				 temp = CalBitNum(crypt_iso7816->respBuf, 2);
				 
				 //判断TD1
				 TD1 = crypt_iso7816->respBuf[temp+1];
				 if((TD1 & 0x10) == 0x10)
				 {//存在TA2
				 	TA2 = crypt_iso7816->respBuf[temp+2];
					if((TA2 & 0x10) == 0x00)
					{//存在TA1, 使用TA1指定的F和D值,否则使用默认值
						if((crypt_iso7816->respBuf[1] & 0x10) == 0x10)
						{
							TA1 = crypt_iso7816->respBuf[2];
							ISO7816_change_baud_rate(TA1);	
						}								
					}
				}
			}
			
			crypt_iso7816->respLen = atr_len;
			break;
		case CMD_ISO7816_PPS:
			ISO7816_t0_tx_poll(crypt_iso7816->reqBuf, crypt_iso7816->reqLen);			
    		ISO7816_t0_rx_poll(crypt_iso7816->respBuf, crypt_iso7816->reqLen);

			ISO7816_change_baud_rate(crypt_iso7816->respBuf[2]);
			
			crypt_iso7816->respLen = crypt_iso7816->reqLen;
			break;
		case CMD_ISO7816_APDU:
			if((crypt_iso7816->cmd & CMD_ISO7816_PROTOCOL_MASK) == CMD_ISO7816_PROTOCOL_T0)
			{
				switch(crypt_iso7816->reqBuf[1])
				{
					case 0xa4:
						scard_apdu.CLA = crypt_iso7816->reqBuf[0];
						scard_apdu.INS = crypt_iso7816->reqBuf[1];
						scard_apdu.P1 = crypt_iso7816->reqBuf[2];
						scard_apdu.P2 = crypt_iso7816->reqBuf[3];
						scard_apdu.P3 = crypt_iso7816->reqBuf[4];
						scard_apdu.data = &crypt_iso7816->reqBuf[5];
						temp = ISO7816_tpdu_data_sent_T0(&scard_apdu);
						memcpy(crypt_iso7816->respBuf, (UINT8*)&temp, sizeof(temp));
						crypt_iso7816->respLen = 2;
						break;
					case 0x84:
						scard_apdu.CLA = crypt_iso7816->reqBuf[0];
						scard_apdu.INS = crypt_iso7816->reqBuf[1];
						scard_apdu.P1 = crypt_iso7816->reqBuf[2];
						scard_apdu.P2 = crypt_iso7816->reqBuf[3];
						scard_apdu.P3 = crypt_iso7816->reqBuf[4];
						scard_apdu.data = crypt_iso7816->respBuf;
						
						temp = ISO7816_tpdu_data_rec_T0(&scard_apdu);
						if(temp != 0x0090)
						{
							scard_apdu.P3 = 0;
						}
						memcpy(crypt_iso7816->respBuf + scard_apdu.P3, (UINT8*)&temp, sizeof(temp));
						crypt_iso7816->respLen = 2 + scard_apdu.P3;
						break;
					default:
						return ISO7816_FAILED;
				}
			}
			else
			{
				ISO7816_t0_tx_poll(crypt_iso7816->reqBuf, crypt_iso7816->reqLen);
				ISO7816_t0_rx_poll(crypt_iso7816->respBuf, 3);
				ISO7816_t0_rx_poll(crypt_iso7816->respBuf + 3, crypt_iso7816->respBuf[2] + 1);
				crypt_iso7816->respLen = crypt_iso7816->respBuf[2] + 4;
			}

			break;
		default:
			return ISO7816_FAILED;
	}

	return ISO7816_SUCCEED;
}
