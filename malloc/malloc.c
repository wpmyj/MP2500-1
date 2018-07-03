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
 * @attention ���֧��3���ڴ�صķ��� ������ֻ�õ�SRAM�ڴ���䣬
			  �ⲿSDRAM��DTCM�ڴ����ΪԤ������
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
#define SRAMBANK 	3	    																						//����֧�ֵ�SRAM����.	

																													//mem1�ڴ�����趨.mem1��ȫ�����ڲ�SRAM����.
#define MEM1_BLOCK_SIZE			64  	  																			//�ڴ���СΪ64�ֽ�
#define MEM1_MAX_SIZE		64*1024  																				//�������ڴ� 160K(160*1024)
#define MEM1_ALLOC_TABLE_SIZE	MEM1_MAX_SIZE/MEM1_BLOCK_SIZE 														//�ڴ���С

																													//mem2�ڴ�����趨.mem2���ڴ�ش����ⲿSDRAM����
#define MEM2_BLOCK_SIZE			64  	  																			//�ڴ���СΪ64�ֽ�
#define MEM2_MAX_SIZE				64 					            													//�������ڴ�28912K(28912 *1024)
#define MEM2_ALLOC_TABLE_SIZE	MEM2_MAX_SIZE/MEM2_BLOCK_SIZE 														//�ڴ���С

																													//mem3�ڴ�����趨.mem3����CCM,���ڹ���DTCM(�ر�ע��,�ⲿ��SRAM,��CPU���Է���!!)
#define MEM3_BLOCK_SIZE			64  	  																			//�ڴ���СΪ64�ֽ�
#define MEM3_MAX_SIZE			64    						    													//�������ڴ�60K
#define MEM3_ALLOC_TABLE_SIZE	MEM3_MAX_SIZE/MEM3_BLOCK_SIZE 														//�ڴ���С(60 *1024)
#endif
/*
********************************************************************************
*                          OS-RELATED    VARIABLES
********************************************************************************
*/
#if(USE_Malloc_File == 1)
//�ڴ���������
struct _m_mallco_dev
{
    void (*init)(uint8_t);																							//��ʼ��
    uint16_t (*perused)(uint8_t);		  	    																	//�ڴ�ʹ����
    uint8_t 	*membase[SRAMBANK];																					//�ڴ�� ����SRAMBANK��������ڴ�
    uint32_t *memmap[SRAMBANK]; 																					//�ڴ����״̬��
    uint8_t  memrdy[SRAMBANK]; 																						//�ڴ�����Ƿ����
};

//�ڴ��(32�ֽڶ���)
__align(64) uint8_t mem1base[MEM1_MAX_SIZE];																		//�ڲ�SRAM�ڴ��
__align(64) uint8_t mem2base[MEM2_MAX_SIZE] __attribute__((at(0XC01F4000)));										//�ⲿSDRAM�ڴ��,ǰ��2M��LTDC����(1280*800*2)
__align(64) uint8_t mem3base[MEM3_MAX_SIZE] __attribute__((at(0X20000000)));										//�ڲ�DTCM�ڴ��
//�ڴ�����
uint32_t mem1mapbase[MEM1_ALLOC_TABLE_SIZE];																		//�ڲ�SRAM�ڴ��MAP
uint32_t mem2mapbase[MEM2_ALLOC_TABLE_SIZE] __attribute__((at(0XC01F4000+MEM2_MAX_SIZE)));							//�ⲿSRAM�ڴ��MAP
uint32_t mem3mapbase[MEM3_ALLOC_TABLE_SIZE] __attribute__((at(0X20000000+MEM3_MAX_SIZE)));							//�ڲ�DTCM�ڴ��MAP
//�ڴ�������
const uint32_t memtblsize[SRAMBANK]= {MEM1_ALLOC_TABLE_SIZE,MEM2_ALLOC_TABLE_SIZE,MEM3_ALLOC_TABLE_SIZE};			//�ڴ���С
const uint32_t memblksize[SRAMBANK]= {MEM1_BLOCK_SIZE,MEM2_BLOCK_SIZE,MEM3_BLOCK_SIZE};								//�ڴ�ֿ��С
const uint32_t memsize[SRAMBANK]= {MEM1_MAX_SIZE,MEM2_MAX_SIZE,MEM3_MAX_SIZE};										//�ڴ��ܴ�С
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
//�ڴ���������
struct _m_mallco_dev mallco_dev=
{
    my_mem_init,																									//�ڴ��ʼ��
    my_mem_perused,																									//�ڴ�ʹ����
    mem1base,mem2base,mem3base,																						//�ڴ��
    mem1mapbase,mem2mapbase,mem3mapbase,																			//�ڴ����״̬��
    0,0,0,  		 																								//�ڴ����δ����
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
 * @brief :	�����ڴ�
 * @param :
			*des:Ŀ�ĵ�ַ
			*src:Դ��ַ
			n:��Ҫ���Ƶ��ڴ泤��(�ֽ�Ϊ��λ)
 * @retval:	none.
 */
static void mymemcpy(void *des,void *src,uint32_t n)
{
    uint8_t *xdes=des;
    uint8_t *xsrc=src;
    while(n--)*xdes++=*xsrc++;
}

/**
 * @brief :	�����ڴ�
 * @param :
			*s:�ڴ��׵�ַ
			c :Ҫ���õ�ֵ
			count:��Ҫ���õ��ڴ��С(�ֽ�Ϊ��λ)
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
 * @brief :	��ȡ�ڴ�ʹ����(�û�����)
 * @param :	memx:�����ڴ��
 * @retval:	ʹ����(������10��,0~1000,����0.0%~100.0%)
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
 * @brief :	�ڴ����(�ڲ�����)
 * @param :
			memx:�����ڴ��
			size:Ҫ������ڴ��С(�ֽ�)
 * @retval:	0XFFFFFFFF,�������;����,�ڴ�ƫ�Ƶ�ַ
 */
static uint32_t my_mem_malloc(uint8_t memx,uint32_t size)
{
    signed long offset=0;
    uint32_t nmemb;																										//��Ҫ���ڴ����
    uint32_t cmemb=0;																									//�������ڴ����
    uint32_t i;
    if(!mallco_dev.memrdy[memx])mallco_dev.init(memx);																	//δ��ʼ��,��ִ�г�ʼ��
    if(size==0)return 0XFFFFFFFF;																						//����Ҫ����
    nmemb=size/memblksize[memx];  																						//��ȡ��Ҫ����������ڴ����
    if(size%memblksize[memx])nmemb++;
    for(offset=memtblsize[memx]-1; offset>=0; offset--)																	//���������ڴ������
    {
        if(!mallco_dev.memmap[memx][offset])cmemb++;																	//�������ڴ��������
        else cmemb=0;																									//�����ڴ������
        if(cmemb==nmemb)																								//�ҵ�������nmemb�����ڴ��
        {
            for(i=0; i<nmemb; i++)  																						//��ע�ڴ��ǿ�
            {
                mallco_dev.memmap[memx][offset+i]=nmemb;
            }
            return (offset*memblksize[memx]);																			//����ƫ�Ƶ�ַ
        }
    }
    return 0XFFFFFFFF;																									//δ�ҵ����Ϸ����������ڴ��
}

/**
 * @brief :	�ͷ��ڴ�(�ڲ�����)
 * @param :
			memx:�����ڴ��
			offset:�ڴ��ַƫ��
 * @retval:	0,�ͷųɹ�;1,�ͷ�ʧ��;
 */
static uint8_t my_mem_free(uint8_t memx,uint32_t offset)
{
    int i;
    if(!mallco_dev.memrdy[memx])																						//δ��ʼ��,��ִ�г�ʼ��
    {
        mallco_dev.init(memx);
        return 1;																										//δ��ʼ��
    }
    if(offset<memsize[memx])																							//ƫ�����ڴ����.
    {
        int index=offset/memblksize[memx];																				//ƫ�������ڴ�����
        int nmemb=mallco_dev.memmap[memx][index];																		//�ڴ������
        for(i=0; i<nmemb; i++)  																						//�ڴ������
        {
            mallco_dev.memmap[memx][index+i]=0;
        }
        return 0;
    } else return 2;																									//ƫ�Ƴ�����.
}

/**
 * @brief :	�ͷ��ڴ�(�ⲿ����)
 * @param :
			memx:�����ڴ��
			ptr:�ڴ��׵�ַ
 * @retval:	none.
 */
void myfree(uint8_t memx,void *ptr)
{
    uint32_t offset;
    if(ptr==NULL)return;																								//��ַΪ0.
    offset=(uint32_t)ptr-(uint32_t)mallco_dev.membase[memx];
		my_mem_free(memx,offset);																						//�ͷ��ڴ�
}

/**
 * @brief :	�����ڴ�(�ⲿ����)
 * @param :
			memx:�����ڴ��
			size:�ڴ��С(�ֽ�)
 * @retval:	���䵽���ڴ��׵�ַ.
 */
void *mymalloc(uint8_t memx,uint32_t size)
{
    uint32_t offset;
    offset=my_mem_malloc(memx,size);
    if(offset==0XFFFFFFFF)return NULL;
    else return (void*)((uint32_t)mallco_dev.membase[memx]+offset);
}

/**
 * @brief :	���·����ڴ�(�ⲿ����)
 * @param :
			memx:�����ڴ��
			*ptr:���ڴ��׵�ַ
			size:Ҫ������ڴ��С(�ֽ�)
 * @retval:	�·��䵽���ڴ��׵�ַ.
 */
void *myrealloc(uint8_t memx,void *ptr,uint32_t size)
{
    uint32_t offset;
    offset=my_mem_malloc(memx,size);
    if(offset==0XFFFFFFFF)return NULL;
    else
    {
        mymemcpy((void*)((uint32_t)mallco_dev.membase[memx]+offset),ptr,size);											//�������ڴ����ݵ����ڴ�
        myfree(memx,ptr);  											  													//�ͷž��ڴ�
        return (void*)((uint32_t)mallco_dev.membase[memx]+offset);  													//�������ڴ��׵�ַ
    }
}
#endif
/**
 * @brief :	�ڴ�����ʼ��(�û� ��ʼ������)
 * @param :	memx:�����ڴ��
 * @retval:	none.
 */
void my_mem_init(uint8_t memx)
{
#if((USE_Malloc_File == 1)&&(USE_Malloc_Conf == 1))
    mymemset(mallco_dev.memmap[memx],0,memtblsize[memx]*4);																//�ڴ�״̬����������
    mallco_dev.memrdy[memx]=1;																							//�ڴ�����ʼ��OK
#endif
}

/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/










