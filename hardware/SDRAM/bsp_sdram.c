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
SDRAM_HandleTypeDef SDRAM_Handler;   //SDRAM句柄
#endif

/*
********************************************************************************
*                              FUNCTION DEFINITION
********************************************************************************
*/

/**
 * @brief :SDRAM初始化
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
 * @brief :	SDRAM初始化
 * @param :	none.
 * @retval:	none.
 */

void SDRAM_Init(void)
{
    FMC_SDRAM_TimingTypeDef SDRAM_Timing;

    SDRAM_Handler.Instance = FMC_SDRAM_DEVICE;                           	//SDRAM在BANK5,6
    SDRAM_Handler.Init.SDBank = FMC_SDRAM_BANK1;                         	//SDRAM接在BANK5上
    SDRAM_Handler.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_9;   	//列数量
    SDRAM_Handler.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13;        	//行数量
    SDRAM_Handler.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;     	//数据宽度为16位
    SDRAM_Handler.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;	//一共4个BANK
    SDRAM_Handler.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;             	//CAS为3
    SDRAM_Handler.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;//失能写保护
    SDRAM_Handler.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;         	//SDRAM时钟为HCLK/2=216M/2=108M=9.3ns
    SDRAM_Handler.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;              	//使能突发
    SDRAM_Handler.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_1;          	//读通道延时

    SDRAM_Timing.LoadToActiveDelay = 2;                                 	//加载模式寄存器到激活时间的延迟为2个时钟周期
    SDRAM_Timing.ExitSelfRefreshDelay = 8;                              	//退出自刷新延迟为8个时钟周期
    SDRAM_Timing.SelfRefreshTime = 6;                                   	//自刷新时间为6个时钟周期
    SDRAM_Timing.RowCycleDelay = 6;                                     	//行循环延迟为6个时钟周期
    SDRAM_Timing.WriteRecoveryTime = 2;                                 	//恢复延迟为2个时钟周期
    SDRAM_Timing.RPDelay = 2;                                           	//行预充电延迟为2个时钟周期
    SDRAM_Timing.RCDDelay = 2;                                          	//行到列延迟为2个时钟周期
    HAL_SDRAM_Init(&SDRAM_Handler, &SDRAM_Timing);

    SDRAM_Initialization_Sequence(&SDRAM_Handler);							//发送SDRAM初始化序列

}

/**
 * @brief :	发送SDRAM初始化序列
 * @param :	none.
 * @retval:	none.
 */

void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram)
{
    uint32_t temp = 0;
																			//SDRAM控制器初始化完成以后还需要按照如下顺序初始化SDRAM
    SDRAM_Send_Cmd(0, FMC_SDRAM_CMD_CLK_ENABLE, 1, 0); 						//时钟配置使能
    delay_us(500);                                  						//至少延时200us
    SDRAM_Send_Cmd(0, FMC_SDRAM_CMD_PALL, 1, 0);    						//对所有存储区预充电
	
    SDRAM_Send_Cmd(0, FMC_SDRAM_CMD_AUTOREFRESH_MODE, 8, 0); 				//设置自刷新次数
																			//配置模式寄存器,SDRAM的bit0~bit2为指定突发访问的长度，
																			//bit3为指定突发访问的类型，bit4~bit6为CAS值，bit7和bit8为运行模式
																			//bit9为指定的写突发模式，bit10和bit11位保留位
	
    temp = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1          | 				//设置突发长度:1(可以是1/2/4/8)
           SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |   						//设置突发类型:连续(可以是连续/交错)
           SDRAM_MODEREG_CAS_LATENCY_3           |   						//设置CAS值:3(可以是2/3)
           SDRAM_MODEREG_OPERATING_MODE_STANDARD |   						//设置操作模式:0,标准模式
           SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;     						//设置突发写模式:1,单点访问
    SDRAM_Send_Cmd(0, FMC_SDRAM_CMD_LOAD_MODE, 1, temp); 					//设置SDRAM的模式寄存器

																			//刷新频率计数器(以SDCLK频率计数),计算方法:
																			//COUNT=SDRAM刷新周期/行数-20=SDRAM刷新周期(us)*SDCLK频率(Mhz)/行数
																			//我们使用的SDRAM刷新周期为64ms,SDCLK=216/2=108Mhz,行数为8192(2^13).
																			//所以,COUNT=64*1000*108/8192-20=823
    HAL_SDRAM_ProgramRefreshRate(&SDRAM_Handler, 823);

}

/**
 * @brief :	SDRAM底层驱动，引脚配置，时钟使能;此函数会被HAL_SDRAM_Init()调用
 * @param :	hsdram:SDRAM句柄.
 * @retval:	none.
 */

void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef *hsdram)
{
    GPIO_InitTypeDef GPIO_Initure;

    /* 时钟使能 */
    __HAL_RCC_FMC_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    /* 初始化GPIO */
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
 * @brief :	向SDRAM发送命令
 * @param :	bankx:0,向BANK5上面的SDRAM发送指令1,向BANK6上面的SDRAM发送指令
 * @param :	cmd:指令(0,正常模式/1,时钟配置使能/2,预充电所有存储区/3,自动刷新/4,加载模式寄存器/5,自刷新/6,掉电)
 * @param :	refresh:自刷新次数
 * @param :	regval:模式寄存器的定义
 * @retval:	返回值:0,正常;1,失败..
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

    Command.CommandMode = cmd;              									//命令
    Command.CommandTarget = target_bank;    									//目标SDRAM存储区域
    Command.AutoRefreshNumber = refresh;    									//自刷新次数
    Command.ModeRegisterDefinition = regval; 									//要写入模式寄存器的值

																				//向SDRAM发送命令
    if(HAL_SDRAM_SendCommand(&SDRAM_Handler, &Command, 0X1000) == HAL_OK) {
        return 0;
    } else {
        return 1;
    }
}

/**
 * @brief :	在指定地址(WriteAddr+Bank5_SDRAM_ADDR)开始,连续写入n个字节.
 * @param :	pBuffer:字节指针.
 * @param :	ReadAddr:要写入的地址
 * @param :	n:要写入的字节数
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
 * @brief :	在指定地址((WriteAddr+Bank5_SDRAM_ADDR))开始,连续读出n个字节.
 * @param :	pBuffer:字节指针.
 * @param :	ReadAddr:要读出的起始地址
 * @param :	n:要写入的字节数
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
