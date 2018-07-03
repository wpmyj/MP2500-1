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
#define EN_BSP_Tim1ChCap         1   		//使能定时器1捕获
#else
#define EN_BSP_Tim1ChCap         0   		//失能定时器1捕获
#endif

#if(USE_Tim4ChCap == 1)
#define EN_BSP_Tim4ChCap         1   		//使能定时器4捕获
#else
#define EN_BSP_Tim4ChCap         0   		//失能定时器4捕获
#endif

#if(USE_Tim5ChCap == 1)
#define EN_BSP_Tim5ChCap         1   		//使能定时器5捕获
#else
#define EN_BSP_Tim5ChCap         0   		//使能定时器5捕获
#endif


#if(USE_Tim8Pwm == 1)
#define EN_BSP_Tim8Pwm           1   		//使能定时器8 pwm输出
#else
#define EN_BSP_Tim8Pwm           0   		//失能定时器8 pwm输出
#endif



#if(USE_Tim9Pwm == 1)
#define EN_BSP_Tim9Pwm           1   		//使能定时器8 pwm输出
#else
#define EN_BSP_Tim9Pwm           0   		//失能定时器8 pwm输出
#endif


#if(EN_BSP_Tim1ChCap == 1)
TIM_HandleTypeDef TIM1_Handler;				//定时器句柄 1输入捕获
#endif

#if(EN_BSP_Tim4ChCap == 1)
TIM_HandleTypeDef TIM4_Handler;				//定时器句柄 4输入捕获
#endif

#if(USE_Tim5ChCap == 1)
TIM_HandleTypeDef TIM5_Handler;				//定时器句柄 5输入捕获
#endif

#if(EN_BSP_Tim8Pwm == 1)
TIM_HandleTypeDef TIM8_Handler;				//定时器句柄 8输出比较
TIM_OC_InitTypeDef TIM8_CHHandler;    		//定时器8PWM句柄 
#endif

#if(USE_Tim9Pwm == 1)
TIM_HandleTypeDef TIM9_Handler;				//定时器句柄 9输出比较
TIM_OC_InitTypeDef TIM9_CHHandler;    		//定时器9PWM句柄
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
//输入捕获部分

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
//pwm输出部分

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
*                             输入捕获部分
********************************************************************************
*/
/**
 * @brief :	输入捕获初始化
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
 * @brief :	定时器1通道2,3输入捕获配置
 * @param :	arr：自动重装值(TIM2,TIM5是32位的!!)
			psc：时钟预分频数
 * @retval:	none.
 */
#if(EN_BSP_Tim1ChCap == 1)
static void BSP_Tim1ChCapInit(uint32_t arr,uint16_t psc)
{
    TIM_IC_InitTypeDef TIM1_CHConfig;

    TIM1_Handler.Instance=TIM1;                          								//通用定时器5
    TIM1_Handler.Init.Prescaler=psc;                     								//分频
    TIM1_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    								//向上计数器
    TIM1_Handler.Init.Period=arr;                        								//自动装载值
    TIM1_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_IC_Init(&TIM1_Handler);

    TIM1_CHConfig.ICPolarity=TIM_ICPOLARITY_RISING;    									//上升沿捕获
    TIM1_CHConfig.ICSelection=TIM_ICSELECTION_DIRECTTI;									//映射到TI1上
    TIM1_CHConfig.ICPrescaler=TIM_ICPSC_DIV1;          									//配置输入分频，不分频
    TIM1_CHConfig.ICFilter=0;                          									//配置输入滤波器，不滤波

    HAL_TIM_IC_ConfigChannel(&TIM1_Handler,&TIM1_CHConfig,TIM_CHANNEL_2);				//配置TIM1通道2
    HAL_TIM_IC_ConfigChannel(&TIM1_Handler,&TIM1_CHConfig,TIM_CHANNEL_3);				//配置TIM1通道3

    HAL_TIM_IC_Start_IT(&TIM1_Handler,TIM_CHANNEL_2);   								//开始捕获TIM1的通道2
    HAL_TIM_IC_Start_IT(&TIM1_Handler,TIM_CHANNEL_3);   								//开始捕获TIM1的通道3

    __HAL_TIM_ENABLE_IT(&TIM1_Handler,TIM_IT_UPDATE);   								//使能更新中断
}
#endif

/**
 * @brief :	定时器4通道1,2,3,4输入捕获配置
 * @param :	arr：自动重装值(TIM2,TIM5是32位的!!)
			psc：时钟预分频数
 * @retval:	none.
 */
#if(EN_BSP_Tim4ChCap == 1)
static void BSP_Tim4ChCapInit(uint32_t arr,uint16_t psc)
{
    TIM_IC_InitTypeDef TIM4_CHConfig;

    TIM4_Handler.Instance=TIM4;                          								//通用定时器5
    TIM4_Handler.Init.Prescaler=psc;                     								//分频
    TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    								//向上计数器
    TIM4_Handler.Init.Period=arr;                        								//自动装载值
    TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_IC_Init(&TIM4_Handler);

    TIM4_CHConfig.ICPolarity=TIM_ICPOLARITY_RISING;    									//上升沿捕获
    TIM4_CHConfig.ICSelection=TIM_ICSELECTION_DIRECTTI;									//映射到TI1上
    TIM4_CHConfig.ICPrescaler=TIM_ICPSC_DIV1;          									//配置输入分频，不分频
    TIM4_CHConfig.ICFilter=0;                          									//配置输入滤波器，不滤波

    HAL_TIM_IC_ConfigChannel(&TIM4_Handler,&TIM4_CHConfig,TIM_CHANNEL_1);				//配置TIM4通道1
    HAL_TIM_IC_ConfigChannel(&TIM4_Handler,&TIM4_CHConfig,TIM_CHANNEL_2);				//配置TIM4通道2
    HAL_TIM_IC_ConfigChannel(&TIM4_Handler,&TIM4_CHConfig,TIM_CHANNEL_3);				//配置TIM4通道3
    HAL_TIM_IC_ConfigChannel(&TIM4_Handler,&TIM4_CHConfig,TIM_CHANNEL_4);				//配置TIM4通道4

    HAL_TIM_IC_Start_IT(&TIM4_Handler,TIM_CHANNEL_1);   								//开始捕获TIM1的通道1
    HAL_TIM_IC_Start_IT(&TIM4_Handler,TIM_CHANNEL_2);   								//开始捕获TIM1的通道2
    HAL_TIM_IC_Start_IT(&TIM4_Handler,TIM_CHANNEL_3);   								//开始捕获TIM1的通道3
    HAL_TIM_IC_Start_IT(&TIM4_Handler,TIM_CHANNEL_4);   								//开始捕获TIM1的通道4

    __HAL_TIM_ENABLE_IT(&TIM4_Handler,TIM_IT_UPDATE);   								//使能更新中断
}
#endif

/**
 * @brief :	定时器5通道1,2,3输入捕获配置
 * @param :	arr：自动重装值(TIM2,TIM5是32位的!!)
			psc：时钟预分频数
 * @retval:	none.
 */
#if(EN_BSP_Tim5ChCap == 1)
static void BSP_Tim5ChCapInit(uint32_t arr,uint16_t psc)
{
    TIM_IC_InitTypeDef TIM5_CHConfig;

    TIM5_Handler.Instance=TIM5;                          								//通用定时器5
    TIM5_Handler.Init.Prescaler=psc;                     								//分频
    TIM5_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    								//向上计数器
    TIM5_Handler.Init.Period=arr;                        								//自动装载值
    TIM5_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_IC_Init(&TIM5_Handler);

    TIM5_CHConfig.ICPolarity=TIM_ICPOLARITY_RISING;    									//上升沿捕获
    TIM5_CHConfig.ICSelection=TIM_ICSELECTION_DIRECTTI;									//映射到TI1上
    TIM5_CHConfig.ICPrescaler=TIM_ICPSC_DIV1;          									//配置输入分频，不分频
    TIM5_CHConfig.ICFilter=0;                          									//配置输入滤波器，不滤波

    HAL_TIM_IC_ConfigChannel(&TIM5_Handler,&TIM5_CHConfig,TIM_CHANNEL_1);				//配置TIM4通道1
//    HAL_TIM_IC_ConfigChannel(&TIM5_Handler,&TIM5_CHConfig,TIM_CHANNEL_2);				//配置TIM4通道2
//    HAL_TIM_IC_ConfigChannel(&TIM5_Handler,&TIM5_CHConfig,TIM_CHANNEL_3);				//配置TIM4通道3

    HAL_TIM_IC_Start_IT(&TIM5_Handler,TIM_CHANNEL_1);   								//开始捕获TIM1的通道1
//    HAL_TIM_IC_Start_IT(&TIM5_Handler,TIM_CHANNEL_2);   								//开始捕获TIM1的通道2
//    HAL_TIM_IC_Start_IT(&TIM5_Handler,TIM_CHANNEL_3);   								//开始捕获TIM1的通道3

    __HAL_TIM_ENABLE_IT(&TIM5_Handler,TIM_IT_UPDATE);   								//使能更新中断
}
#endif

/**
 * @brief :	定时器底层驱动，时钟使能，引脚配置,此函数会被HAL_TIM_IC_Init()调用
 * @param :	htim:定时器句柄
 * @retval:	none.
 */
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{

#if(EN_BSP_Tim1ChCap == 1)
    if(htim->Instance == TIM1)
    {
        GPIO_InitTypeDef GPIO_Initure;
        __HAL_RCC_TIM1_CLK_ENABLE();            										//使能TIM1时钟
        __HAL_RCC_GPIOA_CLK_ENABLE();													//开启GPIOA时钟

        GPIO_Initure.Pin=GPIO_PIN_9|GPIO_PIN_10;										//PA9,PA10
        GPIO_Initure.Mode=GPIO_MODE_AF_PP;      										//复用推挽输出
        GPIO_Initure.Pull=GPIO_PULLDOWN;        										//下拉
        GPIO_Initure.Speed=GPIO_SPEED_HIGH;     										//高速
        GPIO_Initure.Alternate=GPIO_AF1_TIM1;   										//PA9,10复用为TIM1通道2,3
        HAL_GPIO_Init(GPIOA,&GPIO_Initure);

        HAL_NVIC_SetPriority(TIM1_CC_IRQn ,2,0);    									//设置中断优先级，抢占优先级2，子优先级0
        HAL_NVIC_EnableIRQ(TIM1_CC_IRQn );          									//开启ITM1中断
    }
#endif
#if(EN_BSP_Tim4ChCap == 1)
    if(htim->Instance == TIM4)
    {
        GPIO_InitTypeDef GPIO_Initure;
        __HAL_RCC_TIM4_CLK_ENABLE();            										//使能TIM4时钟
        __HAL_RCC_GPIOB_CLK_ENABLE();													//开启GPIOB时钟
        __HAL_RCC_GPIOD_CLK_ENABLE();													//开启GPIOD时钟

        GPIO_Initure.Pin=GPIO_PIN_12;
        GPIO_Initure.Mode=GPIO_MODE_AF_PP;      										//复用推挽输出
        GPIO_Initure.Pull=GPIO_PULLDOWN;        										//下拉
        GPIO_Initure.Speed=GPIO_SPEED_HIGH;     										//高速
        GPIO_Initure.Alternate=GPIO_AF2_TIM4;   										//12复用为TIM4通道1
        HAL_GPIO_Init(GPIOD,&GPIO_Initure);

        GPIO_Initure.Pin=GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
        GPIO_Initure.Mode=GPIO_MODE_AF_PP;      										//复用推挽输出
        GPIO_Initure.Pull=GPIO_PULLDOWN;        										//下拉
        GPIO_Initure.Speed=GPIO_SPEED_HIGH;     										//高速
        GPIO_Initure.Alternate=GPIO_AF2_TIM4;   										//7,8,9复用为TIM4通道2,3,4
        HAL_GPIO_Init(GPIOB,&GPIO_Initure);

        HAL_NVIC_SetPriority(TIM4_IRQn ,2,1);    										//设置中断优先级，抢占优先级2，子优先级2
        HAL_NVIC_EnableIRQ(TIM4_IRQn );          										//开启ITM4中断
    }
#endif

#if(EN_BSP_Tim5ChCap == 1)
    if(htim->Instance == TIM5)
    {
        GPIO_InitTypeDef GPIO_Initure;
        __HAL_RCC_TIM5_CLK_ENABLE();            										//使能TIM5时钟
        __HAL_RCC_GPIOH_CLK_ENABLE();													//开启GPIOH时钟

        GPIO_Initure.Pin=GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
        GPIO_Initure.Mode=GPIO_MODE_AF_PP;      										//复用推挽输出
        GPIO_Initure.Pull=GPIO_PULLDOWN;        										//下拉
        GPIO_Initure.Speed=GPIO_SPEED_HIGH;     										//高速
        GPIO_Initure.Alternate=GPIO_AF2_TIM5;   										//PA9,10复用为TIM1通道2,3
        HAL_GPIO_Init(GPIOH,&GPIO_Initure);

        HAL_NVIC_SetPriority(TIM5_IRQn ,2,2);    										//设置中断优先级，抢占优先级2，子优先级2
        HAL_NVIC_EnableIRQ(TIM5_IRQn );          										//开启ITM5中断

    }
#endif
}

#if((USE_Tim_File == 1)&&(USE_Tim_Conf == 1))
/**
 * @brief :	定时器1中断服务函数
 * @param :	htim:定时器句柄
 * @retval:	none.
 */
#if(EN_BSP_Tim1ChCap == 1)
void TIM1_CC_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM1_Handler);													//定时器共用处理函数
}
#endif
/**
 * @brief :	定时器4中断服务函数
 * @param :	htim:定时器句柄
 * @retval:	none.
 */
#if(EN_BSP_Tim4ChCap == 1)
void TIM4_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM4_Handler);													//定时器共用处理函数
}
#endif
/**
 * @brief :	定时器5中断服务函数
 * @param :	htim:定时器句柄
 * @retval:	none.
 */
#if(EN_BSP_Tim5ChCap == 1)
void TIM5_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM5_Handler);													//定时器共用处理函数
}
#endif

//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到低电平;1,已经捕获到低电平了.
//[5:0]:捕获低电平后溢出的次数(对于32位定时器来说,1us计数器加1,溢出时间:4294秒)
uint8_t  TIM5CH1_CAPTURE_STA=0;	//输入捕获状态
uint32_t	TIM5CH1_CAPTURE_VAL;	//输入捕获值(TIM2/TIM5是32位)
//定时器更新中断（计数溢出）中断处理回调函数， 该函数在HAL_TIM_IRQHandler中会被调用
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)								//更新中断（溢出）发生时执行
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
    if((TIM5CH1_CAPTURE_STA&0X80)==0)													//还未成功捕获
    {
        if(TIM5CH1_CAPTURE_STA&0X40)													//已经捕获到高电平了
        {
            if((TIM5CH1_CAPTURE_STA&0X3F)==0X3F)										//高电平太长了
            {
                TIM5CH1_CAPTURE_STA|=0X80;												//标记成功捕获了一次
                TIM5CH1_CAPTURE_VAL=0XFFFFFFFF;
            } else TIM5CH1_CAPTURE_STA++;
        }
    }
#endif
}


//定时器输入捕获中断处理回调函数，该函数在HAL_TIM_IRQHandler中会被调用
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)								//捕获中断发生时执行
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
    if((TIM5CH1_CAPTURE_STA&0X80)==0)													//还未成功捕获
    {
        if(TIM5CH1_CAPTURE_STA&0X40)													//捕获到一个下降沿
        {
            TIM5CH1_CAPTURE_STA|=0X80;													//标记成功捕获到一次高电平脉宽
            TIM5CH1_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM5_Handler,TIM_CHANNEL_1);	//获取当前的捕获值.
            TIM_RESET_CAPTUREPOLARITY(&TIM5_Handler,TIM_CHANNEL_1);   					//一定要先清除原来的设置！！
            TIM_SET_CAPTUREPOLARITY(&TIM5_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);	//配置TIM5通道1上升沿捕获
        } else  																		//还未开始,第一次捕获上升沿
        {
            TIM5CH1_CAPTURE_STA=0;														//清空
            TIM5CH1_CAPTURE_VAL=0;
            TIM5CH1_CAPTURE_STA|=0X40;													//标记捕获到了上升沿
            __HAL_TIM_DISABLE(&TIM5_Handler);        									//关闭定时器5
            __HAL_TIM_SET_COUNTER(&TIM5_Handler,0);
            TIM_RESET_CAPTUREPOLARITY(&TIM5_Handler,TIM_CHANNEL_1);   					//一定要先清除原来的设置！！
            TIM_SET_CAPTUREPOLARITY(&TIM5_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);//定时器5通道1设置为下降沿捕获
            __HAL_TIM_ENABLE(&TIM5_Handler);											//使能定时器5
        }
    }
#endif
}
#endif



/*
********************************************************************************
*                             PWM输出部分
********************************************************************************
*/
/**
 * @brief :	定时器pwm初始化
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
 * @brief :	定时器8 pwm初始化
 * @param :	arr：自动重装值
			psc：时钟预分频数
 * @retval:	none.
 */
#if(EN_BSP_Tim8Pwm == 1)
static void BSP_Tim8PwmInit(uint16_t arr,uint16_t psc)
{
    TIM8_Handler.Instance=TIM8;            												//定时器8
    TIM8_Handler.Init.Prescaler=psc;       												//定时器分频
    TIM8_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;									//向上计数模式
    TIM8_Handler.Init.Period=arr;          												//自动重装载值
    TIM8_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM8_Handler);       												//初始化PWM

    TIM8_CHHandler.OCMode=TIM_OCMODE_PWM1; 												//模式选择PWM1
    TIM8_CHHandler.Pulse=arr/2;            												//设置比较值,此值用来确定占空比，
																						//默认比较值为自动重装载值的一半,即占空比为50%
    TIM8_CHHandler.OCPolarity=TIM_OCPOLARITY_LOW; 										//输出比较极性为低

    HAL_TIM_PWM_ConfigChannel(&TIM8_Handler,&TIM8_CHHandler,TIM_CHANNEL_1);				//配置TIM8通道1
    HAL_TIM_PWM_ConfigChannel(&TIM8_Handler,&TIM8_CHHandler,TIM_CHANNEL_2);				//配置TIM8通道2
    HAL_TIM_PWM_ConfigChannel(&TIM8_Handler,&TIM8_CHHandler,TIM_CHANNEL_3);				//配置TIM8通道3

    HAL_TIM_PWM_Start(&TIM8_Handler,TIM_CHANNEL_1);										//开启PWM通道1
    HAL_TIM_PWM_Start(&TIM8_Handler,TIM_CHANNEL_2);										//开启PWM通道2
    HAL_TIM_PWM_Start(&TIM8_Handler,TIM_CHANNEL_3);										//开启PWM通道3

}
#endif
/**
 * @brief :	定时器9 pwm初始化
 * @param :	arr：自动重装值
			psc：时钟预分频数
 * @retval:	none.
 */
#if(EN_BSP_Tim9Pwm == 1)
static void BSP_Tim9PwmInit(uint16_t arr,uint16_t psc)
{
    TIM9_Handler.Instance=TIM9;            												//定时器9
    TIM9_Handler.Init.Prescaler=psc;       												//定时器分频
    TIM9_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;									//向上计数模式
    TIM9_Handler.Init.Period=arr;          												//自动重装载值
    TIM9_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM9_Handler);       												//初始化PWM

    TIM9_CHHandler.OCMode=TIM_OCMODE_PWM1; 												//模式选择PWM1
    TIM9_CHHandler.Pulse=arr/2;            												//设置比较值,此值用来确定占空比，
																						//默认比较值为自动重装载值的一半,即占空比为50%
    TIM8_CHHandler.OCPolarity=TIM_OCPOLARITY_LOW; 										//输出比较极性为低

    HAL_TIM_PWM_ConfigChannel(&TIM9_Handler,&TIM9_CHHandler,TIM_CHANNEL_1);				//配置TIM9通道1
    HAL_TIM_PWM_ConfigChannel(&TIM9_Handler,&TIM9_CHHandler,TIM_CHANNEL_2);				//配置TIM9通道2

    HAL_TIM_PWM_Start(&TIM9_Handler,TIM_CHANNEL_1);										//开启PWM通道1
    HAL_TIM_PWM_Start(&TIM9_Handler,TIM_CHANNEL_2);										//开启PWM通道2
}
#endif
/**
 * @brief :	定时器底层驱动，时钟使能，引脚配置,此函数会被HAL_TIM_PWM_Init()调用
 * @param :	htim:定时器句柄
 * @retval:	none.
 */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{

#if(EN_BSP_Tim8Pwm == 1)
    if(htim->Instance == TIM8)
    {
        GPIO_InitTypeDef GPIO_Initure;
        __HAL_RCC_TIM8_CLK_ENABLE();													//使能定时器8
        __HAL_RCC_GPIOI_CLK_ENABLE();													//开启GPIOB时钟

        GPIO_Initure.Pin=GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
        GPIO_Initure.Mode=GPIO_MODE_AF_PP;  											//复用推完输出
        GPIO_Initure.Pull=GPIO_PULLUP;          										//上拉
        GPIO_Initure.Speed=GPIO_SPEED_HIGH;     										//高速
        GPIO_Initure.Alternate=GPIO_AF3_TIM8;											//
        HAL_GPIO_Init(GPIOI,&GPIO_Initure);
    }
#endif

#if(EN_BSP_Tim9Pwm == 1)
    if(htim->Instance == TIM9)
    {
        GPIO_InitTypeDef GPIO_Initure;
        __HAL_RCC_TIM9_CLK_ENABLE();													//使能定时器9
        __HAL_RCC_GPIOE_CLK_ENABLE();													//开启GPIOE时钟

        GPIO_Initure.Pin=GPIO_PIN_5|GPIO_PIN_6;
        GPIO_Initure.Mode=GPIO_MODE_AF_PP;  											//复用推完输出
        GPIO_Initure.Pull=GPIO_PULLUP;          										//上拉
        GPIO_Initure.Speed=GPIO_SPEED_HIGH;     										//高速
        GPIO_Initure.Alternate=GPIO_AF3_TIM9;											//
        HAL_GPIO_Init(GPIOE,&GPIO_Initure);
    }
#endif
}
/**
 * @brief :	设置TIM8通道的占空比
 * @param :	Channel:通道（值为：TIM_CHANNEL_1，TIM_CHANNEL_2，TIM_CHANNEL_3）
			compare:比较值
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
 * @brief :	设置TIM9通道的占空比
 * @param :	Channel:通道（值为：TIM_CHANNEL_1，TIM_CHANNEL_2）
			compare:比较值
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
