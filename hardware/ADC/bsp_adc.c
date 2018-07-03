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
//临时头文件 测试使用
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
ADC_HandleTypeDef ADC1_Handler;//ADC句柄
DMA_HandleTypeDef ADC1_DMA_Handler;      //DMA句柄
#endif
#if(USE_Adc3_Conf == 1)
ADC_HandleTypeDef ADC3_Handler;//ADC句柄
DMA_HandleTypeDef ADC3_DMA_Handler;      //DMA句柄
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
 * @brief :	ADC初始化函数.
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
 * @brief :	ADC1初始化函数.
 * @param :	none.
 * @retval:	none.
 */
#if(USE_Adc1_Conf == 1)	
static void BSP_Adc1Init(void)
{
    __HAL_LINKDMA(&ADC1_Handler,DMA_Handle,ADC1_DMA_Handler);    	//将DMA与ADC1联系起来

    BSP_AdcDmaConfig(DMA2_Stream0,DMA_CHANNEL_0);					//初始化DMA

    ADC1_Handler.Instance=ADC1;
    ADC1_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4;   	//4分频，ADCCLK=PCLK2/4=108/4=27MHZ   (F7  ADC频率不能超过36M)
    ADC1_Handler.Init.Resolution=ADC_RESOLUTION_12B;             	//12位模式
    ADC1_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             	//右对齐
    ADC1_Handler.Init.ScanConvMode=DISABLE;                      	//非扫描模式  允许扫描——多通道
    ADC1_Handler.Init.EOCSelection=DISABLE;                      	//关闭EOC中断
    ADC1_Handler.Init.ContinuousConvMode=DISABLE;                	//关闭连续转换
    ADC1_Handler.Init.NbrOfConversion=1;                         	//1个转换在规则序列中 也就是只转换规则序列1
    ADC1_Handler.Init.DiscontinuousConvMode=DISABLE;             	//禁止不连续采样模式
    ADC1_Handler.Init.NbrOfDiscConversion=0;                     	//不连续采样通道数为0
    ADC1_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       	//软件触发
    ADC1_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;	//使用软件触发
    ADC1_Handler.Init.DMAContinuousRequests=ENABLE;//DISABLE;             	//关闭DMA请求

    HAL_ADC_DeInit(&ADC1_Handler);									//复位ADC
    HAL_ADC_Init(&ADC1_Handler);                                 	//初始化ADC

}
#endif
/**
 * @brief :	ADC3初始化函数.
 * @param :	none.
 * @retval:	none.
 */
 #if(USE_Adc3_Conf == 1)	
static void BSP_Adc3Init(void)
{
    __HAL_LINKDMA(&ADC3_Handler,DMA_Handle,ADC3_DMA_Handler);    	//将DMA与ADC3联系起来

    BSP_AdcDmaConfig(DMA2_Stream0,DMA_CHANNEL_2);					//初始化DMA

    ADC3_Handler.Instance=ADC3;
    ADC3_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4;   	//4分频，ADCCLK=PCLK2/4=108/4=27MHZ   (F7  ADC频率不能超过36M)
    ADC3_Handler.Init.Resolution=ADC_RESOLUTION_12B;             	//12位模式
    ADC3_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             	//右对齐
    ADC3_Handler.Init.ScanConvMode=DISABLE;                       	//非扫描模式  允许扫描——多通道
    ADC3_Handler.Init.EOCSelection=DISABLE;                      	//关闭EOC中断
    ADC3_Handler.Init.ContinuousConvMode=DISABLE;                	//关闭连续转换
    ADC3_Handler.Init.NbrOfConversion=1;                         	//1个转换在规则序列中 也就是只转换规则序列1
    ADC3_Handler.Init.DiscontinuousConvMode=DISABLE;             	//禁止不连续采样模式
    ADC3_Handler.Init.NbrOfDiscConversion=0;                     	//不连续采样通道数为0
    ADC3_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       	//软件触发
    ADC3_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;	//使用软件触发
    ADC3_Handler.Init.DMAContinuousRequests=ENABLE;//DISABLE;             	//关闭DMA请求

    HAL_ADC_DeInit(&ADC3_Handler);									//复位ADC
    HAL_ADC_Init(&ADC3_Handler);                                 	//初始化ADC
}
#endif
/**
 * @brief :	ADC底层初始化函数.
 * @param :	ADC_HandleTypeDef:ADC句柄.
 * @retval:	none.
 */
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
#if(USE_Adc1_Conf == 1)	
	{
    GPIO_InitTypeDef GPIO_Initure;
    if(hadc->Instance == ADC1)
    {
        __HAL_RCC_ADC1_CLK_ENABLE();            					//使能ADC1时钟
        __HAL_RCC_GPIOA_CLK_ENABLE();								//开启GPIOA时钟
//        __HAL_RCC_GPIOB_CLK_ENABLE();								//开启GPIOB时钟
//        __HAL_RCC_GPIOC_CLK_ENABLE();								//开启GPIOC时钟

//        //ADC1_IN0
//        GPIO_Initure.Pin=GPIO_PIN_0;            					//PA0
//        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     					//模拟
//        GPIO_Initure.Pull=GPIO_NOPULL;          					//不带上下拉
//        HAL_GPIO_Init(GPIOA,&GPIO_Initure);

//        //ADC1_IN1
//        GPIO_Initure.Pin=GPIO_PIN_1;            					//PA1
//        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     					//模拟
//        GPIO_Initure.Pull=GPIO_NOPULL;          					//不带上下拉
//        HAL_GPIO_Init(GPIOA,&GPIO_Initure);

//        //ADC1_IN6
//        GPIO_Initure.Pin=GPIO_PIN_6;            					//PA6
//        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     					//模拟
//        GPIO_Initure.Pull=GPIO_NOPULL;          					//不带上下拉
//        HAL_GPIO_Init(GPIOA,&GPIO_Initure);

//        //ADC1_IN7
//        GPIO_Initure.Pin=GPIO_PIN_7;            					//PA7
//        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     					//模拟
//        GPIO_Initure.Pull=GPIO_NOPULL;          					//不带上下拉
//        HAL_GPIO_Init(GPIOA,&GPIO_Initure);

//        //ADC1_IN8
//        GPIO_Initure.Pin=GPIO_PIN_0;            					//PB0
//        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     					//模拟
//        GPIO_Initure.Pull=GPIO_NOPULL;          					//不带上下拉
//        HAL_GPIO_Init(GPIOB,&GPIO_Initure);

//        //ADC1_IN9
//        GPIO_Initure.Pin=GPIO_PIN_1;            					//PB1
//        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     					//模拟
//        GPIO_Initure.Pull=GPIO_NOPULL;          					//不带上下拉
//        HAL_GPIO_Init(GPIOB,&GPIO_Initure);

//        //ADC1_IN10
//        GPIO_Initure.Pin=GPIO_PIN_0;            					//PC0
//        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     					//模拟
//        GPIO_Initure.Pull=GPIO_NOPULL;          					//不带上下拉
//        HAL_GPIO_Init(GPIOC,&GPIO_Initure);
//        //ADC1_IN11
//        GPIO_Initure.Pin=GPIO_PIN_1;            					//PC1
//        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     					//模拟
//        GPIO_Initure.Pull=GPIO_NOPULL;          					//不带上下拉
//        HAL_GPIO_Init(GPIOC,&GPIO_Initure);

//        //ADC1_IN12
//        GPIO_Initure.Pin=GPIO_PIN_2;            					//PC2
//        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     					//模拟
//        GPIO_Initure.Pull=GPIO_NOPULL;          					//不带上下拉
//        HAL_GPIO_Init(GPIOC,&GPIO_Initure);

//        //ADC1_IN13
//        GPIO_Initure.Pin=GPIO_PIN_3;            					//PC3
//        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     					//模拟
//        GPIO_Initure.Pull=GPIO_NOPULL;          					//不带上下拉
//        HAL_GPIO_Init(GPIOC,&GPIO_Initure);

//        //ADC1_IN14
//        GPIO_Initure.Pin=GPIO_PIN_4;            					//PC4
//        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     					//模拟
//        GPIO_Initure.Pull=GPIO_NOPULL;          					//不带上下拉
//        HAL_GPIO_Init(GPIOC,&GPIO_Initure);

//        //ADC1_IN15
//        GPIO_Initure.Pin=GPIO_PIN_5;            					//PC5
//        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     					//模拟
//        GPIO_Initure.Pull=GPIO_NOPULL;         					//不带上下拉
//        HAL_GPIO_Init(GPIOC,&GPIO_Initure);

        //ADC1_IN5
        GPIO_Initure.Pin=GPIO_PIN_5;            					//PA5
        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     					//模拟
        GPIO_Initure.Pull=GPIO_NOPULL;          					//不带上下拉
        HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    }
	}
#endif
#if(USE_Adc3_Conf == 1)	
	{
    GPIO_InitTypeDef GPIO_Initure;
    if(hadc->Instance == ADC3)
    {
        __HAL_RCC_ADC3_CLK_ENABLE();            					//使能ADC1时钟
        __HAL_RCC_GPIOF_CLK_ENABLE();								//开启GPIOA时钟

        //ADC3_IN4
        GPIO_Initure.Pin=GPIO_PIN_6;            					//PF6
        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     					//模拟
        GPIO_Initure.Pull=GPIO_NOPULL;          					//不带上下拉
        HAL_GPIO_Init(GPIOF,&GPIO_Initure);

////////////////////        //ADC3_IN5
////////////////////        GPIO_Initure.Pin=GPIO_PIN_7;            //PF7
////////////////////        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //模拟
////////////////////        GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉
////////////////////        HAL_GPIO_Init(GPIOF,&GPIO_Initure);

////////////////////        //ADC3_IN6
////////////////////        GPIO_Initure.Pin=GPIO_PIN_8;            //PF8
////////////////////        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //模拟
////////////////////        GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉
////////////////////        HAL_GPIO_Init(GPIOF,&GPIO_Initure);

////////////////////        //ADC3_IN7
////////////////////        GPIO_Initure.Pin=GPIO_PIN_9;            //PF9
////////////////////        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //模拟
////////////////////        GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉
////////////////////        HAL_GPIO_Init(GPIOF,&GPIO_Initure);

////////////////////        //ADC3_IN8
////////////////////        GPIO_Initure.Pin=GPIO_PIN_10;            //PF10
////////////////////        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //模拟
////////////////////        GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉
////////////////////        HAL_GPIO_Init(GPIOF,&GPIO_Initure);

////////////////////        //ADC3_IN9
////////////////////        GPIO_Initure.Pin=GPIO_PIN_3;            //PF3
////////////////////        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //模拟
////////////////////        GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉
////////////////////        HAL_GPIO_Init(GPIOF,&GPIO_Initure);

////////////////////        //ADC3_IN14
////////////////////        GPIO_Initure.Pin=GPIO_PIN_4;            //PF4
////////////////////        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //模拟
////////////////////        GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉
////////////////////        HAL_GPIO_Init(GPIOF,&GPIO_Initure);

////////////////////        //ADC3_IN15
////////////////////        GPIO_Initure.Pin=GPIO_PIN_5;            //PF5
////////////////////        GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //模拟
////////////////////        GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉
////////////////////        HAL_GPIO_Init(GPIOF,&GPIO_Initure);
    }
	}
#endif
}
/**
 * @brief :	获得ADC1值.
 * @param :	i_pBuffAddr：读取的数据
			ChUsedNmb: 通道值 0~16，取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
 * @retval:	none.
 */
#if(USE_Adc1_Conf == 1)	
void BSP_Adc1NormalConvertStart(uint32_t *i_pBuffAddr, uint32_t ChUsedNmb)
{

    ADC_ChannelConfTypeDef ADC1_ChanConf;

//    MYDMA_Config(DMA2_Stream0,DMA_CHANNEL_0);						//初始化DMA ADC1和ADC3同时公用DMA2数据流0，但不同通道，每次都要重新初始化配置
    //使用此行，或者下列三行
    ADC1_DMA_Handler.Instance=DMA2_Stream0;                     	//数据流选择
    ADC1_DMA_Handler.Init.Channel=DMA_CHANNEL_0;                	//通道选择
    HAL_DMA_Init(&ADC1_DMA_Handler);

    ADC1_ChanConf.Channel=ChUsedNmb;                        		//通道
    ADC1_ChanConf.Rank=1;                                       	//1个序列
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        	//采样时间
    ADC1_ChanConf.Offset=0;
    HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);        	//通道配置

    if(HAL_ADC_Start_DMA(&ADC1_Handler, i_pBuffAddr, 1) != HAL_OK) 	//启动DMA方式采集ADC信号
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
 * @brief :	获得ADC3值.
 * @param :	i_pBuffAddr：读取的数据
			ChUsedNmb: 通道值 0~16，取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
 * @retval:	none.
 */
 #if(USE_Adc3_Conf == 1)	
void BSP_Adc3NormalConvertStart(uint32_t *i_pBuffAddr, uint32_t ChUsedNmb)
{
    ADC_ChannelConfTypeDef ADC3_ChanConf;

//    MYDMA_Config(DMA2_Stream0,DMA_CHANNEL_2);						//初始化DMA ADC1和ADC3同时公用DMA2数据流0，但不同通道，每次都要重新初始化配置
    //使用此行，或者下列三行
    ADC3_DMA_Handler.Instance=DMA2_Stream0;                     	//数据流选择
    ADC3_DMA_Handler.Init.Channel=DMA_CHANNEL_2;                	//通道选择
    HAL_DMA_Init(&ADC3_DMA_Handler);

    ADC3_ChanConf.Channel=ChUsedNmb;                        		//通道
    ADC3_ChanConf.Rank=1;                                       	//1个序列
    ADC3_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        	//采样时间
    ADC3_ChanConf.Offset=0;
    HAL_ADC_ConfigChannel(&ADC3_Handler,&ADC3_ChanConf);        	//通道配置

    if(HAL_ADC_Start_DMA(&ADC3_Handler, i_pBuffAddr, 1) != HAL_OK)
    {

    }
    else
    {
        LCD_ShowString(30,250+16,200,16,16,"ADC3_CH4_ ok!!");     	//临时显示
    }
}
#endif

/**
 * @brief :	ADC 相关DMA配置.
 * @param :	DMA_Streamx：DMA句柄
			ChUsedNmb: 通道值 0~16，取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
 * @retval:	none.
 */
 #if((USE_Adc1_Conf == 1)||(USE_Adc3_Conf == 1))
static void BSP_AdcDmaConfig(DMA_Stream_TypeDef *DMA_Streamx,uint32_t ChUsedNmb)
{
    if((uint32_t)DMA_Streamx>(uint32_t)DMA2)						//得到当前stream是属于DMA2还是DMA1
    {
        __HAL_RCC_DMA2_CLK_ENABLE();								//DMA2时钟使能
    } else
    {
        __HAL_RCC_DMA1_CLK_ENABLE();								//DMA1时钟使能
    }

#if(USE_Adc1_Conf == 1)	
    if((DMA_Streamx == DMA2_Stream0)&& (ChUsedNmb == DMA_CHANNEL_0))
    {
        // DMA配置
        ADC1_DMA_Handler.Instance=DMA_Streamx;                      //数据流选择
        ADC1_DMA_Handler.Init.Channel=ChUsedNmb;                    //通道选择
        ADC1_DMA_Handler.Init.Direction=DMA_PERIPH_TO_MEMORY ;      //外设到存储器
        ADC1_DMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;           //外设非增量模式
        ADC1_DMA_Handler.Init.MemInc=DMA_MINC_DISABLE;//DMA_MINC_ENABLE;      	//存储器增量模式
        ADC1_DMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_HALFWORD;    	//外设数据长度:8位  16位
        ADC1_DMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_HALFWORD;       	//存储器数据长度:8位 16位
        ADC1_DMA_Handler.Init.Mode=DMA_CIRCULAR;//DMA_NORMAL;                 	//外设流控模式
        ADC1_DMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;             		//中等优先级
        ADC1_DMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;
        ADC1_DMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;
        ADC1_DMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 		//存储器突发单次传输
        ADC1_DMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              		//外设突发单次传输


        HAL_DMA_DeInit(&ADC1_DMA_Handler);										//复位ADC1_DMA_Handler

        HAL_DMA_Init(&ADC1_DMA_Handler);										//初始化ADC1_DMA_Handler

    }
#endif
#if(USE_Adc3_Conf == 1)	
    if((DMA_Streamx == DMA2_Stream0)&& (ChUsedNmb == DMA_CHANNEL_2))
    {
        // DMA配置
        ADC3_DMA_Handler.Instance=DMA_Streamx;                            		//数据流选择
        ADC3_DMA_Handler.Init.Channel=ChUsedNmb;                                		//通道选择
        ADC3_DMA_Handler.Init.Direction=DMA_PERIPH_TO_MEMORY ;             		//外设到存储器
        ADC3_DMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 		//外设非增量模式
        ADC3_DMA_Handler.Init.MemInc=DMA_MINC_DISABLE;//DMA_MINC_ENABLE;      	//存储器增量模式
        ADC3_DMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_HALFWORD;    	//外设数据长度:8位  16位
        ADC3_DMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_HALFWORD;       	//存储器数据长度:8位 16位
        ADC3_DMA_Handler.Init.Mode=DMA_CIRCULAR;//DMA_NORMAL;                 	//外设流控模式
        ADC3_DMA_Handler.Init.Priority=DMA_PRIORITY_HIGH;               		//中等优先级
        ADC3_DMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;					//没用到
        ADC3_DMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;
        ADC3_DMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 		//存储器突发单次传输
        ADC3_DMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              		//外设突发单次传输


        HAL_DMA_DeInit(&ADC3_DMA_Handler);										//复位ADC3_DMA_Handler

        HAL_DMA_Init(&ADC3_DMA_Handler);										//初始化ADC3_DMA_Handler

    }
#endif
#if((USE_Adc1_Conf == 1)||(USE_Adc3_Conf == 1))	
    HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);										//使能DMA2数据流0中断NVIC
    HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 3, 3);								//配置DMA2数据流0中断优先级
#endif
}

#endif


/**
 * @brief :	DMA2数据流0中断服务函数.
 * @param :	none.
 * @retval:	none.
 */
#if((USE_Adc1_Conf == 1)||(USE_Adc3_Conf == 1))
void DMA2_Stream0_IRQHandler(void)
{
#if(USE_Adc1_Conf == 1)
    if(HAL_DMA_GetState(&ADC1_DMA_Handler))										//获取ADC1_DMA_Handler状态
    {
        HAL_DMA_IRQHandler(ADC1_Handler.DMA_Handle);
        //
        //添加发送信号量等信息
        //
    }
#endif
#if(USE_Adc3_Conf == 1)
    if(HAL_DMA_GetState(&ADC3_DMA_Handler))               						//获取ADC3_DMA_Handler状态
    {
        HAL_DMA_IRQHandler(ADC3_Handler.DMA_Handle);
        //
        //添加发送信号量等信息
        //
    }
#endif
}
#endif

#endif

/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/




