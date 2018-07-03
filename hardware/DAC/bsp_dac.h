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
 * @file      DAC.h
 * @author    GuanXianDe
 * @version   V1.0.0
 * @date      12-June-2018
 * @brief     Header for main.c module
 * @attention
 *******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_DAC_H
#define __BSP_DAC_H

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

void BSP_DacInit(void);
#if(USE_Dac_File == 1)

#if(USE_Dac1_Conf == 1)
void BSP_DacSetVol(uint32_t Channel,uint16_t vol);
#endif

#endif


#endif /* __BSP_DAC_H */

/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/
