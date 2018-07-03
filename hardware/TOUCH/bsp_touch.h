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
 * @file      bsp_touch.h
 * @author    GuanXianDe
 * @version   V1.0.0
 * @date      26-June-2018
 * @brief     Header for main.c module
 * @attention
 *******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_TOUCH_H
#define __BSP_TOUCH_H

/*
********************************************************************************
*                                 INCLUDE FILES
********************************************************************************
*/

#include "sys.h"
#include "bsp_ott2001a.h"
#include "bsp_gt9147.h"
#include "bsp_ft5206.h"
/*
********************************************************************************
*                              EXPORTED DATA TYPES
********************************************************************************
*/
#define TP_PRES_DOWN 0x80  //����������	  
#define TP_CATH_PRES 0x40  //�а��������� 
#define CT_MAX_TOUCH  5    //������֧�ֵĵ���,�̶�Ϊ5��


/*
********************************************************************************
*                               EXPORTED CONSTANT
********************************************************************************
*/
//������������
typedef struct
{
    uint8_t (*init)(void);			//��ʼ��������������
    uint8_t (*scan)(uint8_t);				//ɨ�败����.0,��Ļɨ��;1,��������;
    void (*adjust)(void);		//������У׼
    uint16_t x[CT_MAX_TOUCH]; 		//��ǰ����
    uint16_t y[CT_MAX_TOUCH];		//�����������5������,����������x[0],y[0]����:�˴�ɨ��ʱ,����������,��
    //x[4],y[4]�洢��һ�ΰ���ʱ������.
    uint8_t  sta;					//�ʵ�״̬
    //b7:����1/�ɿ�0;
    //b6:0,û�а�������;1,�а�������.
    //b5:����
    //b4~b0:���ݴ��������µĵ���(0,��ʾδ����,1��ʾ����)
/////////////////////������У׼����(����������ҪУ׼)//////////////////////
    float xfac;
    float yfac;
    short xoff;
    short yoff;
//�����Ĳ���,��������������������ȫ�ߵ�ʱ��Ҫ�õ�.
//b0:0,����(�ʺ�����ΪX����,����ΪY�����TP)
//   1,����(�ʺ�����ΪY����,����ΪX�����TP)
//b1~6:����.
//b7:0,������
//   1,������
    uint8_t touchtype;
} _m_tp_dev;

extern _m_tp_dev tp_dev;	 	//������������touch.c���涨��



/*
********************************************************************************
*                                EXPORTED MACRO
********************************************************************************
*/

//������оƬ��������
#define PEN         HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_7) //T_PEN
#define DOUT        HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_3) //T_MISO
#define TDIN(n)     (n?HAL_GPIO_WritePin(GPIOI,GPIO_PIN_3,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOI,GPIO_PIN_3,GPIO_PIN_RESET))//T_MOSI
#define TCLK(n)     (n?HAL_GPIO_WritePin(GPIOH,GPIO_PIN_6,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOH,GPIO_PIN_6,GPIO_PIN_RESET))//T_SCK
#define TCS(n)      (n?HAL_GPIO_WritePin(GPIOI,GPIO_PIN_8,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOI,GPIO_PIN_8,GPIO_PIN_RESET))//T_CS  

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

//����������
void TP_Write_Byte(uint8_t num);									//�����оƬд��һ������
uint16_t TP_Read_AD(uint8_t CMD);									//��ȡADת��ֵ
uint16_t TP_Read_XOY(uint8_t xy);									//���˲��������ȡ(X/Y)
uint8_t TP_Read_XY(uint16_t *x,uint16_t *y);						//˫�����ȡ(X+Y)
uint8_t TP_Read_XY2(uint16_t *x,uint16_t *y);						//����ǿ�˲���˫���������ȡ
void TP_Drow_Touch_Point(uint16_t x,uint16_t y,uint16_t color);		//��һ������У׼��
void TP_Draw_Big_Point(uint16_t x,uint16_t y,uint16_t color);		//��һ�����
void TP_Save_Adjdata(void);											//����У׼����
uint8_t TP_Get_Adjdata(void);										//��ȡУ׼����
void TP_Adjust(void);												//������У׼
void TP_Adj_Info_Show(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t x3,uint16_t y3,uint16_t fac);//��ʾУ׼��Ϣ
//������/������ ���ú���
uint8_t TP_Scan(uint8_t tp);										//ɨ��
uint8_t TP_Init(void);												//��ʼ��
void BSP_TouchInit(void);
#endif /* __BSP_XXX_H */

/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/


