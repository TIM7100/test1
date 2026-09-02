#ifndef __AES_H
#define __AES_H

#include "sys.h"

// 需要与上位机 Form1.cs 中 AES_KEY / AES_IV 一致
static const u8 AES_Key[16] = "BURN2026KEY@0123";   // 16字节
static const u8 AES_Iv[16]  = "0123456789ABCDEF";


// AES-128-CBC 解密(len 必须为16的整数倍),结果写 pOut
// 每次调用处理一整批,如需分扇区流式处理,见示例中 prev 传递方式
void AES128_CBC_Decrypt(u8* pOut, const u8* pIn, u32 len, const u8* Key, const u8* IV);

// 解密 + PKCS7去填充,返回明文实际长度(非法填充返回 len)
u32 AES128_CBC_Decrypt_PKCS7(u8* pOut, const u8* pIn, u32 len, const u8* Key, const u8* IV);

#endif
