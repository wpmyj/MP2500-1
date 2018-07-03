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
 * @file      main.h
 * @author    FanJun
 * @version   V1.0.0
 * @date      22-April-2016
 * @brief     Header for main.c module
 * @attention
 *******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SYS_H
#define __SYS_H

/*
********************************************************************************
*                                 INCLUDE FILES
********************************************************************************
*/

#include "stm32f7xx.h"
#include "core_cm7.h"
#include "stm32f7xx_hal.h"

#include "user_config.h"
#include "bsp_pin.h"

/*
********************************************************************************
*                              EXPORTED DATA TYPES
********************************************************************************
*/

//���õ��������Ͷ̹ؼ��֣��˴������ã�����C99��׼���������ͽ���ʹ�ã��������ض���
//typedef           int32_t         s32;
//typedef           int16_t         s16;
//typedef           int8_t          s8;

//typedef   const   int32_t         sc32;
//typedef   const   int16_t         sc16;
//typedef   const   int8_t          sc8;

//typedef   __IO    int32_t         vs32;
//typedef   __IO    int16_t         vs16;
//typedef   __IO    int8_t          vs8;

//typedef   __I     int32_t         vsc32;
//typedef   __I     int16_t         vsc16;
//typedef   __I     int8_t          vsc8;

//typedef   const   uint32_t        uc32;
//typedef   const   uint16_t        uc16;
//typedef   const   uint8_t         uc8;

//typedef   __IO    uint32_t        vu32;
//typedef   __IO    uint16_t        vu16;
//typedef   __IO    uint8_t         vu8;

//typedef   __I     uint32_t        vuc32;
//typedef   __I     uint16_t        vuc16;
//typedef   __I     uint8_t         vuc8;


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
#define ON  1
#define OFF 0

#define Write_Through() (*(__IO uint32_t*)0XE000EF9C=1UL<<2) 																//Cache͸дģʽ ��SCB->CACR|=1<<2;����һ��



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

void Cache_Enable(void);

void Stm32_Clock_Init(uint32_t plln, uint32_t pllm, uint32_t pllp, uint32_t pllq); 											//����ϵͳʱ��

																															/* �ж�Cache�Ƿ�� */
uint8_t Get_ICahceSta(void);
uint8_t Get_DCahceSta(void);

																															//����Ϊ��ຯ��
void WFI_SET(void);     																									//ִ��WFIָ��
void INTX_DISABLE(void);																									//�ر������ж�
void INTX_ENABLE(void); 																									//���������ж�
void MSR_MSP(uint32_t addr);   																								//���ö�ջ��ַ



#endif /* __SYS_H */

/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/
