/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : app.c
 * Description : application example source file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#include  "common.h"
#include  "app.h"
#include  "ecc.h"
#include  "rsa_keygen.h"

UINT32 g_data_buf[1024];

UINT32 N[NDIGITS];
UINT32 A[NDIGITS];
UINT32 B[NDIGITS];
UINT32 RSA_RESULT[NDIGITS]={0};
UINT32 EXPECT[NDIGITS]={0};
UINT32 seed_m;
UINT8  sw_m;
UINT8 g_Len;

void data_out_back(UINT32* out_data,UINT32 data_len)
{
	int i;
	
	printf("%x",out_data[data_len-1]);
	for(i=(data_len-2);i>=0;i--)
	{
		if(out_data[i] <= 0xfffffff)
			printf("0");
		if(out_data[i] <= 0xffffff)
			printf("0");
		if(out_data[i] <= 0xfffff)
			printf("0");
		if(out_data[i] <= 0xffff)
			printf("0");
		if(out_data[i] <= 0xfff)
			printf("0");
		if(out_data[i] <= 0xff)
			printf("0");
		if(out_data[i] <= 0xf)
			printf("0");
		printf("%x",out_data[i]);
	}
	printf("\n");
}

void lib_variable_initial(RSA_KEYGEN_G_STR *p_rsa_keygen_str)
{
	p_rsa_keygen_str ->RSA_n    =(g_data_buf+ 150);
	p_rsa_keygen_str ->RSA_d    =(g_data_buf+ 150+(NDIGITS));
	p_rsa_keygen_str ->RSA_e    =(g_data_buf+ 150+2*(NDIGITS));
	p_rsa_keygen_str ->RSA_p    =(g_data_buf+ 150+3*(NDIGITS));
	p_rsa_keygen_str ->RSA_q    =(g_data_buf+ 150+3*(NDIGITS)+(HALF_NDIGITS));
	p_rsa_keygen_str ->RSA_dp   =(g_data_buf+ 150+4*(NDIGITS));
	p_rsa_keygen_str ->RSA_dq   =(g_data_buf+ 150+4*(NDIGITS)+(HALF_NDIGITS));
	p_rsa_keygen_str ->RSA_qInv =(g_data_buf+ 150+5*(NDIGITS));
}

void rsa_keygen_demo(void)
{
	UINT8   error;
	UINT8   res_length;
	UINT32  ndigits;
	UINT8 i;
	RSA_G_STR rsa_glb_str;
	MATH_G_STR math_glb_str;
	RSA_KEYGEN_G_STR  rsa_keygen_glb_str;
	UINT32 rsa_mode;

	lib_variable_initial((RSA_KEYGEN_G_STR*)(&rsa_keygen_glb_str));	 
	//ndigits = 32;	//1024bit
	ndigits = 64;	//2048bit
	i=10;
	while(1)
	{
		i=64;
		while(i>2)
		{
		  ndigits = i;
			printf("%d\n",i);
	    error=0;
	   //使用CRT样例：
	   //产生CRT复杂密钥，然后签名验证  
	    NN_AssignZero(rsa_keygen_glb_str.RSA_e,ndigits);  //e
	    rsa_keygen_glb_str.RSA_e[0]=65537;
	 	if(!RSA_keygen_CRT((RSA_KEYGEN_G_STR*)(&rsa_keygen_glb_str),(MATH_G_STR *)(&math_glb_str),ndigits))
		{
				NN_AssignZero(A,ndigits); 			//A为明文，清零	
				gen_random_check(A,ndigits);             //随机产生明文
	   
	  		NN_Mod_variable_initial((MATH_G_STR *)(&math_glb_str),ndigits);
				NN_Mod((MATH_G_STR *)(&math_glb_str),EXPECT,A,ndigits,rsa_keygen_glb_str.RSA_n,ndigits);	 		//EXPECT保存明文
	 
			  NN_AssignZero(RSA_RESULT,ndigits);      //将RESULT清零
				
		    //RSA加密（验证）
				rsa_mul_me(A,ndigits,rsa_keygen_glb_str.RSA_e,1,rsa_keygen_glb_str.RSA_n,ndigits,RSA_RESULT,&res_length,CNST_RSA_EXP);     
				
				if(A[0]&0x01)
				{
					rsa_mode = 	RSA_SECURITY;
					printf("RSA_SECURITY\n");
				}
				else
				{
					rsa_mode = 	RSA_NORMAL;
					printf("RSA_NORMAL\n");
				}

				//RSA解密（快速签名）
				rsa_decrypt_CRT(RSA_RESULT,res_length,rsa_keygen_glb_str.RSA_p,ndigits/2,rsa_keygen_glb_str.RSA_q,ndigits/2,rsa_keygen_glb_str.RSA_dp,ndigits/2,rsa_keygen_glb_str.RSA_dq,ndigits/2,rsa_keygen_glb_str.RSA_qInv,ndigits/2,RSA_RESULT,&res_length,(RSA_G_STR*)(&rsa_glb_str),(MATH_G_STR *)(&math_glb_str),rsa_keygen_glb_str.RSA_e,1,rsa_mode);     
				
				if(!NN_EQUAL(RSA_RESULT,EXPECT,res_length))//比对解密出来的明文与原始明文
				{
					printf("CRT err!\n");
				}				
				
		}
		else  
		{
			printf("CRT GenKey err!\n");
			error =1;
		}
		if(!error)
		{
			printf("CRT success!\n");
		}	
	 	error= 0;
	 	//不使用CRT样例：
	 	//产生一般密钥，然后签名验证
	 	NN_AssignZero(rsa_keygen_glb_str.RSA_e,ndigits);  //e
	    rsa_keygen_glb_str.RSA_e[0]=65537;
		if(!RSA_keygen((RSA_KEYGEN_G_STR*)(&rsa_keygen_glb_str),(MATH_G_STR *)(&math_glb_str),ndigits)) //产生一般密钥
		{				
				NN_AssignZero(A,ndigits); 			//A为明文，清零	
				gen_random_check(A,ndigits);             //随机产生明文
	         
			 	NN_Mod_variable_initial((MATH_G_STR *)(&math_glb_str),ndigits);
				NN_Mod((MATH_G_STR *)(&math_glb_str),EXPECT,A,ndigits,rsa_keygen_glb_str.RSA_n,ndigits);	 		//EXPECT保存明文
		 		
			  NN_AssignZero(RSA_RESULT,ndigits);      //将RESULT清零
				
		        //RSA加密（验证） 
				rsa_mul_me(A,ndigits,rsa_keygen_glb_str.RSA_e,1,rsa_keygen_glb_str.RSA_n,ndigits,RSA_RESULT,&res_length,CNST_RSA_EXP);     
				
				//RSA解密（快速签名） 			
				rsa_mul_me(RSA_RESULT,res_length,rsa_keygen_glb_str.RSA_d,ndigits,rsa_keygen_glb_str.RSA_n,ndigits,RSA_RESULT,&res_length,CNST_RSA_EXP); 
				if(!NN_EQUAL(RSA_RESULT,EXPECT,res_length))//比对解密出来的明文与原始明文
				{
					printf("err!\n");	
				}					
		}
		else  
		{
			printf("GenKey err!\n");
			error =1;
		}
		if(!error)
		{
			printf("success!\n");	
		}
		i=i-2;	
	}
 }
}

void RSA_test(void) 
{
	enable_module(BIT_UAC | BIT_PKI |BIT_HRNG |BIT_HRNGS);
	
	printf("---------RSA test-------\n");
	
	clear_sram();
	rsa_keygen_demo();
	printf("---------RSA test end-------\n");
}
//end for RSA test



