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
 * @file      mpu.c
 * @author   	GuanXianDe
 * @version   V1.0.0
 * @date      25-Jnee-2018
 * @brief     program entry
 * @attention none.
 *******************************************************************************
 */

/*
********************************************************************************
*                              INCLUDE FILES
********************************************************************************
*/
#include "mpu.h"
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
void MPU_Memory_Protection(void);

#if((USE_Mpu_File == 1)&&(USE_Mpu_Conf == 1))
static uint8_t MPU_Set_Protection(uint32_t baseaddr,uint32_t size,uint32_t rnum,uint32_t ap);
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
 * @brief :设置需要保护的存储块
						必须对部分存储区域进行MPU保护,否则可能导致程序运行异常
						比如MCU屏不显示,数据采集不正常等等
 * @param :	none.
 * @retval:	none.
 */
void MPU_Memory_Protection(void)
{
#if((USE_Mpu_File == 1)&&(USE_Mpu_Conf == 1))
    MPU_Set_Protection(0x60000000,MPU_REGION_SIZE_64MB,MPU_REGION_NUMBER0,MPU_REGION_FULL_ACCESS);	//保护MCU LCD屏所在的FMC区域,,共64M字节
    MPU_Set_Protection(0x20000000,MPU_REGION_SIZE_512KB,MPU_REGION_NUMBER1,MPU_REGION_FULL_ACCESS);	//保护整个内部SRAM,包括SRAM1,SRAM2和DTCM,共512K字节
    MPU_Set_Protection(0XC0000000,MPU_REGION_SIZE_32MB,MPU_REGION_NUMBER2,MPU_REGION_FULL_ACCESS);	//保护SDRAM区域,共32M字节
    MPU_Set_Protection(0X80000000,MPU_REGION_SIZE_256MB,MPU_REGION_NUMBER3,MPU_REGION_FULL_ACCESS);	//保护整个NAND FLASH区域,共256M字节
#endif
}

#if(USE_Mpu_File == 1)

#if(USE_Mpu_Conf == 1)
/**
 * @brief :	设置某个区域的MPU保护
 * @param :
			baseaddr:MPU保护区域的基址(首地址)
			size:MPU保护区域的大小(必须是32的倍数,单位为字节),可设置的值参考:CORTEX_MPU_Region_Size
			rnum:MPU保护区编号,范围:0~7,最大支持8个保护区域,可设置的值参考：CORTEX_MPU_Region_Number
			ap:访问权限,访问关系如下:可设置的值参考：CORTEX_MPU_Region_Permission_Attributes
			MPU_REGION_NO_ACCESS,无访问（特权&用户都不可访问）
			MPU_REGION_PRIV_RW,仅支持特权读写访问
			MPU_REGION_PRIV_RW_URO,禁止用户写访问（特权可读写访问）
			MPU_REGION_FULL_ACCESS,全访问（特权&用户都可访问）
			MPU_REGION_PRIV_RO,仅支持特权读访问
			MPU_REGION_PRIV_RO_URO,只读（特权&用户都不可以写）
			详见:STM32F7 Series Cortex-M7 processor programming manual.pdf,4.6节,Table 89.
 * @retval:	0,成功.
			其他,错误.
 */
static uint8_t MPU_Set_Protection(uint32_t baseaddr,uint32_t size,uint32_t rnum,uint32_t ap)
{
    MPU_Region_InitTypeDef MPU_Initure;

    HAL_MPU_Disable();								        										//配置MPU之前先关闭MPU,配置完成以后在使能MPU

    MPU_Initure.Enable=MPU_REGION_ENABLE;			        										//使能该保护区域
    MPU_Initure.Number=rnum;			                    										//设置保护区域
    MPU_Initure.BaseAddress=baseaddr;	                    										//设置基址
    MPU_Initure.Size=size;				                    										//设置保护区域大小
    MPU_Initure.SubRegionDisable=0X00;                      										//禁止子区域
    MPU_Initure.TypeExtField=MPU_TEX_LEVEL0;                										//设置类型扩展域为level0
    MPU_Initure.AccessPermission=(uint8_t)ap;		            									//设置访问权限,
    MPU_Initure.DisableExec=MPU_INSTRUCTION_ACCESS_ENABLE;											//允许指令访问(允许读取指令)
    MPU_Initure.IsShareable=MPU_ACCESS_NOT_SHAREABLE;       										//禁止共用
    MPU_Initure.IsCacheable=MPU_ACCESS_CACHEABLE;       											//使能cache
    MPU_Initure.IsBufferable=MPU_ACCESS_BUFFERABLE;         										//允许缓冲
    HAL_MPU_ConfigRegion(&MPU_Initure);                     										//配置MPU
    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);			        										//开启MPU
    return 0;
}

#endif

/**
 * @brief :	MemManage错误处理中断（进入此中断以后,将无法恢复程序运行!!强行重启）
 * @param :	none.
 * @retval:	none.
 */
void MemManage_Handler(void)
{
    printf("Mem Access Error!!\r\n"); 																//输出错误信息
    delay_ms(1000);
    printf("Soft Reseting...\r\n");																	//提示软件重启
    delay_ms(1000);
    NVIC_SystemReset();																				//软复位
}
#endif

/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/

