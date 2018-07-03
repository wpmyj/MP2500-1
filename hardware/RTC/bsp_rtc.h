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
 * @file      bsp_rtc.h
 * @author    GuanXianDe
 * @version   V1.0.0
 * @date      19-June-2018
 * @brief     Header for main.c module
 * @attention
 *******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_RTC_H
#define __BSP_RTC_H

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
void BSP_RtcInit(void);
#if((USE_Rtc_File == 1)&&(USE_Rtc_Conf == 1))
HAL_StatusTypeDef RTC_Set_Time(uint8_t hour,uint8_t min,uint8_t sec,uint8_t ampm);
HAL_StatusTypeDef RTC_Set_Date(uint8_t year,uint8_t month,uint8_t date,uint8_t week);
void RTC_Set_AlarmA(uint8_t week,uint8_t hour,uint8_t min,uint8_t sec);
void RTC_Set_WakeUp(uint32_t wksel,uint16_t cnt);

void Bsp_TestRTC(void);

uint16_t RTC_GetTimeYear(void);
uint8_t RTC_GetTimeMonth(void);
uint8_t RTC_GetTimeDate(void);
uint8_t RTC_GetTimeWeekDay(void);

uint8_t RTC_GetTimeHours(void);
uint8_t RTC_GetTimeMinutes(void);
uint8_t RTC_GetTimeSeconds(void);
#endif
#endif /* __BSP_RTC_H */

/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/
