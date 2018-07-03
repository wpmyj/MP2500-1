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
 * @file      bsp_can.c
 * @author    GuanXianDe
 * @version   V1.0.0
 * @date      15-June-2018
 * @brief     program entry
 * @attention none.
 *******************************************************************************
 */

/*
********************************************************************************
*                              INCLUDE FILES
********************************************************************************
*/
#include "bsp_can.h"
#include "FreeRTOS.h"
#include "task.h"
/*
********************************************************************************
*                             MACRO DEFINITIONS
********************************************************************************
*/
#if(USE_Can_File == 1)

#if((USE_Can_Conf == 1)&&(USE_Can1_Conf == 1))
#define EN_CAN1_cfg         				1   																										//ʹ��CAN1
#else 
#define EN_CAN1_cfg         				0   																										//ʧ��CAN1
#endif

#if((USE_Can_Conf == 1)&&(USE_Can2_Conf == 1))
#define EN_CAN2_cfg         				1   																										//ʹ��CAN2
#else 
#define EN_CAN2_cfg         				0   																										//ʧ��CAN2
#endif



//CAN�������ݷ���״̬
#define NOT_STARTED    					    	0                        																				//δ��ʼ�ռ�����
#define COLLECTTING     						1																										//�ռ�������
#define READY           						2																										//�ռ��������

#define PRGM_TX_BUFF_SIZE        				60
#define PRGM_RX_BUFF_SIZE         				16

//���
#if(EN_CAN1_cfg == 1)
CAN_HandleTypeDef    CAN1_Handler;   																													//CAN���
#endif
#if(EN_CAN2_cfg == 1)
CAN_HandleTypeDef    CAN2_Handler;   																													//CAN���
#endif


CanTxMsgTypeDef     							TxMessage;      																						//������Ϣ
CanRxMsgTypeDef     							RxMessage;      																						//������Ϣ



//��CAN�����еĽڵ�ID/��������ID,Ĭ��Ϊ255��,�������еĻ�������Ϊ0
uint8_t g_GlobalNetWorkId     =  0x1;//0xFF
/*
********************************************************************************
*                          OS-RELATED    VARIABLES
********************************************************************************
*/
typedef struct {

    uint16_t cob_id;  /**< message's  combine ID */
    uint8_t  rtr;     /**< remote transmission request. (0 if not rtr message, 1 if rtr message) */
    uint8_t  len;     /**< message's length (0 to 8) */
    uint8_t  data[8]; /**< message's datas */

} Message;
/*
********************************************************************************
*                             FUNCTION PROTOTYPES
********************************************************************************
*/
uint8_t BSP_CAN_Mode_Init(CAN_HandleTypeDef * can_HandleTypeDef,CAN_TypeDef * CANx);																	//CAN��ʼ��
#if(EN_CAN1_cfg == 1)
static uint8_t BSP_CAN_Send_Msg(CAN_HandleTypeDef *CAN_HandleType,Message *m);																			//��������


uint8_t CAN1_Receive_Msg(uint8_t *buf);																													//��������


void SetCanBusOnlineFlag(uint8_t i_NewStatus);
uint8_t GetCanBusOnlineFlag(void);


//���Դ�ӡ
void BSP_TestPrintf(CAN_HandleTypeDef *CAN_HandleType);
//can���ݽ������ռ�
void Can_ProtocolAnalysis(CAN_HandleTypeDef *CAN_HandleType);
#endif
/*
********************************************************************************
*                              LOCAL VARIABLES
********************************************************************************
*/
#if(EN_CAN1_cfg == 1)
static uint8_t  g_eCAN_BusOnLineFlag = 1;

static uint8_t  g_u8CanMsgRxBuff[PRGM_RX_BUFF_SIZE];																									//��CAN�ӿڽ��յ��Ĵ����ܵ����ݵĻ�����
static uint8_t  CanMsgRxStatu = NOT_STARTED;       																										//CAN���߽���״̬
#endif
/*
********************************************************************************
*                               GLOBAL VARIABLES
********************************************************************************
*/
#if(EN_CAN1_cfg == 1)
uint8_t     g_eCanMsgRxStatu = 0;																														//�Ƿ���յ���Ϣ

uint8_t     g_u8CanRxMsg[PRGM_RX_BUFF_SIZE];																											//���ܺ�CAN��������
#endif
#endif
/*
********************************************************************************
*                              FUNCTION DEFINITION
********************************************************************************
*/

/**
 * @brief :	��ʼ��CAN�ӿ�
 * @param :	none.
 * @retval:	none.
 */
void BSP_CanInit(void)
{
#if(USE_Can_Conf == 1)
#if(EN_CAN1_cfg == 1)
    BSP_CAN_Mode_Init(&CAN1_Handler,CAN1);																												//CAN1��ʼ��������50Kbps
#endif

#if(EN_CAN2_cfg == 1)
    BSP_CAN_Mode_Init(&CAN2_Handler,CAN2);																												//CAN1��ʼ��������50Kbps
#endif
#endif
}

#if(USE_Can_File == 1)
/**
 * @brief :	��ʼ��CAN�ӿ�����
 * @param :	can_HandleTypeDef�����
			CANx��CANx
 * @retval:	0���ɹ�
			1��CAN��ʼ��ʧ��
			2���˲�����ʼ��ʧ��

			tsjw:����ͬ����Ծʱ�䵥Ԫ.��Χ:CAN_SJW_1TQ~CAN_SJW_4TQ
			tbs2:ʱ���2��ʱ�䵥Ԫ.   ��Χ:CAN_BS2_1TQ~CAN_BS2_8TQ;
			tbs1:ʱ���1��ʱ�䵥Ԫ.   ��Χ:CAN_BS1_1TQ~CAN_BS1_16TQ
			brp :�����ʷ�Ƶ��.��Χ:1~1024; tq=(brp)*tpclk1
			������=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
			mode:CAN_MODE_NORMAL,��ͨģʽ;CAN_MODE_LOOPBACK,�ػ�ģʽ;
			Fpclk1��ʱ���ڳ�ʼ����ʱ������Ϊ54M,�������CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_11tq,6,CAN_MODE_LOOPBACK);
			������Ϊ:54M/((6+11+1)*60)=50Kbps
			����ֵ:0,��ʼ��OK;
			����,��ʼ��ʧ��;
 */
#if((EN_CAN1_cfg == 1)||(EN_CAN2_cfg == 1))
uint8_t BSP_CAN_Mode_Init(CAN_HandleTypeDef *can_HandleTypeDef,CAN_TypeDef * CANx)
{
    CAN_FilterConfTypeDef  CAN_FilerConf;

    can_HandleTypeDef->Instance=CANx;
    can_HandleTypeDef->pTxMsg=&TxMessage;     																						//������Ϣ
    can_HandleTypeDef->pRxMsg=&RxMessage;     																						//������Ϣ
    can_HandleTypeDef->Init.Prescaler=60;    																						//��Ƶϵ��(Fdiv)Ϊbrp+1
    can_HandleTypeDef->Init.Mode=CAN_MODE_NORMAL;        																			//ģʽ����
    can_HandleTypeDef->Init.SJW=CAN_SJW_1TQ;         																				//����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ CAN_SJW_1TQ~CAN_SJW_4TQ
    can_HandleTypeDef->Init.BS1=CAN_BS1_11TQ;         																				//tbs1��ΧCAN_BS1_1TQ~CAN_BS1_16TQ
    can_HandleTypeDef->Init.BS2=CAN_BS2_6TQ;         																				//tbs2��ΧCAN_BS2_1TQ~CAN_BS2_8TQ
    can_HandleTypeDef->Init.TTCM=DISABLE;     																						//��ʱ�䴥��ͨ��ģʽ
    can_HandleTypeDef->Init.ABOM=DISABLE;     																						//����Զ����߹���
    can_HandleTypeDef->Init.AWUM=ENABLE; 																							/* �Զ�����,�б�������ʱ���Զ��˳�����   *///DISABLE;     //˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
    can_HandleTypeDef->Init.NART=DISABLE; 																							/* �����ش�, �������һֱ�����ɹ�ֹ������ֻ��һ�� *///ENABLE;      //��ֹ�����Զ�����
    can_HandleTypeDef->Init.RFLM=DISABLE;     																						//���Ĳ�����,�µĸ��Ǿɵ�
    can_HandleTypeDef->Init.TXFP=ENABLE;  																							/* �������ȼ�  0---�ɱ�ʶ������  1---�ɷ�������˳�����   *///DISABLE;     //���ȼ��ɱ��ı�ʶ������
    if(HAL_CAN_Init(can_HandleTypeDef)!=HAL_OK) return 1;   																		//��ʼ��

    CAN_FilerConf.FilterIdHigh=((g_GlobalNetWorkId << 5) & 0xFFFF); 																//���˵����Ƿ��͸�����������֡//0X0000;     //32λID
    CAN_FilerConf.FilterIdLow=((CAN_ID_STD | CAN_RTR_DATA) & 0xFFFF); 																//ȷ���յ����Ǳ�׼����֡//0X0000;
    CAN_FilerConf.FilterMaskIdHigh=0xFFFF;																							//���е�λȫ������ƥ��//0X0000; //32λMASK
    CAN_FilerConf.FilterMaskIdLow=0xFFFF;																							//0X0000;
    CAN_FilerConf.FilterFIFOAssignment=CAN_FILTER_FIFO0;																			//������0������FIFO0
    CAN_FilerConf.FilterNumber=0;          																							//������0
    CAN_FilerConf.FilterMode=CAN_FILTERMODE_IDMASK;
    CAN_FilerConf.FilterScale=CAN_FILTERSCALE_32BIT;
    CAN_FilerConf.FilterActivation=ENABLE; 																							//�����˲���0
    CAN_FilerConf.BankNumber=14;
    if(HAL_CAN_ConfigFilter(can_HandleTypeDef,&CAN_FilerConf)!=HAL_OK) return 2;													//�˲�����ʼ��
    return 0;
}
#endif

/**
 * @brief :	CAN�ײ��������������ã�ʱ�����ã��ж����ã��˺����ᱻHAL_CAN_Init()���ã�
 * @param :	hcan:CAN���
 * @retval:	none.
 */
void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan)
{
#if(EN_CAN1_cfg == 1)
	{
    GPIO_InitTypeDef GPIO_Initure;
    if(hcan->Instance == CAN1)
    {
        __HAL_RCC_CAN1_CLK_ENABLE();                																				//ʹ��CAN1ʱ��
        __HAL_RCC_GPIOA_CLK_ENABLE();			    																				//����GPIOAʱ��

        GPIO_Initure.Pin=GPIO_PIN_11|GPIO_PIN_12;   																				//PA11,12
        GPIO_Initure.Mode=GPIO_MODE_AF_PP;          																				//���츴��
        GPIO_Initure.Pull=GPIO_PULLUP;              																				//����
        GPIO_Initure.Speed=GPIO_SPEED_FAST;         																				//����
        GPIO_Initure.Alternate=GPIO_AF9_CAN1;       																				//����ΪCAN1
        HAL_GPIO_Init(GPIOA,&GPIO_Initure);         																				//��ʼ��

        __HAL_CAN_ENABLE_IT(&CAN1_Handler,CAN_IT_FMP0);																				//FIFO0��Ϣ�Һ��ж�����.
        HAL_NVIC_SetPriority(CAN1_RX0_IRQn,1,0);    																				//��ռ���ȼ�1�������ȼ�0
        HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);          																				//ʹ���ж�

    }
	}
#endif

#if(EN_CAN2_cfg == 1)
	{
    GPIO_InitTypeDef GPIO_Initure;
    if(hcan->Instance == CAN2)
    {

        __HAL_RCC_CAN2_CLK_ENABLE();                																				//ʹ��CAN1ʱ��
        __HAL_RCC_GPIOB_CLK_ENABLE();			    																				//����GPIOBʱ��

        GPIO_Initure.Pin=GPIO_PIN_12|GPIO_PIN_13;   																				//PB12,13
        GPIO_Initure.Mode=GPIO_MODE_AF_PP;          																				//���츴��
        GPIO_Initure.Pull=GPIO_PULLUP;              																				//����
        GPIO_Initure.Speed=GPIO_SPEED_FAST;         																				//����
        GPIO_Initure.Alternate=GPIO_AF9_CAN2;       																				//����ΪCAN2
        HAL_GPIO_Init(GPIOB,&GPIO_Initure);         																				//��ʼ��

        __HAL_CAN_ENABLE_IT(&CAN2_Handler,CAN_IT_FMP0);																				//FIFO0��Ϣ�Һ��ж�����.
        HAL_NVIC_SetPriority(CAN2_RX0_IRQn,1,0);    																				//��ռ���ȼ�1�������ȼ�0
        HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);          																				//ʹ���ж�
    }
	}
#endif
}
/**
 * @brief :	CAN1 �жϷ�����
 * @param :	none.
 * @retval:	none.
 */
#if(EN_CAN1_cfg ==1)
void CAN1_RX0_IRQHandler(void)
{
    HAL_CAN_IRQHandler(&CAN1_Handler);																								//�˺��������CAN_Receive_IT()��������
}
#endif

/**
 * @brief :	CAN2 �жϷ�����
 * @param :	none.
 * @retval:	none.
 */
#if(EN_CAN2_cfg ==1)
void CAN2_RX0_IRQHandler(void)
{
    HAL_CAN_IRQHandler(&CAN2_Handler);																								//�˺��������CAN_Receive_IT()��������
}
#endif

/**
 * @brief :	CAN�жϴ�����̻ص��������˺����ᱻCAN_Receive_IT()���ã�
 * @param :	hcan:CAN���
 * @retval:	none.
 */
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan)
{
#if(EN_CAN1_cfg == 1)
    if(hcan->Instance == CAN1)
    {
        //CAN_Receive_IT()������ر�FIFO0��Ϣ�Һ��жϣ����������Ҫ���´�
        __HAL_CAN_ENABLE_IT(&CAN1_Handler,CAN_IT_FMP0);																				//���¿���FIF00��Ϣ�Һ��ж�

        BSP_TestPrintf(&CAN1_Handler);																								//���Դ�ӡ

        Can_ProtocolAnalysis(&CAN1_Handler);																					 	//Э��������ռ�
    }
#endif

#if(EN_CAN2_cfg == 1)
    if(hcan->Instance == CAN2)
    {
        //CAN_Receive_IT()������ر�FIFO0��Ϣ�Һ��жϣ����������Ҫ���´�
        __HAL_CAN_ENABLE_IT(&CAN2_Handler,CAN_IT_FMP0);																				//���¿���FIF00��Ϣ�Һ��ж�
    }
#endif
}

/**
 * @brief :	can����һ������
 * @param :	CAN_HandleType:CAN���
			m             :���͵����ݼ����Ƚṹ��
 * @retval:0,�ɹ�;
		   ����,ʧ��;
 */
#if(EN_CAN1_cfg == 1)
static uint8_t BSP_CAN_Send_Msg(CAN_HandleTypeDef *CAN_HandleType,Message *m)
{
    uint8_t i;
    CAN_HandleType->pTxMsg->StdId = (uint32_t)(m->cob_id);// ��׼��ʶ��11λ,���������ȼ�
    CAN_HandleType->pTxMsg->ExtId = 0x00;         // ������չ��ʾ��
    CAN_HandleType->pTxMsg->RTR = m->rtr;         //��Ϣ����Ϊ����֡��һ֡8λ
    CAN_HandleType->pTxMsg->IDE = CAN_ID_STD;     // ʹ�ñ�׼��ʶ��
    CAN_HandleType->pTxMsg->DLC = m->len;         // Ҫ���͵����ݳ���

    for(i = 0; i < m->len; i++) {
        CAN_HandleType->pTxMsg->Data[i] = m->data[i];
    }
    if(HAL_CAN_Transmit(CAN_HandleType,10)!=HAL_OK) return 1;     //����
    return 0;
}
#endif

/**
 * @brief :	The use of this funciton is concordance the node id inf to the can message.�ú�����ʹ���ǽ��ڵ�ID INF��CAN��Ϣһ�µġ�
*  @param :	i_Msglen:���ݳ���
			msg     :Ҫ���͵�����
			i_NodeId���ڵ�ID
 * @retval:	0,�ɹ�;
			����,ʧ��;
 */
#if(EN_CAN1_cfg == 1)
uint8_t SendCanMsgContainNodeId(uint32_t i_Msglen, uint8_t *msg, uint8_t i_NodeId)
{
    uint8_t ret;
    uint32_t i, j;
    uint8_t  u8SendErrCount = 0;
    uint8_t  LastTimeSendErrFlag = 1;
    Message ProcessedData;

    ProcessedData.cob_id = i_NodeId ;																									//�ϲ����ID
    ProcessedData.rtr = CAN_RTR_DATA;

    for(i = 0; i < (i_Msglen / 8 + ((i_Msglen % 8) ? 1 : 0)); i++)
    {   //����Ϣ����ķְ���,8ΪCAN��Ϣ�ı�׼������
        if(LastTimeSendErrFlag == 0)
        {
            ProcessedData.len = ((i_Msglen - i * 8) > 8) ? 8 : (i_Msglen - i * 8);														//ÿ���̰������ݳ��ȣ������һ����Ҫ�����⣬�����Ϊ8

            for(j = 0; j < ProcessedData.len; j++)
                ProcessedData.data[j] = msg[i * 8 + j];
        }
        else
        {
            //�ϴη��ͳ���ֱ�ӷ������鼴�ɣ�����Ҫ���¸���
        }

        ret = BSP_CAN_Send_Msg(&CAN1_Handler,&ProcessedData);

        if(ret != 0)
        {
            LastTimeSendErrFlag = 1;
            i--;
////////////////////            OSTimeDlyHMSM(0, 0, 0, 10,
////////////////////                          OS_OPT_TIME_HMSM_STRICT,
////////////////////                          &err);
            vTaskDelay(10);

            if(++u8SendErrCount >= 10)
            {
                printf("CAN bus has dropped,send err..\r\n");
                SetCanBusOnlineFlag(1);																									//CAN���ߵ���
                //
                //
////////////////////                StartTim6DelayTask(CAN_BUS_AUTO_RECONNECT_AFTER_5_SEC, 5000); 										//���ߺ�ʱ����Ƿ�����
                {
                    //��Ҫ����
                }
                break;
            }
        }
        else
        {
            LastTimeSendErrFlag = 0;																									//���ͳɹ�������ñ�־��������һ��ѭ�������͸�����
        }
    }

    return ret;
}


/**
 * @brief :	can1 �ڽ������ݲ�ѯ
 * @param :	buf:���ݻ�����;
 * @retval:	����ֵ:0,�����ݱ��յ�;
 */

uint8_t CAN1_Receive_Msg(uint8_t *buf)
{
    uint32_t i;
    if(HAL_CAN_Receive(&CAN1_Handler,CAN_FIFO0,0)!=HAL_OK) return 0;																	//��������
    for(i=0; i<CAN1_Handler.pRxMsg->DLC; i++)
        buf[i]=CAN1_Handler.pRxMsg->Data[i];
    return CAN1_Handler.pRxMsg->DLC;
}


/**
 * @brief :	����can��������״̬
 * @param :	none.
 * @retval:	none.
 */
void SetCanBusOnlineFlag(uint8_t i_NewStatus)
{
    g_eCAN_BusOnLineFlag = i_NewStatus;
}
/**
 * @brief :	��ȡcan��������״̬
 * @param :	none.
 * @retval:	none.
 */
uint8_t GetCanBusOnlineFlag(void)
{
    return g_eCAN_BusOnLineFlag;
}

/**
 * @brief :	���Դ�ӡcan����
 * @param :	CAN_HandleType��CAN���
 * @retval:	none.
 */
void BSP_TestPrintf(CAN_HandleTypeDef *CAN_HandleType)
{
    uint8_t i=0;
    printf("id:%d\r\n",CAN_HandleType->pRxMsg->StdId);
    printf("ide:%d\r\n",CAN_HandleType->pRxMsg->IDE);
    printf("rtr:%d\r\n",CAN_HandleType->pRxMsg->RTR);
    printf("len:%d\r\n",CAN_HandleType->pRxMsg->DLC);
    for(i=0; i<CAN_HandleType->pRxMsg->DLC; i++)
        printf("rxbuf[%d]:%d\r\n",i,CAN_HandleType->pRxMsg->Data[i]);
}

/**
 * @brief :	CANЭ�����
 * @param :	CAN_HandleType��CAN���
 * @retval:	none.
 */
void Can_ProtocolAnalysis(CAN_HandleTypeDef *CAN_HandleType)
{
    static   uint8_t i = 0;
    uint8_t  j = 0;

    if((CAN_HandleType->pRxMsg->DLC == 8)
            && (CAN_HandleType->pRxMsg->Data[0] == 0xFC)
            && (CAN_HandleType->pRxMsg->Data[1] == 0xFD)
            && (CAN_HandleType->pRxMsg->Data[2] == 0xFE))
    {   //�յ����Ǳ�ͷ֡����
        for(i = 0; i < CAN_HandleType->pRxMsg->DLC; i++) {
            g_u8CanMsgRxBuff[i] = CAN_HandleType->pRxMsg->Data[i];
        }
        CanMsgRxStatu = COLLECTTING;
    }
    else
    {   //�յ����ǷǱ�ͷ�ĺ���֡����
        if(CanMsgRxStatu == COLLECTTING) {																								//���򽫺������ֽڴ��뵽CAN��Ϣ������
            for(j = 0; j < CAN_HandleType->pRxMsg->DLC;)
            {
                g_u8CanMsgRxBuff[i] = CAN_HandleType->pRxMsg->Data[j];
                j++;
                i++;
                if(i >= PRGM_RX_BUFF_SIZE) {
                    CanMsgRxStatu = READY;																								//�����ռ����
                    break;
                }
            }
        } else { 																														//δ�ռ�����ʱ�յ���δ֪����
            CanMsgRxStatu = NOT_STARTED;
        }
    }

    if(CanMsgRxStatu == READY)
    {   //���ܺ�����ݰ�������ɺ��ٷ��͵���λ��
        if(g_eCanMsgRxStatu == 0)
        {
            taskENTER_CRITICAL();           																							//�����ٽ���

            for(j = 0; j < PRGM_RX_BUFF_SIZE; j++)
                g_u8CanRxMsg[j] = g_u8CanMsgRxBuff[j];

////////////////////            OSTimeDlyResume(&CommTaskTCB,
////////////////////                            &err);

            {
                //��Ҫ��ӻָ�ͨ������
            }
            g_eCanMsgRxStatu = 1;
            taskEXIT_CRITICAL();            																							 //�˳��ٽ���
        }
        CanMsgRxStatu = NOT_STARTED;
    }
    else
    {
        //����δ�����꣬�������
    }
}
#endif

#endif
/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/
