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
 * @file      bsp_tim.c
 * @author    GuanXianDe
 * @version   V1.0.0
 * @date      13-June-2018
 * @brief     program entry
 * @attention none.
 *******************************************************************************
 */

/*
********************************************************************************
*                              INCLUDE FILES
********************************************************************************
*/
#include "bsp_tim.h"
/*
********************************************************************************
*                             MACRO DEFINITIONS
********************************************************************************
*/
#if((USE_Tim_File == 1)&&(USE_Tim_Conf == 1))
#if(USE_Tim1ChCap == 1)
#define EN_BSP_Tim1ChCap         1   		//ʹ�ܶ�ʱ��1����
#else
#define EN_BSP_Tim1ChCap         0   		//ʧ�ܶ�ʱ��1����
#endif

#if(USE_Tim4ChCap == 1)
#define EN_BSP_Tim4ChCap         1   		//ʹ�ܶ�ʱ��4����
#else
#define EN_BSP_Tim4ChCap         0   		//ʧ�ܶ�ʱ��4����
#endif

#if(USE_Tim5ChCap == 1)
#define EN_BSP_Tim5ChCap         1   		//ʹ�ܶ�ʱ��5����
#else
#define EN_BSP_Tim5ChCap         0   		//ʹ�ܶ�ʱ��5����
#endif


#if(USE_Tim8Pwm == 1)
#define EN_BSP_Tim8Pwm           1   		//ʹ�ܶ�ʱ��8 pwm���
#else
#define EN_BSP_Tim8Pwm           0   		//ʧ�ܶ�ʱ��8 pwm���
#endif



#if(USE_Tim9Pwm == 1)
#define EN_BSP_Tim9Pwm           1   		//ʹ�ܶ�ʱ��8 pwm���
#else
#define EN_BSP_Tim9Pwm           0   		//ʧ�ܶ�ʱ��8 pwm���
#endif


#if(EN_BSP_Tim1ChCap == 1)
TIM_HandleTypeDef TIM1_Handler;				//��ʱ����� 1���벶��
#endif

#if(EN_BSP_Tim4ChCap == 1)
TIM_HandleTypeDef TIM4_Handler;				//��ʱ����� 4���벶��
#endif

#if(USE_Tim5ChCap == 1)
TIM_HandleTypeDef TIM5_Handler;				//��ʱ����� 5���벶��
#endif

#if(EN_BSP_Tim8Pwm == 1)
TIM_HandleTypeDef TIM8_Handler;				//��ʱ����� 8����Ƚ�
TIM_OC_InitTypeDef TIM8_CHHandler;    		//��ʱ��8PWM��� 
#endif

#if(USE_Tim9Pwm == 1)
TIM_HandleTypeDef TIM9_Handler;				//��ʱ����� 9����Ƚ�
TIM_OC_InitTypeDef TIM9_CHHandler;    		//��ʱ��9PWM���
#endif

#endif

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
//���벶�񲿷�

#if((USE_Tim_File == 1)&&(USE_Tim_Conf == 1))
#if(EN_BSP_Tim1ChCap == 1)
static void BSP_Tim1ChCapInit(uint32_t arr,uint16_t psc);
#endif
#if(EN_BSP_Tim4ChCap == 1)
static void BSP_Tim4ChCapInit(uint32_t arr,uint16_t psc);
#endif
#if(EN_BSP_Tim5ChCap == 1)
static void BSP_Tim5ChCapInit(uint32_t arr,uint16_t psc);
#endif
#endif
//pwm�������

#if((USE_Tim_File == 1)&&(USE_Tim_Conf == 1))
#if(EN_BSP_Tim8Pwm == 1)
static void BSP_Tim8PwmInit(uint16_t arr,uint16_t psc);
#endif
#if(EN_BSP_Tim9Pwm == 1)
static void BSP_Tim9PwmInit(uint16_t arr,uint16_t psc);
#endif
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
/*
********************************************************************************
*                             ���벶�񲿷�
********************************************************************************
*/
/**
 * @brief :	���벶���ʼ��
 * @param :	none.
 * @retval:	none.
 */
void BSP_TimChCapInit(void)
{
#if((USE_Tim_File == 1)&&(USE_Tim_Conf == 1))
	
#if(EN_BSP_Tim1ChCap == 1)
    BSP_Tim1ChCapInit(0XFFFFFFFF,108-1);
#endif

#if(EN_BSP_Tim4ChCap == 1)
    BSP_Tim4ChCapInit(0XFFFFFFFF,108-1);
#endif

#if(EN_BSP_Tim5ChCap == 1)
    BSP_Tim5ChCapInit(0XFFFFFFFF,108-1);
#endif
	
#endif
}

/**
 * @brief :	��ʱ��1ͨ��2,3���벶������
 * @param :	arr���Զ���װֵ(TIM2,TIM5��32λ��!!)
			psc��ʱ��Ԥ��Ƶ��
 * @retval:	none.
 */
#if(EN_BSP_Tim1ChCap == 1)
static void BSP_Tim1ChCapInit(uint32_t arr,uint16_t psc)
{
    TIM_IC_InitTypeDef TIM1_CHConfig;

    TIM1_Handler.Instance=TIM1;                          								//ͨ�ö�ʱ��5
    TIM1_Handler.Init.Prescaler=psc;                     								//��Ƶ
    TIM1_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    								//���ϼ�����
    TIM1_Handler.Init.Period=arr;                        								//�Զ�װ��ֵ
    TIM1_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_IC_Init(&TIM1_Handler);

    TIM1_CHConfig.ICPolarity=TIM_ICPOLARITY_RISING;    									//�����ز���
    TIM1_CHConfig.ICSelection=TIM_ICSELECTION_DIRECTTI;									//ӳ�䵽TI1��
    TIM1_CHConfig.ICPrescaler=TIM_ICPSC_DIV1;          									//���������Ƶ������Ƶ
    TIM1_CHConfig.ICFilter=0;                          									//���������˲��������˲�

    HAL_TIM_IC_ConfigChannel(&TIM1_Handler,&TIM1_CHConfig,TIM_CHANNEL_2);				//����TIM1ͨ��2
    HAL_TIM_IC_ConfigChannel(&TIM1_Handler,&TIM1_CHConfig,TIM_CHANNEL_3);				//����TIM1ͨ��3

    HAL_TIM_IC_Start_IT(&TIM1_Handler,TIM_CHANNEL_2);   								//��ʼ����TIM1��ͨ��2
    HAL_TIM_IC_Start_IT(&TIM1_Handler,TIM_CHANNEL_3);   								//��ʼ����TIM1��ͨ��3

    __HAL_TIM_ENABLE_IT(&TIM1_Handler,TIM_IT_UPDATE);   								//ʹ�ܸ����ж�
}
#endif

/**
 * @brief :	��ʱ��4ͨ��1,2,3,4���벶������
 * @param :	arr���Զ���װֵ(TIM2,TIM5��32λ��!!)
			psc��ʱ��Ԥ��Ƶ��
 * @retval:	none.
 */
#if(EN_BSP_Tim4ChCap == 1)
static void BSP_Tim4ChCapInit(uint32_t arr,uint16_t psc)
{
    TIM_IC_InitTypeDef TIM4_CHConfig;

    TIM4_Handler.Instance=TIM4;                          								//ͨ�ö�ʱ��5
    TIM4_Handler.Init.Prescaler=psc;                     								//��Ƶ
    TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    								//���ϼ�����
    TIM4_Handler.Init.Period=arr;                        								//�Զ�װ��ֵ
    TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_IC_Init(&TIM4_Handler);

    TIM4_CHConfig.ICPolarity=TIM_ICPOLARITY_RISING;    									//�����ز���
    TIM4_CHConfig.ICSelection=TIM_ICSELECTION_DIRECTTI;									//ӳ�䵽TI1��
    TIM4_CHConfig.ICPrescaler=TIM_ICPSC_DIV1;          									//���������Ƶ������Ƶ
    TIM4_CHConfig.ICFilter=0;                          									//���������˲��������˲�

    HAL_TIM_IC_ConfigChannel(&TIM4_Handler,&TIM4_CHConfig,TIM_CHANNEL_1);				//����TIM4ͨ��1
    HAL_TIM_IC_ConfigChannel(&TIM4_Handler,&TIM4_CHConfig,TIM_CHANNEL_2);				//����TIM4ͨ��2
    HAL_TIM_IC_ConfigChannel(&TIM4_Handler,&TIM4_CHConfig,TIM_CHANNEL_3);				//����TIM4ͨ��3
    HAL_TIM_IC_ConfigChannel(&TIM4_Handler,&TIM4_CHConfig,TIM_CHANNEL_4);				//����TIM4ͨ��4

    HAL_TIM_IC_Start_IT(&TIM4_Handler,TIM_CHANNEL_1);   								//��ʼ����TIM1��ͨ��1
    HAL_TIM_IC_Start_IT(&TIM4_Handler,TIM_CHANNEL_2);   								//��ʼ����TIM1��ͨ��2
    HAL_TIM_IC_Start_IT(&TIM4_Handler,TIM_CHANNEL_3);   								//��ʼ����TIM1��ͨ��3
    HAL_TIM_IC_Start_IT(&TIM4_Handler,TIM_CHANNEL_4);   								//��ʼ����TIM1��ͨ��4

    __HAL_TIM_ENABLE_IT(&TIM4_Handler,TIM_IT_UPDATE);   								//ʹ�ܸ����ж�
}
#endif

/**
 * @brief :	��ʱ��5ͨ��1,2,3���벶������
 * @param :	arr���Զ���װֵ(TIM2,TIM5��32λ��!!)
			psc��ʱ��Ԥ��Ƶ��
 * @retval:	none.
 */
#if(EN_BSP_Tim5ChCap == 1)
static void BSP_Tim5ChCapInit(uint32_t arr,uint16_t psc)
{
    TIM_IC_InitTypeDef TIM5_CHConfig;

    TIM5_Handler.Instance=TIM5;                          								//ͨ�ö�ʱ��5
    TIM5_Handler.Init.Prescaler=psc;                     								//��Ƶ
    TIM5_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    								//���ϼ�����
    TIM5_Handler.Init.Period=arr;                        								//�Զ�װ��ֵ
    TIM5_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_IC_Init(&TIM5_Handler);

    TIM5_CHConfig.ICPolarity=TIM_ICPOLARITY_RISING;    									//�����ز���
    TIM5_CHConfig.ICSelection=TIM_ICSELECTION_DIRECTTI;									//ӳ�䵽TI1��
    TIM5_CHConfig.ICPrescaler=TIM_ICPSC_DIV1;          									//���������Ƶ������Ƶ
    TIM5_CHConfig.ICFilter=0;                          									//���������˲��������˲�

    HAL_TIM_IC_ConfigChannel(&TIM5_Handler,&TIM5_CHConfig,TIM_CHANNEL_1);				//����TIM4ͨ��1
//    HAL_TIM_IC_ConfigChannel(&TIM5_Handler,&TIM5_CHConfig,TIM_CHANNEL_2);				//����TIM4ͨ��2
//    HAL_TIM_IC_ConfigChannel(&TIM5_Handler,&TIM5_CHConfig,TIM_CHANNEL_3);				//����TIM4ͨ��3

    HAL_TIM_IC_Start_IT(&TIM5_Handler,TIM_CHANNEL_1);   								//��ʼ����TIM1��ͨ��1
//    HAL_TIM_IC_Start_IT(&TIM5_Handler,TIM_CHANNEL_2);   								//��ʼ����TIM1��ͨ��2
//    HAL_TIM_IC_Start_IT(&TIM5_Handler,TIM_CHANNEL_3);   								//��ʼ����TIM1��ͨ��3

    __HAL_TIM_ENABLE_IT(&TIM5_Handler,TIM_IT_UPDATE);   								//ʹ�ܸ����ж�
}
#endif

/**
 * @brief :	��ʱ���ײ�������ʱ��ʹ�ܣ���������,�˺����ᱻHAL_TIM_IC_Init()����
 * @param :	htim:��ʱ�����
 * @retval:	none.
 */
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{

#if(EN_BSP_Tim1ChCap == 1)
    if(htim->Instance == TIM1)
    {
        GPIO_InitTypeDef GPIO_Initure;
        __HAL_RCC_TIM1_CLK_ENABLE();            										//ʹ��TIM1ʱ��
        __HAL_RCC_GPIOA_CLK_ENABLE();													//����GPIOAʱ��

        GPIO_Initure.Pin=GPIO_PIN_9|GPIO_PIN_10;										//PA9,PA10
        GPIO_Initure.Mode=GPIO_MODE_AF_PP;      										//�����������
        GPIO_Initure.Pull=GPIO_PULLDOWN;        										//����
        GPIO_Initure.Speed=GPIO_SPEED_HIGH;     										//����
        GPIO_Initure.Alternate=GPIO_AF1_TIM1;   										//PA9,10����ΪTIM1ͨ��2,3
        HAL_GPIO_Init(GPIOA,&GPIO_Initure);

        HAL_NVIC_SetPriority(TIM1_CC_IRQn ,2,0);    									//�����ж����ȼ�����ռ���ȼ�2�������ȼ�0
        HAL_NVIC_EnableIRQ(TIM1_CC_IRQn );          									//����ITM1�ж�
    }
#endif
#if(EN_BSP_Tim4ChCap == 1)
    if(htim->Instance == TIM4)
    {
        GPIO_InitTypeDef GPIO_Initure;
        __HAL_RCC_TIM4_CLK_ENABLE();            										//ʹ��TIM4ʱ��
        __HAL_RCC_GPIOB_CLK_ENABLE();													//����GPIOBʱ��
        __HAL_RCC_GPIOD_CLK_ENABLE();													//����GPIODʱ��

        GPIO_Initure.Pin=GPIO_PIN_12;
        GPIO_Initure.Mode=GPIO_MODE_AF_PP;      										//�����������
        GPIO_Initure.Pull=GPIO_PULLDOWN;        										//����
        GPIO_Initure.Speed=GPIO_SPEED_HIGH;     										//����
        GPIO_Initure.Alternate=GPIO_AF2_TIM4;   										//12����ΪTIM4ͨ��1
        HAL_GPIO_Init(GPIOD,&GPIO_Initure);

        GPIO_Initure.Pin=GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
        GPIO_Initure.Mode=GPIO_MODE_AF_PP;      										//�����������
        GPIO_Initure.Pull=GPIO_PULLDOWN;        										//����
        GPIO_Initure.Speed=GPIO_SPEED_HIGH;     										//����
        GPIO_Initure.Alternate=GPIO_AF2_TIM4;   										//7,8,9����ΪTIM4ͨ��2,3,4
        HAL_GPIO_Init(GPIOB,&GPIO_Initure);

        HAL_NVIC_SetPriority(TIM4_IRQn ,2,1);    										//�����ж����ȼ�����ռ���ȼ�2�������ȼ�2
        HAL_NVIC_EnableIRQ(TIM4_IRQn );          										//����ITM4�ж�
    }
#endif

#if(EN_BSP_Tim5ChCap == 1)
    if(htim->Instance == TIM5)
    {
        GPIO_InitTypeDef GPIO_Initure;
        __HAL_RCC_TIM5_CLK_ENABLE();            										//ʹ��TIM5ʱ��
        __HAL_RCC_GPIOH_CLK_ENABLE();													//����GPIOHʱ��

        GPIO_Initure.Pin=GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
        GPIO_Initure.Mode=GPIO_MODE_AF_PP;      										//�����������
        GPIO_Initure.Pull=GPIO_PULLDOWN;        										//����
        GPIO_Initure.Speed=GPIO_SPEED_HIGH;     										//����
        GPIO_Initure.Alternate=GPIO_AF2_TIM5;   										//PA9,10����ΪTIM1ͨ��2,3
        HAL_GPIO_Init(GPIOH,&GPIO_Initure);

        HAL_NVIC_SetPriority(TIM5_IRQn ,2,2);    										//�����ж����ȼ�����ռ���ȼ�2�������ȼ�2
        HAL_NVIC_EnableIRQ(TIM5_IRQn );          										//����ITM5�ж�

    }
#endif
}

#if((USE_Tim_File == 1)&&(USE_Tim_Conf == 1))
/**
 * @brief :	��ʱ��1�жϷ�����
 * @param :	htim:��ʱ�����
 * @retval:	none.
 */
#if(EN_BSP_Tim1ChCap == 1)
void TIM1_CC_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM1_Handler);													//��ʱ�����ô�����
}
#endif
/**
 * @brief :	��ʱ��4�жϷ�����
 * @param :	htim:��ʱ�����
 * @retval:	none.
 */
#if(EN_BSP_Tim4ChCap == 1)
void TIM4_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM4_Handler);													//��ʱ�����ô�����
}
#endif
/**
 * @brief :	��ʱ��5�жϷ�����
 * @param :	htim:��ʱ�����
 * @retval:	none.
 */
#if(EN_BSP_Tim5ChCap == 1)
void TIM5_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM5_Handler);													//��ʱ�����ô�����
}
#endif

//����״̬
//[7]:0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
//[6]:0,��û���񵽵͵�ƽ;1,�Ѿ����񵽵͵�ƽ��.
//[5:0]:����͵�ƽ������Ĵ���(����32λ��ʱ����˵,1us��������1,���ʱ��:4294��)
uint8_t  TIM5CH1_CAPTURE_STA=0;	//���벶��״̬
uint32_t	TIM5CH1_CAPTURE_VAL;	//���벶��ֵ(TIM2/TIM5��32λ)
//��ʱ�������жϣ�����������жϴ���ص������� �ú�����HAL_TIM_IRQHandler�лᱻ����
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)								//�����жϣ����������ʱִ��
{
#if(EN_BSP_Tim1ChCap == 1)
    //
    //
    //
#endif

#if(EN_BSP_Tim4ChCap == 1)
    //
    //
    //
#endif

#if(EN_BSP_Tim5ChCap == 1)
    if((TIM5CH1_CAPTURE_STA&0X80)==0)													//��δ�ɹ�����
    {
        if(TIM5CH1_CAPTURE_STA&0X40)													//�Ѿ����񵽸ߵ�ƽ��
        {
            if((TIM5CH1_CAPTURE_STA&0X3F)==0X3F)										//�ߵ�ƽ̫����
            {
                TIM5CH1_CAPTURE_STA|=0X80;												//��ǳɹ�������һ��
                TIM5CH1_CAPTURE_VAL=0XFFFFFFFF;
            } else TIM5CH1_CAPTURE_STA++;
        }
    }
#endif
}


//��ʱ�����벶���жϴ���ص��������ú�����HAL_TIM_IRQHandler�лᱻ����
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)								//�����жϷ���ʱִ��
{
#if(EN_BSP_Tim1ChCap == 1)
    //
    //
    //
#endif

#if(EN_BSP_Tim4ChCap == 1)
    //
    //
    //
#endif

#if(EN_BSP_Tim5ChCap == 1)
    if((TIM5CH1_CAPTURE_STA&0X80)==0)													//��δ�ɹ�����
    {
        if(TIM5CH1_CAPTURE_STA&0X40)													//����һ���½���
        {
            TIM5CH1_CAPTURE_STA|=0X80;													//��ǳɹ�����һ�θߵ�ƽ����
            TIM5CH1_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM5_Handler,TIM_CHANNEL_1);	//��ȡ��ǰ�Ĳ���ֵ.
            TIM_RESET_CAPTUREPOLARITY(&TIM5_Handler,TIM_CHANNEL_1);   					//һ��Ҫ�����ԭ�������ã���
            TIM_SET_CAPTUREPOLARITY(&TIM5_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);	//����TIM5ͨ��1�����ز���
        } else  																		//��δ��ʼ,��һ�β���������
        {
            TIM5CH1_CAPTURE_STA=0;														//���
            TIM5CH1_CAPTURE_VAL=0;
            TIM5CH1_CAPTURE_STA|=0X40;													//��ǲ�����������
            __HAL_TIM_DISABLE(&TIM5_Handler);        									//�رն�ʱ��5
            __HAL_TIM_SET_COUNTER(&TIM5_Handler,0);
            TIM_RESET_CAPTUREPOLARITY(&TIM5_Handler,TIM_CHANNEL_1);   					//һ��Ҫ�����ԭ�������ã���
            TIM_SET_CAPTUREPOLARITY(&TIM5_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);//��ʱ��5ͨ��1����Ϊ�½��ز���
            __HAL_TIM_ENABLE(&TIM5_Handler);											//ʹ�ܶ�ʱ��5
        }
    }
#endif
}
#endif



/*
********************************************************************************
*                             PWM�������
********************************************************************************
*/
/**
 * @brief :	��ʱ��pwm��ʼ��
 * @param :	none.
 * @retval:	none.
 */
void BSP_TimPwmInit(void)
{
#if(USE_Tim_Conf == 1)
	
#if(EN_BSP_Tim8Pwm == 1)
    BSP_Tim8PwmInit(500-1,108-1);
#endif
#if(EN_BSP_Tim9Pwm == 1)
    BSP_Tim9PwmInit(500-1,108-1);
#endif

#endif
}

#if((USE_Tim_File == 1)&&(USE_Tim_Conf == 1))
/**
 * @brief :	��ʱ��8 pwm��ʼ��
 * @param :	arr���Զ���װֵ
			psc��ʱ��Ԥ��Ƶ��
 * @retval:	none.
 */
#if(EN_BSP_Tim8Pwm == 1)
static void BSP_Tim8PwmInit(uint16_t arr,uint16_t psc)
{
    TIM8_Handler.Instance=TIM8;            												//��ʱ��8
    TIM8_Handler.Init.Prescaler=psc;       												//��ʱ����Ƶ
    TIM8_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;									//���ϼ���ģʽ
    TIM8_Handler.Init.Period=arr;          												//�Զ���װ��ֵ
    TIM8_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM8_Handler);       												//��ʼ��PWM

    TIM8_CHHandler.OCMode=TIM_OCMODE_PWM1; 												//ģʽѡ��PWM1
    TIM8_CHHandler.Pulse=arr/2;            												//���ñȽ�ֵ,��ֵ����ȷ��ռ�ձȣ�
																						//Ĭ�ϱȽ�ֵΪ�Զ���װ��ֵ��һ��,��ռ�ձ�Ϊ50%
    TIM8_CHHandler.OCPolarity=TIM_OCPOLARITY_LOW; 										//����Ƚϼ���Ϊ��

    HAL_TIM_PWM_ConfigChannel(&TIM8_Handler,&TIM8_CHHandler,TIM_CHANNEL_1);				//����TIM8ͨ��1
    HAL_TIM_PWM_ConfigChannel(&TIM8_Handler,&TIM8_CHHandler,TIM_CHANNEL_2);				//����TIM8ͨ��2
    HAL_TIM_PWM_ConfigChannel(&TIM8_Handler,&TIM8_CHHandler,TIM_CHANNEL_3);				//����TIM8ͨ��3

    HAL_TIM_PWM_Start(&TIM8_Handler,TIM_CHANNEL_1);										//����PWMͨ��1
    HAL_TIM_PWM_Start(&TIM8_Handler,TIM_CHANNEL_2);										//����PWMͨ��2
    HAL_TIM_PWM_Start(&TIM8_Handler,TIM_CHANNEL_3);										//����PWMͨ��3

}
#endif
/**
 * @brief :	��ʱ��9 pwm��ʼ��
 * @param :	arr���Զ���װֵ
			psc��ʱ��Ԥ��Ƶ��
 * @retval:	none.
 */
#if(EN_BSP_Tim9Pwm == 1)
static void BSP_Tim9PwmInit(uint16_t arr,uint16_t psc)
{
    TIM9_Handler.Instance=TIM9;            												//��ʱ��9
    TIM9_Handler.Init.Prescaler=psc;       												//��ʱ����Ƶ
    TIM9_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;									//���ϼ���ģʽ
    TIM9_Handler.Init.Period=arr;          												//�Զ���װ��ֵ
    TIM9_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM9_Handler);       												//��ʼ��PWM

    TIM9_CHHandler.OCMode=TIM_OCMODE_PWM1; 												//ģʽѡ��PWM1
    TIM9_CHHandler.Pulse=arr/2;            												//���ñȽ�ֵ,��ֵ����ȷ��ռ�ձȣ�
																						//Ĭ�ϱȽ�ֵΪ�Զ���װ��ֵ��һ��,��ռ�ձ�Ϊ50%
    TIM8_CHHandler.OCPolarity=TIM_OCPOLARITY_LOW; 										//����Ƚϼ���Ϊ��

    HAL_TIM_PWM_ConfigChannel(&TIM9_Handler,&TIM9_CHHandler,TIM_CHANNEL_1);				//����TIM9ͨ��1
    HAL_TIM_PWM_ConfigChannel(&TIM9_Handler,&TIM9_CHHandler,TIM_CHANNEL_2);				//����TIM9ͨ��2

    HAL_TIM_PWM_Start(&TIM9_Handler,TIM_CHANNEL_1);										//����PWMͨ��1
    HAL_TIM_PWM_Start(&TIM9_Handler,TIM_CHANNEL_2);										//����PWMͨ��2
}
#endif
/**
 * @brief :	��ʱ���ײ�������ʱ��ʹ�ܣ���������,�˺����ᱻHAL_TIM_PWM_Init()����
 * @param :	htim:��ʱ�����
 * @retval:	none.
 */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{

#if(EN_BSP_Tim8Pwm == 1)
    if(htim->Instance == TIM8)
    {
        GPIO_InitTypeDef GPIO_Initure;
        __HAL_RCC_TIM8_CLK_ENABLE();													//ʹ�ܶ�ʱ��8
        __HAL_RCC_GPIOI_CLK_ENABLE();													//����GPIOBʱ��

        GPIO_Initure.Pin=GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
        GPIO_Initure.Mode=GPIO_MODE_AF_PP;  											//�����������
        GPIO_Initure.Pull=GPIO_PULLUP;          										//����
        GPIO_Initure.Speed=GPIO_SPEED_HIGH;     										//����
        GPIO_Initure.Alternate=GPIO_AF3_TIM8;											//
        HAL_GPIO_Init(GPIOI,&GPIO_Initure);
    }
#endif

#if(EN_BSP_Tim9Pwm == 1)
    if(htim->Instance == TIM9)
    {
        GPIO_InitTypeDef GPIO_Initure;
        __HAL_RCC_TIM9_CLK_ENABLE();													//ʹ�ܶ�ʱ��9
        __HAL_RCC_GPIOE_CLK_ENABLE();													//����GPIOEʱ��

        GPIO_Initure.Pin=GPIO_PIN_5|GPIO_PIN_6;
        GPIO_Initure.Mode=GPIO_MODE_AF_PP;  											//�����������
        GPIO_Initure.Pull=GPIO_PULLUP;          										//����
        GPIO_Initure.Speed=GPIO_SPEED_HIGH;     										//����
        GPIO_Initure.Alternate=GPIO_AF3_TIM9;											//
        HAL_GPIO_Init(GPIOE,&GPIO_Initure);
    }
#endif
}
/**
 * @brief :	����TIM8ͨ����ռ�ձ�
 * @param :	Channel:ͨ����ֵΪ��TIM_CHANNEL_1��TIM_CHANNEL_2��TIM_CHANNEL_3��
			compare:�Ƚ�ֵ
 * @retval:	none.
 */
#if(EN_BSP_Tim8Pwm == 1)
void BSP_SetTim8Compare(uint32_t Channel,uint32_t compare)
{
    switch(Channel)
    {
    case TIM_CHANNEL_1:
        TIM8->CCR1=compare;
        break;
    case TIM_CHANNEL_2:
        TIM8->CCR2=compare;
        break;
    case TIM_CHANNEL_3:
        TIM8->CCR3=compare;
        break;
    default:
        break;
    }
}
#endif
/**
 * @brief :	����TIM9ͨ����ռ�ձ�
 * @param :	Channel:ͨ����ֵΪ��TIM_CHANNEL_1��TIM_CHANNEL_2��
			compare:�Ƚ�ֵ
 * @retval:	none.
 */
#if(EN_BSP_Tim9Pwm == 1)
void BSP_SetTim9Compare(uint32_t Channel,uint32_t compare)
{
    switch(Channel)
    {
    case TIM_CHANNEL_1:
        TIM9->CCR1=compare;
        break;
    case TIM_CHANNEL_2:
        TIM9->CCR2=compare;
        break;
    default:
        break;
    }
}
#endif

#endif
/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/
