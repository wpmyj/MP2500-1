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
 * @file      bsp_ft5206.h
 * @author    GuanXianDe
 * @version   V1.0.0
 * @date      26-June-2018
 * @brief     Header for main.c module
 * @attention
 *******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_FT5206_H
#define __BSP_FT5206_H

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
																															//与电容触摸屏连接的芯片引脚(未包含IIC引脚)
																															//IO操作函数
#define FT_RST(n)  (n?HAL_GPIO_WritePin(GPIOI,GPIO_PIN_8,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOI,GPIO_PIN_8,GPIO_PIN_RESET))	//FT5206复位引脚
#define FT_INT      HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_7)  																	//FT5206断引脚	

																															//I2C读写命令
#define FT_CMD_WR 							0X70    																		//写命令
#define FT_CMD_RD 							0X71																			//读命令

																															//FT5206 部分寄存器定义
#define FT_DEVIDE_MODE 						0x00   																			//FT5206模式控制寄存器
#define FT_REG_NUM_FINGER       			0x02																			//触摸状态寄存器

#define FT_TP1_REG 							0X03	  																		//第一个触摸点数据地址
#define FT_TP2_REG 							0X09																			//第二个触摸点数据地址
#define FT_TP3_REG 							0X0F																			//第三个触摸点数据地址
#define FT_TP4_REG 							0X15																			//第四个触摸点数据地址
#define FT_TP5_REG 							0X1B																			//第五个触摸点数据地址  


#define	FT_ID_G_LIB_VERSION					0xA1																			//版本		
#define FT_ID_G_MODE 						0xA4   																			//FT5206中断模式控制寄存器
#define FT_ID_G_THGROUP						0x80   																			//触摸有效值设置寄存器
#define FT_ID_G_PERIODACTIVE				0x88   																			//激活状态周期设置寄存器


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
uint8_t FT5206_WR_Reg(uint16_t reg,uint8_t *buf,uint8_t len);
void FT5206_RD_Reg(uint16_t reg,uint8_t *buf,uint8_t len);
uint8_t FT5206_Init(void);
uint8_t FT5206_Scan(uint8_t mode);


#endif /* __BSP_XXX_H */

/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/


