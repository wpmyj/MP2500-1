/*******************************************************************************
*                               APPLICATION CODE
*
* (c)Copyright Guangdong ENECO POWER Science And Technology Co., Ltd.2015-2018.
*
*      All rights reserved.  Protected by international copyright laws.
*       Knowledge of the source code may NOT be used without authorization.
********************************************************************************
*/


/**
 *******************************************************************************
 * @file      MALLOC.h
 * @author    GuanXianDe
 * @version   V1.0.0
 * @date     21-June-2018
 * @brief     Header for main.c module
 * @attention
 *******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MALLOC_H
#define _MALLOC_H

/*
********************************************************************************
*                                 INCLUDE FILES
********************************************************************************
*/

#include "sys.h"

/*
********************************************************************************
*                              EXPORTED DATA TYPES
********************************************************************************
*/



/*
********************************************************************************
*                               EXPORTED CONSTANT
********************************************************************************
*/



/*
********************************************************************************
*                                EXPORTED MACRO
********************************************************************************
*/
#if(USE_Malloc_File == 1)
#ifndef NULL
#define NULL 0
#endif

//定义三个内存池
#define SRAMIN	    0												//内部内存池
#define SRAMEX      1												//外部内存池(SDRAM)
#define SRAMDTCM    2												//DTCM内存池(此部分SRAM仅仅CPU可以访问!!!)


/*
********************************************************************************
*                                   EXTERNS
********************************************************************************
*/


/*
********************************************************************************
*                              FUNCTION PROTOTYPES
********************************************************************************
*/



//用户调用函数
void myfree(uint8_t memx,void *ptr);  							//内存释放(外部调用)
void *mymalloc(uint8_t memx,uint32_t size);						//内存分配(外部调用)
void *myrealloc(uint8_t memx,void *ptr,uint32_t size);			//重新分配内存(外部调用)
#endif
void my_mem_init(uint8_t memx);									//内存管理初始化函数(外/内部调用)
#endif /* __MALLOC_H */

/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/

	 


