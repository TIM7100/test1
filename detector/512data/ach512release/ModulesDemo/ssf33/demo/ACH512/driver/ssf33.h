/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : ssf33.h
 * Description : ssf33 driver header file
 * Author(s)   : Eric
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#ifndef __SF33_H__
#define __SF33_H__

#include "common.h"

#define SSF33_ENCRYPT		       1
#define SSF33_DECRYPT		       0
#define SSF33_EXTER			       1
#define SSF33_INTER 		       0

#define SSF33_SWAP_ENABLE      1
#define SSF33_SWAP_DISABLE     0

#define SSF33_PASS             0xabef5a58
#define SSF33_FAIL             0

#define SSF33_SECURITY_MODE    0
#define SSF33_NORMAL_MODE      0x5a3489a5

#define SSF33_ECB_MODE         0
#define SSF33_CBC_MODE         1

/************************************************************************
 * Name:	    ssf33_set_key
 * Function:  ssf33 set key
 * Input:
              ks		  --		pointer to buffer of key
              se_sel	--    1: external parameter
                            0: internal parameter
              para		--		pointer to buffer of parameter
              swap    --    SSF33_SWAP_ENABLE, SSF33_SWAP_DISABLE
 *Return: 	  None
************************************************************************/
void ssf33_set_key(UINT32 se_sel,UINT32 *para, UINT32 *ks,UINT32 swap);
	
/************************************************************************
 * Name:	    sf33_crypt
 * Function:  encrypt or decrypt operation of input data with CPU transfter data
 * Input:
              datain	    --		pointer to buffer of input data
              dataout	    --		pointer to buffer of output data
              block_len	  --		block length , unit is 128BIT
              enc	        --		1:encryption
                                0:decryption
              data_mode   --    SSF33_ECB_MODE, SSF33_CBC_MODE,
              iv          --    initial vector for CBC mode
              security_mode --  SSF33_SECURITY_MODE, SSF33_NORMAL_MODE
 *Return: 	  0xabef5a58    --  success
              0             --  fail
************************************************************************/
UINT32 ssf33_crypt_data(UINT32 * datain, UINT32 * dataout, UINT32 block_len, UINT32 enc, UINT32 data_mode, UINT32 iv[], UINT32 security_mode);

#endif
