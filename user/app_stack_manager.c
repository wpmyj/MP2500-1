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
 * @file      app_stack_manager.c
 * @author    FanJun
 * @version   V1.0.0
 * @date      11-June-2018
 * @brief     program entry
 * @attention none.
 *******************************************************************************
 */

/*
********************************************************************************
*                              INCLUDE FILES
********************************************************************************
*/
#include "bsp.h"
#include "app_stack_manager.h"
#include "app_dc_module_communicate_task.h"
#include "app_analog_signal_monitor_task.h"
#include "bsp_pid.h"
/*
***************************************************************************************************
*                                           MACRO DEFINITIONS
***************************************************************************************************
*/
#define STACK_MANAGER_TASK_STK_SIZE                                          256
#define STACK_MANAGER_DELAY_STOP_TASK_STK_SIZE                               100
#define STACK_HYDROGEN_YIELD_MATCHING_OFFSET_VALUE_MONITOR_TASK_STK_SIZE     256
#define STACK_CYALE_VENT_TASK_STK_SIZE                                       100

//大周期排气阶段限流参数
#define CURRENT_LIMIT_POINT_BEFORE_VENT              15.0
#define CURRENT_LIMIT_POINT_RESUME_FIRST_STEP        25.0
#define CURRENT_LIMIT_POINT_RESUME_SECOND_STEP       34.0

#define CURRENT_LIMIT_POINT_EVERY_VENT_CYCLE         30.0

#define CYCLE_INVALID_ACTION         0
#define CYCLE_VENT_STEP_FRONT        1
#define CYCLE_VENT_STEP              2
#define CYCLE_VENT_BEHIND_STEP       3

/*
***************************************************************************************************
*                                         OS-RELATED    VARIABLES
***************************************************************************************************
*/

OS_TCB      StackManagerTaskTCB;
OS_TCB      StackManagerDlyStopTaskTCB;
OS_TCB      StackHydrogenYieldMatchingOffsetValueMonitorTaskTCB;
OS_TCB      StackCycleVentTaskTaskTCB;

OS_FLAG_GRP               StackSubTaskSwGrp;
static      OS_SEM                    StackManagerStopSem;

static      CPU_STK_8BYTE_ALIGNED     StackManagerTaskStk[STACK_MANAGER_TASK_STK_SIZE];
static      CPU_STK                   StackManagerDlyStopTaskStk[STACK_MANAGER_DELAY_STOP_TASK_STK_SIZE];
static      CPU_STK_8BYTE_ALIGNED     StackHydrogenYieldMatchingOffsetValueMonitorStk[STACK_HYDROGEN_YIELD_MATCHING_OFFSET_VALUE_MONITOR_TASK_STK_SIZE];
static      CPU_STK                   StackCycleVentTaskStk[STACK_CYALE_VENT_TASK_STK_SIZE];
/*
***************************************************************************************************
*                                          GLOBAL VARIABLES
***************************************************************************************************
*/

uint8_t            g_eStackManagerStopDlyStatu = DEF_OFF;
STACK_VENT_TIME_PARA_Typedef    StackVentAirTimeParameter = {0, 0.0, 0.0}; //电堆排气时间参数

/*
***************************************************************************************************
*                                           LOCAL VARIABLES
***************************************************************************************************
*/
static  float                           g_fHydrogenYieldMatchOffsetValue = 0.0;//电堆匹氢偏移值
static  uint8_t                         g_u8DecompressCountPerMinute = 0;//每分钟电堆尾部泄压排气次数
static  uint8_t                         g_u8StackFanPidControlSw = DEF_DISABLED;//电堆风扇自动调速开关
static  uint8_t                         g_u8StackHydrogenYieldMatchingOffsetValueMonitorTaskSw = DEF_DISABLED;

/*
***************************************************************************************************
*                                         FUNCTION PROTOTYPES
***************************************************************************************************
*/
static      void        StackManagerTask(void);
static      void        StackManagerDlyStopTask(void);
static      void        StackCycleVentTask(void);
static      void        StackHydrogenYieldMatchingOffsetValueMonitorTask(void);

static      void        StackCycleVentTaskCreate(void);
static      void        SetStackHydrogenYieldMatchingOffsetValueMonitorTaskSwitch(uint8_t i_NewStatu);

static      void        StackManagerTask(void);
static      void        StackManagerDlyStopTask(void);
/*
***************************************************************************************************
*                                      CREATE STACK MANAGER TASK
*
* Description:  This function creates the stack manager task.
*
* Arguments  :  none
*
* Returns    :  none
***************************************************************************************************
*/

void StackManagerTaskCreate(void)
{
    OS_ERR  err;

    OSSemCreate(&StackManagerStopSem, "Stack manager sem", 0, &err);

    OSTaskCreate((OS_TCB *)&StackManagerTaskTCB,                    // Create the start task
                 (CPU_CHAR *)"Stack manager task",
                 (OS_TASK_PTR) StackManagerTask,
                 (void *) 0,
                 (OS_PRIO) STACK_MANAGER_TASK_PRIO,
                 (CPU_STK *)&StackManagerTaskStk[0],
                 (CPU_STK_SIZE) STACK_MANAGER_TASK_STK_SIZE / 10,
                 (CPU_STK_SIZE) STACK_MANAGER_TASK_STK_SIZE,
                 (OS_MSG_QTY) 5u,
                 (OS_TICK) 0u,
                 (void *) 0,
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR *)&err);
    APP_TRACE_INFO(("Created stack manager task, and err code is %d...\n\r", err));
}

/*
***************************************************************************************************
*                                      STACK MANAGER TASK
*
* Description:  The stack manager task.
*
* Arguments  :  none
*
* Returns    :  none
***************************************************************************************************
*/
static void StackManagerTask(void)
{
    OS_ERR      err;
    uint16_t    u16StackFanSpeed = 0;
    float       fCurrent = 0.0;
    float       fOptimumStackTemp = 0.0;
    static      uint16_t u16AmpIntegralSum = 0;
    static      uint8_t   short_congtol_cnt = 0;
    while(DEF_TRUE) {

        OSTaskSuspend(NULL, &err);

        StackWorkTimesInc();
        IncrementType_PID_Init();   //PID参数初始化

        H2_IN_VALVE_PWR_ON;
        APP_TRACE_INFO(("Stack manager start, waitting the hydrogen press up to 36KPa ...\r\n"));
        SetHydrogenPressArrivedWaitSwitch(WAIT_FOR_36KPA);
        OSTaskSemPend(0, OS_OPT_PEND_BLOCKING, NULL, &err);//接收模拟信号监测任务中气压监测任务信号量

        APP_TRACE_INFO(("Start the stack start purify...\r\n"));
        SetStackFanCtrlSpd(1000);
        STACK_H2_OUT_VALVE_PWR_ON;
        OSTimeDlyHMSM(0, 0, 3, 0, OS_OPT_TIME_HMSM_STRICT, &err);
        SetStackFanCtrlSpd(200);
        OSTimeDlyHMSM(0, 0, 2, 0, OS_OPT_TIME_HMSM_STRICT, &err);
        STACK_H2_OUT_VALVE_PWR_OFF;
        APP_TRACE_INFO(("Finished the stack start purify...\r\n"));

        APP_TRACE_INFO(("Stack manager start, waitting the hydrogen press up to 45KPa ...\n\r"));
        SetHydrogenPressArrivedWaitSwitch(WAIT_FOR_45KPA);

        OSTaskSemPend(0, OS_OPT_PEND_BLOCKING, NULL, &err); //接收模拟信号监测任务中气压监测任务信号量

        DC_CONNECTER_POWER_ON;
        DC_OUT_VALVE_POWER_ON;//DC尾部电磁阀

        SetStackWorkStatu(EN_IN_WORK);

//        StackHydrogenYieldMatchingOffsetValueMonitorTaskCreate();//电堆匹氢偏移量监测任务创建
        StackCycleVentTaskCreate();//电堆周期排气任务创建

        OSFlagPost(&StackSubTaskSwGrp,
                   CYCLE_VENT + ANA_SIG_ALM_MON + CURRENT_DYNAMIC_ADJ + CURRENT_SMOOTHLY_ADJ,
                   OS_OPT_POST_FLAG_SET,
                   &err);
//        SetStackHydrogenYieldMatchingOffsetValueMonitorTaskSwitch(DEF_ENABLED);
        SetDCModuleDynamicAdjustTaskSwitch(DEF_ENABLED);   //DC动态限流任务开关
        SetAnaSigAlmRunningMonitorHookSwitch(DEF_ENABLED);
        SetStackFanSpdPidControlSwitch(DEF_ENABLED);


        SetResumeCurrentLimitSmoothlyFlagStatus(START_STEP_RESUME);//设置启动阶段平滑限流延时

//        OSTaskResume(&StackHydrogenYieldMatchingOffsetValueMonitorTaskTCB, &err);  //恢复匹氢偏移监测任务
//        OSTaskResume(&StackShortCircuitTaskTCB, &err);    //恢复短路活化任务
        OSTaskResume(&CurrentLimitResumeSmoothlyTaskTCB,&err);  //恢复平滑限流任务
        OSTaskResume(&StackCycleVentTaskTaskTCB,&err);      //恢复周期排气任务

        while(DEF_TRUE) {

            OSSemPend(&StackManagerStopSem,
                      OS_CFG_TICK_RATE_HZ ,
                      OS_OPT_PEND_BLOCKING,
                      NULL,
                      &err);

            if(err == OS_ERR_NONE) {
                break;
            }
            //安培秒排气
            fCurrent = GetSrcAnaSig(STACK_CURRENT);
            u16AmpIntegralSum += (uint16_t)fCurrent;
            if(u16AmpIntegralSum >= g_u16RunPurifyAmpIntegralValue) {
                if(0 == GET_STACK_H2_OUT_VALVE_STATUS) {
                    STACK_H2_OUT_VALVE_PWR_ON;
                    OSTimeDlyHMSM(0, 0, 0, 600,OS_OPT_TIME_HMSM_STRICT,&err);
                    STACK_H2_OUT_VALVE_PWR_OFF;
                    u16AmpIntegralSum = 0;
                }
            }

            //风机PID控制程序
            if(EN_CONTROL_MODE_AUTO == GetControlMode()) {
                if(g_u8StackFanPidControlSw == DEF_ENABLED) {
                    IPID.CalcCycleCount++;
                    if(IPID.CalcCycleCount >= IPID.Tsam) {
                        fOptimumStackTemp = CalcStackOptimalTempByCurrent();//计算最佳温度
                        u16StackFanSpeed = IncType_PID_Process(fOptimumStackTemp);
                        SetStackFanCtrlSpd(u16StackFanSpeed);
                        IPID.CalcCycleCount = 0;
                    }
                } else {}
            }

            //保持限流不中断
            short_congtol_cnt ++;
            if(short_congtol_cnt >= 8) {
                SetDcModeOutPutNominalVoltageButDifferentCurrent(g_fIvalueNow);
                short_congtol_cnt = 0;
            }
        }

        OSFlagPost(&StackSubTaskSwGrp,
                   CYCLE_VENT + ANA_SIG_ALM_MON + CURRENT_DYNAMIC_ADJ + CURRENT_SMOOTHLY_ADJ,
                   OS_OPT_POST_FLAG_CLR,
                   &err);
//        SetStackHydrogenYieldMatchingOffsetValueMonitorTaskSwitch(DEF_DISABLED);
        SetAnaSigAlmRunningMonitorHookSwitch(DEF_DISABLED);
        SetDCModuleDynamicAdjustTaskSwitch(DEF_DISABLED);
        APP_TRACE_INFO(("Stack manager stop...\n\r"));
    }
}
/*
***************************************************************************************************
*                            SetStackFanSpdPidControlSwitch()
*
* Description:  Enable or Disable the stack fan auto adjust.
*
* Arguments  :  none
*
* Returns    :  none
***************************************************************************************************
*/
void SetStackFanSpdPidControlSwitch(uint8_t i_NewStatu)
{
    g_u8StackFanPidControlSw = i_NewStatu;
}

uint8_t GetStackFanSpdPidControlSwitchStatus(void)
{
    return g_u8StackFanPidControlSw;
}
/*
***************************************************************************************************
*                                    StackManagerDlyStopTaskCreate()
*
* Description:  This function creates the stack manager delay stop task.
*
* Arguments  :  none
*
* Returns    :  none
***************************************************************************************************
*/
void StackManagerDlyStopTaskCreate()
{
    OS_ERR  err;
    OSTaskCreate((OS_TCB *)&StackManagerDlyStopTaskTCB,
                 (CPU_CHAR *)"Stack manager delay stop task",
                 (OS_TASK_PTR) StackManagerDlyStopTask,
                 (void *) 0,
                 (OS_PRIO) STACK_MANAGER_DELAY_STOP_TASK_PRIO,
                 (CPU_STK *)&StackManagerDlyStopTaskStk[0],
                 (CPU_STK_SIZE) STACK_MANAGER_DELAY_STOP_TASK_STK_SIZE / 10,
                 (CPU_STK_SIZE) STACK_MANAGER_DELAY_STOP_TASK_STK_SIZE,
                 (OS_MSG_QTY) 5u,
                 (OS_TICK) 0u,
                 (void *) 0,
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR *)&err);
    APP_TRACE_INFO(("Created the stack manager delay stop task, err code is %d...\n\r", err));
}
/*
***************************************************************************************************
*                                  StackManagerDlyStopTask()
*
* Description:  The stack manager delay stop task.
*
* Arguments  :  none
*
* Returns    :  none
***************************************************************************************************
*/
void StackManagerDlyStopTask(void)
{
    OS_ERR      err;
    uint16_t    u16ShutDownStackFanDlySeconds = 0;  //电堆延时关闭时间计时

    while(DEF_TRUE) {

        OSTaskSuspend(NULL,&err);
        APP_TRACE_INFO(("The stack manager start to delay stop...\n\r"));

        g_eStackManagerStopDlyStatu = DEF_ON;
        SetStackFanSpdPidControlSwitch(DEF_DISABLED);  //关闭风机自动调速

        OSSemPost(&StackManagerStopSem,OS_OPT_POST_1,&err);

        SetStackFanCtrlSpd(1000);

        while(DEF_TRUE) {
            OSTimeDlyHMSM(0, 0, 1, 0,
                          OS_OPT_TIME_HMSM_STRICT,
                          &err);
            u16ShutDownStackFanDlySeconds ++;

            if(u16ShutDownStackFanDlySeconds == 2) { //关闭直流接触器,停止输出
                DC_CONNECTER_POWER_OFF;
            } else if(u16ShutDownStackFanDlySeconds == 5) { //先排一秒气
                STACK_H2_OUT_VALVE_PWR_ON;
            } else if(u16ShutDownStackFanDlySeconds == 6) { //再关闭进气阀
                STACK_H2_OUT_VALVE_PWR_OFF;
                H2_IN_VALVE_PWR_OFF;
            } else if(u16ShutDownStackFanDlySeconds >= 30) {//电堆风扇延时30S后关闭，同时挂起电堆停止任务

                OSTaskSemPost(&AppTaskStartTCB, //发送给主任务内的shutdown函数任务信号量响应半机2电堆延时关闭任务
                              OS_OPT_POST_NO_SCHED,
                              &err);
                u16ShutDownStackFanDlySeconds = 0;

                APP_TRACE_INFO(("The stack delay stop time arrive 30s,task sem send...\n\r"));
                break;
            } else {}
        }

        SetStackWorkStatu(EN_NOT_IN_WORK);
        SetStackFanCtrlSpd(0);

        DC_CONNECTER_POWER_OFF;
        DC_OUT_VALVE_POWER_OFF;
        g_eStackManagerStopDlyStatu = DEF_OFF;
    }
}
/*
***************************************************************************************************
*                             GetStackStopDlyStatu()
*
* Description:  Get the delay statu of the stack stop work.
*
* Arguments  :  none
*
* Returns    :  none
***************************************************************************************************
*/
uint8_t GetStackStopDlyStatu(void)
{
    return g_eStackManagerStopDlyStatu;
}
/*
***************************************************************************************************
*                       StackHydrogenYieldMatchingOffsetValueMonitorTaskCreate()
*
* Description : Stack hydrogen yield matching offset value monitor task creat.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Note(s)     : none.
***************************************************************************************************
*/
void    StackHydrogenYieldMatchingOffsetValueMonitorTaskCreate()
{
    OS_ERR      err;

    OSTaskCreate((OS_TCB *)&StackHydrogenYieldMatchingOffsetValueMonitorTaskTCB,
                 (CPU_CHAR *)"Stack hydrogen yield matching offset value monitor Task",
                 (OS_TASK_PTR) StackHydrogenYieldMatchingOffsetValueMonitorTask,
                 (void *) 0,
                 (OS_PRIO) STACK_HYDROGEN_YIELD_MATCHING_OFFSET_VALUE_MONITOR_TASK_PRIO,
                 (CPU_STK *) &StackHydrogenYieldMatchingOffsetValueMonitorStk[0],
                 (CPU_STK_SIZE) STACK_HYDROGEN_YIELD_MATCHING_OFFSET_VALUE_MONITOR_TASK_STK_SIZE / 10,
                 (CPU_STK_SIZE) STACK_HYDROGEN_YIELD_MATCHING_OFFSET_VALUE_MONITOR_TASK_STK_SIZE,
                 (OS_MSG_QTY) 5u,
                 (OS_TICK) 0u,
                 (void *) 0,
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR *)&err);
    APP_TRACE_INFO(("Created the Stack V-hymo value monitor task, and err code is %d...\n\r", err));
}


/*
***************************************************************************************************
*                        StackCycleVentTaskCreate()
*
* Description:  Create the cycle vent task.
*
* Arguments  :  none
*
* Returns    :  none
***************************************************************************************************
*/
static void StackCycleVentTaskCreate(void)
{
    OS_ERR  err;
    OSTaskCreate((OS_TCB *)&StackCycleVentTaskTaskTCB,
                 (CPU_CHAR *)"Stack cycle vent task",
                 (OS_TASK_PTR) StackCycleVentTask,
                 (void *) 0,
                 (OS_PRIO) STACK_CYALE_VENT_TASK_PRO,
                 (CPU_STK *)&StackCycleVentTaskStk[0],
                 (CPU_STK_SIZE) STACK_CYALE_VENT_TASK_STK_SIZE / 10,
                 (CPU_STK_SIZE) STACK_CYALE_VENT_TASK_STK_SIZE,
                 (OS_MSG_QTY) 5u,
                 (OS_TICK) 0u,
                 (void *) 0,
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR *)&err);
    APP_TRACE_INFO(("Create the cycle vent task, err code is %d...\n\r", err));
}


/*
***************************************************************************************************
*                        StackCycleVentTask()
*
* Description:  The cycle vent task.
*
* Arguments  :  none
*
* Returns    :  none
***************************************************************************************************
*/
static void StackCycleVentTask(void)
{
    OS_ERR      err;
    uint8_t    u8CycleVentCount = 0;
    uint8_t    u8LadderResumePowerFlag = DEF_NO;

    while(DEF_TRUE) {

        OSTaskSuspend(NULL, &err);
        APP_TRACE_INFO(("Resume stack cycle vent task...\n\r"));

        while(DEF_TRUE) {

            OSFlagPend(&StackSubTaskSwGrp,
                       CYCLE_VENT,
                       OS_CFG_TICK_RATE_HZ * 80,//80s周期排气
                       OS_OPT_PEND_FLAG_CLR_ANY + OS_OPT_PEND_BLOCKING,
                       NULL,
                       &err);
            if(err == OS_ERR_NONE) {
                APP_TRACE_INFO(("Stack cycle vent task break...\n\r"));
                break;
            } else if(err == OS_ERR_TIMEOUT) {

                u8CycleVentCount ++;
                if(u8CycleVentCount >= 3) { //3次周期排气进行一次限流排气
                    g_u8StackFanPidControlSw = DEF_DISABLED;//排气限流前停止风机PID
                    //排气前2s限制输出为800W
                    SetDcModeOutPutNominalVoltageButDifferentCurrent(CURRENT_LIMIT_POINT_BEFORE_VENT);
                    u8LadderResumePowerFlag = DEF_YES;
                    u8CycleVentCount = 0;
                    APP_TRACE_INFO(("Stack cycle venting and current limit...\n\r"));
                }

                OSTimeDlyHMSM(0, 0, 2, 0,OS_OPT_TIME_HMSM_STRICT,&err);
                STACK_H2_OUT_VALVE_PWR_ON;
                OSTimeDlyHMSM(0, 0, 1, 200,OS_OPT_TIME_HMSM_STRICT,&err);//排气1.2s
                STACK_H2_OUT_VALVE_PWR_OFF;

                if(u8LadderResumePowerFlag == DEF_YES) {
                    //阶梯恢复功率
                    OSTimeDlyHMSM(0, 0, 5, 0,OS_OPT_TIME_HMSM_STRICT,&err);
                    SetDcModeOutPutNominalVoltageButDifferentCurrent(CURRENT_LIMIT_POINT_RESUME_FIRST_STEP);
                    OSTimeDlyHMSM(0, 0, 5, 0,OS_OPT_TIME_HMSM_STRICT,&err);
                    SetDcModeOutPutNominalVoltageButDifferentCurrent(CURRENT_LIMIT_POINT_RESUME_SECOND_STEP);
                    u8LadderResumePowerFlag = DEF_NO;
                }
            }

        }
    }
}
/*
***************************************************************************************************
*                            GetPassiveDecompressCountPerMinutes()
*
* Description:  电堆尾部泄压排气次数.
*
* Arguments  :  none
*
* Returns    :  none
***************************************************************************************************
*/
uint8_t GetPassiveDecompressCountPerMinutes(void)
{
    return g_u8DecompressCountPerMinute;
}
/*
***************************************************************************************************
*                            SetStackProgramControlAirPressureReleaseTaskSwitch()
*
* Description:  Enable or Disable the stack fan auto adjust.
*
* Arguments  :  none
*
* Returns    :  none
***************************************************************************************************
*/
static void SetStackHydrogenYieldMatchingOffsetValueMonitorTaskSwitch(uint8_t i_NewStatu)
{
    g_u8StackHydrogenYieldMatchingOffsetValueMonitorTaskSw = i_NewStatu;
}

uint8_t GetStackHydrogenYieldMatchingOffsetValueMonitorTaskSwitchStatus()
{
    return g_u8StackHydrogenYieldMatchingOffsetValueMonitorTaskSw ;
}

/************************ (C) COPYRIGHT 2017 Guangdong ENECO *****END OF FILE *********************/
