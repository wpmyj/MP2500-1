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
 * @file      main.c
 * @author    FanJun
 * @version   V1.0.0
 * @date      08-June-2018
 * @brief     program entry
 * @attention none.
 *******************************************************************************
 */

/*
********************************************************************************
*                              INCLUDE FILES
********************************************************************************
*/
#include "bsp_key.h"
#include "delay.h"
/*
********************************************************************************
*                             MACRO DEFINITIONS
********************************************************************************
*/

/*
********************************************************************************
*                          OS-RELATED    VARIABLES
********************************************************************************
*/

/*
********************************************************************************
*                             FUNCTION PROTOTYPES
********************************************************************************
*/
#if(USE_Key_Conf == 1)	
static void KEY_Init(void);
#endif
/*
********************************************************************************
*                              LOCAL VARIABLES
********************************************************************************
*/

/*
********************************************************************************
*                               GLOBAL VARIABLES
********************************************************************************
*/



/*
********************************************************************************
*                              FUNCTION DEFINITION
********************************************************************************
*/

/**
 * @brief :	������ʼ������.
 * @param :	none.
 * @retval:	none.
 */
 void BSP_KeyInit(void)
 {
#if(USE_Key_Conf == 1)	
	 KEY_Init();
#endif
 }
 
#if(USE_Key_File == 1)
/**
 * @brief :������ʼ������.
 * @param :none.
 * @retval:none.
 */

static void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOA_CLK_ENABLE();          					//����GPIOAʱ��
    __HAL_RCC_GPIOC_CLK_ENABLE();           				//����GPIOCʱ��
    __HAL_RCC_GPIOH_CLK_ENABLE();           				//����GPIOHʱ��

    GPIO_Initure.Pin = GPIO_PIN_0;          				//PA0
    GPIO_Initure.Mode = GPIO_MODE_INPUT;    				//����
    GPIO_Initure.Pull = GPIO_PULLDOWN;      				//����
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;   				//����
    HAL_GPIO_Init(GPIOA, &GPIO_Initure);

    GPIO_Initure.Pin = GPIO_PIN_13;         //PC13
    GPIO_Initure.Mode = GPIO_MODE_INPUT;    				//����
    GPIO_Initure.Pull = GPIO_PULLUP;        				//����
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;   				//����
    HAL_GPIO_Init(GPIOC, &GPIO_Initure);

    GPIO_Initure.Pin = GPIO_PIN_2 | GPIO_PIN_3; 			//PH2,3
    HAL_GPIO_Init(GPIOH, &GPIO_Initure);
}

/**
 * @brief :	����������.
 * @param :	mode:0,��֧��������;1,֧��������;.
 * @retval:	���ذ���ֵ.0��û���κΰ�������;1��WKUP���� WK_UP
 * @attention:	ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>WK_UP
 */

uint8_t KEY_Scan(uint8_t mode)
{
    static uint8_t key_up = 1;   							//�����ɿ���־

    if(mode == 1) {
        key_up = 1;    										//֧������
    }

    if(key_up && (KEY0 == 0 || KEY1 == 0 || KEY2 == 0 || WK_UP == 1)) {
        delay_ms(10);
        key_up = 0;

        if(KEY0 == 0) {
            return KEY0_PRES;
        } else if(KEY1 == 0) {
            return KEY1_PRES;
        } else if(KEY2 == 0) {
            return KEY2_PRES;
        } else if(WK_UP == 1) {
            return WKUP_PRES;
        }
    } else if(KEY0 == 1 && KEY1 == 1 && KEY2 == 1 && WK_UP == 0) {
        key_up = 1;
    }

    return 0;   											//�ް�������
}
#else
#if(USE_Key_Conf == 1)	
static void KEY_Init(void)
{
	//NULL
}
#endif
#endif

/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/
