#include "cmd_queue.h"
#include "hmi_driver.h"
#include "cmd_process.h"

extern u16 cmd_addr;   //控件地址

#if IS_CRC16

  static uint16 _crc16 = 0xffff;
  /*!
  *  \brief 添加CRC16校验
  *  \param buffer 待校验的数据
  *  \param n 数据长度，包含CRC16
  *  \param pcrc 校验码
  */
  static void AddCRC16(uint8 *buffer,uint16 n,uint16 *pcrc)
  {
      uint16 i,j,carry_flag,a;

      for (i=0; i<n; i++)
      {
          *pcrc=*pcrc^buffer[i];
          for (j=0; j<8; j++)
          {
              a=*pcrc;
              carry_flag=a&0x0001;
              *pcrc=*pcrc>>1;
              if (carry_flag==1)
                  *pcrc=*pcrc^0xa001;
          }
      }
  }
  /*!
  *  \brief  检查数据是否符合CRC16校验
  *  \param buffer 待校验的数据，末尾存储CRC16
  *  \param n 数据长度，包含CRC16
  *  \return 校验通过返回1，否则返回0
  */
  uint16 CheckCRC16(uint8 *buffer,uint16 n)
  {
      uint16 crc0 = 0x0;
      uint16 crc1 = 0xffff;

      if(n>=2)
      {
          crc0 = ((buffer[n-2]<<8)|buffer[n-1]);
          AddCRC16(buffer,n-2,&crc1);
      }
      return (crc0==crc1);
  }
  /*!
  *  \brief  发送一个字节
  *  \param  c
  */
  void SEND_DATA(uint8 c)
  {
      AddCRC16(&c,1,&_crc16);
      SendChar(c);
  }
  /*!
  *  \brief  帧头
  */
  void BEGIN_CMD()
  {
      SendChar(0XEE);
      _crc16 = 0XFFFF;  //开始计算CRC16
      if(IS_ADDR){SendChar16(cmd_addr);};
  }
  /*!
  *  \brief  帧尾
  */
  void END_CMD()
  {
      u16 crc16 = _crc16;
      SendChar16(crc16);  //发送CRC16
      SendChar32(0xFFFCFFFF); 
  }

#else //NO CRC16

    void SEND_DATA(u8 t)
    {
      SendChar(t);//等待串口发送完毕
    }

    #define SEND_DATA(P) SendChar(P)
    #define BEGIN_CMD() SendChar(0xEE);if(IS_ADDR){Send16(cmd_addr);};  //帧头
    #define END_CMD() Send32(0XFFFCFFFF)    //帧尾

#endif


#if 1 //发送数据

//串口发送16位数据
void SendChar16(u16 t)  
{
  SendChar(t>>8);
  SendChar(t&0xFF);
}
//串口发送32位数据
void SendChar32(u32 t)  
{
  SendChar16(t>>16);
  SendChar16(t&0xFFFF);
}
//串口发送8位数据
void Send8(u8 t)  
{
  SEND_DATA(t);
}
//串口发送16位数据
void Send16(u16 t)  
{
  Send8(t>>8);
  Send8(t);
}
//串口发送32位数据
void Send32(u32 t) 
{
  Send16(t>>16);
  Send16(t);
}
//串口发送送字符串
void SendStr(u8 *str)
{
    while(*str)
    {
        Send8(*str);
        str++;
    }
}
//串口发送送N个字节
void Send8_N(u8 *pData,u16 nDataLen)
{
    uint16 i = 0;
    for (;i<nDataLen;++i)
    {
        Send8(pData[i]);
    }
}
//串口发送送N个16位的数据
void Send16_N(u16 *pData,u16 nDataLen)
{
    uint16 i = 0;
    for (;i<nDataLen;++i)
    {
        Send16(pData[i]);
    }
}

#endif

#if 1 //发送控件指令
//发送握手指令
void Send_HandShake()
{
    BEGIN_CMD(); 
    Send8(0x04);
    END_CMD();
}
//发送重启指令
void Send_HReset()
{
    BEGIN_CMD();
    Send8(0x55);
    END_CMD();
}
//发送页面跳转指令
void Send_SetPage(u16 pid,u16 time)
{
    BEGIN_CMD();
    Send8(0x05);
    Send16(pid);
    Send16(time);
    END_CMD();
}
//发送获取当前页面id指令
void Send_GetPageID()
{
    BEGIN_CMD();
    Send8(0x06);
    END_CMD();
}
//发送设置背光亮度指令
void Send_SetBL(u16 val)
{
    BEGIN_CMD();
    Send8(0x07);
    Send16(val);
    END_CMD();
}
//发送获取背光亮度指令
void Send_GetBL()
{
    BEGIN_CMD();
    Send8(0x08);
    END_CMD();
}
//发送设置波特率指令
void Send_SetBaundRate(u16 val)
{
    BEGIN_CMD();
    Send8(0x09);
    Send16(val);
    END_CMD();
}
//发送设置定时器指令
void Send_SetTimmer(u16 tid, u16 sel, u16 time, u16 en)
{
    BEGIN_CMD();
    Send8(0x0B);
    Send16(tid);
    Send16(sel);
    Send16(time);
    Send16(en);
    END_CMD();
}
//发送保存控件属性指令
void Send_Save()
{
    BEGIN_CMD();
    Send8(0x0C);
    END_CMD();
}
//发送设置延时指令
void Send_SetDelay(u16 val)
{
    BEGIN_CMD();
    Send8(0x0F);
    Send16(val);
    END_CMD();
}
//发送启用休眠定时器指令
void Send_SetSleep(u16 time, u16 bl_val)
{
    BEGIN_CMD();
    Send8(0x10);
    Send16(time);
    Send16(bl_val);
    END_CMD();
}
//发送获取休眠定时器参数指令
void Send_GetSleep()
{
    BEGIN_CMD();
    Send8(0x11);
    END_CMD();
}
//发送设置蜂鸣器时间参数指令
void Send_SetBeep(u16 val)
{
    BEGIN_CMD();
    Send8(0x12);
    Send16(val);
    END_CMD();
}
//发送获蜂鸣器参数指令
void Send_GetBeep()
{
    BEGIN_CMD();
    Send8(0x13);
    END_CMD();
}

//发送设置整型变量指令
void Send_SetVAL(u16 val_id, u32 val)
{
    BEGIN_CMD();
    Send8(0x14);
    Send16(val_id);
    Send32(val);
    END_CMD();
}
//发送获取整型变量的值指令
void Send_GetVAL(u16 val_id)
{
    BEGIN_CMD();
    Send8(0x15);
    Send16(val_id);
    END_CMD();
}

//发送获取字符串变量指令
void Send_GetStrVAL(u16 val_id)
{
    BEGIN_CMD();
    Send8(0x17);
    Send16(val_id);
    END_CMD();
}
//发送设置控件X坐标指令
void Send_SetX(u16 ctr_id, u16 val)
{
    BEGIN_CMD();
    Send8(0x81);
    Send8(0x01);
    Send16(ctr_id);
    Send16(val);
    END_CMD();
}
//发送设置控件Y坐标指令
void Send_SetY(u16 ctr_id, u16 val)
{
    BEGIN_CMD();
    Send8(0x81);
    Send8(0x02);
    Send16(ctr_id);
    Send16(val);
    END_CMD();
}
//发送设置控件W宽度指令
void Send_SetW(u16 ctr_id, u16 val)
{
    BEGIN_CMD();
    Send8(0x81);
    Send8(0x03);
    Send16(ctr_id);
    Send16(val);
    END_CMD();
}
//发送设置控件H高度指令
void Send_SetH(u16 ctr_id, u16 val)
{
    BEGIN_CMD();
    Send8(0x81);
    Send8(0x04);
    Send16(ctr_id);
    Send16(val);
    END_CMD();
}

//发送设置控件的文本指令
void Send_SetText(u16 ctr_id, u8* str, u16 num)
{
    BEGIN_CMD();
    Send8(0x81);
    Send8(0x05);
    Send16(ctr_id);
    Send8_N((u8*)str,num);
    Send8(0);
    END_CMD();
}
//发送设置控件的背景色1指令(24位颜色)
void Send_SetBgColor1(u16 ctr_id, u16 state, u32 val)
{
    BEGIN_CMD();
    Send8(0x81);
    Send8(0x06);
    Send16(ctr_id);
    Send16(state);
    Send32(val);
    END_CMD();
}
//发送设置控件的背景色2指令(24位颜色)
void Send_SetBgColor2(u16 ctr_id, u16 state, u32 val)
{
    BEGIN_CMD();
    Send8(0x81);
    Send8(0x07);
    Send16(ctr_id);
    Send16(state);
    Send32(val);
    END_CMD();
}
//发送设置控件的前景色指令(24位颜色)
void Send_SetFColor(u16 ctr_id,u32 val)
{
    BEGIN_CMD();
    Send8(0x81);
    Send8(0x08);
    Send16(ctr_id);
    Send32(val);
    END_CMD();
}
//发送设置控件的值指令
void Send_SetCtrVAL(u16 ctr_id, u16 val)
{
    BEGIN_CMD();
    Send8(0x81);
    Send8(0x09);
    Send16(ctr_id);
    Send16(val);
    END_CMD();
}
//发送设置控件隐藏指令
void Send_SetHidden(u16 ctr_id, u16 state, u16 offon)
{
    BEGIN_CMD();
    Send8(0x81);
    Send8(0x0A);
    Send16(ctr_id);
    Send16(state);
    Send32(offon);
    END_CMD();
}
//发送获取控件的X坐标指令
void Send_GetX(u16 ctr_id)
{
    BEGIN_CMD();
    Send8(0x82);
    Send8(0x01);
    Send16(ctr_id);
    END_CMD();
}
//发送获取控件的Y坐标指令
void Send_GetY(u16 ctr_id)
{
    BEGIN_CMD();
    Send8(0x82);
    Send8(0x02);
    Send16(ctr_id);
    END_CMD();
}
//发送获取控件的W宽度指令
void Send_GetW(u16 ctr_id)
{
    BEGIN_CMD();
    Send8(0x82);
    Send8(0x03);
    Send16(ctr_id);
    END_CMD();
}
//发送获取控件的H高度指令
void Send_GetH(u16 ctr_id)
{
    BEGIN_CMD();
    Send8(0x82);
    Send8(0x04);
    Send16(ctr_id);
    END_CMD();
}
//发送获取控件的文本内容指令
void Send_GetText(u16 ctr_id)
{
    BEGIN_CMD();
    Send8(0x82);
    Send8(0x05);
    Send16(ctr_id);
    END_CMD();
}
//发送获取控件的背景色1指令
void Send_GetBgColor1(u16 ctr_id, u16 state)
{
    BEGIN_CMD();
    Send8(0x82);
    Send8(0x06);
    Send16(ctr_id);
    Send16(state);
    END_CMD();
}
//发送获取控件的背景色2指令
void Send_GetBgColor2(u16 ctr_id, u16 state)
{
    BEGIN_CMD();
    Send8(0x82);
    Send8(0x07);
    Send16(ctr_id);
    Send16(state);
    END_CMD();
}
//发送获取控件的前景色指令
void Send_GetFColor(u16 ctr_id)
{
    BEGIN_CMD();
    Send8(0x82);
    Send8(0x08);
    Send16(ctr_id);
    END_CMD();
}
//发送获取控件的值指令
void Send_GetCtrVAL(u16 ctr_id)
{
    BEGIN_CMD();
    Send8(0x82);
    Send8(0x09);
    Send16(ctr_id);
    END_CMD();
}


#endif


