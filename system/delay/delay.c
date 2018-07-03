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
 * @file      delay.c
 * @author    FanJun
 * @version   V1.0.0
 * @date      11-June-2018
 * @brief     program entry
 * @attention none.
 *******************************************************************************
 */

/*
********************************************************************************
*                              INCLUDE FILES
********************************************************************************
*/
#include "delay.h"
#include "sys.h"
#include "FreeRTOS.h"
#include "task.h"

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
static uint32_t fac_us = 0;                 																		//us��ʱ������
static uint16_t fac_ms = 0;                 																		//ms��ʱ������,��os��,����ÿ�����ĵ�ms��
/*
********************************************************************************
*                               GLOBAL VARIABLES
********************************************************************************
*/

extern void xPortSysTickHandler(void);

/*
********************************************************************************
*                              FUNCTION DEFINITION
********************************************************************************
*/



/**
 * @brief :	systick�жϷ�����,ʹ��OSʱ�õ�
 * @param :	none.
 * @retval:	none.
 */

void SysTick_Handler(void)
{
    if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) { 												//ϵͳ�Ѿ�����
        xPortSysTickHandler();
    }

    HAL_IncTick();
}

/**
 * @brief :	��ʼ���ӳٺ���
 * @param :	SYSTICK��ʱ�ӹ̶�ΪAHBʱ�ӵ�1/8, SYSCLK:ϵͳʱ��Ƶ��
 * @retval:	none.
 */

void delay_init(uint8_t SYSCLK)
{
    uint32_t reload;
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);														//SysTickƵ��ΪHCLK
    fac_us = SYSCLK;                        																	//�����Ƿ�ʹ��OS,fac_us����Ҫʹ��
    reload = SYSCLK;                        																	//ÿ���ӵļ������� ��λΪK
    reload *= 1000000 / configTICK_RATE_HZ; 																	//����delay_ostickspersec�趨���ʱ��
																												//reloadΪ24λ�Ĵ���,���ֵ:16777216,��216M��,Լ��77.7ms����
    fac_ms = 1000 / configTICK_RATE_HZ;     																	//����OS������ʱ�����ٵ�λ
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; 																	//����SYSTICK�ж�
    SysTick->LOAD = reload;                 																	//ÿ1/OS_TICKS_PER_SEC���ж�һ��
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; 																	//����SYSTICK
}

/**
 * @brief :	��ʱnus.
 * @param :	nus:Ҫ��ʱ��us��; nus:0~204522252(���ֵ��2^32/fac_us@fac_us=21)
 * @retval:	none.
 */

void delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;            																//LOAD��ֵ
    ticks = nus * fac_us;                   																	//��Ҫ�Ľ�����
    told = SysTick->VAL;                    																	//�ս���ʱ�ļ�����ֵ

    while(1) {
        tnow = SysTick->VAL;

        if(tnow != told) {
            if(tnow < told) {
                tcnt += told - tnow;    																		//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
            } else {
                tcnt += reload - tnow + told;
            }

            told = tnow;

            if(tcnt >= ticks) {
                break;    																						//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
            }
        }
    };
}
/**
 * @brief :	��ʱnms,�������������.
 * @param :	nms:Ҫ��ʱ��ms��,nms:0~65535.
 * @retval:	none.
 */

void delay_ms(uint32_t nms)
{
    if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) { 												//ϵͳ�Ѿ�����
        if(nms >= fac_ms) {                 																	//��ʱ��ʱ�����OS������ʱ������
            vTaskDelay(nms / fac_ms);       																	//FreeRTOS��ʱ
        }

        nms %= fac_ms;                      																	//OS�Ѿ��޷��ṩ��ôС����ʱ��,������ͨ��ʽ��ʱ
    }

    delay_us((uint32_t)(nms * 1000));           																//��ͨ��ʽ��ʱ
}
/**
 * @brief :	��ʱnms,���������������.
 * @param :	nms:Ҫ��ʱ��ms��.
 * @retval:	none.
 */
void delay_xms(uint32_t nms)
{
    uint32_t i;

    for(i = 0; i < nms; i++) {
        delay_us(1000);
    }
}

/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/
