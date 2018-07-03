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
RTC_HandleTypeDef RTC_Handler;  //RTC句柄
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
 * @brief :	RTC初始化
 * @param :	none.
 * @retval:
			0,初始化成功;
			2,进入初始化模式失败;
 */
static uint8_t RTC_Init(void)
{
    RTC_Handler.Instance=RTC;
    RTC_Handler.Init.HourFormat=RTC_HOURFORMAT_24;											//RTC设置为24小时格式
    RTC_Handler.Init.AsynchPrediv=0X7F;           											//RTC异步分频系数(1~0X7F)
    RTC_Handler.Init.SynchPrediv=0XFF;            											//RTC同步分频系数(0~7FFF)
    RTC_Handler.Init.OutPut=RTC_OUTPUT_DISABLE;
    RTC_Handler.Init.OutPutPolarity=RTC_OUTPUT_POLARITY_HIGH;
    RTC_Handler.Init.OutPutType=RTC_OUTPUT_TYPE_OPENDRAIN;
    if(HAL_RTC_Init(&RTC_Handler)!=HAL_OK) return 2;

    if(HAL_RTCEx_BKUPRead(&RTC_Handler,RTC_BKP_DR0)!=0X5055)								//是否第一次配置
    {
        RTC_Set_Time(9,12,0,RTC_HOURFORMAT12_PM);	        								//设置时间 ,根据实际时间修改
        RTC_Set_Date(18,6,25,1);		                    								//设置日期
        HAL_RTCEx_BKUPWrite(&RTC_Handler,RTC_BKP_DR0,0X5055);								//标记已经初始化过了
    }
    return 0;
}

/**
 * @brief :	RTC底层驱动，时钟配置(此函数会被HAL_RTC_Init()调用)
 * @param :	hrtc:RTC句柄
 * @retval:	none.
 */
void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    __HAL_RCC_PWR_CLK_ENABLE();																//使能电源时钟PWR
    HAL_PWR_EnableBkUpAccess();																//取消备份区域写保护

    RCC_OscInitStruct.OscillatorType=RCC_OSCILLATORTYPE_LSE;								//LSE配置
    RCC_OscInitStruct.PLL.PLLState=RCC_PLL_NONE;
    RCC_OscInitStruct.LSEState=RCC_LSE_ON;                  								//RTC使用LSE
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    PeriphClkInitStruct.PeriphClockSelection=RCC_PERIPHCLK_RTC;								//外设为RTC
    PeriphClkInitStruct.RTCClockSelection=RCC_RTCCLKSOURCE_LSE;								//RTC时钟源为LSE
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

    __HAL_RCC_RTC_ENABLE();																	//RTC时钟使能
}

/**
 * @brief :	RTC时间设置
 * @param :
			hour,min,sec:小时,分钟,秒钟
			ampm:@RTC_AM_PM_Definitions:RTC_HOURFORMAT12_AM/RTC_HOURFORMAT12_PM
 * @retval:	SUCEE(1),成功
			ERROR(0),进入初始化模式失败
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
 * @brief :	RTC日期设置
 * @param :
			year,month,date:年(0~99),月(1~12),日(0~31)
			week:星期(1~7,0,非法!)
 * @retval:	SUCEE(1),成功
			ERROR(0),进入初始化模式失败
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
 * @brief :	设置闹钟时间(按星期闹铃,24小时制)
 * @param :
			week:星期几(1~7) @ref  RTC_WeekDay_Definitions
			hour,min,sec:小时,分钟,秒钟
 * @retval:	none.
 */
void RTC_Set_AlarmA(uint8_t week,uint8_t hour,uint8_t min,uint8_t sec)
{
    RTC_AlarmTypeDef RTC_AlarmSturuct;

    RTC_AlarmSturuct.AlarmTime.Hours=hour;  													//小时
    RTC_AlarmSturuct.AlarmTime.Minutes=min; 													//分钟
    RTC_AlarmSturuct.AlarmTime.Seconds=sec; 													//秒
    RTC_AlarmSturuct.AlarmTime.SubSeconds=0;
    RTC_AlarmSturuct.AlarmTime.TimeFormat=RTC_HOURFORMAT12_AM;

    RTC_AlarmSturuct.AlarmMask=RTC_ALARMMASK_NONE;												//精确匹配星期，时分秒
    RTC_AlarmSturuct.AlarmSubSecondMask=RTC_ALARMSUBSECONDMASK_NONE;
    RTC_AlarmSturuct.AlarmDateWeekDaySel=RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;						//按星期
    RTC_AlarmSturuct.AlarmDateWeekDay=week; 													//星期
    RTC_AlarmSturuct.Alarm=RTC_ALARM_A;     													//闹钟A
    HAL_RTC_SetAlarm_IT(&RTC_Handler,&RTC_AlarmSturuct,RTC_FORMAT_BIN);

    HAL_NVIC_SetPriority(RTC_Alarm_IRQn,0x01,0x00); 											//抢占优先级1,子优先级2
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
}

/**
 * @brief :	周期性唤醒定时器设置
 * @param :
			wksel:  RTCEx_Wakeup_Timer_Definitions
			#define RTC_WAKEUPCLOCK_RTCCLK_DIV16        ((uint32_t)0x00000000)
			#define RTC_WAKEUPCLOCK_RTCCLK_DIV8         ((uint32_t)0x00000001)
			#define RTC_WAKEUPCLOCK_RTCCLK_DIV4         ((uint32_t)0x00000002)
			#define RTC_WAKEUPCLOCK_RTCCLK_DIV2         ((uint32_t)0x00000003)
			#define RTC_WAKEUPCLOCK_CK_SPRE_16BITS      ((uint32_t)0x00000004)
			#define RTC_WAKEUPCLOCK_CK_SPRE_17BITS      ((uint32_t)0x00000006)

			cnt:自动重装载值.减到0,产生中断.
 * @retval:	none.
 */
void RTC_Set_WakeUp(uint32_t wksel,uint16_t cnt)
{
    __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&RTC_Handler, RTC_FLAG_WUTF);								//清除RTC WAKE UP的标志

    HAL_RTCEx_SetWakeUpTimer_IT(&RTC_Handler,cnt,wksel);            							//设置重装载值和时钟

    HAL_NVIC_SetPriority(RTC_WKUP_IRQn,0x02,0x00); 												//抢占优先级2,子优先级0
    HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
}

/**
 * @brief :	RTC闹钟中断服务函数
 * @param :	none.
 * @retval:	none.
 */
void RTC_Alarm_IRQHandler(void)
{
    HAL_RTC_AlarmIRQHandler(&RTC_Handler);

}

/**
 * @brief :	RTC闹钟A中断处理回调函数
 * @param :	none.
 * @retval:	none.
 */
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
    printf("ALARM A!\r\n");
}

/**
 * @brief :	RTC WAKE UP中断服务函数
 * @param :	none.
 * @retval:	none.
 */
void RTC_WKUP_IRQHandler(void)
{
    HAL_RTCEx_WakeUpTimerIRQHandler(&RTC_Handler);
}

/**
 * @brief :	RTC WAKE UP中断处理
 * @param :	none.
 * @retval:	none.
 */
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
    //
    //
}
/**
 * @brief :	RTC获取年份，已加上2000
 * @param :	none.
 * @retval:	年
 */
uint16_t RTC_GetTimeYear(void)
{
    RTC_DateTypeDef RTC_DateStruct;
    HAL_RTC_GetDate(&RTC_Handler,&RTC_DateStruct,RTC_FORMAT_BIN);
    return RTC_DateStruct.Year+2000;
}
/**
 * @brief :	RTC获取月份
 * @param :	none.
 * @retval:	月
 */
uint8_t RTC_GetTimeMonth(void)
{
    RTC_DateTypeDef RTC_DateStruct;
    HAL_RTC_GetDate(&RTC_Handler,&RTC_DateStruct,RTC_FORMAT_BIN);
    return RTC_DateStruct.Month;
}
/**
 * @brief :	RTC获取日期
 * @param :	none.
 * @retval:	日
 */
uint8_t RTC_GetTimeDate(void)
{
    RTC_DateTypeDef RTC_DateStruct;
    HAL_RTC_GetDate(&RTC_Handler,&RTC_DateStruct,RTC_FORMAT_BIN);
    return RTC_DateStruct.Date;
}
/**
 * @brief :	RTC获取星期
 * @param :	none.
 * @retval:	星期
 */
uint8_t RTC_GetTimeWeekDay(void)
{
    RTC_DateTypeDef RTC_DateStruct;
    HAL_RTC_GetDate(&RTC_Handler,&RTC_DateStruct,RTC_FORMAT_BIN);
    return RTC_DateStruct.WeekDay;
}
/**
 * @brief :	RTC获取小时
 * @param :	none.
 * @retval:	小时
 */
uint8_t RTC_GetTimeHours(void)
{
    RTC_TimeTypeDef RTC_TimeStruct;
    HAL_RTC_GetTime(&RTC_Handler,&RTC_TimeStruct,RTC_FORMAT_BIN);
    return RTC_TimeStruct.Hours;
}
/**
 * @brief :	RTC获取分钟
 * @param :	none.
 * @retval:	分钟
 */
uint8_t RTC_GetTimeMinutes(void)
{
    RTC_TimeTypeDef RTC_TimeStruct;
    HAL_RTC_GetTime(&RTC_Handler,&RTC_TimeStruct,RTC_FORMAT_BIN);
    return RTC_TimeStruct.Minutes;
}
/**
 * @brief :	RTC获取秒
 * @param :	none.
 * @retval:	秒
 */
uint8_t RTC_GetTimeSeconds(void)
{
    RTC_TimeTypeDef RTC_TimeStruct;
    HAL_RTC_GetTime(&RTC_Handler,&RTC_TimeStruct,RTC_FORMAT_BIN);
    return RTC_TimeStruct.Seconds;
}

/**
 * @brief :	RTC 测试
 * @param :	none.
 * @retval:	none.
 */
#include "bsp_lcd.h"

void Bsp_TestRTC(void)
{
    static uint8_t t=0;
    uint8_t tbuf[40];
    if((t%250)==0)																						//每100ms更新一次显示数据
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
