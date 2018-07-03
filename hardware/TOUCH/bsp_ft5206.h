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
 * @file      bsp_ft5206.h
 * @author    GuanXianDe
 * @version   V1.0.0
 * @date      26-June-2018
 * @brief     Header for main.c module
 * @attention
 *******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_FT5206_H
#define __BSP_FT5206_H

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
																															//����ݴ��������ӵ�оƬ����(δ����IIC����)
																															//IO��������
#define FT_RST(n)  (n?HAL_GPIO_WritePin(GPIOI,GPIO_PIN_8,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOI,GPIO_PIN_8,GPIO_PIN_RESET))	//FT5206��λ����
#define FT_INT      HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_7)  																	//FT5206������	

																															//I2C��д����
#define FT_CMD_WR 							0X70    																		//д����
#define FT_CMD_RD 							0X71																			//������

																															//FT5206 ���ּĴ�������
#define FT_DEVIDE_MODE 						0x00   																			//FT5206ģʽ���ƼĴ���
#define FT_REG_NUM_FINGER       			0x02																			//����״̬�Ĵ���

#define FT_TP1_REG 							0X03	  																		//��һ�����������ݵ�ַ
#define FT_TP2_REG 							0X09																			//�ڶ������������ݵ�ַ
#define FT_TP3_REG 							0X0F																			//���������������ݵ�ַ
#define FT_TP4_REG 							0X15																			//���ĸ����������ݵ�ַ
#define FT_TP5_REG 							0X1B																			//��������������ݵ�ַ  


#define	FT_ID_G_LIB_VERSION					0xA1																			//�汾		
#define FT_ID_G_MODE 						0xA4   																			//FT5206�ж�ģʽ���ƼĴ���
#define FT_ID_G_THGROUP						0x80   																			//������Чֵ���üĴ���
#define FT_ID_G_PERIODACTIVE				0x88   																			//����״̬�������üĴ���


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
uint8_t FT5206_WR_Reg(uint16_t reg,uint8_t *buf,uint8_t len);
void FT5206_RD_Reg(uint16_t reg,uint8_t *buf,uint8_t len);
uint8_t FT5206_Init(void);
uint8_t FT5206_Scan(uint8_t mode);


#endif /* __BSP_XXX_H */

/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/


