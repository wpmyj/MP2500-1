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
#define EN_CAN1_cfg         				1   																										//使能CAN1
#else 
#define EN_CAN1_cfg         				0   																										//失能CAN1
#endif

#if((USE_Can_Conf == 1)&&(USE_Can2_Conf == 1))
#define EN_CAN2_cfg         				1   																										//使能CAN2
#else 
#define EN_CAN2_cfg         				0   																										//失能CAN2
#endif



//CAN总线数据发送状态
#define NOT_STARTED    					    	0                        																				//未开始收集数据
#define COLLECTTING     						1																										//收集数据中
#define READY           						2																										//收集数据完成

#define PRGM_TX_BUFF_SIZE        				60
#define PRGM_RX_BUFF_SIZE         				16

//句柄
#if(EN_CAN1_cfg == 1)
CAN_HandleTypeDef    CAN1_Handler;   																													//CAN句柄
#endif
#if(EN_CAN2_cfg == 1)
CAN_HandleTypeDef    CAN2_Handler;   																													//CAN句柄
#endif


CanTxMsgTypeDef     							TxMessage;      																						//发送消息
CanRxMsgTypeDef     							RxMessage;      																						//接收消息



//在CAN总线中的节点ID/本地组网ID,默认为255号,独立运行的机器设置为0
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
uint8_t BSP_CAN_Mode_Init(CAN_HandleTypeDef * can_HandleTypeDef,CAN_TypeDef * CANx);																	//CAN初始化
#if(EN_CAN1_cfg == 1)
static uint8_t BSP_CAN_Send_Msg(CAN_HandleTypeDef *CAN_HandleType,Message *m);																			//发送数据


uint8_t CAN1_Receive_Msg(uint8_t *buf);																													//接收数据


void SetCanBusOnlineFlag(uint8_t i_NewStatus);
uint8_t GetCanBusOnlineFlag(void);


//测试打印
void BSP_TestPrintf(CAN_HandleTypeDef *CAN_HandleType);
//can数据解析及收集
void Can_ProtocolAnalysis(CAN_HandleTypeDef *CAN_HandleType);
#endif
/*
********************************************************************************
*                              LOCAL VARIABLES
********************************************************************************
*/
#if(EN_CAN1_cfg == 1)
static uint8_t  g_eCAN_BusOnLineFlag = 1;

static uint8_t  g_u8CanMsgRxBuff[PRGM_RX_BUFF_SIZE];																									//从CAN接口接收到的待汇总的数据的缓冲区
static uint8_t  CanMsgRxStatu = NOT_STARTED;       																										//CAN总线接收状态
#endif
/*
********************************************************************************
*                               GLOBAL VARIABLES
********************************************************************************
*/
#if(EN_CAN1_cfg == 1)
uint8_t     g_eCanMsgRxStatu = 0;																														//是否接收到信息

uint8_t     g_u8CanRxMsg[PRGM_RX_BUFF_SIZE];																											//汇总后CAN接收数据
#endif
#endif
/*
********************************************************************************
*                              FUNCTION DEFINITION
********************************************************************************
*/

/**
 * @brief :	初始化CAN接口
 * @param :	none.
 * @retval:	none.
 */
void BSP_CanInit(void)
{
#if(USE_Can_Conf == 1)
#if(EN_CAN1_cfg == 1)
    BSP_CAN_Mode_Init(&CAN1_Handler,CAN1);																												//CAN1初始化波特率50Kbps
#endif

#if(EN_CAN2_cfg == 1)
    BSP_CAN_Mode_Init(&CAN2_Handler,CAN2);																												//CAN1初始化波特率50Kbps
#endif
#endif
}

#if(USE_Can_File == 1)
/**
 * @brief :	初始化CAN接口配置
 * @param :	can_HandleTypeDef：句柄
			CANx：CANx
 * @retval:	0：成功
			1：CAN初始化失败
			2：滤波器初始化失败

			tsjw:重新同步跳跃时间单元.范围:CAN_SJW_1TQ~CAN_SJW_4TQ
			tbs2:时间段2的时间单元.   范围:CAN_BS2_1TQ~CAN_BS2_8TQ;
			tbs1:时间段1的时间单元.   范围:CAN_BS1_1TQ~CAN_BS1_16TQ
			brp :波特率分频器.范围:1~1024; tq=(brp)*tpclk1
			波特率=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
			mode:CAN_MODE_NORMAL,普通模式;CAN_MODE_LOOPBACK,回环模式;
			Fpclk1的时钟在初始化的时候设置为54M,如果设置CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_11tq,6,CAN_MODE_LOOPBACK);
			则波特率为:54M/((6+11+1)*60)=50Kbps
			返回值:0,初始化OK;
			其他,初始化失败;
 */
#if((EN_CAN1_cfg == 1)||(EN_CAN2_cfg == 1))
uint8_t BSP_CAN_Mode_Init(CAN_HandleTypeDef *can_HandleTypeDef,CAN_TypeDef * CANx)
{
    CAN_FilterConfTypeDef  CAN_FilerConf;

    can_HandleTypeDef->Instance=CANx;
    can_HandleTypeDef->pTxMsg=&TxMessage;     																						//发送消息
    can_HandleTypeDef->pRxMsg=&RxMessage;     																						//接收消息
    can_HandleTypeDef->Init.Prescaler=60;    																						//分频系数(Fdiv)为brp+1
    can_HandleTypeDef->Init.Mode=CAN_MODE_NORMAL;        																			//模式设置
    can_HandleTypeDef->Init.SJW=CAN_SJW_1TQ;         																				//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1TQ~CAN_SJW_4TQ
    can_HandleTypeDef->Init.BS1=CAN_BS1_11TQ;         																				//tbs1范围CAN_BS1_1TQ~CAN_BS1_16TQ
    can_HandleTypeDef->Init.BS2=CAN_BS2_6TQ;         																				//tbs2范围CAN_BS2_1TQ~CAN_BS2_8TQ
    can_HandleTypeDef->Init.TTCM=DISABLE;     																						//非时间触发通信模式
    can_HandleTypeDef->Init.ABOM=DISABLE;     																						//软件自动离线管理
    can_HandleTypeDef->Init.AWUM=ENABLE; 																							/* 自动唤醒,有报文来的时候自动退出休眠   *///DISABLE;     //睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
    can_HandleTypeDef->Init.NART=DISABLE; 																							/* 报文重传, 如果错误一直传到成功止，否则只传一次 *///ENABLE;      //禁止报文自动传送
    can_HandleTypeDef->Init.RFLM=DISABLE;     																						//报文不锁定,新的覆盖旧的
    can_HandleTypeDef->Init.TXFP=ENABLE;  																							/* 发送优先级  0---由标识符决定  1---由发送请求顺序决定   *///DISABLE;     //优先级由报文标识符决定
    if(HAL_CAN_Init(can_HandleTypeDef)!=HAL_OK) return 1;   																		//初始化

    CAN_FilerConf.FilterIdHigh=((g_GlobalNetWorkId << 5) & 0xFFFF); 																//过滤掉不是发送给本机的数据帧//0X0000;     //32位ID
    CAN_FilerConf.FilterIdLow=((CAN_ID_STD | CAN_RTR_DATA) & 0xFFFF); 																//确保收到的是标准数据帧//0X0000;
    CAN_FilerConf.FilterMaskIdHigh=0xFFFF;																							//所有的位全部必须匹配//0X0000; //32位MASK
    CAN_FilerConf.FilterMaskIdLow=0xFFFF;																							//0X0000;
    CAN_FilerConf.FilterFIFOAssignment=CAN_FILTER_FIFO0;																			//过滤器0关联到FIFO0
    CAN_FilerConf.FilterNumber=0;          																							//过滤器0
    CAN_FilerConf.FilterMode=CAN_FILTERMODE_IDMASK;
    CAN_FilerConf.FilterScale=CAN_FILTERSCALE_32BIT;
    CAN_FilerConf.FilterActivation=ENABLE; 																							//激活滤波器0
    CAN_FilerConf.BankNumber=14;
    if(HAL_CAN_ConfigFilter(can_HandleTypeDef,&CAN_FilerConf)!=HAL_OK) return 2;													//滤波器初始化
    return 0;
}
#endif

/**
 * @brief :	CAN底层驱动，引脚配置，时钟配置，中断配置（此函数会被HAL_CAN_Init()调用）
 * @param :	hcan:CAN句柄
 * @retval:	none.
 */
void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan)
{
#if(EN_CAN1_cfg == 1)
	{
    GPIO_InitTypeDef GPIO_Initure;
    if(hcan->Instance == CAN1)
    {
        __HAL_RCC_CAN1_CLK_ENABLE();                																				//使能CAN1时钟
        __HAL_RCC_GPIOA_CLK_ENABLE();			    																				//开启GPIOA时钟

        GPIO_Initure.Pin=GPIO_PIN_11|GPIO_PIN_12;   																				//PA11,12
        GPIO_Initure.Mode=GPIO_MODE_AF_PP;          																				//推挽复用
        GPIO_Initure.Pull=GPIO_PULLUP;              																				//上拉
        GPIO_Initure.Speed=GPIO_SPEED_FAST;         																				//快速
        GPIO_Initure.Alternate=GPIO_AF9_CAN1;       																				//复用为CAN1
        HAL_GPIO_Init(GPIOA,&GPIO_Initure);         																				//初始化

        __HAL_CAN_ENABLE_IT(&CAN1_Handler,CAN_IT_FMP0);																				//FIFO0消息挂号中断允许.
        HAL_NVIC_SetPriority(CAN1_RX0_IRQn,1,0);    																				//抢占优先级1，子优先级0
        HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);          																				//使能中断

    }
	}
#endif

#if(EN_CAN2_cfg == 1)
	{
    GPIO_InitTypeDef GPIO_Initure;
    if(hcan->Instance == CAN2)
    {

        __HAL_RCC_CAN2_CLK_ENABLE();                																				//使能CAN1时钟
        __HAL_RCC_GPIOB_CLK_ENABLE();			    																				//开启GPIOB时钟

        GPIO_Initure.Pin=GPIO_PIN_12|GPIO_PIN_13;   																				//PB12,13
        GPIO_Initure.Mode=GPIO_MODE_AF_PP;          																				//推挽复用
        GPIO_Initure.Pull=GPIO_PULLUP;              																				//上拉
        GPIO_Initure.Speed=GPIO_SPEED_FAST;         																				//快速
        GPIO_Initure.Alternate=GPIO_AF9_CAN2;       																				//复用为CAN2
        HAL_GPIO_Init(GPIOB,&GPIO_Initure);         																				//初始化

        __HAL_CAN_ENABLE_IT(&CAN2_Handler,CAN_IT_FMP0);																				//FIFO0消息挂号中断允许.
        HAL_NVIC_SetPriority(CAN2_RX0_IRQn,1,0);    																				//抢占优先级1，子优先级0
        HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);          																				//使能中断
    }
	}
#endif
}
/**
 * @brief :	CAN1 中断服务函数
 * @param :	none.
 * @retval:	none.
 */
#if(EN_CAN1_cfg ==1)
void CAN1_RX0_IRQHandler(void)
{
    HAL_CAN_IRQHandler(&CAN1_Handler);																								//此函数会调用CAN_Receive_IT()接收数据
}
#endif

/**
 * @brief :	CAN2 中断服务函数
 * @param :	none.
 * @retval:	none.
 */
#if(EN_CAN2_cfg ==1)
void CAN2_RX0_IRQHandler(void)
{
    HAL_CAN_IRQHandler(&CAN2_Handler);																								//此函数会调用CAN_Receive_IT()接收数据
}
#endif

/**
 * @brief :	CAN中断处理过程回调函数（此函数会被CAN_Receive_IT()调用）
 * @param :	hcan:CAN句柄
 * @retval:	none.
 */
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan)
{
#if(EN_CAN1_cfg == 1)
    if(hcan->Instance == CAN1)
    {
        //CAN_Receive_IT()函数会关闭FIFO0消息挂号中断，因此我们需要重新打开
        __HAL_CAN_ENABLE_IT(&CAN1_Handler,CAN_IT_FMP0);																				//重新开启FIF00消息挂号中断

        BSP_TestPrintf(&CAN1_Handler);																								//测试打印

        Can_ProtocolAnalysis(&CAN1_Handler);																					 	//协议解析及收集
    }
#endif

#if(EN_CAN2_cfg == 1)
    if(hcan->Instance == CAN2)
    {
        //CAN_Receive_IT()函数会关闭FIFO0消息挂号中断，因此我们需要重新打开
        __HAL_CAN_ENABLE_IT(&CAN2_Handler,CAN_IT_FMP0);																				//重新开启FIF00消息挂号中断
    }
#endif
}

/**
 * @brief :	can发送一组数据
 * @param :	CAN_HandleType:CAN句柄
			m             :发送的数据及长度结构体
 * @retval:0,成功;
		   其他,失败;
 */
#if(EN_CAN1_cfg == 1)
static uint8_t BSP_CAN_Send_Msg(CAN_HandleTypeDef *CAN_HandleType,Message *m)
{
    uint8_t i;
    CAN_HandleType->pTxMsg->StdId = (uint32_t)(m->cob_id);// 标准标识符11位,即发送优先级
    CAN_HandleType->pTxMsg->ExtId = 0x00;         // 设置扩展标示符
    CAN_HandleType->pTxMsg->RTR = m->rtr;         //消息类型为数据帧，一帧8位
    CAN_HandleType->pTxMsg->IDE = CAN_ID_STD;     // 使用标准标识符
    CAN_HandleType->pTxMsg->DLC = m->len;         // 要发送的数据长度

    for(i = 0; i < m->len; i++) {
        CAN_HandleType->pTxMsg->Data[i] = m->data[i];
    }
    if(HAL_CAN_Transmit(CAN_HandleType,10)!=HAL_OK) return 1;     //发送
    return 0;
}
#endif

/**
 * @brief :	The use of this funciton is concordance the node id inf to the can message.该函数的使用是将节点ID INF与CAN消息一致的。
*  @param :	i_Msglen:数据长度
			msg     :要发送的数据
			i_NodeId：节点ID
 * @retval:	0,成功;
			其他,失败;
 */
#if(EN_CAN1_cfg == 1)
uint8_t SendCanMsgContainNodeId(uint32_t i_Msglen, uint8_t *msg, uint8_t i_NodeId)
{
    uint8_t ret;
    uint32_t i, j;
    uint8_t  u8SendErrCount = 0;
    uint8_t  LastTimeSendErrFlag = 1;
    Message ProcessedData;

    ProcessedData.cob_id = i_NodeId ;																									//合并后的ID
    ProcessedData.rtr = CAN_RTR_DATA;

    for(i = 0; i < (i_Msglen / 8 + ((i_Msglen % 8) ? 1 : 0)); i++)
    {   //长消息拆包的分包数,8为CAN消息的标准包长度
        if(LastTimeSendErrFlag == 0)
        {
            ProcessedData.len = ((i_Msglen - i * 8) > 8) ? 8 : (i_Msglen - i * 8);														//每个短包的数据长度，除最后一包需要另算外，其余均为8

            for(j = 0; j < ProcessedData.len; j++)
                ProcessedData.data[j] = msg[i * 8 + j];
        }
        else
        {
            //上次发送出错，直接发送数组即可，不需要重新复制
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
                SetCanBusOnlineFlag(1);																									//CAN总线掉线
                //
                //
////////////////////                StartTim6DelayTask(CAN_BUS_AUTO_RECONNECT_AFTER_5_SEC, 5000); 										//掉线后定时监测是否在线
                {
                    //需要更改
                }
                break;
            }
        }
        else
        {
            LastTimeSendErrFlag = 0;																									//发送成功，清零该标志，进入下一次循环，发送该数据
        }
    }

    return ret;
}


/**
 * @brief :	can1 口接收数据查询
 * @param :	buf:数据缓存区;
 * @retval:	返回值:0,无数据被收到;
 */

uint8_t CAN1_Receive_Msg(uint8_t *buf)
{
    uint32_t i;
    if(HAL_CAN_Receive(&CAN1_Handler,CAN_FIFO0,0)!=HAL_OK) return 0;																	//接收数据
    for(i=0; i<CAN1_Handler.pRxMsg->DLC; i++)
        buf[i]=CAN1_Handler.pRxMsg->Data[i];
    return CAN1_Handler.pRxMsg->DLC;
}


/**
 * @brief :	设置can总线在线状态
 * @param :	none.
 * @retval:	none.
 */
void SetCanBusOnlineFlag(uint8_t i_NewStatus)
{
    g_eCAN_BusOnLineFlag = i_NewStatus;
}
/**
 * @brief :	获取can总线在线状态
 * @param :	none.
 * @retval:	none.
 */
uint8_t GetCanBusOnlineFlag(void)
{
    return g_eCAN_BusOnLineFlag;
}

/**
 * @brief :	测试打印can数据
 * @param :	CAN_HandleType：CAN句柄
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
 * @brief :	CAN协议解析
 * @param :	CAN_HandleType：CAN句柄
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
    {   //收到的是报头帧数据
        for(i = 0; i < CAN_HandleType->pRxMsg->DLC; i++) {
            g_u8CanMsgRxBuff[i] = CAN_HandleType->pRxMsg->Data[i];
        }
        CanMsgRxStatu = COLLECTTING;
    }
    else
    {   //收到的是非报头的后续帧数据
        if(CanMsgRxStatu == COLLECTTING) {																								//否则将后续的字节存入到CAN消息缓冲区
            for(j = 0; j < CAN_HandleType->pRxMsg->DLC;)
            {
                g_u8CanMsgRxBuff[i] = CAN_HandleType->pRxMsg->Data[j];
                j++;
                i++;
                if(i >= PRGM_RX_BUFF_SIZE) {
                    CanMsgRxStatu = READY;																								//数据收集完成
                    break;
                }
            }
        } else { 																														//未收集数据时收到了未知数据
            CanMsgRxStatu = NOT_STARTED;
        }
    }

    if(CanMsgRxStatu == READY)
    {   //汇总后的数据包接收完成后再发送到上位机
        if(g_eCanMsgRxStatu == 0)
        {
            taskENTER_CRITICAL();           																							//进入临界区

            for(j = 0; j < PRGM_RX_BUFF_SIZE; j++)
                g_u8CanRxMsg[j] = g_u8CanMsgRxBuff[j];

////////////////////            OSTimeDlyResume(&CommTaskTCB,
////////////////////                            &err);

            {
                //需要添加恢复通信任务
            }
            g_eCanMsgRxStatu = 1;
            taskEXIT_CRITICAL();            																							 //退出临界区
        }
        CanMsgRxStatu = NOT_STARTED;
    }
    else
    {
        //否则未接收完，不予解析
    }
}
#endif

#endif
/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/
