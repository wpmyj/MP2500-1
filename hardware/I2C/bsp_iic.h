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
 * @file      bsp_iic.h
 * @author    GuanXianDe
 * @version   V1.0.0
 * @date      14-June-2018
 * @brief     Header for main.c module
 * @attention
 *******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_IIC_H
#define __BSP_IIC_H

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

void BSP_IicInit(void);
#if(USE_I2c_File == 1)
uint8_t BSP_AT24CXX_ReadOneByte(uint16_t ReadAddr);
void BSP_AT24CXX_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite);
void BSP_AT24CXX_WriteLenByte(uint16_t WriteAddr,uint32_t DataToWrite,uint8_t Len);
uint32_t BSP_AT24CXX_ReadLenByte(uint16_t ReadAddr,uint8_t Len);
uint8_t AT24CXX_Check(void);
void BSP_AT24CXX_Read(uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead);
void BSP_AT24CXX_Write(uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite);
#endif
#endif /* __BSP_IIC_H */

/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/
