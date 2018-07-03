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
DAC_HandleTypeDef DAC1_Handler;//DAC���
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
 * @brief :	Dac��ʼ��
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
 * @brief :	Dac��ʼ��
 * @param :	none.
 * @retval:	none.
 */
#if(USE_Dac1_Conf == 1)
static void BSP_Dac1Init(void)
{
    DAC_ChannelConfTypeDef DACCH_Config;

    DAC1_Handler.Instance=DAC;
    HAL_DAC_Init(&DAC1_Handler);                 											//��ʼ��DAC

    DACCH_Config.DAC_Trigger=DAC_TRIGGER_NONE;             									//��ʹ�ô�������
    DACCH_Config.DAC_OutputBuffer=DAC_OUTPUTBUFFER_DISABLE;									//DAC1�������ر�
    HAL_DAC_ConfigChannel(&DAC1_Handler,&DACCH_Config,DAC_CHANNEL_1|DAC_CHANNEL_2);			//DACͨ��1����


    HAL_DAC_Start(&DAC1_Handler,DAC_CHANNEL_1);  											//����DACͨ��1
//		HAL_DAC_Start(&DAC1_Handler,DAC_CHANNEL_2);  										//����DACͨ��1
}
#endif
/**
 * @brief :	DAC�ײ�������ʱ�����ã����� ����
 * @param :	hdac:DAC���.
 * @retval:	none.
 */
void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_DAC_CLK_ENABLE();             												//ʹ��DACʱ��
    __HAL_RCC_GPIOA_CLK_ENABLE();															//����GPIOAʱ��

    GPIO_Initure.Pin=GPIO_PIN_4;            												//PA4
    GPIO_Initure.Mode=GPIO_MODE_ANALOG;     												//ģ��
    GPIO_Initure.Pull=GPIO_NOPULL;          												//����������
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);

//	  GPIO_Initure.Pin=GPIO_PIN_5;            												//PA5
//    GPIO_Initure.Mode=GPIO_MODE_ANALOG;     												//ģ��
//    GPIO_Initure.Pull=GPIO_NOPULL;          												//����������
//    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
}

/**
 * @brief :	����DAC��ֵ
 * @param :	Channel��ͨ����vol������ֵ��0-4096��
 * @retval:	none.
 */
#if(USE_Dac1_Conf == 1)
void BSP_DacSetVol(uint32_t Channel,uint16_t vol)
{
    HAL_DAC_SetValue(&DAC1_Handler,Channel,DAC_ALIGN_12B_R,vol);							//12λ�Ҷ������ݸ�ʽ����DACֵ
}
#endif
#endif
/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/
