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
#include  "binary_ecc.h"

UINT32 g_data_buf[1024];

//for ecc test
UINT32 *P_Array;
UINT32 *a_Array;
UINT32 *b_Array;
UINT32 *N_Array;
UINT32 *BaseX_Array;
UINT32 *BaseY_Array;
UINT32 *ECC_P0X;
UINT32 *ECC_P0Y;
UINT32 *ECC_P1X;
UINT32 *ECC_P1Y;
UINT32 *ECC_k;
UINT32 *EXPECT_RESULTX;
UINT32 *EXPECT_RESULTY;
UINT32 *RESULTX;
UINT32 *RESULTY;
UINT32 CurveLength;

void data_out_back(UINT32 *out_data, UINT32 data_len)
{
	int i;

	printfS("0x%x", out_data[data_len - 1]);
	for(i = (data_len - 2); i >= 0; i--)
	{
		if(out_data[i] <= 0xfffffff) printfS("0");
		if(out_data[i] <= 0xffffff) printfS("0");
		if(out_data[i] <= 0xfffff) printfS("0");
		if(out_data[i] <= 0xffff) printfS("0");
		if(out_data[i] <= 0xfff) printfS("0");
		if(out_data[i] <= 0xff) printfS("0");
		if(out_data[i] <= 0xf) printfS("0");
		printfS("%x", out_data[i]);
	}
	printfS("\n");
}
void NN_AssignZero(UINT32 *a, UINT32 digits)
{
	if(digits)
	{
		do
		{
			*a++ = 0;
		}
		while(--digits);
	}
}
//test PM
void BINARY_PM_variable_init(void)
{

	P_Array[0] = 0x000000c9;
	P_Array[1] = 0x00000000;
	P_Array[2] = 0x00000000;
	P_Array[3] = 0x00000000;
	P_Array[4] = 0x00000000;
	P_Array[5] = 0x00000008;

	a_Array[0] = 0xd2782ae2;
	a_Array[1] = 0xbd88e246;
	a_Array[2] = 0x54ff8428;
	a_Array[3] = 0xefa84f95;
	a_Array[4] = 0xb6882caa;
	a_Array[5] = 0x00000007;

	b_Array[0] = 0xf958afd9;
	b_Array[1] = 0xca91f73a;
	b_Array[2] = 0x946bda29;
	b_Array[3] = 0xdcb40aab;
	b_Array[4] = 0x13612dcd;
	b_Array[5] = 0x00000007;

	ECC_k[0] = 0x103;
	ECC_k[1] = 0x0;
	ECC_k[2] = 0x0;
	ECC_k[3] = 0x0;
	ECC_k[4] = 0x0;
	ECC_k[5] = 0x0;

	ECC_P0X[0] = 0x7876a654;
	ECC_P0X[1] = 0x567f787a;
	ECC_P0X[2] = 0x89566789;
	ECC_P0X[3] = 0xab438977;
	ECC_P0X[4] = 0x69979697;
	ECC_P0X[5] = 0x00000003;

	ECC_P0Y[0] = 0xf41ff883;
	ECC_P0Y[1] = 0xe3c80988;
	ECC_P0Y[2] = 0x9d51fefc;
	ECC_P0Y[3] = 0xefafb298;
	ECC_P0Y[4] = 0x435edb42;
	ECC_P0Y[5] = 0x00000000;

	EXPECT_RESULTX[0] = 0xe6d48445;
	EXPECT_RESULTX[1] = 0x8232e779;
	EXPECT_RESULTX[2] = 0xb398f27d;
	EXPECT_RESULTX[3] = 0x3a8a5fa3;
	EXPECT_RESULTX[4] = 0x42d0f12a;
	EXPECT_RESULTX[5] = 0x00000000;


	EXPECT_RESULTY[0] = 0x09c41a03;
	EXPECT_RESULTY[1] = 0x1b657032;
	EXPECT_RESULTY[2] = 0x3887d7d3;
	EXPECT_RESULTY[3] = 0x38d05a64;
	EXPECT_RESULTY[4] = 0xf0885891;
	EXPECT_RESULTY[5] = 0x00000007;

}

void BINARY_ECC_PM_test(void)
{
	UINT8  result;
	ECC_G_STR ECC_glb_str;
	printfS("---------BINARY ECC PM test-------\n");

	result = 0;

	BINARY_PM_variable_init();
	CurveLength = 6;
	ECC_para_initial((ECC_G_STR *)(&ECC_glb_str), CurveLength, (UINT32 *)P_Array, (UINT32 *)a_Array, (UINT32 *)b_Array, (UINT32 *)N_Array, (UINT32 *)BaseX_Array, (UINT32 *)BaseY_Array);
	NN_AssignZero(RESULTX, 6);
	NN_AssignZero(RESULTY, 6);
	if(BINARY_ECC_PM((ECC_G_STR *)(&ECC_glb_str), ECC_k, ECC_P0X, ECC_P0Y, RESULTX, RESULTY))
	{
		printfS("BINARY ECC PM test fail 1 \n");
		return;
	}
	result = memcmp(EXPECT_RESULTX, RESULTX, 4 * CurveLength);
	result |= memcmp(EXPECT_RESULTY, RESULTY, 4 * CurveLength);
	if(result)
	{
		printfS("BINARY ECC PM  test fail 2\n");
		return;
	}
	else
	{
		printfS("BINARY ECC PM  test pass \n");
	}

	printfS("---------BINARY ECC PM test end-------\n");
}
//end test PM

//test PD
void BINARY_PD_variable_init(void)
{

	EXPECT_RESULTX[0] = 0xfc3672b4;
	EXPECT_RESULTX[1] = 0x87bcf6d6;
	EXPECT_RESULTX[2] = 0x62e3006a;
	EXPECT_RESULTX[3] = 0xad56a688;
	EXPECT_RESULTX[4] = 0xe1456ffe;
	EXPECT_RESULTX[5] = 0x00000004;


	EXPECT_RESULTY[0] = 0x87e528a8;
	EXPECT_RESULTY[1] = 0x764f48a7;
	EXPECT_RESULTY[2] = 0x1d903623;
	EXPECT_RESULTY[3] = 0xb164d4e5;
	EXPECT_RESULTY[4] = 0x23f5dd8a;
	EXPECT_RESULTY[5] = 0x00000002;

}
void BINARY_ECC_PD_test(void)
{
	UINT8  result;
	ECC_G_STR ECC_glb_str;

	printfS("---------BINARY ECC PD test-------\n");

	result = 0;

	BINARY_PM_variable_init();
	BINARY_PD_variable_init();
	CurveLength = 6;
	ECC_para_initial((ECC_G_STR *)(&ECC_glb_str), CurveLength, (UINT32 *)P_Array, (UINT32 *)a_Array, (UINT32 *)b_Array, (UINT32 *)N_Array, (UINT32 *)BaseX_Array, (UINT32 *)BaseY_Array);
	NN_AssignZero(RESULTX, CurveLength);
	NN_AssignZero(RESULTY, CurveLength);
	if(BINARY_ECC_PD((ECC_G_STR *)(&ECC_glb_str), ECC_P0X, ECC_P0Y, RESULTX, RESULTY))
	{
		printfS("BINARY ECC PD test fail \n");
		return;
	}

	result = memcmp(EXPECT_RESULTX, RESULTX, 4 * CurveLength);
	result |= memcmp(EXPECT_RESULTY, RESULTY, 4 * CurveLength);
	if(result)
	{
		printfS("BINARY ECC PD test fail \n");
		return;
	}
	else
	{
		printfS("BINARY ECC PD test pass \n");
	}

	printfS("---------BINARY ECC PD test end-------\n");
}
//end test PD

//test PA
void BINARY_PA_variable_init(void)
{

	ECC_P1X[0] = 0x15aa87d1;
	ECC_P1X[1] = 0x93381c67;
	ECC_P1X[2] = 0x3ea98cee;
	ECC_P1X[3] = 0xd53dfb02;
	ECC_P1X[4] = 0x8a0a8a89;
	ECC_P1X[5] = 0x00000004;

	ECC_P1Y[0] = 0xbae5d412;
	ECC_P1Y[1] = 0xdb5fe237;
	ECC_P1Y[2] = 0xff25554d;
	ECC_P1Y[3] = 0x1ad9ac2e;
	ECC_P1Y[4] = 0xbe5460da;
	ECC_P1Y[5] = 0x00000006;

	EXPECT_RESULTX[0] = 0x448f0057;
	EXPECT_RESULTX[1] = 0xc2d9320d;
	EXPECT_RESULTX[2] = 0x7f7c8bf2;
	EXPECT_RESULTX[3] = 0x23991253;
	EXPECT_RESULTX[4] = 0x580f74ee;
	EXPECT_RESULTX[5] = 0x00000006;


	EXPECT_RESULTY[0] = 0x428dccc9;
	EXPECT_RESULTY[1] = 0x1627d63c;
	EXPECT_RESULTY[2] = 0x09dac22a;
	EXPECT_RESULTY[3] = 0x09c6b649;
	EXPECT_RESULTY[4] = 0xe641d37c;
	EXPECT_RESULTY[5] = 0x00000007;
}
void BINARY_ECC_PA_test(void)
{
	UINT8  result;
	ECC_G_STR ECC_glb_str;

	printfS("---------BINARY ECC PA test-------\n");

	result = 0;

	BINARY_PM_variable_init();
	BINARY_PA_variable_init();
	CurveLength = 6;
	ECC_para_initial((ECC_G_STR *)(&ECC_glb_str), CurveLength, (UINT32 *)P_Array, (UINT32 *)a_Array, (UINT32 *)b_Array, (UINT32 *)N_Array, (UINT32 *)BaseX_Array, (UINT32 *)BaseY_Array);
	NN_AssignZero(RESULTX, CurveLength);
	NN_AssignZero(RESULTY, CurveLength);
	if(BINARY_ECC_PA_PS((ECC_G_STR *)(&ECC_glb_str), CNST_BINARY_POINT_ADD, ECC_P0X, ECC_P0Y, ECC_P1X, ECC_P1Y, RESULTX, RESULTY))
	{
		printfS("BINARY ECC PA test fail \n");
		return;
	}

	result = memcmp(EXPECT_RESULTX, RESULTX, 4 * CurveLength);
	result |= memcmp(EXPECT_RESULTY, RESULTY, 4 * CurveLength);
	if(result)
	{
		printfS("BINARY ECC PA test fail \n");
		return;
	}
	else
	{
		printfS("BINARY ECC PA test pass \n");
	}

	printfS("---------BINARY ECC PA test end-------\n");
}
//end test PA

//test PS
void BINARY_PS_variable_init(void)
{

	EXPECT_RESULTX[0] = 0xfc3672b4;
	EXPECT_RESULTX[1] = 0x87bcf6d6;
	EXPECT_RESULTX[2] = 0x62e3006a;
	EXPECT_RESULTX[3] = 0xad56a688;
	EXPECT_RESULTX[4] = 0xe1456ffe;
	EXPECT_RESULTX[5] = 0x00000004;

	EXPECT_RESULTY[0] = 0x87e528a8;
	EXPECT_RESULTY[1] = 0x764f48a7;
	EXPECT_RESULTY[2] = 0x1d903623;
	EXPECT_RESULTY[3] = 0xb164d4e5;
	EXPECT_RESULTY[4] = 0x23f5dd8a;
	EXPECT_RESULTY[5] = 0x00000002;
}

void BINARY_ECC_PS_test(void)
{
	UINT8  result;
	ECC_G_STR ECC_glb_str;

	printfS("---------BINARY ECC PS test-------\n");

	result = 0;

	BINARY_PM_variable_init();
	BINARY_PA_variable_init();
	BINARY_PS_variable_init();
	CurveLength = 6;
	ECC_para_initial((ECC_G_STR *)(&ECC_glb_str), CurveLength, (UINT32 *)P_Array, (UINT32 *)a_Array, (UINT32 *)b_Array, (UINT32 *)N_Array, (UINT32 *)BaseX_Array, (UINT32 *)BaseY_Array);
	NN_AssignZero(RESULTX, CurveLength);
	NN_AssignZero(RESULTY, CurveLength);
	if(BINARY_ECC_PA_PS((ECC_G_STR *)(&ECC_glb_str), CNST_BINARY_POINT_SUB, ECC_P0X, ECC_P0Y, ECC_P1X, ECC_P1Y, RESULTX, RESULTY))
	{
		printfS("BINARY ECC PS test fail \n");
		return;
	}

	result = memcmp(EXPECT_RESULTX, RESULTX, 4 * CurveLength);
	result |= memcmp(EXPECT_RESULTY, RESULTY, 4 * CurveLength);
	if(result)
	{
		printfS("BINARY ECC PS test fail \n");
		return;
	}
	else
	{
		printfS("BINARY ECC PS test pass \n");
	}

	printfS("---------BINARY ECC PS test end-------\n");
}
//end test PS
//test PJ

void BINARY_ECC_PJ_test(void)
{
//	UINT8  result;
	ECC_G_STR ECC_glb_str;

	printfS("---------BINARY ECC PJ test-------\n");

//	result = 0;

	BINARY_PM_variable_init();
	CurveLength = 6;
	ECC_para_initial((ECC_G_STR *)(&ECC_glb_str), CurveLength, (UINT32 *)P_Array, (UINT32 *)a_Array, (UINT32 *)b_Array, (UINT32 *)N_Array, (UINT32 *)BaseX_Array, (UINT32 *)BaseY_Array);
	if(!BINARY_ECC_PJ((ECC_G_STR *)(&ECC_glb_str), ECC_P0X, ECC_P0Y))
	{
		printfS("BINARY ECC PJ test pass \n");
	}
	else
	{
		printfS("BINARY ECC PJ test fail \n");
		return;
	}
	printfS("---------BINARY ECC PJ test end-------\n");
}
//end test PJ
//test MODME
void BINARY_modme_variable_init(void)
{
	P_Array[0] = 0x000000c9;
	P_Array[1] = 0x00000000;
	P_Array[2] = 0x00000000;
	P_Array[3] = 0x00000000;
	P_Array[4] = 0x00000000;
	P_Array[5] = 0x00000008;

	a_Array[0] = 0xa2c87f87;
	a_Array[1] = 0xde8e23ca;
	a_Array[2] = 0x51a60694;
	a_Array[3] = 0x47546dbc;
	a_Array[4] = 0x8deff45e;
	a_Array[5] = 0x00000003;

	b_Array[0] = 0x00000000;
	b_Array[1] = 0x00000000;
	b_Array[2] = 0x00000000;
	b_Array[3] = 0x00000000;
	b_Array[4] = 0x00000000;
	b_Array[5] = 0x00000008;

	EXPECT_RESULTX[0] = 0xa2c87f87;
	EXPECT_RESULTX[1] = 0xde8e23ca;
	EXPECT_RESULTX[2] = 0x51a60694;
	EXPECT_RESULTX[3] = 0x47546dbc;
	EXPECT_RESULTX[4] = 0x8deff45e;
	EXPECT_RESULTX[5] = 0x00000003;

}
void BINARY_ECC_MODME_test(void)
{
	UINT8  result;
	UINT8  x_len;

	printfS("---------BINARY ECC MODME test-------\n");
	result = 0;
	BINARY_modme_variable_init();
	CurveLength = 6;
	NN_AssignZero(RESULTX, CurveLength);
	BINARY_ECC_mul_me(a_Array, CurveLength, b_Array, CurveLength, P_Array, CurveLength, RESULTX, &x_len, CNST_BINARY_EXP);
	result = memcmp(EXPECT_RESULTX, RESULTX, 4 * CurveLength);
	if(result)
	{
		printfS("BINARY ECC MODME test fail \n");
		return;
	}
	else
	{
		printfS("BINARY ECC MODME test pass \n");
	}

	printfS("---------BINARY ECC MODME test end-------\n");
}
//end test MODME
//test MODMUL
void BINARY_modmul_variable_init(void)
{
	b_Array[0] = 0x0f5b31f5;
	b_Array[1] = 0xae9046ff;
	b_Array[2] = 0x160b62c2;
	b_Array[3] = 0x21ec00cf;
	b_Array[4] = 0x20fca806;
	b_Array[5] = 0x00000006;

	EXPECT_RESULTX[0] = 0x629307a6;
	EXPECT_RESULTX[1] = 0xafd6de6e;
	EXPECT_RESULTX[2] = 0x0c7ccd8f;
	EXPECT_RESULTX[3] = 0x07c65ebf;
	EXPECT_RESULTX[4] = 0x470380e6;
	EXPECT_RESULTX[5] = 0x00000006;
}
void BINARY_ECC_MODMUL_test(void)
{
	UINT8  result;
	UINT8  x_len;

	printfS("---------BINARY ECC MODMUL test-------\n");
	result = 0;
	BINARY_modme_variable_init();
	BINARY_modmul_variable_init();
	CurveLength = 6;
	NN_AssignZero(RESULTX, CurveLength);
	BINARY_ECC_mul_me(a_Array, CurveLength, b_Array, CurveLength, P_Array, CurveLength, RESULTX, &x_len, CNST_BINARY_MUL);
	result = memcmp(EXPECT_RESULTX, RESULTX, 4 * CurveLength);
	if(result)
	{
		printfS("BINARY ECC MODMUL test fail \n");
		return;
	}
	else
	{
		printfS("BINARY ECC MODMUL test pass \n");
	}

	printfS("---------BINARY ECC MODMUL test end-------\n");
}
//end test MODMUL
//test MODSQR
void BINARY_modsqr_variable_init(void)
{
	EXPECT_RESULTX[0] = 0x9e6a4930;
	EXPECT_RESULTX[1] = 0xb541423b;
	EXPECT_RESULTX[2] = 0x17eb7af7;
	EXPECT_RESULTX[3] = 0x5767cccb;
	EXPECT_RESULTX[4] = 0xefd0d830;
	EXPECT_RESULTX[5] = 0x00000007;
}
void BINARY_ECC_MODSQR_test(void)
{
	UINT8  result;
	UINT8  x_len;

	printfS("---------BINARY ECC MODSQR test-------\n");
	result = 0;
	BINARY_modme_variable_init();
	BINARY_modsqr_variable_init();
	CurveLength = 6;
	NN_AssignZero(RESULTX, CurveLength);
	BINARY_ECC_mod_sqr(a_Array, CurveLength, P_Array, CurveLength, RESULTX, &x_len);
	result = memcmp(EXPECT_RESULTX, RESULTX, 4 * CurveLength);
	if(result)
	{
		printfS("BINARY ECC MODSQR test fail \n");
		return;
	}
	else
	{
		printfS("BINARY ECC MODSQR test pass \n");
	}

	printfS("---------BINARY ECC MODSQR test end-------\n");
}
//end test MODSQR


//test MODMA
void BINARY_modma_variable_init(void)
{
	EXPECT_RESULTX[0] = 0xad934e72;
	EXPECT_RESULTX[1] = 0x701e6535;
	EXPECT_RESULTX[2] = 0x47ad6456;
	EXPECT_RESULTX[3] = 0x66b86d73;
	EXPECT_RESULTX[4] = 0xad135c58;
	EXPECT_RESULTX[5] = 0x00000005;

}
void BINARY_ECC_MODMA_test(void)
{
	UINT8  result;
	UINT8  x_len;

	printfS("---------BINARY ECC MODMA test-------\n");
	result = 0;
	BINARY_modme_variable_init();
	BINARY_modmul_variable_init();
	BINARY_modma_variable_init();
	CurveLength = 6;
	NN_AssignZero(RESULTX, CurveLength);
	BINARY_ECC_mod_add(a_Array, CurveLength, b_Array, CurveLength, P_Array, CurveLength, RESULTX, &x_len);
	result = memcmp(EXPECT_RESULTX, RESULTX, 4 * CurveLength);
	if(result)
	{
		printfS("BINARY ECC MODMA test fail \n");
		return;
	}
	else
	{
		printfS("BINARY ECC MODMA test pass \n");
	}

	printfS("---------BINARY ECC MODMA test end-------\n");
}
//end test MODMA

//end test ecc sram as normal sram
void BINARY_ECC_test_variable_init(void)
{
	P_Array     	 = g_data_buf;
	a_Array     	 = g_data_buf + WL;
	b_Array     	 = g_data_buf + 2 * WL;
	N_Array     	 = g_data_buf + 3 * WL;
	BaseX_Array 	 = g_data_buf + 4 * WL;
	BaseY_Array 	 = g_data_buf + 5 * WL;
	ECC_P0X 		 = g_data_buf + 6 * WL;
	ECC_P0Y 		 = g_data_buf + 7 * WL;
	ECC_P1X 		 = g_data_buf + 8 * WL;
	ECC_P1Y 		 = g_data_buf + 9 * WL;
	ECC_k 			 = g_data_buf + 10 * WL;
	EXPECT_RESULTX 	 = g_data_buf + 11 * WL;
	EXPECT_RESULTY 	 = g_data_buf + 12 * WL;
	RESULTX          = g_data_buf + 13 * WL;
	RESULTY          = g_data_buf + 14 * WL;
}
void binary_pki_test(void)
{
	enable_module(BIT_UAC | BIT_PKI);

	BINARY_ECC_test_variable_init();
	printfS("---------BINARY ECC test-------\n");
	BINARY_ECC_PM_test();
	BINARY_ECC_PD_test();
	BINARY_ECC_PA_test();
	BINARY_ECC_PS_test();
	BINARY_ECC_PJ_test();
	BINARY_ECC_MODME_test();
	BINARY_ECC_MODMUL_test();
	BINARY_ECC_MODSQR_test();
	BINARY_ECC_MODMA_test();

	printfS("---------BINARY ECC test end-------\n");
	
}

//test PM
void PM_variable_init(void)
{

	P_Array[0] = 0x9F3FF94F;
	P_Array[1] = 0x3AD0496B;
	P_Array[2] = 0x887653B5;
	P_Array[3] = 0x18C8DD29;
	P_Array[4] = 0xBF414E4D;

	a_Array[0] = 0x9F3FF94C;
	a_Array[1] = 0x3AD0496B;
	a_Array[2] = 0x887653B5;
	a_Array[3] = 0x18C8DD29;
	a_Array[4] = 0xBF414E4D;

	ECC_k[0] = 0x88F086D8;
	ECC_k[1] = 0xA858F812;
	ECC_k[2] = 0x4089054D;
	ECC_k[3] = 0x3E74A47C;
	ECC_k[4] = 0x7C2AD93A;

	ECC_P0X[0] = 0x901C3020;
	ECC_P0X[1] = 0x7FB87D12;
	ECC_P0X[2] = 0x23F71D58;
	ECC_P0X[3] = 0xD500F4BF;
	ECC_P0X[4] = 0x8E77A9CA;

	ECC_P0Y[0] = 0x9F4D5807;
	ECC_P0Y[1] = 0x62C343FE;
	ECC_P0Y[2] = 0xAD7F8C49;
	ECC_P0Y[3] = 0xEB9353EC;
	ECC_P0Y[4] = 0x5D92C9A4;

	EXPECT_RESULTX[0] = 0x604E3E53;
	EXPECT_RESULTX[1] = 0x94715695;
	EXPECT_RESULTX[2] = 0x96E4B1CC;
	EXPECT_RESULTX[3] = 0x8BFCE3FB;
	EXPECT_RESULTX[4] = 0xBDEF18C1;

	EXPECT_RESULTY[0] = 0x10676D1B;
	EXPECT_RESULTY[1] = 0xC57EA6A7;
	EXPECT_RESULTY[2] = 0x0F0EF963;
	EXPECT_RESULTY[3] = 0x8A4D71DD;
	EXPECT_RESULTY[4] = 0x2746DAB5;
}

void ECC_PM_test(void)
{
	UINT8  result;
	ECC_G_STR ecc_glb_str;
	printfS("---------ECC PM test-------\n");

	result = 0;

	PM_variable_init();
	CurveLength = 5;
	ECC_para_initial((ECC_G_STR *)(&ecc_glb_str), CurveLength, (UINT32 *)P_Array, (UINT32 *)a_Array, (UINT32 *)b_Array, (UINT32 *)N_Array, (UINT32 *)BaseX_Array, (UINT32 *)BaseY_Array);
	NN_AssignZero(RESULTX, 6);
	NN_AssignZero(RESULTY, 6);
	if(ECC_PM((ECC_G_STR *)(&ecc_glb_str), ECC_k, ECC_P0X, ECC_P0Y, RESULTX, RESULTY))
	{
		printfS("ECC PM test fail 1 \n");
		return;
	}
	result = memcmp(EXPECT_RESULTX, RESULTX, 4 * CurveLength);
	result |= memcmp(EXPECT_RESULTY, RESULTY, 4 * CurveLength);
	if(result)
	{
		printfS("ECC PM  test fail 2\n");
		return;
	}
	else
	{
		printfS("ECC PM  test pass \n");
	}

	printfS("---------ECC PM test end-------\n");
}
//end test PM

//test PD
void PD_variable_init(void)
{

	P_Array[0] = 0xffffffff;
	P_Array[1] = 0xffffffff;
	P_Array[2] = 0xffffffff;
	P_Array[3] = 0x00000000;
	P_Array[4] = 0x00000000;
	P_Array[5] = 0x00000000;
	P_Array[6] = 0x00000001;
	P_Array[7] = 0xffffffff;

	a_Array[0] = 0xfffffffc;
	a_Array[1] = 0xffffffff;
	a_Array[2] = 0xffffffff;
	a_Array[3] = 0x00000000;
	a_Array[4] = 0x00000000;
	a_Array[5] = 0x00000000;
	a_Array[6] = 0x00000001;
	a_Array[7] = 0xffffffff;

	ECC_P0X[0] = 0x219AADAA;
	ECC_P0X[1] = 0xF1F03503;
	ECC_P0X[2] = 0x28197506;
	ECC_P0X[3] = 0x300C3135;
	ECC_P0X[4] = 0x257CBFD1;
	ECC_P0X[5] = 0x07B1BE3C;
	ECC_P0X[6] = 0x0481FFF1;
	ECC_P0X[7] = 0xB4F47706;

	ECC_P0Y[0] = 0xFCA60D1F;
	ECC_P0Y[1] = 0xED7D9DA8;
	ECC_P0Y[2] = 0x73B61392;
	ECC_P0Y[3] = 0x0CFFED11;
	ECC_P0Y[4] = 0x2583B493;
	ECC_P0Y[5] = 0x902E2770;
	ECC_P0Y[6] = 0x06E93FF4;
	ECC_P0Y[7] = 0xABC596DC;

	EXPECT_RESULTX[0] = 0x42C03C2C;
	EXPECT_RESULTX[1] = 0x07CC2CC4;
	EXPECT_RESULTX[2] = 0x1761B301;
	EXPECT_RESULTX[3] = 0x48AD4CE0;
	EXPECT_RESULTX[4] = 0x9E7E4F11;
	EXPECT_RESULTX[5] = 0x314157DA;
	EXPECT_RESULTX[6] = 0xF7B41DC9;
	EXPECT_RESULTX[7] = 0x9E531B39;

	EXPECT_RESULTY[0] = 0x1D7C045B;
	EXPECT_RESULTY[1] = 0x326A5949;
	EXPECT_RESULTY[2] = 0xE02C9103;
	EXPECT_RESULTY[3] = 0xDC68A857;
	EXPECT_RESULTY[4] = 0x8E984C19;
	EXPECT_RESULTY[5] = 0x98AD9B6F;
	EXPECT_RESULTY[6] = 0x80C2F4BA;
	EXPECT_RESULTY[7] = 0xF4BD6F8F;

}
void ECC_PD_test(void)
{
	UINT8  result;
	ECC_G_STR ecc_glb_str;

	printfS("---------ECC PD test-------\n");

	result = 0;

	PD_variable_init();
	CurveLength = 8;
	ECC_para_initial((ECC_G_STR *)(&ecc_glb_str), CurveLength, (UINT32 *)P_Array, (UINT32 *)a_Array, (UINT32 *)b_Array, (UINT32 *)N_Array, (UINT32 *)BaseX_Array, (UINT32 *)BaseY_Array);
	NN_AssignZero(RESULTX, CurveLength);
	NN_AssignZero(RESULTY, CurveLength);
	if(ECC_PD((ECC_G_STR *)(&ecc_glb_str), ECC_P0X, ECC_P0Y, RESULTX, RESULTY))
	{
		printfS("ECC PD  test fail \n");
		return;
	}

	result = memcmp(EXPECT_RESULTX, RESULTX, 4 * CurveLength);
	result |= memcmp(EXPECT_RESULTY, RESULTY, 4 * CurveLength);
	if(result)
	{
		printfS("ECC PD  test fail \n");
		return;
	}
	else
	{
		printfS("ECC PD  test pass \n");
	}

	printfS("---------ECC PD test end-------\n");
}
//end test PD

//test PA
void PA_variable_init(void)
{

	P_Array[0] = 0xffffffff;
	P_Array[1] = 0xffffffff;
	P_Array[2] = 0xffffffff;
	P_Array[3] = 0xffffffff;
	P_Array[4] = 0xffffffff;
	P_Array[5] = 0xffffffff;
	P_Array[6] = 0xffffffff;
	P_Array[7] = 0xffffffff;
	P_Array[8] = 0xffffffff;
	P_Array[9] = 0xffffffff;
	P_Array[10] = 0xffffffff;
	P_Array[11] = 0xffffffff;
	P_Array[12] = 0xffffffff;
	P_Array[13] = 0xffffffff;
	P_Array[14] = 0xffffffff;
	P_Array[15] = 0xffffffff;
	P_Array[16] = 0x000001ff;

	a_Array[0] = 0xfffffffc;
	a_Array[1] = 0xffffffff;
	a_Array[2] = 0xffffffff;
	a_Array[3] = 0xffffffff;
	a_Array[4] = 0xffffffff;
	a_Array[5] = 0xffffffff;
	a_Array[6] = 0xffffffff;
	a_Array[7] = 0xffffffff;
	a_Array[8] = 0xffffffff;
	a_Array[9] = 0xffffffff;
	a_Array[10] = 0xffffffff;
	a_Array[11] = 0xffffffff;
	a_Array[12] = 0xffffffff;
	a_Array[13] = 0xffffffff;
	a_Array[14] = 0xffffffff;
	a_Array[15] = 0xffffffff;
	a_Array[16] = 0x000001ff;

	ECC_P0X[0] = 0xE77BAC04;
	ECC_P0X[1] = 0xD9EF2EC9;
	ECC_P0X[2] = 0x659D25A6;
	ECC_P0X[3] = 0x051B76DE;
	ECC_P0X[4] = 0xB8067853;
	ECC_P0X[5] = 0xFDC66DBD;
	ECC_P0X[6] = 0x92013D99;
	ECC_P0X[7] = 0x68133B2B;
	ECC_P0X[8] = 0x6E28372C;
	ECC_P0X[9] = 0x46B0B43A;
	ECC_P0X[10] = 0x99C01CDE;
	ECC_P0X[11] = 0x0A3CCE3B;
	ECC_P0X[12] = 0xE7E61DB4;
	ECC_P0X[13] = 0xB397D707;
	ECC_P0X[14] = 0x9DCC811E;
	ECC_P0X[15] = 0x8279FC83;
	ECC_P0X[16] = 0x0000001F;

	ECC_P0Y[0] = 0xDE84E2ED;
	ECC_P0Y[1] = 0xD89A552F;
	ECC_P0Y[2] = 0x825B7BAC;
	ECC_P0Y[3] = 0xE38136A2;
	ECC_P0Y[4] = 0xA048078F;
	ECC_P0Y[5] = 0x21D618B7;
	ECC_P0Y[6] = 0x4A42464C;
	ECC_P0Y[7] = 0xDDFA9028;
	ECC_P0Y[8] = 0xD739B318;
	ECC_P0Y[9] = 0x8A0E7B49;
	ECC_P0Y[10] = 0x875A70BD;
	ECC_P0Y[11] = 0x37D2D246;
	ECC_P0Y[12] = 0x1CB587BA;
	ECC_P0Y[13] = 0x2894F0C5;
	ECC_P0Y[14] = 0x0D44681D;
	ECC_P0Y[15] = 0x63A548E9;
	ECC_P0Y[16] = 0x000001F2;

	ECC_P1X[0] = 0x581A859F;
	ECC_P1X[1] = 0xE6F02B8D;
	ECC_P1X[2] = 0xA52406DD;
	ECC_P1X[3] = 0xA93A7C08;
	ECC_P1X[4] = 0xB09EEA96;
	ECC_P1X[5] = 0x2965C5BC;
	ECC_P1X[6] = 0xD8E378DD;
	ECC_P1X[7] = 0x5B0F0464;
	ECC_P1X[8] = 0x8F7D1318;
	ECC_P1X[9] = 0x0A9EB7A9;
	ECC_P1X[10] = 0x7179FEA6;
	ECC_P1X[11] = 0xDFC641A6;
	ECC_P1X[12] = 0x0CF09FE4;
	ECC_P1X[13] = 0xFDBB143E;
	ECC_P1X[14] = 0x3AC1444E;
	ECC_P1X[15] = 0x1461D5AC;
	ECC_P1X[16] = 0x0000001A;

	ECC_P1Y[0] = 0x2FF078B3;
	ECC_P1Y[1] = 0x9D1DF801;
	ECC_P1Y[2] = 0x546CD21A;
	ECC_P1Y[3] = 0xD1B5BDD7;
	ECC_P1Y[4] = 0xED443A3C;
	ECC_P1Y[5] = 0x2602A26D;
	ECC_P1Y[6] = 0xAE628508;
	ECC_P1Y[7] = 0xEA4A362D;
	ECC_P1Y[8] = 0xF6203F9E;
	ECC_P1Y[9] = 0xEC5D6EBB;
	ECC_P1Y[10] = 0xE649BDC6;
	ECC_P1Y[11] = 0x5F3A2AE1;
	ECC_P1Y[12] = 0x789B9459;
	ECC_P1Y[13] = 0x2C48EAC4;
	ECC_P1Y[14] = 0xD74B3583;
	ECC_P1Y[15] = 0x06C56878;
	ECC_P1Y[16] = 0x000000D4;

	EXPECT_RESULTX[0] = 0x341B7705;
	EXPECT_RESULTX[1] = 0xA2AE03AF;
	EXPECT_RESULTX[2] = 0x40C7F9C0;
	EXPECT_RESULTX[3] = 0xFFCC8D39;
	EXPECT_RESULTX[4] = 0x9F3F273E;
	EXPECT_RESULTX[5] = 0x1243BF33;
	EXPECT_RESULTX[6] = 0xFBB0517E;
	EXPECT_RESULTX[7] = 0xBCF56541;
	EXPECT_RESULTX[8] = 0xE3381F60;
	EXPECT_RESULTX[9] = 0x6A58B29C;
	EXPECT_RESULTX[10] = 0x32CEA618;
	EXPECT_RESULTX[11] = 0xC6880875;
	EXPECT_RESULTX[12] = 0xB0E01019;
	EXPECT_RESULTX[13] = 0xE58CA6C3;
	EXPECT_RESULTX[14] = 0x19511043;
	EXPECT_RESULTX[15] = 0xB2826A73;
	EXPECT_RESULTX[16] = 0x00000121;

	EXPECT_RESULTY[0] = 0xA99EF62A;
	EXPECT_RESULTY[1] = 0x57EBE35A;
	EXPECT_RESULTY[2] = 0xA01A4DA5;
	EXPECT_RESULTY[3] = 0x2F4F2C61;
	EXPECT_RESULTY[4] = 0xF9E2EC8E;
	EXPECT_RESULTY[5] = 0xA13686D4;
	EXPECT_RESULTY[6] = 0x16723E11;
	EXPECT_RESULTY[7] = 0xFB52F22E;
	EXPECT_RESULTY[8] = 0x0007824C;
	EXPECT_RESULTY[9] = 0x228BDC8E;
	EXPECT_RESULTY[10] = 0x64560836;
	EXPECT_RESULTY[11] = 0xC0C8C2D2;
	EXPECT_RESULTY[12] = 0xA4F631CB;
	EXPECT_RESULTY[13] = 0xFA487832;
	EXPECT_RESULTY[14] = 0xD61C9D54;
	EXPECT_RESULTY[15] = 0xC037DC2D;
	EXPECT_RESULTY[16] = 0x000000CE;
}
void ECC_PA_test(void)
{
	UINT8  result;
	ECC_G_STR ecc_glb_str;

	printfS("---------ECC PA test-------\n");

	result = 0;

	PA_variable_init();
	CurveLength = 17;
	ECC_para_initial((ECC_G_STR *)(&ecc_glb_str), CurveLength, (UINT32 *)P_Array, (UINT32 *)a_Array, (UINT32 *)b_Array, (UINT32 *)N_Array, (UINT32 *)BaseX_Array, (UINT32 *)BaseY_Array);
	NN_AssignZero(RESULTX, CurveLength);
	NN_AssignZero(RESULTY, CurveLength);
	if(ECC_PA_PS((ECC_G_STR *)(&ecc_glb_str), CNST_POINT_ADD, ECC_P0X, ECC_P0Y, ECC_P1X, ECC_P1Y, RESULTX, RESULTY))
	{
		printfS("ECC PA  test fail \n");
		return;
	}

	result = memcmp(EXPECT_RESULTX, RESULTX, 4 * CurveLength);
	result |= memcmp(EXPECT_RESULTY, RESULTY, 4 * CurveLength);
	if(result)
	{
		printfS("ECC PA  test fail \n");
		return;
	}
	else
	{
		printfS("ECC PA  test pass \n");
	}

	printfS("---------ECC PA test end-------\n");
}
//end test PA

//test PS
void PS_variable_init(void)
{

	P_Array[0] = 0xffffffff;
	P_Array[1] = 0xffffffff;
	P_Array[2] = 0xffffffff;
	P_Array[3] = 0x00000000;
	P_Array[4] = 0x00000000;
	P_Array[5] = 0x00000000;
	P_Array[6] = 0x00000001;
	P_Array[7] = 0xffffffff;

	a_Array[0] = 0xfffffffc;
	a_Array[1] = 0xffffffff;
	a_Array[2] = 0xffffffff;
	a_Array[3] = 0x00000000;
	a_Array[4] = 0x00000000;
	a_Array[5] = 0x00000000;
	a_Array[6] = 0x00000001;
	a_Array[7] = 0xffffffff;

	ECC_P1X[0] = 0x90371D2F;
	ECC_P1X[1] = 0x4F06106C;
	ECC_P1X[2] = 0x271AA8F8;
	ECC_P1X[3] = 0x7363965C;
	ECC_P1X[4] = 0xF269087E;
	ECC_P1X[5] = 0x3136E6D1;
	ECC_P1X[6] = 0xCE1A7804;
	ECC_P1X[7] = 0xBD7DA1F3;


	ECC_P1Y[0] = 0xA015B0C5;
	ECC_P1Y[1] = 0x44A3F25C;
	ECC_P1Y[2] = 0xFC246E55;
	ECC_P1Y[3] = 0x4D4E3CC2;
	ECC_P1Y[4] = 0x602DDBDF;
	ECC_P1Y[5] = 0x02C953A3;
	ECC_P1Y[6] = 0xC551B575;
	ECC_P1Y[7] = 0x861139FB;

	ECC_P0X[0] = 0x2B02D072;
	ECC_P0X[1] = 0xB560C72F;
	ECC_P0X[2] = 0x955A3CB4;
	ECC_P0X[3] = 0x2D1BA353;
	ECC_P0X[4] = 0x9526AFF4;
	ECC_P0X[5] = 0x03CCE000;
	ECC_P0X[6] = 0x0181876F;
	ECC_P0X[7] = 0x5560F040;

	ECC_P0Y[0] = 0xD45971A1;
	ECC_P0Y[1] = 0xCF54B570;
	ECC_P0Y[2] = 0xE1FCBEF3;
	ECC_P0Y[3] = 0xC3790205;
	ECC_P0Y[4] = 0x3B304673;
	ECC_P0Y[5] = 0x5AF74B35;
	ECC_P0Y[6] = 0x8D099312;
	ECC_P0Y[7] = 0x85EA703D;


	EXPECT_RESULTX[0] = 0xE93F0715;
	EXPECT_RESULTX[1] = 0x9E040DAC;
	EXPECT_RESULTX[2] = 0xFE3CC5C1;
	EXPECT_RESULTX[3] = 0x798F4137;
	EXPECT_RESULTX[4] = 0x991F3699;
	EXPECT_RESULTX[5] = 0x0A26F2D4;
	EXPECT_RESULTX[6] = 0x698543D2;
	EXPECT_RESULTX[7] = 0x1814E7F6;

	EXPECT_RESULTY[0] = 0x78919626;
	EXPECT_RESULTY[1] = 0x802ED215;
	EXPECT_RESULTY[2] = 0xAA9A0B60;
	EXPECT_RESULTY[3] = 0xEE09EDAF;
	EXPECT_RESULTY[4] = 0x9E0B477D;
	EXPECT_RESULTY[5] = 0x15A169CB;
	EXPECT_RESULTY[6] = 0x7DCECB3E;
	EXPECT_RESULTY[7] = 0xB6CDBF6D;
}

void ECC_PS_test(void)
{
	UINT8  result;
	ECC_G_STR ecc_glb_str;

	printfS("---------ECC PS test-------\n");

	result = 0;

	PS_variable_init();
	CurveLength = 8;
	ECC_para_initial((ECC_G_STR *)(&ecc_glb_str), CurveLength, (UINT32 *)P_Array, (UINT32 *)a_Array, (UINT32 *)b_Array, (UINT32 *)N_Array, (UINT32 *)BaseX_Array, (UINT32 *)BaseY_Array);
	NN_AssignZero(RESULTX, CurveLength);
	NN_AssignZero(RESULTY, CurveLength);
	if(ECC_PA_PS((ECC_G_STR *)(&ecc_glb_str), CNST_POINT_SUB, ECC_P0X, ECC_P0Y, ECC_P1X, ECC_P1Y, RESULTX, RESULTY))
	{
		printfS("ECC PS  test fail \n");
		return;
	}

	result = memcmp(EXPECT_RESULTX, RESULTX, 4 * CurveLength);
	result |= memcmp(EXPECT_RESULTY, RESULTY, 4 * CurveLength);
	if(result)
	{
		printfS("ECC PS  test fail \n");
		return;
	}
	else
	{
		printfS("ECC PS  test pass \n");
	}

	printfS("---------ECC PS test end-------\n");
}
//end test PS
//test PJ
void PJ_variable_init(void)
{
	P_Array[0] = 0x9F3FF94F;
	P_Array[1] = 0x3AD0496B;
	P_Array[2] = 0x887653B5;
	P_Array[3] = 0x18C8DD29;
	P_Array[4] = 0xBF414E4D;

	a_Array[0] = 0x9F3FF94C;
	a_Array[1] = 0x3AD0496B;
	a_Array[2] = 0x887653B5;
	a_Array[3] = 0x18C8DD29;
	a_Array[4] = 0xBF414E4D;

	b_Array[0] = 0x95872863;
	b_Array[1] = 0xC73EDED1;
	b_Array[2] = 0x1020EFC3;
	b_Array[3] = 0x3C0584E5;
	b_Array[4] = 0x6C245E1F;


	ECC_P0X[0] = 0x901C3020;
	ECC_P0X[1] = 0x7FB87D12;
	ECC_P0X[2] = 0x23F71D58;
	ECC_P0X[3] = 0xD500F4BF;
	ECC_P0X[4] = 0x8E77A9CA;

	ECC_P0Y[0] = 0x9F4D5807;
	ECC_P0Y[1] = 0x62C343FE;
	ECC_P0Y[2] = 0xAD7F8C49;
	ECC_P0Y[3] = 0xEB9353EC;
	ECC_P0Y[4] = 0x5D92C9A4;
}

void ECC_PJ_test(void)
{
//	UINT8  result;
	ECC_G_STR ecc_glb_str;

	printfS("---------ECC PJ test-------\n");

//	result = 0;

	PJ_variable_init();
	CurveLength = 5;
	ECC_para_initial((ECC_G_STR *)(&ecc_glb_str), CurveLength, (UINT32 *)P_Array, (UINT32 *)a_Array, (UINT32 *)b_Array, (UINT32 *)N_Array, (UINT32 *)BaseX_Array, (UINT32 *)BaseY_Array);
	if(!ECC_PJ((ECC_G_STR *)(&ecc_glb_str), ECC_P0X, ECC_P0Y))
	{
		printfS("ECC PJ  test pass \n");
	}
	else
	{
		printfS("ECC PJ  test fail \n");
		return;
	}
	printfS("---------ECC PJ test end-------\n");
}
//end test PJ
//test MODME
void modme_variable_init(void)
{
	P_Array[0] = 0x497f130d;
	P_Array[1] = 0xaa12983b;
	P_Array[2] = 0x095ca770;
	P_Array[3] = 0x0a411316;
	P_Array[4] = 0x2f1e15a9;
	P_Array[5] = 0x3c33a384;
	P_Array[6] = 0xb33bde67;
	P_Array[7] = 0xcc90401f;

	a_Array[0] = 0x2c9d0f76;
	a_Array[1] = 0xdd0c13ed;
	a_Array[2] = 0x46ce37e6;
	a_Array[3] = 0x62a9e454;
	a_Array[4] = 0x1b59f701;
	a_Array[5] = 0x5659aed1;
	a_Array[6] = 0x60fafbd1;
	a_Array[7] = 0xba7d0648;

	b_Array[0] = 0xfff35aa8;
	b_Array[1] = 0xa627f4a7;
	b_Array[2] = 0xc4365b53;
	b_Array[3] = 0xfe0f18ac;
	b_Array[4] = 0x80cd312b;
	b_Array[5] = 0x5496c43d;
	b_Array[6] = 0xb59c514c;
	b_Array[7] = 0x0bfc0cd6;

	EXPECT_RESULTX[0] = 0x89100a43;
	EXPECT_RESULTX[1] = 0xd0b83305;
	EXPECT_RESULTX[2] = 0xc34fe2c3;
	EXPECT_RESULTX[3] = 0x649f3c9a;
	EXPECT_RESULTX[4] = 0xd7222727;
	EXPECT_RESULTX[5] = 0xca3cc051;
	EXPECT_RESULTX[6] = 0x2f0ab686;
	EXPECT_RESULTX[7] = 0x35996d82;

}
void ECC_MODME_test(void)
{
	UINT8  result;
	UINT8  x_len;

	printfS("---------ECC MODME test-------\n");
	result = 0;
	modme_variable_init();
	CurveLength = 8;
	NN_AssignZero(RESULTX, CurveLength);
	ECC_mul_me(a_Array, CurveLength, b_Array, CurveLength, P_Array, CurveLength, RESULTX, &x_len, CNST_RSA_EXP);
	result = memcmp(EXPECT_RESULTX, RESULTX, 4 * CurveLength);
	if(result)
	{
		printfS("ECC MODME  test fail \n");
		return;
	}
	else
	{
		printfS("ECC MODME  test pass \n");
	}

	printfS("---------ECC MODME test end-------\n");
}
//end test MODME
//test MODMUL
void modmul_variable_init(void)
{
	P_Array[0] = 0xA48269A5;
	P_Array[1] = 0x3BF28F77;
	P_Array[2] = 0x6945F576;
	P_Array[3] = 0x00000000;
	P_Array[4] = 0x00000000;


	a_Array[0] = 0xAC0D58C8;
	a_Array[1] = 0x750927B3;
	a_Array[2] = 0x331C06E7;
	a_Array[3] = 0x00000000;
	a_Array[4] = 0x00000000;

	b_Array[0] = 0x85A99042;
	b_Array[1] = 0xBAD4D514;
	b_Array[2] = 0x21AABA18;
	b_Array[3] = 0x00000000;
	b_Array[4] = 0x00000000;

	EXPECT_RESULTX[0] = 0x02B47BFD;
	EXPECT_RESULTX[1] = 0x292564EA;
	EXPECT_RESULTX[2] = 0x4E3ADF58;
	EXPECT_RESULTX[3] = 0x00000000;
	EXPECT_RESULTX[4] = 0x00000000;
}
void ECC_MODMUL_test(void)
{
	UINT8  result;
	UINT8  x_len;

	printfS("---------ECC MODMUL test-------\n");
	result = 0;
	modmul_variable_init();
	CurveLength = 5;
	NN_AssignZero(RESULTX, CurveLength);
	ECC_mul_me(a_Array, CurveLength, b_Array, CurveLength, P_Array, CurveLength, RESULTX, &x_len, CNST_RSA_MUL);
	result = memcmp(EXPECT_RESULTX, RESULTX, 4 * CurveLength);
	if(result)
	{
		printfS("ECC MODMUL  test fail \n");
		return;
	}
	else
	{
		printfS("ECC MODMUL  test pass \n");
	}

	printfS("---------ECC MODMUL test end-------\n");
}
//end test MODMUL
//test MODSQR
void modsqr_variable_init(void)
{
	P_Array[0] = 0x69925f03;
	P_Array[1] = 0xde42dbf0;

	a_Array[0] = 0x69925f03;
	a_Array[1] = 0xde42dbf0;

	EXPECT_RESULTX[0] = 0x0;
	EXPECT_RESULTX[1] = 0x0;
}
void ECC_MODSQR_test(void)
{
	UINT8  result;
	UINT8  x_len;

	printfS("---------ECC MODSQR test-------\n");
	result = 0;
	modsqr_variable_init();
	CurveLength = 2;
	NN_AssignZero(RESULTX, CurveLength);
	ECC_mod_sqr(a_Array, CurveLength, P_Array, CurveLength, RESULTX, &x_len);
	result = memcmp(EXPECT_RESULTX, RESULTX, 4 * CurveLength);
	if(result)
	{
		printfS("ECC MODSQR  test fail \n");
		return;
	}
	else
	{
		printfS("ECC MODSQR  test pass \n");
	}

	printfS("---------ECC MODSQR test end-------\n");
}
//end test MODSQR

//test MODINV
void modinv_variable_init(void)
{
	P_Array[0] = 0x9f3ff94f;
	P_Array[1] = 0x3ad0496b;
	P_Array[2] = 0x887653b5;
	P_Array[3] = 0x18c8dd29;
	P_Array[4] = 0xbf414e4d;

	a_Array[0] = 0x388035b8;
	a_Array[1] = 0xf8dd6752;
	a_Array[2] = 0x8f7fedad;
	a_Array[3] = 0xb1b6e23b;
	a_Array[4] = 0x023d6f2c;

	EXPECT_RESULTX[0] = 0xed5dacb6;
	EXPECT_RESULTX[1] = 0x2d1b71de;
	EXPECT_RESULTX[2] = 0xac5f2c4c;
	EXPECT_RESULTX[3] = 0xf3d4cadb;
	EXPECT_RESULTX[4] = 0x3e21e960;

}
void ECC_MODINV_test(void)
{
	UINT8  result;
	UINT8  x_len;

	printfS("---------ECC MODINV test-------\n");
	result = 0;
	modinv_variable_init();
	CurveLength = 5;
	NN_AssignZero(RESULTX, CurveLength);
	ECC_mod_inv(a_Array, CurveLength, P_Array, CurveLength, RESULTX, &x_len);
	result = memcmp(EXPECT_RESULTX, RESULTX, 4 * CurveLength);
	if(result)
	{
		printfS("ECC MODINV  test fail \n");
		return;
	}
	else
	{
		printfS("ECC MODINV  test pass \n");
	}

	printfS("---------ECC MODINV test end-------\n");
}
//end test MODINV
//test MODMA
void modma_variable_init(void)
{
	P_Array[0] = 0x00000001;
	P_Array[1] = 0xffffffff;
	P_Array[2] = 0xffffffff;
	P_Array[3] = 0xffffffff;
	P_Array[4] = 0xffffffff;
	P_Array[5] = 0xffffffff;
	P_Array[6] = 0xffffffff;
	P_Array[7] = 0xffffffff;

	a_Array[0] = 0x5d6148b4;
	a_Array[1] = 0xde044d89;
	a_Array[2] = 0xa4659e2e;
	a_Array[3] = 0xcb7506e5;
	a_Array[4] = 0xf419e955;
	a_Array[5] = 0x5556a811;
	a_Array[6] = 0xd329e28c;
	a_Array[7] = 0xfbb51281;

	b_Array[0] = 0x9d49623a;
	b_Array[1] = 0xc23ffb61;
	b_Array[2] = 0x49e4f17b;
	b_Array[3] = 0x987c8f4e;
	b_Array[4] = 0x19fcaef6;
	b_Array[5] = 0x5be55176;
	b_Array[6] = 0x1e168365;
	b_Array[7] = 0x216e6bcc;

	EXPECT_RESULTX[0] = 0xfaaaaaed;
	EXPECT_RESULTX[1] = 0xa04448eb;
	EXPECT_RESULTX[2] = 0xee4a8faa;
	EXPECT_RESULTX[3] = 0x63f19633;
	EXPECT_RESULTX[4] = 0x0e16984c;
	EXPECT_RESULTX[5] = 0xb13bf988;
	EXPECT_RESULTX[6] = 0xf14065f1;
	EXPECT_RESULTX[7] = 0x1d237e4d;

}
void ECC_MODMA_test(void)
{
	UINT8  result;
	UINT8  x_len;

	printfS("---------ECC MODMA test-------\n");
	result = 0;
	modma_variable_init();
	CurveLength = 8;
	NN_AssignZero(RESULTX, CurveLength);
	ECC_mod_add_sub(a_Array, CurveLength, b_Array, CurveLength, P_Array, CurveLength, RESULTX, &x_len, CNST_MOD_ADD);
	result = memcmp(EXPECT_RESULTX, RESULTX, 4 * CurveLength);
	if(result)
	{
		printfS("ECC MODMA  test fail \n");
		return;
	}
	else
	{
		printfS("ECC MODMA  test pass \n");
	}

	printfS("---------ECC MODMA test end-------\n");
}
//end test MODMA
//test MODMS
void modms_variable_init(void)
{
	P_Array[0] = 0x00000001;
	P_Array[1] = 0xffffffff;
	P_Array[2] = 0xffffffff;
	P_Array[3] = 0xffffffff;
	P_Array[4] = 0xffffffff;
	P_Array[5] = 0xffffffff;
	P_Array[6] = 0xffffffff;
	P_Array[7] = 0xffffffff;

	a_Array[0] = 0x5d6148b4;
	a_Array[1] = 0xde044d89;
	a_Array[2] = 0xa4659e2e;
	a_Array[3] = 0xcb7506e5;
	a_Array[4] = 0xf419e955;
	a_Array[5] = 0x5556a811;
	a_Array[6] = 0xd329e28c;
	a_Array[7] = 0xfbb51281;

	b_Array[0] = 0x9d49623a;
	b_Array[1] = 0xc23ffb61;
	b_Array[2] = 0x49e4f17b;
	b_Array[3] = 0x987c8f4e;
	b_Array[4] = 0x19fcaef6;
	b_Array[5] = 0x5be55176;
	b_Array[6] = 0x1e168365;
	b_Array[7] = 0x216e6bcc;

	EXPECT_RESULTX[0] = 0xc017e67a;
	EXPECT_RESULTX[1] = 0x1bc45227;
	EXPECT_RESULTX[2] = 0x5a80acb3;
	EXPECT_RESULTX[3] = 0x32f87797;
	EXPECT_RESULTX[4] = 0xda1d3a5f;
	EXPECT_RESULTX[5] = 0xf971569b;
	EXPECT_RESULTX[6] = 0xb5135f26;
	EXPECT_RESULTX[7] = 0xda46a6b5;
}
void ECC_MODMS_test(void)
{
	UINT8  result;
	UINT8  x_len;

	printfS("---------ECC MODMS test-------\n");
	result = 0;
	modms_variable_init();
	CurveLength = 8;
	NN_AssignZero(RESULTX, CurveLength);
	ECC_mod_add_sub(a_Array, CurveLength, b_Array, CurveLength, P_Array, CurveLength, RESULTX, &x_len, CNST_MOD_SUB);
	result = memcmp(EXPECT_RESULTX, RESULTX, 4 * CurveLength);
	if(result)
	{
		printfS("ECC MODMS  test fail \n");
		return;
	}
	else
	{
		printfS("ECC MODMS  test pass \n");
	}

	printfS("---------ECC MODMS test end-------\n");
}
//end test MODMS

//test ECC DVM
void dvm_variable_init_6432(void)
{
	a_Array[0] = 0xCC789A5E;
	a_Array[1] = 0x9E4D53BB;

	P_Array[0] = 0xD2AE1829;
	//r
	EXPECT_RESULTY[0] = 0x21F9BDCB;
	EXPECT_RESULTY[1] = 0x0;
	//q
	EXPECT_RESULTX[0] = 0xc05ad65b;
	EXPECT_RESULTX[1] = 0x0;
}
void ECC_DVM_test(void)
{
	UINT8  result;
	UINT8  x_len;
	UINT8  y_len;

	printfS("---------ECC DVM test-------\n");
	result = 0;
	dvm_variable_init_6432();
	CurveLength = 2;
	NN_AssignZero(RESULTX, CurveLength);
	NN_AssignZero(RESULTY, CurveLength);
	ECC_red_dvm(a_Array, CurveLength, P_Array, (CurveLength - 1), RESULTY, &y_len, RESULTX, &x_len);
	result = memcmp(EXPECT_RESULTX, RESULTX, 4 * x_len);
	result |= memcmp(EXPECT_RESULTY, RESULTY, 4 * y_len);
	if(result)
	{
		printfS("ECC DVM  test fail \n");
		return;
	}
	else
	{
		printfS("ECC DVM  test pass \n");
	}

	printfS("---------ECC DVM test end-------\n");
}
//end test ECC DVM
//test ecc sram as normal sram
//fuction: memory March C+ test
//length ：数据长度，单位为WORD
UINT32 mem_test_march_C2(UINT32 startaddr, UINT32 length, UINT32 value_0, UINT32 value_1)
{
	UINT32 i;
	UINT32 *ramaddr;

	ramaddr = (UINT32 *)startaddr;

	// step 0: W0
	for(i = 0; i < length; i++)
	{
		ramaddr[i] = value_0;
	}


	// step 1: R0->W1->R1  ,address ++
	for(i = 0; i < length; i++)
	{
		if(ramaddr[i] != value_0)
		{
			return 1;
		}
		ramaddr[i] = value_1;
		if(ramaddr[i] != value_1)
		{
			return 2;
		}
	}

	// step 2: R1->W0->R0  ,address ++
	for(i = 0; i < length; i++)
	{

		if(ramaddr[i] != value_1)
		{
			return 3;
		}
		ramaddr[i] = value_0;
		if(ramaddr[i] != value_0)
		{
			return 4;
		}
	}

	// step 3: R0->W1->R1  ,address --
	for(i = length; i > 0; i--)
	{
		i = i - 1;
		if(ramaddr[i] != value_0)
		{
			return 5;
		}
		ramaddr[i] = value_1;
		if(ramaddr[i] != value_1)
		{
			return 6;
		}
	}

	// step 4: R1->W0->R0  ,address --
	for(i = length; i > 0; i--)
	{
		i = i - 1;
		if(ramaddr[i] != value_1)
		{
			return 7;
		}
		ramaddr[i] = value_0;
		if(ramaddr[i] != value_0)
		{
			return 8;
		}
	}

	// step 5: R0
	for(i = 0; i < length; i++)
	{
		if(ramaddr[i] != value_0)
		{
			return 9;
		}
	}

	return 0;
}

//length ：数据长度，单位为WORD
UINT32 sram_test(UINT32 startaddr, UINT32 length)
{
	UINT32 result = 0;

	result = mem_test_march_C2(startaddr, length, 0x00000000, 0xffffffff);
	if(result)
	{
		result  = result + (0x0a << 8);
		return result;
	}

	result = mem_test_march_C2(startaddr, length, 0x0000ffff, 0xffff0000);
	if(result)
	{
		result  = result + (0x0b << 8);
		return result;
	}

	result = mem_test_march_C2(startaddr, length, 0x55555555, 0xaaaaaaaa);
	if(result)
	{
		result  = result + (0x0c << 8);
		return result;
	}

	result = mem_test_march_C2(startaddr, length, 0x33333333, 0xcccccccc);
	if(result)
	{
		result  = result + (0x0d << 8);
		return result;
	}

	return 0;
}
#define SRAM_BUF  (volatile unsigned long *)0x40011000
void ECC_sram_test(void)
{
	int i;
	UINT32 temp;
	UINT8  result = 0;

	printfS("---------ECC SRAM AS NORMAL SRAM test-------\n");
	*SRAM_BUF = 0x12345678;
	temp = *SRAM_BUF;
	printfS("temp is %x\n", temp);
	if(temp == 0x12345678)
	{
		printfS("ECC SRAM AS NORMAL SRAM test fail 1 \n");
		return;
	}
	alg2nor_sram();

	for(i = 0; i < 1; i++)
	{
		result = sram_test(0x40011000, (4096) / 4);
		if(result)
		{
			break;
		}
	}
	if(result)
	{
		printfS("ECC SRAM AS NORMAL SRAM  test fail 2 \n");
		return;
	}
	else
	{
		printfS("ECC SRAM AS NORMAL SRAM  test pass \n");
	}

	printfS("---------ECC SRAM AS NORMAL SRAM test end-------\n");

}
//end test ecc sram as normal sram
void ECC_test_variable_init(void)
{
	P_Array     	 = g_data_buf;
	a_Array     	 = g_data_buf + WL;
	b_Array     	 = g_data_buf + 2 * WL;
	N_Array     	 = g_data_buf + 3 * WL;
	BaseX_Array 	 = g_data_buf + 4 * WL;
	BaseY_Array 	 = g_data_buf + 5 * WL;
	ECC_P0X 		 = g_data_buf + 6 * WL;
	ECC_P0Y 		 = g_data_buf + 7 * WL;
	ECC_P1X 		 = g_data_buf + 8 * WL;
	ECC_P1Y 		 = g_data_buf + 9 * WL;
	ECC_k 			 = g_data_buf + 10 * WL;
	EXPECT_RESULTX 	 = g_data_buf + 11 * WL;
	EXPECT_RESULTY 	 = g_data_buf + 12 * WL;
	RESULTX          = g_data_buf + 13 * WL;
	RESULTY          = g_data_buf + 14 * WL;
}
void pki_test(void)
{
	enable_module(BIT_UAC | BIT_PKI);

	ECC_test_variable_init();
	printfS("---------ECC test-------\n");
	ECC_PM_test();
	ECC_PD_test();
	ECC_PA_test();
	ECC_PS_test();
	ECC_PJ_test();
	ECC_MODME_test();
	ECC_MODMUL_test();
	ECC_MODSQR_test();
	ECC_MODINV_test();
	ECC_MODMA_test();
	ECC_MODMS_test();
	ECC_DVM_test();
	ECC_sram_test();
	printfS("---------ECC test end-------\n");
	
}
//end for ecc test
