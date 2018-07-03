/*******************************************************************************
*                              APPLICATION CODE
*
* (c)Copyright Guangdong ENECO POWER Science And Technology Co., Ltd.2015-2018.
*
*      All rights reserved.  Protected by international copyright laws.
*       Knowledge of the source code may NOT be used without authorization.
********************************************************************************
*/

/**
 *******************************************************************************
 * @file      bsp_iic.c
 * @author    GuanXianDe
 * @version   V1.0.0
 * @date      14-June-2018
 * @brief     program entry
 * @attention none.
 *******************************************************************************
 */

/*
********************************************************************************
*                              INCLUDE FILES
********************************************************************************
*/
#include "bsp_iic.h"
#include "delay.h"
/*
********************************************************************************
*                             MACRO DEFINITIONS
********************************************************************************
*/
#if(USE_I2c_File == 1)
//////////////////////IO方向设置
////////////////////#define SDA_IN()  {GPIOF->MODER&=~(3<<(0*2));GPIOF->MODER|=0<<0*2;}															//PF0输入模式
////////////////////#define SDA_OUT() {GPIOF->MODER&=~(3<<(0*2));GPIOF->MODER|=1<<0*2;} 														//PF0输出模式
//////////////////////IO操作
////////////////////#define IIC_SCL(n)  (n?HAL_GPIO_WritePin(GPIOF,GPIO_PIN_1,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOF,GPIO_PIN_1,GPIO_PIN_RESET)) //SCL
////////////////////#define IIC_SDA(n)  (n?HAL_GPIO_WritePin(GPIOF,GPIO_PIN_0,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOF,GPIO_PIN_0,GPIO_PIN_RESET)) //SDA
////////////////////#define READ_SDA    HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_0)  //输入SDA
//IO方向设置
#define SDA_IN()  {GPIOH->MODER&=~(3<<(5*2));GPIOH->MODER|=0<<5*2;}																				//PH5输入模式
#define SDA_OUT() {GPIOH->MODER&=~(3<<(5*2));GPIOH->MODER|=1<<5*2;} 																			//PH5输出模式
//IO操作
#define IIC_SCL(n)  (n?HAL_GPIO_WritePin(GPIOH,GPIO_PIN_4,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOH,GPIO_PIN_4,GPIO_PIN_RESET)) 					//SCL
#define IIC_SDA(n)  (n?HAL_GPIO_WritePin(GPIOH,GPIO_PIN_5,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOH,GPIO_PIN_5,GPIO_PIN_RESET)) 					//SDA
#define READ_SDA    HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_5)  																						//输入SDA
#endif
/*
********************************************************************************
*                          OS-RELATED    VARIABLES
********************************************************************************
*/

/*
********************************************************************************
*                             FUNCTION PROTOTYPES
********************************************************************************
*/
//iic引脚配置

#if(USE_I2c_File == 1)

static void BSP_I2cInit(void);
//iic时序
static void BSP_IicStart(void);
static void BSP_IicStop(void);
static uint8_t BSP_IicWaitAck(void);
static void BSP_IicAck(void);
static void BSP_IicNAck(void);
static void BSP_IicSendByte(uint8_t txd);
static uint8_t BSP_IicReadByte(unsigned char ack);

#endif
/*
********************************************************************************
*                              LOCAL VARIABLES
********************************************************************************
*/

/*
********************************************************************************
*                               GLOBAL VARIABLES
********************************************************************************
*/



/*
********************************************************************************
*                              FUNCTION DEFINITION
********************************************************************************
*/

/**
 * @brief :	iic初始化
 * @param :	none.
 * @retval:	none.
 */
void BSP_IicInit(void)
{
#if(USE_I2c_File == 1)
#if(USE_I2c_Conf == 1)
	BSP_I2cInit();
#endif
#endif
}
#if(USE_I2c_File == 1)
/**
 * @brief :	iic初始化
 * @param :	none.
 * @retval:	none.
 */
static void BSP_I2cInit(void)
{
    GPIO_InitTypeDef GPIO_Initure;

//////////////////////    __HAL_RCC_GPIOF_CLK_ENABLE();   									//使能GPIOH时钟

//////////////////////    																	//PH4,5初始化设置
//////////////////////    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1;
//////////////////////    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  							//推挽输出
//////////////////////    GPIO_Initure.Pull=GPIO_PULLUP;          							//上拉
//////////////////////    GPIO_Initure.Speed=GPIO_SPEED_FAST;     							//快速
//////////////////////    HAL_GPIO_Init(GPIOF,&GPIO_Initure);
    __HAL_RCC_GPIOH_CLK_ENABLE();   														//使能GPIOH时钟

																							//PH4,5初始化设置
    GPIO_Initure.Pin=GPIO_PIN_4|GPIO_PIN_5;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  												//推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          												//上拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     												//快速
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);

    IIC_SDA(1);																				//初始化是拉高时钟线和数据线表示总线空闲
    IIC_SCL(1);  																			//
}
/*
********************************************************************************
*                              IIC读写时序
********************************************************************************
*/
/**
 * @brief :	IIC起始信号
 * @param :	none.
 * @retval:	none.
 */
static void BSP_IicStart(void)
{
    SDA_OUT();     																			//sda线输出
    IIC_SDA(1);
    IIC_SCL(1);
    delay_us(4);
    IIC_SDA(0);																				//START:when CLK is high,DATA change form high to low
    delay_us(4);
    IIC_SCL(0);																				//钳住I2C总线，准备发送或接收数据
}
/**
 * @brief :	IIC停止信号
 * @param :	none.
 * @retval:	none.
 */
static void BSP_IicStop(void)
{
    SDA_OUT();																				//sda线输出
    IIC_SCL(0);
    IIC_SDA(0);																				//STOP:when CLK is high DATA change form low to high
    delay_us(4);
    IIC_SCL(1);
    delay_us(4);
    IIC_SDA(1);																				//发送I2C总线结束信号
}
/**
 * @brief :	等待应答信号到来
 * @param :	none.
 * @retval:	1，接收应答失败
			0，接收应答成功
 */
static uint8_t BSP_IicWaitAck(void)
{
    uint8_t ucErrTime=0;
    SDA_IN();      																			//SDA设置为输入
    IIC_SDA(1);
    delay_us(1);
    IIC_SCL(1);
    delay_us(1);
    while(READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            BSP_IicStop();
            return 1;
        }
    }
    IIC_SCL(0);																				//时钟输出0
    return 0;
}
/**
 * @brief :	产生ACK应答
 * @param :	none.
 * @retval:	none.
 */
static void BSP_IicAck(void)
{
    IIC_SCL(0);
    SDA_OUT();
    IIC_SDA(0);
    delay_us(2);
    IIC_SCL(1);
    delay_us(2);
    IIC_SCL(0);
}
/**
 * @brief :	不产生ACK应答
 * @param :	none.
 * @retval:	none.
 */
static void BSP_IicNAck(void)
{
    IIC_SCL(0);
    SDA_OUT();
    IIC_SDA(1);
    delay_us(2);
    IIC_SCL(1);
    delay_us(2);
    IIC_SCL(0);
}
/**
 * @brief :	IIC发送一个字节
 * @param :	txd：要发送的字节
 * @retval:	1，有应答
			0，无应答
 */
static void BSP_IicSendByte(uint8_t txd)
{
    uint8_t t;
    SDA_OUT();
    IIC_SCL(0);																				//拉低时钟开始数据传输
    for(t=0; t<8; t++)
    {
        IIC_SDA((txd&0x80)>>7);
        txd<<=1;
        delay_us(2);   																		//对TEA5767这三个延时都是必须的
        IIC_SCL(1);
        delay_us(2);
        IIC_SCL(0);
        delay_us(2);
    }
}
/**
 * @brief :	读1个字节
 * @param :	ack=1时，发送ACK，ack=0，发送nACK
 * @retval:	读取的数据

 */
static uint8_t BSP_IicReadByte(unsigned char ack)
{
    unsigned char i,receive=0;
    SDA_IN();																				//SDA设置为输入
    for(i=0; i<8; i++ )
    {
        IIC_SCL(0);
        delay_us(2);
        IIC_SCL(1);
        receive<<=1;
        if(READ_SDA)receive++;
        delay_us(1);
    }
    if (!ack)
        BSP_IicNAck();																		//发送nACK
    else
        BSP_IicAck(); 																		//发送ACK
    return receive;
}
/*
********************************************************************************
*                              AT24CXX操作
********************************************************************************
*/
/**
 * @brief :	在AT24CXX指定地址读出一个数据
 * @param :	ReadAddr:开始读数的地址
 * @retval:	读取的数据

 */
uint8_t BSP_AT24CXX_ReadOneByte(uint16_t ReadAddr)
{
    uint8_t temp=0;
    BSP_IicStart();
    if(0)//EE_TYPE>AT24C16)
    {
        BSP_IicSendByte(0XA0);	   															//发送写命令
        BSP_IicWaitAck();
        BSP_IicSendByte(ReadAddr>>8);														//发送高地址
        BSP_IicWaitAck();
    } else {
        BSP_IicSendByte(0XA0);	    														//发送写命令
    }

    BSP_IicWaitAck();
    BSP_IicSendByte(ReadAddr>>8);   														//
    BSP_IicWaitAck();
    BSP_IicSendByte(ReadAddr%256);   														//发送低地址
    BSP_IicWaitAck();
    BSP_IicStart();
    BSP_IicSendByte(0XA1);           														//进入接收模式
    BSP_IicWaitAck();
    temp=BSP_IicReadByte(0);
    BSP_IicStop();																			//产生一个停止条件
    return temp;
}
/**
 * @brief :	在AT24CXX指定地址写入一个数据
 * @param :	WriteAddr  :写入数据的目的地址
			DataToWrite:要写入的数据
 * @retval:	none

 */
void BSP_AT24CXX_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite)
{
    BSP_IicStart();
    if(0)//EE_TYPE>AT24C16)
    {
        BSP_IicSendByte(0XA0);	    														//发送写命令
        BSP_IicWaitAck();
        BSP_IicSendByte(WriteAddr>>8);														//发送高地址
    } else
    {
        BSP_IicSendByte(0XA0);	    														//发送写命令
    }
    BSP_IicWaitAck();
    BSP_IicSendByte(WriteAddr>>8);   														//
    BSP_IicWaitAck();
    BSP_IicSendByte(WriteAddr%256);   														//发送低地址
    BSP_IicWaitAck();
    BSP_IicSendByte(DataToWrite);    	 													//发送字节
    BSP_IicWaitAck();
    BSP_IicStop();																			//产生一个停止条件
    delay_ms(10);
}
/**
 * @brief :	在AT24CXX里面的指定地址开始写入长度为Len的数据(该函数用于写入16bit或者32bit的数据)
 * @param :	WriteAddr  :开始写入的地址
			DataToWrite:数据数组首地址
			Len        :要写入数据的长度2,4
 * @retval:	none

 */
void BSP_AT24CXX_WriteLenByte(uint16_t WriteAddr,uint32_t DataToWrite,uint8_t Len)
{
    uint8_t t;
    for(t=0; t<Len; t++)
    {
        BSP_AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
    }
}
/**
 * @brief :	在AT24CXX里面的指定地址开始读出长度为Len的数据(该函数用于读出16bit或者32bit的数据)
 * @param :	ReadAddr   :开始读出的地址
			Len        :要读出数据的长度2,4
 * @retval:	读出数据

 */
uint32_t BSP_AT24CXX_ReadLenByte(uint16_t ReadAddr,uint8_t Len)
{
    uint8_t t;
    uint32_t temp = 0;
    for(t=0; t<Len; t++)
    {
        temp<<=8;
        temp+=BSP_AT24CXX_ReadOneByte(ReadAddr+Len-t-1);
    }
    return temp;
}
/**
 * @brief :	检查AT24CXX是否正常，这里用了24XX的一个地址(255)来存储标志字（实际应用再修改）
 * @param :	none
 * @retval:	1:检测失败
			0:检测成功
 */
uint8_t AT24CXX_Check(void)
{
    uint8_t temp;
    temp=BSP_AT24CXX_ReadOneByte(255);													//避免每次开机都写AT24CXX
    if(temp==0X55)return 0;
    else																				//排除第一次初始化的情况
    {
        BSP_AT24CXX_WriteOneByte(255,0X55);
        temp=BSP_AT24CXX_ReadOneByte(255);
        if(temp==0X55)return 0;
    }
    return 1;
}
/**
 * @brief :	在AT24CXX里面的指定地址开始读出指定个数的数据
 * @param :	ReadAddr :开始读出的地址 对24c02为0~255
			pBuffer  :数据数组首地址
			NumToRead:要读出数据的个数
 * @retval:	none
 */
void BSP_AT24CXX_Read(uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead)
{
    while(NumToRead)
    {
        *pBuffer++=BSP_AT24CXX_ReadOneByte(ReadAddr++);
        NumToRead--;
    }
}
/**
 * @brief :	在AT24CXX里面的指定地址开始写入指定个数的数据
 * @param :	WriteAddr :开始写入的地址 对24c02为0~255
			pBuffer   :数据数组首地址
			NumToWrite:要写入数据的个数
 * @retval:	none
 */
void BSP_AT24CXX_Write(uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite)
{
    while(NumToWrite--)
    {
        BSP_AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
        WriteAddr++;
        pBuffer++;
    }
}

#endif
/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/
