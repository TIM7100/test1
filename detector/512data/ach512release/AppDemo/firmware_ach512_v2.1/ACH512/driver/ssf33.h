/******************************************************************************
* File Name:			ssf33.h
* Author:				AisinoChip
* Data First Issued:	2010-03-05
* Description:			Header fo ssf33.c
******************************************************************************/


#ifndef _SF33_H
#define _SF33_H

#include "common.h"
#include "hrng.h"

#define SSF33_ENCRYPT		1
#define SSF33_DECRYPT		0
#define SSF33_EXTER		1
#define SSF33_INTER 		0
#define SSF33_EMW_EN           (3 << 2)
#define EMW_START               1

#define SSF33_SWAP_ENABLE  1
#define SSF33_SWAP_DISABLE 0

#define SSF33_MASK_ENABLE 1
#define SSF33_MASK_DISABLE 0

#define SSF33_VIRTUAL_ENABLE      1
#define SSF33_VIRTUAL_DISABLE     0

#define SSF33_EMW_MODE        1
#define SSF33_CPU_MODE        0

#define VSSF33_ROUND_2        0
#define VSSF33_ROUND_4        1
#define VSSF33_ROUND_8        2
#define VSSF33_ROUND_16       3
#define VSSF33_ROUND_32       4
#define VSSF33_ROUND_64       5

#define SSF33_ECB_MODE         0
#define SSF33_CBC_MODE         1

#define SSF33_SECURITY_MODE    0
#define SSF33_NORMAL_MODE      0x5a3489a5

#define SSF33_PASS             0xabef5a58
#define SSF33_FAIL             0

/****************************************************************************** 
Name:		   ssf33_set_key
Function:	 ssf33_set_key sm4 key for encryption and decryption
Input:
           se_sel	   --    SSF33_EXTER or SSF33_INTER         	
		       para      --    buffer of external parameter
		       ks        --    buffer of key
           swap      --    SSF33_SWAP_ENABLE, SSF33_SWAP_DISABLE               
Return:	   None
*******************************************************************************/

void ssf33_set_key(UINT32 se_sel,UINT32 *para, UINT32 *ks,UINT32 swap);


/******************************************************************************

Name:		 ssf33_crypt
Function:	 
Input:
         datain		       --  pointer to buffer of input
         dataout	       --	pointer to buffer of result
         block_len	     --	block(128bit) length for  cryption
         operation	     --	SSF33_ENCRYPT, SSF33_DECRYPT
				 mode            --   SSF33_ECB_MODE, SSF33_CBC_MODE
				 iv              --   initial vector for CBC mode
         security_mode   --   SSF33_NORMAL_MODE, SSF33_SECURITY_MODE
Return:	 SSF33_PASS, SSF33_FAIL

*******************************************************************************/
UINT32 ssf33_crypt_data(
	UINT32 * datain, 
	UINT32 * dataout, 
	UINT32 block_len, 
	UINT32 operation, 
	UINT32 mode, 
	UINT32 iv[], 
	UINT32 security_mode);

#endif
