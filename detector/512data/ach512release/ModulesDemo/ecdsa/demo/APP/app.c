/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : app.c
 * Description : application example source file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/

#include  "app.h"
#include  "ecc.h"
#include  "rsa_keygen.h"
#include  "ecdsa.h"
#include  "hrng.h"

//test ECDSA
#ifdef _ECC_521
UINT32 CurveLength = 17;
const UINT32 P_Array[17] = { 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x000001ff };
const UINT32 a_Array[17] = { 0xfffffffc, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x000001ff };
const UINT32 b_Array[17] = { 0x6b503f00, 0xef451fd4, 0x3d2c34f1, 0x3573df88, 0x3bb1bf07, 0x1652c0bd, 0xec7e937b, 0x56193951, 0x8ef109e1, 0xb8b48991, 0x99b315f3, 0xa2da725b, 0xb68540ee, 0x929a21a0, 0x8e1c9a1f, 0x953eb961, 0x00000051 };
const UINT32 N_Array[17] = { 0x91386409, 0xbb6fb71e, 0x899c47ae, 0x3bb5c9b8, 0xf709a5d0, 0x7fcc0148, 0xbf2f966b, 0x51868783, 0xfffffffa, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x000001ff };
const UINT32 BaseX_Array[17] = { 0xc2e5bd66, 0xf97e7e31, 0x856a429b, 0x3348b3c1, 0xa2ffa8de, 0xfe1dc127, 0xefe75928, 0xa14b5e77, 0x6b4d3dba, 0xf828af60, 0x053fb521, 0x9c648139, 0x2395b442, 0x9e3ecb66, 0x0404e9cd, 0x858e06b7, 0x000000c6 };
const UINT32 BaseY_Array[17] = { 0x9fd16650, 0x88be9476, 0xa272c240, 0x353c7086, 0x3fad0761, 0xc550b901, 0x5ef42640, 0x97ee7299, 0x273e662c, 0x17afbd17, 0x579b4468, 0x98f54449, 0x2c7d1bd9, 0x5c8a5fb4, 0x9a3bc004, 0x39296a78, 0x00000118 };

#endif

#ifdef _ECC_384
UINT32 CurveLength = 12;
const UINT32 P_Array[12] = { 0xffffffff, 0x00000000, 0x00000000, 0xffffffff, 0xfffffffe, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff };
const UINT32 a_Array[12] = { 0xfffffffc, 0x00000000, 0x00000000, 0xffffffff, 0xfffffffe, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff };
const UINT32 b_Array[12] = { 0xd3ec2aef, 0x2a85c8ed, 0x8a2ed19d, 0xc656398d, 0x5013875a, 0x0314088f, 0xfe814112, 0x181d9c6e, 0xe3f82d19, 0x988e056b, 0xe23ee7e4, 0xb3312fa7 };
const UINT32 N_Array[12] = { 0xCCC52973, 0xECEC196A, 0x48B0A77A, 0x581A0DB2, 0xF4372DDF, 0xC7634D81, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };
const UINT32 BaseX_Array[12] = { 0x72760ab7, 0x3a545e38, 0xbf55296c, 0x5502f25d, 0x82542a38, 0x59f741e0, 0x8ba79b98, 0x6e1d3b62, 0xf320ad74, 0x8eb1c71e, 0xbe8b0537, 0xaa87ca22 };
const UINT32 BaseY_Array[12] = { 0x90ea0e5f, 0x7a431d7c, 0x1d7e819d, 0x0a60b1ce, 0xb5f0b8c0, 0xe9da3113, 0x289a147c, 0xf8f41dbd, 0x9292dc29, 0x5d9e98bf, 0x96262c6f, 0x3617de4a };

#endif

#ifdef _ECC_256
UINT32 CurveLength = 8;
const UINT32 P_Array[8] = { 0xffffffff, 0xffffffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0xffffffff };
const UINT32 a_Array[8] = { 0xfffffffc, 0xffffffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0xffffffff };
const UINT32 b_Array[8] = { 0x27d2604b, 0x3bce3c3e, 0xcc53b0f6, 0x651d06b0, 0x769886bc, 0xb3ebbd55, 0xaa3a93e7, 0x5ac635d8 };
const UINT32 N_Array[8] = { 0xfc632551, 0xf3b9cac2, 0xa7179e84, 0xbce6faad, 0xffffffff, 0xffffffff, 0x00000000, 0xffffffff };
const UINT32 BaseX_Array[8] = { 0xd898c296, 0xf4a13945, 0x2deb33a0, 0x77037d81, 0x63a440f2, 0xf8bce6e5, 0xe12c4247, 0x6b17d1f2 };
const UINT32 BaseY_Array[8] = { 0x37bf51f5, 0xcbb64068, 0x6b315ece, 0x2bce3357, 0x7c0f9e16, 0x8ee7eb4a, 0xfe1a7f9b, 0x4fe342e2 };

#endif

#ifdef _ECC_224
UINT32 CurveLength = 7;
const UINT32 P_Array[7] = { 0x00000001, 0x00000000, 0x00000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff };
const UINT32 a_Array[7] = { 0xfffffffe, 0xffffffff, 0xffffffff, 0xfffffffe, 0xffffffff, 0xffffffff, 0xffffffff };
const UINT32 b_Array[7] = { 0x2355ffb4, 0x270b3943, 0xd7bfd8ba, 0x5044b0b7, 0xf5413256, 0x0c04b3ab, 0xb4050a85 };
const UINT32 N_Array[7] = { 0x5c5c2a3d, 0x13dd2945, 0xe0b8f03e, 0xffff16a2, 0xffffffff, 0xffffffff, 0xffffffff };
const UINT32 BaseX_Array[7] = { 0x115c1d21, 0x343280d6, 0x56c21122, 0x4a03c1d3, 0x321390b9, 0x6bb4bf7f, 0xb70e0cbd };
const UINT32 BaseY_Array[7] = { 0x85007e34, 0x44d58199, 0x5a074764, 0xcd4375a0, 0x4c22dfe6, 0xb5f723fb, 0xbd376388 };
#endif

#ifdef _ECC_192
UINT32 CurveLength = 6;
const UINT32 P_Array[6] = { 0xffffffff, 0xffffffff, 0xfffffffe, 0xffffffff, 0xffffffff, 0xffffffff };
const UINT32 a_Array[6] = { 0xfffffffc, 0xffffffff, 0xfffffffe, 0xffffffff, 0xffffffff, 0xffffffff };
const UINT32 b_Array[6] = { 0xc146b9b1, 0xfeb8deec, 0x72243049, 0x0fa7e9ab, 0xe59c80e7, 0x64210519 };
const UINT32 N_Array[6] = { 0xb4d22831, 0x146bc9b1, 0x99def836, 0xffffffff, 0xffffffff, 0xffffffff };
const UINT32 BaseX_Array[6] = { 0x82ff1012, 0xf4ff0afd, 0x43a18800, 0x7cbf20eb, 0xb03090f6, 0x188da80e };
const UINT32 BaseY_Array[6] = { 0x1e794811, 0x73f977a1, 0x6b24cdd5, 0x631011ed, 0xffc8da78, 0x07192b95 };
#endif

#ifdef _ECC_160
UINT32 CurveLength = 5;
const UINT32 P_Array[5] = { 0x9F3FF94F, 0x3AD0496B, 0x887653B5, 0x18C8DD29, 0xBF414E4D };
const UINT32 a_Array[5] = { 0x9F3FF94C, 0x3AD0496B, 0x887653B5, 0x18C8DD29, 0xBF414E4D };
const UINT32 b_Array[5] = { 0x95872863, 0xC73EDED1, 0x1020EFC3, 0x3C0584E5, 0x6C245E1F };
const UINT32 N_Array[5] = { 0xA48269A5, 0x3BF28F77, 0x6945F576, 0x00000000, 0x00000000 };
const UINT32 BaseX_Array[5] = { 0x92499C3C, 0x23706176, 0xB3495328, 0xA8AF9EE8, 0x59D02079 };
const UINT32 BaseY_Array[5] = { 0x4F490058, 0x7E0AECB8, 0x5A7F007B, 0xB05F780B, 0xBE20A6A8 };
#endif

UINT32 dKey[WL];
UINT32 QxKey[WL];
UINT32 QyKey[WL];
UINT32 Signr[WL];
UINT32 Signs[WL];
UINT32 hashdata0[WL];
UINT32 hashdata1[WL];
UINT8 error_code = 0;

UINT32 ECC_Sign_Test(ECC_G_STR *p_ecc_para, MATH_G_STR *p_math_str)
{
	UINT32 digest[WL] = { 0x11223344, 0x55667788, 0x99aabbcc, 0xddeeff00 };
	UINT32 digest1[WL] = { 0x11223344, 0x55667788, 0x99aabbcc, 0xddeeff01 };

	error_code = 0;

	//ECC signature demo starts:
	NN_AssignZero(QxKey, WL);
	NN_AssignZero(QyKey, WL);

	ECDSA_keypair(p_ecc_para, dKey, QxKey, QyKey);

	//1st step:Key Pair Generation Success!!;
	NN_AssignZero(Signr, WL);
	NN_AssignZero(Signs, WL);
	ECDSA_sign(p_ecc_para, p_math_str, digest, dKey, Signr, Signs);

	//2nd step:Sign digest Success!!
	//@digest hasn't changed!!
	if(!ECDSA_verify(p_ecc_para, p_math_str, digest, QxKey, QyKey, Signr, Signs))
	{
		//error_code |= 0;//Verification Success!!
		printfS("Verification Success!!!\n");
	}
	else
	{
		error_code |= 1; //Verification Failed!!
		printfS("Verification Failed!!!\n");
	}

	//@digest has changed!! should  Verification Failed
	if(!ECDSA_verify(p_ecc_para, p_math_str, digest1, QxKey, QyKey, Signr, Signs))
	{
		error_code |= 1; //Verification Success!!
		printfS("Verification should Failed!!!\n");
	}
	else
	{
		//error_code |= 0;//Verification Failed!!
		printfS("Verification should not Success!!!\n");
	}

	return error_code;
}


void ECDSA_test(void)
{
	ECC_G_STR ecc_glb_str;
	MATH_G_STR math_glb_str;
	
	enable_module(BIT_UAC | BIT_PKI |BIT_HRNG |BIT_HRNGS);
	
	ECC_para_initial((ECC_G_STR *)(&ecc_glb_str), CurveLength, (UINT32 *)P_Array, (UINT32 *)a_Array, (UINT32 *)b_Array, (UINT32 *)N_Array, (UINT32 *)BaseX_Array, (UINT32 *)BaseY_Array);

	//ECC sign  starts:
	printfS("\nECDSA_Sign_Test start!\n\n");
	error_code = ECC_Sign_Test((ECC_G_STR *)(&ecc_glb_str), (MATH_G_STR *)(&math_glb_str));
	if(error_code)
	{
		printfS("\nECDSA_Sign_Test end error!!!!\n");
	}
	else
	{
		printfS("\nECDSA_Sign_Test end success!!!!\n");
	}
	
	printfS("\nECDSA Test Over.\n");

}

//end test ECDSA
