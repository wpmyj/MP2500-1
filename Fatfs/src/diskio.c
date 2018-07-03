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
 * @file      diskio.c
 * @author    GuanXianDe
 * @version   V1.0.0
 * @date      22-June-2018
 * @brief     program entry
 * @attention none.
 *******************************************************************************
 */
/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/
#include "user_config.h"
#if(USE_Fatfs_File== 1)
#define  Support_SD_CARD                  1                      											//֧��SD��
#define  Support_EX_FLASH                 0                      											//֧���ⲿspi flash
#define  Support_EX_NAND                  0                      											//֧���ⲿnand flash
/*
********************************************************************************
*                              INCLUDE FILES
********************************************************************************
*/
#include "diskio.h"																							/* FatFs lower layer API */
#include "malloc.h"

#include "bsp_rtc.h"

#if(Support_SD_CARD  == 1)
#include "bsp_sdmmc_sdcard.h"
#endif

#if(Support_EX_FLASH  == 1)
#include "w25qxx.h"
#endif

#if(Support_EX_NAND == 1)
#include "nand.h"
#include "ftl.h"
#endif
/*
********************************************************************************
*                             MACRO DEFINITIONS
********************************************************************************
*/
#define SD_CARD	 						0  																//SD��,���Ϊ0
#define EX_FLASH 						1																//�ⲿspi flash,���Ϊ1
#define EX_NAND  						2																//�ⲿnand flash,���Ϊ2

#if(Support_EX_FLASH  == 1)
																										//����W25Q256
																										//ǰ25M�ֽڸ�fatfs��,25M�ֽں�,���ڴ���ֿ�,�ֿ�ռ��6.01M.	ʣ�ಿ��,���ͻ��Լ���
#define FLASH_SECTOR_SIZE 	512
#define FLASH_SECTOR_COUNT 	1024*25*2																	//W25Q256,ǰ25M�ֽڸ�FATFSռ��	
#define FLASH_BLOCK_SIZE   	8     																		//ÿ��BLOCK��8������
#endif
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



/*
********************************************************************************
*                              FUNCTION DEFINITION
********************************************************************************
*/

/**
 * @brief :��ô���״̬
 * @param :none.
 * @retval:none.
 */
DSTATUS disk_status (
    BYTE pdrv																						/* Physical drive nmuber to identify the drive */
)
{
    return RES_OK;
}

/**
 * @brief :��ʼ������
 * @param :none.
 * @retval:none.
 */
DSTATUS disk_initialize (
    BYTE pdrv																						/* Physical drive nmuber to identify the drive */
)
{
    uint8_t res=0;
    switch(pdrv)
    {
#if(Support_SD_CARD  == 1)
    case SD_CARD:																					//SD��
        res=SD_Init();																				//SD����ʼ��
        break;
#endif

#if(Support_EX_FLASH  == 1)
    case EX_FLASH:																					//�ⲿflash
        W25QXX_Init();  																			//W25QXX��ʼ��
        break;
#endif

#if(Support_EX_NAND == 1)
    case EX_NAND:																					//�ⲿNAND
        res=FTL_Init();																				//NAND��ʼ��
        break;
#endif
    default:
        res=1;
        break;
    }
    if(res)return  STA_NOINIT;
    else return 0; 																					//��ʼ���ɹ�
}

/**
 * @brief :	������
 * @param :
			pdrv:���̱��0~9
			*buff:���ݽ��ջ����׵�ַ
			sector:������ַ
			count:��Ҫ��ȡ��������
 * @retval:none.
 */
DRESULT disk_read (
    BYTE pdrv,																						/* Physical drive nmuber to identify the drive */
    BYTE *buff,																						/* Data buffer to store read data */
    DWORD sector,																					/* Sector address in LBA */
    UINT count																						/* Number of sectors to read */
)
{
    uint8_t res=0;
    if (!count)return RES_PARERR;																	//count���ܵ���0�����򷵻ز�������
    switch(pdrv)
    {
#if(Support_SD_CARD  == 1)
    case SD_CARD:																					//SD��
        res=SD_ReadDisk(buff,sector,count);
        while(res)																					//������
        {
            SD_Init();																				//���³�ʼ��SD��
            res=SD_ReadDisk(buff,sector,count);
            //printf("sd rd error:%d\r\n",res);
        }
        break;
#endif

#if(Support_EX_FLASH  == 1)
    case EX_FLASH:																					//�ⲿflash
        for(; count>0; count--)
        {
            W25QXX_Read(buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
            sector++;
            buff+=FLASH_SECTOR_SIZE;
        }
        res=0;
        break;
#endif

#if(Support_EX_NAND == 1)
    case EX_NAND:																					//�ⲿNAND
        res=FTL_ReadSectors(buff,sector,512,count);													//��ȡ����
        break;
#endif

    default:
        res=1;
    }
																									//������ֵ����SPI_SD_driver.c�ķ���ֵת��ff.c�ķ���ֵ
    if(res==0x00)return RES_OK;
    else return RES_ERROR;
}

/**
 * @brief :	д����
 * @param :
			pdrv:���̱��0~9
			*buff:���������׵�ַ
			sector:������ַ
			count:��Ҫд���������
 * @retval:none.
 */
DRESULT disk_write (
    BYTE pdrv,																						/* Physical drive nmuber to identify the drive */
    const BYTE *buff,																				/* Data to be written */
    DWORD sector,																					/* Sector address in LBA */
    UINT count																						/* Number of sectors to write */
)
{
    uint8_t res=0;
    if (!count)return RES_PARERR;																	//count���ܵ���0�����򷵻ز�������
    switch(pdrv)
    {
#if(Support_SD_CARD  == 1)
    case SD_CARD:																					//SD��
        res=SD_WriteDisk((uint8_t*)buff,sector,count);
        while(res)																					//д����
        {
            SD_Init();																				//���³�ʼ��SD��
            res=SD_WriteDisk((uint8_t*)buff,sector,count);
            //printf("sd wr error:%d\r\n",res);
        }
        break;
#endif

#if(Support_EX_FLASH  == 1)
    case EX_FLASH:																					//�ⲿflash
        for(; count>0; count--)
        {
            W25QXX_Write((uint8_t*)buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
            sector++;
            buff+=FLASH_SECTOR_SIZE;
        }
        res=0;
        break;
#endif

#if(Support_EX_NAND == 1)
    case EX_NAND:																					//�ⲿNAND
        res=FTL_WriteSectors((uint8_t*)buff,sector,512,count);										//д������
        break;
#endif

    default:
        res=1;
    }
																									//������ֵ����SPI_SD_driver.c�ķ���ֵת��ff.c�ķ���ֵ
    if(res == 0x00)return RES_OK;
    else return RES_ERROR;
}

/**
 * @brief :	����������Ļ��
 * @param :
			pdrv:���̱��0~9
			ctrl:���ƴ���
			*buff:����/���ջ�����ָ��
 * @retval:none.
 */
DRESULT disk_ioctl (
    BYTE pdrv,																						/* Physical drive nmuber (0..) */
    BYTE cmd,																						/* Control code */
    void *buff																						/* Buffer to send/receive control data */
)
{
    DRESULT res;
#if(Support_SD_CARD  == 1)
    if(pdrv==SD_CARD)																				//SD��
    {
        switch(cmd)
        {
        case CTRL_SYNC:
            res = RES_OK;
            break;
        case GET_SECTOR_SIZE:
            *(DWORD*)buff = 512;
            res = RES_OK;
            break;
        case GET_BLOCK_SIZE:
            *(WORD*)buff = SDCardInfo.CardBlockSize;
            res = RES_OK;
            break;
        case GET_SECTOR_COUNT:
            *(DWORD*)buff = SDCardInfo.CardCapacity/512;
            res = RES_OK;
            break;
        default:
            res = RES_PARERR;
            break;
        }
    }
#endif

#if(Support_EX_FLASH  == 1)
    if(pdrv==EX_FLASH)																				//�ⲿFLASH
    {
        switch(cmd)
        {
        case CTRL_SYNC:
            res = RES_OK;
            break;
        case GET_SECTOR_SIZE:
            *(WORD*)buff = FLASH_SECTOR_SIZE;
            res = RES_OK;
            break;
        case GET_BLOCK_SIZE:
            *(WORD*)buff = FLASH_BLOCK_SIZE;
            res = RES_OK;
            break;
        case GET_SECTOR_COUNT:
            *(DWORD*)buff = FLASH_SECTOR_COUNT;
            res = RES_OK;
            break;
        default:
            res = RES_PARERR;
            break;
        }
    }
#endif

#if(Support_EX_NAND == 1)
    if(pdrv==EX_NAND)																				//�ⲿNAND FLASH
    {
        switch(cmd)
        {
        case CTRL_SYNC:
            res = RES_OK;
            break;
        case GET_SECTOR_SIZE:
            *(WORD*)buff = 512;																		//NAND FLASH����ǿ��Ϊ512�ֽڴ�С
            res = RES_OK;
            break;
        case GET_BLOCK_SIZE:
            *(WORD*)buff = nand_dev.page_mainsize/512;												//block��С,�����һ��page�Ĵ�С
            res = RES_OK;
            break;
        case GET_SECTOR_COUNT:
            *(DWORD*)buff = nand_dev.valid_blocknum*nand_dev.block_pagenum*nand_dev.page_mainsize/512;	//NAND FLASH����������С
            res = RES_OK;
            break;
        default:
            res = RES_PARERR;
            break;
        }
    }
#endif

    else res=RES_ERROR;																				//�����Ĳ�֧��
    return res;
}

/**
 * @brief :	���ʱ��
			User defined function to give a current time to fatfs module
			31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31)
			15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2)
 * @param :	none.
 * @retval:	ʱ��.
 */
DWORD get_fattime (void)
{

    uint32_t time=0;

    time =(RTC_GetTimeYear()+2000-1980)<<25;														//��
    time |=(RTC_GetTimeMonth())<<21;																//��
    time |=(RTC_GetTimeDate())<<16;																	//��

    time |=(RTC_GetTimeHours())<<11;																//ʱ
    time |=(RTC_GetTimeMinutes())<<5;																//��
    time |=(RTC_GetTimeSeconds()/2);																//��

    return time;
}

/**
 * @brief :	��̬�����ڴ�
 * @param :	none.
 * @retval:	none.
 */
void *ff_memalloc (UINT size)
{
    return (void*)mymalloc(SRAMIN,size);
}

/**
 * @brief :	�ͷ��ڴ�
 * @param :	none.
 * @retval:	none.
 */
void ff_memfree (void* mf)
{
    myfree(SRAMIN,mf);
}

/**
 * @brief :	�����ٽ���
 * @param :	none.
 * @retval:	none.
 */
uint8_t cnt0=0;
void ff_enter(FATFS *fs)
{
    taskENTER_CRITICAL();																		//�����ٽ���(�޷����жϴ��)
    if(cnt0)
    {
        printf("in shit:%d\r\n",cnt0);
    }
    cnt0++;
}
/**
 * @brief :	�˳��ٽ���
 * @param :	none.
 * @retval:	none.
 */
void ff_leave(FATFS* fs)
{
    if(cnt0)
    {
        cnt0--;
        taskEXIT_CRITICAL();																	//�˳��ٽ���(���Ա��жϴ��)
    }
}

#endif
/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/









