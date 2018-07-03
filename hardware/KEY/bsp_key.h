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
 * @date      11-June-2018
 * @brief     Header for main.c module
 * @attention
 *******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_KEY_H
#define __BSP_KEY_H

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
#if(USE_Key_File == 1 )
#define KEY0        HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_3)  							//KEY0按键PH3
#define KEY1        HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_2)  							//KEY1按键PH2
#define KEY2        HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13) 							//KEY2按键PC13
#define WK_UP       HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)  							//WKUP按键PA0

#define KEY0_PRES   1
#define KEY1_PRES   2
#define KEY2_PRES   3
#define WKUP_PRES   4
#endif

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
void BSP_KeyInit(void);

#if(USE_Key_File == 1)
uint8_t KEY_Scan(uint8_t mode);
#endif

#endif /* __BSP_KEY_H */

/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/
