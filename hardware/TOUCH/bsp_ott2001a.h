/*******************************************************************************
*                               APPLICATION CODE
*
* (c)Copyright Guangdong ENECO POWER Science And Technology Co., Ltd.2015-2018.
*
*      All rights reserved.  Protected by international copyright laws.
*       Knowledge of the source code may NOT be used without authorization.
********************************************************************************
*/


/**
 *******************************************************************************
 * @file      bsp_ott2001a.h
 * @author    GuanXianDe
 * @version   V1.0.0
 * @date      26-June-2018
 * @brief     Header for main.c module
 * @attention
 *******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_OTT2001A_H
#define __BSP_OTT2001A_H

/*
********************************************************************************
*                                 INCLUDE FILES
********************************************************************************
*/

#include "sys.h"

/*
********************************************************************************
*                              EXPORTED DATA TYPES
********************************************************************************
*/

//IO��������
#define OTT_RST(n)  (n?HAL_GPIO_WritePin(GPIOI,GPIO_PIN_8,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOI,GPIO_PIN_8,GPIO_PIN_RESET))//OTT2001A��λ����
#define OTT_INT      HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_7)  //OTT2001A������		

									//ͨ��OTT_SET_REGָ��,���Բ�ѯ�������Ϣ
									//ע��,�����X,Y����Ļ������ϵ�պ��Ƿ���.
#define OTT_MAX_X 		2700	 	//TP X��������ֵ(������)
#define OTT_MAX_Y 		1500    	//TP Y��������ֵ(�᷽��)

//��������
#define OTT_SCAL_X		0.2963		//��Ļ�� ������/OTT_MAX_X		
#define OTT_SCAL_Y		0.32		//��Ļ�� ������/OTT_MAX_Y		

//I2C��д����
#define OTT_CMD_WR 		0XB2     	//д����
#define OTT_CMD_RD 		0XB3		//������

//�Ĵ�����ַ
#define OTT_GSTID_REG 	0X0000   	//OTT2001A��ǰ��⵽�Ĵ������
#define OTT_TP1_REG 	0X0100  	//��һ�����������ݵ�ַ
#define OTT_TP2_REG 	0X0500		//�ڶ������������ݵ�ַ
#define OTT_TP3_REG 	0X1000		//���������������ݵ�ַ
#define OTT_TP4_REG 	0X1400		//���ĸ����������ݵ�ַ
#define OTT_TP5_REG 	0X1800		//��������������ݵ�ַ  
#define OTT_SET_REG   	0X0900   	//�ֱ������üĴ�����ַ
#define OTT_CTRL_REG  	0X0D00   	//����������(��/��)  

/*
********************************************************************************
*                               EXPORTED CONSTANT
********************************************************************************
*/



/*
********************************************************************************
*                                EXPORTED MACRO
********************************************************************************
*/



/*
********************************************************************************
*                                   EXTERNS
********************************************************************************
*/


/*
********************************************************************************
*                              FUNCTION PROTOTYPES
********************************************************************************
*/
uint8_t OTT2001A_WR_Reg(uint16_t reg,uint8_t *buf,uint8_t len);		//д�Ĵ���(ʵ������)
void OTT2001A_RD_Reg(uint16_t reg,uint8_t *buf,uint8_t len);		//���Ĵ���
void OTT2001A_SensorControl(uint8_t cmd);							//��������/�رղ���
uint8_t OTT2001A_Init(void); 										//4.3���ݴ�����ʼ������
uint8_t OTT2001A_Scan(uint8_t mode);								//���ݴ�����ɨ�躯��


#endif /* __BSP_OTT2001A_H */

/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/


