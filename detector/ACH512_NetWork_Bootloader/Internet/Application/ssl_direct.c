#include "ssl_direct.h"              //包含需要的头文件
#include "w5500_user_conf.h"

//mbedtls库所需包含头文件
#include "config.h"         //包含需要的头文件
#include "ssl.h"            //包含需要的头文件
#include "entropy.h"        //包含需要的头文件
#include "ctr_drbg.h"       //包含需要的头文件
#include "debug.h"          //包含需要的头文件
#include "entropy_poll.h"   //包含需要的头文件
#include "platform.h"       //包含需要的头文件
#include "net_sockets.h"    //包含需要的头文件
#include "delay.h"
//SSL
int mbedtls_ssl_ret;
const char *pers = "ssl_client1";
mbedtls_entropy_context entropy;
mbedtls_ctr_drbg_context ctr_drbg;
mbedtls_ssl_context ssl;
mbedtls_ssl_config conf;


/**
* @brief   产生随机数
* @param   None
* @retval  None
* @warning None
* @example
**/
int mbedtls_hardware_poll(void *data, unsigned char *output, size_t len, size_t *olen)
{
    unsigned long randomValue = ((rand() * 20) + 1000);
    ((void) data);
    *olen = 0;

    if (len < sizeof(unsigned long))
    {
        return (0);
    }

    memcpy(output, &randomValue, sizeof(unsigned long));
    *olen = sizeof(unsigned long);

    return 0;
}

/**
* @brief   返回时间戳(SSL底层会调用时间验证证书是否过期)
* @param   None
* @retval  None
* @warning None
* @example
**/
//struct tm *lcTime;
//time_t startTime;
//lcTime = localtime (&startTime);
_ARMABI time_t time(time_t *t)
{
//    time_t it;
    if (t)
    {
        return *t;
    }
    else
    {
//        startTime = 0;
//        lcTime = localtime (&startTime);
//        it = mktime(lcTime);
//        return it ;
        return 0;
    }
}

/**
* @brief   自定义网络接收函数(底层会调用此函数获取网络数据)
函数内部需要自己实现,把网络数据拷贝到数据指针(底层要多少数据就拷贝多少数据)
* @param   buf:SSL底层传递过来的数据指针(需要把数据拷贝到此地址)
* @param   len:SSL底层告诉需要的数据个数
* @retval  None
* @warning None
* @example
**/
static int CustomSSLRecv(void *ctx, unsigned char *RxBuf, size_t Len)
{
    return W5500_RxData(RxBuf, Len);
}

/**
* @brief   自定义网络发送函数(内部填写自己的网络数据发送程序)
* @param   None
* @retval  None
* @warning None
* @example
**/
static int CustomSSLSend(void *ctx, const unsigned char *TxBuf, size_t Len)
{
    ctx = ctx;

    //调用自己的TCP发送函数
    W5500_TxData((uint8_t *)TxBuf, Len);

    return (int)Len;
}

/**
* @brief   通过SSL发送数据
* @param   buf:发送的数据地址
* @param   len:发送的数据长度
* @retval  None
* @warning None
* @example
**/
int SSL_SendData(const unsigned char *TxBuf, int Len)
{
    //向服务器发送数据
    return mbedtls_ssl_write(&ssl, TxBuf, Len) ;
}

/**
* @brief   通过SSL接收数据
* @param   buf:接收数据的数组
* @param   len:数组的长度
* @retval  实际返回的数据个数
* @warning None
* @example
**/
int SSL_ReadData(unsigned char *RxBuf, int Len)
{
    //调用SSL函数获取数据
    return mbedtls_ssl_read(&ssl, RxBuf, Len);
}

/**
* @brief   debug
* @param   None
* @retval  None
* @warning None
* @example
**/
//设置debug输出函数
//static void ssl_debug(void *ctx, int level, const char *file, int line, const char *str)
//{
//    u1_printf("%s:%04d: %s\r\n", file, line, str);
//}

/**
* @brief   ssl_init
* @param   None
* @retval  None
* @warning None
* @example
**/
void SSL_Init(void)
{
    mbedtls_ssl_close_notify(&ssl);
    mbedtls_ssl_free(&ssl);
    mbedtls_ssl_config_free(&conf);
    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);
//    mbedtls_debug_set_threshold(1);
    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_init(&conf);
    mbedtls_ctr_drbg_init(&ctr_drbg);
    mbedtls_entropy_init(&entropy);

//    printf("Seeding the random number generator...\r\n");
    //DRBG---->Deterministic Random Bit Generators 伪随机数产生器
    if ((mbedtls_ssl_ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
                           (const unsigned char *) pers,
                           strlen(pers))) != 0)
    {
        printfS(" failed\n  ! mbedtls_ctr_drbg_seed returned %d\r\n", mbedtls_ssl_ret);
    }
    //MBEDTLS_SSL_IS_CLIENT 表示配置为客户端
    //MBEDTLS_SSL_TRANSPORT_STREAM 表示传输方式为TLS
    //设置版本， MBEDTLS_SSL_PRESET_DEFAULT 表示 TLS1.0
    if ((mbedtls_ssl_ret = mbedtls_ssl_config_defaults(&conf,
                           MBEDTLS_SSL_IS_CLIENT,
                           MBEDTLS_SSL_TRANSPORT_STREAM,
                           MBEDTLS_SSL_PRESET_DEFAULT)) != 0)
    {
        printfS(" failed\n  ! mbedtls_ssl_config_defaults returned %d\r\n", mbedtls_ssl_ret);
    }

    //设置数字证书检查模式
    mbedtls_ssl_conf_authmode(&conf, MBEDTLS_SSL_VERIFY_NONE);  //不校验证书
    // 配置随机数生成器的回调函数
    mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);
    // 配置调试回调函数
//    mbedtls_ssl_conf_dbg(&conf, ssl_debug, stdout);
    // 根据conf设置ssl结构
    if ((mbedtls_ssl_ret = mbedtls_ssl_setup(&ssl, &conf)) != 0)
    {
        printfS(" failed\n  ! mbedtls_ssl_setup returned 0x%x\r\n", -mbedtls_ssl_ret);
    }
    // 设置发送和接收接口
    mbedtls_ssl_set_bio(&ssl, NULL, CustomSSLSend, CustomSSLRecv, NULL);
}

/**
* @brief   SSL握手状态(需用轮训调用)
* @param   None
* @retval  0:握手成功; 其它:错误码
* @warning None
* @example
**/
char SSL_HandshakeAwait(void)
{
    int16_t timeout = 10;                       //超时时间为10 * 500ms = 5s

    while (timeout--)                            //等待超时时间到0
    {
        if (mbedtls_ssl_handshake(&ssl) == 0)
        {
            return 0;
        }
        Timer0DelayMs(500);                            //延时50ms
    }

    return 1;
}
