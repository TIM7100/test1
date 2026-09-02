#ifndef __SSL_DIRECT_H
#define __SSL_DIRECT_H

#include "fxx_std.h"
//#include "LoopList.h"

//#define  rb_t_ssl_buff_len (1024*12)

//extern rb_t rb_t_ssl;                          //存储环形队列信息的结构体
//extern char rb_t_ssl_buff[rb_t_ssl_buff_len];  //存储数据的环形队列Buff

/**
* @brief   ssl_init
* @param   None
* @retval  None
* @warning None
* @example
**/
void SSL_Init(void);

/**
* @brief   SSL握手状态(需用轮训调用)
* @param   None
* @retval  0:握手成功; 其它:错误码
* @warning None
* @example
**/
char SSL_HandshakeAwait(void);

/**
* @brief   通过SSL发送数据
* @param   buf:发送的数据地址
* @param   len:发送的数据长度
* @retval  None
* @warning None
* @example
**/
int SSL_SendData(const unsigned char *TxBuf, int Len);

/**
* @brief   通过SSL接收数据
* @param   buf:接收数据的数组
* @param   len:数组的长度
* @retval  实际返回的数据个数
* @warning None
* @example
**/
int SSL_ReadData(unsigned char *RxBuf, int Len);

#endif
