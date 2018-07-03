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
#define EN_USART1_cfg         1   												//ʹ�ܴ���1
#else
#define EN_USART1_cfg         0   												//ʧ�ܴ���1
#endif

#if(USE_Uart2_Conf)
#define EN_USART2_cfg         1   												//ʹ�ܴ���2
#else
#define EN_USART2_cfg         0   												//ʧ�ܴ���2
#endif

#if(USE_Uart3_Conf)
#define EN_USART3_cfg         1  	 											//ʹ�ܴ���3
#else
#define EN_USART3_cfg         0   												//ʧ�ܴ���3
#endif

#if(USE_Uart4_Conf)
#define EN_UART4_cfg          1   												//ʹ�ܴ���4
#else
#define EN_UART4_cfg          0   												//ʧ�ܴ���4
#endif

#if(USE_Uart5_Conf)
#define EN_UART5_cfg          1   												//ʹ�ܴ���5
#else
#define EN_UART5_cfg          0   												//ʧ�ܴ���5
#endif

#if(USE_Uart6_Conf)
#define EN_USART6_cfg         1   												//ʹ�ܴ���6
#else
#define EN_USART6_cfg         0   												//ʧ�ܴ���6
#endif

#if(USE_Uart8_Conf)
#define EN_UART8_cfg          1   												//ʹ�ܴ���8
#else
#define EN_UART8_cfg          0   												//ʧ�ܴ���8
#endif


#define USART_REC_LEN  			200  											//�����������ֽ��� 200

#define RXBUFFERSIZE   1 														//�����С
/*
********************************************************************************
*                          OS-RELATED    VARIABLES
********************************************************************************
*/
typedef struct
{
    uint8_t aRxBuffer[RXBUFFERSIZE];											//HAL��USART����Buffer
    UART_HandleTypeDef UART_Handler;    										//UART���
    uint32_t timeout;
    uint32_t maxDelay;

    uint16_t UART_RX_STA;
																				//����״̬
																				//bit15��	������ɱ�־
																				//bit14��	���յ�0x0d
																				//bit13~0��	���յ�����Ч�ֽ���Ŀ
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

//���ڲ���
static void BSP_TestUart(BSP_Uart_type *bsp_uart);
#endif
/*
********************************************************************************
*                              LOCAL VARIABLES
********************************************************************************
*/
#if(USE_Uart_File == 1)
uint8_t USART1_RX_BUF[USART_REC_LEN];     										//���ջ���,���USART_REC_LEN���ֽ�.
uint8_t USART2_RX_BUF[USART_REC_LEN];     										//���ջ���,���USART_REC_LEN���ֽ�.

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
 * @brief :	���ڳ�ʼ��
 * @param :	none.
 * @retval:	none.
 */
void BSP_UartInit(void)		        											//���ڳ�ʼ��
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
 * @brief :	���ڵײ��ʼ��
 * @param :	bsp_uart ��bsp_uartXXX
			USART    : UARTXXX
			BaudRate : ������
			Buf      ��ָ�����ջ�����
 * @retval:	none.
 */
#if((USE_Uart_File == 1)&&(USE_Uart_Conf == 1))
static void bsp_uart_init(BSP_Uart_type *bsp_uart,USART_TypeDef * USART,uint32_t BaudRate,uint8_t *Buf)
{
																									//UART ��ʼ������
    bsp_uart->UART_Handler.Instance=USART;					    									//USARTx

    bsp_uart->UART_Handler.Init.BaudRate=BaudRate;				    								//������
    bsp_uart->UART_Handler.Init.WordLength=UART_WORDLENGTH_8B;   									//�ֳ�Ϊ8λ���ݸ�ʽ
    bsp_uart->UART_Handler.Init.StopBits=UART_STOPBITS_1;	    									//һ��ֹͣλ
    bsp_uart->UART_Handler.Init.Parity=UART_PARITY_NONE;		    								//����żУ��λ
    bsp_uart->UART_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   									//��Ӳ������
    bsp_uart->UART_Handler.Init.Mode=UART_MODE_TX_RX;		    									//�շ�ģʽ
    HAL_UART_Init(&(bsp_uart->UART_Handler));					    								//HAL_UART_Init()��ʹ��UART1

    HAL_UART_Receive_IT(&(bsp_uart->UART_Handler), (uint8_t *)(bsp_uart->aRxBuffer), RXBUFFERSIZE);	//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������

    bsp_uart->UART_RX_BUF = Buf;
}

/**
 * @brief :	UART�ײ��ʼ����ʱ��ʹ�ܣ��������ã��ж����ã��˺����ᱻHAL_UART_Init()���ã�
 * @param :	huart:���ھ��
 * @retval:	none.
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_Initure;
#if(EN_USART1_cfg   == 1)
    if(huart->Instance==USART1)																		//����Ǵ���1�����д���1 MSP��ʼ��
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();																//ʹ��GPIOAʱ��
        __HAL_RCC_USART1_CLK_ENABLE();																//ʹ��USART1ʱ��
        {
																									//��Ҫ�޸�Ϊ��ӳ�䵽PB14(TX),PB15(RX)
            GPIO_Initure.Pin=GPIO_PIN_9;															//PA9(TX)
            GPIO_Initure.Mode=GPIO_MODE_AF_PP;														//�����������
            GPIO_Initure.Pull=GPIO_PULLUP;															//����
            GPIO_Initure.Speed=GPIO_SPEED_FAST;														//����
            GPIO_Initure.Alternate=GPIO_AF7_USART1;													//����ΪUSART1
            HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   													//��ʼ��PA9

            GPIO_Initure.Pin=GPIO_PIN_10;															//PA10(RX)
            HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   													//��ʼ��PA10
        }

        HAL_NVIC_EnableIRQ(USART1_IRQn);															//ʹ��USART1�ж�ͨ��
        HAL_NVIC_SetPriority(USART1_IRQn,4,0);														//��ռ���ȼ�3�������ȼ�0
    }
#endif


#if(EN_USART2_cfg   == 1)
    if(huart->Instance==USART2)																		//����Ǵ���2�����д���2 MSP��ʼ��
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();																//ʹ��GPIOAʱ��
        __HAL_RCC_USART2_CLK_ENABLE();																//ʹ��USART2ʱ��

        GPIO_Initure.Pin=GPIO_PIN_2;																//PA2(TX)
        GPIO_Initure.Mode=GPIO_MODE_AF_PP;															//�����������
        GPIO_Initure.Pull=GPIO_PULLUP;																//����
        GPIO_Initure.Speed=GPIO_SPEED_FAST;															//����
        GPIO_Initure.Alternate=GPIO_AF7_USART2;														//����ΪUSART2
        HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   														//��ʼ��PA2

        GPIO_Initure.Pin=GPIO_PIN_3;																//PA3(RX)
        HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   														//��ʼ��PA3


        HAL_NVIC_EnableIRQ(USART2_IRQn);															//ʹ��USART2�ж�ͨ��
        HAL_NVIC_SetPriority(USART2_IRQn,3,0);														//��ռ���ȼ�3�������ȼ�0
    }
#endif

#if(EN_USART3_cfg   == 1)
    if(huart->Instance==USART3)																		//����Ǵ���3�����д���3 MSP��ʼ��
    {
        __HAL_RCC_GPIOB_CLK_ENABLE();																//ʹ��GPIOBʱ��
        __HAL_RCC_USART3_CLK_ENABLE();																//ʹ��USART3ʱ��

        GPIO_Initure.Pin=GPIO_PIN_10;																//PB10(TX)
        GPIO_Initure.Mode=GPIO_MODE_AF_PP;															//�����������
        GPIO_Initure.Pull=GPIO_PULLUP;																//����
        GPIO_Initure.Speed=GPIO_SPEED_FAST;															//����
        GPIO_Initure.Alternate=GPIO_AF7_USART3;														//����ΪUSART3
        HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   														//��ʼ��PB10

        GPIO_Initure.Pin=GPIO_PIN_11;																//PB11(RX)
        HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   														//��ʼ��PB11


        HAL_NVIC_EnableIRQ(USART3_IRQn);															//ʹ��USART3�ж�ͨ��
        HAL_NVIC_SetPriority(USART3_IRQn,3,0);														//��ռ���ȼ�3�������ȼ�0
    }
#endif

#if(EN_UART4_cfg   == 1)
    if(huart->Instance==UART4)																		//����Ǵ���4�����д���4 MSP��ʼ��
    {
        __HAL_RCC_GPIOH_CLK_ENABLE();																//ʹ��GPIOHʱ��
        __HAL_RCC_UART4_CLK_ENABLE();																//ʹ��UART4ʱ��

        GPIO_Initure.Pin=GPIO_PIN_13;																//PH13(TX)
        GPIO_Initure.Mode=GPIO_MODE_AF_PP;															//�����������
        GPIO_Initure.Pull=GPIO_PULLUP;																//����
        GPIO_Initure.Speed=GPIO_SPEED_FAST;															//����
        GPIO_Initure.Alternate=GPIO_AF8_UART4;														//����ΪUART4
        HAL_GPIO_Init(GPIOH,&GPIO_Initure);	   														//��ʼ��PH13

        GPIO_Initure.Pin=GPIO_PIN_14;																//PH14(RX)
        HAL_GPIO_Init(GPIOH,&GPIO_Initure);	   														//��ʼ��PH14


        HAL_NVIC_EnableIRQ(UART4_IRQn);																//ʹ��UART4�ж�ͨ��
        HAL_NVIC_SetPriority(UART4_IRQn,3,0);														//��ռ���ȼ�3�������ȼ�0
    }
#endif

#if(EN_UART5_cfg   == 1)
    if(huart->Instance==UART5)																		//����Ǵ���5�����д���5 MSP��ʼ��
    {
        __HAL_RCC_GPIOB_CLK_ENABLE();																//ʹ��GPIOBʱ��
        __HAL_RCC_UART5_CLK_ENABLE();																//ʹ��UART5ʱ��

        GPIO_Initure.Pin=GPIO_PIN_6;																//PB6(TX)
        GPIO_Initure.Mode=GPIO_MODE_AF_PP;															//�����������
        GPIO_Initure.Pull=GPIO_PULLUP;																//����
        GPIO_Initure.Speed=GPIO_SPEED_FAST;															//����
        GPIO_Initure.Alternate=GPIO_AF8_UART5;														//����ΪUART5
        HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   														//��ʼ��PB6

        GPIO_Initure.Pin=GPIO_PIN_5;																//PB5(RX)
        HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   														//��ʼ��PB5


        HAL_NVIC_EnableIRQ(UART5_IRQn);																//ʹ��UART5�ж�ͨ��
        HAL_NVIC_SetPriority(UART5_IRQn,3,0);														//��ռ���ȼ�3�������ȼ�0
    }
#endif

#if(EN_USART6_cfg   == 1)
    if(huart->Instance==USART6)																		//����Ǵ���6�����д���6 MSP��ʼ��
    {
        __HAL_RCC_GPIOC_CLK_ENABLE();																//ʹ��GPIOCʱ��
        __HAL_RCC_USART6_CLK_ENABLE();																//ʹ��UART6ʱ��

        GPIO_Initure.Pin=GPIO_PIN_6;																//PC6(TX)
        GPIO_Initure.Mode=GPIO_MODE_AF_PP;															//�����������
        GPIO_Initure.Pull=GPIO_PULLUP;																//����
        GPIO_Initure.Speed=GPIO_SPEED_FAST;															//����
        GPIO_Initure.Alternate=GPIO_AF8_USART6;														//����ΪUSART6
        HAL_GPIO_Init(GPIOC,&GPIO_Initure);	   														//��ʼ��PC6

        GPIO_Initure.Pin=GPIO_PIN_7;																//PC7(RX)
        HAL_GPIO_Init(GPIOC,&GPIO_Initure);	   														//��ʼ��PC7


        HAL_NVIC_EnableIRQ(USART6_IRQn);															//ʹ��USART6�ж�ͨ��
        HAL_NVIC_SetPriority(USART6_IRQn,3,0);														//��ռ���ȼ�3�������ȼ�0
    }
#endif

#if(EN_UART8_cfg   == 1)
    if(huart->Instance==UART8)																		//����Ǵ���8�����д���8 MSP��ʼ��
    {
        __HAL_RCC_GPIOE_CLK_ENABLE();																//ʹ��GPIOCʱ��
        __HAL_RCC_UART8_CLK_ENABLE();																//ʹ��UART6ʱ��

        GPIO_Initure.Pin=GPIO_PIN_1;																//PE1(TX)
        GPIO_Initure.Mode=GPIO_MODE_AF_PP;															//�����������
        GPIO_Initure.Pull=GPIO_PULLUP;																//����
        GPIO_Initure.Speed=GPIO_SPEED_FAST;															//����
        GPIO_Initure.Alternate=GPIO_AF8_UART8;														//����ΪUSART8
        HAL_GPIO_Init(GPIOE,&GPIO_Initure);	   														//��ʼ��PE1

        GPIO_Initure.Pin=GPIO_PIN_0;																//PE0(RX)
        HAL_GPIO_Init(GPIOE,&GPIO_Initure);	   														//��ʼ��PE0


        HAL_NVIC_EnableIRQ(UART8_IRQn);																//ʹ��USART6�ж�ͨ��
        HAL_NVIC_SetPriority(UART8_IRQn,3,0);														//��ռ���ȼ�3�������ȼ�0
    }
#endif
}

/**
 * @brief :	���ڽ��յ�һ���ֽ�
 * @param :	bsp_uart��bsp_uartXXX
 * @retval:	none.
 */
static void BSP_UartReceiveByte(BSP_Uart_type *bsp_uart)
{
    bsp_uart->timeout=0;
    bsp_uart->maxDelay=0x1FFFF;

    while (HAL_UART_GetState(&(bsp_uart->UART_Handler))!=HAL_UART_STATE_READY)						//�ȴ�����
    {
        bsp_uart->timeout++;																		////��ʱ����
        if(bsp_uart->timeout > bsp_uart->maxDelay) break;
    }

    bsp_uart->timeout=0;
    while(HAL_UART_Receive_IT(&(bsp_uart->UART_Handler),(uint8_t *)(bsp_uart->aRxBuffer), RXBUFFERSIZE)!=HAL_OK)	//һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
    {
        bsp_uart->timeout++; 																		//��ʱ����
        if(bsp_uart->timeout > bsp_uart->maxDelay) break;
    }
}

/**
 * @brief :	���ڽ������ݰ�
 * @param :	bsp_uart��bsp_uartXXX
 * @retval:	none.
 */
static void BSP_UartRecPack(BSP_Uart_type *bsp_uart)
{
    if((bsp_uart->UART_RX_STA&0x8000)==0)															//����δ���
    {
        if(bsp_uart->UART_RX_STA&0x4000)															//���յ���0x0d
        {
            if(bsp_uart->aRxBuffer[0]!=0x0a)bsp_uart->UART_RX_STA=0;								//���մ���,���¿�ʼ
            else bsp_uart->UART_RX_STA|=0x8000;														//���������
        }
        else 																						//��û�յ�0X0D
        {
            if(bsp_uart->aRxBuffer[0]==0x0d)bsp_uart->UART_RX_STA|=0x4000;
            else
            {
                bsp_uart->UART_RX_BUF[bsp_uart->UART_RX_STA&0X3FFF]=bsp_uart->aRxBuffer[0] ;
                bsp_uart->UART_RX_STA++;
                if(bsp_uart->UART_RX_STA>(USART_REC_LEN-1))bsp_uart->UART_RX_STA=0;					//�������ݴ���,���¿�ʼ����
            }
        }
    }
}


/**
 * @brief :	������ɴ���ص�����
 * @param :	huart�����ھ��
 * @retval:	none.
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

#if(EN_USART1_cfg   == 1)
    if(huart->Instance==USART1)																		//����Ǵ���1
    {
        //
        //����bsp_uart1.aRxBuffer[0]
        //
        BSP_UartRecPack(&bsp_uart1);
    }
#endif

#if(EN_USART2_cfg   == 1)
    if(huart->Instance==USART2)																		//����Ǵ���2
    {
        //
        //����bsp_uart2.aRxBuffer[0]
        //
        BSP_UartRecPack(&bsp_uart2);
    }
#endif

#if(EN_USART3_cfg   == 1)
    if(huart->Instance==USART3)																		//����Ǵ���3
    {
        //
        //����bsp_uart3.aRxBuffer[0]
        //
        BSP_UartRecPack(&bsp_uart3);
    }
#endif

#if(EN_UART4_cfg   == 1)
    if(huart->Instance==UART4)																		//����Ǵ���4
    {
        //
        //����bsp_uart4.aRxBuffer[0]
        //
        BSP_UartRecPack(&bsp_uart4);
    }
#endif

#if(EN_UART5_cfg   == 1)
    if(huart->Instance==UART5)																		//����Ǵ���5
    {
        //
        //����bsp_uart5.aRxBuffer[0]
        //
        BSP_UartRecPack(&bsp_uart5);
    }
#endif

#if(EN_USART6_cfg   == 1)
    if(huart->Instance==USART6)																		//����Ǵ���6
    {
        //
        //����bsp_uart6.aRxBuffer[0]
        //
        BSP_UartRecPack(&bsp_uart6);
    }
#endif

#if(EN_UART8_cfg   == 1)
    if(huart->Instance==UART8)																		//����Ǵ���8
    {
        //
        //����bsp_uart8.aRxBuffer[0]
        //
        BSP_UartRecPack(&bsp_uart8);
    }
#endif
}

/**
 * @brief :	�����жϷ������
 * @param :	none.
 * @retval:	none.
 */
//����1�жϷ������
#if(EN_USART1_cfg   == 1)
void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&(bsp_uart1.UART_Handler));													//����HAL���жϴ����ú���
    BSP_UartReceiveByte(&bsp_uart1);
}
#endif

//����2�жϷ������
#if(EN_USART2_cfg   == 1)
void USART2_IRQHandler(void)
{
    HAL_UART_IRQHandler(&(bsp_uart2.UART_Handler));													//����HAL���жϴ����ú���
    BSP_UartReceiveByte(&bsp_uart2);
}
#endif

//����3�жϷ������
#if(EN_USART3_cfg   == 1)
void USART3_IRQHandler(void)
{
    HAL_UART_IRQHandler(&(bsp_uart3.UART_Handler));													//����HAL���жϴ����ú���
    BSP_UartReceiveByte(&bsp_uart3);
}
#endif

//����4�жϷ������
#if(EN_UART4_cfg   == 1)
void UART4_IRQHandler(void)
{
    HAL_UART_IRQHandler(&(bsp_uart4.UART_Handler));													//����HAL���жϴ����ú���
    BSP_UartReceiveByte(&bsp_uart4);
}
#endif

//����5�жϷ������
#if(EN_UART5_cfg   == 1)
void UART5_IRQHandler(void)
{
    HAL_UART_IRQHandler(&(bsp_uart5.UART_Handler));													//����HAL���жϴ����ú���
    BSP_UartReceiveByte(&bsp_uart5);
}
#endif

//����6�жϷ������
#if(EN_USART6_cfg   == 1)
void USART6_IRQHandler(void)
{
    HAL_UART_IRQHandler(&(bsp_uart6.UART_Handler));													//����HAL���жϴ����ú���
    BSP_UartReceiveByte(&bsp_uart6);
}
#endif

//����8�жϷ������
#if(EN_UART8_cfg   == 1)
void UART8_IRQHandler(void)
{
    HAL_UART_IRQHandler(&(bsp_uart8.UART_Handler));													//����HAL���жϴ����ú���
    BSP_UartReceiveByte(&bsp_uart8);
}
#endif

/**
 * @brief :	���ڲ��ԣ���ʱ�����ڽ��յ��������ô���1��ӡ����
 * @param :	none.
 * @retval:	none.
 */
#include <string.h>
static void BSP_TestUart(BSP_Uart_type *bsp_uart)
{
    if(bsp_uart->UART_RX_STA&0x8000)
    {
        uint8_t len=bsp_uart->UART_RX_STA&0x3fff;													//�õ��˴ν��յ������ݳ���
        printf ("���յ�������Ϊ��%s\r\n",bsp_uart->UART_RX_BUF);
        memset(bsp_uart->UART_RX_BUF,0,len);														//����
        bsp_uart->UART_RX_STA=0;
    }
}

/**
 * @brief :	���ڲ��ԣ���ʱ��APP����
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
