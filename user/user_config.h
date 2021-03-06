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
 * @file      user_config.h
 * @author    GuanXianDe
 * @version   V1.0.0
 * @date      28-June-2018
 * @brief     Header for main.c module
 * @attention	此头文件用来裁减没用到的功能
 *******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USER_CONFIG_H
#define __USER_CONFIG_H

/*
********************************************************************************
*                                 INCLUDE FILES
********************************************************************************
*/

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
/*注：所有配置都是在最高级别开启的时候才能操作第一或者第二开关。。。。*/
/*
********************************************************************************
*                                 （第一组）LED相关
********************************************************************************
*/
/*
使用bsp_lec.c和bsp_lec.h
需要开启USE_Led_File											（最高级别）  										1：开启 0：不开启
*/
#define USE_Led_File																								(1)
/*
使用LED需要开启USE_Led_Conf										（第一开关）  										1：开启 0：不开启
*/
#define USE_Led_Conf																								(1)

/*
********************************************************************************
*                                 （第二组）按键相关
********************************************************************************
*/
/*
使用bsp_key.c和bsp_key.h
需要开启USE_Key_File											（最高级别）  										1：开启 0：不开启
*/
#define USE_Key_File																								(0)
/*
使用按键检测需要开启USE_Key_Conf								（第一开关） 										1：开启 0：不开启
*/
#define USE_Key_Conf																								(0)


/*
********************************************************************************
*                                 （第三组）外部中断相关
********************************************************************************
*/
/*
使用bsp_Exti.c和bsp_Exti.h
需要开启USE_Exti_File											（最高级别）  										1：开启 0：不开启
*/
#define USE_Exti_File																								(0)
/*
使用外部中断需要开启USE_Exti_Conf								（第一开关）										1：开启 0：不开启
*/
#define USE_Exti_Conf																								(0)


/*
********************************************************************************
*                                 （第四组）LCD相关
********************************************************************************
*/
/*
使用LCD驱动文件包括bsp_lcd.c与bsp_ltdc.c
需要开启USE_Lcd_File											（最高级别）  										1：开启 0：不开启
*/
#define USE_Lcd_File																								(1)
/*
使用LCD屏需要开启USE_Lcd_Conf									（第一开关）										1：开启 0：不开启
*/
#define USE_Lcd_Conf																								(1)



/*
********************************************************************************
*                                 （第五组）SDRAM相关
********************************************************************************
*/
/*
使用Sdram驱动文件包括bsp_Sdram.c
需要开启USE_Sdram_File											（最高级别）  										1：开启 0：不开启
*/
#define USE_Sdram_File																								(1)
/*
使用SDRAM需要开启USE_Sdram_Conf									（第一开关）										1：开启 0：不开启
*/
#define USE_Sdram_Conf																								(1)


/*
********************************************************************************
*                                 （第六组）触摸相关
********************************************************************************
*/
/*
使用LCD触摸驱动需要开启USE_Touch_Conf							（第一开关）										1：开启 0：不开启
*/
#define USE_Touch_Conf																								(1)


/*
********************************************************************************
*                                 （第七组）ADC相关
********************************************************************************
*/
/*
使用ADC驱动文件包括bsp_Adc.c
需要开启USE_Adc_File											（最高级别）  									  	1：开启 0：不开启
*/
#define USE_Adc_File																							 	(0)
/*
使用ADC需要开启USE_Adc_Conf										（第一开关）										1：开启 0：不开启
*/
#define USE_Adc_Conf																								(0)

/*使用Adc1需要开启USE_Adc1_Conf 								（第二开关）										1：开启 0：不开启	
*/
#define USE_Adc1_Conf																								(0)

/*使用Adc3需要开启USE_Adc3_Conf 								（第二开关）										1：开启 0：不开启	
*/
#define USE_Adc3_Conf																								(0)



/*
********************************************************************************
*                                 （第八组）DAC相关
********************************************************************************
*/
/*
使用ADC驱动文件包括bsp_Dac.c
需要开启USE_Dac_File											（最高级别）  									 	1：开启 0：不开启
*/
#define USE_Dac_File																								(0)
/*
使用DAC需要开启USE_Dac_Conf										（第一开关）										1：开启 0：不开启
*/
#define USE_Dac_Conf																								(0)

/*使用Dac1需要开启USE_Dac1_Conf 								（第二开关）										1：开启 0：不开启	
*/
#define USE_Dac1_Conf																								(0)


/*
********************************************************************************
*                                 （第九组）TIM相关
********************************************************************************
*/
/*
使用定时器文件BSP_TIM.C
需要开启USE_Tim_File											（最高级别）  									  	1：开启 0：不开启
*/
#define USE_Tim_File																								(0)
/*
使用定时器需要开启USE_Tim_Conf									（第一开关）										1：开启 0：不开启
*/
#define USE_Tim_Conf																								(0)
/*
使用定时器1捕获需要开启USE_Tim1ChCap							（第二开关）										1：开启 0：不开启
*/
#define USE_Tim1ChCap																								(0)
/*
使用定时器4捕获需要开启USE_Tim4ChCap							（第二开关）										1：开启 0：不开启
*/
#define USE_Tim4ChCap																								(0)
/*
使用定时器5捕获需要开启USE_Tim5ChCap							（第二开关）										1：开启 0：不开启
*/
#define USE_Tim5ChCap																								(0)
/*
使用定时器8PWM输出需要开启USE_Tim8Pwm 							（第二开关）										1：开启 0：不开启
*/
#define USE_Tim8Pwm																									(0)
/*
使用定时器9PWM输出需要开启USE_Tim9Pwm 							（第二开关）										1：开启 0：不开启
*/
#define USE_Tim9Pwm																									(0)

/*
********************************************************************************
*                                 （第十组）I2C相关
********************************************************************************
*/
/*
使用I2C驱动文件包括bsp_iic.c
需要开启USE_I2c_File											（最高级别）  									 	1：开启 0：不开启
*/
#define USE_I2c_File																								(1)
/*
使用I2C(24CXX)需要开启USE_I2c_Conf								（第一开关）										1：开启 0：不开启
*/
#define USE_I2c_Conf																								(1)



/*
********************************************************************************
*                                 （第十一组）串口相关
********************************************************************************
*/
/*
使用串口通信文件
需要开启USE_Uart_File											（最高级别）  									 	1：开启 0：不开启
*/
#define USE_Uart_File																								(1)
/*
使用串口需要开启USE_Uart_Conf									（第一开关）										1：开启 0：不开启
*/
#define USE_Uart_Conf																								(1)
/*
使用串口需要开启USE_Uart1_Conf									（第二开关）										1：开启 0：不开启
*/
#define USE_Uart1_Conf																								(1)
/*
使用串口需要开启USE_Uart2_Conf									（第二开关）										1：开启 0：不开启
*/
#define USE_Uart2_Conf																								(1)
/*
使用串口需要开启USE_Uart3_Conf									（第二开关）										1：开启 0：不开启
*/
#define USE_Uart3_Conf																								(0)
/*
使用串口需要开启USE_Uart4_Conf									（第二开关）										1：开启 0：不开启
*/
#define USE_Uart4_Conf																								(0)
/*	
使用串口需要开启USE_Uart5_Conf									（第二开关）										1：开启 0：不开启
*/
#define USE_Uart5_Conf																								(0)
/*
使用串口需要开启USE_Uart6_Conf									（第二开关）										1：开启 0：不开启
*/
#define USE_Uart6_Conf																								(0)
/*
使用串口需要开启USE_Uart8_Conf									（第二开关）										1：开启 0：不开启
*/
#define USE_Uart8_Conf																								(0)


/*
********************************************************************************
*                                 （第十二组）CAN相关
********************************************************************************
*/
/*
使用CAN通信文件
需要开启USE_Can_File											（最高级别）  									 	1：开启 0：不开启
*/
#define USE_Can_File																								(1)
/*
使用CAN需要开启USE_Can_Conf										（第一开关）										1：开启 0：不开启
*/
#define USE_Can_Conf																								(1)
/*
使用CAN1需要开启USE_Can_Conf									（第二开关）										1：开启 0：不开启
*/
#define USE_Can1_Conf																								(0)
/*
使用CAN2需要开启USE_Can_Conf									（第二开关）										1：开启 0：不开启
*/
#define USE_Can2_Conf																								(1)

/*
********************************************************************************
*                                 （第十三组）RTC相关
********************************************************************************
*/
/*
使用RTC实时时钟
需要开启USE_Rtc_File											（最高级别）  									 	1：开启 0：不开启
*/
#define USE_Rtc_File																								(1)
/*
使用RTC实时时钟需要开启USE_Rtc_Conf								（第一开关）										1：开启 0：不开启
*/
#define USE_Rtc_Conf																								(1)


/*
********************************************************************************
*                                 （第十四组）SD卡相关
********************************************************************************
*/
/*
使用SD卡文件
需要开启USE_Sdcard_File											（最高级别）  										1：开启 0：不开启
*/
#define USE_Sdcard_File																								(1)
/*
使用SD卡需要开启USE_SdCard_Conf									（第一开关）										1：开启 0：不开启
*/
#define USE_SdCard_Conf																								(1)

/*
********************************************************************************
*                                 （第十五组）MPU相关
********************************************************************************
*/
/*
使用MPU驱动文件包括mpu.c
需要开启USE_Mpu_File											（最高级别）  									 	1：开启 0：不开启
*/
#define USE_Mpu_File																								(1)
/*
使用MPU需要开启USE_Mpu_Conf										（第一开关）										1：开启 0：不开启
*/
#define USE_Mpu_Conf																								(1)

/*
********************************************************************************
*                                 （第十六组）MALLOC 内存分配相关
********************************************************************************
*/
/*
使用内存分配文件包括malloc.c
需要开启USE_Malloc_File											（最高级别）  									 	1：开启 0：不开启
*/
#define USE_Malloc_File																								(1)
/*
使用MPU需要开启USE_Mpu_Conf										（第一开关）										 1：开启 0：不开启
*/
#define USE_Malloc_Conf																								(1)

/*
********************************************************************************
*                                 （第十七组）FATFS文件系统相关
********************************************************************************
*/
/*
使用文件系统FATFS
开启步骤：
1.需要开启malloc内存分配
2.需要开启SD卡驱动
3.需要开启RTC实时时钟驱动
4.需要设置USE_Fatfs_File为1
需要开启USE_Fatfs_File											（最高级别）  									 	1：开启 0：不开启
*/
#define USE_Fatfs_File																								(1)
/*
使用文件系统需要开启USE_Fatfs_Conf								（第一开关 预留）									1：开启 0：不开启
*/
#define USE_Fatfs_Conf																								(0)

/*
********************************************************************************
*                                 （第十八组）STEMWIN相关
********************************************************************************
*/
/*
使用STemwin
开启步骤：
1.USE_STemwin_File设置为1
2.开启DSP_LIB分组
3.开启EMWIN_LIB分组
4.内存分配malloc得开启
5.LCD驱动和触摸驱动得开启
6.使用操作系统FreeRTOS
需要开启USE_STemwin_File										（最高级别）  									 	1：开启 0：不开启
*/
#define USE_STemwin_File																							(1)
/*
使用STemwin demo
测试STemwin demo代码时使用，如果使用测试demo，需要把EMWIN_DEMO分组的文件包括进来，默认为屏蔽状态
1.选中分组->右击魔术棒->Include in target Build
2.USE_EmwinDemo_Conf设置为1
需要开启USE_EmwinDemo_Conf										（第一开关）  										 1：开启 0：不开启
*/
#define USE_EmwinDemo_Conf																							(0)

/*
********************************************************************************
*                                 （第十九组）MAX6675相关
********************************************************************************
*/
/*
使用bsp_max6675.c驱动文件
需要开启USE_Max6675_File										（最高级别）  									 	1：开启 0：不开启
*/
#define USE_Max6675_File																							(1)
/*
使用MPU需要开启USE_Max6675_Conf									（第一开关）										1：开启 0：不开启
*/
#define USE_Max6675_Conf																							(1)
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



#endif /* __USER_CONFIG_H */

/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/
