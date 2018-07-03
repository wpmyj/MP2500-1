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
 * @file      bsp.h
 * @author    FanJun
 * @version   V1.0.0
 * @date      11-June-2018
 * @brief     Header for bsp.c module
 * @attention
 *******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_H
#define __BSP_H

/*
********************************************************************************
*                                 INCLUDE FILES
********************************************************************************
*/
/* 这是底层驱动模块所有的h文件的汇总文件 */
#include "sys.h"
#include "bsp_lcd.h"
#include "bsp_sdram.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_exti.h"

#include "bsp_adc.h"
#include "bsp_dac.h"
#include "bsp_tim.h"
#include "bsp_iic.h"
#include "bsp_uart.h"
#include "bsp_can.h"
#include "bsp_rtc.h"
#include "bsp_sdmmc_sdcard.h"
#include "mpu.h"
//内存分配
#include "malloc.h"
//文件系统

#include "ff.h"
#include "exfuns.h"

#include "bsp_touch.h"

#if(USE_STemwin_File == 1)
#include "GUI.h"
#include "WM.h"
#if(USE_EmwinDemo_Conf==1)
#include "GUIDEMO.h"
#endif
#endif
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
void Bsp_Init(void);


#endif /* __BSP_H */

/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/
