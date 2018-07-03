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
 * @file      bsp_OTT2001A.c
 * @author    GuanXianDe
 * @version   V1.0.0
 * @date      26-June-2018
 * @brief     program entry
 * @attention OTT2001A����������
 *******************************************************************************
 */

/*
********************************************************************************
*                              INCLUDE FILES
********************************************************************************
*/
#include "bsp_ott2001a.h"
#include "bsp_touch.h"
#include "bsp_ctiic.h"
#include "usart.h"
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
 * @brief :	��OTT2001Aд��һ������
 * @param :
			reg:��ʼ�Ĵ�����ַ
			buf:���ݻ�������
			len:д���ݳ���
 * @retval:	0,�ɹ�;1,ʧ��.
 */
uint8_t OTT2001A_WR_Reg(uint16_t reg,uint8_t *buf,uint8_t len)
{
    uint8_t i;
    uint8_t ret=0;
    CT_IIC_Start();
    CT_IIC_Send_Byte(OTT_CMD_WR);   								//����д����
    CT_IIC_Wait_Ack();
    CT_IIC_Send_Byte(reg>>8);   									//���͸�8λ��ַ
    CT_IIC_Wait_Ack();
    CT_IIC_Send_Byte(reg&0XFF);   									//���͵�8λ��ַ
    CT_IIC_Wait_Ack();
    for(i=0; i<len; i++)
    {
        CT_IIC_Send_Byte(buf[i]);  									//������
        ret=CT_IIC_Wait_Ack();
        if(ret)break;
    }
    CT_IIC_Stop();													//����һ��ֹͣ����
    return ret;
}

/**
 * @brief :	��OTT2001A����һ������
 * @param :
			reg:��ʼ�Ĵ�����ַ
			buf:���ݻ�������
			len:�����ݳ���
 * @retval:	none.
 */
void OTT2001A_RD_Reg(uint16_t reg,uint8_t *buf,uint8_t len)
{
    uint8_t i;
    CT_IIC_Start();
    CT_IIC_Send_Byte(OTT_CMD_WR);   								//����д����
    CT_IIC_Wait_Ack();
    CT_IIC_Send_Byte(reg>>8);   									//���͸�8λ��ַ
    CT_IIC_Wait_Ack();
    CT_IIC_Send_Byte(reg&0XFF);   									//���͵�8λ��ַ
    CT_IIC_Wait_Ack();
    CT_IIC_Start();
    CT_IIC_Send_Byte(OTT_CMD_RD);   								//���Ͷ�����
    CT_IIC_Wait_Ack();
    for(i=0; i<len; i++)
    {
        buf[i]=CT_IIC_Read_Byte(i==(len-1)?0:1); 					//������
    }
    CT_IIC_Stop();													//����һ��ֹͣ����
}

/**
 * @brief :	��������/�رղ���
 * @param :	cmd:1,�򿪴�����;0,�رմ�����
 * @retval:	none.
 */
void OTT2001A_SensorControl(uint8_t cmd)
{
    uint8_t regval=0X00;
    if(cmd)regval=0X80;
    OTT2001A_WR_Reg(OTT_CTRL_REG,&regval,1);
}

/**
 * @brief :	��ʼ��������
 * @param :	none.
 * @retval:	����ֵ:0,��ʼ���ɹ�;1,��ʼ��ʧ��
 */
uint8_t OTT2001A_Init(void)
{
    uint8_t regval=0;
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOH_CLK_ENABLE();									//����GPIOHʱ��
    __HAL_RCC_GPIOI_CLK_ENABLE();									//����GPIOIʱ��

    //PH7
    GPIO_Initure.Pin=GPIO_PIN_7;            						//PH7
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      						//����
    GPIO_Initure.Pull=GPIO_PULLUP;          						//����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     						//����
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);     						//��ʼ��

    //PI8
    GPIO_Initure.Pin=GPIO_PIN_8;            						//PI8
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  						//�������
    HAL_GPIO_Init(GPIOI,&GPIO_Initure);     						//��ʼ��

    CT_IIC_Init();      											//��ʼ����������I2C����
    OTT_RST(0);														//��λ
    delay_ms(100);
    OTT_RST(1);														//�ͷŸ�λ
    delay_ms(100);
    OTT2001A_SensorControl(1);										//�򿪴�����
    OTT2001A_RD_Reg(OTT_CTRL_REG,&regval,1);						//��ȡ���������мĴ�����ֵ���ж�I2Cͨ���Ƿ�����
    printf("CTP ID:%x\r\n",regval);
    if(regval==0x80)return 0;
    return 1;
}


const uint16_t OTT_TPX_TBL[5]= {OTT_TP1_REG,OTT_TP2_REG,OTT_TP3_REG,OTT_TP4_REG,OTT_TP5_REG};

/**
 * @brief :	ɨ�败����(���ò�ѯ��ʽ)
 * @param :	mode:0,����ɨ��.
 * @retval:	��ǰ����״̬.0,�����޴���;1,�����д���
 */
uint8_t OTT2001A_Scan(uint8_t mode)
{
    uint8_t buf[4];
    uint8_t i=0;
    uint8_t res=0;
    static uint8_t t=0;												//���Ʋ�ѯ���,�Ӷ�����CPUռ����
    t++;
    if((t%10)==0||t<10)												//����ʱ,ÿ����10��CTP_Scan�����ż��1��,�Ӷ���ʡCPUʹ����
    {
        OTT2001A_RD_Reg(OTT_GSTID_REG,&mode,1);						//��ȡ�������״̬
        if(mode&0X1F)
        {
            tp_dev.sta=(mode&0X1F)|TP_PRES_DOWN|TP_CATH_PRES;
            for(i=0; i<5; i++)
            {
                if(tp_dev.sta&(1<<i))								//������Ч?
                {
                    OTT2001A_RD_Reg(OTT_TPX_TBL[i],buf,4);			//��ȡXY����ֵ
                    if(tp_dev.touchtype&0X01)						//����
                    {
                        tp_dev.y[i]=(((uint16_t)buf[2]<<8)+buf[3])*OTT_SCAL_Y;
                        tp_dev.x[i]=800-((((uint16_t)buf[0]<<8)+buf[1])*OTT_SCAL_X);
                    } else
                    {
                        tp_dev.x[i]=(((uint16_t)buf[2]<<8)+buf[3])*OTT_SCAL_Y;
                        tp_dev.y[i]=(((uint16_t)buf[0]<<8)+buf[1])*OTT_SCAL_X;
                    }
                    //printf("x[%d]:%d,y[%d]:%d\r\n",i,tp_dev.x[i],i,tp_dev.y[i]);
                }
            }
            res=1;
            if(tp_dev.x[0]==0 && tp_dev.y[0]==0)mode=0;				//���������ݶ���0,����Դ˴�����
            t=0;													//����һ��,��������������10��,�Ӷ����������
        }
    }
    if((mode&0X1F)==0)												//�޴����㰴��
    {
        if(tp_dev.sta&TP_PRES_DOWN)									//֮ǰ�Ǳ����µ�
        {
            tp_dev.sta&=~(1<<7);									//��ǰ����ɿ�
        } else														//֮ǰ��û�б�����
        {
            tp_dev.x[0]=0xffff;
            tp_dev.y[0]=0xffff;
            tp_dev.sta&=0XE0;										//�������Ч���
        }
    }
    if(t>240)t=10;													//���´�10��ʼ����
    return res;
}


/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/


