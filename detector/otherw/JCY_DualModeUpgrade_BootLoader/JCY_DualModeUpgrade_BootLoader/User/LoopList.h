#ifndef LOOPLIST_H_
#define LOOPLIST_H_

#ifndef LOOPLIST_C_
#define LOOPLIST_Ex_ extern
#else
#define LOOPLIST_Ex_
#endif

#include "fxx_std.h"
	
#define min(a, b) (a)<(b)?(a):(b)                   ///< 获取最小值


/** 环形缓冲区数据结构 */
typedef struct {
    uint32_t  rbCapacity;//空间大小
    char  *rbHead; //头
    char  *rbTail; //尾
    char  *rbBuff; //数组的首地址
}rb_t;

#define  rb_t_ssl_buff_len (1024*10)
extern rb_t rb_t_ssl;                          //存储环形队列信息的结构体
extern char rb_t_ssl_buff[rb_t_ssl_buff_len];  //存储数据的环形队列Buff

void rbCreate(rb_t *rb,void *Buff,uint32_t BuffLen);//创建或者说初始化环形缓冲区
void rbDelete(rb_t* rb);
int32_t rbCapacity(rb_t *rb);//得到环形大小
int32_t rbCanRead(rb_t *rb);//能读出数据的个数
int32_t rbCanWrite(rb_t *rb);//还剩余的空间
int32_t rbRead(rb_t *rb, void *data, uint32_t count);//读取数据
int32_t rbWrite(rb_t *rb, const void *data, uint32_t count);
int32_t PutData(rb_t *rb ,void *buf, uint32_t len);
int32_t rbWriteByte(rb_t *rb ,u8 buf);
#endif
