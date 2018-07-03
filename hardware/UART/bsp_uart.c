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
 * @file      bsp_uart.c
 * @author    GuanXianDe
 * @version   V1.0.0
 * @date      14-June-2018
 * @brief     program entry
 * @attention none.
 *******************************************************************************
 */

/*
********************************************************************************
*                              INCLUDE FILES
********************************************************************************
*/
#include "bsp_uart.h"
/*
********************************************************************************
*                             MACRO DEFINITIONS
********************************************************************************
*/
#if(USE_Uart_File == 1)

#if(USE_Uart1_Conf)
#define EN_USART1_cfg         1   												//使能串口1
#else
#define EN_USART1_cfg         0   												//失能串口1
#endif

#if(USE_Uart2_Conf)
#define EN_USART2_cfg         1   												//使能串口2
#else
#define EN_USART2_cfg         0   												//失能串口2
#endif

#if(USE_Uart3_Conf)
#define EN_USART3_cfg         1  	 											//使能串口3
#else
#define EN_USART3_cfg         0   												//失能串口3
#endif

#if(USE_Uart4_Conf)
#define EN_UART4_cfg          1   												//使能串口4
#else
#define EN_UART4_cfg          0   												//失能串口4
#endif

#if(USE_Uart5_Conf)
#define EN_UART5_cfg          1   												//使能串口5
#else
#define EN_UART5_cfg          0   												//失能串口5
#endif

#if(USE_Uart6_Conf)
#define EN_USART6_cfg         1   												//使能串口6
#else
#define EN_USART6_cfg         0   												//失能串口6
#endif

#if(USE_Uart8_Conf)
#define EN_UART8_cfg          1   												//使能串口8
#else
#define EN_UART8_cfg          0   												//失能串口8
#endif


#define USART_REC_LEN  			200  											//定义最大接收字节数 200

#define RXBUFFERSIZE   1 														//缓存大小
/*
********************************************************************************
*                          OS-RELATED    VARIABLES
********************************************************************************
*/
typedef struct
{
    uint8_t aRxBuffer[RXBUFFERSIZE];											//HAL库USART接收Buffer
    UART_HandleTypeDef UART_Handler;    										//UART句柄
    uint32_t timeout;
    uint32_t maxDelay;

    uint16_t UART_RX_STA;
																				//接收状态
																				//bit15，	接收完成标志
																				//bit14，	接收到0x0d
																				//bit13~0，	接收到的有效字节数目
    uint8_t  *UART_RX_BUF;
} BSP_Uart_type;

#if(EN_USART1_cfg == 1)
BSP_Uart_type bsp_uart1;
#endif
#if(EN_USART2_cfg == 1)
BSP_Uart_type bsp_uart2;
#endif
#if(EN_USART3_cfg == 1)
BSP_Uart_type bsp_uart3;
#endif
#if(EN_UART4_cfg == 1)
BSP_Uart_type bsp_uart4;
#endif
#if(EN_UART5_cfg == 1)
BSP_Uart_type bsp_uart5;
#endif
#if(EN_USART6_cfg == 1)
BSP_Uart_type bsp_uart6;
#endif
#if(EN_UART8_cfg == 1)
BSP_Uart_type bsp_uart8;
#endif
#endif
/*
********************************************************************************
*                             FUNCTION PROTOTYPES
********************************************************************************
*/
#if(USE_Uart_File == 1)
static void bsp_uart_init(BSP_Uart_type *bsp_uart,USART_TypeDef * USART,uint32_t BaudRate,uint8_t *Buf);
static void BSP_UartReceiveByte(BSP_Uart_type *bsp_uart);
static void BSP_UartRecPack(BSP_Uart_type *bsp_uart);

//串口测试
static void BSP_TestUart(BSP_Uart_type *bsp_uart);
#endif
/*
********************************************************************************
*                              LOCAL VARIABLES
********************************************************************************
*/
#if(USE_Uart_File == 1)
uint8_t USART1_RX_BUF[USART_REC_LEN];     										//接收缓冲,最大USART_REC_LEN个字节.
uint8_t USART2_RX_BUF[USART_REC_LEN];     										//接收缓冲,最大USART_REC_LEN个字节.

#endif
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
 * @brief :	串口初始化
 * @param :	none.
 * @retval:	none.
 */
void BSP_UartInit(void)		        											//串口初始化
{
#if((USE_Uart_File == 1)&&(USE_Uart_Conf == 1))

#if(EN_USART1_cfg   == 1)
    bsp_uart_init(&bsp_uart1,USART1,115200,USART1_RX_BUF);
#endif

#if(EN_USART2_cfg   == 1)
    bsp_uart_init(&bsp_uart2,USART2,115200,USART2_RX_BUF);
#endif

#if(EN_USART3_cfg   == 1)
    bsp_uart_init(&bsp_uart3,USART3,115200,NULL);
#endif

#if(EN_UART4_cfg   == 1)
    bsp_uart_init(&bsp_uart4,UART4,115200,NULL);
#endif

#if(EN_UART5_cfg   == 1)
    bsp_uart_init(&bsp_uart5,UART5,115200,NULL);
#endif

#if(EN_USART6_cfg   == 1)
    bsp_uart_init(&bsp_uart6,USART6,115200,NULL);
#endif

#if(EN_USART8_cfg   == 1)
    bsp_uart_init(&bsp_uart8,UART8,115200,NULL);
#endif

#endif
}

/**
 * @brief :	串口底层初始化
 * @param :	bsp_uart ：bsp_uartXXX
			USART    : UARTXXX
			BaudRate : 波特率
			Buf      ：指定接收缓冲区
 * @retval:	none.
 */
#if((USE_Uart_File == 1)&&(USE_Uart_Conf == 1))
static void bsp_uart_init(BSP_Uart_type *bsp_uart,USART_TypeDef * USART,uint32_t BaudRate,uint8_t *Buf)
{
																									//UART 初始化设置
    bsp_uart->UART_Handler.Instance=USART;					    									//USARTx

    bsp_uart->UART_Handler.Init.BaudRate=BaudRate;				    								//波特率
    bsp_uart->UART_Handler.Init.WordLength=UART_WORDLENGTH_8B;   									//字长为8位数据格式
    bsp_uart->UART_Handler.Init.StopBits=UART_STOPBITS_1;	    									//一个停止位
    bsp_uart->UART_Handler.Init.Parity=UART_PARITY_NONE;		    								//无奇偶校验位
    bsp_uart->UART_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   									//无硬件流控
    bsp_uart->UART_Handler.Init.Mode=UART_MODE_TX_RX;		    									//收发模式
    HAL_UART_Init(&(bsp_uart->UART_Handler));					    								//HAL_UART_Init()会使能UART1

    HAL_UART_Receive_IT(&(bsp_uart->UART_Handler), (uint8_t *)(bsp_uart->aRxBuffer), RXBUFFERSIZE);	//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量

    bsp_uart->UART_RX_BUF = Buf;
}

/**
 * @brief :	UART底层初始化，时钟使能，引脚配置，中断配置（此函数会被HAL_UART_Init()调用）
 * @param :	huart:串口句柄
 * @retval:	none.
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_Initure;
#if(EN_USART1_cfg   == 1)
    if(huart->Instance==USART1)																		//如果是串口1，进行串口1 MSP初始化
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();																//使能GPIOA时钟
        __HAL_RCC_USART1_CLK_ENABLE();																//使能USART1时钟
        {
																									//需要修改为重映射到PB14(TX),PB15(RX)
            GPIO_Initure.Pin=GPIO_PIN_9;															//PA9(TX)
            GPIO_Initure.Mode=GPIO_MODE_AF_PP;														//复用推挽输出
            GPIO_Initure.Pull=GPIO_PULLUP;															//上拉
            GPIO_Initure.Speed=GPIO_SPEED_FAST;														//高速
            GPIO_Initure.Alternate=GPIO_AF7_USART1;													//复用为USART1
            HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   													//初始化PA9

            GPIO_Initure.Pin=GPIO_PIN_10;															//PA10(RX)
            HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   													//初始化PA10
        }

        HAL_NVIC_EnableIRQ(USART1_IRQn);															//使能USART1中断通道
        HAL_NVIC_SetPriority(USART1_IRQn,4,0);														//抢占优先级3，子优先级0
    }
#endif


#if(EN_USART2_cfg   == 1)
    if(huart->Instance==USART2)																		//如果是串口2，进行串口2 MSP初始化
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();																//使能GPIOA时钟
        __HAL_RCC_USART2_CLK_ENABLE();																//使能USART2时钟

        GPIO_Initure.Pin=GPIO_PIN_2;																//PA2(TX)
        GPIO_Initure.Mode=GPIO_MODE_AF_PP;															//复用推挽输出
        GPIO_Initure.Pull=GPIO_PULLUP;																//上拉
        GPIO_Initure.Speed=GPIO_SPEED_FAST;															//高速
        GPIO_Initure.Alternate=GPIO_AF7_USART2;														//复用为USART2
        HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   														//初始化PA2

        GPIO_Initure.Pin=GPIO_PIN_3;																//PA3(RX)
        HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   														//初始化PA3


        HAL_NVIC_EnableIRQ(USART2_IRQn);															//使能USART2中断通道
        HAL_NVIC_SetPriority(USART2_IRQn,3,0);														//抢占优先级3，子优先级0
    }
#endif

#if(EN_USART3_cfg   == 1)
    if(huart->Instance==USART3)																		//如果是串口3，进行串口3 MSP初始化
    {
        __HAL_RCC_GPIOB_CLK_ENABLE();																//使能GPIOB时钟
        __HAL_RCC_USART3_CLK_ENABLE();																//使能USART3时钟

        GPIO_Initure.Pin=GPIO_PIN_10;																//PB10(TX)
        GPIO_Initure.Mode=GPIO_MODE_AF_PP;															//复用推挽输出
        GPIO_Initure.Pull=GPIO_PULLUP;																//上拉
        GPIO_Initure.Speed=GPIO_SPEED_FAST;															//高速
        GPIO_Initure.Alternate=GPIO_AF7_USART3;														//复用为USART3
        HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   														//初始化PB10

        GPIO_Initure.Pin=GPIO_PIN_11;																//PB11(RX)
        HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   														//初始化PB11


        HAL_NVIC_EnableIRQ(USART3_IRQn);															//使能USART3中断通道
        HAL_NVIC_SetPriority(USART3_IRQn,3,0);														//抢占优先级3，子优先级0
    }
#endif

#if(EN_UART4_cfg   == 1)
    if(huart->Instance==UART4)																		//如果是串口4，进行串口4 MSP初始化
    {
        __HAL_RCC_GPIOH_CLK_ENABLE();																//使能GPIOH时钟
        __HAL_RCC_UART4_CLK_ENABLE();																//使能UART4时钟

        GPIO_Initure.Pin=GPIO_PIN_13;																//PH13(TX)
        GPIO_Initure.Mode=GPIO_MODE_AF_PP;															//复用推挽输出
        GPIO_Initure.Pull=GPIO_PULLUP;																//上拉
        GPIO_Initure.Speed=GPIO_SPEED_FAST;															//高速
        GPIO_Initure.Alternate=GPIO_AF8_UART4;														//复用为UART4
        HAL_GPIO_Init(GPIOH,&GPIO_Initure);	   														//初始化PH13

        GPIO_Initure.Pin=GPIO_PIN_14;																//PH14(RX)
        HAL_GPIO_Init(GPIOH,&GPIO_Initure);	   														//初始化PH14


        HAL_NVIC_EnableIRQ(UART4_IRQn);																//使能UART4中断通道
        HAL_NVIC_SetPriority(UART4_IRQn,3,0);														//抢占优先级3，子优先级0
    }
#endif

#if(EN_UART5_cfg   == 1)
    if(huart->Instance==UART5)																		//如果是串口5，进行串口5 MSP初始化
    {
        __HAL_RCC_GPIOB_CLK_ENABLE();																//使能GPIOB时钟
        __HAL_RCC_UART5_CLK_ENABLE();																//使能UART5时钟

        GPIO_Initure.Pin=GPIO_PIN_6;																//PB6(TX)
        GPIO_Initure.Mode=GPIO_MODE_AF_PP;															//复用推挽输出
        GPIO_Initure.Pull=GPIO_PULLUP;																//上拉
        GPIO_Initure.Speed=GPIO_SPEED_FAST;															//高速
        GPIO_Initure.Alternate=GPIO_AF8_UART5;														//复用为UART5
        HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   														//初始化PB6

        GPIO_Initure.Pin=GPIO_PIN_5;																//PB5(RX)
        HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   														//初始化PB5


        HAL_NVIC_EnableIRQ(UART5_IRQn);																//使能UART5中断通道
        HAL_NVIC_SetPriority(UART5_IRQn,3,0);														//抢占优先级3，子优先级0
    }
#endif

#if(EN_USART6_cfg   == 1)
    if(huart->Instance==USART6)																		//如果是串口6，进行串口6 MSP初始化
    {
        __HAL_RCC_GPIOC_CLK_ENABLE();																//使能GPIOC时钟
        __HAL_RCC_USART6_CLK_ENABLE();																//使能UART6时钟

        GPIO_Initure.Pin=GPIO_PIN_6;																//PC6(TX)
        GPIO_Initure.Mode=GPIO_MODE_AF_PP;															//复用推挽输出
        GPIO_Initure.Pull=GPIO_PULLUP;																//上拉
        GPIO_Initure.Speed=GPIO_SPEED_FAST;															//高速
        GPIO_Initure.Alternate=GPIO_AF8_USART6;														//复用为USART6
        HAL_GPIO_Init(GPIOC,&GPIO_Initure);	   														//初始化PC6

        GPIO_Initure.Pin=GPIO_PIN_7;																//PC7(RX)
        HAL_GPIO_Init(GPIOC,&GPIO_Initure);	   														//初始化PC7


        HAL_NVIC_EnableIRQ(USART6_IRQn);															//使能USART6中断通道
        HAL_NVIC_SetPriority(USART6_IRQn,3,0);														//抢占优先级3，子优先级0
    }
#endif

#if(EN_UART8_cfg   == 1)
    if(huart->Instance==UART8)																		//如果是串口8，进行串口8 MSP初始化
    {
        __HAL_RCC_GPIOE_CLK_ENABLE();																//使能GPIOC时钟
        __HAL_RCC_UART8_CLK_ENABLE();																//使能UART6时钟

        GPIO_Initure.Pin=GPIO_PIN_1;																//PE1(TX)
        GPIO_Initure.Mode=GPIO_MODE_AF_PP;															//复用推挽输出
        GPIO_Initure.Pull=GPIO_PULLUP;																//上拉
        GPIO_Initure.Speed=GPIO_SPEED_FAST;															//高速
        GPIO_Initure.Alternate=GPIO_AF8_UART8;														//复用为USART8
        HAL_GPIO_Init(GPIOE,&GPIO_Initure);	   														//初始化PE1

        GPIO_Initure.Pin=GPIO_PIN_0;																//PE0(RX)
        HAL_GPIO_Init(GPIOE,&GPIO_Initure);	   														//初始化PE0


        HAL_NVIC_EnableIRQ(UART8_IRQn);																//使能USART6中断通道
        HAL_NVIC_SetPriority(UART8_IRQn,3,0);														//抢占优先级3，子优先级0
    }
#endif
}

/**
 * @brief :	串口接收到一个字节
 * @param :	bsp_uart：bsp_uartXXX
 * @retval:	none.
 */
static void BSP_UartReceiveByte(BSP_Uart_type *bsp_uart)
{
    bsp_uart->timeout=0;
    bsp_uart->maxDelay=0x1FFFF;

    while (HAL_UART_GetState(&(bsp_uart->UART_Handler))!=HAL_UART_STATE_READY)						//等待就绪
    {
        bsp_uart->timeout++;																		////超时处理
        if(bsp_uart->timeout > bsp_uart->maxDelay) break;
    }

    bsp_uart->timeout=0;
    while(HAL_UART_Receive_IT(&(bsp_uart->UART_Handler),(uint8_t *)(bsp_uart->aRxBuffer), RXBUFFERSIZE)!=HAL_OK)	//一次处理完成之后，重新开启中断并设置RxXferCount为1
    {
        bsp_uart->timeout++; 																		//超时处理
        if(bsp_uart->timeout > bsp_uart->maxDelay) break;
    }
}

/**
 * @brief :	串口接收数据包
 * @param :	bsp_uart：bsp_uartXXX
 * @retval:	none.
 */
static void BSP_UartRecPack(BSP_Uart_type *bsp_uart)
{
    if((bsp_uart->UART_RX_STA&0x8000)==0)															//接收未完成
    {
        if(bsp_uart->UART_RX_STA&0x4000)															//接收到了0x0d
        {
            if(bsp_uart->aRxBuffer[0]!=0x0a)bsp_uart->UART_RX_STA=0;								//接收错误,重新开始
            else bsp_uart->UART_RX_STA|=0x8000;														//接收完成了
        }
        else 																						//还没收到0X0D
        {
            if(bsp_uart->aRxBuffer[0]==0x0d)bsp_uart->UART_RX_STA|=0x4000;
            else
            {
                bsp_uart->UART_RX_BUF[bsp_uart->UART_RX_STA&0X3FFF]=bsp_uart->aRxBuffer[0] ;
                bsp_uart->UART_RX_STA++;
                if(bsp_uart->UART_RX_STA>(USART_REC_LEN-1))bsp_uart->UART_RX_STA=0;					//接收数据错误,重新开始接收
            }
        }
    }
}


/**
 * @brief :	接收完成处理回调函数
 * @param :	huart：串口句柄
 * @retval:	none.
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

#if(EN_USART1_cfg   == 1)
    if(huart->Instance==USART1)																		//如果是串口1
    {
        //
        //操作bsp_uart1.aRxBuffer[0]
        //
        BSP_UartRecPack(&bsp_uart1);
    }
#endif

#if(EN_USART2_cfg   == 1)
    if(huart->Instance==USART2)																		//如果是串口2
    {
        //
        //操作bsp_uart2.aRxBuffer[0]
        //
        BSP_UartRecPack(&bsp_uart2);
    }
#endif

#if(EN_USART3_cfg   == 1)
    if(huart->Instance==USART3)																		//如果是串口3
    {
        //
        //操作bsp_uart3.aRxBuffer[0]
        //
        BSP_UartRecPack(&bsp_uart3);
    }
#endif

#if(EN_UART4_cfg   == 1)
    if(huart->Instance==UART4)																		//如果是串口4
    {
        //
        //操作bsp_uart4.aRxBuffer[0]
        //
        BSP_UartRecPack(&bsp_uart4);
    }
#endif

#if(EN_UART5_cfg   == 1)
    if(huart->Instance==UART5)																		//如果是串口5
    {
        //
        //操作bsp_uart5.aRxBuffer[0]
        //
        BSP_UartRecPack(&bsp_uart5);
    }
#endif

#if(EN_USART6_cfg   == 1)
    if(huart->Instance==USART6)																		//如果是串口6
    {
        //
        //操作bsp_uart6.aRxBuffer[0]
        //
        BSP_UartRecPack(&bsp_uart6);
    }
#endif

#if(EN_UART8_cfg   == 1)
    if(huart->Instance==UART8)																		//如果是串口8
    {
        //
        //操作bsp_uart8.aRxBuffer[0]
        //
        BSP_UartRecPack(&bsp_uart8);
    }
#endif
}

/**
 * @brief :	串口中断服务程序
 * @param :	none.
 * @retval:	none.
 */
//串口1中断服务程序
#if(EN_USART1_cfg   == 1)
void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&(bsp_uart1.UART_Handler));													//调用HAL库中断处理公用函数
    BSP_UartReceiveByte(&bsp_uart1);
}
#endif

//串口2中断服务程序
#if(EN_USART2_cfg   == 1)
void USART2_IRQHandler(void)
{
    HAL_UART_IRQHandler(&(bsp_uart2.UART_Handler));													//调用HAL库中断处理公用函数
    BSP_UartReceiveByte(&bsp_uart2);
}
#endif

//串口3中断服务程序
#if(EN_USART3_cfg   == 1)
void USART3_IRQHandler(void)
{
    HAL_UART_IRQHandler(&(bsp_uart3.UART_Handler));													//调用HAL库中断处理公用函数
    BSP_UartReceiveByte(&bsp_uart3);
}
#endif

//串口4中断服务程序
#if(EN_UART4_cfg   == 1)
void UART4_IRQHandler(void)
{
    HAL_UART_IRQHandler(&(bsp_uart4.UART_Handler));													//调用HAL库中断处理公用函数
    BSP_UartReceiveByte(&bsp_uart4);
}
#endif

//串口5中断服务程序
#if(EN_UART5_cfg   == 1)
void UART5_IRQHandler(void)
{
    HAL_UART_IRQHandler(&(bsp_uart5.UART_Handler));													//调用HAL库中断处理公用函数
    BSP_UartReceiveByte(&bsp_uart5);
}
#endif

//串口6中断服务程序
#if(EN_USART6_cfg   == 1)
void USART6_IRQHandler(void)
{
    HAL_UART_IRQHandler(&(bsp_uart6.UART_Handler));													//调用HAL库中断处理公用函数
    BSP_UartReceiveByte(&bsp_uart6);
}
#endif

//串口8中断服务程序
#if(EN_UART8_cfg   == 1)
void UART8_IRQHandler(void)
{
    HAL_UART_IRQHandler(&(bsp_uart8.UART_Handler));													//调用HAL库中断处理公用函数
    BSP_UartReceiveByte(&bsp_uart8);
}
#endif

/**
 * @brief :	串口测试（临时）串口接收到的数据用串口1打印出来
 * @param :	none.
 * @retval:	none.
 */
#include <string.h>
static void BSP_TestUart(BSP_Uart_type *bsp_uart)
{
    if(bsp_uart->UART_RX_STA&0x8000)
    {
        uint8_t len=bsp_uart->UART_RX_STA&0x3fff;													//得到此次接收到的数据长度
        printf ("接收到的数据为：%s\r\n",bsp_uart->UART_RX_BUF);
        memset(bsp_uart->UART_RX_BUF,0,len);														//清零
        bsp_uart->UART_RX_STA=0;
    }
}

/**
 * @brief :	串口测试（临时）APP调用
 * @param :	none.
 * @retval:	none.
 */
void APP_TestUart(void)
{
    BSP_TestUart(&bsp_uart1);
    BSP_TestUart(&bsp_uart2);
}
#endif
/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/
