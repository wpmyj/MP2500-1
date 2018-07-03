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
 * @file      bsp_rtc.c
 * @author    GuanXianDe
 * @version   V1.0.0
 * @date      19-June-2018
 * @brief     program entry
 * @attention none.
 *******************************************************************************
 */

/*
********************************************************************************
*                              INCLUDE FILES
********************************************************************************
*/
#include "bsp_rtc.h"
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
#if((USE_Rtc_File == 1)&&(USE_Rtc_Conf == 1))
RTC_HandleTypeDef RTC_Handler;  //RTC���
#endif
/*
********************************************************************************
*                             FUNCTION PROTOTYPES
********************************************************************************
*/
void BSP_RtcInit(void);
#if(USE_Rtc_File == 1)
static uint8_t RTC_Init(void);
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
void BSP_RtcInit(void)
{
#if((USE_Rtc_File == 1)&&(USE_Rtc_Conf == 1))
	RTC_Init();
#endif
}
#if((USE_Rtc_File == 1)&&(USE_Rtc_Conf == 1))
/**
 * @brief :	RTC��ʼ��
 * @param :	none.
 * @retval:
			0,��ʼ���ɹ�;
			2,�����ʼ��ģʽʧ��;
 */
static uint8_t RTC_Init(void)
{
    RTC_Handler.Instance=RTC;
    RTC_Handler.Init.HourFormat=RTC_HOURFORMAT_24;											//RTC����Ϊ24Сʱ��ʽ
    RTC_Handler.Init.AsynchPrediv=0X7F;           											//RTC�첽��Ƶϵ��(1~0X7F)
    RTC_Handler.Init.SynchPrediv=0XFF;            											//RTCͬ����Ƶϵ��(0~7FFF)
    RTC_Handler.Init.OutPut=RTC_OUTPUT_DISABLE;
    RTC_Handler.Init.OutPutPolarity=RTC_OUTPUT_POLARITY_HIGH;
    RTC_Handler.Init.OutPutType=RTC_OUTPUT_TYPE_OPENDRAIN;
    if(HAL_RTC_Init(&RTC_Handler)!=HAL_OK) return 2;

    if(HAL_RTCEx_BKUPRead(&RTC_Handler,RTC_BKP_DR0)!=0X5055)								//�Ƿ��һ������
    {
        RTC_Set_Time(9,12,0,RTC_HOURFORMAT12_PM);	        								//����ʱ�� ,����ʵ��ʱ���޸�
        RTC_Set_Date(18,6,25,1);		                    								//��������
        HAL_RTCEx_BKUPWrite(&RTC_Handler,RTC_BKP_DR0,0X5055);								//����Ѿ���ʼ������
    }
    return 0;
}

/**
 * @brief :	RTC�ײ�������ʱ������(�˺����ᱻHAL_RTC_Init()����)
 * @param :	hrtc:RTC���
 * @retval:	none.
 */
void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    __HAL_RCC_PWR_CLK_ENABLE();																//ʹ�ܵ�Դʱ��PWR
    HAL_PWR_EnableBkUpAccess();																//ȡ����������д����

    RCC_OscInitStruct.OscillatorType=RCC_OSCILLATORTYPE_LSE;								//LSE����
    RCC_OscInitStruct.PLL.PLLState=RCC_PLL_NONE;
    RCC_OscInitStruct.LSEState=RCC_LSE_ON;                  								//RTCʹ��LSE
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    PeriphClkInitStruct.PeriphClockSelection=RCC_PERIPHCLK_RTC;								//����ΪRTC
    PeriphClkInitStruct.RTCClockSelection=RCC_RTCCLKSOURCE_LSE;								//RTCʱ��ԴΪLSE
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

    __HAL_RCC_RTC_ENABLE();																	//RTCʱ��ʹ��
}

/**
 * @brief :	RTCʱ������
 * @param :
			hour,min,sec:Сʱ,����,����
			ampm:@RTC_AM_PM_Definitions:RTC_HOURFORMAT12_AM/RTC_HOURFORMAT12_PM
 * @retval:	SUCEE(1),�ɹ�
			ERROR(0),�����ʼ��ģʽʧ��
 */
HAL_StatusTypeDef RTC_Set_Time(uint8_t hour,uint8_t min,uint8_t sec,uint8_t ampm)
{
    RTC_TimeTypeDef RTC_TimeStructure;

    RTC_TimeStructure.Hours=hour;
    RTC_TimeStructure.Minutes=min;
    RTC_TimeStructure.Seconds=sec;
    RTC_TimeStructure.TimeFormat=ampm;
    RTC_TimeStructure.DayLightSaving=RTC_DAYLIGHTSAVING_NONE;
    RTC_TimeStructure.StoreOperation=RTC_STOREOPERATION_RESET;
    return HAL_RTC_SetTime(&RTC_Handler,&RTC_TimeStructure,RTC_FORMAT_BIN);
}

/**
 * @brief :	RTC��������
 * @param :
			year,month,date:��(0~99),��(1~12),��(0~31)
			week:����(1~7,0,�Ƿ�!)
 * @retval:	SUCEE(1),�ɹ�
			ERROR(0),�����ʼ��ģʽʧ��
 */
HAL_StatusTypeDef RTC_Set_Date(uint8_t year,uint8_t month,uint8_t date,uint8_t week)
{
    RTC_DateTypeDef RTC_DateStructure;

    RTC_DateStructure.Date=date;
    RTC_DateStructure.Month=month;
    RTC_DateStructure.WeekDay=week;
    RTC_DateStructure.Year=year;
    return HAL_RTC_SetDate(&RTC_Handler,&RTC_DateStructure,RTC_FORMAT_BIN);
}


/**
 * @brief :	��������ʱ��(����������,24Сʱ��)
 * @param :
			week:���ڼ�(1~7) @ref  RTC_WeekDay_Definitions
			hour,min,sec:Сʱ,����,����
 * @retval:	none.
 */
void RTC_Set_AlarmA(uint8_t week,uint8_t hour,uint8_t min,uint8_t sec)
{
    RTC_AlarmTypeDef RTC_AlarmSturuct;

    RTC_AlarmSturuct.AlarmTime.Hours=hour;  													//Сʱ
    RTC_AlarmSturuct.AlarmTime.Minutes=min; 													//����
    RTC_AlarmSturuct.AlarmTime.Seconds=sec; 													//��
    RTC_AlarmSturuct.AlarmTime.SubSeconds=0;
    RTC_AlarmSturuct.AlarmTime.TimeFormat=RTC_HOURFORMAT12_AM;

    RTC_AlarmSturuct.AlarmMask=RTC_ALARMMASK_NONE;												//��ȷƥ�����ڣ�ʱ����
    RTC_AlarmSturuct.AlarmSubSecondMask=RTC_ALARMSUBSECONDMASK_NONE;
    RTC_AlarmSturuct.AlarmDateWeekDaySel=RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;						//������
    RTC_AlarmSturuct.AlarmDateWeekDay=week; 													//����
    RTC_AlarmSturuct.Alarm=RTC_ALARM_A;     													//����A
    HAL_RTC_SetAlarm_IT(&RTC_Handler,&RTC_AlarmSturuct,RTC_FORMAT_BIN);

    HAL_NVIC_SetPriority(RTC_Alarm_IRQn,0x01,0x00); 											//��ռ���ȼ�1,�����ȼ�2
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
}

/**
 * @brief :	�����Ի��Ѷ�ʱ������
 * @param :
			wksel:  RTCEx_Wakeup_Timer_Definitions
			#define RTC_WAKEUPCLOCK_RTCCLK_DIV16        ((uint32_t)0x00000000)
			#define RTC_WAKEUPCLOCK_RTCCLK_DIV8         ((uint32_t)0x00000001)
			#define RTC_WAKEUPCLOCK_RTCCLK_DIV4         ((uint32_t)0x00000002)
			#define RTC_WAKEUPCLOCK_RTCCLK_DIV2         ((uint32_t)0x00000003)
			#define RTC_WAKEUPCLOCK_CK_SPRE_16BITS      ((uint32_t)0x00000004)
			#define RTC_WAKEUPCLOCK_CK_SPRE_17BITS      ((uint32_t)0x00000006)

			cnt:�Զ���װ��ֵ.����0,�����ж�.
 * @retval:	none.
 */
void RTC_Set_WakeUp(uint32_t wksel,uint16_t cnt)
{
    __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&RTC_Handler, RTC_FLAG_WUTF);								//���RTC WAKE UP�ı�־

    HAL_RTCEx_SetWakeUpTimer_IT(&RTC_Handler,cnt,wksel);            							//������װ��ֵ��ʱ��

    HAL_NVIC_SetPriority(RTC_WKUP_IRQn,0x02,0x00); 												//��ռ���ȼ�2,�����ȼ�0
    HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
}

/**
 * @brief :	RTC�����жϷ�����
 * @param :	none.
 * @retval:	none.
 */
void RTC_Alarm_IRQHandler(void)
{
    HAL_RTC_AlarmIRQHandler(&RTC_Handler);

}

/**
 * @brief :	RTC����A�жϴ���ص�����
 * @param :	none.
 * @retval:	none.
 */
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
    printf("ALARM A!\r\n");
}

/**
 * @brief :	RTC WAKE UP�жϷ�����
 * @param :	none.
 * @retval:	none.
 */
void RTC_WKUP_IRQHandler(void)
{
    HAL_RTCEx_WakeUpTimerIRQHandler(&RTC_Handler);
}

/**
 * @brief :	RTC WAKE UP�жϴ���
 * @param :	none.
 * @retval:	none.
 */
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
    //
    //
}
/**
 * @brief :	RTC��ȡ��ݣ��Ѽ���2000
 * @param :	none.
 * @retval:	��
 */
uint16_t RTC_GetTimeYear(void)
{
    RTC_DateTypeDef RTC_DateStruct;
    HAL_RTC_GetDate(&RTC_Handler,&RTC_DateStruct,RTC_FORMAT_BIN);
    return RTC_DateStruct.Year+2000;
}
/**
 * @brief :	RTC��ȡ�·�
 * @param :	none.
 * @retval:	��
 */
uint8_t RTC_GetTimeMonth(void)
{
    RTC_DateTypeDef RTC_DateStruct;
    HAL_RTC_GetDate(&RTC_Handler,&RTC_DateStruct,RTC_FORMAT_BIN);
    return RTC_DateStruct.Month;
}
/**
 * @brief :	RTC��ȡ����
 * @param :	none.
 * @retval:	��
 */
uint8_t RTC_GetTimeDate(void)
{
    RTC_DateTypeDef RTC_DateStruct;
    HAL_RTC_GetDate(&RTC_Handler,&RTC_DateStruct,RTC_FORMAT_BIN);
    return RTC_DateStruct.Date;
}
/**
 * @brief :	RTC��ȡ����
 * @param :	none.
 * @retval:	����
 */
uint8_t RTC_GetTimeWeekDay(void)
{
    RTC_DateTypeDef RTC_DateStruct;
    HAL_RTC_GetDate(&RTC_Handler,&RTC_DateStruct,RTC_FORMAT_BIN);
    return RTC_DateStruct.WeekDay;
}
/**
 * @brief :	RTC��ȡСʱ
 * @param :	none.
 * @retval:	Сʱ
 */
uint8_t RTC_GetTimeHours(void)
{
    RTC_TimeTypeDef RTC_TimeStruct;
    HAL_RTC_GetTime(&RTC_Handler,&RTC_TimeStruct,RTC_FORMAT_BIN);
    return RTC_TimeStruct.Hours;
}
/**
 * @brief :	RTC��ȡ����
 * @param :	none.
 * @retval:	����
 */
uint8_t RTC_GetTimeMinutes(void)
{
    RTC_TimeTypeDef RTC_TimeStruct;
    HAL_RTC_GetTime(&RTC_Handler,&RTC_TimeStruct,RTC_FORMAT_BIN);
    return RTC_TimeStruct.Minutes;
}
/**
 * @brief :	RTC��ȡ��
 * @param :	none.
 * @retval:	��
 */
uint8_t RTC_GetTimeSeconds(void)
{
    RTC_TimeTypeDef RTC_TimeStruct;
    HAL_RTC_GetTime(&RTC_Handler,&RTC_TimeStruct,RTC_FORMAT_BIN);
    return RTC_TimeStruct.Seconds;
}

/**
 * @brief :	RTC ����
 * @param :	none.
 * @retval:	none.
 */
#include "bsp_lcd.h"

void Bsp_TestRTC(void)
{
    static uint8_t t=0;
    uint8_t tbuf[40];
    if((t%250)==0)																						//ÿ100ms����һ����ʾ����
    {
        sprintf((char*)tbuf,"Time:%02d:%02d:%02d",RTC_GetTimeHours(),RTC_GetTimeMinutes(),RTC_GetTimeSeconds());
        LCD_ShowString(30,300,210,16,16,tbuf);
        sprintf((char*)tbuf,"Date:%04d-%02d-%02d",RTC_GetTimeYear(),RTC_GetTimeMonth(),RTC_GetTimeDate());
        LCD_ShowString(30,320,210,16,16,tbuf);
        sprintf((char*)tbuf,"Week:%d",RTC_GetTimeWeekDay());
        LCD_ShowString(30,340,210,16,16,tbuf);
    }

}
#endif
/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/
