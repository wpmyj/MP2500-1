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
 * @file      bsp_pin.h
 * @author    GuanXianDe
 * @version   V1.0.0
 * @date      02-July-2018
 * @brief     Header for main.c module
 * @attention
 *******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_PIN_H
#define __BSP_PIN_H

/*
********************************************************************************
*                                 INCLUDE FILES
********************************************************************************
*/

//#include "sys.h"

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


/*
********************************************************************************
*                                GPIO PIN DEFINITIONS
********************************************************************************
*/
//管脚定义分为:组别+管脚号
/* --------------------------------- GPIOA PINS ------------------------------ */
#define  BSP_PORT_GPIOA                                         GPIOA
#define  H_RVD_THERMOCOUPLE_CH7                                 GPIO_PIN_0		//热电偶7（预留2）(热电偶CS7)						
#define  FC_CURRENT_ANA_SIG_CH1_PIN                             GPIO_PIN_1  	//电流传感器（电堆）							ADC_CH1
#define  INTERFACE_USART2_TX_PIN                                GPIO_PIN_2		//4G模块数据传输接口 发送						USART2_TX
#define  INTERFACE_USART2_RX_PIN                                GPIO_PIN_3  	//4G模块数据传输接口 接收    					USART2_RX
#define  H_PUMP_SPD_ANA_SIG_CTRL_PIN                            GPIO_PIN_4		//制氢机泵速模拟量信号控制						DAC_OUT1
#define  H_FAN_SPD_ANA_SIG_CTRL_PIN                             GPIO_PIN_5		//制氢机风机模拟量信号控制脚					DAC_OUT2
#define  SYS_BATTERY_CURRENT_ANA_SIG_PIN                        GPIO_PIN_6		//电池电流传感器								ADC12_IN6
#define  H_HEATER_CURRENT_ANA_SIG_PIN                           GPIO_PIN_7		//制氢机加热器电流传感器						ADC12_IN7
#define  SYS_SHUT_DOWN_KEY_EXTI_PIN                             GPIO_PIN_8		//关机按键(三秒关机)							EXTI
#define  H_PUMP_SPD_FB_SIG_PIN                               	GPIO_PIN_9		//制氢机水泵速度反馈							TIM1_CH2（捕获）
#define  H_FAN_SPD_FB_SIG_PIN                                	GPIO_PIN_10		//制氢机风机速度反馈							TIM1_CH3（捕获）
#define  CAN1_RX_OR_FS_DM_PIN                                   GPIO_PIN_11		//CAN1 OR USB									CAN1_RX OR USB_DM
#define  CAN1_TX_OR_FS_DP_PIN                                   GPIO_PIN_12 	//CAN1 OR USB									CAN1_TX OR USB_DP
#define  DBG_SWDIO_PIN                                          GPIO_PIN_13		//SWD调试口SWDIO
#define  DBG_SWCLK_PIN                                          GPIO_PIN_14		//SWD调试口SWCLK
#define  SYS_START_KEY_EXTI_PIN                                 GPIO_PIN_15		//启动按键(一键启动)							EXTI

/* --------------------------------- GPIOB PINS ------------------------------ */
#define  BSP_PORT_GPIOB                                         GPIOB
#define  RVD2_ANA_SIG_PIN                           			GPIO_PIN_0		//预留传感器2									ADC12_IN9
#define  RVD1_ANA_SIG_PIN                           			GPIO_PIN_1		//预留传感器1									ADC12_IN8
#define  BUZZER_PWR_CTRL_PIN                        			GPIO_PIN_2		//蜂鸣器控制
#define  SYS_220_AC_MONITOR_EXTI_PIN                			GPIO_PIN_3		//市电掉电监测									EXTI
#define  SYS_SELF_CHECK_SELECTOR_EN3_CTRL_PIN                	GPIO_PIN_4		//自检数据选择器使能引脚（第3组）
#define  USART5_RX_PIN                                          GPIO_PIN_5  	//485-I接口										USART5_RX
#define  USART5_TX_PIN                                          GPIO_PIN_6		//485-I接口										USART5_TX
#define  SYS_SELF_CHECK_SELECTOR3_OUTPUT_DATA_PIN              	GPIO_PIN_7  	//自检数据选择器3数据输出脚（第3组）
#define  FC_INLET_VENT_MONITOR_PIN                				GPIO_PIN_8		//电堆进气口排气次数监测						中断计数
#define  FC_TAIL_VENT_MONITOR_PIN                				GPIO_PIN_9		//电堆出气口排气次数监测						中断计数
#define  INTERFACE_USART3_TX_PIN                                GPIO_PIN_10 	//485-II接口									USART3_TX
#define  INTERFACE_USART3_RX_PIN                                GPIO_PIN_11  	//485-II接口									USART3_RX
#define  INTERFACE_CAN2_RX_PIN                                  GPIO_PIN_12		//CAN2											CAN2_RX
#define  INTERFACE_CAN2_TX_PIN                                  GPIO_PIN_13 	//CAN2											CAN2_TX
#define  INTERFACE_USART1_TX_PIN                                GPIO_PIN_14 	//程序下载&调试口								USART1_TX
#define  INTERFACE_USART1_RX_PIN                                GPIO_PIN_15		//程序下载&调试口								USART1_RX
/* --------------------------------- GPIOC PINS ------------------------------ */
#define  BSP_PORT_GPIOC                                         GPIOC
#define  H_LQD_LEVEL1_ANA_SIG_PIN                            	GPIO_PIN_0		//液位传感器1 小水箱							ADC123_IN10
#define  H_LQD_LEVEL2_ANA_SIG_PIN                             	GPIO_PIN_1		//液位传感器2 大水箱							ADC123_IN11
#define  H_LQD_PRESS_ANA_SIG_PIN                             	GPIO_PIN_2		//液位传感器									ADC12_IN12
#define  FC_H2_PRESS1_ANA_SIG_PIN                             	GPIO_PIN_3		//气压传感器1									ADC12_IN13
#define  FC_H2_PRESS2_ANA_SIG_PIN                             	GPIO_PIN_4		//气压传感器2									ADC12_IN14
#define  H_NEGATIVE_PRESSURE_ANA_SIG_PIN                        GPIO_PIN_5		//负压传感器信号								ADC12_IN15
#define  INTERFACE_USART6_TX_PIN                                GPIO_PIN_6 		//DC限流接口									USART6_TX
#define  INTERFACE_USART6_RX_PIN                                GPIO_PIN_7		//DC限流接口									USART6_RX
#define  INTERFACE_SD_CARD_D0_PIN                               GPIO_PIN_8		//SD卡数据脚D0									SD_CARD_D0
#define  INTERFACE_SD_CARD_D1_PIN                               GPIO_PIN_9		//SD卡数据脚D1									SD_CARD_D1
#define  INTERFACE_SD_CARD_D2_PIN                               GPIO_PIN_10		//SD卡数据脚D2									SD_CARD_D2
#define  INTERFACE_SD_CARD_D3_PIN                               GPIO_PIN_11		//SD卡数据脚D3									SD_CARD_D3
#define  INTERFACE_SD_CARD_CLK_PIN                              GPIO_PIN_12		//SD卡时钟脚									SD_CARD_CLK
#define  SYS_DOOR_FB_SIG_PIN                                    GPIO_PIN_13    //门禁信号反馈脚
//14.15引脚为32.768K晶振

/* --------------------------------- GPIOD PINS ------------------------------ */
#define  BSP_PORT_GPIOD                                         GPIOD

#define  FSMC_D02_PIN                                          	GPIO_PIN_0		//FSMC											FSMC_D02
#define  FSMC_D03_PIN                                          	GPIO_PIN_1		//FSMC											FSMC_D03
#define  INTERFACE_SD_CARD_CMD_PIN                              GPIO_PIN_2		//SD卡指令脚									SD_CARD_CMD
#define  SYS_SELF_CHECK_SELECTOR_EN2_CTRL_PIN                	GPIO_PIN_3		//自检数据选择器使能引脚（第2组）
#define  INTERFACE_FSMC_RD_PIN                                  GPIO_PIN_4		//FSMC											FSMC_RD
#define  INTERFACE_FSMC_WR_PIN                                  GPIO_PIN_5		//FSMC											FSMC_WR
#define  SYS_RVD_HEART_LED_CTRL_PIN                             GPIO_PIN_6		//预留系统心跳灯控制脚（空）
#define  INTERFACE_LCD_TOUCH_SPI1_MOSI_PIN                      GPIO_PIN_7		//LCD触摸数据SPI输出引脚      					SPI1_MOSI
#define  INTERFACE_FSMC_D13_PIN                                 GPIO_PIN_8		//FSMC											FSMC_D13
#define  INTERFACE_FSMC_D14_PIN                                 GPIO_PIN_9		//FSMC											FSMC_D14
#define  INTERFACE_FSMC_D15_PIN                                 GPIO_PIN_10		//FSMC											FSMC_D15
#define  INTERFACE_FSMC_RST_PIN                                 GPIO_PIN_11		//FSMC											FSMC_RST
#define  RVD_48V_HEART_PWR_CTRL_PIN                             GPIO_PIN_12		//预留48V电源控制脚
#define  INTERFACE_FSMC_RS_PIN                                  GPIO_PIN_13		//FSMC											FSMC_RS
#define  INTERFACE_FSMC_D00_PIN                                 GPIO_PIN_14		//FSMC											FSMC_D00
#define  INTERFACE_FSMC_D01_PIN                                 GPIO_PIN_15		//FSMC											FSMC_D01

/* --------------------------------- GPIOE PINS ------------------------------ */
#define  BSP_PORT_GPIOE                                         GPIOE

#define  INTERFACE_USART8_RX_PIN                                GPIO_PIN_0 		//预留串口										USART8_RX
#define  INTERFACE_USART8_TX_PIN                                GPIO_PIN_1		//预留串口										USART8_TX
#define  FC_H2_IN_VAL_PWR_CRTL_PIN                              GPIO_PIN_2		//电堆进气电磁阀
#define  FC_H2_OUT_VAL_PWR_CRTL_PIN                             GPIO_PIN_3		//电堆排气电磁阀
#define  FC_DC_CONTACTOR_PWR_CRTL_PIN                           GPIO_PIN_4		//电堆直流接触器(电堆输出开关)
#define  FC_IGBT1_CRTL_PIN                                      GPIO_PIN_5		//电堆短路活化IGBT1控制脚
#define  FC_IGBT2_CRTL_PIN                                      GPIO_PIN_6		//电堆短路活化IGBT2控制脚
#define  INTERFACE_FSMC_D04_PIN                                 GPIO_PIN_7		//FSMC											FSMC_D04
#define  INTERFACE_FSMC_D05_PIN                                 GPIO_PIN_8		//FSMC											FSMC_D05
#define  INTERFACE_FSMC_D06_PIN                                 GPIO_PIN_9		//FSMC											FSMC_D06
#define  INTERFACE_FSMC_D07_PIN                                 GPIO_PIN_10		//FSMC											FSMC_D07
#define  INTERFACE_FSMC_D08_PIN                                 GPIO_PIN_11		//FSMC											FSMC_D08
#define  INTERFACE_FSMC_D09_PIN                                 GPIO_PIN_12		//FSMC											FSMC_D09
#define  INTERFACE_FSMC_D010_PIN                                GPIO_PIN_13		//FSMC											FSMC_D010
#define  INTERFACE_FSMC_D011_PIN                                GPIO_PIN_14		//FSMC											FSMC_D011
#define  INTERFACE_FSMC_D012_PIN                                GPIO_PIN_15		//FSMC											FSMC_D012

/* --------------------------------- GPIOF PINS ------------------------------ */
#define  BSP_PORT_GPIOF                                         GPIOF

#define  I2C2_EEPROM_SDA_PIN                                    GPIO_PIN_0		//EEPROM数据传输								EEPROM_SDA
#define  I2C2_EEPROM_SCL_PIN                                    GPIO_PIN_1		//EEPROM数据传输								EEPROM_SCL
#define  RVD4_VAL_PWR_CTRL_PIN                           		GPIO_PIN_2		//预留控制阀4									
#define  H_H2_CONCENTRATION_ANA_SIG_PIN                         GPIO_PIN_3		//氢气浓度信号									ADC3_IN9
#define  FC_TEMP_ANA_SIG_PIN                             		GPIO_PIN_4		//电堆温度信号									ADC3_IN14
#define  H_FIRE_STATUS_ANA_SIG_PIN                             	GPIO_PIN_5		//制氢机点火状态检测脚							ADC3_IN15
#define  SYS_BATTERY_VOLETAGE_ANA_SIG_PIN                       GPIO_PIN_6		//电池电源信号									ADC3_IN4
#define  FC_VOLETAGE_ANA_SIG_PIN                             	GPIO_PIN_7		//电堆电压信号									ADC3_IN5
#define  SYS_FAN_PWR_CTRL_PIN                           		GPIO_PIN_8		//系统风扇控制
#define  SYS_EXT_PUMP_PWR_CTRL_PIN                           	GPIO_PIN_9		//外置自动加液泵控制脚
#define  H_VACUM_PUMP_PWR_CTRL_PIN                           	GPIO_PIN_10		//真空泵电源控制脚
#define  H_FAST_LQD_INLET_VAL_PWR_CTRL_PIN                      GPIO_PIN_11		//制氢机快速进液电磁阀电源控制脚
#define  SYS_SELF_CHECK_SELECTOR_ADDR_A_PIN                		GPIO_PIN_12		//自检数据选择器地址A
#define  SYS_SELF_CHECK_SELECTOR_ADDR_B_PIN                		GPIO_PIN_13		//自检数据选择器地址B
#define  SYS_SELF_CHECK_SELECTOR_ADDR_C_PIN                		GPIO_PIN_14		//自检数据选择器地址C
#define  SYS_SELF_CHECK_SELECTOR_EN1_CTRL_PIN                	GPIO_PIN_15		//自检数据选择器使能引脚（第1组）

/* --------------------------------- GPIOG PINS ------------------------------ */
#define  BSP_PORT_GPIOG                                         GPIOG

#define  SYS_SELF_CHECK_SELECTOR1_OUTPUT_DATA_PIN               GPIO_PIN_0    	//自检数据选择器1数据输出脚（第1组）
#define  SYS_SELF_CHECK_SELECTOR2_OUTPUT_DATA_PIN               GPIO_PIN_1    	//自检数据选择器2数据输出脚（第2组）
#define  H_FAN_PWR_CTRL_PIN                                     GPIO_PIN_2		//制氢机风机电源
#define  H_HEATER_PWR_CTRL_PIN                                  GPIO_PIN_3		//制氢机快速加热器电源
#define  H_IGNITER_PWR_CTRL_PIN                                 GPIO_PIN_4		//制氢机点火器电源
#define  H_REFORM_LQD_INLET_VAL_PWR_CTRL_PIN                    GPIO_PIN_5		//制氢机重整进液电磁阀电源
#define  INTERFACE_FSMC_CS_PIN                                  GPIO_PIN_6		//FSMC接口片选引脚								FSMC_CS
#define  INTERFACE_SD_CARD_WP_PIN                               GPIO_PIN_7		//SD卡写保护									SD_CARD_WP
#define  INTERFACE_SD_CARD_DETECT_PIN                           GPIO_PIN_8		//SD卡插入检测									SD_CARD_DETECT
#define  INTERFACE_LCD_T_SPI1_MISO_PIN                          GPIO_PIN_9		//LCD触摸数据SPI输出引脚    					SPI1_MISO
#define  INTERFACE_LCD_T_SPI1_CS_PIN                            GPIO_PIN_10		//LCD触摸数据SPI输出引脚    					SPI1_CS
#define  INTERFACE_LCD_T_SPI1_SCK_PIN                           GPIO_PIN_11		//LCD触摸数据SPI输出引脚    					SPI1_SCK
#define  INTERFACE_LCD_BL_CTR_PIN                               GPIO_PIN_12		//LCD背光控制									LCD_BL_CTR
#define  INTERFACE_LCD_TOUCH_IRQ_PIN                            GPIO_PIN_13		//LCD触摸输入中断
#define  H_TEMP_MEMBRANCE_UP_SIG_PIN                            GPIO_PIN_14		//膜管顶部温度(热电偶CS1)			
#define  H_TEMP_MEMBRANCE_DOWN_SIG_PIN                          GPIO_PIN_15		//膜管底部温度（热点偶CS2）

/* --------------------------------- GPIOH PINS ------------------------------ */
#define  BSP_PORT_GPIOH                                         GPIOH

//PH0,1为25M晶振引脚
#define  H_TEMP_COMBUSTION_CHAMBER_SIG_PIN                      GPIO_PIN_2		//燃烧腔温度(热电偶CS3)
#define  H_TEMP_VAPORIZING_SIG_PIN                              GPIO_PIN_3		//汽化室温度(热电偶CS4)
#define  H_TEMP_REFORM_CHAMBER_SIG_PIN                          GPIO_PIN_4		//重整室温度(热电偶CS5)
#define  H_RVD_THERMOCOUPLE_TEMP_CH6_PIN                        GPIO_PIN_5		//热电偶6 预留1(热电偶CS6)
#define  H_PURE_H2_OUTLET_PWR_CTRL_PIN                          GPIO_PIN_6		//制氢机纯氢出口阀电源
#define  FC_H2_RELEASE_VAL_PWR_CTRL_PIN                         GPIO_PIN_7		//电堆泄压阀电源
#define  RVD1_VAL_PWR_CTRL_PIN                                  GPIO_PIN_8		//预留控制阀1
#define  H_PUMP_PWR_CTRL_PIN                                    GPIO_PIN_9		//自动加液泵
#define  FC_FAN2_SPD_FB_SIG_PIN                                 GPIO_PIN_10		//电堆风机2馈速信号脚							TIM5_CH1（捕获）
#define  FC_FAN3_SPD_FB_SIG_PIN                                 GPIO_PIN_11		//电堆风机3馈速信号脚							TIM5_CH2（捕获）
#define  FC_FAN4_SPD_FB_SIG_PIN                                 GPIO_PIN_12		//电堆风机4馈速信号脚							TIM5_CH3（捕获）
#define  INTERFACE_USART4_RX_PIN                                GPIO_PIN_13 	//WIFI模块通信接口								USART4_RX
#define  INTERFACE_USART4_TX_PIN                                GPIO_PIN_14		//WIFI模块通信接口								USART4_TX
#define  SYS_4G_MODULE_PWR_CTRL_PIN                             GPIO_PIN_15		//4G模块电源控制脚

/* --------------------------------- GPIOI PINS ------------------------------ */
#define  BSP_PORT_GPIOI                                         GPIOI

#define  SYS_MCU_HEART_CTRL_PIN                                 GPIO_PIN_0		//系统心跳指示灯
#define  MAX6675_SPI2_SCK_PIN                                   GPIO_PIN_1		//MAX6675										SPI2_SCK
#define  MAX6675_SPI2_MISO_PIN                                  GPIO_PIN_2		//MAX6675										SPI2_MISO
#define  DHT22_SIG_PIN                                   		GPIO_PIN_3		//DHT22温湿度传感器
#define  FC_FAN_PWR_CTRL_PIN                                    GPIO_PIN_4		//电堆风机电源
#define  FC_FAN1_SPD_FB_SIG_PIN                                 GPIO_PIN_5		//电堆风机1速度反馈								TIM8_CH1（捕获）
#define  FC_FAN1_SPD_CTRL_SIG_PIN                               GPIO_PIN_6		//电堆风扇1--控速								TIM8_CH2(pwm)	
#define  FC_FAN2_SPD_CTRL_SIG_PIN                               GPIO_PIN_7		//电堆风扇控速2（备）       					TIM8_CH3(pwm)	
#define  FC_SHORT_PROTECT_CTRL_PIN                              GPIO_PIN_8		//短路活化保护开关
#define  FC_DC_OUT_CONTACTOR_CTRL_PIN                           GPIO_PIN_9		//DC-DC输出直流接触器(开关)
#define  RVD2_VAL_PWR_CTRL_PIN                                  GPIO_PIN_10		//预留控制阀2
#define  RVD3_VAL_PWR_CTRL_PIN                                  GPIO_PIN_11		//预留控制阀3


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



#endif /* __BSP_PIN_H */

/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/
