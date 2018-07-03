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
//�ܽŶ����Ϊ:���+�ܽź�
/* --------------------------------- GPIOA PINS ------------------------------ */
#define  BSP_PORT_GPIOA                                         GPIOA
#define  H_RVD_THERMOCOUPLE_CH7                                 GPIO_PIN_0		//�ȵ�ż7��Ԥ��2��(�ȵ�żCS7)						
#define  FC_CURRENT_ANA_SIG_CH1_PIN                             GPIO_PIN_1  	//��������������ѣ�							ADC_CH1
#define  INTERFACE_USART2_TX_PIN                                GPIO_PIN_2		//4Gģ�����ݴ���ӿ� ����						USART2_TX
#define  INTERFACE_USART2_RX_PIN                                GPIO_PIN_3  	//4Gģ�����ݴ���ӿ� ����    					USART2_RX
#define  H_PUMP_SPD_ANA_SIG_CTRL_PIN                            GPIO_PIN_4		//���������ģ�����źſ���						DAC_OUT1
#define  H_FAN_SPD_ANA_SIG_CTRL_PIN                             GPIO_PIN_5		//��������ģ�����źſ��ƽ�					DAC_OUT2
#define  SYS_BATTERY_CURRENT_ANA_SIG_PIN                        GPIO_PIN_6		//��ص���������								ADC12_IN6
#define  H_HEATER_CURRENT_ANA_SIG_PIN                           GPIO_PIN_7		//���������������������						ADC12_IN7
#define  SYS_SHUT_DOWN_KEY_EXTI_PIN                             GPIO_PIN_8		//�ػ�����(����ػ�)							EXTI
#define  H_PUMP_SPD_FB_SIG_PIN                               	GPIO_PIN_9		//�����ˮ���ٶȷ���							TIM1_CH2������
#define  H_FAN_SPD_FB_SIG_PIN                                	GPIO_PIN_10		//���������ٶȷ���							TIM1_CH3������
#define  CAN1_RX_OR_FS_DM_PIN                                   GPIO_PIN_11		//CAN1 OR USB									CAN1_RX OR USB_DM
#define  CAN1_TX_OR_FS_DP_PIN                                   GPIO_PIN_12 	//CAN1 OR USB									CAN1_TX OR USB_DP
#define  DBG_SWDIO_PIN                                          GPIO_PIN_13		//SWD���Կ�SWDIO
#define  DBG_SWCLK_PIN                                          GPIO_PIN_14		//SWD���Կ�SWCLK
#define  SYS_START_KEY_EXTI_PIN                                 GPIO_PIN_15		//��������(һ������)							EXTI

/* --------------------------------- GPIOB PINS ------------------------------ */
#define  BSP_PORT_GPIOB                                         GPIOB
#define  RVD2_ANA_SIG_PIN                           			GPIO_PIN_0		//Ԥ��������2									ADC12_IN9
#define  RVD1_ANA_SIG_PIN                           			GPIO_PIN_1		//Ԥ��������1									ADC12_IN8
#define  BUZZER_PWR_CTRL_PIN                        			GPIO_PIN_2		//����������
#define  SYS_220_AC_MONITOR_EXTI_PIN                			GPIO_PIN_3		//�е������									EXTI
#define  SYS_SELF_CHECK_SELECTOR_EN3_CTRL_PIN                	GPIO_PIN_4		//�Լ�����ѡ����ʹ�����ţ���3�飩
#define  USART5_RX_PIN                                          GPIO_PIN_5  	//485-I�ӿ�										USART5_RX
#define  USART5_TX_PIN                                          GPIO_PIN_6		//485-I�ӿ�										USART5_TX
#define  SYS_SELF_CHECK_SELECTOR3_OUTPUT_DATA_PIN              	GPIO_PIN_7  	//�Լ�����ѡ����3��������ţ���3�飩
#define  FC_INLET_VENT_MONITOR_PIN                				GPIO_PIN_8		//��ѽ����������������						�жϼ���
#define  FC_TAIL_VENT_MONITOR_PIN                				GPIO_PIN_9		//��ѳ����������������						�жϼ���
#define  INTERFACE_USART3_TX_PIN                                GPIO_PIN_10 	//485-II�ӿ�									USART3_TX
#define  INTERFACE_USART3_RX_PIN                                GPIO_PIN_11  	//485-II�ӿ�									USART3_RX
#define  INTERFACE_CAN2_RX_PIN                                  GPIO_PIN_12		//CAN2											CAN2_RX
#define  INTERFACE_CAN2_TX_PIN                                  GPIO_PIN_13 	//CAN2											CAN2_TX
#define  INTERFACE_USART1_TX_PIN                                GPIO_PIN_14 	//��������&���Կ�								USART1_TX
#define  INTERFACE_USART1_RX_PIN                                GPIO_PIN_15		//��������&���Կ�								USART1_RX
/* --------------------------------- GPIOC PINS ------------------------------ */
#define  BSP_PORT_GPIOC                                         GPIOC
#define  H_LQD_LEVEL1_ANA_SIG_PIN                            	GPIO_PIN_0		//Һλ������1 Сˮ��							ADC123_IN10
#define  H_LQD_LEVEL2_ANA_SIG_PIN                             	GPIO_PIN_1		//Һλ������2 ��ˮ��							ADC123_IN11
#define  H_LQD_PRESS_ANA_SIG_PIN                             	GPIO_PIN_2		//Һλ������									ADC12_IN12
#define  FC_H2_PRESS1_ANA_SIG_PIN                             	GPIO_PIN_3		//��ѹ������1									ADC12_IN13
#define  FC_H2_PRESS2_ANA_SIG_PIN                             	GPIO_PIN_4		//��ѹ������2									ADC12_IN14
#define  H_NEGATIVE_PRESSURE_ANA_SIG_PIN                        GPIO_PIN_5		//��ѹ�������ź�								ADC12_IN15
#define  INTERFACE_USART6_TX_PIN                                GPIO_PIN_6 		//DC�����ӿ�									USART6_TX
#define  INTERFACE_USART6_RX_PIN                                GPIO_PIN_7		//DC�����ӿ�									USART6_RX
#define  INTERFACE_SD_CARD_D0_PIN                               GPIO_PIN_8		//SD�����ݽ�D0									SD_CARD_D0
#define  INTERFACE_SD_CARD_D1_PIN                               GPIO_PIN_9		//SD�����ݽ�D1									SD_CARD_D1
#define  INTERFACE_SD_CARD_D2_PIN                               GPIO_PIN_10		//SD�����ݽ�D2									SD_CARD_D2
#define  INTERFACE_SD_CARD_D3_PIN                               GPIO_PIN_11		//SD�����ݽ�D3									SD_CARD_D3
#define  INTERFACE_SD_CARD_CLK_PIN                              GPIO_PIN_12		//SD��ʱ�ӽ�									SD_CARD_CLK
#define  SYS_DOOR_FB_SIG_PIN                                    GPIO_PIN_13    //�Ž��źŷ�����
//14.15����Ϊ32.768K����

/* --------------------------------- GPIOD PINS ------------------------------ */
#define  BSP_PORT_GPIOD                                         GPIOD

#define  FSMC_D02_PIN                                          	GPIO_PIN_0		//FSMC											FSMC_D02
#define  FSMC_D03_PIN                                          	GPIO_PIN_1		//FSMC											FSMC_D03
#define  INTERFACE_SD_CARD_CMD_PIN                              GPIO_PIN_2		//SD��ָ���									SD_CARD_CMD
#define  SYS_SELF_CHECK_SELECTOR_EN2_CTRL_PIN                	GPIO_PIN_3		//�Լ�����ѡ����ʹ�����ţ���2�飩
#define  INTERFACE_FSMC_RD_PIN                                  GPIO_PIN_4		//FSMC											FSMC_RD
#define  INTERFACE_FSMC_WR_PIN                                  GPIO_PIN_5		//FSMC											FSMC_WR
#define  SYS_RVD_HEART_LED_CTRL_PIN                             GPIO_PIN_6		//Ԥ��ϵͳ�����ƿ��ƽţ��գ�
#define  INTERFACE_LCD_TOUCH_SPI1_MOSI_PIN                      GPIO_PIN_7		//LCD��������SPI�������      					SPI1_MOSI
#define  INTERFACE_FSMC_D13_PIN                                 GPIO_PIN_8		//FSMC											FSMC_D13
#define  INTERFACE_FSMC_D14_PIN                                 GPIO_PIN_9		//FSMC											FSMC_D14
#define  INTERFACE_FSMC_D15_PIN                                 GPIO_PIN_10		//FSMC											FSMC_D15
#define  INTERFACE_FSMC_RST_PIN                                 GPIO_PIN_11		//FSMC											FSMC_RST
#define  RVD_48V_HEART_PWR_CTRL_PIN                             GPIO_PIN_12		//Ԥ��48V��Դ���ƽ�
#define  INTERFACE_FSMC_RS_PIN                                  GPIO_PIN_13		//FSMC											FSMC_RS
#define  INTERFACE_FSMC_D00_PIN                                 GPIO_PIN_14		//FSMC											FSMC_D00
#define  INTERFACE_FSMC_D01_PIN                                 GPIO_PIN_15		//FSMC											FSMC_D01

/* --------------------------------- GPIOE PINS ------------------------------ */
#define  BSP_PORT_GPIOE                                         GPIOE

#define  INTERFACE_USART8_RX_PIN                                GPIO_PIN_0 		//Ԥ������										USART8_RX
#define  INTERFACE_USART8_TX_PIN                                GPIO_PIN_1		//Ԥ������										USART8_TX
#define  FC_H2_IN_VAL_PWR_CRTL_PIN                              GPIO_PIN_2		//��ѽ�����ŷ�
#define  FC_H2_OUT_VAL_PWR_CRTL_PIN                             GPIO_PIN_3		//���������ŷ�
#define  FC_DC_CONTACTOR_PWR_CRTL_PIN                           GPIO_PIN_4		//���ֱ���Ӵ���(����������)
#define  FC_IGBT1_CRTL_PIN                                      GPIO_PIN_5		//��Ѷ�·�IGBT1���ƽ�
#define  FC_IGBT2_CRTL_PIN                                      GPIO_PIN_6		//��Ѷ�·�IGBT2���ƽ�
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

#define  I2C2_EEPROM_SDA_PIN                                    GPIO_PIN_0		//EEPROM���ݴ���								EEPROM_SDA
#define  I2C2_EEPROM_SCL_PIN                                    GPIO_PIN_1		//EEPROM���ݴ���								EEPROM_SCL
#define  RVD4_VAL_PWR_CTRL_PIN                           		GPIO_PIN_2		//Ԥ�����Ʒ�4									
#define  H_H2_CONCENTRATION_ANA_SIG_PIN                         GPIO_PIN_3		//����Ũ���ź�									ADC3_IN9
#define  FC_TEMP_ANA_SIG_PIN                             		GPIO_PIN_4		//����¶��ź�									ADC3_IN14
#define  H_FIRE_STATUS_ANA_SIG_PIN                             	GPIO_PIN_5		//��������״̬����							ADC3_IN15
#define  SYS_BATTERY_VOLETAGE_ANA_SIG_PIN                       GPIO_PIN_6		//��ص�Դ�ź�									ADC3_IN4
#define  FC_VOLETAGE_ANA_SIG_PIN                             	GPIO_PIN_7		//��ѵ�ѹ�ź�									ADC3_IN5
#define  SYS_FAN_PWR_CTRL_PIN                           		GPIO_PIN_8		//ϵͳ���ȿ���
#define  SYS_EXT_PUMP_PWR_CTRL_PIN                           	GPIO_PIN_9		//�����Զ���Һ�ÿ��ƽ�
#define  H_VACUM_PUMP_PWR_CTRL_PIN                           	GPIO_PIN_10		//��ձõ�Դ���ƽ�
#define  H_FAST_LQD_INLET_VAL_PWR_CTRL_PIN                      GPIO_PIN_11		//��������ٽ�Һ��ŷ���Դ���ƽ�
#define  SYS_SELF_CHECK_SELECTOR_ADDR_A_PIN                		GPIO_PIN_12		//�Լ�����ѡ������ַA
#define  SYS_SELF_CHECK_SELECTOR_ADDR_B_PIN                		GPIO_PIN_13		//�Լ�����ѡ������ַB
#define  SYS_SELF_CHECK_SELECTOR_ADDR_C_PIN                		GPIO_PIN_14		//�Լ�����ѡ������ַC
#define  SYS_SELF_CHECK_SELECTOR_EN1_CTRL_PIN                	GPIO_PIN_15		//�Լ�����ѡ����ʹ�����ţ���1�飩

/* --------------------------------- GPIOG PINS ------------------------------ */
#define  BSP_PORT_GPIOG                                         GPIOG

#define  SYS_SELF_CHECK_SELECTOR1_OUTPUT_DATA_PIN               GPIO_PIN_0    	//�Լ�����ѡ����1��������ţ���1�飩
#define  SYS_SELF_CHECK_SELECTOR2_OUTPUT_DATA_PIN               GPIO_PIN_1    	//�Լ�����ѡ����2��������ţ���2�飩
#define  H_FAN_PWR_CTRL_PIN                                     GPIO_PIN_2		//����������Դ
#define  H_HEATER_PWR_CTRL_PIN                                  GPIO_PIN_3		//��������ټ�������Դ
#define  H_IGNITER_PWR_CTRL_PIN                                 GPIO_PIN_4		//������������Դ
#define  H_REFORM_LQD_INLET_VAL_PWR_CTRL_PIN                    GPIO_PIN_5		//�����������Һ��ŷ���Դ
#define  INTERFACE_FSMC_CS_PIN                                  GPIO_PIN_6		//FSMC�ӿ�Ƭѡ����								FSMC_CS
#define  INTERFACE_SD_CARD_WP_PIN                               GPIO_PIN_7		//SD��д����									SD_CARD_WP
#define  INTERFACE_SD_CARD_DETECT_PIN                           GPIO_PIN_8		//SD��������									SD_CARD_DETECT
#define  INTERFACE_LCD_T_SPI1_MISO_PIN                          GPIO_PIN_9		//LCD��������SPI�������    					SPI1_MISO
#define  INTERFACE_LCD_T_SPI1_CS_PIN                            GPIO_PIN_10		//LCD��������SPI�������    					SPI1_CS
#define  INTERFACE_LCD_T_SPI1_SCK_PIN                           GPIO_PIN_11		//LCD��������SPI�������    					SPI1_SCK
#define  INTERFACE_LCD_BL_CTR_PIN                               GPIO_PIN_12		//LCD�������									LCD_BL_CTR
#define  INTERFACE_LCD_TOUCH_IRQ_PIN                            GPIO_PIN_13		//LCD���������ж�
#define  H_TEMP_MEMBRANCE_UP_SIG_PIN                            GPIO_PIN_14		//Ĥ�ܶ����¶�(�ȵ�żCS1)			
#define  H_TEMP_MEMBRANCE_DOWN_SIG_PIN                          GPIO_PIN_15		//Ĥ�ܵײ��¶ȣ��ȵ�żCS2��

/* --------------------------------- GPIOH PINS ------------------------------ */
#define  BSP_PORT_GPIOH                                         GPIOH

//PH0,1Ϊ25M��������
#define  H_TEMP_COMBUSTION_CHAMBER_SIG_PIN                      GPIO_PIN_2		//ȼ��ǻ�¶�(�ȵ�żCS3)
#define  H_TEMP_VAPORIZING_SIG_PIN                              GPIO_PIN_3		//�������¶�(�ȵ�żCS4)
#define  H_TEMP_REFORM_CHAMBER_SIG_PIN                          GPIO_PIN_4		//�������¶�(�ȵ�żCS5)
#define  H_RVD_THERMOCOUPLE_TEMP_CH6_PIN                        GPIO_PIN_5		//�ȵ�ż6 Ԥ��1(�ȵ�żCS6)
#define  H_PURE_H2_OUTLET_PWR_CTRL_PIN                          GPIO_PIN_6		//�����������ڷ���Դ
#define  FC_H2_RELEASE_VAL_PWR_CTRL_PIN                         GPIO_PIN_7		//���йѹ����Դ
#define  RVD1_VAL_PWR_CTRL_PIN                                  GPIO_PIN_8		//Ԥ�����Ʒ�1
#define  H_PUMP_PWR_CTRL_PIN                                    GPIO_PIN_9		//�Զ���Һ��
#define  FC_FAN2_SPD_FB_SIG_PIN                                 GPIO_PIN_10		//��ѷ��2�����źŽ�							TIM5_CH1������
#define  FC_FAN3_SPD_FB_SIG_PIN                                 GPIO_PIN_11		//��ѷ��3�����źŽ�							TIM5_CH2������
#define  FC_FAN4_SPD_FB_SIG_PIN                                 GPIO_PIN_12		//��ѷ��4�����źŽ�							TIM5_CH3������
#define  INTERFACE_USART4_RX_PIN                                GPIO_PIN_13 	//WIFIģ��ͨ�Žӿ�								USART4_RX
#define  INTERFACE_USART4_TX_PIN                                GPIO_PIN_14		//WIFIģ��ͨ�Žӿ�								USART4_TX
#define  SYS_4G_MODULE_PWR_CTRL_PIN                             GPIO_PIN_15		//4Gģ���Դ���ƽ�

/* --------------------------------- GPIOI PINS ------------------------------ */
#define  BSP_PORT_GPIOI                                         GPIOI

#define  SYS_MCU_HEART_CTRL_PIN                                 GPIO_PIN_0		//ϵͳ����ָʾ��
#define  MAX6675_SPI2_SCK_PIN                                   GPIO_PIN_1		//MAX6675										SPI2_SCK
#define  MAX6675_SPI2_MISO_PIN                                  GPIO_PIN_2		//MAX6675										SPI2_MISO
#define  DHT22_SIG_PIN                                   		GPIO_PIN_3		//DHT22��ʪ�ȴ�����
#define  FC_FAN_PWR_CTRL_PIN                                    GPIO_PIN_4		//��ѷ����Դ
#define  FC_FAN1_SPD_FB_SIG_PIN                                 GPIO_PIN_5		//��ѷ��1�ٶȷ���								TIM8_CH1������
#define  FC_FAN1_SPD_CTRL_SIG_PIN                               GPIO_PIN_6		//��ѷ���1--����								TIM8_CH2(pwm)	
#define  FC_FAN2_SPD_CTRL_SIG_PIN                               GPIO_PIN_7		//��ѷ��ȿ���2������       					TIM8_CH3(pwm)	
#define  FC_SHORT_PROTECT_CTRL_PIN                              GPIO_PIN_8		//��·���������
#define  FC_DC_OUT_CONTACTOR_CTRL_PIN                           GPIO_PIN_9		//DC-DC���ֱ���Ӵ���(����)
#define  RVD2_VAL_PWR_CTRL_PIN                                  GPIO_PIN_10		//Ԥ�����Ʒ�2
#define  RVD3_VAL_PWR_CTRL_PIN                                  GPIO_PIN_11		//Ԥ�����Ʒ�3


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
