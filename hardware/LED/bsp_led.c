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
 * @file      main.c
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
#include "bsp_led.h"

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
#if(USE_Led_Conf == 1)
static void LED_Init(void);
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
 * @brief :	初始化PB0,PB1为输出.并使能这两个口的时钟
 * @param :	none.
 * @retval:	none.
 */
void BSP_LedInit(void)
{
#if(USE_Led_Conf == 1)	
	 LED_Init();
#endif
}


/**
 * @brief :	初始化PB0,PB1为输出.并使能这两个口的时钟
 * @param :	none.
 * @retval:	none.
 */
#if(USE_Led_File == 1)
static void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE();           						//开启GPIOB时钟

    GPIO_Initure.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP; 						//推挽输出
    GPIO_Initure.Pull = GPIO_PULLUP;        						//上拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_Initure);    						//初始化GPIOB.0和GPIOB.1

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); 			//PB1置0
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); 			//PB1置1
}
#else
#if(USE_Led_Conf == 1)
static void LED_Init(void)
{
	//NULL
}
#endif
#endif
/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/
