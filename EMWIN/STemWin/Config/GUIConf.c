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
#define USE_EXRAM  0	//ʹ���ⲿRAM
//����EMWIN�ڴ��С
#define GUI_NUMBYTES  (40*1024)//(8*1024*1024)
#define GUI_BLOCKSIZE 0X80  //���С
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
 * @brief :	GUI_X_Config(��ʼ����ʱ�����,��������emwin��ʹ�õ��ڴ�)
 * @param :	none.
 * @retval:	none.
 */
void GUI_X_Config(void) {
    if(USE_EXRAM) //ʹ���ⲿRAM
    {
        U32 *aMemory = mymalloc(SRAMEX,GUI_NUMBYTES); //���ⲿSRAM�з���GUI_NUMBYTES�ֽڵ��ڴ�
        GUI_ALLOC_AssignMemory((void*)aMemory, GUI_NUMBYTES); //Ϊ�洢����ϵͳ����һ���洢��
        GUI_ALLOC_SetAvBlockSize(GUI_BLOCKSIZE); //���ô洢���ƽ���ߴ�,����Խ��,���õĴ洢������Խ��
        GUI_SetDefaultFont(GUI_FONT_6X8); //����Ĭ������
    } else  //ʹ���ڲ�RAM
    {
        U32 *aMemory = mymalloc(SRAMIN,GUI_NUMBYTES); //���ڲ�RAM�з���GUI_NUMBYTES�ֽڵ��ڴ�
        GUI_ALLOC_AssignMemory((U32 *)aMemory, GUI_NUMBYTES); //Ϊ�洢����ϵͳ����һ���洢��
        GUI_ALLOC_SetAvBlockSize(GUI_BLOCKSIZE); //���ô洢���ƽ���ߴ�,����Խ��,���õĴ洢������Խ��
        GUI_SetDefaultFont(GUI_FONT_6X8); //����Ĭ������
    }
}
#endif


/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/





