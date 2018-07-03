/*
***************************************************************************************************
*                                         APPLICATION CODE
*
*                      (c) Copyright 2017; Guangdong ENECO Science And Technology Co.,Ltd
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used without authorization.
***************************************************************************************************
*/

/********************************************************************************************************
* Filename      :  app_stack_manager.h
* Programmer(s) :  Fanjun
* Version       :  V1.0
* data          :  2017.4.4
* brief         :  All the functions prototypes for the stack manager task.
*
*********************************************************************************************************/
#ifndef __APP_STACK_MANAGER_H__
#define __APP_STACK_MANAGER_H__
/*
***************************************************************************************************
*                                                 MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               BSP present pre-processor macro definition.
***************************************************************************************************
*/
/*
***************************************************************************************************
*                                           INCLUDE FILES
***************************************************************************************************
*/
#include "os.h"
#include "stdint.h"
#include "app_system_real_time_parameters.h"

/*
***************************************************************************************************
*                                     MICRO DEFINITIONS
***************************************************************************************************
*/
#define  CYCLE_VENT                        (OS_FLAGS)0x01
#define  ANA_SIG_ALM_MON                   (OS_FLAGS)0x02 //模拟量运行警报监测
#define  CURRENT_DYNAMIC_ADJ               (OS_FLAGS)0x04 //动态调节电流
#define  CURRENT_SMOOTHLY_ADJ              (OS_FLAGS)0x08 //平滑调节电流

/*
***************************************************************************************************
*                                           TYPE DEFINITIONS
***************************************************************************************************
*/
typedef enum {
    DO_NOT_WAIT = 0,
    WAIT_FOR_36KPA,
    WAIT_FOR_45KPA,
} HYDRO_PRESS_WAIT_Typedef;

/*
***************************************************************************************************
*                                    EXPORTED OS VARIABLE DECLARATIONS
***************************************************************************************************
*/

extern      OS_TCB      StackManagerTaskTCB;
extern      OS_TCB      StackManagerDlyStopTaskTCB;
extern      OS_TCB      StackHydrogenYieldMatchingOffsetValueMonitorTaskTCB;
extern      OS_TCB      StackProgramControlAirPressureReleaseTaskTCB;

extern      OS_FLAG_GRP StackSubTaskSwGrp;
/*
***************************************************************************************************
*                                   EXPORTED GLOABLE VARIABLE DECLARATIONS
***************************************************************************************************
*/
extern      STACK_VENT_TIME_PARA_Typedef            StackVentAirTimeParameter;

/*
***************************************************************************************************
*                                           EXPORTED FUNCTION
***************************************************************************************************
*/
void        StackManagerTaskCreate(void);
void        StackManagerDlyStopTaskCreate(void);
void        StackHydrogenYieldMatchingOffsetValueMonitorTaskCreate(void);
void        StackProgramControlPressureReleaseTaskCreate(void);

uint8_t     GetStackProgramControlAirPressureReleaseTaskSwitch(void);
void        SetAnaSigAlmRunningMonitorHookSwitch(uint8_t);
void        SetStackOutAirValveStatus(uint8_t i_NewStatu);
uint8_t     GetStackOutAirValveStatus(void);

uint8_t     GetPassiveDecompressCountPerMinutes(void);
float       GetStackHydrogenYieldMatchOffsetValue(void);
uint8_t     GetSStackHydrogenMarginMonitorTaskSwitchStatus(void);


uint8_t     GetStackStopDlyStatu(void);

float       GetStackAmpIntegralSum(void);

uint8_t     GetStackFanSpdPidControlSwitchStatus(void);
void        SetStackFanSpdPidControlSwitch(uint8_t i_NewStatu);
/*
***************************************************************************************************
*                                             MODULE END
***************************************************************************************************
*/


#endif                                                          /* End of module include */
