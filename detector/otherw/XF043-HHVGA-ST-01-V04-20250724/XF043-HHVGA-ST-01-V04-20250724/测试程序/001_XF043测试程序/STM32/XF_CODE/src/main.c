//==========================================
//主程序文件
//==========================================
#include "stdio.h"
#include "string.h"
#include "ulitity.h"
#include "hmi_user_uart.h"
#include "cmd_queue.h"
#include "cmd_process.h"
#include "hmi_driver.h"
#include "hw_config.h"

//==========================================
#define debug_en  0 //调试模式
//==========================================
//volatile uint32  timer_tick_count = 0;                                               //定时器节拍
//static int32 test_value = 0;                                                         //测试值
uint8  cmd_buffer[CMD_MAX_SIZE];                                                     //指令缓存

static qdata CMD_HEAD = NOTIFY_HEAD_TOUCH;   //默认触摸帧头

static u8 cmd_head = 0;     //指令头
static u8 cmd_type = 0;     //指令类型码
static u8 cmd_type_sub = 0;     //子指令类型
static u16 cmd_page_id= 0;  //页面ID
static u16 cmd_ctrol_id= 0; //控件ID
//static u16 cmd_x= 0;      //X坐标
//static u16 cmd_y= 0;      //Y坐标
static u16 cmd_ctrol_value = 0; //控件值
static u8 cmd_type_pos=1; //指令类型码位置

#if IS_ADDR
  u16 cmd_addr = 0x0000;   //控件地址
#endif

u8 temp_Str[50]={0}; //文本字符buffer

u8 brightValue=100;  //测屏幕亮度，初值100

//==========================================
#if 1 //基础函数
void LED_Init(void) //LED初始化 PC15
{
   GPIO_InitTypeDef  GPIO_InitStructure;
    
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PC端口时钟
    
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				 //LED0-->PC15 端口配置
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
   GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化
   GPIO_SetBits(GPIOC,GPIO_Pin_15);						 //PC15 输出高
}

void Num2Str(u8* str, u16 num, u8 sp, u8 ep)  //数字转为字符
{
    unsigned char index[]= "0123456789";
    char i=ep;
    do
    {
      str[--i] = index[num%10];
      num /= 10;
    }while (i==sp);
}

void AddStr(u8* str1, u8* str2, u8 sp, u8 len)//添加字符
{
  u8 i=0;
  while(i<len)
  {
    str1[sp++]=str2[i];
    i++;
  }
}

void debugshow(u16 n) //调试cmd_buffer数据
{
   int i;
   for(i=0;i<n;i++)
      SendChar(cmd_buffer[i]);
}
#endif
//==========================================
int main()  //主程序入口                                                                         
{    
#if 1   //初始化
    qsize size = 0; 

    //配置时钟                                                                    
    Set_System();                                                                   
    //配置串口中断                                                                
    Interrupts_Config();                                                            
    //配置时钟节拍                                                                
    systicket_init();                                                               
    //串口初始化                                              
    UartInit(115200);                                                               
    
    LED_Init();
    //清空串口接收缓冲区                                                          
    queue_reset();                                                                  

    //延时等待串口屏初始化完毕,必须等待300ms                                      
    delay_ms(300); 
  
    #if IS_ADDR
      cmd_type_pos+=2; //指令类型位置
    #endif

#endif
  
//==========================================
//    Send_SetText(0x0010,(u8*)"pages", 5);  //CRC校验
//    CMD_HEAD = NOTIFY_HEAD_TOUCH;
//      
    while(1)                                                                        
    { 
        size = queue_find_cmd(cmd_buffer,CMD_MAX_SIZE,CMD_HEAD); //从缓冲区中获取一条指令         
        if(size>0)  //接收到指令
        {
#if debug_en  //调试数据
  debugshow(size); //调试cmd_buffer数据
#endif
//==========================================
          cmd_head = cmd_buffer[0]; //获取数据头
          #if IS_ADDR
            cmd_addr = Get_Buff_16(1);   //获取地址
          #endif
          cmd_type = Get_Buff_8(cmd_type_pos);   //获取指令类型
          
//==========================================
          switch(cmd_head)
          {
            case NOTIFY_HEAD_TOUCH: //触摸指令头
              Process_Touch((PCTRL_MSG)cmd_buffer, size);  //触摸指令处理  
              break;
            case NOTIFY_HEAD_RES:  //回发指令头
              Process_Postback((PCTRL_MSG)cmd_buffer, size);   //回发指令处理  
              break;
          }
        } 
    }  
}
//触摸指令处理
void Process_Touch(PCTRL_MSG msg, uint16 size)  //触摸指令处理  
{
    cmd_page_id = Get_Buff_16(cmd_type_pos+1);   //获取页面ID
    cmd_ctrol_id = Get_Buff_16(cmd_type_pos+3);   //获取控件ID
    cmd_ctrol_value = Get_Buff_16(cmd_type_pos+9);  //获取控件值 
  
//==========================================
   switch(cmd_type) //指令类型
   {  
      case 0x06:  //按钮控件
          if(cmd_page_id==0x0000)  //页面ID
          {
              switch(cmd_ctrol_id) //控件ID
              {
                case 0x0001:  //照明灯
                  Touch_LED_ONOFF(cmd_ctrol_value);
                  break;
                case 0x0002:  //彩灯状态
                  Touch_Color_LED_ONOFF(cmd_ctrol_value);
                  break;
                case 0x0005:  //窗帘状态（全开/全关/关开）
                  Touch_Curtain(cmd_ctrol_value);
                  break;
                case 0x0006:  //感应灯，读取当前页面ID
                  Touch_Get_PageID(cmd_ctrol_value);
                  break;
                case 0x0020:  //亮度减少
                  Touch_BL_Down();
                  break;
                 case 0x001F:  //亮度增加
                  Touch_BL_Up();
                  break;
                case 0x0021:  //显示电池状态
                  Touch_Bat_State(cmd_ctrol_value);
                  break;
             }
          }
          break;
      default:
          break;
    }
}
//回发指令处理
void Process_Postback(PCTRL_MSG msg, uint16 size)  //回发指令处理
{
    u8* p_t;
    cmd_type_sub = Get_Buff_8(cmd_type_pos+1);   //获取从指令类型
    cmd_ctrol_id = Get_Buff_16(cmd_type_pos+2);   //获取控件ID
    p_t=msg->param;
    p_t+=cmd_type_pos+3;  //文本内容位置

    //=====================================
    switch(cmd_type) //指令类型
    {  
      case 0x06:  //获取页面ID
        Postback_Get_PageID();
        break;
      case 0x82:  //主指令码为0x82
        switch(cmd_type_sub) //从指令码
        {  
          case 0x05:  //获取控件的文本内容
            switch(cmd_ctrol_id) //控件ID
            {
              case 0x0028:  //控件ID
                Postback_Get_Bat_Vol(p_t);   //获取电池电量
                break;
              case 0x002B:  //控件ID
                Postback_Get_Bat_Power(p_t);   //获取电池总功率
                break;
            }
            break;  //0x05 END
          default:
            break;
        }
        break;  //0x82 END
      default:
        break;
    }
 
  //回发指令处理完，帧头改为触摸通知
  CMD_HEAD = 0xCC;
}
//===============================================
//触摸数据处理
//===============================================
void Touch_LED_ONOFF(u16 val) //控制继电器点亮台灯
{
  if(val) //判断触摸值
  {
    GPIO_SetBits(GPIOC,GPIO_Pin_15);  //PC15 输出高
  }
  else
  {
    GPIO_ResetBits(GPIOC,GPIO_Pin_15);  //PC15 输出低
  }
}
//===========================
void Touch_Color_LED_ONOFF(u16 val) //显示彩灯状态（test/OK切换）
{
  //帧头改为主动发送，无需等待回复通知
  CMD_HEAD=NOTIFY_HEAD_RES;  //主动发送数据帧头
  
  if(val) //判断触摸值
  {
    Send_SetText(0x000E,(u8*)"OK", 2);  //让0x000E控件显示 OK 文本
  }
  else
  {
    Send_SetText(0x000E,(u8*)"Test", 4);  //让0x000E控件显示 Test 文本
  }
  
  //无回发指令，帧头改为触摸通知
  CMD_HEAD = NOTIFY_HEAD_TOUCH;
}
//===========================
void Touch_Curtain(u16 val) //窗帘状态（全开/全关/关开）
{
  //帧头改为主动发送，等待回发通知
  CMD_HEAD=NOTIFY_HEAD_RES;  //主动发送数据帧头
  
 #if debug_en  //调试信息
  Send16(val);  //调试数据   
#endif
  if(val%3==0) //判断触摸值
  {
    Send_SetText(0x000F,(u8*)"OFF", 3); //全关状态
  }
  else if(val%3==1)
  {
    Send_SetText(0x000F,(u8*)"Half", 4); //半关状态
  }
  else if(val%3==2)
  {
    Send_SetText(0x000F,(u8*)"ON", 2); //全开状态
  }
#if debug_en  //调试信息
  AddStr(temp_Str,(u8*)"VAL",0,4); //添加字符
  Num2Str(temp_Str,val,4,5);  //添加数字
  Send_SetText(0x000D,temp_Str, 5); //显示字符
#endif
  
  if(val>=65535)  //如果按钮的值达到65535，则把按钮值设置为0（避免值溢出）
  {
    Send_SetCtrVAL(0x0005, 0);  
  }
 
//无回发指令，帧头改为触摸通知
  CMD_HEAD = NOTIFY_HEAD_TOUCH;
}
//===========================
void Touch_Get_PageID(u16 val) //读取当前页面ID
{
  //帧头改为主动发送，等待回发通知
  CMD_HEAD=NOTIFY_HEAD_RES;  //主动发送数据帧头
 
  if(val%2==1)
  {
    Send_GetPageID(); //获取当前页面
  }
  else
  {
    Send_SetText(0x0010,(u8*)"pages", 5);
  //无回发指令，帧头改为触摸通知
    CMD_HEAD = NOTIFY_HEAD_TOUCH;
  }
}
//===========================
void Touch_Bat_State(u16 val) //电池状态
{
  //帧头改为主动发送，等待回发通知
  CMD_HEAD=NOTIFY_HEAD_RES;  //主动发送数据帧头
  
  if(val%2==0)
  {
    Send_GetText(0x0028); //获取电压label值
  }
  else
  {
    Send_GetText(0x002B); //获取总功率label值
  }
}
//===========================
void Touch_BL_Up() //亮度增加
{
    brightValue++; //亮度值增加
    if(brightValue>=100)
    {
      brightValue=100;
    }
  
#if 1 // debug_en  //调试信息
  Send16(brightValue);  //调试数据   
#endif
}
//===========================
void Touch_BL_Down() //亮度减少
{
   if(brightValue<1)  //最少为0
   {
      brightValue=0;
   }
   else
   {
      brightValue--; //亮度值减少
   }
   
#if 1 // debug_en  //调试信息
  Send16(brightValue);  //调试数据   
#endif
}

//===============================================
//回发数据处理
//===============================================
void Postback_Get_Bat_Vol(u8* p) //获取电池电压label值
{
  Send_SetText(0x002A, p, 3);
}
//===========================
void Postback_Get_Bat_Power(u8* p) //获取电池总功率label值
{
  Send_SetText(0x002A, p, 5);
}
//===========================
void Postback_Get_PageID() //获取当前页面ID
{
  u16 pageid=Get_Buff_16(cmd_type_pos+1); //获取当前页面ID
  AddStr(temp_Str,(u8*)"Page",0,4); //添加字符
  Num2Str(temp_Str,pageid,5,6);  //添加数字

  Send_SetText(0x0010,temp_Str, 6); //显示字符
}


