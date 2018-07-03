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
 * @file      bsp_touch.c
 * @author    GuanXianDe
 * @version   V1.0.0
 * @date      26-June-2018
 * @brief     program entry
 * @attention none.
 *******************************************************************************
 */

/*
********************************************************************************
*                              INCLUDE FILES
********************************************************************************
*/
#include "bsp_touch.h"
#include "bsp_lcd.h"
#include "delay.h"
#include "stdlib.h"
#include "math.h"
#include "bsp_iic.h"
/*
********************************************************************************
*                             MACRO DEFINITIONS
********************************************************************************
*/

/*
********************************************************************************
*                          OS-RELATED    VARIABLES
********************************************************************************
*/
_m_tp_dev tp_dev=
{
    TP_Init,
    TP_Scan,
    TP_Adjust,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};
																										//Ĭ��Ϊtouchtype=0������.
uint8_t CMD_RDX=0XD0;
uint8_t CMD_RDY=0X90;
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
 * @brief :	SPIд����,������ICд��1byte����
 * @param :	num:Ҫд�������
 * @retval:	none.
 */
void TP_Write_Byte(uint8_t num)
{
    uint8_t count=0;
    for(count=0; count<8; count++)
    {
        if(num&0x80)TDIN(1);
        else TDIN(0);
        num<<=1;
        TCLK(0);
        delay_us(1);
        TCLK(1);																						//��������Ч
    }
}

/**
 * @brief :	SPI������,�Ӵ�����IC��ȡadcֵ
 * @param :	CMD:ָ��
 * @retval:	����������
 */
uint16_t TP_Read_AD(uint8_t CMD)
{
    uint8_t count=0;
    uint16_t Num=0;
    TCLK(0);																							//������ʱ��
    TDIN(0); 																							//����������
    TCS(0); 																							//ѡ�д�����IC
    TP_Write_Byte(CMD);																					//����������
    delay_us(6);																						//ADS7846��ת��ʱ���Ϊ6us
    TCLK(0);
    delay_us(1);
    TCLK(1);																							//��1��ʱ�ӣ����BUSY
    delay_us(1);
    TCLK(0);
    for(count=0; count<16; count++) 																	//����16λ����,ֻ�и�12λ��Ч
    {
        Num<<=1;
        TCLK(0);																						//�½�����Ч
        delay_us(1);
        TCLK(1);
        if(DOUT)Num++;
    }
    Num>>=4;   																							//ֻ�и�12λ��Ч.
    TCS(1);																								//�ͷ�Ƭѡ
    return(Num);
}


#define READ_TIMES 5 																					//��ȡ����
#define LOST_VAL 1	  																					//����ֵ

/**
 * @brief :	��ȡһ������ֵ(x����y),������ȡREAD_TIMES������,����Щ������������,Ȼ��ȥ����ͺ����LOST_VAL����,ȡƽ��ֵ
 * @param :	xy:ָ�CMD_RDX/CMD_RDY��
 * @retval:	����������
 */
uint16_t TP_Read_XOY(uint8_t xy)
{
    uint16_t i, j;
    uint16_t buf[READ_TIMES];
    uint16_t sum=0;
    uint16_t temp;
    for(i=0; i<READ_TIMES; i++)buf[i]=TP_Read_AD(xy);
    for(i=0; i<READ_TIMES-1; i++) 																		//����
    {
        for(j=i+1; j<READ_TIMES; j++)
        {
            if(buf[i]>buf[j])																			//��������
            {
                temp=buf[i];
                buf[i]=buf[j];
                buf[j]=temp;
            }
        }
    }
    sum=0;
    for(i=LOST_VAL; i<READ_TIMES-LOST_VAL; i++)sum+=buf[i];
    temp=sum/(READ_TIMES-2*LOST_VAL);
    return temp;
}

/**
 * @brief :	��ȡx,y����(��Сֵ��������100.)
 * @param :	x,y:��ȡ��������ֵ
 * @retval:	0,ʧ��;1,�ɹ���
 */
uint8_t TP_Read_XY(uint16_t *x,uint16_t *y)
{
    uint16_t xtemp,ytemp;
    xtemp=TP_Read_XOY(CMD_RDX);
    ytemp=TP_Read_XOY(CMD_RDY);
    //if(xtemp<100||ytemp<100)return 0;																	//����ʧ��
    *x=xtemp;
    *y=ytemp;
    return 1;																							//�����ɹ�
}

#define ERR_RANGE 50 																					//��Χ 

/**
 * @brief :	����2�ζ�ȡ������IC,�������ε�ƫ��ܳ���,ERR_RANGE,��������,����Ϊ������ȷ,�����������.
 * @param :	x,y:��ȡ��������ֵ
 * @retval:	0,ʧ��;1,�ɹ���
 */
uint8_t TP_Read_XY2(uint16_t *x,uint16_t *y)
{
    uint16_t x1,y1;
    uint16_t x2,y2;
    uint8_t flag;
    flag=TP_Read_XY(&x1,&y1);
    if(flag==0)return(0);
    flag=TP_Read_XY(&x2,&y2);
    if(flag==0)return(0);
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))											//ǰ�����β�����+-50��
            &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    } else return 0;
}

//////////////////////////////////////////////////////////////////////////////////
//��LCD�����йصĺ���

/**
 * @brief :	��һ��������(����У׼�õ�)
 * @param :	x,y:����
			color:��ɫ
 * @retval:	none.
 */
void TP_Drow_Touch_Point(uint16_t x,uint16_t y,uint16_t color)
{
    POINT_COLOR=color;
    LCD_DrawLine(x-12,y,x+13,y);																		//����
    LCD_DrawLine(x,y-12,x,y+13);																		//����
    LCD_DrawPoint(x+1,y+1);
    LCD_DrawPoint(x-1,y+1);
    LCD_DrawPoint(x+1,y-1);
    LCD_DrawPoint(x-1,y-1);
    LCD_Draw_Circle(x,y,6);																				//������Ȧ
}

/**
 * @brief :	��һ�����(2*2�ĵ�)
 * @param :	x,y:����
			color:��ɫ
 * @retval:	none.
 */
void TP_Draw_Big_Point(uint16_t x,uint16_t y,uint16_t color)
{
    POINT_COLOR=color;
    LCD_DrawPoint(x,y);																					//���ĵ�
    LCD_DrawPoint(x+1,y);
    LCD_DrawPoint(x,y+1);
    LCD_DrawPoint(x+1,y+1);
}

/**
 * @brief :	��������ɨ��
 * @param :	tp:0,��Ļ����;1,��������(У׼�����ⳡ����)
 * @retval:	��ǰ����״̬.0,�����޴���;1,�����д���
 */
uint8_t TP_Scan(uint8_t tp)
{
    if(PEN==0)//�а�������
    {
        if(tp)TP_Read_XY2(&tp_dev.x[0],&tp_dev.y[0]);													//��ȡ��������
        else if(TP_Read_XY2(&tp_dev.x[0],&tp_dev.y[0]))													//��ȡ��Ļ����
        {
            tp_dev.x[0]=tp_dev.xfac*tp_dev.x[0]+tp_dev.xoff;											//�����ת��Ϊ��Ļ����
            tp_dev.y[0]=tp_dev.yfac*tp_dev.y[0]+tp_dev.yoff;
        }
        if((tp_dev.sta&TP_PRES_DOWN)==0)																//֮ǰû�б�����
        {
            tp_dev.sta=TP_PRES_DOWN|TP_CATH_PRES;														//��������
            tp_dev.x[4]=tp_dev.x[0];																	//��¼��һ�ΰ���ʱ������
            tp_dev.y[4]=tp_dev.y[0];
        }
    } else
    {
        if(tp_dev.sta&TP_PRES_DOWN)																		//֮ǰ�Ǳ����µ�
        {
            tp_dev.sta&=~(1<<7);																		//��ǰ����ɿ�
        } else																							//֮ǰ��û�б�����
        {
            tp_dev.x[4]=0;
            tp_dev.y[4]=0;
            tp_dev.x[0]=0xffff;
            tp_dev.y[0]=0xffff;
        }
    }
    return tp_dev.sta&TP_PRES_DOWN;																		//���ص�ǰ�Ĵ���״̬
}

																										//������EEPROM����ĵ�ַ�����ַ,ռ��13���ֽ�(RANGE:SAVE_ADDR_BASE~SAVE_ADDR_BASE+12)
#define SAVE_ADDR_BASE 40

/**
 * @brief :	����У׼����
 * @param :	none.
 * @retval:	none.
 */
void TP_Save_Adjdata(void)
{
    int temp;
																										//����У�����!
    temp=tp_dev.xfac*100000000;																			//����xУ������
    BSP_AT24CXX_WriteLenByte(SAVE_ADDR_BASE,temp,4);
    temp=tp_dev.yfac*100000000;																			//����yУ������
    BSP_AT24CXX_WriteLenByte(SAVE_ADDR_BASE+4,temp,4);
																										//����xƫ����
    BSP_AT24CXX_WriteLenByte(SAVE_ADDR_BASE+8,tp_dev.xoff,2);
																										//����yƫ����
    BSP_AT24CXX_WriteLenByte(SAVE_ADDR_BASE+10,tp_dev.yoff,2);
																										//���津������
    BSP_AT24CXX_WriteOneByte(SAVE_ADDR_BASE+12,tp_dev.touchtype);
    temp=0X0A;																							//���У׼����
    BSP_AT24CXX_WriteOneByte(SAVE_ADDR_BASE+13,temp);
}

/**
 * @brief :	�õ�������EEPROM�����У׼ֵ
 * @param :	none.
 * @retval:
			1���ɹ���ȡ����
			0����ȡʧ�ܣ�Ҫ����У׼
 */       
uint8_t TP_Get_Adjdata(void)
{
    int tempfac;
    tempfac=BSP_AT24CXX_ReadOneByte(SAVE_ADDR_BASE+13);													//��ȡ�����,���Ƿ�У׼����
    if(tempfac==0X0A)																					//�������Ѿ�У׼����
    {
        tempfac=BSP_AT24CXX_ReadLenByte(SAVE_ADDR_BASE,4);
        tp_dev.xfac=(float)tempfac/100000000;															//�õ�xУ׼����
        tempfac=BSP_AT24CXX_ReadLenByte(SAVE_ADDR_BASE+4,4);
        tp_dev.yfac=(float)tempfac/100000000;															//�õ�yУ׼����
																										//�õ�xƫ����
        tp_dev.xoff=BSP_AT24CXX_ReadLenByte(SAVE_ADDR_BASE+8,2);
																										//�õ�yƫ����
        tp_dev.yoff=BSP_AT24CXX_ReadLenByte(SAVE_ADDR_BASE+10,2);
        tp_dev.touchtype=BSP_AT24CXX_ReadOneByte(SAVE_ADDR_BASE+12);									//��ȡ�������ͱ��
        if(tp_dev.touchtype)																			//X,Y��������Ļ�෴
        {
            CMD_RDX=0X90;
            CMD_RDY=0XD0;
        } else				   																			//X,Y��������Ļ��ͬ
        {
            CMD_RDX=0XD0;
            CMD_RDY=0X90;
        }
        return 1;
    }
    return 0;
}


																										//��ʾ�ַ���
uint8_t* const TP_REMIND_MSG_TBL="Please use the stylus click the cross on the screen.The cross will always move until the screen adjustment is completed.";

/**
 * @brief :	��ʾУ׼���(��������)
 * @param :	none.
 * @retval:	none.
 */
void TP_Adj_Info_Show(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t x3,uint16_t y3,uint16_t fac)
{
    POINT_COLOR=RED;
    LCD_ShowString(40,160,lcddev.width,lcddev.height,16,"x1:");
    LCD_ShowString(40+80,160,lcddev.width,lcddev.height,16,"y1:");
    LCD_ShowString(40,180,lcddev.width,lcddev.height,16,"x2:");
    LCD_ShowString(40+80,180,lcddev.width,lcddev.height,16,"y2:");
    LCD_ShowString(40,200,lcddev.width,lcddev.height,16,"x3:");
    LCD_ShowString(40+80,200,lcddev.width,lcddev.height,16,"y3:");
    LCD_ShowString(40,220,lcddev.width,lcddev.height,16,"x4:");
    LCD_ShowString(40+80,220,lcddev.width,lcddev.height,16,"y4:");
    LCD_ShowString(40,240,lcddev.width,lcddev.height,16,"fac is:");
    LCD_ShowNum(40+24,160,x0,4,16);																		//��ʾ��ֵ
    LCD_ShowNum(40+24+80,160,y0,4,16);																	//��ʾ��ֵ
    LCD_ShowNum(40+24,180,x1,4,16);																		//��ʾ��ֵ
    LCD_ShowNum(40+24+80,180,y1,4,16);																	//��ʾ��ֵ
    LCD_ShowNum(40+24,200,x2,4,16);																		//��ʾ��ֵ
    LCD_ShowNum(40+24+80,200,y2,4,16);																	//��ʾ��ֵ
    LCD_ShowNum(40+24,220,x3,4,16);																		//��ʾ��ֵ
    LCD_ShowNum(40+24+80,220,y3,4,16);																	//��ʾ��ֵ
    LCD_ShowNum(40+56,240,fac,3,16); 																	//��ʾ��ֵ,����ֵ������95~105��Χ֮��.

}

/**
 * @brief :	������У׼����(�õ��ĸ�У׼����)
 * @param :	none.
 * @retval:	none.
 */
void TP_Adjust(void)
{
    uint16_t pos_temp[4][2];																			//���껺��ֵ
    uint8_t  cnt=0;
    uint16_t d1,d2;
    uint32_t tem1,tem2;
    double fac;
    uint16_t outtime=0;
    cnt=0;
    POINT_COLOR=BLUE;
    BACK_COLOR =WHITE;
    LCD_Clear(WHITE);																					//����
    POINT_COLOR=RED;																					//��ɫ
    LCD_Clear(WHITE);																					//����
    POINT_COLOR=BLACK;
    LCD_ShowString(40,40,160,100,16,(uint8_t*)TP_REMIND_MSG_TBL);										//��ʾ��ʾ��Ϣ
    TP_Drow_Touch_Point(20,20,RED);																		//����1
    tp_dev.sta=0;																						//���������ź�
    tp_dev.xfac=0;																						//xfac��������Ƿ�У׼��,����У׼֮ǰ�������!�������
    while(1)																							//�������10����û�а���,���Զ��˳�
    {
        tp_dev.scan(1);																					//ɨ����������
        if((tp_dev.sta&0xc0)==TP_CATH_PRES)																//����������һ��(��ʱ�����ɿ���.)
        {
            outtime=0;
            tp_dev.sta&=~(1<<6);																		//��ǰ����Ѿ����������.

            pos_temp[cnt][0]=tp_dev.x[0];
            pos_temp[cnt][1]=tp_dev.y[0];
            cnt++;
            switch(cnt)
            {
            case 1:
                TP_Drow_Touch_Point(20,20,WHITE);														//�����1
                TP_Drow_Touch_Point(lcddev.width-20,20,RED);											//����2
                break;
            case 2:
                TP_Drow_Touch_Point(lcddev.width-20,20,WHITE);											//�����2
                TP_Drow_Touch_Point(20,lcddev.height-20,RED);											//����3
                break;
            case 3:
                TP_Drow_Touch_Point(20,lcddev.height-20,WHITE);											//�����3
                TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,RED);								//����4
                break;
            case 4:	 																					//ȫ���ĸ����Ѿ��õ�
																										//�Ա����
                tem1=abs(pos_temp[0][0]-pos_temp[1][0]);//x1-x2
                tem2=abs(pos_temp[0][1]-pos_temp[1][1]);//y1-y2
                tem1*=tem1;
                tem2*=tem2;
                d1=sqrt(tem1+tem2);																		//�õ�1,2�ľ���

                tem1=abs(pos_temp[2][0]-pos_temp[3][0]);//x3-x4
                tem2=abs(pos_temp[2][1]-pos_temp[3][1]);//y3-y4
                tem1*=tem1;
                tem2*=tem2;
                d2=sqrt(tem1+tem2);																		//�õ�3,4�ľ���
                fac=(float)d1/d2;
                if(fac<0.95||fac>1.05||d1==0||d2==0)													//���ϸ�
                {
                    cnt=0;
                    TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,WHITE);						//�����4
                    TP_Drow_Touch_Point(20,20,RED);														//����1
                    TP_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);//��ʾ����
                    continue;
                }
                tem1=abs(pos_temp[0][0]-pos_temp[2][0]);												//x1-x3
                tem2=abs(pos_temp[0][1]-pos_temp[2][1]);												//y1-y3
                tem1*=tem1;
                tem2*=tem2;
                d1=sqrt(tem1+tem2);																		//�õ�1,3�ľ���

                tem1=abs(pos_temp[1][0]-pos_temp[3][0]);//x2-x4
                tem2=abs(pos_temp[1][1]-pos_temp[3][1]);//y2-y4
                tem1*=tem1;
                tem2*=tem2;
                d2=sqrt(tem1+tem2);																		//�õ�2,4�ľ���
                fac=(float)d1/d2;
                if(fac<0.95||fac>1.05)																	//���ϸ�
                {
                    cnt=0;
                    TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,WHITE);						//�����4
                    TP_Drow_Touch_Point(20,20,RED);														//����1
                    TP_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);//��ʾ����
                    continue;
                }																						//��ȷ��

																										//�Խ������
                tem1=abs(pos_temp[1][0]-pos_temp[2][0]);												//x1-x3
                tem2=abs(pos_temp[1][1]-pos_temp[2][1]);												//y1-y3
                tem1*=tem1;
                tem2*=tem2;
                d1=sqrt(tem1+tem2);																		//�õ�1,4�ľ���

                tem1=abs(pos_temp[0][0]-pos_temp[3][0]);												//x2-x4
                tem2=abs(pos_temp[0][1]-pos_temp[3][1]);												//y2-y4
                tem1*=tem1;
                tem2*=tem2;
                d2=sqrt(tem1+tem2);																		//�õ�2,3�ľ���
                fac=(float)d1/d2;
                if(fac<0.95||fac>1.05)																	//���ϸ�
                {
                    cnt=0;
                    TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,WHITE);						//�����4
                    TP_Drow_Touch_Point(20,20,RED);														//����1
                    TP_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);//��ʾ����
                    continue;
                }																						//��ȷ��
																										//������
                tp_dev.xfac=(float)(lcddev.width-40)/(pos_temp[1][0]-pos_temp[0][0]);					//�õ�xfac
                tp_dev.xoff=(lcddev.width-tp_dev.xfac*(pos_temp[1][0]+pos_temp[0][0]))/2;				//�õ�xoff

                tp_dev.yfac=(float)(lcddev.height-40)/(pos_temp[2][1]-pos_temp[0][1]);					//�õ�yfac
                tp_dev.yoff=(lcddev.height-tp_dev.yfac*(pos_temp[2][1]+pos_temp[0][1]))/2;				//�õ�yoff
                if(abs(tp_dev.xfac)>2||abs(tp_dev.yfac)>2)												//������Ԥ����෴��.
                {
                    cnt=0;
                    TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,WHITE);						//�����4
                    TP_Drow_Touch_Point(20,20,RED);														//����1
                    LCD_ShowString(40,26,lcddev.width,lcddev.height,16,"TP Need readjust!");
                    tp_dev.touchtype=!tp_dev.touchtype;													//�޸Ĵ�������.
                    if(tp_dev.touchtype)																//X,Y��������Ļ�෴
                    {
                        CMD_RDX=0X90;
                        CMD_RDY=0XD0;
                    } else				   																//X,Y��������Ļ��ͬ
                    {
                        CMD_RDX=0XD0;
                        CMD_RDY=0X90;
                    }
                    continue;
                }
                POINT_COLOR=BLUE;
                LCD_Clear(WHITE);																		//����
                LCD_ShowString(35,110,lcddev.width,lcddev.height,16,"Touch Screen Adjust OK!");			//У�����
                delay_ms(1000);
                TP_Save_Adjdata();
                LCD_Clear(WHITE);																		//����
                return;																					//У�����
            }
        }
        delay_ms(10);
        outtime++;
        if(outtime>1000)
        {
            TP_Get_Adjdata();
            break;
        }
    }
}

/**
 * @brief :	��������ʼ��
 * @param :	none.
 * @retval:	none.
 */ 
void BSP_TouchInit(void)
{
#if(USE_Touch_Conf == 1)	
	 TP_Init();
#endif
}
/**
 * @brief :	��������ʼ��
 * @param :	none.
 * @retval:
			0,û�н���У׼
			1,���й�У׼
 */ 
uint8_t TP_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    if(lcddev.id==0X5510||lcddev.id==0X4342) 															//4.3 800*40MCU���ݴ���������4.3��480*272 RGB��
    {
        if(GT9147_Init()==0)																			//��GT9147
        {
            tp_dev.scan=GT9147_Scan;																	//ɨ�躯��ָ��GT9147������ɨ��
        } else
        {
            OTT2001A_Init();
            tp_dev.scan=OTT2001A_Scan;																	//ɨ�躯��ָ��OTT2001A������ɨ��
        }
        tp_dev.touchtype|=0X80;																			//������
        tp_dev.touchtype|=lcddev.dir&0X01;																//������������
        return 0;
    } else if(lcddev.id==0X1963||lcddev.id==0X7084||lcddev.id==0X7016)									//SSD1963 7�������� 7��800*480/1024*600 RGB��
    {
        FT5206_Init();
        tp_dev.scan=FT5206_Scan;																		//ɨ�躯��ָ��GT9147������ɨ��
        tp_dev.touchtype|=0X80;																			//������
        tp_dev.touchtype|=lcddev.dir&0X01;																//������������
        return 0;
    } else
    {

        __HAL_RCC_GPIOH_CLK_ENABLE();																	//����GPIOHʱ��
        __HAL_RCC_GPIOI_CLK_ENABLE();																	//����GPIOIʱ��
        __HAL_RCC_GPIOG_CLK_ENABLE();																	//����GPIOGʱ��

        //PH6
        GPIO_Initure.Pin=GPIO_PIN_6;            														//PH6
        GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  														//�������
        GPIO_Initure.Pull=GPIO_PULLUP;          														//����
        GPIO_Initure.Speed=GPIO_SPEED_HIGH;     														//����
        HAL_GPIO_Init(GPIOH,&GPIO_Initure);     														//��ʼ��

																										//PI3,8
        GPIO_Initure.Pin=GPIO_PIN_3|GPIO_PIN_8; 														//PI3,8
        HAL_GPIO_Init(GPIOI,&GPIO_Initure);     														//��ʼ��

																										//PH7
        GPIO_Initure.Pin=GPIO_PIN_7;            														//PH7
        GPIO_Initure.Mode=GPIO_MODE_INPUT;      														//����
        HAL_GPIO_Init(GPIOH,&GPIO_Initure);     														//��ʼ��

        //PG3
        GPIO_Initure.Pin=GPIO_PIN_3;            														//PG3
        HAL_GPIO_Init(GPIOG,&GPIO_Initure);     														//��ʼ��

        TP_Read_XY(&tp_dev.x[0],&tp_dev.y[0]);															//��һ�ζ�ȡ��ʼ��	
		
        BSP_IicInit();																					//��ʼ��24CXX
		
        if(TP_Get_Adjdata())return 0;																	//�Ѿ�У׼
        else			   																				//δУ׼?
        {
            LCD_Clear(WHITE);																			//����
            TP_Adjust();  																				//��ĻУ׼
            TP_Save_Adjdata();
        }
        TP_Get_Adjdata();
    }
    return 1;
}



/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/





