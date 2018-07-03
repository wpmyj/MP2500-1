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
 * @file      bsp_ft5206.c
 * @author    GuanXianDe
 * @version   V1.0.0
 * @date      26-June-2018
 * @brief     program entry
 * @attention FT5206��������
 *******************************************************************************
 */

/*
********************************************************************************
*                              INCLUDE FILES
********************************************************************************
*/
#include "bsp_ft5206.h"
#include "bsp_touch.h"
#include "bsp_ctiic.h"
#include "usart.h"
#include "delay.h"
#include "string.h"
#include "bsp_lcd.h"
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
 * @brief :	��FT5206д��һ������
 * @param :
			reg:��ʼ�Ĵ�����ַ
			buf:���ݻ�������
			len:д���ݳ���
 * @retval:	0,�ɹ�;1,ʧ��.
 */
uint8_t FT5206_WR_Reg(uint16_t reg,uint8_t *buf,uint8_t len)
{
    uint8_t i;
    uint8_t ret=0;
    CT_IIC_Start();
    CT_IIC_Send_Byte(FT_CMD_WR);												//����д����
    CT_IIC_Wait_Ack();
    CT_IIC_Send_Byte(reg&0XFF);   												//���͵�8λ��ַ
    CT_IIC_Wait_Ack();
    for(i=0; i<len; i++)
    {
        CT_IIC_Send_Byte(buf[i]);  												//������
        ret=CT_IIC_Wait_Ack();
        if(ret)break;
    }
    CT_IIC_Stop();																//����һ��ֹͣ����
    return ret;
}

/**
 * @brief :	��FT5206����һ������
 * @param :
			reg:��ʼ�Ĵ�����ַ
			buf:���ݻ�������
			len:�����ݳ���
 * @retval:	none.
 */
void FT5206_RD_Reg(uint16_t reg,uint8_t *buf,uint8_t len)
{
    uint8_t i;
    CT_IIC_Start();
    CT_IIC_Send_Byte(FT_CMD_WR);   												//����д����
    CT_IIC_Wait_Ack();
    CT_IIC_Send_Byte(reg&0XFF);   												//���͵�8λ��ַ
    CT_IIC_Wait_Ack();
    CT_IIC_Start();
    CT_IIC_Send_Byte(FT_CMD_RD);   												//���Ͷ�����
    CT_IIC_Wait_Ack();
    for(i=0; i<len; i++)
    {
        buf[i]=CT_IIC_Read_Byte(i==(len-1)?0:1); 								//������
    }
    CT_IIC_Stop();																//����һ��ֹͣ����
}

/**
 * @brief :	��ʼ��FT5206������
 * @param :	none.
 * @retval:	0,��ʼ���ɹ�;1,��ʼ��ʧ��
 */
uint8_t FT5206_Init(void)
{
    uint8_t temp[2];
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOH_CLK_ENABLE();												//����GPIOHʱ��
    __HAL_RCC_GPIOI_CLK_ENABLE();												//����GPIOIʱ��

    //PH7
    GPIO_Initure.Pin=GPIO_PIN_7;            									//PH7
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      									//����
    GPIO_Initure.Pull=GPIO_PULLUP;          									//����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     									//����
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);     									//��ʼ��

    //PI8
    GPIO_Initure.Pin=GPIO_PIN_8;            									//PI8
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  									//�������
    HAL_GPIO_Init(GPIOI,&GPIO_Initure);     									//��ʼ��

    CT_IIC_Init();      														//��ʼ����������I2C����
    FT_RST(0);																	//��λ
    delay_ms(20);
    FT_RST(1);																	//�ͷŸ�λ
    delay_ms(50);
    temp[0]=0;
    FT5206_WR_Reg(FT_DEVIDE_MODE,temp,1);										//������������ģʽ
    FT5206_WR_Reg(FT_ID_G_MODE,temp,1);											//��ѯģʽ
    temp[0]=22;																	//������Чֵ��22��ԽСԽ����
    FT5206_WR_Reg(FT_ID_G_THGROUP,temp,1);										//���ô�����Чֵ
    temp[0]=12;																	//�������ڣ�����С��12�����14
    FT5206_WR_Reg(FT_ID_G_PERIODACTIVE,temp,1);
																				//��ȡ�汾�ţ��ο�ֵ��0x3003
    FT5206_RD_Reg(FT_ID_G_LIB_VERSION,&temp[0],2);
    if((temp[0]==0X30&&temp[1]==0X03)||temp[1]==0X01||temp[1]==0X02)			//�汾:0X3003/0X0001/0X0002
    {
        printf("CTP ID:%x\r\n",((uint16_t)temp[0]<<8)+temp[1]);
        return 0;
    }
    return 1;
}

const uint16_t FT5206_TPX_TBL[5]= {FT_TP1_REG,FT_TP2_REG,FT_TP3_REG,FT_TP4_REG,FT_TP5_REG};

/**
 * @brief :	ɨ�败����(���ò�ѯ��ʽ)
 * @param :	mode:0,����ɨ��.
 * @retval:	��ǰ����״̬.
			0,�����޴���;1,�����д���
 */
uint8_t FT5206_Scan(uint8_t mode)
{
    uint8_t buf[4];
    uint8_t i=0;
    uint8_t res=0;
    uint8_t temp;
    static uint8_t t=0;															//���Ʋ�ѯ���,�Ӷ�����CPUռ����
    t++;
    if((t%10)==0||t<10)															//����ʱ,ÿ����10��CTP_Scan�����ż��1��,�Ӷ���ʡCPUʹ����
    {
        FT5206_RD_Reg(FT_REG_NUM_FINGER,&mode,1);								//��ȡ�������״̬
        if((mode&0XF)&&((mode&0XF)<6))
        {
            temp=0XFF<<(mode&0XF);												//����ĸ���ת��Ϊ1��λ��,ƥ��tp_dev.sta����
            tp_dev.sta=(~temp)|TP_PRES_DOWN|TP_CATH_PRES;
            for(i=0; i<5; i++)
            {
                if(tp_dev.sta&(1<<i))											//������Ч?
                {
                    FT5206_RD_Reg(FT5206_TPX_TBL[i],buf,4);						//��ȡXY����ֵ
                    if(tp_dev.touchtype&0X01)									//����
                    {
                        tp_dev.y[i]=((uint16_t)(buf[0]&0X0F)<<8)+buf[1];
                        tp_dev.x[i]=((uint16_t)(buf[2]&0X0F)<<8)+buf[3];
                    } else
                    {
                        tp_dev.x[i]=lcddev.width-(((uint16_t)(buf[0]&0X0F)<<8)+buf[1]);
                        tp_dev.y[i]=((uint16_t)(buf[2]&0X0F)<<8)+buf[3];
                    }
                    if((buf[0]&0XF0)!=0X80)tp_dev.x[i]=tp_dev.y[i]=0;			//������contact�¼�������Ϊ��Ч
                    //printf("x[%d]:%d,y[%d]:%d\r\n",i,tp_dev.x[i],i,tp_dev.y[i]);
                }
            }
            res=1;
            if(tp_dev.x[0]==0 && tp_dev.y[0]==0)mode=0;							//���������ݶ���0,����Դ˴�����
            t=0;																//����һ��,��������������10��,�Ӷ����������
        }
    }
    if((mode&0X1F)==0)															//�޴����㰴��
    {
        if(tp_dev.sta&TP_PRES_DOWN)												//֮ǰ�Ǳ����µ�
        {
            tp_dev.sta&=~(1<<7);												//��ǰ����ɿ�
        } else																	//֮ǰ��û�б�����
        {
            tp_dev.x[0]=0xffff;
            tp_dev.y[0]=0xffff;
            tp_dev.sta&=0XE0;													//�������Ч���
        }
    }
    if(t>240)t=10;																//���´�10��ʼ����
    return res;
}

/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/






























