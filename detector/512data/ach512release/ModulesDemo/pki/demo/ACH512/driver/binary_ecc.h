 /***********************************************************************
 * Copyright (c)  2012,Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.

 * Filename    : ecc.h
 * Description : define some function and variable in ecc.c;;
 * Author(s)   : lhm  
 * version     : 1.0
 * Modify date : 2012-03-02 13:49, first edition
 ***********************************************************************/
#ifndef _BINARY_ECC_H
#define _BINARY_ECC_H

/**********************************************************
*	definitions
**********************************************************/	
//#define _ECC_571
//#define _ECC_409
//#define _ECC_283
//#define _ECC_239
//#define _ECC_233
//#define _ECC_193
#define _ECC_163

#define CNST_BINARY_EXP 0x70
#define CNST_BINARY_MUL 0x41
#define CNST_BINARY_POINT_ADD 0x64
#define CNST_BINARY_POINT_SUB 0x68

/**********************************************************
*	include files
**********************************************************/
#include "common.h"

/**********************************************************
*	extern variable
***********************************************************/


/**********************************************************
*	extern functions
***********************************************************/

/*********************************************************************************
* Function Name  : BINARY_ECC_precal
* Description    : ecc pre calculate R^2 mod N
* Input          : - *n_data     : n,modular for ecc or rsa operation
				 : - n_length    : length of n_data, in words
* Output         : NONE
* Return         : NONE
*********************************************************************************/
void BINARY_ECC_precal(UINT32 *n_data,	UINT8 n_length);

/*********************************************************************************
* Function Name  : BINARY_ECC_mul_me
* Description    : rsa modexp modmul calculate:out_data=(in_data^e_data) mod n_data
*                :                             out_data=(in_data*e_data) mod n_data
* Input          : - *in_data    : input number for RSA modular exp operation
				 : - in_length   : length of in_data, in words
				 : - *e_data     : exponent for RSA modular exp operation
				 : - e_length    : length of e_data, in words
				 : - *n_data     : n,modular for RSA modular exp operation
				 : - n_length    : length of n_data, in words
				 : - mode        : 0x30: modexp; 0x01: modmul
* Output         : - *out_data   : result for RSA modular exp operation
				 : - *out_length : length of output, in words 
* Return         : NONE
*********************************************************************************/
void BINARY_ECC_mul_me(     
	UINT32 *in_data, 	
	UINT8 in_length,	
	UINT32 *e_data,		
	UINT8 e_length,	 
	UINT32 *n_data,		
	UINT8 n_length,		 
	UINT32 *out_data,	
	UINT8 *out_length,
	UINT8 mode	
	);
/*********************************************************************************
* Function Name  : BINARY_ECC_mod_sqr
* Description    : mod square calculate:out_data=(in_data*in_data) mod n_data
* Input          : - *in_data    : input number for modular sqr operation
				 : - in_length   : length of in_data, in words
				 : - *n_data     : n,modular for modular sqr operation
				 : - n_length    : length of n_data, in words
* Output         : - *out_data   : result for modular sqr operation
				 : - *out_length : length of output, in words 
* Return         : NONE
*********************************************************************************/
void BINARY_ECC_mod_sqr(    
	UINT32 *in_data, 	
	UINT8 in_length,	
	UINT32 *n_data,		
	UINT8 n_length,		
	UINT32 *out_data,		
	UINT8 *out_length
	);

/*********************************************************************************
* Function Name  : BINARY_ECC_mod_add
* Description    : ecc modadd calculate:out_data=(a_data+b_data) mod n_data	
* Input          : - *a_data    : input number for ecc modular add operation
				 : - a_length   : length of in_data, in words
				 : - *b_data     : exponent for ecc modular add operation
				 : - b_length    : length of e_data, in words
				 : - *n_data     : n,modular for ecc modular add operation
				 : - n_length    : length of n_data, in words
* Output         : - *out_data   : result for ecc modular add operation
				 : - *out_length : length of output, in words 
* Return         : NONE
*********************************************************************************/
void BINARY_ECC_mod_add(     
	UINT32 *a_data, 	
	UINT8 a_length,	
	UINT32 *b_data,		
	UINT8 b_length,		
	UINT32 *n_data,		
	UINT8 n_length,	
	UINT32 *out_data,	
	UINT8 *out_length
	);
/*********************************************************************************
* Function Name  : BINARY_ECC_PM
* Description    : ecc point multiple
* Input          : - p_ecc_para    : the struct of ecc curve parameter point
				 : - *k_data       : the big number k
				 : - *X0_data      : the operation point x coordinate
				 : - *Y0_data      : the operation point y coordinate				
* Output         : - *pmx_data     : the result point x coordinate
				 : - *pmy_data     : the result point y coordinate
* Return         : 0: PM success; 1: result is infinite point or point add error
*********************************************************************************/
UINT8 BINARY_ECC_PM(ECC_G_STR *p_ecc_para,UINT32 *k_data,UINT32 *X0_data,UINT32 *Y0_data,UINT32 *pmx_data,UINT32 *pmy_data);
/*********************************************************************************
* Function Name  : BINARY_ECC_PA_PS
* Description    : ecc point addition(p0+p1) or subtration(p1-p0)  
* Input          : - p_ecc_para    : the struct of ecc curve parameter point
				 : - cmd           : 0x24: point add; 0x28: point sub
				 : - *X0_data      : x coordinate of p0 
				 : - *Y0_data      : y coordinate of p0
				 : - *X1_data      : x coordinate of p1 
				 : - *Y1_data      : y coordinate of p1
* Output         : - *x_data       : the result point x coordinate
				 : - *y_data       : the result point y coordinate
* Return         : 0: PA or PS success; 1: result is infinite point or point add error
*********************************************************************************/
UINT8 BINARY_ECC_PA_PS(ECC_G_STR *p_ecc_para,UINT32 cmd,UINT32 *X0_data,UINT32 *Y0_data,UINT32 *X1_data,UINT32 *Y1_data,UINT32 *x_data,UINT32 *y_data);
/*********************************************************************************
* Function Name  : BINARY_ECC_PD
* Description    : ecc double point (2*p0) 
* Input          : - p_ecc_para    : the struct of ecc curve parameter point
				 : - *X0_data      : x coordinate of p0	
				 : - *Y0_data      : y coordinate of p0				
* Output         : - *x_data       : the result point x coordinate
				 : - *y_data       : the result point y coordinate
* Return         : 0: PD success; 1: result is infinite point or point add error
*********************************************************************************/
UINT8 BINARY_ECC_PD(ECC_G_STR *p_ecc_para,UINT32 *X0_data,UINT32 *Y0_data,UINT32 *x_data,UINT32 *y_data) ;
/*********************************************************************************
* Function Name  : ECC_PJ
* Description    : ecc point p0 on curve judgement				  				 
* Input          : - p_ecc_para    : the struct of ecc curve parameter point
				 : - *X0_data      : x coordinate of p0	
				 : - *Y0_data      : y coordinate of p0 				
* Output         : NONE
* Return         : 0:point on curve; 1:point not on curve
*********************************************************************************/
UINT8 BINARY_ECC_PJ(ECC_G_STR *p_ecc_para,UINT32 *X0_data,UINT32 *Y0_data);


#endif	//_ECC_H


