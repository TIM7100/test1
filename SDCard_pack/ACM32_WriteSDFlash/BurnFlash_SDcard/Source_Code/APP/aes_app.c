#include "aes_app.h"
#include "HAL_AES.h"

const uint8_t AES_Key[16] = "BURN2026KEY@0123";
const uint8_t AES_Iv[16]  = "0123456789ABCDEF";

/******************************************************************************
 * AES128_CBC_Decrypt - Hardware AES-128-CBC Decryption
 *
 * Upper computer uses Aes.Create() which produces standard byte order
 * (big-endian per 32-bit word). The ciphertext is sent as-is (no byte-swap).
 *
 * With AES_SWAP_ENABLE, the hardware expects key/IV/input in big-endian
 * per 32-bit word (standard AES byte order), and produces output in
 * big-endian per word. No byte-swap operations are needed.
 ******************************************************************************/
void AES128_CBC_Decrypt(uint8_t* pOut, const uint8_t* pIn, uint32_t len, const uint8_t* Key, const uint8_t* IV)
{
    uint32_t block_len;
    uint8_t iv[16];
    uint8_t j;

    if ((len == 0) || (len % 16)) return;

    /* Copy IV to local buffer (HAL_AES_Crypt_U8 may modify IV in CBC mode) */
    for (j = 0; j < 16; j++) iv[j] = IV[j];

    block_len = len / 16;

    /* AES_SWAP_ENABLE: key/IV/data in big-endian per 32-bit word (standard AES byte order)
     * Hardware does the byte-swap internally, input/output remain big-endian. */
    HAL_AES_SetKey_U8((uint8_t*)Key, AES_KEY_128, AES_SWAP_ENABLE);

    /* Hardware AES CBC decryption
     * Input:  big-endian per word (standard AES, matches upper computer)
     * Output: big-endian per word (standard AES, plaintext ready to use) */
    HAL_AES_Crypt_U8((uint8_t*)pIn, pOut, block_len, AES_DECRYPTION, AES_CBC_MODE, iv, AES_NORMAL_MODE);
}