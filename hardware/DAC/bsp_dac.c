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
 * @file      dac.c
 * @author    GuanXianDe
 * @version   V1.0.0
 * @date      12-June-2018
 * @brief     program entry
 * @attention none.
 *******************************************************************************
 */

/*
********************************************************************************
*                              INCLUDE FILES
********************************************************************************
*/
#include "bsp_dac.h"
/*
********************************************************************************
*                             MACRO DEFINITIONS
********************************************************************************
*/
#if((USE_Dac_File == 1)&&(USE_Dac1_Conf == 1))
DAC_HandleTypeDef DAC1_Handler;//DAC句柄
#endif
/*
********************************************************************************
*                          		OS-RELATED    VARIABLES
********************************************************************************
*/

/*
********************************************************************************
*                             	FUNCTION PROTOTYPES
********************************************************************************
*/
#if((USE_Dac_File == 1)&&(USE_Dac1_Conf == 1))
static void BSP_Dac1Init(void);
#endif
/*
********************************************************************************
*                              	LOCAL VARIABLES
********************************************************************************
*/

/*
********************************************************************************
*                               GLOBAL VARIABLES
********************************************************************************
*/



/*
********************************************************************************
*                              	FUNCTION DEFINITION
********************************************************************************
*/
/**
 * @brief :	Dac初始化
 * @param :	none.
 * @retval:	none.
 */
void BSP_DacInit(void)
{
#if(USE_Dac_Conf == 1)

#if(USE_Dac1_Conf == 1)
	BSP_Dac1Init();
#endif	
	
#endif
}

#if(USE_Dac_File == 1)
/**
 * @brief :	Dac初始化
 * @param :	none.
 * @retval:	none.
 */
#if(USE_Dac1_Conf == 1)
static void BSP_Dac1Init(void)
{
    DAC_ChannelConfTypeDef DACCH_Config;

    DAC1_Handler.Instance=DAC;
    HAL_DAC_Init(&DAC1_Handler);                 											//初始化DAC

    DACCH_Config.DAC_Trigger=DAC_TRIGGER_NONE;             									//不使用触发功能
    DACCH_Config.DAC_OutputBuffer=DAC_OUTPUTBUFFER_DISABLE;									//DAC1输出缓冲关闭
    HAL_DAC_ConfigChannel(&DAC1_Handler,&DACCH_Config,DAC_CHANNEL_1|DAC_CHANNEL_2);			//DAC通道1配置


    HAL_DAC_Start(&DAC1_Handler,DAC_CHANNEL_1);  											//开启DAC通道1
//		HAL_DAC_Start(&DAC1_Handler,DAC_CHANNEL_2);  										//开启DAC通道1
}
#endif
/**
 * @brief :	DAC底层驱动，时钟配置，引脚 配置
 * @param :	hdac:DAC句柄.
 * @retval:	none.
 */
void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_DAC_CLK_ENABLE();             												//使能DAC时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();															//开启GPIOA时钟

    GPIO_Initure.Pin=GPIO_PIN_4;            												//PA4
    GPIO_Initure.Mode=GPIO_MODE_ANALOG;     												//模拟
    GPIO_Initure.Pull=GPIO_NOPULL;          												//不带上下拉
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);

//	  GPIO_Initure.Pin=GPIO_PIN_5;            												//PA5
//    GPIO_Initure.Mode=GPIO_MODE_ANALOG;     												//模拟
//    GPIO_Initure.Pull=GPIO_NOPULL;          												//不带上下拉
//    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
}

/**
 * @brief :	设置DAC的值
 * @param :	Channel：通道；vol：设置值（0-4096）
 * @retval:	none.
 */
#if(USE_Dac1_Conf == 1)
void BSP_DacSetVol(uint32_t Channel,uint16_t vol)
{
    HAL_DAC_SetValue(&DAC1_Handler,Channel,DAC_ALIGN_12B_R,vol);							//12位右对齐数据格式设置DAC值
}
#endif
#endif
/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/
