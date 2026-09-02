
#ifndef _CMD_QUEUE
#define _CMD_QUEUE
#include "hmi_driver.h"

typedef unsigned char qdata;
typedef unsigned short qsize;

//#define CMD_HEAD1 0XCC;                                                  //帧头
#define CMD_TAIL 0XFFFCFFFF                                            //帧尾

#define CMD_MAX_SIZE 200           // 单条指令大小，根据需要调整，尽量设置大一些
#define QUEUE_MAX_SIZE 512        // 指令接收缓冲区大小，根据需要调整，尽量设置大一些

/*! 
*  \brief  清空指令数据
*/
extern void queue_reset(void);

/*! 
* \brief  添加指令数据
* \detial 串口接收的数据，通过此函数放入指令队列 
*  \param  _data 指令数据
*/
extern void queue_push(qdata _data);

/*! 
*  \brief  从指令队列中取出一条完整的指令
*  \param  cmd 指令接收缓存区
*  \param  buf_len 指令接收缓存区大小
*  \return  指令长度，0表示队列中无完整指令
*/
extern qsize queue_find_cmd(qdata *cmd,qsize buf_len,qdata cmd_head);

#endif

