/*
  ******************************************************************************
  * @file    main.c
  * @brief   main source File.
  ******************************************************************************
*/
#include "app.h"    
#include "BurnSD.h"
#include "aes_app.h"
#include "bsp_sdcard.h"

#define UART_BAUD_RATE  921600

UART_HandleTypeDef UART1_Handle;

/************************************************************************
 * function   : Uart_Init
 * Description: Uart Initiation. 
 ************************************************************************/ 
void Uart_Init(void) 
{
    UART1_Handle.Instance        = UART1;    
    UART1_Handle.Init.BaudRate   = UART_BAUD_RATE; 
    UART1_Handle.Init.WordLength = UART_WORDLENGTH_8B;
    UART1_Handle.Init.StopBits   = UART_STOPBITS_1;
    UART1_Handle.Init.Parity     = UART_PARITY_NONE;
    UART1_Handle.Init.Mode       = UART_MODE_TX_RX_DEBUG;
    UART1_Handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    
    HAL_UART_Init(&UART1_Handle);  
    
    /* UART_DEBUG_ENABLE control printfS */     
    printfS("MCU is running, HCLK=%dHz, PCLK=%dHz\n", System_Get_SystemClock(), System_Get_APBClock());       
}





static void VerifySdDecrypt(void)
{
	uint8_t SD_Status;
    static uint8_t rdBuf[1024];
    static uint8_t decBuf[1024];
    static uint8_t sec0[512];
    static uint8_t sec1[512];
    uint8_t iv[16];
    uint32_t i;
  

//		SD_Initialize();
    // 分别读2个扇区（避免多块读取问题）
    SD_ReadDisk(sec0, 0, 1);
    SD_ReadDisk(sec1, 1, 1);

    // 合并到连续缓冲区
    for (i = 0; i < 512; i++) rdBuf[i] = sec0[i];
    for (i = 0; i < 512; i++) rdBuf[512 + i] = sec1[i];

    // 第1段: 512字节
    for (i = 0; i < 16; i++) iv[i] = AES_Iv[i];
    AES128_CBC_Decrypt(decBuf, rdBuf, 512, AES_Key, iv);

    // 第2段: 512字节, IV = 第1段密文末尾16字节
    for (i = 0; i < 16; i++) iv[i] = rdBuf[496 + i];
    AES128_CBC_Decrypt(&decBuf[512], &rdBuf[512], 512, AES_Key, iv);
    __NOP();

}

/*********************************************************************************
* Function    : main
* Description : 
* Input       : 
* Outpu       : 
* Author      : cindy                      Date:2021 
**********************************************************************************/
int main(void)
{
    System_Init();
    
    Uart_Init();
	  SD_Initialize();

  	VerifySdDecrypt();
	
    printfS("SD Card write start\n");
    Write_SDCard();
    printfS("SD Card write over\n");
	
    while(1)
    {

    }
}

