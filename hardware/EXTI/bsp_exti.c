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
 * @file      bsp_exti.c
 * @author    FanJun
 * @version   V1.0.0
 * @date      08-June-2018
 * @brief     program entry
 * @attention none.
 *******************************************************************************
 */

/*
********************************************************************************
*                              INCLUDE FILES
********************************************************************************
*/

#include "bsp_exti.h"
#include "delay.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "FreeRTOS.h"
#include "task.h"

/*
********************************************************************************
*                             MACRO DEFINITIONS
********************************************************************************
*/
#if(USE_Exti_Conf == 1)
/* �ⲿ�ж��ߵ�GPIO */
#define EXTI_PORT           GPIOH
#define EXTI_PIN            GPIO_PIN_9
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
#if(USE_Exti_Conf == 1)
static void EXTI_Init(void);
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
  * @brief  �ⲿ�жϳ�ʼ��.
  * @param  void *pvParameters:
  * @retval HAL status
  */
void BSP_ExitInit(void)
{
#if(USE_Exti_Conf == 1)
		EXTI_Init();
#endif
}
#if(USE_Exti_File	== 1 )
/**
  * @brief  �ⲿ�жϳ�ʼ��.
  * @param  void *pvParameters:
  * @retval HAL status
  */
static void EXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    /* ����GPIOʱ�� */
    __HAL_RCC_GPIOH_CLK_ENABLE();

    /* GPIO��ʼ��-PH9 */
    GPIO_Initure.Pin = EXTI_PIN;
    GPIO_Initure.Mode = GPIO_MODE_IT_FALLING;   							//�½��ش���
    GPIO_Initure.Pull = GPIO_PULLUP;            							//����
    HAL_GPIO_Init(EXTI_PORT, &GPIO_Initure);

    /* �ж����� */
    HAL_NVIC_SetPriority(EXTI3_IRQn, 6, 0);    								//��ռ���ȼ�Ϊ6�������ȼ�Ϊ0
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);
}

//������
//extern TaskHandle_t Task2Task_Handler;

void EXTI3_IRQHandler(void)
{
//    BaseType_t YieldRequired;

//    delay_xms(50);                      									//����

//    if(KEY0 == 0) {
//        YieldRequired = xTaskResumeFromISR(Task2Task_Handler); 			//�ָ�����2

//        if(YieldRequired == pdTRUE) {
//            /*�������xTaskResumeFromISR()����ֵΪpdTRUE����ô˵��Ҫ�ָ������
//            ������������ȼ����ڻ��߸����������е�����(���жϴ�ϵ�����),������
//            �˳��жϵ�ʱ��һ��Ҫ�����������л���*/
//            portYIELD_FROM_ISR(YieldRequired);
//        }
//    }

    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);   								//����жϱ�־λ
}
#else
#if(USE_Exti_Conf == 1)
static void EXTI_Init(void)
{
	//NULL
}
#endif
#endif

/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/
