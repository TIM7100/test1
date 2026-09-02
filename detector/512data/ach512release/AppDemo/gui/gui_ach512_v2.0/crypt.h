#ifndef _CRYPT_H_
#define _CRYPT_H_

#define USB_OPCODE_CRYPT    0xF8
#define USB_STATUS_ERROR    0xFFFFFFFF

enum ip_code {USBC = 0, SM1, SM4, SSF33, AES, DES, SM2, SM3, RSA, HRNG, ISO7816};

typedef struct _crypt_request
{
	UCHAR	IP_code;
	UCHAR	cmd_code;
	USHORT	key_length;		// in byte
	UINT	crypt_length;	// in byte
} CRYPT_REQUEST,*PCRYPT_REQUEST;

#define SWAP(_x_)   ((((_x_) & 0xFF) << 8) | (((_x_) >> 8) & 0xFF))

#define SwapWord(_a_)	(((_a_) >> 24) | (((_a_) >> 8) & 0x0000ff00) | (((_a_) << 8) & 0x00ff0000) | (((_a_) << 24) & 0xff000000))

#endif