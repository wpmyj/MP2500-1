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
 * @file      GUIConf.c
 * @author    GuanXianDe
 * @version   V1.0.0
 * @date      27-June-2018
 * @brief     program entry
 * @attention none.
 *******************************************************************************
 */

/*
********************************************************************************
*                              INCLUDE FILES
********************************************************************************
*/
#include "user_config.h"
#if(USE_STemwin_File == 1)
#include "GUI.h"
#include "malloc.h"
#if(USE_Sdram_File == 1)
#include "bsp_sdram.h"
#endif

/*
********************************************************************************
*                             MACRO DEFINITIONS
********************************************************************************
*/
#define USE_EXRAM  0	//使用外部RAM
//设置EMWIN内存大小
#define GUI_NUMBYTES  (40*1024)//(8*1024*1024)
#define GUI_BLOCKSIZE 0X80  //块大小
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
 * @brief :	GUI_X_Config(初始化的时候调用,用来设置emwin所使用的内存)
 * @param :	none.
 * @retval:	none.
 */
void GUI_X_Config(void) {
    if(USE_EXRAM) //使用外部RAM
    {
        U32 *aMemory = mymalloc(SRAMEX,GUI_NUMBYTES); //从外部SRAM中分配GUI_NUMBYTES字节的内存
        GUI_ALLOC_AssignMemory((void*)aMemory, GUI_NUMBYTES); //为存储管理系统分配一个存储块
        GUI_ALLOC_SetAvBlockSize(GUI_BLOCKSIZE); //设置存储快的平均尺寸,该区越大,可用的存储快数量越少
        GUI_SetDefaultFont(GUI_FONT_6X8); //设置默认字体
    } else  //使用内部RAM
    {
        U32 *aMemory = mymalloc(SRAMIN,GUI_NUMBYTES); //从内部RAM中分配GUI_NUMBYTES字节的内存
        GUI_ALLOC_AssignMemory((U32 *)aMemory, GUI_NUMBYTES); //为存储管理系统分配一个存储块
        GUI_ALLOC_SetAvBlockSize(GUI_BLOCKSIZE); //设置存储快的平均尺寸,该区越大,可用的存储快数量越少
        GUI_SetDefaultFont(GUI_FONT_6X8); //设置默认字体
    }
}
#endif


/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/





