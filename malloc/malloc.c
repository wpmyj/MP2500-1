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
 * @file      malloc.c
 * @author    GuanXianDe
 * @version   V1.0.0
 * @date      21-June-2018
 * @brief     program entry
 * @attention 最大支持3个内存池的分配 本工程只用到SRAM内存分配，
			  外部SDRAM和DTCM内存池作为预留部分
 *******************************************************************************
 */

/*
********************************************************************************
*                              INCLUDE FILES
********************************************************************************
*/
#include "malloc.h"
/*
********************************************************************************
*                             MACRO DEFINITIONS
********************************************************************************
*/
#if(USE_Malloc_File == 1)
#define SRAMBANK 	3	    																						//定义支持的SRAM块数.	

																													//mem1内存参数设定.mem1完全处于内部SRAM里面.
#define MEM1_BLOCK_SIZE			64  	  																			//内存块大小为64字节
#define MEM1_MAX_SIZE		64*1024  																				//最大管理内存 160K(160*1024)
#define MEM1_ALLOC_TABLE_SIZE	MEM1_MAX_SIZE/MEM1_BLOCK_SIZE 														//内存表大小

																													//mem2内存参数设定.mem2的内存池处于外部SDRAM里面
#define MEM2_BLOCK_SIZE			64  	  																			//内存块大小为64字节
#define MEM2_MAX_SIZE				64 					            													//最大管理内存28912K(28912 *1024)
#define MEM2_ALLOC_TABLE_SIZE	MEM2_MAX_SIZE/MEM2_BLOCK_SIZE 														//内存表大小

																													//mem3内存参数设定.mem3处于CCM,用于管理DTCM(特别注意,这部分SRAM,仅CPU可以访问!!)
#define MEM3_BLOCK_SIZE			64  	  																			//内存块大小为64字节
#define MEM3_MAX_SIZE			64    						    													//最大管理内存60K
#define MEM3_ALLOC_TABLE_SIZE	MEM3_MAX_SIZE/MEM3_BLOCK_SIZE 														//内存表大小(60 *1024)
#endif
/*
********************************************************************************
*                          OS-RELATED    VARIABLES
********************************************************************************
*/
#if(USE_Malloc_File == 1)
//内存管理控制器
struct _m_mallco_dev
{
    void (*init)(uint8_t);																							//初始化
    uint16_t (*perused)(uint8_t);		  	    																	//内存使用率
    uint8_t 	*membase[SRAMBANK];																					//内存池 管理SRAMBANK个区域的内存
    uint32_t *memmap[SRAMBANK]; 																					//内存管理状态表
    uint8_t  memrdy[SRAMBANK]; 																						//内存管理是否就绪
};

//内存池(32字节对齐)
__align(64) uint8_t mem1base[MEM1_MAX_SIZE];																		//内部SRAM内存池
__align(64) uint8_t mem2base[MEM2_MAX_SIZE] __attribute__((at(0XC01F4000)));										//外部SDRAM内存池,前面2M给LTDC用了(1280*800*2)
__align(64) uint8_t mem3base[MEM3_MAX_SIZE] __attribute__((at(0X20000000)));										//内部DTCM内存池
//内存管理表
uint32_t mem1mapbase[MEM1_ALLOC_TABLE_SIZE];																		//内部SRAM内存池MAP
uint32_t mem2mapbase[MEM2_ALLOC_TABLE_SIZE] __attribute__((at(0XC01F4000+MEM2_MAX_SIZE)));							//外部SRAM内存池MAP
uint32_t mem3mapbase[MEM3_ALLOC_TABLE_SIZE] __attribute__((at(0X20000000+MEM3_MAX_SIZE)));							//内部DTCM内存池MAP
//内存管理参数
const uint32_t memtblsize[SRAMBANK]= {MEM1_ALLOC_TABLE_SIZE,MEM2_ALLOC_TABLE_SIZE,MEM3_ALLOC_TABLE_SIZE};			//内存表大小
const uint32_t memblksize[SRAMBANK]= {MEM1_BLOCK_SIZE,MEM2_BLOCK_SIZE,MEM3_BLOCK_SIZE};								//内存分块大小
const uint32_t memsize[SRAMBANK]= {MEM1_MAX_SIZE,MEM2_MAX_SIZE,MEM3_MAX_SIZE};										//内存总大小
#endif
/*
********************************************************************************
*                             FUNCTION PROTOTYPES
********************************************************************************
*/
#if(USE_Malloc_File == 1)

#if(USE_Malloc_Conf == 1)
static void mymemset(void *s,uint8_t c,uint32_t count);
#endif
static void mymemcpy(void *des,void *src,uint32_t n);
uint16_t my_mem_perused(uint8_t memx);
static uint32_t my_mem_malloc(uint8_t memx,uint32_t size);
static uint8_t my_mem_free(uint8_t memx,uint32_t offset);

#endif
/*
********************************************************************************
*                              LOCAL VARIABLES
********************************************************************************
*/
#if(USE_Malloc_File == 1)
//内存管理控制器
struct _m_mallco_dev mallco_dev=
{
    my_mem_init,																									//内存初始化
    my_mem_perused,																									//内存使用率
    mem1base,mem2base,mem3base,																						//内存池
    mem1mapbase,mem2mapbase,mem3mapbase,																			//内存管理状态表
    0,0,0,  		 																								//内存管理未就绪
};
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
#if(USE_Malloc_File == 1)
/**
 * @brief :	复制内存
 * @param :
			*des:目的地址
			*src:源地址
			n:需要复制的内存长度(字节为单位)
 * @retval:	none.
 */
static void mymemcpy(void *des,void *src,uint32_t n)
{
    uint8_t *xdes=des;
    uint8_t *xsrc=src;
    while(n--)*xdes++=*xsrc++;
}

/**
 * @brief :	设置内存
 * @param :
			*s:内存首地址
			c :要设置的值
			count:需要设置的内存大小(字节为单位)
 * @retval:	none.
 */
#if(USE_Malloc_Conf == 1)
static void mymemset(void *s,uint8_t c,uint32_t count)
{
    uint8_t *xs = s;
    while(count--)*xs++=c;
}
#endif


/**
 * @brief :	获取内存使用率(用户调用)
 * @param :	memx:所属内存块
 * @retval:	使用率(扩大了10倍,0~1000,代表0.0%~100.0%)
 */
uint16_t my_mem_perused(uint8_t memx)
{
    uint32_t used=0;
    uint32_t i;
    for(i=0; i<memtblsize[memx]; i++)
    {
        if(mallco_dev.memmap[memx][i])used++;
    }
    return (used*1000)/(memtblsize[memx]);
}

/**
 * @brief :	内存分配(内部调用)
 * @param :
			memx:所属内存块
			size:要分配的内存大小(字节)
 * @retval:	0XFFFFFFFF,代表错误;其他,内存偏移地址
 */
static uint32_t my_mem_malloc(uint8_t memx,uint32_t size)
{
    signed long offset=0;
    uint32_t nmemb;																										//需要的内存块数
    uint32_t cmemb=0;																									//连续空内存块数
    uint32_t i;
    if(!mallco_dev.memrdy[memx])mallco_dev.init(memx);																	//未初始化,先执行初始化
    if(size==0)return 0XFFFFFFFF;																						//不需要分配
    nmemb=size/memblksize[memx];  																						//获取需要分配的连续内存块数
    if(size%memblksize[memx])nmemb++;
    for(offset=memtblsize[memx]-1; offset>=0; offset--)																	//搜索整个内存控制区
    {
        if(!mallco_dev.memmap[memx][offset])cmemb++;																	//连续空内存块数增加
        else cmemb=0;																									//连续内存块清零
        if(cmemb==nmemb)																								//找到了连续nmemb个空内存块
        {
            for(i=0; i<nmemb; i++)  																						//标注内存块非空
            {
                mallco_dev.memmap[memx][offset+i]=nmemb;
            }
            return (offset*memblksize[memx]);																			//返回偏移地址
        }
    }
    return 0XFFFFFFFF;																									//未找到符合分配条件的内存块
}

/**
 * @brief :	释放内存(内部调用)
 * @param :
			memx:所属内存块
			offset:内存地址偏移
 * @retval:	0,释放成功;1,释放失败;
 */
static uint8_t my_mem_free(uint8_t memx,uint32_t offset)
{
    int i;
    if(!mallco_dev.memrdy[memx])																						//未初始化,先执行初始化
    {
        mallco_dev.init(memx);
        return 1;																										//未初始化
    }
    if(offset<memsize[memx])																							//偏移在内存池内.
    {
        int index=offset/memblksize[memx];																				//偏移所在内存块号码
        int nmemb=mallco_dev.memmap[memx][index];																		//内存块数量
        for(i=0; i<nmemb; i++)  																						//内存块清零
        {
            mallco_dev.memmap[memx][index+i]=0;
        }
        return 0;
    } else return 2;																									//偏移超区了.
}

/**
 * @brief :	释放内存(外部调用)
 * @param :
			memx:所属内存块
			ptr:内存首地址
 * @retval:	none.
 */
void myfree(uint8_t memx,void *ptr)
{
    uint32_t offset;
    if(ptr==NULL)return;																								//地址为0.
    offset=(uint32_t)ptr-(uint32_t)mallco_dev.membase[memx];
		my_mem_free(memx,offset);																						//释放内存
}

/**
 * @brief :	分配内存(外部调用)
 * @param :
			memx:所属内存块
			size:内存大小(字节)
 * @retval:	分配到的内存首地址.
 */
void *mymalloc(uint8_t memx,uint32_t size)
{
    uint32_t offset;
    offset=my_mem_malloc(memx,size);
    if(offset==0XFFFFFFFF)return NULL;
    else return (void*)((uint32_t)mallco_dev.membase[memx]+offset);
}

/**
 * @brief :	重新分配内存(外部调用)
 * @param :
			memx:所属内存块
			*ptr:旧内存首地址
			size:要分配的内存大小(字节)
 * @retval:	新分配到的内存首地址.
 */
void *myrealloc(uint8_t memx,void *ptr,uint32_t size)
{
    uint32_t offset;
    offset=my_mem_malloc(memx,size);
    if(offset==0XFFFFFFFF)return NULL;
    else
    {
        mymemcpy((void*)((uint32_t)mallco_dev.membase[memx]+offset),ptr,size);											//拷贝旧内存内容到新内存
        myfree(memx,ptr);  											  													//释放旧内存
        return (void*)((uint32_t)mallco_dev.membase[memx]+offset);  													//返回新内存首地址
    }
}
#endif
/**
 * @brief :	内存管理初始化(用户 初始化调用)
 * @param :	memx:所属内存块
 * @retval:	none.
 */
void my_mem_init(uint8_t memx)
{
#if((USE_Malloc_File == 1)&&(USE_Malloc_Conf == 1))
    mymemset(mallco_dev.memmap[memx],0,memtblsize[memx]*4);																//内存状态表数据清零
    mallco_dev.memrdy[memx]=1;																							//内存管理初始化OK
#endif
}

/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/










