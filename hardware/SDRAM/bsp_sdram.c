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
 * @file      bsp_sdram.c
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
#include "bsp_sdram.h"
#include "delay.h"

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

/*
********************************************************************************
*                               GLOBAL VARIABLES
********************************************************************************
*/
#if(USE_Sdram_File == 1)
SDRAM_HandleTypeDef SDRAM_Handler;   //SDRAM���
#endif

/*
********************************************************************************
*                              FUNCTION DEFINITION
********************************************************************************
*/

/**
 * @brief :SDRAM��ʼ��
 * @param :none.
 * @retval:none.
 */
void BSP_SdramInit(void)
{
#if(USE_Sdram_Conf == 1)	
	 SDRAM_Init();
#endif
}

#if(USE_Sdram_File == 1)
/**
 * @brief :	SDRAM��ʼ��
 * @param :	none.
 * @retval:	none.
 */

void SDRAM_Init(void)
{
    FMC_SDRAM_TimingTypeDef SDRAM_Timing;

    SDRAM_Handler.Instance = FMC_SDRAM_DEVICE;                           	//SDRAM��BANK5,6
    SDRAM_Handler.Init.SDBank = FMC_SDRAM_BANK1;                         	//SDRAM����BANK5��
    SDRAM_Handler.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_9;   	//������
    SDRAM_Handler.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13;        	//������
    SDRAM_Handler.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;     	//���ݿ��Ϊ16λ
    SDRAM_Handler.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;	//һ��4��BANK
    SDRAM_Handler.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;             	//CASΪ3
    SDRAM_Handler.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;//ʧ��д����
    SDRAM_Handler.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;         	//SDRAMʱ��ΪHCLK/2=216M/2=108M=9.3ns
    SDRAM_Handler.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;              	//ʹ��ͻ��
    SDRAM_Handler.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_1;          	//��ͨ����ʱ

    SDRAM_Timing.LoadToActiveDelay = 2;                                 	//����ģʽ�Ĵ���������ʱ����ӳ�Ϊ2��ʱ������
    SDRAM_Timing.ExitSelfRefreshDelay = 8;                              	//�˳���ˢ���ӳ�Ϊ8��ʱ������
    SDRAM_Timing.SelfRefreshTime = 6;                                   	//��ˢ��ʱ��Ϊ6��ʱ������
    SDRAM_Timing.RowCycleDelay = 6;                                     	//��ѭ���ӳ�Ϊ6��ʱ������
    SDRAM_Timing.WriteRecoveryTime = 2;                                 	//�ָ��ӳ�Ϊ2��ʱ������
    SDRAM_Timing.RPDelay = 2;                                           	//��Ԥ����ӳ�Ϊ2��ʱ������
    SDRAM_Timing.RCDDelay = 2;                                          	//�е����ӳ�Ϊ2��ʱ������
    HAL_SDRAM_Init(&SDRAM_Handler, &SDRAM_Timing);

    SDRAM_Initialization_Sequence(&SDRAM_Handler);							//����SDRAM��ʼ������

}

/**
 * @brief :	����SDRAM��ʼ������
 * @param :	none.
 * @retval:	none.
 */

void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram)
{
    uint32_t temp = 0;
																			//SDRAM��������ʼ������Ժ���Ҫ��������˳���ʼ��SDRAM
    SDRAM_Send_Cmd(0, FMC_SDRAM_CMD_CLK_ENABLE, 1, 0); 						//ʱ������ʹ��
    delay_us(500);                                  						//������ʱ200us
    SDRAM_Send_Cmd(0, FMC_SDRAM_CMD_PALL, 1, 0);    						//�����д洢��Ԥ���
	
    SDRAM_Send_Cmd(0, FMC_SDRAM_CMD_AUTOREFRESH_MODE, 8, 0); 				//������ˢ�´���
																			//����ģʽ�Ĵ���,SDRAM��bit0~bit2Ϊָ��ͻ�����ʵĳ��ȣ�
																			//bit3Ϊָ��ͻ�����ʵ����ͣ�bit4~bit6ΪCASֵ��bit7��bit8Ϊ����ģʽ
																			//bit9Ϊָ����дͻ��ģʽ��bit10��bit11λ����λ
	
    temp = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1          | 				//����ͻ������:1(������1/2/4/8)
           SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |   						//����ͻ������:����(����������/����)
           SDRAM_MODEREG_CAS_LATENCY_3           |   						//����CASֵ:3(������2/3)
           SDRAM_MODEREG_OPERATING_MODE_STANDARD |   						//���ò���ģʽ:0,��׼ģʽ
           SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;     						//����ͻ��дģʽ:1,�������
    SDRAM_Send_Cmd(0, FMC_SDRAM_CMD_LOAD_MODE, 1, temp); 					//����SDRAM��ģʽ�Ĵ���

																			//ˢ��Ƶ�ʼ�����(��SDCLKƵ�ʼ���),���㷽��:
																			//COUNT=SDRAMˢ������/����-20=SDRAMˢ������(us)*SDCLKƵ��(Mhz)/����
																			//����ʹ�õ�SDRAMˢ������Ϊ64ms,SDCLK=216/2=108Mhz,����Ϊ8192(2^13).
																			//����,COUNT=64*1000*108/8192-20=823
    HAL_SDRAM_ProgramRefreshRate(&SDRAM_Handler, 823);

}

/**
 * @brief :	SDRAM�ײ��������������ã�ʱ��ʹ��;�˺����ᱻHAL_SDRAM_Init()����
 * @param :	hsdram:SDRAM���.
 * @retval:	none.
 */

void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef *hsdram)
{
    GPIO_InitTypeDef GPIO_Initure;

    /* ʱ��ʹ�� */
    __HAL_RCC_FMC_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    /* ��ʼ��GPIO */
    GPIO_Initure.Pin = GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_Initure.Mode = GPIO_MODE_AF_PP;
    GPIO_Initure.Pull = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;
    GPIO_Initure.Alternate = GPIO_AF12_FMC;
    HAL_GPIO_Init(GPIOC, &GPIO_Initure);

    GPIO_Initure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOD, &GPIO_Initure);

    GPIO_Initure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOE, &GPIO_Initure);

    GPIO_Initure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOF, &GPIO_Initure);

    GPIO_Initure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOG, &GPIO_Initure);
}

/**
 * @brief :	��SDRAM��������
 * @param :	bankx:0,��BANK5�����SDRAM����ָ��1,��BANK6�����SDRAM����ָ��
 * @param :	cmd:ָ��(0,����ģʽ/1,ʱ������ʹ��/2,Ԥ������д洢��/3,�Զ�ˢ��/4,����ģʽ�Ĵ���/5,��ˢ��/6,����)
 * @param :	refresh:��ˢ�´���
 * @param :	regval:ģʽ�Ĵ����Ķ���
 * @retval:	����ֵ:0,����;1,ʧ��..
 */

uint8_t SDRAM_Send_Cmd(uint8_t bankx, uint8_t cmd, uint8_t refresh, uint16_t regval)
{
    uint32_t target_bank = 0;
    FMC_SDRAM_CommandTypeDef Command;

    if(bankx == 0) {
        target_bank = FMC_SDRAM_CMD_TARGET_BANK1;
    } else if(bankx == 1) {
        target_bank = FMC_SDRAM_CMD_TARGET_BANK2;
    }

    Command.CommandMode = cmd;              									//����
    Command.CommandTarget = target_bank;    									//Ŀ��SDRAM�洢����
    Command.AutoRefreshNumber = refresh;    									//��ˢ�´���
    Command.ModeRegisterDefinition = regval; 									//Ҫд��ģʽ�Ĵ�����ֵ

																				//��SDRAM��������
    if(HAL_SDRAM_SendCommand(&SDRAM_Handler, &Command, 0X1000) == HAL_OK) {
        return 0;
    } else {
        return 1;
    }
}

/**
 * @brief :	��ָ����ַ(WriteAddr+Bank5_SDRAM_ADDR)��ʼ,����д��n���ֽ�.
 * @param :	pBuffer:�ֽ�ָ��.
 * @param :	ReadAddr:Ҫд��ĵ�ַ
 * @param :	n:Ҫд����ֽ���
 * @retval:	none.
 */

void FMC_SDRAM_WriteBuffer(uint8_t *pBuffer, uint32_t WriteAddr, uint32_t n)
{
    for(; n != 0; n--) {
        *(__IO uint8_t *)(Bank5_SDRAM_ADDR + WriteAddr) = *pBuffer;
        WriteAddr++;
        pBuffer++;
    }
}

/**
 * @brief :	��ָ����ַ((WriteAddr+Bank5_SDRAM_ADDR))��ʼ,��������n���ֽ�.
 * @param :	pBuffer:�ֽ�ָ��.
 * @param :	ReadAddr:Ҫ��������ʼ��ַ
 * @param :	n:Ҫд����ֽ���
 * @retval:	none.
 */

void FMC_SDRAM_ReadBuffer(uint8_t *pBuffer, uint32_t ReadAddr, uint32_t n)
{
    for(; n != 0; n--) {
        *pBuffer++ = *(__IO uint8_t *)(Bank5_SDRAM_ADDR + ReadAddr);
        ReadAddr++;
    }
}
#else
#if(USE_Sdram_Conf == 1)	
void SDRAM_Init(void)
{
	//NULL
}
#endif
#endif

/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/
