#include  "iic.h"
#include  "delay.h"
#include  "fxx_gpio.h"

#define IICDebug


#ifdef  IICDebug
void IIC_Data_Printf(uint8_t DevAddress, const uint8_t *pData, uint16_t Size, uint8_t ACK);
#define IIC_Debug_printf(DevAddress, pData, Size, ACK)  IIC_Data_Printf(DevAddress, pData, Size, ACK)
#else
#define IIC_Debug_printf(DevAddress, pData, Size, ACK)  ((void)0)
#endif



/*IIC 引脚初始化*/
void IIC_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_Handle;

    /* Enable Clock */
    System_Module_Enable(EN_GPIOAB);

    /* I2C1 SCL ->PB8 I2C1 SDA ->PB9 */
    GPIO_Handle.Pin            = GPIO_PIN_8 ;
    GPIO_Handle.Mode           = GPIO_MODE_OUTPUT_PP;
    GPIO_Handle.Pull           = GPIO_NOPULL;
    GPIO_Handle.Alternate      = GPIO_FUNCTION_0;
    GPIO_Init(GPIOB, &GPIO_Handle);

    GPIO_Handle.Pin            = GPIO_PIN_9;
    GPIO_Handle.Mode           = GPIO_MODE_OUTPUT_PP;
    GPIO_Handle.Pull           = GPIO_NOPULL;
    GPIO_Handle.Alternate      = GPIO_FUNCTION_0;
    GPIO_Init(GPIOB, &GPIO_Handle);
}



void User_IIC_Init(void)
{
    IIC_GPIO_Init();

    SDA_H();    //SDA置高
    SCL_H();    //SCL置高
}



void IIC_SDA_OUTPUT(void)
{
    GPIO_InitTypeDef GPIO_Handle;

    /* Enable Clock */
    System_Module_Enable(EN_GPIOAB);

    /* I2C1 SCL ->PB8 I2C1 SDA ->PB9 */
    GPIO_Handle.Pin            = GPIO_PIN_9;
    GPIO_Handle.Mode           = GPIO_MODE_OUTPUT_PP;
    GPIO_Handle.Pull           = GPIO_NOPULL;
    GPIO_Handle.Alternate      = GPIO_FUNCTION_0;
    GPIO_Init(GPIOB, &GPIO_Handle);
}



void IIC_SDA_INPUT(void)
{
    GPIO_InitTypeDef GPIO_Handle;

    /* Enable Clock */
    System_Module_Enable(EN_GPIOAB);

    /* I2C1 SCL ->PB8 I2C1 SDA ->PB9 */
    GPIO_Handle.Pin            = GPIO_PIN_9;
    GPIO_Handle.Mode           = GPIO_MODE_INPUT;
    GPIO_Handle.Pull           = GPIO_PULLUP;
    GPIO_Handle.Alternate      = GPIO_FUNCTION_0;
    GPIO_Init(GPIOB, &GPIO_Handle);
}



void IIC_Start(void)
{
    IIC_SDA_OUTPUT();       //SDA输出

    SDA_H();            //SDA=1,准备数据
    Delay_us(5);
    SCL_H();            //SCL=1,拉高时钟线通知读取
    Delay_us(5);
    SDA_L();            //SDA=0，SCL高电平期间，拉低SDA，产生起始信号
    Delay_us(3);
    SCL_L();
}



void IIC_Stop(void)
{
    IIC_SDA_OUTPUT();   //SDA输出

    SDA_L();    //SDA=0
    Delay_us(3);
    SCL_H();    //SCL=1
    Delay_us(5);
    SDA_H();    //SDA=1
}



void IIC_Send_Ack(void)
{
    SDA_OUTPUT();   //SDA输出
    SCL_L();
    SDA_L();
    Delay_us(2);

    SCL_H();
    Delay_us(2);

    SCL_L();
    //Delay_us(8);
}



void IIC_Send_NAck(void)
{
    SDA_OUTPUT();
    SCL_L();

    Delay_us(2);
    SDA_H();    //SDA=1
    SCL_H();    //SCL=1
    Delay_us(2);

    SCL_L();   //SCL=0
    //Delay_us(8);
}



uint8_t IIC_Read_Byte(uint8_t uAck)
{
    uint8_t i = 8;
    uint8_t Rx_Byte = 0x00;  //接收字节的变量

		SDA_INPUT();



    while (i--)
    {
        SCL_L();
        Delay_us(2);
        SCL_H();
        Rx_Byte = Rx_Byte << 1;//将接收的位往高位移
        if (SDA_READ() == 1)
        {
            Rx_Byte |= 0x01;   //将末位置1
        }
        else
        {
            Rx_Byte &= 0xFE;   //将末位置0
        }
				Delay_us(2);
        SCL_L();
				
    }
    if (uAck == IIC_ACK) //ack=1时，发送ack，ack=0时，发送nack
    {
        IIC_Send_Ack();
    }
    else
    {
        IIC_Send_NAck();
    }
    return Rx_Byte;//返回接收的一个字节
}



/************************************************************************
 * function   : IIC_Read_Bytes
 * Description: 主机连续读取多个字节
 * input :
 *         uint8_t DevAddress       设备地址（输入时读写位应为0）
 *         uint8_t* Rx_Buff         存放接收到的字节的数组名
 *         uint16_t NumByteToRead   需要读取的个数
 * return: none
 ************************************************************************/
void IIC_Read_Bytes(uint8_t DevAddress, uint8_t* Rx_Buff, uint16_t Size)
{
    uint16_t i;
    uint8_t Ack;

    if (IIC_Write_Byte(DevAddress | 0x01) == 0)         //发送设备地址
    {
        Delay_ms(1);
        for (i = 0; i < Size; i++)//主机接收数据后判断发送应答or非应答
        {
            if (i < (Size - 1))
            {
                Ack = IIC_ACK;
            }
            else
            {
                Ack = IIC_NACK;

            }
            Rx_Buff[i] = IIC_Read_Byte(Ack);        //将接收的数据存放在数组中
        }
    }
}



/************************************************************************
 * function   : IIC_Ack_Check
 * Description: 主机检查接收从机发送回来的响应
 * input :
 *         void
 * return: 0或1 1表示从机接收失败  0表示从机接收成功
 ************************************************************************/
uint8_t IIC_Ack_Check(void)
{
    uint8_t ACK;
		
    SDA_INPUT();//SDA设置为输入

		Delay_us(2);
    /* 产生一个电平,读取应答位 */
    SCL_H();//SCL=1
		Delay_us(1);
    ACK = SDA_READ();
    SCL_L();        //应答位读取结束

    return ACK;
}



/************************************************************************
 * function   : IIC_Write_Byte
 * Description: 主机写入一个字节
 * input :
 *         uint8_t Tx_Byte:需要写入的字节
 * return: none
 ************************************************************************/
uint8_t IIC_Write_Byte(uint8_t Tx_Byte)
{
    uint8_t i;


    SDA_OUTPUT();//SDA输出

    /* SDA，变化一个位7us */
    for (i = 1; i <= 8; i++)
    {
        if (Tx_Byte & 0x80)//判断高位是否为1
        {

            SDA_H();
        }
        else
        {
            ;
            SDA_L();
        }
        Tx_Byte = Tx_Byte << 1;//将发送的位往左移，后续输出
        Delay_us(2);
        /* 时钟高电平 4us */
        SCL_H();    //SCL=1 拉高，发出
        Delay_us(2);
        SCL_L();//拉低时钟，数据准备
    }
    if (IIC_Ack_Check())
    {
        return IIC_NACK;    //没接收到应答
    }
    return IIC_ACK;     //接收到应答
}



/************************************************************************
 * function   : IIC_Send_Array
 * Description: 发送一个数组
 * input :
 *         uint8_t DevAddress   设备地址
 *         uint8_t *pTxData:    指针,指向发送的数据
 *         uint16_t Size:       发送的个数
 * return:
 *          ACK_Flg: ACK:接收成功
 *                   NACK:接收失败
 ************************************************************************/
uint8_t IIC_Send_Array(uint8_t DevAddress, const uint8_t *pTxData, uint16_t Size)
{
    uint8_t i;
    uint8_t ACK_Flg;

    /* 开始发送 */
    IIC_Start();

    ACK_Flg = IIC_Write_Byte(DevAddress);
    if (ACK_Flg == IIC_ACK)
    {
        for (i = 0; i < Size; i++)
        {
            ACK_Flg = IIC_Write_Byte(pTxData[i]);

            /*若没接收到应答, 结束发送数据*/
            if (ACK_Flg != IIC_ACK)
            {
                break;
            }
        }
    }
    Delay_us(8);
    IIC_Stop();
    /* 停止发送,返回上述流程结束时，获得的应答状态ACK_State */

    /* 打印成功发送的数据 */
    IIC_Debug_printf(DevAddress, pTxData, i, ACK_Flg);
    return  ACK_Flg;
}



/************************************************************************
 * function   : IIC_Receive_Array
 * Description: 接收数据组
 * input :
 *         uint8_t DevAddress    设备的地址
 *         uint8_t *pTxData      写入的命令
 *         uint16_t TxSize       命令的长度
 *         uint8_t *pRxData      读出的数据的存储区
 *         uint16_t RxSize       读出的长度
 *         uint8_t  StopState    接收数据成功后，停止信号的标志位(0: 跳过停止信号)
 * return:
 *         ACK_Flg: ACK:接收成功
 *                  NACK:接收失败
 ************************************************************************/
uint8_t IIC_Receive_Array(uint8_t DevAddress, const uint8_t *pTxData, uint16_t TxSize, uint8_t *pRxData, uint16_t RxSize)
{
    uint8_t i;
    uint8_t ACK_Flg;
    uint8_t Ack;
    IIC_Start();

    ACK_Flg = IIC_Write_Byte(DevAddress);
    if (ACK_Flg == IIC_ACK)
    {
        /* 先写入从哪个寄存器地址获取数据 */
        for (i = 0; i < TxSize; i++)
        {
            ACK_Flg = IIC_Write_Byte(pTxData[i]);

            /*若没接收到应答, 结束接收数据*/
            if (ACK_Flg != IIC_ACK)
            {
                IIC_Stop();
                return ACK_Flg;
            }
        }
				
			IIC_Stop();
    }
    if (pRxData != NULL && RxSize != 0)
    {
        /* 寄存器地址写入成功，准备读取数据 */
        Delay_ms(50);

        IIC_Start();
        IIC_Read_Bytes(0x51, pRxData, RxSize);
        



    }

     IIC_Stop();

    return  ACK_Flg;
}



/* 调试版本时数据打印，用于打印输出的数据 */
void IIC_Data_Printf(uint8_t DevAddress, const uint8_t *pData, uint16_t Size, uint8_t ACK)
{
    uint16_t i;
    if (pData == NULL)
    {
        return;
    }
    printfS("%02X ", DevAddress);
    for (i = 0; i < Size; i++)
    {
        printfS("%02X ", pData[i]);
    }
    if (ACK)
    {
        printf("NACK");
    }
    printfS("\r\n");
}






