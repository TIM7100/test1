#include "bsp_spi.h"

static SPI_HandleTypeDef  hspi;

static void SPI_GpioConfig(SPI_TypeDef* SPIx)
{
    GPIO_InitTypeDef   SPI_GpioInit;

    /* SPI2 CS   PortB Pin12 */
    /* SPI2 CLK  PortB Pin13 */
    /* SPI2 MOSI PortB Pin15 */
    /* SPI2 MISO PortB Pin14 */
    if (SPIx == SPI2)
    {
        SPI_GpioInit.Pin            = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
        SPI_GpioInit.Mode           = GPIO_MODE_AF_PP;
        SPI_GpioInit.Pull           = GPIO_PULLUP;
        SPI_GpioInit.Alternate      = GPIO_FUNCTION_4;
        HAL_GPIO_Init(GPIOB, &SPI_GpioInit);

        SPI_GpioInit.Pin            = GPIO_PIN_12;
        SPI_GpioInit.Mode           = GPIO_MODE_OUTPUT_PP;
        SPI_GpioInit.Pull           = GPIO_PULLUP;
        SPI_GpioInit.Alternate      = GPIO_FUNCTION_0;
        HAL_GPIO_Init(GPIOB, &SPI_GpioInit);
    }
}

void SPI_Configuration(SPI_TypeDef* SPIx)
{
    SPI_GpioConfig(SPIx);

    if (SPIx == SPI1) System_Module_Enable(EN_SPI1);
    else if (SPIx == SPI2) System_Module_Enable(EN_SPI2);
    else if (SPIx == SPI3) System_Module_Enable(EN_SPI3);

    hspi.Instance = SPIx;
    hspi.Init.SPI_Mode              = SPI_MODE_MASTER;
    hspi.Init.SPI_Work_Mode         = SPI_WORK_MODE_3;
    hspi.Init.X_Mode                = SPI_1X_MODE;
    hspi.Init.First_Bit             = SPI_FIRSTBIT_MSB;
    hspi.Init.BaudRate_Prescaler    = SPI_BAUDRATE_PRESCALER_64;

    HAL_SPI_Init(&hspi);
}

void SPIx_SetSpeed(SPI_TypeDef* SPIx, uint8_t speed)
{
    if (SPIx == SPI1) System_Module_Enable(EN_SPI1);
    else if (SPIx == SPI2) System_Module_Enable(EN_SPI2);
    else if (SPIx == SPI3) System_Module_Enable(EN_SPI3);

    hspi.Instance = SPIx;
    hspi.Init.BaudRate_Prescaler = speed;
    HAL_SPI_Init(&hspi);
}

SPI_StatusTypeDef SPI_Transmit(SPI_TypeDef* SPIx, uint8_t *pData, uint32_t Size, uint32_t Timeout)
{
    if (HAL_SPI_Transmit(&hspi, pData, Size, Timeout) == HAL_OK)
        return SPI_OK;
    return SPI_ERROR;
}

SPI_StatusTypeDef SPI_Receive(SPI_TypeDef* SPIx, uint8_t *pData, uint32_t Size, uint32_t Timeout)
{
    if (HAL_SPI_Receive(&hspi, pData, Size, Timeout) == HAL_OK)
        return SPI_OK;
    return SPI_ERROR;
}

SPI_StatusTypeDef SPI_TransmitReceive(SPI_TypeDef* SPIx, uint8_t *pTxData, uint8_t *pRxData, uint32_t Size, uint32_t Timeout)
{
    if (HAL_SPI_TransmitReceive(&hspi, pTxData, pRxData, Size, Timeout) == HAL_OK)
        return SPI_OK;
    return SPI_ERROR;
}

uint8_t SPIx_WriteReadByte(SPI_TypeDef* SPIx, uint8_t TxData)
{
    uint8_t RxData;
    SPI_TransmitReceive(SPIx, &TxData, &RxData, 1, 0);
    return RxData;
}