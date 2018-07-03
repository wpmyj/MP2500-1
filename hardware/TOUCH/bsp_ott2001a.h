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
 * @file      bsp_ott2001a.h
 * @author    GuanXianDe
 * @version   V1.0.0
 * @date      26-June-2018
 * @brief     Header for main.c module
 * @attention
 *******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_OTT2001A_H
#define __BSP_OTT2001A_H

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

//IO操作函数
#define OTT_RST(n)  (n?HAL_GPIO_WritePin(GPIOI,GPIO_PIN_8,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOI,GPIO_PIN_8,GPIO_PIN_RESET))//OTT2001A复位引脚
#define OTT_INT      HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_7)  //OTT2001A断引脚		

									//通过OTT_SET_REG指令,可以查询到这个信息
									//注意,这里的X,Y和屏幕的坐标系刚好是反的.
#define OTT_MAX_X 		2700	 	//TP X方向的最大值(竖方向)
#define OTT_MAX_Y 		1500    	//TP Y方向的最大值(横方向)

//缩放因子
#define OTT_SCAL_X		0.2963		//屏幕的 纵坐标/OTT_MAX_X		
#define OTT_SCAL_Y		0.32		//屏幕的 横坐标/OTT_MAX_Y		

//I2C读写命令
#define OTT_CMD_WR 		0XB2     	//写命令
#define OTT_CMD_RD 		0XB3		//读命令

//寄存器地址
#define OTT_GSTID_REG 	0X0000   	//OTT2001A当前检测到的触摸情况
#define OTT_TP1_REG 	0X0100  	//第一个触摸点数据地址
#define OTT_TP2_REG 	0X0500		//第二个触摸点数据地址
#define OTT_TP3_REG 	0X1000		//第三个触摸点数据地址
#define OTT_TP4_REG 	0X1400		//第四个触摸点数据地址
#define OTT_TP5_REG 	0X1800		//第五个触摸点数据地址  
#define OTT_SET_REG   	0X0900   	//分辨率设置寄存器地址
#define OTT_CTRL_REG  	0X0D00   	//传感器控制(开/关)  

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
uint8_t OTT2001A_WR_Reg(uint16_t reg,uint8_t *buf,uint8_t len);		//写寄存器(实际无用)
void OTT2001A_RD_Reg(uint16_t reg,uint8_t *buf,uint8_t len);		//读寄存器
void OTT2001A_SensorControl(uint8_t cmd);							//传感器打开/关闭操作
uint8_t OTT2001A_Init(void); 										//4.3电容触摸屏始化函数
uint8_t OTT2001A_Scan(uint8_t mode);								//电容触摸屏扫描函数


#endif /* __BSP_OTT2001A_H */

/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/


