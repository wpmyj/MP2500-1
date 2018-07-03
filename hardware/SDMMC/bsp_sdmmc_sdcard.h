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
 * @file      BSP_SDMMC_SDCARD.h
 * @author    GuanXianDe
 * @version   V1.0.0
 * @date      20-June-2018
 * @brief     Header for main.c module
 * @attention
 *******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_SDMMC_SDCARD_H
#define __BSP_SDMMC_SDCARD_H

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
#if((USE_Sdcard_File == 1)&&(USE_SdCard_Conf == 1))
extern SD_HandleTypeDef        SDCARD_Handler;      //SD卡句柄
extern HAL_SD_CardInfoTypedef  SDCardInfo;          //SD卡信息结构体
#endif

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
void BSP_SdcardInit(void);
#if((USE_Sdcard_File == 1)&&(USE_SdCard_Conf == 1))
uint8_t SD_Init(void);
uint8_t SD_ReadDisk(uint8_t* buf,uint32_t sector,uint8_t cnt);
uint8_t SD_WriteDisk(uint8_t *buf,uint32_t sector,uint8_t cnt);
#endif
#endif /* __BSP_SDMMC_SDCARD_H */

/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/
