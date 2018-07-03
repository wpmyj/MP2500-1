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
																				//START任务
																				/* START任务优先级 */
#define START_TASK_PRIO     	1

																				/* START任务堆栈大小   */
#define START_STK_SIZE     		128*4

																				//TOUCH任务
																				/* TOUCH任务优先级 */
#define TOUCH_TASK_PRIO			2

																				/* TOUCH任务堆栈大小   */
#define TOUCH_STK_SIZE			256

																				//EMWINDEMO任务
																				/* EMWINDEMO任务优先级 */
#define EMWINDEMO_TASK_PRIO		4

																				/* EMWINDEMO任务堆栈大小   */
#define EMWINDEMO_STK_SIZE		512


/*
********************************************************************************
*                          OS-RELATED    VARIABLES
********************************************************************************
*/
																				/*Start任务句柄*/
TaskHandle_t StartTask_Handler;
																				/*Touch任务句柄*/
TaskHandle_t TouchTask_Handler;
																				//Emwindemo任务句柄
TaskHandle_t EmwindemoTask_Handler;

/*
********************************************************************************
*                             FUNCTION PROTOTYPES
********************************************************************************
*/
																				/* start任务函数 */
void start_task(void *pvParameters);

																				/* touch任务函数 */
void touch_task(void *pvParameters);

																				/* emwindemo任务函数 */
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
																				/* 关闭全局中断 */
////////////////////    __set_PRIMASK(1);

																				/* 硬件初始化 */
    Bsp_Init();

																				/* 创建开始任务 */
    xTaskCreate((TaskFunction_t)start_task,             						//任务函数
                (const char *)"start_task",             						//任务名称
                (uint16_t)START_STK_SIZE,               						//任务堆栈大小
                (void *)NULL,                           						//传递给任务函数的参数
                (UBaseType_t)START_TASK_PRIO,           						//任务优先级
                (TaskHandle_t *)&StartTask_Handler);    						//任务句柄
    vTaskStartScheduler();                              						//开启任务调度
}

																				//临时测试变量
uint32_t Adc1_Gather_Buf[1]= {0},Adc3_Gather_Buf[1]= {0};
extern DAC_HandleTypeDef DAC1_Handler;											//DAC句柄
uint16_t dacval=0;

extern uint8_t  TIM5CH1_CAPTURE_STA;											//输入捕获状态
extern uint32_t	TIM5CH1_CAPTURE_VAL;											//输入捕获值


/**
  * @brief  开始任务函数.
  * @param  *pvParameters.
  * @retval none.
  */
void start_task(void *pvParameters)
{
    uint16_t i=0;
	
#if(USE_STemwin_File == 1)
    /*STemwin*/
    __HAL_RCC_CRC_CLK_ENABLE();													//使能CRC时钟
    WM_SetCreateFlags(WM_CF_MEMDEV);
    GUI_Init();  																//STemWin初始化
    WM_MULTIBUF_Enable(1);  													//开启STemWin多缓冲,RGB屏可能会用到
    /*STemwin*/
#endif
	
    taskENTER_CRITICAL();           											//进入临界区

																				//创建触摸任务
    xTaskCreate((TaskFunction_t )touch_task,
                (const char*    )"touch_task",
                (uint16_t       )TOUCH_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TOUCH_TASK_PRIO,
                (TaskHandle_t*  )&TouchTask_Handler);

																				//创建EMWIN Demo任务
    xTaskCreate((TaskFunction_t )emwindemo_task,
                (const char*    )"emwindemo_task",
                (uint16_t       )EMWINDEMO_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )EMWINDEMO_TASK_PRIO,
                (TaskHandle_t*  )&EmwindemoTask_Handler);

    taskEXIT_CRITICAL();            											//退出临界区
//    uint16_t adcx;
//    float temp;
//    long long temp1=0;
//    POINT_COLOR=BLUE;															//设置字体为蓝色
//    LCD_ShowString(30,130,200,16,16,"ADC1_CH5_VAL:");
//    LCD_ShowString(30,150,200,16,16,"ADC1_CH5_VOL:0.000V");					//先在固定位置显示小数点
#if(USE_STemwin_File == 1)
	#if((USE_EmwinDemo_Conf == 0))
		GUI_SetBkColor(GUI_BLUE); 												//设置背景颜色
		GUI_SetColor(GUI_YELLOW); 												//设置颜色
		GUI_Clear(); 															//清屏
		GUI_SetFont(&GUI_Font24_ASCII); 										//设置字体
		GUI_DispStringAt("Hello Word!",0,0); 									//在指定位置显示字符串
	#endif
#else
    POINT_COLOR = RED;
    LCD_ShowString(30, 10, 200, 16, 16, "ENECO TEST STM32F7");
#endif
    while(1)
    {

        APP_TestUart();															//串口测试
//			Bsp_TestRTC();
        i++;
        if(i>=500)
        {
            i=0;
			#if(USE_Fatfs_File == 1)
			test_fatfa_cvs();													//测试fatfs中写读CSV文件 原函数在exfuns.c
			#endif
			LED0_Toggle;
        }
        vTaskDelay(10);

    }
}

/**
  * @brief  触摸任务的任务函数
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
        vTaskDelay(5);															//延时5ms
    }
}

//EMWINDEMO任务
void emwindemo_task(void *pvParameters)
{
    while(1)
    {
#if(USE_EmwinDemo_Conf == 1)
        GUIDEMO_Main();
#else
		vTaskDelay(5);															//延时5ms
#endif
    }
}
/**
  * @brief  用户中断优先级分组配置,分组设置为4，可配置0-15级抢占式优先级，0级子优先级，即不存在子优先级.
  * @param  none.
  * @retval none.
  */

void USER_NVIC_Cfg(void)
{
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
}

/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/


//        {   //adc测试部分
//            BSP_Adc1NormalConvertStart(Adc1_Gather_Buf, ADC_CHANNEL_5);

//            SCB_DisableDCache();	                              //关闭DCache，使用DMA读取数据要先关闭DCache，读取结束后要打开DCache
//            adcx=(uint16_t)Adc1_Gather_Buf[0];
//            SCB_EnableDCache();																	//开启DCache

//            LCD_ShowxNum(134,130,adcx,4,16,0);    //显示ADCC采样后的原始值
//            temp=(float)adcx*(3.3/4096);          //获取计算后的带小数的实际电压值，比如3.1111
//            adcx=temp;                            //赋值整数部分给adcx变量，因为adcx为u16整形
//            LCD_ShowxNum(134,150,adcx,1,16,0);    //显示电压值的整数部分，3.1111的话，这里就是显示3
//            temp-=adcx;                           //把已经显示的整数部分去掉，留下小数部分，比如3.1111-3=0.1111
//            temp*=1000;                           //小数部分乘以1000，例如：0.1111就转换为111.1，相当于保留三位小数。
//            LCD_ShowxNum(150,150,temp,3,16,0X80); //显示小数部分（前面转换为了整形显示），这里显示的就是111.

//            BSP_Adc3NormalConvertStart(Adc3_Gather_Buf, ADC_CHANNEL_4);

//            SCB_DisableDCache();	                              //关闭DCache，使用DMA读取数据要先关闭DCache，读取结束后要打开DCache
//            LCD_ShowxNum(150,250+16,Adc3_Gather_Buf[0],4,16,0X80);
//            SCB_EnableDCache();																	//开启DCache
//        }
//        {   //dac测试部分
//            dacval+=200;
//            if(dacval>4096) dacval=0;
//            BSP_DacSetVol(DAC_CHANNEL_1,dacval);
//            SCB_DisableDCache();	                              //关闭DCache，使用DMA读取数据要先关闭DCache，读取结束后要打开DCache
//            adcx=HAL_DAC_GetValue(&DAC1_Handler,DAC_CHANNEL_1);//读取前面设置DAC的值
//            SCB_EnableDCache();																	//开启DCache

//            LCD_ShowxNum(94,300,adcx,4,16,0);     	    //显示DAC寄存器值
//            temp=(float)adcx*(3.3/4096);			    //得到DAC电压值
//            adcx=temp;
//            LCD_ShowxNum(94,316,temp,1,16,0);     	    //显示电压值整数部分
//            temp-=adcx;
//            temp*=1000;
//            LCD_ShowxNum(110,316,temp,3,16,0X80); 	    //显示电压值的小数部分
//        }
//        {   //输入捕获部分
//            if(TIM5CH1_CAPTURE_STA&0X80)        //成功捕获到了一次高电平
//            {
//                temp=TIM5CH1_CAPTURE_STA&0X3F;
//                temp1*=0XFFFFFFFF;		 	    //溢出时间总和
//                temp1+=TIM5CH1_CAPTURE_VAL;      //得到总的高电平时间
//                printf("HIGH:%lld us\r\n",temp1);//打印总的高点平时间
//                TIM5CH1_CAPTURE_STA=0;          //开启下一次捕获
//            }
//        }

