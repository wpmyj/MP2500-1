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
#define  Support_SD_CARD                  1                      											//支持SD卡
#define  Support_EX_FLASH                 0                      											//支持外部spi flash
#define  Support_EX_NAND                  0                      											//支持外部nand flash
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
#define SD_CARD	 						0  																//SD卡,卷标为0
#define EX_FLASH 						1																//外部spi flash,卷标为1
#define EX_NAND  						2																//外部nand flash,卷标为2

#if(Support_EX_FLASH  == 1)
																										//对于W25Q256
																										//前25M字节给fatfs用,25M字节后,用于存放字库,字库占用6.01M.	剩余部分,给客户自己用
#define FLASH_SECTOR_SIZE 	512
#define FLASH_SECTOR_COUNT 	1024*25*2																	//W25Q256,前25M字节给FATFS占用	
#define FLASH_BLOCK_SIZE   	8     																		//每个BLOCK有8个扇区
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
 * @brief :获得磁盘状态
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
 * @brief :初始化磁盘
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
    case SD_CARD:																					//SD卡
        res=SD_Init();																				//SD卡初始化
        break;
#endif

#if(Support_EX_FLASH  == 1)
    case EX_FLASH:																					//外部flash
        W25QXX_Init();  																			//W25QXX初始化
        break;
#endif

#if(Support_EX_NAND == 1)
    case EX_NAND:																					//外部NAND
        res=FTL_Init();																				//NAND初始化
        break;
#endif
    default:
        res=1;
        break;
    }
    if(res)return  STA_NOINIT;
    else return 0; 																					//初始化成功
}

/**
 * @brief :	读扇区
 * @param :
			pdrv:磁盘编号0~9
			*buff:数据接收缓冲首地址
			sector:扇区地址
			count:需要读取的扇区数
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
    if (!count)return RES_PARERR;																	//count不能等于0，否则返回参数错误
    switch(pdrv)
    {
#if(Support_SD_CARD  == 1)
    case SD_CARD:																					//SD卡
        res=SD_ReadDisk(buff,sector,count);
        while(res)																					//读出错
        {
            SD_Init();																				//重新初始化SD卡
            res=SD_ReadDisk(buff,sector,count);
            //printf("sd rd error:%d\r\n",res);
        }
        break;
#endif

#if(Support_EX_FLASH  == 1)
    case EX_FLASH:																					//外部flash
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
    case EX_NAND:																					//外部NAND
        res=FTL_ReadSectors(buff,sector,512,count);													//读取数据
        break;
#endif

    default:
        res=1;
    }
																									//处理返回值，将SPI_SD_driver.c的返回值转成ff.c的返回值
    if(res==0x00)return RES_OK;
    else return RES_ERROR;
}

/**
 * @brief :	写扇区
 * @param :
			pdrv:磁盘编号0~9
			*buff:发送数据首地址
			sector:扇区地址
			count:需要写入的扇区数
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
    if (!count)return RES_PARERR;																	//count不能等于0，否则返回参数错误
    switch(pdrv)
    {
#if(Support_SD_CARD  == 1)
    case SD_CARD:																					//SD卡
        res=SD_WriteDisk((uint8_t*)buff,sector,count);
        while(res)																					//写出错
        {
            SD_Init();																				//重新初始化SD卡
            res=SD_WriteDisk((uint8_t*)buff,sector,count);
            //printf("sd wr error:%d\r\n",res);
        }
        break;
#endif

#if(Support_EX_FLASH  == 1)
    case EX_FLASH:																					//外部flash
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
    case EX_NAND:																					//外部NAND
        res=FTL_WriteSectors((uint8_t*)buff,sector,512,count);										//写入数据
        break;
#endif

    default:
        res=1;
    }
																									//处理返回值，将SPI_SD_driver.c的返回值转成ff.c的返回值
    if(res == 0x00)return RES_OK;
    else return RES_ERROR;
}

/**
 * @brief :	其他表参数的获得
 * @param :
			pdrv:磁盘编号0~9
			ctrl:控制代码
			*buff:发送/接收缓冲区指针
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
    if(pdrv==SD_CARD)																				//SD卡
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
    if(pdrv==EX_FLASH)																				//外部FLASH
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
    if(pdrv==EX_NAND)																				//外部NAND FLASH
    {
        switch(cmd)
        {
        case CTRL_SYNC:
            res = RES_OK;
            break;
        case GET_SECTOR_SIZE:
            *(WORD*)buff = 512;																		//NAND FLASH扇区强制为512字节大小
            res = RES_OK;
            break;
        case GET_BLOCK_SIZE:
            *(WORD*)buff = nand_dev.page_mainsize/512;												//block大小,定义成一个page的大小
            res = RES_OK;
            break;
        case GET_SECTOR_COUNT:
            *(DWORD*)buff = nand_dev.valid_blocknum*nand_dev.block_pagenum*nand_dev.page_mainsize/512;	//NAND FLASH的总扇区大小
            res = RES_OK;
            break;
        default:
            res = RES_PARERR;
            break;
        }
    }
#endif

    else res=RES_ERROR;																				//其他的不支持
    return res;
}

/**
 * @brief :	获得时间
			User defined function to give a current time to fatfs module
			31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31)
			15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2)
 * @param :	none.
 * @retval:	时间.
 */
DWORD get_fattime (void)
{

    uint32_t time=0;

    time =(RTC_GetTimeYear()+2000-1980)<<25;														//年
    time |=(RTC_GetTimeMonth())<<21;																//月
    time |=(RTC_GetTimeDate())<<16;																	//日

    time |=(RTC_GetTimeHours())<<11;																//时
    time |=(RTC_GetTimeMinutes())<<5;																//分
    time |=(RTC_GetTimeSeconds()/2);																//秒

    return time;
}

/**
 * @brief :	动态分配内存
 * @param :	none.
 * @retval:	none.
 */
void *ff_memalloc (UINT size)
{
    return (void*)mymalloc(SRAMIN,size);
}

/**
 * @brief :	释放内存
 * @param :	none.
 * @retval:	none.
 */
void ff_memfree (void* mf)
{
    myfree(SRAMIN,mf);
}

/**
 * @brief :	进入临界区
 * @param :	none.
 * @retval:	none.
 */
uint8_t cnt0=0;
void ff_enter(FATFS *fs)
{
    taskENTER_CRITICAL();																		//进入临界区(无法被中断打断)
    if(cnt0)
    {
        printf("in shit:%d\r\n",cnt0);
    }
    cnt0++;
}
/**
 * @brief :	退出临界区
 * @param :	none.
 * @retval:	none.
 */
void ff_leave(FATFS* fs)
{
    if(cnt0)
    {
        cnt0--;
        taskEXIT_CRITICAL();																	//退出临界区(可以被中断打断)
    }
}

#endif
/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/









