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
/* 外部中断线的GPIO */
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
  * @brief  外部中断初始化.
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
  * @brief  外部中断初始化.
  * @param  void *pvParameters:
  * @retval HAL status
  */
static void EXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    /* 开启GPIO时钟 */
    __HAL_RCC_GPIOH_CLK_ENABLE();

    /* GPIO初始化-PH9 */
    GPIO_Initure.Pin = EXTI_PIN;
    GPIO_Initure.Mode = GPIO_MODE_IT_FALLING;   							//下降沿触发
    GPIO_Initure.Pull = GPIO_PULLUP;            							//上拉
    HAL_GPIO_Init(EXTI_PORT, &GPIO_Initure);

    /* 中断配置 */
    HAL_NVIC_SetPriority(EXTI3_IRQn, 6, 0);    								//抢占优先级为6，子优先级为0
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);
}

//任务句柄
//extern TaskHandle_t Task2Task_Handler;

void EXTI3_IRQHandler(void)
{
//    BaseType_t YieldRequired;

//    delay_xms(50);                      									//消抖

//    if(KEY0 == 0) {
//        YieldRequired = xTaskResumeFromISR(Task2Task_Handler); 			//恢复任务2

//        if(YieldRequired == pdTRUE) {
//            /*如果函数xTaskResumeFromISR()返回值为pdTRUE，那么说明要恢复的这个
//            任务的任务优先级等于或者高于正在运行的任务(被中断打断的任务),所以在
//            退出中断的时候一定要进行上下文切换！*/
//            portYIELD_FROM_ISR(YieldRequired);
//        }
//    }

    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);   								//清除中断标志位
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
