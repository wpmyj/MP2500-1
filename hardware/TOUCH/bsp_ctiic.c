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
 * @file      bsp_ctiic.c
 * @author    GuanXianDe
 * @version   V1.0.0
 * @date      26-June-2018
 * @brief     program entry
 * @attention ctiic���ݴ���оƬ����
 *******************************************************************************
 */

/*
********************************************************************************
*                              INCLUDE FILES
********************************************************************************
*/
#include "bsp_ctiic.h"
#include "delay.h"
/*
********************************************************************************
*                             MACRO DEFINITIONS
********************************************************************************
*/

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
 * @brief :	����I2C�ٶȵ���ʱ
 * @param :	none.
 * @retval:	none.
 */
void CT_Delay(void)
{
    delay_us(2);
}

/**
 * @brief :	���ݴ���оƬIIC�ӿڳ�ʼ��
 * @param :	none.
 * @retval:	none.
 */
void CT_IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOH_CLK_ENABLE();															//����GPIOHʱ��
    __HAL_RCC_GPIOI_CLK_ENABLE();															//����GPIOIʱ��

    GPIO_Initure.Pin=GPIO_PIN_6;            												//PH6
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  												//�������
    GPIO_Initure.Pull=GPIO_PULLUP;          												//����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     												//����
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);     												//��ʼ��

    GPIO_Initure.Pin=GPIO_PIN_3;            												//PI3
    HAL_GPIO_Init(GPIOI,&GPIO_Initure);     												//��ʼ��
}

/**
 * @brief :	����IIC��ʼ�ź�
 * @param :	none.
 * @retval:	none.
 */
void CT_IIC_Start(void)
{
    CT_SDA_OUT();     																		//sda�����
    CT_IIC_SDA(1);
    CT_IIC_SCL(1);
    delay_us(30);
    CT_IIC_SDA(0);																			//START:when CLK is high,DATA change form high to low
    CT_Delay();
    CT_IIC_SCL(0);																			//ǯסI2C���ߣ�׼�����ͻ��������
}

/**
 * @brief :	����IICֹͣ�ź�
 * @param :	none.
 * @retval:	none.
 */
void CT_IIC_Stop(void)
{
    CT_SDA_OUT();																			//sda�����
    CT_IIC_SCL(1);
    delay_us(30);
    CT_IIC_SDA(0);																			//STOP:when CLK is high DATA change form low to high
    CT_Delay();
    CT_IIC_SDA(1);																			//����I2C���߽����ź�
}

/**
 * @brief :	�ȴ�Ӧ���źŵ���
 * @param :	none.
 * @retval:
			1������Ӧ��ʧ��
			0������Ӧ��ɹ�
 */
uint8_t CT_IIC_Wait_Ack(void)
{
    uint8_t ucErrTime=0;
    CT_SDA_IN();      																		//SDA����Ϊ����
    CT_IIC_SDA(1);
    CT_IIC_SCL(1);
    CT_Delay();
    while(CT_READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            CT_IIC_Stop();
            return 1;
        }
        CT_Delay();
    }
    CT_IIC_SCL(0);																			//ʱ�����0
    return 0;
}

/**
 * @brief :	����ACKӦ��
 * @param :	none.
 * @retval:	none.
 */
void CT_IIC_Ack(void)
{
    CT_IIC_SCL(0);
    CT_SDA_OUT();
    CT_Delay();
    CT_IIC_SDA(0);
    CT_Delay();
    CT_IIC_SCL(1);
    CT_Delay();
    CT_IIC_SCL(0);
}
/**
 * @brief :	������ACKӦ��
 * @param :	none.
 * @retval:	none.
 */
void CT_IIC_NAck(void)
{
    CT_IIC_SCL(0);
    CT_SDA_OUT();
    CT_Delay();
    CT_IIC_SDA(1);
    CT_Delay();
    CT_IIC_SCL(1);
    CT_Delay();
    CT_IIC_SCL(0);
}

/**
 * @brief :	IIC����һ���ֽ�
 * @param :	none.
 * @retval:
			1����Ӧ��
			0����Ӧ��
 */
void CT_IIC_Send_Byte(uint8_t txd)
{
    uint8_t t;
    CT_SDA_OUT();
    CT_IIC_SCL(0);																			//����ʱ�ӿ�ʼ���ݴ���
    CT_Delay();
    for(t=0; t<8; t++)
    {
        CT_IIC_SDA((txd&0x80)>>7);
        txd<<=1;
        CT_IIC_SCL(1);
        CT_Delay();
        CT_IIC_SCL(0);
        CT_Delay();
    }
}

/**
 * @brief :	��1���ֽ�
 * @param :	ack=1ʱ������ACK��ack=0������nACK
 * @retval:	none.
 */
uint8_t CT_IIC_Read_Byte(unsigned char ack)
{
    uint8_t i,receive=0;
    CT_SDA_IN();																			//SDA����Ϊ����
    delay_us(30);
    for(i=0; i<8; i++ )
    {
        CT_IIC_SCL(0);
        CT_Delay();
        CT_IIC_SCL(1);
        receive<<=1;
        if(CT_READ_SDA)receive++;
    }
    if (!ack)CT_IIC_NAck();																	//����nACK
    else CT_IIC_Ack(); 																		//����ACK
    return receive;
}


/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/































