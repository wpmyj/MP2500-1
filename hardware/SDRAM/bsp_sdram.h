
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
 * @file      bsp_sdram.h
 * @author    FanJun
 * @version   V1.0.0
 * @date      11-June-2018
 * @brief     Header for bsp_sdram.c module
 * @attention
 *******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SDRAM_H
#define __SDRAM_H

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
#if(USE_Sdram_File == 1)
#define Bank5_SDRAM_ADDR    ((uint32_t)(0XC0000000)) //SDRAM开始地址

//SDRAM配置参数
#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)
#endif

/*
********************************************************************************
*                                   EXTERNS
********************************************************************************
*/
#if(USE_Sdram_File == 1)
extern SDRAM_HandleTypeDef SDRAM_Handler;//SDRAM句柄
#endif
/*
********************************************************************************
*                              FUNCTION PROTOTYPES
********************************************************************************
*/
#if(USE_Sdram_File == 1)
void SDRAM_MPU_Config(void);
uint8_t SDRAM_Send_Cmd(uint8_t bankx, uint8_t cmd, uint8_t refresh, uint16_t regval);
void FMC_SDRAM_WriteBuffer(uint8_t *pBuffer, uint32_t WriteAddr, uint32_t n);
void FMC_SDRAM_ReadBuffer(uint8_t *pBuffer, uint32_t ReadAddr, uint32_t n);
void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram);
#endif
#if(USE_Sdram_Conf == 1)
void SDRAM_Init(void);
#endif
void BSP_SdramInit(void);
#endif /* __SDRAM_H */

/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/

