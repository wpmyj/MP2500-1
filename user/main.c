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
#include "sys.h"
#include "bsp.h"
#include "FreeRTOS.h"
#include "task.h"
#include "string.h"


/*
********************************************************************************
*                             MACRO DEFINITIONS
********************************************************************************
*/
																				//START����
																				/* START�������ȼ� */
#define START_TASK_PRIO     	1

																				/* START�����ջ��С   */
#define START_STK_SIZE     		128*4

																				//TOUCH����
																				/* TOUCH�������ȼ� */
#define TOUCH_TASK_PRIO			2

																				/* TOUCH�����ջ��С   */
#define TOUCH_STK_SIZE			256

																				//EMWINDEMO����
																				/* EMWINDEMO�������ȼ� */
#define EMWINDEMO_TASK_PRIO		4

																				/* EMWINDEMO�����ջ��С   */
#define EMWINDEMO_STK_SIZE		512


/*
********************************************************************************
*                          OS-RELATED    VARIABLES
********************************************************************************
*/
																				/*Start������*/
TaskHandle_t StartTask_Handler;
																				/*Touch������*/
TaskHandle_t TouchTask_Handler;
																				//Emwindemo������
TaskHandle_t EmwindemoTask_Handler;

/*
********************************************************************************
*                             FUNCTION PROTOTYPES
********************************************************************************
*/
																				/* start������ */
void start_task(void *pvParameters);

																				/* touch������ */
void touch_task(void *pvParameters);

																				/* emwindemo������ */
void emwindemo_task(void *pvParameters);

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
 * @brief :This is the standard entry point for C code.You have performed all
 *         necessary initialization in this function.
 * @param :none.
 * @retval:none.
 */

int main(void)
{
																				/* �ر�ȫ���ж� */
////////////////////    __set_PRIMASK(1);

																				/* Ӳ����ʼ�� */
    Bsp_Init();

																				/* ������ʼ���� */
    xTaskCreate((TaskFunction_t)start_task,             						//������
                (const char *)"start_task",             						//��������
                (uint16_t)START_STK_SIZE,               						//�����ջ��С
                (void *)NULL,                           						//���ݸ��������Ĳ���
                (UBaseType_t)START_TASK_PRIO,           						//�������ȼ�
                (TaskHandle_t *)&StartTask_Handler);    						//������
    vTaskStartScheduler();                              						//�����������
}

																				//��ʱ���Ա���
uint32_t Adc1_Gather_Buf[1]= {0},Adc3_Gather_Buf[1]= {0};
extern DAC_HandleTypeDef DAC1_Handler;											//DAC���
uint16_t dacval=0;

extern uint8_t  TIM5CH1_CAPTURE_STA;											//���벶��״̬
extern uint32_t	TIM5CH1_CAPTURE_VAL;											//���벶��ֵ


/**
  * @brief  ��ʼ������.
  * @param  *pvParameters.
  * @retval none.
  */
void start_task(void *pvParameters)
{
    uint16_t i=0;
	
#if(USE_STemwin_File == 1)
    /*STemwin*/
    __HAL_RCC_CRC_CLK_ENABLE();													//ʹ��CRCʱ��
    WM_SetCreateFlags(WM_CF_MEMDEV);
    GUI_Init();  																//STemWin��ʼ��
    WM_MULTIBUF_Enable(1);  													//����STemWin�໺��,RGB�����ܻ��õ�
    /*STemwin*/
#endif
	
    taskENTER_CRITICAL();           											//�����ٽ���

																				//������������
    xTaskCreate((TaskFunction_t )touch_task,
                (const char*    )"touch_task",
                (uint16_t       )TOUCH_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TOUCH_TASK_PRIO,
                (TaskHandle_t*  )&TouchTask_Handler);

																				//����EMWIN Demo����
    xTaskCreate((TaskFunction_t )emwindemo_task,
                (const char*    )"emwindemo_task",
                (uint16_t       )EMWINDEMO_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )EMWINDEMO_TASK_PRIO,
                (TaskHandle_t*  )&EmwindemoTask_Handler);

    taskEXIT_CRITICAL();            											//�˳��ٽ���
//    uint16_t adcx;
//    float temp;
//    long long temp1=0;
//    POINT_COLOR=BLUE;															//��������Ϊ��ɫ
//    LCD_ShowString(30,130,200,16,16,"ADC1_CH5_VAL:");
//    LCD_ShowString(30,150,200,16,16,"ADC1_CH5_VOL:0.000V");					//���ڹ̶�λ����ʾС����
#if(USE_STemwin_File == 1)
	#if((USE_EmwinDemo_Conf == 0))
		GUI_SetBkColor(GUI_BLUE); 												//���ñ�����ɫ
		GUI_SetColor(GUI_YELLOW); 												//������ɫ
		GUI_Clear(); 															//����
		GUI_SetFont(&GUI_Font24_ASCII); 										//��������
		GUI_DispStringAt("Hello Word!",0,0); 									//��ָ��λ����ʾ�ַ���
	#endif
#else
    POINT_COLOR = RED;
    LCD_ShowString(30, 10, 200, 16, 16, "ENECO TEST STM32F7");
#endif
    while(1)
    {

        APP_TestUart();															//���ڲ���
//			Bsp_TestRTC();
        i++;
        if(i>=500)
        {
            i=0;
			#if(USE_Fatfs_File == 1)
			test_fatfa_cvs();													//����fatfs��д��CSV�ļ� ԭ������exfuns.c
			#endif
			LED0_Toggle;
        }
        vTaskDelay(10);

    }
}

/**
  * @brief  ���������������
  * @param  *pvParameters.
  * @retval none.
  */
void touch_task(void *pvParameters)
{
    while(1)
    {
#if(USE_EmwinDemo_Conf == 1)
        GUI_TOUCH_Exec();
#endif
//		LED0_Toggle;
        vTaskDelay(5);															//��ʱ5ms
    }
}

//EMWINDEMO����
void emwindemo_task(void *pvParameters)
{
    while(1)
    {
#if(USE_EmwinDemo_Conf == 1)
        GUIDEMO_Main();
#else
		vTaskDelay(5);															//��ʱ5ms
#endif
    }
}
/**
  * @brief  �û��ж����ȼ���������,��������Ϊ4��������0-15����ռʽ���ȼ���0�������ȼ����������������ȼ�.
  * @param  none.
  * @retval none.
  */

void USER_NVIC_Cfg(void)
{
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
}

/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/


//        {   //adc���Բ���
//            BSP_Adc1NormalConvertStart(Adc1_Gather_Buf, ADC_CHANNEL_5);

//            SCB_DisableDCache();	                              //�ر�DCache��ʹ��DMA��ȡ����Ҫ�ȹر�DCache����ȡ������Ҫ��DCache
//            adcx=(uint16_t)Adc1_Gather_Buf[0];
//            SCB_EnableDCache();																	//����DCache

//            LCD_ShowxNum(134,130,adcx,4,16,0);    //��ʾADCC�������ԭʼֵ
//            temp=(float)adcx*(3.3/4096);          //��ȡ�����Ĵ�С����ʵ�ʵ�ѹֵ������3.1111
//            adcx=temp;                            //��ֵ�������ָ�adcx��������ΪadcxΪu16����
//            LCD_ShowxNum(134,150,adcx,1,16,0);    //��ʾ��ѹֵ���������֣�3.1111�Ļ������������ʾ3
//            temp-=adcx;                           //���Ѿ���ʾ����������ȥ��������С�����֣�����3.1111-3=0.1111
//            temp*=1000;                           //С�����ֳ���1000�����磺0.1111��ת��Ϊ111.1���൱�ڱ�����λС����
//            LCD_ShowxNum(150,150,temp,3,16,0X80); //��ʾС�����֣�ǰ��ת��Ϊ��������ʾ����������ʾ�ľ���111.

//            BSP_Adc3NormalConvertStart(Adc3_Gather_Buf, ADC_CHANNEL_4);

//            SCB_DisableDCache();	                              //�ر�DCache��ʹ��DMA��ȡ����Ҫ�ȹر�DCache����ȡ������Ҫ��DCache
//            LCD_ShowxNum(150,250+16,Adc3_Gather_Buf[0],4,16,0X80);
//            SCB_EnableDCache();																	//����DCache
//        }
//        {   //dac���Բ���
//            dacval+=200;
//            if(dacval>4096) dacval=0;
//            BSP_DacSetVol(DAC_CHANNEL_1,dacval);
//            SCB_DisableDCache();	                              //�ر�DCache��ʹ��DMA��ȡ����Ҫ�ȹر�DCache����ȡ������Ҫ��DCache
//            adcx=HAL_DAC_GetValue(&DAC1_Handler,DAC_CHANNEL_1);//��ȡǰ������DAC��ֵ
//            SCB_EnableDCache();																	//����DCache

//            LCD_ShowxNum(94,300,adcx,4,16,0);     	    //��ʾDAC�Ĵ���ֵ
//            temp=(float)adcx*(3.3/4096);			    //�õ�DAC��ѹֵ
//            adcx=temp;
//            LCD_ShowxNum(94,316,temp,1,16,0);     	    //��ʾ��ѹֵ��������
//            temp-=adcx;
//            temp*=1000;
//            LCD_ShowxNum(110,316,temp,3,16,0X80); 	    //��ʾ��ѹֵ��С������
//        }
//        {   //���벶�񲿷�
//            if(TIM5CH1_CAPTURE_STA&0X80)        //�ɹ�������һ�θߵ�ƽ
//            {
//                temp=TIM5CH1_CAPTURE_STA&0X3F;
//                temp1*=0XFFFFFFFF;		 	    //���ʱ���ܺ�
//                temp1+=TIM5CH1_CAPTURE_VAL;      //�õ��ܵĸߵ�ƽʱ��
//                printf("HIGH:%lld us\r\n",temp1);//��ӡ�ܵĸߵ�ƽʱ��
//                TIM5CH1_CAPTURE_STA=0;          //������һ�β���
//            }
//        }

