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
//////////////////////IO��������
////////////////////#define SDA_IN()  {GPIOF->MODER&=~(3<<(0*2));GPIOF->MODER|=0<<0*2;}															//PF0����ģʽ
////////////////////#define SDA_OUT() {GPIOF->MODER&=~(3<<(0*2));GPIOF->MODER|=1<<0*2;} 														//PF0���ģʽ
//////////////////////IO����
////////////////////#define IIC_SCL(n)  (n?HAL_GPIO_WritePin(GPIOF,GPIO_PIN_1,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOF,GPIO_PIN_1,GPIO_PIN_RESET)) //SCL
////////////////////#define IIC_SDA(n)  (n?HAL_GPIO_WritePin(GPIOF,GPIO_PIN_0,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOF,GPIO_PIN_0,GPIO_PIN_RESET)) //SDA
////////////////////#define READ_SDA    HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_0)  //����SDA
//IO��������
#define SDA_IN()  {GPIOH->MODER&=~(3<<(5*2));GPIOH->MODER|=0<<5*2;}																				//PH5����ģʽ
#define SDA_OUT() {GPIOH->MODER&=~(3<<(5*2));GPIOH->MODER|=1<<5*2;} 																			//PH5���ģʽ
//IO����
#define IIC_SCL(n)  (n?HAL_GPIO_WritePin(GPIOH,GPIO_PIN_4,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOH,GPIO_PIN_4,GPIO_PIN_RESET)) 					//SCL
#define IIC_SDA(n)  (n?HAL_GPIO_WritePin(GPIOH,GPIO_PIN_5,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOH,GPIO_PIN_5,GPIO_PIN_RESET)) 					//SDA
#define READ_SDA    HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_5)  																						//����SDA
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
//iic��������

#if(USE_I2c_File == 1)

static void BSP_I2cInit(void);
//iicʱ��
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
 * @brief :	iic��ʼ��
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
 * @brief :	iic��ʼ��
 * @param :	none.
 * @retval:	none.
 */
static void BSP_I2cInit(void)
{
    GPIO_InitTypeDef GPIO_Initure;

//////////////////////    __HAL_RCC_GPIOF_CLK_ENABLE();   									//ʹ��GPIOHʱ��

//////////////////////    																	//PH4,5��ʼ������
//////////////////////    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1;
//////////////////////    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  							//�������
//////////////////////    GPIO_Initure.Pull=GPIO_PULLUP;          							//����
//////////////////////    GPIO_Initure.Speed=GPIO_SPEED_FAST;     							//����
//////////////////////    HAL_GPIO_Init(GPIOF,&GPIO_Initure);
    __HAL_RCC_GPIOH_CLK_ENABLE();   														//ʹ��GPIOHʱ��

																							//PH4,5��ʼ������
    GPIO_Initure.Pin=GPIO_PIN_4|GPIO_PIN_5;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  												//�������
    GPIO_Initure.Pull=GPIO_PULLUP;          												//����
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     												//����
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);

    IIC_SDA(1);																				//��ʼ��������ʱ���ߺ������߱�ʾ���߿���
    IIC_SCL(1);  																			//
}
/*
********************************************************************************
*                              IIC��дʱ��
********************************************************************************
*/
/**
 * @brief :	IIC��ʼ�ź�
 * @param :	none.
 * @retval:	none.
 */
static void BSP_IicStart(void)
{
    SDA_OUT();     																			//sda�����
    IIC_SDA(1);
    IIC_SCL(1);
    delay_us(4);
    IIC_SDA(0);																				//START:when CLK is high,DATA change form high to low
    delay_us(4);
    IIC_SCL(0);																				//ǯסI2C���ߣ�׼�����ͻ��������
}
/**
 * @brief :	IICֹͣ�ź�
 * @param :	none.
 * @retval:	none.
 */
static void BSP_IicStop(void)
{
    SDA_OUT();																				//sda�����
    IIC_SCL(0);
    IIC_SDA(0);																				//STOP:when CLK is high DATA change form low to high
    delay_us(4);
    IIC_SCL(1);
    delay_us(4);
    IIC_SDA(1);																				//����I2C���߽����ź�
}
/**
 * @brief :	�ȴ�Ӧ���źŵ���
 * @param :	none.
 * @retval:	1������Ӧ��ʧ��
			0������Ӧ��ɹ�
 */
static uint8_t BSP_IicWaitAck(void)
{
    uint8_t ucErrTime=0;
    SDA_IN();      																			//SDA����Ϊ����
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
    IIC_SCL(0);																				//ʱ�����0
    return 0;
}
/**
 * @brief :	����ACKӦ��
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
 * @brief :	������ACKӦ��
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
 * @brief :	IIC����һ���ֽ�
 * @param :	txd��Ҫ���͵��ֽ�
 * @retval:	1����Ӧ��
			0����Ӧ��
 */
static void BSP_IicSendByte(uint8_t txd)
{
    uint8_t t;
    SDA_OUT();
    IIC_SCL(0);																				//����ʱ�ӿ�ʼ���ݴ���
    for(t=0; t<8; t++)
    {
        IIC_SDA((txd&0x80)>>7);
        txd<<=1;
        delay_us(2);   																		//��TEA5767��������ʱ���Ǳ����
        IIC_SCL(1);
        delay_us(2);
        IIC_SCL(0);
        delay_us(2);
    }
}
/**
 * @brief :	��1���ֽ�
 * @param :	ack=1ʱ������ACK��ack=0������nACK
 * @retval:	��ȡ������

 */
static uint8_t BSP_IicReadByte(unsigned char ack)
{
    unsigned char i,receive=0;
    SDA_IN();																				//SDA����Ϊ����
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
        BSP_IicNAck();																		//����nACK
    else
        BSP_IicAck(); 																		//����ACK
    return receive;
}
/*
********************************************************************************
*                              AT24CXX����
********************************************************************************
*/
/**
 * @brief :	��AT24CXXָ����ַ����һ������
 * @param :	ReadAddr:��ʼ�����ĵ�ַ
 * @retval:	��ȡ������

 */
uint8_t BSP_AT24CXX_ReadOneByte(uint16_t ReadAddr)
{
    uint8_t temp=0;
    BSP_IicStart();
    if(0)//EE_TYPE>AT24C16)
    {
        BSP_IicSendByte(0XA0);	   															//����д����
        BSP_IicWaitAck();
        BSP_IicSendByte(ReadAddr>>8);														//���͸ߵ�ַ
        BSP_IicWaitAck();
    } else {
        BSP_IicSendByte(0XA0);	    														//����д����
    }

    BSP_IicWaitAck();
    BSP_IicSendByte(ReadAddr>>8);   														//
    BSP_IicWaitAck();
    BSP_IicSendByte(ReadAddr%256);   														//���͵͵�ַ
    BSP_IicWaitAck();
    BSP_IicStart();
    BSP_IicSendByte(0XA1);           														//�������ģʽ
    BSP_IicWaitAck();
    temp=BSP_IicReadByte(0);
    BSP_IicStop();																			//����һ��ֹͣ����
    return temp;
}
/**
 * @brief :	��AT24CXXָ����ַд��һ������
 * @param :	WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ
			DataToWrite:Ҫд�������
 * @retval:	none

 */
void BSP_AT24CXX_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite)
{
    BSP_IicStart();
    if(0)//EE_TYPE>AT24C16)
    {
        BSP_IicSendByte(0XA0);	    														//����д����
        BSP_IicWaitAck();
        BSP_IicSendByte(WriteAddr>>8);														//���͸ߵ�ַ
    } else
    {
        BSP_IicSendByte(0XA0);	    														//����д����
    }
    BSP_IicWaitAck();
    BSP_IicSendByte(WriteAddr>>8);   														//
    BSP_IicWaitAck();
    BSP_IicSendByte(WriteAddr%256);   														//���͵͵�ַ
    BSP_IicWaitAck();
    BSP_IicSendByte(DataToWrite);    	 													//�����ֽ�
    BSP_IicWaitAck();
    BSP_IicStop();																			//����һ��ֹͣ����
    delay_ms(10);
}
/**
 * @brief :	��AT24CXX�����ָ����ַ��ʼд�볤��ΪLen������(�ú�������д��16bit����32bit������)
 * @param :	WriteAddr  :��ʼд��ĵ�ַ
			DataToWrite:���������׵�ַ
			Len        :Ҫд�����ݵĳ���2,4
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
 * @brief :	��AT24CXX�����ָ����ַ��ʼ��������ΪLen������(�ú������ڶ���16bit����32bit������)
 * @param :	ReadAddr   :��ʼ�����ĵ�ַ
			Len        :Ҫ�������ݵĳ���2,4
 * @retval:	��������

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
 * @brief :	���AT24CXX�Ƿ���������������24XX��һ����ַ(255)���洢��־�֣�ʵ��Ӧ�����޸ģ�
 * @param :	none
 * @retval:	1:���ʧ��
			0:���ɹ�
 */
uint8_t AT24CXX_Check(void)
{
    uint8_t temp;
    temp=BSP_AT24CXX_ReadOneByte(255);													//����ÿ�ο�����дAT24CXX
    if(temp==0X55)return 0;
    else																				//�ų���һ�γ�ʼ�������
    {
        BSP_AT24CXX_WriteOneByte(255,0X55);
        temp=BSP_AT24CXX_ReadOneByte(255);
        if(temp==0X55)return 0;
    }
    return 1;
}
/**
 * @brief :	��AT24CXX�����ָ����ַ��ʼ����ָ������������
 * @param :	ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
			pBuffer  :���������׵�ַ
			NumToRead:Ҫ�������ݵĸ���
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
 * @brief :	��AT24CXX�����ָ����ַ��ʼд��ָ������������
 * @param :	WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
			pBuffer   :���������׵�ַ
			NumToWrite:Ҫд�����ݵĸ���
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
