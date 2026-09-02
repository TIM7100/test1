/***********************************************************************
 * Filename    : app.c
 * Description : app source file
 * Author(s)   : cc  
 * version     : V1.0
 * Modify date : 2019-09-23
 ***********************************************************************/
#include  "app.h"
#include <string.h>

#define SM3_SWAP32(a)       ((a<<24)|((a&0x0000ff00)<<8)|((a&0x00ff0000)>>8)|(a>>24))

void swap(UINT32 *indata,UINT32 len)
{
	UINT32 i;
	for(i = 0; i < len; i ++)
	{
		indata[i] = SM3_SWAP32(indata[i]);	
	}
}


/************************************************************************
 * function   : delay
 * Description: delay for a while.  
 * input : 
 *         count: count to decrease 
 * return: none 
 ************************************************************************/
void delay(UINT32 count)
{
	volatile UINT32 delay_count;

	delay_count = count; 

	while(delay_count--);  
}

UINT32 aes_ecb_test(void)  //ECB Test
{    
	UINT32 mode;
	UINT32 i;
	UINT32 result[8];
	UINT32 blk_len;
	
	UINT32 key_ecb_128[4]     = {0xe04ad1a4, 0x65fe706b, 0x80cf2960, 0x5da2636a};  // key_ecb    = 0xe04ad1a4 65fe706b 80cf2960 5da2636a
	UINT32 iv_ecb_128[4]      = {0x00000000, 0x00000000, 0x00000000, 0x00000000};  // iv_ecb     = 0x0;
	UINT32 plain_ecb_128[4]   = {0xc42821e1, 0x3ac9b622, 0x72304ad5, 0xe5f5937d};  // plain_ecb  = 0xc42821e1 3ac9b622 72304ad5 e5f5937d
	UINT32 cipher_ecb_128[4]  = {0x1f4f49e4, 0xfb96cca5, 0xa309032f, 0xb1a46dde};  // cipher_ecb = 0x1f4f49e4 fb96cca5 a309032f b1a46dde

	UINT32 key_ecb_192[6]     = {0xe04ad1a4, 0x65fe706b, 0x80cf2960, 0x5da2636a, 0xc42821e1, 0x3ac9b622};  // key_ecb    = 0xe04ad1a4 65fe706b 80cf2960 5da2636a c42821e1 3ac9b622
	UINT32 iv_ecb_192[4]      = {0x00000000, 0x00000000, 0x00000000, 0x00000000};  // iv_ecb     = 0x0;
	UINT32 plain_ecb_192[4]   = {0x72304ad5, 0xe5f5937d, 0xf38d8adf, 0x367b0c53};  // plain_ecb  = 0x72304ad5 e5f5937d f38d8adf 367b0c53
	UINT32 cipher_ecb_192[4]  = {0xc8f26de1, 0x68a21c9d, 0x21323441, 0x9e11db4e};  // cipher_ecb = 0xc8f26de1 68a21c9d 21323441 9e11db4e

	UINT32 key_ecb_256[8]     = {0xe04ad1a4, 0x65fe706b, 0x80cf2960, 0x5da2636a, 0xc42821e1, 0x3ac9b622, 0x72304ad5, 0xe5f5937d};  // key_ecb    = 0xe04ad1a4 65fe706b 80cf2960 5da2636a c42821e1 3ac9b622 72304ad5 e5f5937d
	UINT32 iv_ecb_256[4]      = {0x00000000, 0x00000000, 0x00000000, 0x00000000};  // iv_ecb     = 0x0;
	UINT32 plain_ecb_256[4]   = {0xf38d8adf, 0x367b0c53, 0x1d273eb7, 0x60632039};  // plain_ecb  = 0xf38d8adf 367b0c53 1d273eb7 60632039
	UINT32 cipher_ecb_256[4]  = {0xaddf70c6, 0x3034834d, 0x5c3ad484, 0xebbf7ed0};  // cipher_ecb = 0xaddf70c6 3034834d 5c3ad484 ebbf7ed0


    //-----------------------    ECB mode (no swap)   ---------------------
//  128
	mode = AES_NORMAL_MODE;
//  mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_ecb_128) / 16;
	HAL_AES_SetKey(key_ecb_128, AES_KEY_128, AES_SWAP_DISABLE);	  //128bit key
	HAL_AES_Crypt(plain_ecb_128, result, blk_len, AES_ENCRYPTION, AES_ECB_MODE, iv_ecb_128, mode);

	for( i = 0; i < (blk_len*4); i ++)
	{
		if(result[i] != cipher_ecb_128[i])
		{
			return 1;
		}
	}    

	HAL_AES_Crypt(cipher_ecb_128, result, blk_len, AES_DECRYPTION,AES_ECB_MODE, iv_ecb_128, mode);
	for( i = 0; i < (blk_len*4); i ++)
	{
		if(result[i] != plain_ecb_128[i])
		{
			return 1;
		}
	}

//  192
	mode = AES_NORMAL_MODE;
//  mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_ecb_192) / 16;

	HAL_AES_SetKey(key_ecb_192, AES_KEY_192, AES_SWAP_DISABLE);	  //192bit key
	HAL_AES_Crypt(plain_ecb_192, result, blk_len, AES_ENCRYPTION, AES_ECB_MODE, iv_ecb_192, mode);

	for( i = 0; i < (blk_len*4); i ++)
	{
		if(result[i] != cipher_ecb_192[i])
		{
			return 1;
		}
	}    

	HAL_AES_Crypt(cipher_ecb_192, result, blk_len, AES_DECRYPTION,AES_ECB_MODE, iv_ecb_192, mode);
	for( i = 0; i < (blk_len*4); i ++)
	{
		if(result[i] != plain_ecb_192[i])
		{
			return 1;
		}
	}

//  256
	mode = AES_NORMAL_MODE;
//  mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_ecb_256) / 16;

	HAL_AES_SetKey(key_ecb_256, AES_KEY_256, AES_SWAP_DISABLE);	  //256bit key
	HAL_AES_Crypt(plain_ecb_256, result, blk_len, AES_ENCRYPTION, AES_ECB_MODE, iv_ecb_256, mode);

	for( i = 0; i < (blk_len*4); i ++)
	{
		if(result[i] != cipher_ecb_256[i])
		{
			return 1;
		}
	}    

	HAL_AES_Crypt(cipher_ecb_256, result, blk_len, AES_DECRYPTION,AES_ECB_MODE, iv_ecb_256, mode);
	for( i = 0; i < (blk_len*4); i ++)
	{
		if(result[i] != plain_ecb_256[i])
		{
			return 1;
		}
	}

	//-----------------------   ECB mode (swap)   ---------------------
    mode = AES_NORMAL_MODE;
//	mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_ecb_128) / 16;
																			 
	swap(key_ecb_128,4);            //key_ecb[0] = 0xa4d14ae0,...,key_ecb[3] = 0x6a63a25d
	swap(iv_ecb_128,4);
	swap(plain_ecb_128,blk_len*4);
	swap(cipher_ecb_128,blk_len*4);

	HAL_AES_SetKey(key_ecb_128, AES_KEY_128, AES_SWAP_ENABLE);	  //128bit key
	HAL_AES_Crypt(plain_ecb_128, result, blk_len, AES_ENCRYPTION, AES_ECB_MODE, iv_ecb_128, mode);

	for( i = 0; i < (blk_len*4); i ++)
	{
		if(result[i] != cipher_ecb_128[i])
		{
			return 1;
		}
	}    

	HAL_AES_Crypt(cipher_ecb_128, result, blk_len, AES_DECRYPTION,AES_ECB_MODE, iv_ecb_128, mode);
	for( i = 0; i < (blk_len*4); i ++)
	{
		if(result[i] != plain_ecb_128[i])
		{
			return 1;
		}
	}		
	
	swap(key_ecb_128,4);            //key_ecb[0] = 0xa4d14ae0,...,key_ecb[3] = 0x6a63a25d
	swap(iv_ecb_128,4);
	swap(plain_ecb_128,blk_len*4);
	swap(cipher_ecb_128,blk_len*4);
		
    mode = AES_NORMAL_MODE;
//	mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_ecb_256) / 16;
																			 
	swap(key_ecb_256,8);            //key_ecb[0] = 0xa4d14ae0,...,key_ecb[3] = 0x6a63a25d
	swap(iv_ecb_256,4);
	swap(plain_ecb_256,blk_len*4);
	swap(cipher_ecb_256,blk_len*4);

	HAL_AES_SetKey(key_ecb_256, AES_KEY_256, AES_SWAP_ENABLE);	  //128bit key
	HAL_AES_Crypt(plain_ecb_256, result, blk_len, AES_ENCRYPTION, AES_ECB_MODE, iv_ecb_256, mode);

	for( i = 0; i < (blk_len*4); i ++)
	{
		if(result[i] != cipher_ecb_256[i])
		{
			return 1;
		}
	}    

	HAL_AES_Crypt(cipher_ecb_256, result, blk_len, AES_DECRYPTION,AES_ECB_MODE, iv_ecb_256, mode);
	for( i = 0; i < (blk_len*4); i ++)
	{
		if(result[i] != plain_ecb_256[i])
		{
			return 1;
		}
	}	
	swap(key_ecb_256,8);            //key_ecb[0] = 0xa4d14ae0,...,key_ecb[3] = 0x6a63a25d
	swap(iv_ecb_256,4);
	swap(plain_ecb_256,blk_len*4);
	swap(cipher_ecb_256,blk_len*4);
	
    //-------------------------security mode ---------------------
   
	//-----------------------    ECB mode (no swap)   ---------------------
//  128
//	mode = AES_NORMAL_MODE;
    mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_ecb_128) / 16;
	HAL_AES_SetKey(key_ecb_128, AES_KEY_128, AES_SWAP_DISABLE);	  //128bit key
	HAL_AES_Crypt(plain_ecb_128, result, blk_len, AES_ENCRYPTION, AES_ECB_MODE, iv_ecb_128, mode);

	for( i = 0; i < (blk_len*4); i ++)
	{
		if(result[i] != cipher_ecb_128[i])
		{
			return 1;
		}
	}    

	HAL_AES_Crypt(cipher_ecb_128, result, blk_len, AES_DECRYPTION,AES_ECB_MODE, iv_ecb_128, mode);
	for( i = 0; i < (blk_len*4); i ++)
	{
		if(result[i] != plain_ecb_128[i])
		{
			return 1;
		}
	}

//  192
//	mode = AES_NORMAL_MODE;
    mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_ecb_192) / 16;

	HAL_AES_SetKey(key_ecb_192, AES_KEY_192, AES_SWAP_DISABLE);	  //192bit key
	HAL_AES_Crypt(plain_ecb_192, result, blk_len, AES_ENCRYPTION, AES_ECB_MODE, iv_ecb_192, mode);

	for( i = 0; i < (blk_len*4); i ++)
	{
		if(result[i] != cipher_ecb_192[i])
		{
			return 1;
		}
	}    

	HAL_AES_Crypt(cipher_ecb_192, result, blk_len, AES_DECRYPTION,AES_ECB_MODE, iv_ecb_192, mode);
	for( i = 0; i < (blk_len*4); i ++)
	{
		if(result[i] != plain_ecb_192[i])
		{
			return 1;
		}
	}

//  256
//	mode = AES_NORMAL_MODE;
    mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_ecb_256) / 16;

	HAL_AES_SetKey(key_ecb_256, AES_KEY_256, AES_SWAP_DISABLE);	  //256bit key
	HAL_AES_Crypt(plain_ecb_256, result, blk_len, AES_ENCRYPTION, AES_ECB_MODE, iv_ecb_256, mode);

	for( i = 0; i < (blk_len*4); i ++)
	{
		if(result[i] != cipher_ecb_256[i])
		{
			return 1;
		}
	}    

	HAL_AES_Crypt(cipher_ecb_256, result, blk_len, AES_DECRYPTION,AES_ECB_MODE, iv_ecb_256, mode);
	for( i = 0; i < (blk_len*4); i ++)
	{
		if(result[i] != plain_ecb_256[i])
		{
			return 1;
		}
	}

	//-----------------------   ECB mode (swap)   ---------------------
//  mode = AES_NORMAL_MODE;
	mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_ecb_128) / 16;
																			 
	swap(key_ecb_128,4);            //key_ecb[0] = 0xa4d14ae0,...,key_ecb[3] = 0x6a63a25d
	swap(iv_ecb_128,4);
	swap(plain_ecb_128,blk_len*4);
	swap(cipher_ecb_128,blk_len*4);

	HAL_AES_SetKey(key_ecb_128, AES_KEY_128, AES_SWAP_ENABLE);	  //128bit key
	HAL_AES_Crypt(plain_ecb_128, result, blk_len, AES_ENCRYPTION, AES_ECB_MODE, iv_ecb_128, mode);

	for( i = 0; i < (blk_len*4); i ++)
	{
		if(result[i] != cipher_ecb_128[i])
		{
			return 1;
		}
	}    

	HAL_AES_Crypt(cipher_ecb_128, result, blk_len, AES_DECRYPTION,AES_ECB_MODE, iv_ecb_128, mode);
	for( i = 0; i < (blk_len*4); i ++)
	{
		if(result[i] != plain_ecb_128[i])
		{
			return 1;
		}
	}		
//  mode = AES_NORMAL_MODE;
	mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_ecb_256) / 16;
																			 
	swap(key_ecb_256,8);            //key_ecb[0] = 0xa4d14ae0,...,key_ecb[3] = 0x6a63a25d
	swap(iv_ecb_256,4);
	swap(plain_ecb_256,blk_len*4);
	swap(cipher_ecb_256,blk_len*4);

	HAL_AES_SetKey(key_ecb_256, AES_KEY_256, AES_SWAP_ENABLE);	  //128bit key
	HAL_AES_Crypt(plain_ecb_256, result, blk_len, AES_ENCRYPTION, AES_ECB_MODE, iv_ecb_256, mode);

	for( i = 0; i < (blk_len*4); i ++)
	{
		if(result[i] != cipher_ecb_256[i])
		{
			return 1;
		}
	}    

	HAL_AES_Crypt(cipher_ecb_256, result, blk_len, AES_DECRYPTION,AES_ECB_MODE, iv_ecb_256, mode);
	for( i = 0; i < (blk_len*4); i ++)
	{
		if(result[i] != plain_ecb_256[i])
		{
			return 1;
		}
	}	

	return 0;
}

UINT32 aes_cbc_test(void)   //CBC Test
{
	UINT32 mode;
	UINT32 i;
	UINT32 result[20];
	UINT32 blk_len;

//128	
	UINT32 key_cbc_128[4]     = {0xe04ad1a4, 0x65fe706b, 0x80cf2960, 0x5da2636a};  //key_cbc    = 0xe04ad1a4 65fe706b 80cf2960 5da2636a
	UINT32 iv_cbc_128[4]      = {0xc42821e1, 0x3ac9b622, 0x72304ad5, 0xe5f5937d};  //iv_cbc     = 0xc42821e1 3ac9b622 72304ad5 e5f5937d  
//  plain_cbc  = 0xf38d8adf 367b0c53 1d273eb7 60632039...
	UINT32 plain_cbc_128[20]   = {0xf38d8adf, 0x367b0c53, 0x1d273eb7, 0x60632039, 
							 0x7e314c2f, 0xa1ab5e18, 0x79aa587e, 0xb834e5de, 
							 0x51d45354, 0xafcebfd6, 0x7a9cd97c, 0x5be67590, 
							 0x40f27cfd, 0xc60d77f7, 0xf923668d, 0xb2fde000, 
							 0x20020634, 0x3fee8224, 0x097ed2f3, 0x4f872281};  
//  cipher_cbc = 0x099ed143 4af12194 c2cb0351 075ea970...
	UINT32 cipher_cbc_128[20]  = {0x099ed143, 0x4af12194, 0xc2cb0351, 0x075ea970, 
							  0x59c1e350, 0x9c5bcc75, 0x34503402, 0xe1ad79a9, 
							  0xae2bd3d2, 0x4ec9e23d, 0x35162f55, 0x993a1b83, 
							  0x35779406, 0x00d30e40, 0xc28639a1, 0x4ea39e14, 
							 0x5a94dd9d, 0x80adf249, 0x863da72a, 0x71021e78};  

//  192
	UINT32 key_cbc_192[6]     = {0xe04ad1a4, 0x65fe706b, 0x80cf2960, 0x5da2636a, 0xc42821e1, 0x3ac9b622};  // key_ecb    = 0xe04ad1a4 65fe706b 80cf2960 5da2636a c42821e1 3ac9b622
	UINT32 iv_cbc_192[4]      = {0x72304ad5, 0xe5f5937d, 0xf38d8adf, 0x367b0c53};  //iv_cbc    = 0x72304ad5 e5f5937d f38d8adf 367b0c53
	UINT32 plain_cbc_192[20]   = {0x1d273eb7, 0x60632039, 0x7e314c2f, 0xa1ab5e18, 
							  0x79aa587e, 0xb834e5de, 0x51d45354, 0xafcebfd6, 
							  0x7a9cd97c, 0x5be67590, 0x40f27cfd, 0xc60d77f7, 
							  0xf923668d, 0xb2fde000, 0x20020634, 0x3fee8224, 
							  0x097ed2f3, 0x4f872281, 0x4329f38c, 0x6c564821}; //plain_cbc  = 0x1d273eb7 60632039 7e314c2f a1ab5e18...
	UINT32 cipher_cbc_192[20]  = {0x4c9abdee, 0x35097f17, 0x37de6468, 0x0a14af58, 
							  0x20e48235, 0x31a46420, 0x93a283f4, 0x7e982a45, 
							  0x9e15fd81, 0xf80f2328, 0x0701d24d, 0x4c7bcb79, 
							  0xbcfd3d10, 0xdc3dceb2, 0x800fa9d1, 0xadc513f9, 
							  0xfa161563, 0x3c65e394, 0xbad169b6, 0xac2f04cd}; //cipher_cbc = 0x4c9abdee 35097f17 37de6468 0a14af58...

//  256
	UINT32 key_cbc_256[8]     = {0xe04ad1a4, 0x65fe706b, 0x80cf2960, 0x5da2636a, 0xc42821e1, 0x3ac9b622, 0x72304ad5, 0xe5f5937d};  // key_ecb    = 0xe04ad1a4 65fe706b 80cf2960 5da2636a c42821e1 3ac9b622 72304ad5 e5f5937d
	UINT32 iv_cbc_256[4]      = {0xf38d8adf, 0x367b0c53, 0x1d273eb7, 0x60632039};  // iv_cbc    = 0xf38d8adf 367b0c53 1d273eb7 60632039;
	UINT32 plain_cbc_256[20]   = {0x7e314c2f, 0xa1ab5e18, 0x79aa587e, 0xb834e5de,
							 0x51d45354, 0xafcebfd6, 0x7a9cd97c, 0x5be67590, 
							 0x40f27cfd, 0xc60d77f7, 0xf923668d, 0xb2fde000,
							 0x20020634, 0x3fee8224, 0x097ed2f3, 0x4f872281,
							 0x4329f38c, 0x6c564821, 0x6e897e46, 0x25ac03f3};  // plain_cbc  = 0x7e314c2f a1ab5e18 79aa587e b834e5de...
	UINT32 cipher_cbc_256[20]  = {0xc8324f39, 0xcf6969a7, 0x35f97dd9, 0x34a90318,
							 0x72866a00, 0x2fd8e17d, 0xf8003085, 0x4e796949,
							 0xcd70af28, 0xe13be0d2, 0x2d613def, 0x8da350f6,
							 0x7107db84, 0x1c3f5585, 0xc77b89dc, 0x4ccd5abc,
							 0x307463b8, 0x5098659e, 0xdd3c2694, 0xedd32cdb};  // cipher_cbc = 0xc8324f39 cf6969a7 35f97dd9 34a90318...


    //-----------------------    CBC mode (no swap)   ---------------------
//  128
	mode = AES_NORMAL_MODE;
//  mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_cbc_128) / 16;
		
    HAL_AES_SetKey(key_cbc_128, AES_KEY_128, AES_SWAP_DISABLE);	  //128bit key
    HAL_AES_Crypt(plain_cbc_128, result, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_128, mode);
    
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(result[i] != cipher_cbc_128[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt(cipher_cbc_128, result, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_128, mode);
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(result[i] != plain_cbc_128[i])
    	{
    		return 1;
    	}
    }

//  192
	mode = AES_NORMAL_MODE;
//  mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_cbc_192) / 16;
		
    HAL_AES_SetKey(key_cbc_192, AES_KEY_192, AES_SWAP_DISABLE);	  //192bit key
    HAL_AES_Crypt(plain_cbc_192, result, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_192, mode);
    
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(result[i] != cipher_cbc_192[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt(cipher_cbc_192, result, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_192, mode);
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(result[i] != plain_cbc_192[i])
    	{
    		return 1;
    	}
    }

//  256
	mode = AES_NORMAL_MODE;
//  mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_cbc_256) / 16;
		
    HAL_AES_SetKey(key_cbc_256, AES_KEY_256, AES_SWAP_DISABLE);	  //256bit key
    HAL_AES_Crypt(plain_cbc_256, result, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_256, mode);
    
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(result[i] != cipher_cbc_256[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt(cipher_cbc_256, result, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_256, mode);
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(result[i] != plain_cbc_256[i])
    	{
    		return 1;
    	}
    }

  //-----------------------    CBC mode (swap)   ---------------------
	mode = AES_NORMAL_MODE;
//  mode = AES_SECURITY_MODE;
                                             
	blk_len = sizeof(plain_cbc_128) / 16;

	swap(key_cbc_128,4);            //key_ecb[0] = 0xa4d14ae0,...,key_ecb[3] = 0x6a63a25d
	swap(iv_cbc_128,4);
	swap(plain_cbc_128,blk_len*4);
	swap(cipher_cbc_128,blk_len*4);	

    HAL_AES_SetKey(key_cbc_128, AES_KEY_128, AES_SWAP_ENABLE);	//128bit key
    HAL_AES_Crypt(plain_cbc_128, result, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_128, mode);
    
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(result[i] != cipher_cbc_128[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt(cipher_cbc_128, result, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_128, mode);
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(result[i] != plain_cbc_128[i])
    	{
    		return 1;
    	}
    }   
	swap(key_cbc_128,4);            //key_ecb[0] = 0xa4d14ae0,...,key_ecb[3] = 0x6a63a25d
	swap(iv_cbc_128,4);
	swap(plain_cbc_128,blk_len*4);
	swap(cipher_cbc_128,blk_len*4);	
		
	mode = AES_NORMAL_MODE;
//  mode = AES_SECURITY_MODE;
                                             
	blk_len = sizeof(plain_cbc_256) / 16;

	swap(key_cbc_256,8);            //key_ecb[0] = 0xa4d14ae0,...,key_ecb[3] = 0x6a63a25d
	swap(iv_cbc_256,4);
	swap(plain_cbc_256,blk_len*4);
	swap(cipher_cbc_256,blk_len*4);	

    HAL_AES_SetKey(key_cbc_256, AES_KEY_256, AES_SWAP_ENABLE);	//128bit key
    HAL_AES_Crypt(plain_cbc_256, result, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_256, mode);
    
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(result[i] != cipher_cbc_256[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt(cipher_cbc_256, result, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_256, mode);
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(result[i] != plain_cbc_256[i])
    	{
    		return 1;
    	}
    }  
	swap(key_cbc_256,8);            //key_ecb[0] = 0xa4d14ae0,...,key_ecb[3] = 0x6a63a25d
	swap(iv_cbc_256,4);
	swap(plain_cbc_256,blk_len*4);
	swap(cipher_cbc_256,blk_len*4);		
		
	//--------------------security-------------------
	//-----------------------    CBC mode (no swap)   ---------------------
//  128
//	mode = AES_NORMAL_MODE;
	mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_cbc_128) / 16;
		
    HAL_AES_SetKey(key_cbc_128, AES_KEY_128, AES_SWAP_DISABLE);	  //128bit key
    HAL_AES_Crypt(plain_cbc_128, result, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_128, mode);
    
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(result[i] != cipher_cbc_128[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt(cipher_cbc_128, result, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_128, mode);
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(result[i] != plain_cbc_128[i])
    	{
    		return 1;
    	}
    }

//  192
//	mode = AES_NORMAL_MODE;
	mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_cbc_192) / 16;
		
    HAL_AES_SetKey(key_cbc_192, AES_KEY_192, AES_SWAP_DISABLE);	  //192bit key
    HAL_AES_Crypt(plain_cbc_192, result, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_192, mode);
    
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(result[i] != cipher_cbc_192[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt(cipher_cbc_192, result, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_192, mode);
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(result[i] != plain_cbc_192[i])
    	{
    		return 1;
    	}
    }

//  256
//	mode = AES_NORMAL_MODE;
    mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_cbc_256) / 16;
		
    HAL_AES_SetKey(key_cbc_256, AES_KEY_256, AES_SWAP_DISABLE);	  //256bit key
    HAL_AES_Crypt(plain_cbc_256, result, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_256, mode);
    
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(result[i] != cipher_cbc_256[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt(cipher_cbc_256, result, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_256, mode);
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(result[i] != plain_cbc_256[i])
    	{
    		return 1;
    	}
    }

  //-----------------------    CBC mode (swap)   ---------------------
//	mode = AES_NORMAL_MODE;
    mode = AES_SECURITY_MODE;
                                             
	blk_len = sizeof(plain_cbc_128) / 16;

	swap(key_cbc_128,4);            //key_ecb[0] = 0xa4d14ae0,...,key_ecb[3] = 0x6a63a25d
	swap(iv_cbc_128,4);
	swap(plain_cbc_128,blk_len*4);
	swap(cipher_cbc_128,blk_len*4);	

    HAL_AES_SetKey(key_cbc_128, AES_KEY_128, AES_SWAP_ENABLE);	//128bit key
    HAL_AES_Crypt(plain_cbc_128, result, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_128, mode);
    
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(result[i] != cipher_cbc_128[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt(cipher_cbc_128, result, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_128, mode);
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(result[i] != plain_cbc_128[i])
    	{
    		return 1;
    	}
    }        
//	mode = AES_NORMAL_MODE;
    mode = AES_SECURITY_MODE;
                                             
	blk_len = sizeof(plain_cbc_256) / 16;

	swap(key_cbc_256,8);            //key_ecb[0] = 0xa4d14ae0,...,key_ecb[3] = 0x6a63a25d
	swap(iv_cbc_256,4);
	swap(plain_cbc_256,blk_len*4);
	swap(cipher_cbc_256,blk_len*4);	

    HAL_AES_SetKey(key_cbc_256, AES_KEY_256, AES_SWAP_ENABLE);	//128bit key
    HAL_AES_Crypt(plain_cbc_256, result, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_256, mode);
    
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(result[i] != cipher_cbc_256[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt(cipher_cbc_256, result, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_256, mode);
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(result[i] != plain_cbc_256[i])
    	{
    		return 1;
    	}
    }  
		
	//-----------------------   End of  CBC mode   ---------------------
	return 0;
}



UINT32 aes_ecb_test_u8(void)  //ECB Test
{    
	UINT32 mode;
	UINT32 i;
//	UINT32 result[8];
	uint8_t result[32];

	UINT32 blk_len;
	
//	UINT32 key_ecb_128[4]     = {0xe04ad1a4, 0x65fe706b, 0x80cf2960, 0x5da2636a};  // key_ecb    = 0xe04ad1a4 65fe706b 80cf2960 5da2636a
//	UINT32 iv_ecb_128[4]      = {0x00000000, 0x00000000, 0x00000000, 0x00000000};  // iv_ecb     = 0x0;
//	UINT32 plain_ecb_128[4]   = {0xc42821e1, 0x3ac9b622, 0x72304ad5, 0xe5f5937d};  // plain_ecb  = 0xc42821e1 3ac9b622 72304ad5 e5f5937d
//	UINT32 cipher_ecb_128[4]  = {0x1f4f49e4, 0xfb96cca5, 0xa309032f, 0xb1a46dde};  // cipher_ecb = 0x1f4f49e4 fb96cca5 a309032f b1a46dde

//	UINT32 key_ecb_192[6]     = {0xe04ad1a4, 0x65fe706b, 0x80cf2960, 0x5da2636a, 0xc42821e1, 0x3ac9b622};  // key_ecb    = 0xe04ad1a4 65fe706b 80cf2960 5da2636a c42821e1 3ac9b622
//	UINT32 iv_ecb_192[4]      = {0x00000000, 0x00000000, 0x00000000, 0x00000000};  // iv_ecb     = 0x0;
//	UINT32 plain_ecb_192[4]   = {0x72304ad5, 0xe5f5937d, 0xf38d8adf, 0x367b0c53};  // plain_ecb  = 0x72304ad5 e5f5937d f38d8adf 367b0c53
//	UINT32 cipher_ecb_192[4]  = {0xc8f26de1, 0x68a21c9d, 0x21323441, 0x9e11db4e};  // cipher_ecb = 0xc8f26de1 68a21c9d 21323441 9e11db4e

//	UINT32 key_ecb_256[8]     = {0xe04ad1a4, 0x65fe706b, 0x80cf2960, 0x5da2636a, 0xc42821e1, 0x3ac9b622, 0x72304ad5, 0xe5f5937d};  // key_ecb    = 0xe04ad1a4 65fe706b 80cf2960 5da2636a c42821e1 3ac9b622 72304ad5 e5f5937d
//	UINT32 iv_ecb_256[4]      = {0x00000000, 0x00000000, 0x00000000, 0x00000000};  // iv_ecb     = 0x0;
//	UINT32 plain_ecb_256[4]   = {0xf38d8adf, 0x367b0c53, 0x1d273eb7, 0x60632039};  // plain_ecb  = 0xf38d8adf 367b0c53 1d273eb7 60632039
//	UINT32 cipher_ecb_256[4]  = {0xaddf70c6, 0x3034834d, 0x5c3ad484, 0xebbf7ed0};  // cipher_ecb = 0xaddf70c6 3034834d 5c3ad484 ebbf7ed0

	uint8_t key_ecb_128[16]     = {0xe0,0x4a,0xd1,0xa4, 0x65,0xfe,0x70,0x6b, 0x80,0xcf,0x29,0x60, 0x5d,0xa2,0x63,0x6a};  // key_ecb    = 0xe04ad1a4 65fe706b 80cf2960 5da2636a
	uint8_t iv_ecb_128[16]      = {0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00};  // iv_ecb     = 0x0;
	uint8_t plain_ecb_128[16]   = {0xc4,0x28,0x21,0xe1, 0x3a,0xc9,0xb6,0x22, 0x72,0x30,0x4a,0xd5, 0xe5,0xf5,0x93,0x7d};  // plain_ecb  = 0xc42821e1 3ac9b622 72304ad5 e5f5937d
	uint8_t cipher_ecb_128[16]  = {0x1f,0x4f,0x49,0xe4, 0xfb,0x96,0xcc,0xa5, 0xa3,0x09,0x03,0x2f, 0xb1,0xa4,0x6d,0xde};  // cipher_ecb = 0x1f4f49e4 fb96cca5 a309032f b1a46dde

	uint8_t key_ecb_192[24]     = {0xe0,0x4a,0xd1,0xa4, 0x65,0xfe,0x70,0x6b, 0x80,0xcf,0x29,0x60, 0x5d,0xa2,0x63,0x6a, 0xc4,0x28,0x21,0xe1, 0x3a,0xc9,0xb6,0x22};  // key_ecb    = 0xe04ad1a4 65fe706b 80cf2960 5da2636a c42821e1 3ac9b622
	uint8_t iv_ecb_192[16]      = {0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00};  // iv_ecb     = 0x0;
	uint8_t plain_ecb_192[16]   = {0x72,0x30,0x4a,0xd5, 0xe5,0xf5,0x93,0x7d, 0xf3,0x8d,0x8a,0xdf, 0x36,0x7b,0x0c,0x53};  // plain_ecb  = 0x72304ad5 e5f5937d f38d8adf 367b0c53
	uint8_t cipher_ecb_192[16]  = {0xc8,0xf2,0x6d,0xe1, 0x68,0xa2,0x1c,0x9d, 0x21,0x32,0x34,0x41, 0x9e,0x11,0xdb,0x4e};  // cipher_ecb = 0xc8f26de1 68a21c9d 21323441 9e11db4e

	uint8_t key_ecb_256[32]     = {0xe0,0x4a,0xd1,0xa4, 0x65,0xfe,0x70,0x6b, 0x80,0xcf,0x29,0x60, 0x5d,0xa2,0x63,0x6a, 0xc4,0x28,0x21,0xe1, 0x3a,0xc9,0xb6,0x22, 0x72,0x30,0x4a,0xd5, 0xe5,0xf5,0x93,0x7d};  // key_ecb    = 0xe04ad1a4 65fe706b 80cf2960 5da2636a c42821e1 3ac9b622 72304ad5 e5f5937d
	uint8_t iv_ecb_256[16]      = {0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00};  // iv_ecb     = 0x0;
	uint8_t plain_ecb_256[16]   = {0xf3,0x8d,0x8a,0xdf, 0x36,0x7b,0x0c,0x53, 0x1d,0x27,0x3e,0xb7, 0x60,0x63,0x20,0x39};  // plain_ecb  = 0xf38d8adf 367b0c53 1d273eb7 60632039
	uint8_t cipher_ecb_256[16]  = {0xad,0xdf,0x70,0xc6, 0x30,0x34,0x83,0x4d, 0x5c,0x3a,0xd4,0x84, 0xeb,0xbf,0x7e,0xd0};  // cipher_ecb = 0xaddf70c6 3034834d 5c3ad484 ebbf7ed0


    //-----------------------    ECB mode (no swap)   ---------------------
//  128
	mode = AES_NORMAL_MODE;
//  mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_ecb_128) / 16;
	HAL_AES_SetKey_U8(key_ecb_128, AES_KEY_128, AES_SWAP_ENABLE);	  //128bit key
	HAL_AES_Crypt_U8(plain_ecb_128, result, blk_len, AES_ENCRYPTION, AES_ECB_MODE, iv_ecb_128, mode);

	for( i = 0; i < (blk_len*4*4); i ++)
	{
		if(result[i] != cipher_ecb_128[i])
		{
			return 1;
		}
	}    

	HAL_AES_Crypt_U8(cipher_ecb_128, result, blk_len, AES_DECRYPTION,AES_ECB_MODE, iv_ecb_128, mode);
	for( i = 0; i < (blk_len*4*4); i ++)
	{
		if(result[i] != plain_ecb_128[i])
		{
			return 1;
		}
	}

//  192
	mode = AES_NORMAL_MODE;
//  mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_ecb_192) / 16;

	HAL_AES_SetKey_U8(key_ecb_192, AES_KEY_192, AES_SWAP_ENABLE);	  //192bit key
	HAL_AES_Crypt_U8(plain_ecb_192, result, blk_len, AES_ENCRYPTION, AES_ECB_MODE, iv_ecb_192, mode);

	for( i = 0; i < (blk_len*4*4); i ++)
	{
		if(result[i] != cipher_ecb_192[i])
		{
			return 1;
		}
	}    

	HAL_AES_Crypt_U8(cipher_ecb_192, result, blk_len, AES_DECRYPTION,AES_ECB_MODE, iv_ecb_192, mode);
	for( i = 0; i < (blk_len*4*4); i ++)
	{
		if(result[i] != plain_ecb_192[i])
		{
			return 1;
		}
	}

//  256
	mode = AES_NORMAL_MODE;
//  mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_ecb_256) / 16;

	HAL_AES_SetKey_U8(key_ecb_256, AES_KEY_256, AES_SWAP_ENABLE );	  //256bit key
	HAL_AES_Crypt_U8(plain_ecb_256, result, blk_len, AES_ENCRYPTION, AES_ECB_MODE, iv_ecb_256, mode);

	for( i = 0; i < (blk_len*4*4); i ++)
	{
		if(result[i] != cipher_ecb_256[i])
		{
			return 1;
		}
	}    

	HAL_AES_Crypt_U8(cipher_ecb_256, result, blk_len, AES_DECRYPTION,AES_ECB_MODE, iv_ecb_256, mode);
	for( i = 0; i < (blk_len*4*4); i ++)
	{
		if(result[i] != plain_ecb_256[i])
		{
			return 1;
		}
	}

	//-----------------------   ECB mode (swap)   ---------------------
    mode = AES_NORMAL_MODE;
//	mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_ecb_128) / 16;
																			 
	swap((UINT32 *)key_ecb_128,4);            //key_ecb[0] = 0xa4d14ae0,...,key_ecb[3] = 0x6a63a25d
	swap((UINT32 *)iv_ecb_128,4);
	swap((UINT32 *)plain_ecb_128,blk_len*4);
	swap((UINT32 *)cipher_ecb_128,blk_len*4);

	HAL_AES_SetKey_U8(key_ecb_128, AES_KEY_128, AES_SWAP_DISABLE);	  //128bit key
	HAL_AES_Crypt_U8(plain_ecb_128, result, blk_len, AES_ENCRYPTION, AES_ECB_MODE, iv_ecb_128, mode);

	for( i = 0; i < (blk_len*4*4); i ++)
	{
		if(result[i] != cipher_ecb_128[i])
		{
			return 1;
		}
	}    

	HAL_AES_Crypt_U8(cipher_ecb_128, result, blk_len, AES_DECRYPTION,AES_ECB_MODE, iv_ecb_128, mode);
	for( i = 0; i < (blk_len*4*4); i ++)
	{
		if(result[i] != plain_ecb_128[i])
		{
			return 1;
		}
	}		
	
	swap((UINT32 *)key_ecb_128,4);            //key_ecb[0] = 0xa4d14ae0,...,key_ecb[3] = 0x6a63a25d
	swap((UINT32 *)iv_ecb_128,4);
	swap((UINT32 *)plain_ecb_128,blk_len*4);
	swap((UINT32 *)cipher_ecb_128,blk_len*4);
		
    mode = AES_NORMAL_MODE;
//	mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_ecb_256) / 16;
																			 
	swap((UINT32 *)key_ecb_256,8);            //key_ecb[0] = 0xa4d14ae0,...,key_ecb[3] = 0x6a63a25d
	swap((UINT32 *)iv_ecb_256,4);
	swap((UINT32 *)plain_ecb_256,blk_len*4);
	swap((UINT32 *)cipher_ecb_256,blk_len*4);

	HAL_AES_SetKey_U8(key_ecb_256, AES_KEY_256, AES_SWAP_DISABLE);	  //128bit key
	HAL_AES_Crypt_U8(plain_ecb_256, result, blk_len, AES_ENCRYPTION, AES_ECB_MODE, iv_ecb_256, mode);

	for( i = 0; i < (blk_len*4*4); i ++)
	{
		if(result[i] != cipher_ecb_256[i])
		{
			return 1;
		}
	}    

	HAL_AES_Crypt_U8(cipher_ecb_256, result, blk_len, AES_DECRYPTION,AES_ECB_MODE, iv_ecb_256, mode);
	for( i = 0; i < (blk_len*4*4); i ++)
	{
		if(result[i] != plain_ecb_256[i])
		{
			return 1;
		}
	}	
	swap((UINT32 *)key_ecb_256,8);            //key_ecb[0] = 0xa4d14ae0,...,key_ecb[3] = 0x6a63a25d
	swap((UINT32 *)iv_ecb_256,4);
	swap((UINT32 *)plain_ecb_256,blk_len*4);
	swap((UINT32 *)cipher_ecb_256,blk_len*4);
	//-----------------------   End of  ECB mode   ---------------------
		
	//-------------------------security mode ---------------------
//  128
//	mode = AES_NORMAL_MODE;
    mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_ecb_128) / 16;
	HAL_AES_SetKey_U8(key_ecb_128, AES_KEY_128, AES_SWAP_ENABLE);	  //128bit key
	HAL_AES_Crypt_U8(plain_ecb_128, result, blk_len, AES_ENCRYPTION, AES_ECB_MODE, iv_ecb_128, mode);

	for( i = 0; i < (blk_len*4*4); i ++)
	{
		if(result[i] != cipher_ecb_128[i])
		{
			return 1;
		}
	}    

	HAL_AES_Crypt_U8(cipher_ecb_128, result, blk_len, AES_DECRYPTION,AES_ECB_MODE, iv_ecb_128, mode);
	for( i = 0; i < (blk_len*4*4); i ++)
	{
		if(result[i] != plain_ecb_128[i])
		{
			return 1;
		}
	}

//  192
//	mode = AES_NORMAL_MODE;
    mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_ecb_192) / 16;

	HAL_AES_SetKey_U8(key_ecb_192, AES_KEY_192, AES_SWAP_ENABLE);	  //192bit key
	HAL_AES_Crypt_U8(plain_ecb_192, result, blk_len, AES_ENCRYPTION, AES_ECB_MODE, iv_ecb_192, mode);

	for( i = 0; i < (blk_len*4*4); i ++)
	{
		if(result[i] != cipher_ecb_192[i])
		{
			return 1;
		}
	}    

	HAL_AES_Crypt_U8(cipher_ecb_192, result, blk_len, AES_DECRYPTION,AES_ECB_MODE, iv_ecb_192, mode);
	for( i = 0; i < (blk_len*4*4); i ++)
	{
		if(result[i] != plain_ecb_192[i])
		{
			return 1;
		}
	}

//  256
//	mode = AES_NORMAL_MODE;
    mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_ecb_256) / 16;

	HAL_AES_SetKey_U8(key_ecb_256, AES_KEY_256, AES_SWAP_ENABLE);	  //256bit key
	HAL_AES_Crypt_U8(plain_ecb_256, result, blk_len, AES_ENCRYPTION, AES_ECB_MODE, iv_ecb_256, mode);

	for( i = 0; i < (blk_len*4*4); i ++)
	{
		if(result[i] != cipher_ecb_256[i])
		{
			return 1;
		}
	}    

	HAL_AES_Crypt_U8(cipher_ecb_256, result, blk_len, AES_DECRYPTION,AES_ECB_MODE, iv_ecb_256, mode);
	for( i = 0; i < (blk_len*4*4); i ++)
	{
		if(result[i] != plain_ecb_256[i])
		{
			return 1;
		}
	}

	//-----------------------   ECB mode (swap)   ---------------------
//  mode = AES_NORMAL_MODE;
	mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_ecb_128) / 16;
																			 
	swap((UINT32 *)key_ecb_128,4);            //key_ecb[0] = 0xa4d14ae0,...,key_ecb[3] = 0x6a63a25d
	swap((UINT32 *)iv_ecb_128,4);
	swap((UINT32 *)plain_ecb_128,blk_len*4);
	swap((UINT32 *)cipher_ecb_128,blk_len*4);

	HAL_AES_SetKey_U8(key_ecb_128, AES_KEY_128, AES_SWAP_DISABLE);	  //128bit key
	HAL_AES_Crypt_U8(plain_ecb_128, result, blk_len, AES_ENCRYPTION, AES_ECB_MODE, iv_ecb_128, mode);

	for( i = 0; i < (blk_len*4*4); i ++)
	{
		if(result[i] != cipher_ecb_128[i])
		{
			return 1;
		}
	}    

	HAL_AES_Crypt_U8(cipher_ecb_128, result, blk_len, AES_DECRYPTION,AES_ECB_MODE, iv_ecb_128, mode);
	for( i = 0; i < (blk_len*4*4); i ++)
	{
		if(result[i] != plain_ecb_128[i])
		{
			return 1;
		}
	}		
//  mode = AES_NORMAL_MODE;
	mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_ecb_256) / 16;
																			 
	swap((UINT32 *)key_ecb_256,8);            //key_ecb[0] = 0xa4d14ae0,...,key_ecb[3] = 0x6a63a25d
	swap((UINT32 *)iv_ecb_256,4);
	swap((UINT32 *)plain_ecb_256,blk_len*4);
	swap((UINT32 *)cipher_ecb_256,blk_len*4);

	HAL_AES_SetKey_U8(key_ecb_256, AES_KEY_256, AES_SWAP_DISABLE);	  //128bit key
	HAL_AES_Crypt_U8(plain_ecb_256, result, blk_len, AES_ENCRYPTION, AES_ECB_MODE, iv_ecb_256, mode);

	for( i = 0; i < (blk_len*4*4); i ++)
	{
		if(result[i] != cipher_ecb_256[i])
		{
			return 1;
		}
	}    

	HAL_AES_Crypt_U8(cipher_ecb_256, result, blk_len, AES_DECRYPTION,AES_ECB_MODE, iv_ecb_256, mode);
	for( i = 0; i < (blk_len*4*4); i ++)
	{
		if(result[i] != plain_ecb_256[i])
		{
			return 1;
		}
	}	

	return 0;
}

UINT32 aes_cbc_test_u8(void)   //CBC Test
{
	UINT32 mode;
	UINT32 i;
//	UINT32 result[20];
	uint8_t result[80];

	UINT32 blk_len;
//  128	
//	UINT32 key_cbc_128[4]     = {0xe04ad1a4, 0x65fe706b, 0x80cf2960, 0x5da2636a};  //key_cbc    = 0xe04ad1a4 65fe706b 80cf2960 5da2636a
//	UINT32 iv_cbc_128[4]      = {0xc42821e1, 0x3ac9b622, 0x72304ad5, 0xe5f5937d};  //iv_cbc     = 0xc42821e1 3ac9b622 72304ad5 e5f5937d  
//  plain_cbc  = 0xf38d8adf 367b0c53 1d273eb7 60632039...
//	UINT32 plain_cbc_128[20]   = {0xf38d8adf, 0x367b0c53, 0x1d273eb7, 0x60632039, 
//                                 0x7e314c2f, 0xa1ab5e18, 0x79aa587e, 0xb834e5de, 
//                                 0x51d45354, 0xafcebfd6, 0x7a9cd97c, 0x5be67590, 
//                                 0x40f27cfd, 0xc60d77f7, 0xf923668d, 0xb2fde000, 
//                                 0x20020634, 0x3fee8224, 0x097ed2f3, 0x4f872281};  
//  //cipher_cbc = 0x099ed143 4af12194 c2cb0351 075ea970...
//	UINT32 cipher_cbc_128[20]  = {0x099ed143, 0x4af12194, 0xc2cb0351, 0x075ea970, 
//                                  0x59c1e350, 0x9c5bcc75, 0x34503402, 0xe1ad79a9, 
//                                  0xae2bd3d2, 0x4ec9e23d, 0x35162f55, 0x993a1b83, 
//                                  0x35779406, 0x00d30e40, 0xc28639a1, 0x4ea39e14, 
//                                  0x5a94dd9d, 0x80adf249, 0x863da72a, 0x71021e78};  
	uint8_t key_cbc_128[16]     = {0xe0,0x4a,0xd1,0xa4, 0x65,0xfe,0x70,0x6b, 0x80,0xcf,0x29,0x60, 0x5d,0xa2,0x63,0x6a};  //key_cbc    = 0xe04ad1a4 65fe706b 80cf2960 5da2636a
	uint8_t iv_cbc_128[16]      = {0xc4,0x28,0x21,0xe1, 0x3a,0xc9,0xb6,0x22, 0x72,0x30,0x4a,0xd5, 0xe5,0xf5,0x93,0x7d};  //iv_cbc     = 0xc42821e1 3ac9b622 72304ad5 e5f5937d  
//  plain_cbc  = 0xf38d8adf 367b0c53 1d273eb7 60632039...
	uint8_t plain_cbc_128[80]   = {0xf3,0x8d,0x8a,0xdf, 0x36,0x7b,0x0c,0x53, 0x1d,0x27,0x3e,0xb7, 0x60,0x63,0x20,0x39, 
							 0x7e,0x31,0x4c,0x2f, 0xa1,0xab,0x5e,0x18, 0x79,0xaa,0x58,0x7e, 0xb8,0x34,0xe5,0xde, 
							 0x51,0xd4,0x53,0x54, 0xaf,0xce,0xbf,0xd6, 0x7a,0x9c,0xd9,0x7c, 0x5b,0xe6,0x75,0x90, 
							 0x40,0xf2,0x7c,0xfd, 0xc6,0x0d,0x77,0xf7, 0xf9,0x23,0x66,0x8d, 0xb2,0xfd,0xe0,0x00, 
							 0x20,0x02,0x06,0x34, 0x3f,0xee,0x82,0x24, 0x09,0x7e,0xd2,0xf3, 0x4f,0x87,0x22,0x81};  
//  cipher_cbc = 0x099ed143 4af12194 c2cb0351 075ea970...
	uint8_t cipher_cbc_128[80]  = {0x09,0x9e,0xd1,0x43, 0x4a,0xf1,0x21,0x94, 0xc2,0xcb,0x03,0x51, 0x07,0x5e,0xa9,0x70, 
							 0x59,0xc1,0xe3,0x50, 0x9c,0x5b,0xcc,0x75, 0x34,0x50,0x34,0x02, 0xe1,0xad,0x79,0xa9, 
							 0xae,0x2b,0xd3,0xd2, 0x4e,0xc9,0xe2,0x3d, 0x35,0x16,0x2f,0x55, 0x99,0x3a,0x1b,0x83, 
							 0x35,0x77,0x94,0x06, 0x00,0xd3,0x0e,0x40, 0xc2,0x86,0x39,0xa1, 0x4e,0xa3,0x9e,0x14, 
							 0x5a,0x94,0xdd,0x9d, 0x80,0xad,0xf2,0x49, 0x86,0x3d,0xa7,0x2a, 0x71,0x02,0x1e,0x78};  
//  192
//	UINT32 key_cbc_192[6]     = {0xe04ad1a4, 0x65fe706b, 0x80cf2960, 0x5da2636a, 0xc42821e1, 0x3ac9b622};  // key_ecb    = 0xe04ad1a4 65fe706b 80cf2960 5da2636a c42821e1 3ac9b622
//	UINT32 iv_cbc_192[4]      = {0x72304ad5, 0xe5f5937d, 0xf38d8adf, 0x367b0c53};  //iv_cbc    = 0x72304ad5 e5f5937d f38d8adf 367b0c53
//	UINT32 plain_cbc_192[20]   = {0x1d273eb7, 0x60632039, 0x7e314c2f, 0xa1ab5e18, 
//                                0x79aa587e, 0xb834e5de, 0x51d45354, 0xafcebfd6, 
//                                0x7a9cd97c, 0x5be67590, 0x40f27cfd, 0xc60d77f7, 
//                                0xf923668d, 0xb2fde000, 0x20020634, 0x3fee8224, 
//                                0x097ed2f3, 0x4f872281, 0x4329f38c, 0x6c564821}; //plain_cbc  = 0x1d273eb7 60632039 7e314c2f a1ab5e18...
//	UINT32 cipher_cbc_192[20]  = {0x4c9abdee, 0x35097f17, 0x37de6468, 0x0a14af58, 
//                                0x20e48235, 0x31a46420, 0x93a283f4, 0x7e982a45, 
//                                0x9e15fd81, 0xf80f2328, 0x0701d24d, 0x4c7bcb79, 
//                                0xbcfd3d10, 0xdc3dceb2, 0x800fa9d1, 0xadc513f9, 
//                                0xfa161563, 0x3c65e394, 0xbad169b6, 0xac2f04cd}; //cipher_cbc = 0x4c9abdee 35097f17 37de6468 0a14af58...
	uint8_t key_cbc_192[24]     = {0xe0,0x4a,0xd1,0xa4, 0x65,0xfe,0x70,0x6b, 0x80,0xcf,0x29,0x60, 0x5d,0xa2,0x63,0x6a, 0xc4,0x28,0x21,0xe1, 0x3a,0xc9,0xb6,0x22};  // key_ecb    = 0xe04ad1a4 65fe706b 80cf2960 5da2636a c42821e1 3ac9b622
	uint8_t iv_cbc_192[16]      = {0x72,0x30,0x4a,0xd5, 0xe5,0xf5,0x93,0x7d, 0xf3,0x8d,0x8a,0xdf, 0x36,0x7b,0x0c,0x53};  //iv_cbc    = 0x72304ad5 e5f5937d f38d8adf 367b0c53
	uint8_t plain_cbc_192[80]   = {0x1d,0x27,0x3e,0xb7, 0x60,0x63,0x20,0x39, 0x7e,0x31,0x4c,0x2f, 0xa1,0xab,0x5e,0x18, 
							 0x79,0xaa,0x58,0x7e, 0xb8,0x34,0xe5,0xde, 0x51,0xd4,0x53,0x54, 0xaf,0xce,0xbf,0xd6, 
							 0x7a,0x9c,0xd9,0x7c, 0x5b,0xe6,0x75,0x90, 0x40,0xf2,0x7c,0xfd, 0xc6,0x0d,0x77,0xf7, 
							 0xf9,0x23,0x66,0x8d, 0xb2,0xfd,0xe0,0x00, 0x20,0x02,0x06,0x34, 0x3f,0xee,0x82,0x24, 
							 0x09,0x7e,0xd2,0xf3, 0x4f,0x87,0x22,0x81, 0x43,0x29,0xf3,0x8c, 0x6c,0x56,0x48,0x21}; //plain_cbc  = 0x1d273eb7 60632039 7e314c2f a1ab5e18...
	uint8_t cipher_cbc_192[80]  = {0x4c,0x9a,0xbd,0xee, 0x35,0x09,0x7f,0x17, 0x37,0xde,0x64,0x68, 0x0a,0x14,0xaf,0x58, 
							 0x20,0xe4,0x82,0x35, 0x31,0xa4,0x64,0x20, 0x93,0xa2,0x83,0xf4, 0x7e,0x98,0x2a,0x45, 
							 0x9e,0x15,0xfd,0x81, 0xf8,0x0f,0x23,0x28, 0x07,0x01,0xd2,0x4d, 0x4c,0x7b,0xcb,0x79, 
							 0xbc,0xfd,0x3d,0x10, 0xdc,0x3d,0xce,0xb2, 0x80,0x0f,0xa9,0xd1, 0xad,0xc5,0x13,0xf9, 
							 0xfa,0x16,0x15,0x63, 0x3c,0x65,0xe3,0x94, 0xba,0xd1,0x69,0xb6, 0xac,0x2f,0x04,0xcd}; //cipher_cbc = 0x4c9abdee 35097f17 37de6468 0a14af58...

//  256
//	UINT32 key_cbc_256[8]     = {0xe04ad1a4, 0x65fe706b, 0x80cf2960, 0x5da2636a, 0xc42821e1, 0x3ac9b622, 0x72304ad5, 0xe5f5937d};  // key_ecb    = 0xe04ad1a4 65fe706b 80cf2960 5da2636a c42821e1 3ac9b622 72304ad5 e5f5937d
//	UINT32 iv_cbc_256[4]      = {0xf38d8adf, 0x367b0c53, 0x1d273eb7, 0x60632039};  // iv_cbc    = 0xf38d8adf 367b0c53 1d273eb7 60632039;
//	UINT32 plain_cbc_256[20]   = {0x7e314c2f, 0xa1ab5e18, 0x79aa587e, 0xb834e5de,
//                                0x51d45354, 0xafcebfd6, 0x7a9cd97c, 0x5be67590, 
//                                0x40f27cfd, 0xc60d77f7, 0xf923668d, 0xb2fde000,
//                                0x20020634, 0x3fee8224, 0x097ed2f3, 0x4f872281,
//                                0x4329f38c, 0x6c564821, 0x6e897e46, 0x25ac03f3};  // plain_cbc  = 0x7e314c2f a1ab5e18 79aa587e b834e5de...
//	UINT32 cipher_cbc_256[20]  = {0xc8324f39, 0xcf6969a7, 0x35f97dd9, 0x34a90318,
//                                0x72866a00, 0x2fd8e17d, 0xf8003085, 0x4e796949,
//                                0xcd70af28, 0xe13be0d2, 0x2d613def, 0x8da350f6,
//                                0x7107db84, 0x1c3f5585, 0xc77b89dc, 0x4ccd5abc,
//                                0x307463b8, 0x5098659e, 0xdd3c2694, 0xedd32cdb};  // cipher_cbc = 0xc8324f39 cf6969a7 35f97dd9 34a90318...
	uint8_t key_cbc_256[32]     = {0xe0,0x4a,0xd1,0xa4, 0x65,0xfe,0x70,0x6b, 0x80,0xcf,0x29,0x60, 0x5d,0xa2,0x63,0x6a, 0xc4,0x28,0x21,0xe1, 0x3a,0xc9,0xb6,0x22, 0x72,0x30,0x4a,0xd5, 0xe5,0xf5,0x93,0x7d};  // key_ecb    = 0xe04ad1a4 65fe706b 80cf2960 5da2636a c42821e1 3ac9b622 72304ad5 e5f5937d
	uint8_t iv_cbc_256[16]      = {0xf3,0x8d,0x8a,0xdf, 0x36,0x7b,0x0c,0x53, 0x1d,0x27,0x3e,0xb7, 0x60,0x63,0x20,0x39};  // iv_cbc    = 0xf38d8adf 367b0c53 1d273eb7 60632039;
	uint8_t plain_cbc_256[80]   = {0x7e,0x31,0x4c,0x2f, 0xa1,0xab,0x5e,0x18, 0x79,0xaa,0x58,0x7e, 0xb8,0x34,0xe5,0xde,
							 0x51,0xd4,0x53,0x54, 0xaf,0xce,0xbf,0xd6, 0x7a,0x9c,0xd9,0x7c, 0x5b,0xe6,0x75,0x90, 
							 0x40,0xf2,0x7c,0xfd, 0xc6,0x0d,0x77,0xf7, 0xf9,0x23,0x66,0x8d, 0xb2,0xfd,0xe0,0x00,
							 0x20,0x02,0x06,0x34, 0x3f,0xee,0x82,0x24, 0x09,0x7e,0xd2,0xf3, 0x4f,0x87,0x22,0x81,
							 0x43,0x29,0xf3,0x8c, 0x6c,0x56,0x48,0x21, 0x6e,0x89,0x7e,0x46, 0x25,0xac,0x03,0xf3};  // plain_cbc  = 0x7e314c2f a1ab5e18 79aa587e b834e5de...
	uint8_t cipher_cbc_256[80]  = {0xc8,0x32,0x4f,0x39, 0xcf,0x69,0x69,0xa7, 0x35,0xf9,0x7d,0xd9, 0x34,0xa9,0x03,0x18,
							 0x72,0x86,0x6a,0x00, 0x2f,0xd8,0xe1,0x7d, 0xf8,0x00,0x30,0x85, 0x4e,0x79,0x69,0x49,
							 0xcd,0x70,0xaf,0x28, 0xe1,0x3b,0xe0,0xd2, 0x2d,0x61,0x3d,0xef, 0x8d,0xa3,0x50,0xf6,
							 0x71,0x07,0xdb,0x84, 0x1c,0x3f,0x55,0x85, 0xc7,0x7b,0x89,0xdc, 0x4c,0xcd,0x5a,0xbc,
							 0x30,0x74,0x63,0xb8, 0x50,0x98,0x65,0x9e, 0xdd,0x3c,0x26,0x94, 0xed,0xd3,0x2c,0xdb};  // cipher_cbc = 0xc8324f39 cf6969a7 35f97dd9 34a90318...


    //-----------------------    CBC mode (no swap)   ---------------------
//  128
	mode = AES_NORMAL_MODE;
//  mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_cbc_128) / 16;
		
    HAL_AES_SetKey_U8(key_cbc_128, AES_KEY_128, AES_SWAP_ENABLE);	  //128bit key
	HAL_AES_Crypt_U8(plain_cbc_128, result, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_128, mode);
    
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(result[i] != cipher_cbc_128[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt_U8(cipher_cbc_128, result, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_128, mode);
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(result[i] != plain_cbc_128[i])
    	{
    		return 1;
    	}
    }

//  192
	mode = AES_NORMAL_MODE;
//  mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_cbc_192) / 16;
		
    HAL_AES_SetKey_U8(key_cbc_192, AES_KEY_192, AES_SWAP_ENABLE);	  //192bit key
    HAL_AES_Crypt_U8(plain_cbc_192, result, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_192, mode);
    
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(result[i] != cipher_cbc_192[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt_U8(cipher_cbc_192, result, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_192, mode);
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(result[i] != plain_cbc_192[i])
    	{
    		return 1;
    	}
    }

//  256
	mode = AES_NORMAL_MODE;
//  mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_cbc_256) / 16;
		
    HAL_AES_SetKey_U8(key_cbc_256, AES_KEY_256, AES_SWAP_ENABLE );	  //256bit key
    HAL_AES_Crypt_U8(plain_cbc_256, result, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_256, mode);
    
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(result[i] != cipher_cbc_256[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt_U8(cipher_cbc_256, result, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_256, mode);
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(result[i] != plain_cbc_256[i])
    	{
    		return 1;
    	}
    }

  //-----------------------    CBC mode (swap)   ---------------------
	mode = AES_NORMAL_MODE;
//  mode = AES_SECURITY_MODE;
                                             
	blk_len = sizeof(plain_cbc_128) / 16;

	swap((UINT32 *)key_cbc_128,4);            //key_ecb[0] = 0xa4d14ae0,...,key_ecb[3] = 0x6a63a25d
	swap((UINT32 *)iv_cbc_128,4);
	swap((UINT32 *)plain_cbc_128,blk_len*4);
	swap((UINT32 *)cipher_cbc_128,blk_len*4);	

    HAL_AES_SetKey_U8(key_cbc_128, AES_KEY_128, AES_SWAP_DISABLE);	//128bit key
    HAL_AES_Crypt_U8(plain_cbc_128, result, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_128, mode);
    
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(result[i] != cipher_cbc_128[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt_U8(cipher_cbc_128, result, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_128, mode);
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(result[i] != plain_cbc_128[i])
    	{
    		return 1;
    	}
    }   
		
	swap((UINT32 *)key_cbc_128,4);            //key_ecb[0] = 0xa4d14ae0,...,key_ecb[3] = 0x6a63a25d
	swap((UINT32 *)iv_cbc_128,4);
	swap((UINT32 *)plain_cbc_128,blk_len*4);
	swap((UINT32 *)cipher_cbc_128,blk_len*4);		
		
	mode = AES_NORMAL_MODE;
//  mode = AES_SECURITY_MODE;
                                             
	blk_len = sizeof(plain_cbc_256) / 16;

	swap((UINT32 *)key_cbc_256,8);            //key_ecb[0] = 0xa4d14ae0,...,key_ecb[3] = 0x6a63a25d
	swap((UINT32 *)iv_cbc_256,4);
	swap((UINT32 *)plain_cbc_256,blk_len*4);
	swap((UINT32 *)cipher_cbc_256,blk_len*4);	

    HAL_AES_SetKey_U8(key_cbc_256, AES_KEY_256, AES_SWAP_DISABLE);	//128bit key
    HAL_AES_Crypt_U8(plain_cbc_256, result, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_256, mode);
    
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(result[i] != cipher_cbc_256[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt_U8(cipher_cbc_256, result, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_256, mode);
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(result[i] != plain_cbc_256[i])
    	{
    		return 1;
    	}
    }     
	swap((UINT32 *)key_cbc_256,8);            //key_ecb[0] = 0xa4d14ae0,...,key_ecb[3] = 0x6a63a25d
	swap((UINT32 *)iv_cbc_256,4);
	swap((UINT32 *)plain_cbc_256,blk_len*4);
	swap((UINT32 *)cipher_cbc_256,blk_len*4);	
		
	//---------------------security-----------------
//  128
//	mode = AES_NORMAL_MODE;
	mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_cbc_128) / 16;
		
    HAL_AES_SetKey_U8(key_cbc_128, AES_KEY_128, AES_SWAP_ENABLE);	  //128bit key
    HAL_AES_Crypt_U8(plain_cbc_128, result, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_128, mode);
    
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(result[i] != cipher_cbc_128[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt_U8(cipher_cbc_128, result, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_128, mode);
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(result[i] != plain_cbc_128[i])
    	{
    		return 1;
    	}
    }

//  192
//	mode = AES_NORMAL_MODE;
    mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_cbc_192) / 16;
		
    HAL_AES_SetKey_U8(key_cbc_192, AES_KEY_192, AES_SWAP_ENABLE);	  //192bit key
    HAL_AES_Crypt_U8(plain_cbc_192, result, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_192, mode);
    
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(result[i] != cipher_cbc_192[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt_U8(cipher_cbc_192, result, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_192, mode);
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(result[i] != plain_cbc_192[i])
    	{
    		return 1;
    	}
    }

//  256
//	mode = AES_NORMAL_MODE;
    mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_cbc_256) / 16;
		
    HAL_AES_SetKey_U8(key_cbc_256, AES_KEY_256, AES_SWAP_ENABLE );	  //256bit key
    HAL_AES_Crypt_U8(plain_cbc_256, result, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_256, mode);
    
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(result[i] != cipher_cbc_256[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt_U8(cipher_cbc_256, result, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_256, mode);
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(result[i] != plain_cbc_256[i])
    	{
    		return 1;
    	}
    }

  //-----------------------    CBC mode (swap)   ---------------------
//	mode = AES_NORMAL_MODE;
    mode = AES_SECURITY_MODE;
                                             
	blk_len = sizeof(plain_cbc_128) / 16;

	swap((UINT32 *)key_cbc_128,4);            //key_ecb[0] = 0xa4d14ae0,...,key_ecb[3] = 0x6a63a25d
	swap((UINT32 *)iv_cbc_128,4);
	swap((UINT32 *)plain_cbc_128,blk_len*4);
	swap((UINT32 *)cipher_cbc_128,blk_len*4);	

    HAL_AES_SetKey_U8(key_cbc_128, AES_KEY_128, AES_SWAP_DISABLE);	//128bit key
    HAL_AES_Crypt_U8(plain_cbc_128, result, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_128, mode);
    
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(result[i] != cipher_cbc_128[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt_U8(cipher_cbc_128, result, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_128, mode);
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(result[i] != plain_cbc_128[i])
    	{
    		return 1;
    	}
    }        
//	mode = AES_NORMAL_MODE;
    mode = AES_SECURITY_MODE;
                                             
	blk_len = sizeof(plain_cbc_256) / 16;

	swap((UINT32 *)key_cbc_256,8);            //key_ecb[0] = 0xa4d14ae0,...,key_ecb[3] = 0x6a63a25d
	swap((UINT32 *)iv_cbc_256,4);
	swap((UINT32 *)plain_cbc_256,blk_len*4);
	swap((UINT32 *)cipher_cbc_256,blk_len*4);	

    HAL_AES_SetKey_U8(key_cbc_256, AES_KEY_256, AES_SWAP_DISABLE);	//128bit key
    HAL_AES_Crypt_U8(plain_cbc_256, result, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_256, mode);
    
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(result[i] != cipher_cbc_256[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt_U8(cipher_cbc_256, result, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_256, mode);
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(result[i] != plain_cbc_256[i])
    	{
    		return 1;
    	}
    } 
 		
	//-----------------------   End of  CBC mode   ---------------------
	return 0;
}

UINT32 aes_inout_test_u8(void)   //CBC Test
{
	UINT32 mode;
	UINT32 i;

	UINT32 blk_len;
//  128	
//	UINT32 key_cbc_128[4]     = {0xe04ad1a4, 0x65fe706b, 0x80cf2960, 0x5da2636a};  //key_cbc    = 0xe04ad1a4 65fe706b 80cf2960 5da2636a
//	UINT32 iv_cbc_128[4]      = {0xc42821e1, 0x3ac9b622, 0x72304ad5, 0xe5f5937d};  //iv_cbc     = 0xc42821e1 3ac9b622 72304ad5 e5f5937d  
//  plain_cbc  = 0xf38d8adf 367b0c53 1d273eb7 60632039...
//	UINT32 plain_cbc_128[20]   = {0xf38d8adf, 0x367b0c53, 0x1d273eb7, 0x60632039, 
//                                 0x7e314c2f, 0xa1ab5e18, 0x79aa587e, 0xb834e5de, 
//                                 0x51d45354, 0xafcebfd6, 0x7a9cd97c, 0x5be67590, 
//                                 0x40f27cfd, 0xc60d77f7, 0xf923668d, 0xb2fde000, 
//                                 0x20020634, 0x3fee8224, 0x097ed2f3, 0x4f872281};  
//  cipher_cbc = 0x099ed143 4af12194 c2cb0351 075ea970...
//	UINT32 cipher_cbc_128[20]  = {0x099ed143, 0x4af12194, 0xc2cb0351, 0x075ea970, 
//                                  0x59c1e350, 0x9c5bcc75, 0x34503402, 0xe1ad79a9, 
//                                  0xae2bd3d2, 0x4ec9e23d, 0x35162f55, 0x993a1b83, 
//                                  0x35779406, 0x00d30e40, 0xc28639a1, 0x4ea39e14, 
//                                  0x5a94dd9d, 0x80adf249, 0x863da72a, 0x71021e78};  
	uint8_t key_cbc_128[16]     = {0xe0,0x4a,0xd1,0xa4, 0x65,0xfe,0x70,0x6b, 0x80,0xcf,0x29,0x60, 0x5d,0xa2,0x63,0x6a};  //key_cbc    = 0xe04ad1a4 65fe706b 80cf2960 5da2636a
	uint8_t iv_cbc_128[16]      = {0xc4,0x28,0x21,0xe1, 0x3a,0xc9,0xb6,0x22, 0x72,0x30,0x4a,0xd5, 0xe5,0xf5,0x93,0x7d};  //iv_cbc     = 0xc42821e1 3ac9b622 72304ad5 e5f5937d  
//  plain_cbc  = 0xf38d8adf 367b0c53 1d273eb7 60632039...
	uint8_t plain_cbc_128[80]   = {0xf3,0x8d,0x8a,0xdf, 0x36,0x7b,0x0c,0x53, 0x1d,0x27,0x3e,0xb7, 0x60,0x63,0x20,0x39, 
                                 0x7e,0x31,0x4c,0x2f, 0xa1,0xab,0x5e,0x18, 0x79,0xaa,0x58,0x7e, 0xb8,0x34,0xe5,0xde, 
                                 0x51,0xd4,0x53,0x54, 0xaf,0xce,0xbf,0xd6, 0x7a,0x9c,0xd9,0x7c, 0x5b,0xe6,0x75,0x90, 
                                 0x40,0xf2,0x7c,0xfd, 0xc6,0x0d,0x77,0xf7, 0xf9,0x23,0x66,0x8d, 0xb2,0xfd,0xe0,0x00, 
                                 0x20,0x02,0x06,0x34, 0x3f,0xee,0x82,0x24, 0x09,0x7e,0xd2,0xf3, 0x4f,0x87,0x22,0x81};  
//  cipher_cbc = 0x099ed143 4af12194 c2cb0351 075ea970...
	uint8_t cipher_cbc_128[80]  = {0x09,0x9e,0xd1,0x43, 0x4a,0xf1,0x21,0x94, 0xc2,0xcb,0x03,0x51, 0x07,0x5e,0xa9,0x70, 
                                 0x59,0xc1,0xe3,0x50, 0x9c,0x5b,0xcc,0x75, 0x34,0x50,0x34,0x02, 0xe1,0xad,0x79,0xa9, 
                                 0xae,0x2b,0xd3,0xd2, 0x4e,0xc9,0xe2,0x3d, 0x35,0x16,0x2f,0x55, 0x99,0x3a,0x1b,0x83, 
                                 0x35,0x77,0x94,0x06, 0x00,0xd3,0x0e,0x40, 0xc2,0x86,0x39,0xa1, 0x4e,0xa3,0x9e,0x14, 
                                 0x5a,0x94,0xdd,0x9d, 0x80,0xad,0xf2,0x49, 0x86,0x3d,0xa7,0x2a, 0x71,0x02,0x1e,0x78};  
//  192
//	UINT32 key_cbc_192[6]     = {0xe04ad1a4, 0x65fe706b, 0x80cf2960, 0x5da2636a, 0xc42821e1, 0x3ac9b622};  // key_ecb    = 0xe04ad1a4 65fe706b 80cf2960 5da2636a c42821e1 3ac9b622
//	UINT32 iv_cbc_192[4]      = {0x72304ad5, 0xe5f5937d, 0xf38d8adf, 0x367b0c53};  //iv_cbc    = 0x72304ad5 e5f5937d f38d8adf 367b0c53
//	UINT32 plain_cbc_192[20]   = {0x1d273eb7, 0x60632039, 0x7e314c2f, 0xa1ab5e18, 
//                                  0x79aa587e, 0xb834e5de, 0x51d45354, 0xafcebfd6, 
//                                  0x7a9cd97c, 0x5be67590, 0x40f27cfd, 0xc60d77f7, 
//                                  0xf923668d, 0xb2fde000, 0x20020634, 0x3fee8224, 
//                                  0x097ed2f3, 0x4f872281, 0x4329f38c, 0x6c564821}; //plain_cbc  = 0x1d273eb7 60632039 7e314c2f a1ab5e18...
//	UINT32 cipher_cbc_192[20]  = {0x4c9abdee, 0x35097f17, 0x37de6468, 0x0a14af58, 
//                                  0x20e48235, 0x31a46420, 0x93a283f4, 0x7e982a45, 
//                                  0x9e15fd81, 0xf80f2328, 0x0701d24d, 0x4c7bcb79, 
//                                  0xbcfd3d10, 0xdc3dceb2, 0x800fa9d1, 0xadc513f9, 
//                                  0xfa161563, 0x3c65e394, 0xbad169b6, 0xac2f04cd}; //cipher_cbc = 0x4c9abdee 35097f17 37de6468 0a14af58...
	uint8_t key_cbc_192[24]     = {0xe0,0x4a,0xd1,0xa4, 0x65,0xfe,0x70,0x6b, 0x80,0xcf,0x29,0x60, 0x5d,0xa2,0x63,0x6a, 0xc4,0x28,0x21,0xe1, 0x3a,0xc9,0xb6,0x22};  // key_ecb    = 0xe04ad1a4 65fe706b 80cf2960 5da2636a c42821e1 3ac9b622
	uint8_t iv_cbc_192[16]      = {0x72,0x30,0x4a,0xd5, 0xe5,0xf5,0x93,0x7d, 0xf3,0x8d,0x8a,0xdf, 0x36,0x7b,0x0c,0x53};  //iv_cbc    = 0x72304ad5 e5f5937d f38d8adf 367b0c53
	uint8_t plain_cbc_192[80]   = {0x1d,0x27,0x3e,0xb7, 0x60,0x63,0x20,0x39, 0x7e,0x31,0x4c,0x2f, 0xa1,0xab,0x5e,0x18, 
                                 0x79,0xaa,0x58,0x7e, 0xb8,0x34,0xe5,0xde, 0x51,0xd4,0x53,0x54, 0xaf,0xce,0xbf,0xd6, 
                                 0x7a,0x9c,0xd9,0x7c, 0x5b,0xe6,0x75,0x90, 0x40,0xf2,0x7c,0xfd, 0xc6,0x0d,0x77,0xf7, 
                                 0xf9,0x23,0x66,0x8d, 0xb2,0xfd,0xe0,0x00, 0x20,0x02,0x06,0x34, 0x3f,0xee,0x82,0x24, 
                                 0x09,0x7e,0xd2,0xf3, 0x4f,0x87,0x22,0x81, 0x43,0x29,0xf3,0x8c, 0x6c,0x56,0x48,0x21}; //plain_cbc  = 0x1d273eb7 60632039 7e314c2f a1ab5e18...
	uint8_t cipher_cbc_192[80]  = {0x4c,0x9a,0xbd,0xee, 0x35,0x09,0x7f,0x17, 0x37,0xde,0x64,0x68, 0x0a,0x14,0xaf,0x58, 
                                 0x20,0xe4,0x82,0x35, 0x31,0xa4,0x64,0x20, 0x93,0xa2,0x83,0xf4, 0x7e,0x98,0x2a,0x45, 
                                 0x9e,0x15,0xfd,0x81, 0xf8,0x0f,0x23,0x28, 0x07,0x01,0xd2,0x4d, 0x4c,0x7b,0xcb,0x79, 
                                 0xbc,0xfd,0x3d,0x10, 0xdc,0x3d,0xce,0xb2, 0x80,0x0f,0xa9,0xd1, 0xad,0xc5,0x13,0xf9, 
                                 0xfa,0x16,0x15,0x63, 0x3c,0x65,0xe3,0x94, 0xba,0xd1,0x69,0xb6, 0xac,0x2f,0x04,0xcd}; //cipher_cbc = 0x4c9abdee 35097f17 37de6468 0a14af58...

//256
//	UINT32 key_cbc_256[8]     = {0xe04ad1a4, 0x65fe706b, 0x80cf2960, 0x5da2636a, 0xc42821e1, 0x3ac9b622, 0x72304ad5, 0xe5f5937d};  // key_ecb    = 0xe04ad1a4 65fe706b 80cf2960 5da2636a c42821e1 3ac9b622 72304ad5 e5f5937d
//	UINT32 iv_cbc_256[4]      = {0xf38d8adf, 0x367b0c53, 0x1d273eb7, 0x60632039};  // iv_cbc    = 0xf38d8adf 367b0c53 1d273eb7 60632039;
//	UINT32 plain_cbc_256[20]   = {0x7e314c2f, 0xa1ab5e18, 0x79aa587e, 0xb834e5de,
//                                 0x51d45354, 0xafcebfd6, 0x7a9cd97c, 0x5be67590, 
//                                 0x40f27cfd, 0xc60d77f7, 0xf923668d, 0xb2fde000,
//                                 0x20020634, 0x3fee8224, 0x097ed2f3, 0x4f872281,
//                                 0x4329f38c, 0x6c564821, 0x6e897e46, 0x25ac03f3};  // plain_cbc  = 0x7e314c2f a1ab5e18 79aa587e b834e5de...
//	UINT32 cipher_cbc_256[20]  = {0xc8324f39, 0xcf6969a7, 0x35f97dd9, 0x34a90318,
//                                 0x72866a00, 0x2fd8e17d, 0xf8003085, 0x4e796949,
//                                 0xcd70af28, 0xe13be0d2, 0x2d613def, 0x8da350f6,
//                                 0x7107db84, 0x1c3f5585, 0xc77b89dc, 0x4ccd5abc,
//                                 0x307463b8, 0x5098659e, 0xdd3c2694, 0xedd32cdb};  // cipher_cbc = 0xc8324f39 cf6969a7 35f97dd9 34a90318...
	uint8_t key_cbc_256[32]     = {0xe0,0x4a,0xd1,0xa4, 0x65,0xfe,0x70,0x6b, 0x80,0xcf,0x29,0x60, 0x5d,0xa2,0x63,0x6a, 0xc4,0x28,0x21,0xe1, 0x3a,0xc9,0xb6,0x22, 0x72,0x30,0x4a,0xd5, 0xe5,0xf5,0x93,0x7d};  // key_ecb    = 0xe04ad1a4 65fe706b 80cf2960 5da2636a c42821e1 3ac9b622 72304ad5 e5f5937d
	uint8_t iv_cbc_256[16]      = {0xf3,0x8d,0x8a,0xdf, 0x36,0x7b,0x0c,0x53, 0x1d,0x27,0x3e,0xb7, 0x60,0x63,0x20,0x39};  // iv_cbc    = 0xf38d8adf 367b0c53 1d273eb7 60632039;
	uint8_t plain_cbc_256[80]   = {0x7e,0x31,0x4c,0x2f, 0xa1,0xab,0x5e,0x18, 0x79,0xaa,0x58,0x7e, 0xb8,0x34,0xe5,0xde,
                                 0x51,0xd4,0x53,0x54, 0xaf,0xce,0xbf,0xd6, 0x7a,0x9c,0xd9,0x7c, 0x5b,0xe6,0x75,0x90, 
                                 0x40,0xf2,0x7c,0xfd, 0xc6,0x0d,0x77,0xf7, 0xf9,0x23,0x66,0x8d, 0xb2,0xfd,0xe0,0x00,
                                 0x20,0x02,0x06,0x34, 0x3f,0xee,0x82,0x24, 0x09,0x7e,0xd2,0xf3, 0x4f,0x87,0x22,0x81,
                                 0x43,0x29,0xf3,0x8c, 0x6c,0x56,0x48,0x21, 0x6e,0x89,0x7e,0x46, 0x25,0xac,0x03,0xf3};  // plain_cbc  = 0x7e314c2f a1ab5e18 79aa587e b834e5de...
	uint8_t cipher_cbc_256[80]  = {0xc8,0x32,0x4f,0x39, 0xcf,0x69,0x69,0xa7, 0x35,0xf9,0x7d,0xd9, 0x34,0xa9,0x03,0x18,
                                 0x72,0x86,0x6a,0x00, 0x2f,0xd8,0xe1,0x7d, 0xf8,0x00,0x30,0x85, 0x4e,0x79,0x69,0x49,
                                 0xcd,0x70,0xaf,0x28, 0xe1,0x3b,0xe0,0xd2, 0x2d,0x61,0x3d,0xef, 0x8d,0xa3,0x50,0xf6,
                                 0x71,0x07,0xdb,0x84, 0x1c,0x3f,0x55,0x85, 0xc7,0x7b,0x89,0xdc, 0x4c,0xcd,0x5a,0xbc,
                                 0x30,0x74,0x63,0xb8, 0x50,0x98,0x65,0x9e, 0xdd,0x3c,0x26,0x94, 0xed,0xd3,0x2c,0xdb};  // cipher_cbc = 0xc8324f39 cf6969a7 35f97dd9 34a90318...

	uint8_t temp_cbc[80];
    //-----------------------    CBC mode (no swap)   ---------------------
//128
	mode = AES_NORMAL_MODE;
//  mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_cbc_128) / 16;
		
	memcpy((uint8_t*)temp_cbc,(uint8_t*)plain_cbc_128,sizeof(plain_cbc_128));
    HAL_AES_SetKey_U8(key_cbc_128, AES_KEY_128, AES_SWAP_ENABLE);	  //128bit key
    HAL_AES_Crypt_U8(temp_cbc, temp_cbc, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_128, mode);
    
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(temp_cbc[i] != cipher_cbc_128[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt_U8(temp_cbc, temp_cbc, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_128, mode);
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(temp_cbc[i] != plain_cbc_128[i])
    	{
    		return 1;
    	}
    }

//192
	mode = AES_NORMAL_MODE;
//  mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_cbc_192) / 16;
	memcpy((uint8_t*)temp_cbc,(uint8_t*)plain_cbc_192,sizeof(plain_cbc_192));		
    HAL_AES_SetKey_U8(key_cbc_192, AES_KEY_192, AES_SWAP_ENABLE);	  //192bit key
    HAL_AES_Crypt_U8(temp_cbc, temp_cbc, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_192, mode);
    
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(temp_cbc[i] != cipher_cbc_192[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt_U8(temp_cbc, temp_cbc, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_192, mode);
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(temp_cbc[i] != plain_cbc_192[i])
    	{
    		return 1;
    	}
    }

//256
	mode = AES_NORMAL_MODE;
//  mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_cbc_256) / 16;
	memcpy((uint8_t*)temp_cbc,(uint8_t*)plain_cbc_256,sizeof(plain_cbc_256));
    HAL_AES_SetKey_U8(key_cbc_256, AES_KEY_256, AES_SWAP_ENABLE);	  //256bit key
    HAL_AES_Crypt_U8(temp_cbc, temp_cbc, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_256, mode);
    
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(temp_cbc[i] != cipher_cbc_256[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt_U8(temp_cbc, temp_cbc, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_256, mode);
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(temp_cbc[i] != plain_cbc_256[i])
    	{
    		return 1;
    	}
    }

  //-----------------------    CBC mode (swap)   ---------------------
	mode = AES_NORMAL_MODE;
//  mode = AES_SECURITY_MODE;
                                             
	blk_len = sizeof(plain_cbc_128) / 16;

	swap((UINT32 *)key_cbc_128,4);            //key_ecb[0] = 0xa4d14ae0,...,key_ecb[3] = 0x6a63a25d
	swap((UINT32 *)iv_cbc_128,4);
	swap((UINT32 *)plain_cbc_128,blk_len*4);
	swap((UINT32 *)cipher_cbc_128,blk_len*4);	
	
	memcpy((uint8_t*)temp_cbc,(uint8_t*)plain_cbc_128,sizeof(plain_cbc_128));
    HAL_AES_SetKey_U8(key_cbc_128, AES_KEY_128, AES_SWAP_DISABLE);	//128bit key
    HAL_AES_Crypt_U8(temp_cbc, temp_cbc, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_128, mode);
    
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(temp_cbc[i] != cipher_cbc_128[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt_U8(temp_cbc, temp_cbc, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_128, mode);
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(temp_cbc[i] != plain_cbc_128[i])
    	{
    		return 1;
    	}
    }   
		
	swap((UINT32 *)key_cbc_128,4);            //key_ecb[0] = 0xa4d14ae0,...,key_ecb[3] = 0x6a63a25d
	swap((UINT32 *)iv_cbc_128,4);
	swap((UINT32 *)plain_cbc_128,blk_len*4);
	swap((UINT32 *)cipher_cbc_128,blk_len*4);		
		
	mode = AES_NORMAL_MODE;
//  mode = AES_SECURITY_MODE;
                                             
	blk_len = sizeof(plain_cbc_256) / 16;

	swap((UINT32 *)key_cbc_256,8);            //key_ecb[0] = 0xa4d14ae0,...,key_ecb[3] = 0x6a63a25d
	swap((UINT32 *)iv_cbc_256,4);
	swap((UINT32 *)plain_cbc_256,blk_len*4);
	swap((UINT32 *)cipher_cbc_256,blk_len*4);	

	memcpy((uint8_t*)temp_cbc,(uint8_t*)plain_cbc_256,sizeof(plain_cbc_256));
    HAL_AES_SetKey_U8(key_cbc_256, AES_KEY_256, AES_SWAP_DISABLE);	//128bit key
    HAL_AES_Crypt_U8(temp_cbc, temp_cbc, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_256, mode);
    
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(temp_cbc[i] != cipher_cbc_256[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt_U8(temp_cbc, temp_cbc, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_256, mode);
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(temp_cbc[i] != plain_cbc_256[i])
    	{
    		return 1;
    	}
    }     
	swap((UINT32 *)key_cbc_256,8);            //key_ecb[0] = 0xa4d14ae0,...,key_ecb[3] = 0x6a63a25d
	swap((UINT32 *)iv_cbc_256,4);
	swap((UINT32 *)plain_cbc_256,blk_len*4);
	swap((UINT32 *)cipher_cbc_256,blk_len*4);	
//---------------------security-----------------
//  128
//	mode = AES_NORMAL_MODE;
	mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_cbc_128) / 16;
		
	memcpy((uint8_t*)temp_cbc,(uint8_t*)plain_cbc_128,sizeof(plain_cbc_128));		
    HAL_AES_SetKey_U8(key_cbc_128, AES_KEY_128, AES_SWAP_ENABLE);	  //128bit key
    HAL_AES_Crypt_U8(temp_cbc, temp_cbc, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_128, mode);
    
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(temp_cbc[i] != cipher_cbc_128[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt_U8(temp_cbc, temp_cbc, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_128, mode);
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(temp_cbc[i] != plain_cbc_128[i])
    	{
    		return 1;
    	}
    }

//  192
//  mode = AES_NORMAL_MODE;
    mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_cbc_192) / 16;
	memcpy((uint8_t*)temp_cbc,(uint8_t*)plain_cbc_192,sizeof(plain_cbc_192));		
    HAL_AES_SetKey_U8(key_cbc_192, AES_KEY_192, AES_SWAP_ENABLE);	  //192bit key
    HAL_AES_Crypt_U8(temp_cbc, temp_cbc, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_192, mode);
    
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(temp_cbc[i] != cipher_cbc_192[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt_U8(temp_cbc, temp_cbc, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_192, mode);
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(temp_cbc[i] != plain_cbc_192[i])
    	{
    		return 1;
    	}
    }

//  256
//	mode = AES_NORMAL_MODE;
    mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_cbc_256) / 16;

	memcpy((uint8_t*)temp_cbc,(uint8_t*)plain_cbc_256,sizeof(plain_cbc_256));				
    HAL_AES_SetKey_U8(key_cbc_256, AES_KEY_256, AES_SWAP_ENABLE);	  //256bit key
    HAL_AES_Crypt_U8(temp_cbc, temp_cbc, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_256, mode);
    
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(temp_cbc[i] != cipher_cbc_256[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt_U8(temp_cbc, temp_cbc, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_256, mode);
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(temp_cbc[i] != plain_cbc_256[i])
    	{
    		return 1;
    	}
    }

  //-----------------------    CBC mode (swap)   ---------------------
//	mode = AES_NORMAL_MODE;
    mode = AES_SECURITY_MODE;
                                             
	blk_len = sizeof(plain_cbc_128) / 16;

	swap((UINT32 *)key_cbc_128,4);            //key_ecb[0] = 0xa4d14ae0,...,key_ecb[3] = 0x6a63a25d
	swap((UINT32 *)iv_cbc_128,4);
	swap((UINT32 *)plain_cbc_128,blk_len*4);
	swap((UINT32 *)cipher_cbc_128,blk_len*4);	
	
	memcpy((uint8_t*)temp_cbc,(uint8_t*)plain_cbc_128,sizeof(plain_cbc_128));				

    HAL_AES_SetKey_U8(key_cbc_128, AES_KEY_128, AES_SWAP_DISABLE);	//128bit key
    HAL_AES_Crypt_U8(temp_cbc, temp_cbc, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_128, mode);
    
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(temp_cbc[i] != cipher_cbc_128[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt_U8(temp_cbc, temp_cbc, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_128, mode);
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(temp_cbc[i] != plain_cbc_128[i])
    	{
    		return 1;
    	}
    }        
//	mode = AES_NORMAL_MODE;
    mode = AES_SECURITY_MODE;
                                             
	blk_len = sizeof(plain_cbc_256) / 16;

	swap((UINT32 *)key_cbc_256,8);            //key_ecb[0] = 0xa4d14ae0,...,key_ecb[3] = 0x6a63a25d
	swap((UINT32 *)iv_cbc_256,4);
	swap((UINT32 *)plain_cbc_256,blk_len*4);
	swap((UINT32 *)cipher_cbc_256,blk_len*4);	

	memcpy((uint8_t*)temp_cbc,(uint8_t*)plain_cbc_256,sizeof(plain_cbc_256));	
    HAL_AES_SetKey_U8(key_cbc_256, AES_KEY_256, AES_SWAP_DISABLE);	//128bit key
    HAL_AES_Crypt_U8(temp_cbc, temp_cbc, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_256, mode);
    
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(temp_cbc[i] != cipher_cbc_256[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt_U8(temp_cbc, temp_cbc, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_256, mode);
    for( i = 0; i < (blk_len*4*4); i ++)
    {
    	if(temp_cbc[i] != plain_cbc_256[i])
    	{
    		return 1;
    	}
    }  		
	//-----------------------   End of  CBC mode   ---------------------
	return 0;
}


UINT32 aes_inout_test(void)   //CBC Test
{
	UINT32 mode;
	UINT32 i;
	UINT32 blk_len;

//  128	
	UINT32 key_cbc_128[4]     = {0xe04ad1a4, 0x65fe706b, 0x80cf2960, 0x5da2636a};  //key_cbc    = 0xe04ad1a4 65fe706b 80cf2960 5da2636a
	UINT32 iv_cbc_128[4]      = {0xc42821e1, 0x3ac9b622, 0x72304ad5, 0xe5f5937d};  //iv_cbc     = 0xc42821e1 3ac9b622 72304ad5 e5f5937d  
//  plain_cbc  = 0xf38d8adf 367b0c53 1d273eb7 60632039...
	UINT32 plain_cbc_128[20]   = {0xf38d8adf, 0x367b0c53, 0x1d273eb7, 0x60632039, 
							 0x7e314c2f, 0xa1ab5e18, 0x79aa587e, 0xb834e5de, 
							 0x51d45354, 0xafcebfd6, 0x7a9cd97c, 0x5be67590, 
							 0x40f27cfd, 0xc60d77f7, 0xf923668d, 0xb2fde000, 
							 0x20020634, 0x3fee8224, 0x097ed2f3, 0x4f872281};  
//  cipher_cbc = 0x099ed143 4af12194 c2cb0351 075ea970...
	UINT32 cipher_cbc_128[20]  = {0x099ed143, 0x4af12194, 0xc2cb0351, 0x075ea970, 
							  0x59c1e350, 0x9c5bcc75, 0x34503402, 0xe1ad79a9, 
							  0xae2bd3d2, 0x4ec9e23d, 0x35162f55, 0x993a1b83, 
							  0x35779406, 0x00d30e40, 0xc28639a1, 0x4ea39e14, 
							  0x5a94dd9d, 0x80adf249, 0x863da72a, 0x71021e78};  
//  192
	UINT32 key_cbc_192[6]     = {0xe04ad1a4, 0x65fe706b, 0x80cf2960, 0x5da2636a, 0xc42821e1, 0x3ac9b622};  // key_ecb    = 0xe04ad1a4 65fe706b 80cf2960 5da2636a c42821e1 3ac9b622
	UINT32 iv_cbc_192[4]      = {0x72304ad5, 0xe5f5937d, 0xf38d8adf, 0x367b0c53};  //iv_cbc    = 0x72304ad5 e5f5937d f38d8adf 367b0c53
	UINT32 plain_cbc_192[20]   = {0x1d273eb7, 0x60632039, 0x7e314c2f, 0xa1ab5e18, 
							  0x79aa587e, 0xb834e5de, 0x51d45354, 0xafcebfd6, 
							  0x7a9cd97c, 0x5be67590, 0x40f27cfd, 0xc60d77f7, 
							  0xf923668d, 0xb2fde000, 0x20020634, 0x3fee8224, 
							  0x097ed2f3, 0x4f872281, 0x4329f38c, 0x6c564821}; //plain_cbc  = 0x1d273eb7 60632039 7e314c2f a1ab5e18...
	UINT32 cipher_cbc_192[20]  = {0x4c9abdee, 0x35097f17, 0x37de6468, 0x0a14af58, 
							  0x20e48235, 0x31a46420, 0x93a283f4, 0x7e982a45, 
							  0x9e15fd81, 0xf80f2328, 0x0701d24d, 0x4c7bcb79, 
							  0xbcfd3d10, 0xdc3dceb2, 0x800fa9d1, 0xadc513f9, 
							  0xfa161563, 0x3c65e394, 0xbad169b6, 0xac2f04cd}; //cipher_cbc = 0x4c9abdee 35097f17 37de6468 0a14af58...

//  256
	UINT32 key_cbc_256[8]     = {0xe04ad1a4, 0x65fe706b, 0x80cf2960, 0x5da2636a, 0xc42821e1, 0x3ac9b622, 0x72304ad5, 0xe5f5937d};  // key_ecb    = 0xe04ad1a4 65fe706b 80cf2960 5da2636a c42821e1 3ac9b622 72304ad5 e5f5937d
	UINT32 iv_cbc_256[4]      = {0xf38d8adf, 0x367b0c53, 0x1d273eb7, 0x60632039};  // iv_cbc    = 0xf38d8adf 367b0c53 1d273eb7 60632039;
	UINT32 plain_cbc_256[20]   = {0x7e314c2f, 0xa1ab5e18, 0x79aa587e, 0xb834e5de,
							 0x51d45354, 0xafcebfd6, 0x7a9cd97c, 0x5be67590, 
							 0x40f27cfd, 0xc60d77f7, 0xf923668d, 0xb2fde000,
							 0x20020634, 0x3fee8224, 0x097ed2f3, 0x4f872281,
							 0x4329f38c, 0x6c564821, 0x6e897e46, 0x25ac03f3};  // plain_cbc  = 0x7e314c2f a1ab5e18 79aa587e b834e5de...
	UINT32 cipher_cbc_256[20]  = {0xc8324f39, 0xcf6969a7, 0x35f97dd9, 0x34a90318,
							 0x72866a00, 0x2fd8e17d, 0xf8003085, 0x4e796949,
							 0xcd70af28, 0xe13be0d2, 0x2d613def, 0x8da350f6,
							 0x7107db84, 0x1c3f5585, 0xc77b89dc, 0x4ccd5abc,
							 0x307463b8, 0x5098659e, 0xdd3c2694, 0xedd32cdb};  // cipher_cbc = 0xc8324f39 cf6969a7 35f97dd9 34a90318...

	UINT32 temp[20];
    //-----------------------    CBC mode (no swap)   ---------------------
//  128
	mode = AES_NORMAL_MODE;
//  mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_cbc_128) / 16;
	
	memcpy((uint8_t*)temp,(uint8_t*)plain_cbc_128,sizeof(plain_cbc_128));
    HAL_AES_SetKey(key_cbc_128, AES_KEY_128, AES_SWAP_DISABLE);	  //128bit key
    HAL_AES_Crypt(temp, temp, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_128, mode);
    
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(temp[i] != cipher_cbc_128[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt(temp, temp, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_128, mode);
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(temp[i] != plain_cbc_128[i])
    	{
    		return 1;
    	}
    }

//  192
	mode = AES_NORMAL_MODE;
//  mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_cbc_192) / 16;
		
	memcpy((uint8_t*)temp,(uint8_t*)plain_cbc_192,sizeof(plain_cbc_192));

    HAL_AES_SetKey(key_cbc_192, AES_KEY_192, AES_SWAP_DISABLE);	  //192bit key
    HAL_AES_Crypt(temp, temp, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_192, mode);
    
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(temp[i] != cipher_cbc_192[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt(temp, temp, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_192, mode);
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(temp[i] != plain_cbc_192[i])
    	{
    		return 1;
    	}
    }

//  256
	mode = AES_NORMAL_MODE;
//  mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_cbc_256) / 16;

	memcpy((uint8_t*)temp,(uint8_t*)plain_cbc_256,sizeof(plain_cbc_256));
		
    HAL_AES_SetKey(key_cbc_256, AES_KEY_256, AES_SWAP_DISABLE);	  //256bit key
    HAL_AES_Crypt(temp, temp, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_256, mode);
    
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(temp[i] != cipher_cbc_256[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt(temp, temp, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_256, mode);
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(temp[i] != plain_cbc_256[i])
    	{
    		return 1;
    	}
    }

  //-----------------------    CBC mode (swap)   ---------------------
	mode = AES_NORMAL_MODE;
//  mode = AES_SECURITY_MODE;
                                             
	blk_len = sizeof(plain_cbc_128) / 16;

	swap(key_cbc_128,4);            //key_ecb[0] = 0xa4d14ae0,...,key_ecb[3] = 0x6a63a25d
	swap(iv_cbc_128,4);
	swap(plain_cbc_128,blk_len*4);
	swap(cipher_cbc_128,blk_len*4);	

	memcpy((uint8_t*)temp,(uint8_t*)plain_cbc_128,sizeof(plain_cbc_128));

    HAL_AES_SetKey(key_cbc_128, AES_KEY_128, AES_SWAP_ENABLE);	//128bit key
    HAL_AES_Crypt(temp, temp, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_128, mode);
    
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(temp[i] != cipher_cbc_128[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt(temp, temp, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_128, mode);
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(temp[i] != plain_cbc_128[i])
    	{
    		return 1;
    	}
    }   
	swap(key_cbc_128,4);            //key_ecb[0] = 0xa4d14ae0,...,key_ecb[3] = 0x6a63a25d
	swap(iv_cbc_128,4);
	swap(plain_cbc_128,blk_len*4);
	swap(cipher_cbc_128,blk_len*4);	
		
	mode = AES_NORMAL_MODE;
//  mode = AES_SECURITY_MODE;
                                             
	blk_len = sizeof(plain_cbc_256) / 16;

	swap(key_cbc_256,8);            //key_ecb[0] = 0xa4d14ae0,...,key_ecb[3] = 0x6a63a25d
	swap(iv_cbc_256,4);
	swap(plain_cbc_256,blk_len*4);
	swap(cipher_cbc_256,blk_len*4);	

	memcpy((uint8_t*)temp,(uint8_t*)plain_cbc_256,sizeof(plain_cbc_256));

    HAL_AES_SetKey(key_cbc_256, AES_KEY_256, AES_SWAP_ENABLE);	//128bit key
    HAL_AES_Crypt(temp, temp, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_256, mode);
    
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(temp[i] != cipher_cbc_256[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt(temp, temp, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_256, mode);
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(temp[i] != plain_cbc_256[i])
    	{
    		return 1;
    	}
    }  
	swap(key_cbc_256,8);            //key_ecb[0] = 0xa4d14ae0,...,key_ecb[3] = 0x6a63a25d
	swap(iv_cbc_256,4);
	swap(plain_cbc_256,blk_len*4);
	swap(cipher_cbc_256,blk_len*4);			
//--------------------security-------------------

   //-----------------------    CBC mode (no swap)   ---------------------
//  128
//	mode = AES_NORMAL_MODE;
    mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_cbc_128) / 16;

	memcpy((uint8_t*)temp,(uint8_t*)plain_cbc_128,sizeof(plain_cbc_128));
		
    HAL_AES_SetKey(key_cbc_128, AES_KEY_128, AES_SWAP_DISABLE);	  //128bit key
    HAL_AES_Crypt(temp, temp, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_128, mode);
    
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(temp[i] != cipher_cbc_128[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt(temp, temp, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_128, mode);
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(temp[i] != plain_cbc_128[i])
    	{
    		return 1;
    	}
    }

//  192
//	mode = AES_NORMAL_MODE;
	mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_cbc_192) / 16;

	memcpy((uint8_t*)temp,(uint8_t*)plain_cbc_192,sizeof(plain_cbc_192));
		
    HAL_AES_SetKey(key_cbc_192, AES_KEY_192, AES_SWAP_DISABLE);	  //192bit key
    HAL_AES_Crypt(temp, temp, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_192, mode);
    
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(temp[i] != cipher_cbc_192[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt(temp, temp, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_192, mode);
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(temp[i] != plain_cbc_192[i])
    	{
    		return 1;
    	}
    }

//  256
//	mode = AES_NORMAL_MODE;
    mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_cbc_256) / 16;

	memcpy((uint8_t*)temp,(uint8_t*)plain_cbc_256,sizeof(plain_cbc_256));
		
    HAL_AES_SetKey(key_cbc_256, AES_KEY_256, AES_SWAP_DISABLE);	  //256bit key
    HAL_AES_Crypt(temp, temp, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_256, mode);
    
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(temp[i] != cipher_cbc_256[i])
    	{
    		return 1;
    	}
    }
  
    HAL_AES_Crypt(temp, temp, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_256, mode);
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(temp[i] != plain_cbc_256[i])
    	{
    		return 1;
    	}
    }

  //-----------------------    CBC mode (swap)   ---------------------
//	mode = AES_NORMAL_MODE;
    mode = AES_SECURITY_MODE;
                                             
	blk_len = sizeof(plain_cbc_128) / 16;

	swap(key_cbc_128,4);            //key_ecb[0] = 0xa4d14ae0,...,key_ecb[3] = 0x6a63a25d
	swap(iv_cbc_128,4);
	swap(plain_cbc_128,blk_len*4);
	swap(cipher_cbc_128,blk_len*4);	

	memcpy((uint8_t*)temp,(uint8_t*)plain_cbc_128,sizeof(plain_cbc_128));

    HAL_AES_SetKey(key_cbc_128, AES_KEY_128, AES_SWAP_ENABLE);	//128bit key
    HAL_AES_Crypt(temp, temp, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_128, mode);
    
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(temp[i] != cipher_cbc_128[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt(temp, temp, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_128, mode);
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(temp[i] != plain_cbc_128[i])
    	{
    		return 1;
    	}
    }        
//	mode = AES_NORMAL_MODE;
    mode = AES_SECURITY_MODE;
                                             
	blk_len = sizeof(plain_cbc_256) / 16;

	swap(key_cbc_256,8);            //key_ecb[0] = 0xa4d14ae0,...,key_ecb[3] = 0x6a63a25d
	swap(iv_cbc_256,4);
	swap(plain_cbc_256,blk_len*4);
	swap(cipher_cbc_256,blk_len*4);	

	memcpy((uint8_t*)temp,(uint8_t*)plain_cbc_256,sizeof(plain_cbc_256));

    HAL_AES_SetKey(key_cbc_256, AES_KEY_256, AES_SWAP_ENABLE);	//128bit key
    HAL_AES_Crypt(temp, temp, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_256, mode);
    
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(temp[i] != cipher_cbc_256[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt(temp, temp, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_256, mode);
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(temp[i] != plain_cbc_256[i])
    	{
    		return 1;
    	}
    }   
	//-----------------------   End of  CBC mode   ---------------------
	return 0;
}



void aes_test(void)
{
	if(aes_ecb_test())
		printfS(" AES ECB Test 32 Fail!\r\n");
	else
		printfS(" AES ECB Test 32 Pass!\r\n");

	if(aes_cbc_test())
		printfS(" AES CBC Test 32 Fail!\r\n");
	else
		printfS(" AES CBC Test 32 Pass!\r\n\n");	
	
	if(aes_ecb_test_u8())
		printfS(" AES ECB Test 8 Fail!\r\n");
	else
		printfS(" AES ECB Test 8 Pass!\r\n");

	if(aes_cbc_test_u8())
		printfS(" AES CBC Test 8 Fail!\r\n");
	else
		printfS(" AES CBC Test 8 Pass!\r\n\n");	
	
	if(aes_inout_test())
		printfS(" AES inout Test 32 Fail!\r\n");
	else
		printfS(" AES inout Test 32 Pass!\r\n");	
	if(aes_inout_test_u8())
		printfS(" AES inout Test 8 Fail!\r\n");
	else
		printfS(" AES inout Test 8 Pass!\r\n\n");	

}




