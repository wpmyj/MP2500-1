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
 * @file      bsp_adc.c
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
#include "bsp_adc.h"
//��ʱͷ�ļ� ����ʹ��
#include "bsp_lcd.h"
#include "bsp_led.h"
//
/*
********************************************************************************
*                             MACRO DEFINITIONS
********************************************************************************
*/
#if(USE_Adc_File == 1)

#if(USE_Adc1_Conf == 1)
ADC_HandleTypeDef ADC1_Handler;//ADC���
DMA_HandleTypeDef ADC1_DMA_Handler;      //DMA���
#endif
#if(USE_Adc3_Conf == 1)
ADC_HandleTypeDef ADC3_Handler;//ADC���
DMA_HandleTypeDef ADC3_DMA_Handler;      //DMA���
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

#if(USE_Adc_File == 1)

#if(USE_Adc1_Conf == 1)	
static void BSP_Adc1Init(void);
#endif

#if(USE_Adc3_Conf == 1)	
static void BSP_Adc3Init(void);
#endif

#if((USE_Adc1_Conf == 1)||(USE_Adc3_Conf == 1))
static void BSP_AdcDmaConfig(DMA_Stream_TypeDef *DMA_Streamx,uint32_t chx);
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


/**
 * @brief :	ADC��ʼ������.
 * @param :	none.
 * @retval:	none.
 */
void BSP_AdcInit(void)
{
#if(USE_Adc_Conf == 1)
	
#if(USE_Adc1_Conf == 1)	
    BSP_Adc1Init();
#endif
	
#if(USE_Adc3_Conf == 1)	
    BSP_Adc3Init();
#endif
	
#endif
}

#if(USE_Adc_File == 1)
/**
 * @brief :	ADC1��ʼ������.
 * @param :	none.
 * @retval:	none.
 */
#if(USE_Adc1_Conf == 1)	
static void BSP_Adc1Init(void)
{
    __HAL_LINKDMA(&ADC1_Handler,DMA_Handle,ADC1_DMA_Handler);    	//��DMA��ADC1��ϵ����

    BSP_AdcDmaConfig(DMA2_Stream0,DMA_CHANNEL_0);					//��ʼ��DMA

    ADC1_Handler.Instance=ADC1;
    ADC1_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4;   	//4��Ƶ��ADCCLK=PCLK2/4=108/4=27MHZ   (F7  ADCƵ�ʲ��ܳ���36M)
    ADC1_Handler.Init.Resolution=ADC_RESOLUTION_12B;             	//12λģʽ
    ADC1_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             	//�Ҷ���
    ADC1_Handler.Init.ScanConvMode=DISABLE;                      	//��ɨ��ģʽ  ����ɨ�衪����ͨ��
    ADC1_Handler.Init.EOCSelection=DISABLE;                      	//�ر�EOC�ж�
    ADC1_Handler.Init.ContinuousConvMode=DISABLE;                	//�ر�����ת��
    ADC1_Handler.Init.NbrOfConversion=1;                         	//1��ת���ڹ��������� Ҳ����ֻת����������1
    ADC1_Handler.Init.DiscontinuousConvMode=DISABLE;             	//��ֹ����������ģʽ
    ADC1_Handler.Init.NbrOfDiscConversion=0;                     	//����������ͨ����Ϊ0
    ADC1_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       	//�������
    ADC1_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;	//ʹ���������
    ADC1_Handler.Init.DMAContinuousRequests=ENABLE;//DISABLE;             	//�ر�DMA����

    HAL_ADC_DeInit(&ADC1_Handler);									//��λADC
    HAL_ADC_Init(&ADC1_Handler);                                 	//��ʼ��ADC

}
#endif
/**
 * @brief :	ADC3��ʼ������.
 * @param :	none.
 * @retval:	none.
 */
 #if(USE_Adc3_Conf == 1)	
static void BSP_Adc3Init(void)
{
    __HAL_LINKDMA(&ADC3_Handler,DMA_Handle,ADC3_DMA_Handler);    	//��DMA��ADC3��ϵ����

    BSP_AdcDmaConfig(DMA2_Stream0,DMA_CHANNEL_2);					//��ʼ��DMA

    ADC3_Handler.Instance=ADC3;
    ADC3_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4;   	//4��Ƶ��ADCCLK=PCLK2/4=108/4=27MHZ   (F7  ADCƵ�ʲ��ܳ���36M)
    ADC3_Handler.Init.Resolution=ADC_RESOLUTION_12B;             	//12λģʽ
    ADC3_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             	//�Ҷ���
    ADC3_Handler.Init.ScanConvMode=DISABLE;                       	//��ɨ��ģʽ  ����ɨ�衪����ͨ��
    ADC3_Handler.Init.EOCSelection=DISABLE;                      	//�ر�EOC�ж�
    ADC3_Handler.Init.ContinuousConvMode=DISABLE;                	//�ر�����ת��
    ADC3_Handler.Init.NbrOfConversion=1;                         	//1��ת���ڹ��������� Ҳ����ֻת����������1
    ADC3_Handler.Init.DiscontinuousConvMode=DISABLE;             	//��ֹ����������ģʽ
    ADC3_Handler.Init.NbrOfDiscConversion=0;                     	//����������ͨ����Ϊ0
    ADC3_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       	//�������
    ADC3_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;	//ʹ���������
    ADC3_Handler.Init.DMAContinuousRequests=ENABLE;//DISABLE;             	//�ر�DMA����

    HAL_ADC_DeInit(&ADC3_Handler);									//��λADC
    HAL_ADC_Init(&ADC3_Handler);                                 	//��ʼ��ADC
}
#endif
/**
 * @brief :	ADC�ײ��ʼ������.
 * @param :	ADC_HandleTypeDef:ADC���.
 * @retval:	none.
 */
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
#if(USE_Adc1_Conf == 1)	
	{
    GPIO_InitTypeDef GPIO_Initure;
    if(hadc->Instance == ADC1)
    {
        __HAL_RCC_ADC1_CLK_ENABLE();            					//ʹ��ADC1ʱ��
        __HAL_RCC_GPIOA_CLK_ENABLE();								//����GPIOAʱ��
//        __HAL_RCC_GPIOB_CLK_ENABLE();								//����GPIOBʱ��
//        __HAL_RCC_GPIOC_CLK_ENABLE();								//����GPIOCʱ��

//        //ADC1_IN0
//        GPIO_Initure.Pin=GPIO_PIN_0;            					//PA0
//        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     					//ģ��
//        GPIO_Initure.Pull=GPIO_NOPULL;          					//����������
//        HAL_GPIO_Init(GPIOA,&GPIO_Initure);

//        //ADC1_IN1
//        GPIO_Initure.Pin=GPIO_PIN_1;            					//PA1
//        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     					//ģ��
//        GPIO_Initure.Pull=GPIO_NOPULL;          					//����������
//        HAL_GPIO_Init(GPIOA,&GPIO_Initure);

//        //ADC1_IN6
//        GPIO_Initure.Pin=GPIO_PIN_6;            					//PA6
//        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     					//ģ��
//        GPIO_Initure.Pull=GPIO_NOPULL;          					//����������
//        HAL_GPIO_Init(GPIOA,&GPIO_Initure);

//        //ADC1_IN7
//        GPIO_Initure.Pin=GPIO_PIN_7;            					//PA7
//        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     					//ģ��
//        GPIO_Initure.Pull=GPIO_NOPULL;          					//����������
//        HAL_GPIO_Init(GPIOA,&GPIO_Initure);

//        //ADC1_IN8
//        GPIO_Initure.Pin=GPIO_PIN_0;            					//PB0
//        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     					//ģ��
//        GPIO_Initure.Pull=GPIO_NOPULL;          					//����������
//        HAL_GPIO_Init(GPIOB,&GPIO_Initure);

//        //ADC1_IN9
//        GPIO_Initure.Pin=GPIO_PIN_1;            					//PB1
//        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     					//ģ��
//        GPIO_Initure.Pull=GPIO_NOPULL;          					//����������
//        HAL_GPIO_Init(GPIOB,&GPIO_Initure);

//        //ADC1_IN10
//        GPIO_Initure.Pin=GPIO_PIN_0;            					//PC0
//        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     					//ģ��
//        GPIO_Initure.Pull=GPIO_NOPULL;          					//����������
//        HAL_GPIO_Init(GPIOC,&GPIO_Initure);
//        //ADC1_IN11
//        GPIO_Initure.Pin=GPIO_PIN_1;            					//PC1
//        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     					//ģ��
//        GPIO_Initure.Pull=GPIO_NOPULL;          					//����������
//        HAL_GPIO_Init(GPIOC,&GPIO_Initure);

//        //ADC1_IN12
//        GPIO_Initure.Pin=GPIO_PIN_2;            					//PC2
//        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     					//ģ��
//        GPIO_Initure.Pull=GPIO_NOPULL;          					//����������
//        HAL_GPIO_Init(GPIOC,&GPIO_Initure);

//        //ADC1_IN13
//        GPIO_Initure.Pin=GPIO_PIN_3;            					//PC3
//        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     					//ģ��
//        GPIO_Initure.Pull=GPIO_NOPULL;          					//����������
//        HAL_GPIO_Init(GPIOC,&GPIO_Initure);

//        //ADC1_IN14
//        GPIO_Initure.Pin=GPIO_PIN_4;            					//PC4
//        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     					//ģ��
//        GPIO_Initure.Pull=GPIO_NOPULL;          					//����������
//        HAL_GPIO_Init(GPIOC,&GPIO_Initure);

//        //ADC1_IN15
//        GPIO_Initure.Pin=GPIO_PIN_5;            					//PC5
//        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     					//ģ��
//        GPIO_Initure.Pull=GPIO_NOPULL;         					//����������
//        HAL_GPIO_Init(GPIOC,&GPIO_Initure);

        //ADC1_IN5
        GPIO_Initure.Pin=GPIO_PIN_5;            					//PA5
        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     					//ģ��
        GPIO_Initure.Pull=GPIO_NOPULL;          					//����������
        HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    }
	}
#endif
#if(USE_Adc3_Conf == 1)	
	{
    GPIO_InitTypeDef GPIO_Initure;
    if(hadc->Instance == ADC3)
    {
        __HAL_RCC_ADC3_CLK_ENABLE();            					//ʹ��ADC1ʱ��
        __HAL_RCC_GPIOF_CLK_ENABLE();								//����GPIOAʱ��

        //ADC3_IN4
        GPIO_Initure.Pin=GPIO_PIN_6;            					//PF6
        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     					//ģ��
        GPIO_Initure.Pull=GPIO_NOPULL;          					//����������
        HAL_GPIO_Init(GPIOF,&GPIO_Initure);

////////////////////        //ADC3_IN5
////////////////////        GPIO_Initure.Pin=GPIO_PIN_7;            //PF7
////////////////////        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //ģ��
////////////////////        GPIO_Initure.Pull=GPIO_NOPULL;          //����������
////////////////////        HAL_GPIO_Init(GPIOF,&GPIO_Initure);

////////////////////        //ADC3_IN6
////////////////////        GPIO_Initure.Pin=GPIO_PIN_8;            //PF8
////////////////////        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //ģ��
////////////////////        GPIO_Initure.Pull=GPIO_NOPULL;          //����������
////////////////////        HAL_GPIO_Init(GPIOF,&GPIO_Initure);

////////////////////        //ADC3_IN7
////////////////////        GPIO_Initure.Pin=GPIO_PIN_9;            //PF9
////////////////////        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //ģ��
////////////////////        GPIO_Initure.Pull=GPIO_NOPULL;          //����������
////////////////////        HAL_GPIO_Init(GPIOF,&GPIO_Initure);

////////////////////        //ADC3_IN8
////////////////////        GPIO_Initure.Pin=GPIO_PIN_10;            //PF10
////////////////////        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //ģ��
////////////////////        GPIO_Initure.Pull=GPIO_NOPULL;          //����������
////////////////////        HAL_GPIO_Init(GPIOF,&GPIO_Initure);

////////////////////        //ADC3_IN9
////////////////////        GPIO_Initure.Pin=GPIO_PIN_3;            //PF3
////////////////////        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //ģ��
////////////////////        GPIO_Initure.Pull=GPIO_NOPULL;          //����������
////////////////////        HAL_GPIO_Init(GPIOF,&GPIO_Initure);

////////////////////        //ADC3_IN14
////////////////////        GPIO_Initure.Pin=GPIO_PIN_4;            //PF4
////////////////////        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //ģ��
////////////////////        GPIO_Initure.Pull=GPIO_NOPULL;          //����������
////////////////////        HAL_GPIO_Init(GPIOF,&GPIO_Initure);

////////////////////        //ADC3_IN15
////////////////////        GPIO_Initure.Pin=GPIO_PIN_5;            //PF5
////////////////////        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //ģ��
////////////////////        GPIO_Initure.Pull=GPIO_NOPULL;          //����������
////////////////////        HAL_GPIO_Init(GPIOF,&GPIO_Initure);
    }
	}
#endif
}
/**
 * @brief :	���ADC1ֵ.
 * @param :	i_pBuffAddr����ȡ������
			ChUsedNmb: ͨ��ֵ 0~16��ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
 * @retval:	none.
 */
#if(USE_Adc1_Conf == 1)	
void BSP_Adc1NormalConvertStart(uint32_t *i_pBuffAddr, uint32_t ChUsedNmb)
{

    ADC_ChannelConfTypeDef ADC1_ChanConf;

//    MYDMA_Config(DMA2_Stream0,DMA_CHANNEL_0);						//��ʼ��DMA ADC1��ADC3ͬʱ����DMA2������0������ͬͨ����ÿ�ζ�Ҫ���³�ʼ������
    //ʹ�ô��У�������������
    ADC1_DMA_Handler.Instance=DMA2_Stream0;                     	//������ѡ��
    ADC1_DMA_Handler.Init.Channel=DMA_CHANNEL_0;                	//ͨ��ѡ��
    HAL_DMA_Init(&ADC1_DMA_Handler);

    ADC1_ChanConf.Channel=ChUsedNmb;                        		//ͨ��
    ADC1_ChanConf.Rank=1;                                       	//1������
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        	//����ʱ��
    ADC1_ChanConf.Offset=0;
    HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);        	//ͨ������

    if(HAL_ADC_Start_DMA(&ADC1_Handler, i_pBuffAddr, 1) != HAL_OK) 	//����DMA��ʽ�ɼ�ADC�ź�
    {
        LCD_ShowString(30,250,200,16,16,"ADC1_CH5_ ERRO!!");
    }
    else
    {
        LCD_ShowString(30,250,200,16,16,"ADC1_CH5_ ok!!");
    }
}
#endif
/**
 * @brief :	���ADC3ֵ.
 * @param :	i_pBuffAddr����ȡ������
			ChUsedNmb: ͨ��ֵ 0~16��ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
 * @retval:	none.
 */
 #if(USE_Adc3_Conf == 1)	
void BSP_Adc3NormalConvertStart(uint32_t *i_pBuffAddr, uint32_t ChUsedNmb)
{
    ADC_ChannelConfTypeDef ADC3_ChanConf;

//    MYDMA_Config(DMA2_Stream0,DMA_CHANNEL_2);						//��ʼ��DMA ADC1��ADC3ͬʱ����DMA2������0������ͬͨ����ÿ�ζ�Ҫ���³�ʼ������
    //ʹ�ô��У�������������
    ADC3_DMA_Handler.Instance=DMA2_Stream0;                     	//������ѡ��
    ADC3_DMA_Handler.Init.Channel=DMA_CHANNEL_2;                	//ͨ��ѡ��
    HAL_DMA_Init(&ADC3_DMA_Handler);

    ADC3_ChanConf.Channel=ChUsedNmb;                        		//ͨ��
    ADC3_ChanConf.Rank=1;                                       	//1������
    ADC3_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        	//����ʱ��
    ADC3_ChanConf.Offset=0;
    HAL_ADC_ConfigChannel(&ADC3_Handler,&ADC3_ChanConf);        	//ͨ������

    if(HAL_ADC_Start_DMA(&ADC3_Handler, i_pBuffAddr, 1) != HAL_OK)
    {

    }
    else
    {
        LCD_ShowString(30,250+16,200,16,16,"ADC3_CH4_ ok!!");     	//��ʱ��ʾ
    }
}
#endif

/**
 * @brief :	ADC ���DMA����.
 * @param :	DMA_Streamx��DMA���
			ChUsedNmb: ͨ��ֵ 0~16��ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
 * @retval:	none.
 */
 #if((USE_Adc1_Conf == 1)||(USE_Adc3_Conf == 1))
static void BSP_AdcDmaConfig(DMA_Stream_TypeDef *DMA_Streamx,uint32_t ChUsedNmb)
{
    if((uint32_t)DMA_Streamx>(uint32_t)DMA2)						//�õ���ǰstream������DMA2����DMA1
    {
        __HAL_RCC_DMA2_CLK_ENABLE();								//DMA2ʱ��ʹ��
    } else
    {
        __HAL_RCC_DMA1_CLK_ENABLE();								//DMA1ʱ��ʹ��
    }

#if(USE_Adc1_Conf == 1)	
    if((DMA_Streamx == DMA2_Stream0)&& (ChUsedNmb == DMA_CHANNEL_0))
    {
        // DMA����
        ADC1_DMA_Handler.Instance=DMA_Streamx;                      //������ѡ��
        ADC1_DMA_Handler.Init.Channel=ChUsedNmb;                    //ͨ��ѡ��
        ADC1_DMA_Handler.Init.Direction=DMA_PERIPH_TO_MEMORY ;      //���赽�洢��
        ADC1_DMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;           //���������ģʽ
        ADC1_DMA_Handler.Init.MemInc=DMA_MINC_DISABLE;//DMA_MINC_ENABLE;      	//�洢������ģʽ
        ADC1_DMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_HALFWORD;    	//�������ݳ���:8λ  16λ
        ADC1_DMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_HALFWORD;       	//�洢�����ݳ���:8λ 16λ
        ADC1_DMA_Handler.Init.Mode=DMA_CIRCULAR;//DMA_NORMAL;                 	//��������ģʽ
        ADC1_DMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;             		//�е����ȼ�
        ADC1_DMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;
        ADC1_DMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;
        ADC1_DMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 		//�洢��ͻ�����δ���
        ADC1_DMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              		//����ͻ�����δ���


        HAL_DMA_DeInit(&ADC1_DMA_Handler);										//��λADC1_DMA_Handler

        HAL_DMA_Init(&ADC1_DMA_Handler);										//��ʼ��ADC1_DMA_Handler

    }
#endif
#if(USE_Adc3_Conf == 1)	
    if((DMA_Streamx == DMA2_Stream0)&& (ChUsedNmb == DMA_CHANNEL_2))
    {
        // DMA����
        ADC3_DMA_Handler.Instance=DMA_Streamx;                            		//������ѡ��
        ADC3_DMA_Handler.Init.Channel=ChUsedNmb;                                		//ͨ��ѡ��
        ADC3_DMA_Handler.Init.Direction=DMA_PERIPH_TO_MEMORY ;             		//���赽�洢��
        ADC3_DMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 		//���������ģʽ
        ADC3_DMA_Handler.Init.MemInc=DMA_MINC_DISABLE;//DMA_MINC_ENABLE;      	//�洢������ģʽ
        ADC3_DMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_HALFWORD;    	//�������ݳ���:8λ  16λ
        ADC3_DMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_HALFWORD;       	//�洢�����ݳ���:8λ 16λ
        ADC3_DMA_Handler.Init.Mode=DMA_CIRCULAR;//DMA_NORMAL;                 	//��������ģʽ
        ADC3_DMA_Handler.Init.Priority=DMA_PRIORITY_HIGH;               		//�е����ȼ�
        ADC3_DMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;					//û�õ�
        ADC3_DMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;
        ADC3_DMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 		//�洢��ͻ�����δ���
        ADC3_DMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              		//����ͻ�����δ���


        HAL_DMA_DeInit(&ADC3_DMA_Handler);										//��λADC3_DMA_Handler

        HAL_DMA_Init(&ADC3_DMA_Handler);										//��ʼ��ADC3_DMA_Handler

    }
#endif
#if((USE_Adc1_Conf == 1)||(USE_Adc3_Conf == 1))	
    HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);										//ʹ��DMA2������0�ж�NVIC
    HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 3, 3);								//����DMA2������0�ж����ȼ�
#endif
}

#endif


/**
 * @brief :	DMA2������0�жϷ�����.
 * @param :	none.
 * @retval:	none.
 */
#if((USE_Adc1_Conf == 1)||(USE_Adc3_Conf == 1))
void DMA2_Stream0_IRQHandler(void)
{
#if(USE_Adc1_Conf == 1)
    if(HAL_DMA_GetState(&ADC1_DMA_Handler))										//��ȡADC1_DMA_Handler״̬
    {
        HAL_DMA_IRQHandler(ADC1_Handler.DMA_Handle);
        //
        //��ӷ����ź�������Ϣ
        //
    }
#endif
#if(USE_Adc3_Conf == 1)
    if(HAL_DMA_GetState(&ADC3_DMA_Handler))               						//��ȡADC3_DMA_Handler״̬
    {
        HAL_DMA_IRQHandler(ADC3_Handler.DMA_Handle);
        //
        //��ӷ����ź�������Ϣ
        //
    }
#endif
}
#endif

#endif

/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/




