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
 * @file      delay.c
 * @author    FanJun
 * @version   V1.0.0
 * @date      11-June-2018
 * @brief     program entry
 * @attention none.
 *******************************************************************************
 */

/*
********************************************************************************
*                              INCLUDE FILES
********************************************************************************
*/
#include "delay.h"
#include "sys.h"
#include "FreeRTOS.h"
#include "task.h"

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

/*
********************************************************************************
*                              LOCAL VARIABLES
********************************************************************************
*/
static uint32_t fac_us = 0;                 																		//us延时倍乘数
static uint16_t fac_ms = 0;                 																		//ms延时倍乘数,在os下,代表每个节拍的ms数
/*
********************************************************************************
*                               GLOBAL VARIABLES
********************************************************************************
*/

extern void xPortSysTickHandler(void);

/*
********************************************************************************
*                              FUNCTION DEFINITION
********************************************************************************
*/



/**
 * @brief :	systick中断服务函数,使用OS时用到
 * @param :	none.
 * @retval:	none.
 */

void SysTick_Handler(void)
{
    if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) { 												//系统已经运行
        xPortSysTickHandler();
    }

    HAL_IncTick();
}

/**
 * @brief :	初始化延迟函数
 * @param :	SYSTICK的时钟固定为AHB时钟的1/8, SYSCLK:系统时钟频率
 * @retval:	none.
 */

void delay_init(uint8_t SYSCLK)
{
    uint32_t reload;
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);														//SysTick频率为HCLK
    fac_us = SYSCLK;                        																	//不论是否使用OS,fac_us都需要使用
    reload = SYSCLK;                        																	//每秒钟的计数次数 单位为K
    reload *= 1000000 / configTICK_RATE_HZ; 																	//根据delay_ostickspersec设定溢出时间
																												//reload为24位寄存器,最大值:16777216,在216M下,约合77.7ms左右
    fac_ms = 1000 / configTICK_RATE_HZ;     																	//代表OS可以延时的最少单位
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; 																	//开启SYSTICK中断
    SysTick->LOAD = reload;                 																	//每1/OS_TICKS_PER_SEC秒中断一次
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; 																	//开启SYSTICK
}

/**
 * @brief :	延时nus.
 * @param :	nus:要延时的us数; nus:0~204522252(最大值即2^32/fac_us@fac_us=21)
 * @retval:	none.
 */

void delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;            																//LOAD的值
    ticks = nus * fac_us;                   																	//需要的节拍数
    told = SysTick->VAL;                    																	//刚进入时的计数器值

    while(1) {
        tnow = SysTick->VAL;

        if(tnow != told) {
            if(tnow < told) {
                tcnt += told - tnow;    																		//这里注意一下SYSTICK是一个递减的计数器就可以了.
            } else {
                tcnt += reload - tnow + told;
            }

            told = tnow;

            if(tcnt >= ticks) {
                break;    																						//时间超过/等于要延迟的时间,则退出.
            }
        }
    };
}
/**
 * @brief :	延时nms,会引起任务调度.
 * @param :	nms:要延时的ms数,nms:0~65535.
 * @retval:	none.
 */

void delay_ms(uint32_t nms)
{
    if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) { 												//系统已经运行
        if(nms >= fac_ms) {                 																	//延时的时间大于OS的最少时间周期
            vTaskDelay(nms / fac_ms);       																	//FreeRTOS延时
        }

        nms %= fac_ms;                      																	//OS已经无法提供这么小的延时了,采用普通方式延时
    }

    delay_us((uint32_t)(nms * 1000));           																//普通方式延时
}
/**
 * @brief :	延时nms,不会引起任务调度.
 * @param :	nms:要延时的ms数.
 * @retval:	none.
 */
void delay_xms(uint32_t nms)
{
    uint32_t i;

    for(i = 0; i < nms; i++) {
        delay_us(1000);
    }
}

/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/
