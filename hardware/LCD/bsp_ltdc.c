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
 * @file      bsp_ltdc.c
 * @author    FanJun
 * @version   V1.0.0
 * @date      08-June-2018
 * @brief     program entry
 * @attention none.
 *******************************************************************************
 */

/*
********************************************************************************
*                              INCLUDE FILES
********************************************************************************
*/
#include "bsp_ltdc.h"
#include "bsp_lcd.h"


#if(USE_Lcd_File == 1)
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
LTDC_HandleTypeDef  LTDC_Handler;       //LTDC���
DMA2D_HandleTypeDef DMA2D_Handler;      //DMA2D���
/*
********************************************************************************
*                              FUNCTION DEFINITION
********************************************************************************
*/



																				//���ݲ�ͬ����ɫ��ʽ,����֡��������
#if LCD_PIXFORMAT==LCD_PIXFORMAT_ARGB8888||LCD_PIXFORMAT==LCD_PIXFORMAT_RGB888
uint32_t ltdc_lcd_framebuf[1280][800] __attribute__((at(LCD_FRAME_BUF_ADDR)));  //����������ֱ���ʱ,LCD�����֡���������С
#else
uint16_t ltdc_lcd_framebuf[1280][800] __attribute__((at(LCD_FRAME_BUF_ADDR)));  //����������ֱ���ʱ,LCD�����֡���������С
#endif

uint32_t *ltdc_framebuf[2];                 									//LTDC LCD֡��������ָ��,����ָ���Ӧ��С���ڴ�����
_ltdc_dev lcdltdc;                      										//����LCD LTDC����Ҫ����

																				//��LCD����
																				//lcd_switch:1 ��,0���ر�
void LTDC_Switch(uint8_t sw)
{
    if(sw == 1) {
        __HAL_LTDC_ENABLE(&LTDC_Handler);
    } else if(sw == 0) {
        __HAL_LTDC_DISABLE(&LTDC_Handler);
    }
}

																				//����ָ����
																				//layerx:���,0,��һ��; 1,�ڶ���
																				//sw:1 ��;0�ر�
void LTDC_Layer_Switch(uint8_t layerx, uint8_t sw)
{
    if(sw == 1) {
        __HAL_LTDC_LAYER_ENABLE(&LTDC_Handler, layerx);
    } else if(sw == 0) {
        __HAL_LTDC_LAYER_DISABLE(&LTDC_Handler, layerx);
    }

    __HAL_LTDC_RELOAD_CONFIG(&LTDC_Handler);
}

																				//ѡ���
																				//layerx:���;0,��һ��;1,�ڶ���;
void LTDC_Select_Layer(uint8_t layerx)
{
    lcdltdc.activelayer = layerx;
}

																				//����LCD��ʾ����
																				//dir:0,������1,����
void LTDC_Display_Dir(uint8_t dir)
{
    lcdltdc.dir = dir;  														//��ʾ����

    if(dir == 0) {      														//����
        lcdltdc.width = lcdltdc.pheight;
        lcdltdc.height = lcdltdc.pwidth;
    } else if(dir == 1) { 														//����
        lcdltdc.width = lcdltdc.pwidth;
        lcdltdc.height = lcdltdc.pheight;
    }
}

																				//���㺯��
																				//x,y:����
																				//color:��ɫֵ
void LTDC_Draw_Point(uint16_t x, uint16_t y, uint32_t color)
{
#if LCD_PIXFORMAT==LCD_PIXFORMAT_ARGB8888||LCD_PIXFORMAT==LCD_PIXFORMAT_RGB888

    if(lcdltdc.dir) { 															//����
        *(uint32_t *)((uint32_t)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * y + x)) = color;
    } else {         															//����
        *(uint32_t *)((uint32_t)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * (lcdltdc.pheight - x - 1) + y)) = color;
    }

#else

    if(lcdltdc.dir) { 															//����
        *(uint16_t *)((uint32_t)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * y + x)) = color;
    } else {         															//����
        *(uint16_t *)((uint32_t)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * (lcdltdc.pheight - x - 1) + y)) = color;
    }

#endif
}

																				//���㺯��
																				//����ֵ:��ɫֵ
uint32_t LTDC_Read_Point(uint16_t x, uint16_t y)
{
#if LCD_PIXFORMAT==LCD_PIXFORMAT_ARGB8888||LCD_PIXFORMAT==LCD_PIXFORMAT_RGB888

    if(lcdltdc.dir) { 															//����
        return *(uint32_t *)((uint32_t)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * y + x));
    } else {         															//����
        return *(uint32_t *)((uint32_t)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * (lcdltdc.pheight - x - 1) + y));
    }

#else

    if(lcdltdc.dir) { 															//����
        return *(uint16_t *)((uint32_t)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * y + x));
    } else {         															//����
        return *(uint16_t *)((uint32_t)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * (lcdltdc.pheight - x - 1) + y));
    }

#endif
}

																				//LTDC������,DMA2D���
																				//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)
																				//ע��:sx,ex,���ܴ���lcddev.width-1;sy,ey,���ܴ���lcddev.height-1!!!
																				//color:Ҫ������ɫ
void LTDC_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint32_t color)
{
    uint32_t psx, psy, pex, pey; 												//��LCD���Ϊ��׼������ϵ,����������仯���仯
    uint32_t timeout = 0;
    uint16_t offline;
    uint32_t addr;

																				//����ϵת��
    if(lcdltdc.dir) { 															//����
        psx = sx;
        psy = sy;
        pex = ex;
        pey = ey;
    } else {         															//����
        psx = sy;
        psy = lcdltdc.pheight - ex - 1;
        pex = ey;
        pey = lcdltdc.pheight - sx - 1;
    }

    offline = lcdltdc.pwidth - (pex - psx + 1);
    addr = ((uint32_t)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * psy + psx));
    RCC->AHB1ENR |= 1 << 23;        											//ʹ��DM2Dʱ��
    DMA2D->CR = 3 << 16;            											//�Ĵ������洢��ģʽ
    DMA2D->OPFCCR = LCD_PIXFORMAT;  											//������ɫ��ʽ
    DMA2D->OOR = offline;           											//������ƫ��
    DMA2D->CR &= ~(1 << 0);         											//��ֹͣDMA2D
    DMA2D->OMAR = addr;             											//����洢����ַ
    DMA2D->NLR = (pey - psy + 1) | ((pex - psx + 1) << 16); 					//�趨�����Ĵ���
    DMA2D->OCOLR = color;           											//�趨�����ɫ�Ĵ���
    DMA2D->CR |= 1 << 0;           				 								//����DMA2D

    while((DMA2D->ISR & (1 << 1)) == 0) { 										//�ȴ��������
        timeout++;

        if(timeout > 0X1FFFFF) {
            break;    															//��ʱ�˳�
        }
    }

    DMA2D->IFCR |= 1 << 1;          											//���������ɱ�־
}
//��ָ����������䵥����ɫ
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)
//color:Ҫ������ɫ
//void LTDC_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint32_t color)
//{
//  uint32_t psx,psy,pex,pey;  								 					//��LCD���Ϊ��׼������ϵ,����������仯���仯
//  uint32_t timeout=0;
//  uint16_t offline;
//  uint32_t addr;
//    if(ex>=lcdltdc.width)ex=lcdltdc.width-1;
//  if(ey>=lcdltdc.height)ey=lcdltdc.height-1;
//  //����ϵת��
//  if(lcdltdc.dir) 															//����
//  {
//      psx=sx;psy=sy;
//      pex=ex;pey=ey;
//  }else           															//����
//  {
//      psx=sy;psy=lcdltdc.pheight-ex-1;
//      pex=ey;pey=lcdltdc.pheight-sx-1;
//  }
//  offline=lcdltdc.pwidth-(pex-psx+1);
//  addr=((uint32_t)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*psy+psx));
//    if(LCD_PIXFORMAT==LCD_PIXEL_FORMAT_RGB565)  								//�����RGB565��ʽ�Ļ���Ҫ������ɫת������16bitת��Ϊ32bit��
//    {
//        color=((color&0X0000F800)<<8)|((color&0X000007E0)<<5)|((color&0X0000001F)<<3);
//    }
//  //����DMA2D��ģʽ
//  DMA2D_Handler.Instance=DMA2D;
//  DMA2D_Handler.Init.Mode=DMA2D_R2M;          								//�ڴ浽�洢��ģʽ
//  DMA2D_Handler.Init.ColorMode=LCD_PIXFORMAT; 								//������ɫ��ʽ
//  DMA2D_Handler.Init.OutputOffset=offline;        							//���ƫ��
//  HAL_DMA2D_Init(&DMA2D_Handler);              								//��ʼ��DMA2D
//    HAL_DMA2D_ConfigLayer(&DMA2D_Handler,lcdltdc.activelayer); 				//������
//    HAL_DMA2D_Start(&DMA2D_Handler,color,(uint32_t)addr,pex-psx+1,pey-psy+1);	//��������
//    HAL_DMA2D_PollForTransfer(&DMA2D_Handler,1000);							//��������
//    while((__HAL_DMA2D_GET_FLAG(&DMA2D_Handler,DMA2D_FLAG_TC)==0)&&(timeout<0X5000))//�ȴ�DMA2D���
//    {
//        timeout++;
//    }
//    __HAL_DMA2D_CLEAR_FLAG(&DMA2D_Handler,DMA2D_FLAG_TC);       				//���������ɱ�־
//}

																				//��ָ�����������ָ����ɫ��,DMA2D���
																				//�˺�����֧��uint16_t,RGB565��ʽ����ɫ�������.
																				//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)
																				//ע��:sx,ex,���ܴ���lcddev.width-1;sy,ey,���ܴ���lcddev.height-1!!!
																				//color:Ҫ������ɫ�����׵�ַ
void LTDC_Color_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color)
{
    uint32_t psx, psy, pex, pey; 												//��LCD���Ϊ��׼������ϵ,����������仯���仯
    uint32_t timeout = 0;
    uint16_t offline;
    uint32_t addr;

																				//����ϵת��
    if(lcdltdc.dir) { 															//����
        psx = sx;
        psy = sy;
        pex = ex;
        pey = ey;
    } else {         															//����
        psx = sy;
        psy = lcdltdc.pheight - ex - 1;
        pex = ey;
        pey = lcdltdc.pheight - sx - 1;
    }

    offline = lcdltdc.pwidth - (pex - psx + 1);
    addr = ((uint32_t)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * psy + psx));
    RCC->AHB1ENR |= 1 << 23;        											//ʹ��DM2Dʱ��
    DMA2D->CR = 0 << 16;            											//�洢�����洢��ģʽ
    DMA2D->FGPFCCR = LCD_PIXFORMAT; 											//������ɫ��ʽ
    DMA2D->FGOR = 0;                											//ǰ������ƫ��Ϊ0
    DMA2D->OOR = offline;           											//������ƫ��
    DMA2D->CR &= ~(1 << 0);         											//��ֹͣDMA2D
    DMA2D->FGMAR = (uint32_t)color;     										//Դ��ַ
    DMA2D->OMAR = addr;             											//����洢����ַ
    DMA2D->NLR = (pey - psy + 1) | ((pex - psx + 1) << 16); 					//�趨�����Ĵ���
    DMA2D->CR |= 1 << 0;            											//����DMA2D

    while((DMA2D->ISR & (1 << 1)) == 0) {	 									//�ȴ��������
        timeout++;

        if(timeout > 0X1FFFFF) {
            break;    															//��ʱ�˳�
        }
    }

    DMA2D->IFCR |= 1 << 1;          											//���������ɱ�־
}

																				//LCD����
																				//color:��ɫֵ
void LTDC_Clear(uint32_t color)
{
    LTDC_Fill(0, 0, lcdltdc.width - 1, lcdltdc.height - 1, color);
}

//LTDCʱ��(Fdclk)���ú���
//Fvco=Fin*pllsain;
//Fdclk=Fvco/pllsair/2*2^pllsaidivr=Fin*pllsain/pllsair/2*2^pllsaidivr;

//Fvco:VCOƵ��
//Fin:����ʱ��Ƶ��һ��Ϊ1Mhz(����ϵͳʱ��PLLM��Ƶ���ʱ��,��ʱ����ͼ)
//pllsain:SAIʱ�ӱ�Ƶϵ��N,ȡֵ��Χ:50~432.
//pllsair:SAIʱ�ӵķ�Ƶϵ��R,ȡֵ��Χ:2~7
//pllsaidivr:LCDʱ�ӷ�Ƶϵ��,ȡֵ��Χ:0~3,��Ӧ��Ƶ2~16
//����:�ⲿ����Ϊ25M,pllm=25��ʱ��,Fin=1Mhz.
//����:Ҫ�õ�20M��LTDCʱ��,���������:pllsain=400,pllsair=5,pllsaidivr=1
//Fdclk=1*396/3/2*2^1=396/12=33Mhz
//����ֵ:0,�ɹ�;1,ʧ�ܡ�
uint8_t LTDC_Clk_Set(uint32_t pllsain, uint32_t pllsair, uint32_t pllsaidivr)
{
    RCC_PeriphCLKInitTypeDef PeriphClkIniture;

																				//LTDC�������ʱ�ӣ���Ҫ�����Լ���ʹ�õ�LCD�����ֲ������ã�
    PeriphClkIniture.PeriphClockSelection = RCC_PERIPHCLK_LTDC; 				//LTDCʱ��
    PeriphClkIniture.PLLSAI.PLLSAIN = pllsain;
    PeriphClkIniture.PLLSAI.PLLSAIR = pllsair;
    PeriphClkIniture.PLLSAIDivR = pllsaidivr;

    if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkIniture) == HAL_OK) { 				//��������ʱ�ӣ���������Ϊʱ��Ϊ18.75MHZ
        return 0;   //�ɹ�
    } else {
        return 1;    //ʧ��
    }
}

//LTDC,���մ�������,������LCD�������ϵΪ��׼
//ע��:�˺���������LTDC_Layer_Parameter_Config֮��������.
//layerx:��ֵ,0/1.
//sx,sy:��ʼ����
//width,height:��Ⱥ͸߶�
void LTDC_Layer_Window_Config(uint8_t layerx, uint16_t sx, uint16_t sy, uint16_t width, uint16_t height)
{
    HAL_LTDC_SetWindowPosition(&LTDC_Handler, sx, sy, layerx); 					//���ô��ڵ�λ��
    HAL_LTDC_SetWindowSize(&LTDC_Handler, width, height, layerx); 				//���ô��ڴ�С
}

//LTDC,������������.
//ע��:�˺���,������LTDC_Layer_Window_Config֮ǰ����.
//layerx:��ֵ,0/1.
//bufaddr:����ɫ֡������ʼ��ַ
//pixformat:��ɫ��ʽ.0,ARGB8888;1,RGB888;2,RGB565;3,ARGB1555;4,ARGB4444;5,L8;6;AL44;7;AL88
//alpha:����ɫAlphaֵ,0,ȫ͸��;255,��͸��
//alpha0:Ĭ����ɫAlphaֵ,0,ȫ͸��;255,��͸��
//bfac1:���ϵ��1,4(100),�㶨��Alpha;6(101),����Alpha*�㶨Alpha
//bfac2:���ϵ��2,5(101),�㶨��Alpha;7(111),����Alpha*�㶨Alpha
//bkcolor:��Ĭ����ɫ,32λ,��24λ��Ч,RGB888��ʽ
//����ֵ:��
void LTDC_Layer_Parameter_Config(uint8_t layerx, uint32_t bufaddr, uint8_t pixformat, uint8_t alpha, uint8_t alpha0, uint8_t bfac1, uint8_t bfac2, uint32_t bkcolor)
{
    LTDC_LayerCfgTypeDef pLayerCfg;

    pLayerCfg.WindowX0 = 0;                     								//������ʼX����
    pLayerCfg.WindowY0 = 0;                     								//������ʼY����
    pLayerCfg.WindowX1 = lcdltdc.pwidth;        								//������ֹX����
    pLayerCfg.WindowY1 = lcdltdc.pheight;       								//������ֹY����
    pLayerCfg.PixelFormat = pixformat;         	 								//���ظ�ʽ
    pLayerCfg.Alpha = alpha;                    								//Alphaֵ���ã�0~255,255Ϊ��ȫ��͸��
    pLayerCfg.Alpha0 = alpha0;                  								//Ĭ��Alphaֵ
    pLayerCfg.BlendingFactor1 = (uint32_t)bfac1 << 8; 							//���ò���ϵ��
    pLayerCfg.BlendingFactor2 = (uint32_t)bfac2 << 8; 							//���ò���ϵ��
    pLayerCfg.FBStartAdress = bufaddr;          								//���ò���ɫ֡������ʼ��ַ
    pLayerCfg.ImageWidth = lcdltdc.pwidth;      								//������ɫ֡�������Ŀ��
    pLayerCfg.ImageHeight = lcdltdc.pheight;    								//������ɫ֡�������ĸ߶�
    pLayerCfg.Backcolor.Red = (uint8_t)(bkcolor & 0X00FF0000) >> 16; 			//������ɫ��ɫ����
    pLayerCfg.Backcolor.Green = (uint8_t)(bkcolor & 0X0000FF00) >> 8; 			//������ɫ��ɫ����
    pLayerCfg.Backcolor.Blue = (uint8_t)bkcolor & 0X000000FF;    				//������ɫ��ɫ����
    HAL_LTDC_ConfigLayer(&LTDC_Handler, &pLayerCfg, layerx); 					//������ѡ�еĲ�
}

//��ȡ������
//PG6=R7(M0);PI2=G7(M1);PI7=B7(M2);
//M2:M1:M0
//0 :0 :0   //4.3��480*272 RGB��,ID=0X4342
//0 :0 :1   //7��800*480 RGB��,ID=0X7084
//0 :1 :0   //7��1024*600 RGB��,ID=0X7016
//0 :1 :1   //7��1280*800 RGB��,ID=0X7018
//1 :0 :0   //8��1024*768 RGB��,ID=0X8017
//����ֵ:LCD ID:0,�Ƿ�;����ֵ,ID;
uint16_t LTDC_PanelID_Read(void)
{
    uint8_t idx = 0;
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOG_CLK_ENABLE();       										//ʹ��GPIOGʱ��
    __HAL_RCC_GPIOI_CLK_ENABLE();       										//ʹ��GPIOIʱ��

    GPIO_Initure.Pin = GPIO_PIN_6;      										//PG6
    GPIO_Initure.Mode = GPIO_MODE_INPUT; 										//����
    GPIO_Initure.Pull = GPIO_PULLUP;    										//����
    GPIO_Initure.Speed = GPIO_SPEED_HIGH; 										//����
    HAL_GPIO_Init(GPIOG, &GPIO_Initure); 										//��ʼ��

    GPIO_Initure.Pin = GPIO_PIN_2 | GPIO_PIN_7; 								//PI2,7
    HAL_GPIO_Init(GPIOI, &GPIO_Initure);    									//��ʼ��

    idx = (uint8_t)HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_6); 						//��ȡM0
    idx |= (uint8_t)HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_2) << 1; 					//��ȡM1
    idx |= (uint8_t)HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_7) << 2; 					//��ȡM2

    if(idx == 0) {
        return 0X4342;   														//4.3����,480*272�ֱ���
    } else if(idx == 1) {
        return 0X7084;    														//7����,800*480�ֱ���
    } else if(idx == 2) {
        return 0X7016;    														//7����,1024*600�ֱ���
    } else if(idx == 3) {
        return 0X7018;    														//7����,1280*800�ֱ���
    } else if(idx == 4) {
        return 0X8017;    														//8����,1024*768�ֱ���
    } else {
        return 0;
    }
}

//LCD��ʼ������
void LTDC_Init(void)
{
    uint16_t lcdid = 0;

    lcdid = LTDC_PanelID_Read();        										//��ȡLCD���ID

    if(lcdid == 0X4342) {
        lcdltdc.pwidth = 480;          											//�����,��λ:����
        lcdltdc.pheight = 272;          										//���߶�,��λ:����
        lcdltdc.hsw = 1;                										//ˮƽͬ�����
        lcdltdc.vsw = 1;                										//��ֱͬ�����
        lcdltdc.hbp = 40;               										//ˮƽ����
        lcdltdc.vbp = 8;                										//��ֱ����
        lcdltdc.hfp = 5;                										//ˮƽǰ��
        lcdltdc.vfp = 8;                										//��ֱǰ��
        LTDC_Clk_Set(288, 4, RCC_PLLSAIDIVR_8); 								//��������ʱ�� 9Mhz
        //������������.
    } else if(lcdid == 0X7084) {
        lcdltdc.pwidth = 800;           										//�����,��λ:����
        lcdltdc.pheight = 480;          										//���߶�,��λ:����
        lcdltdc.hsw = 1;                										//ˮƽͬ�����
        lcdltdc.vsw = 1;                										//��ֱͬ�����
        lcdltdc.hbp = 46;               										//ˮƽ����
        lcdltdc.vbp = 23;               										//��ֱ����
        lcdltdc.hfp = 210;              										//ˮƽǰ��
        lcdltdc.vfp = 22;               										//��ֱǰ��
        LTDC_Clk_Set(396, 3, RCC_PLLSAIDIVR_4); 								//��������ʱ�� 33M(�����˫��,��Ҫ����DCLK��:18.75Mhz  300/4/4,�Ż�ȽϺ�)
    } else if(lcdid == 0X7016) {
        lcdltdc.pwidth = 1024;          										//�����,��λ:����
        lcdltdc.pheight = 600;          										//���߶�,��λ:����
        lcdltdc.hsw = 20;               										//ˮƽͬ�����
        lcdltdc.vsw = 3;                										//��ֱͬ�����
        lcdltdc.hbp = 140;              										//ˮƽ����
        lcdltdc.vbp = 20;               										//��ֱ����
        lcdltdc.hfp = 160;              										//ˮƽǰ��
        lcdltdc.vfp = 12;               										//��ֱǰ��
        LTDC_Clk_Set(360, 2, RCC_PLLSAIDIVR_4); 								//��������ʱ��  45Mhz
        //������������.
    } else if(lcdid == 0X7018) {
        lcdltdc.pwidth = 1280;          										//�����,��λ:����
        lcdltdc.pheight = 800;          										//���߶�,��λ:����
        //������������.
    } else if(lcdid == 0X8017) {
        lcdltdc.pwidth = 1024;          										//�����,��λ:����
        lcdltdc.pheight = 768;          										//���߶�,��λ:����
        //������������.
    }

    lcddev.width = lcdltdc.pwidth;
    lcddev.height = lcdltdc.pheight;

#if LCD_PIXFORMAT==LCD_PIXFORMAT_ARGB8888||LCD_PIXFORMAT==LCD_PIXFORMAT_RGB888
    ltdc_framebuf[0] = (uint32_t *)&ltdc_lcd_framebuf;
    lcdltdc.pixsize = 4;            //ÿ������ռ4���ֽ�
#else
    lcdltdc.pixsize = 2;            //ÿ������ռ2���ֽ�
    ltdc_framebuf[0] = (uint32_t *)&ltdc_lcd_framebuf;
#endif

    //LTDC����
    LTDC_Handler.Instance = LTDC;
    LTDC_Handler.Init.HSPolarity = LTDC_HSPOLARITY_AL;							//ˮƽͬ������
    LTDC_Handler.Init.VSPolarity = LTDC_VSPOLARITY_AL;       					//��ֱͬ������
    LTDC_Handler.Init.DEPolarity = LTDC_DEPOLARITY_AL;       					//����ʹ�ܼ���
    LTDC_Handler.Init.PCPolarity = LTDC_PCPOLARITY_IPC;      					//����ʱ�Ӽ���
    LTDC_Handler.Init.HorizontalSync = lcdltdc.hsw - 1;      					//ˮƽͬ�����
    LTDC_Handler.Init.VerticalSync = lcdltdc.vsw - 1;        					//��ֱͬ�����
    LTDC_Handler.Init.AccumulatedHBP = lcdltdc.hsw + lcdltdc.hbp - 1; 			//ˮƽͬ�����ؿ��
    LTDC_Handler.Init.AccumulatedVBP = lcdltdc.vsw + lcdltdc.vbp - 1; 			//��ֱͬ�����ظ߶�
    LTDC_Handler.Init.AccumulatedActiveW = lcdltdc.hsw + lcdltdc.hbp + lcdltdc.pwidth - 1; 				//��Ч���
    LTDC_Handler.Init.AccumulatedActiveH = lcdltdc.vsw + lcdltdc.vbp + lcdltdc.pheight - 1; 			//��Ч�߶�
    LTDC_Handler.Init.TotalWidth = lcdltdc.hsw + lcdltdc.hbp + lcdltdc.pwidth + lcdltdc.hfp - 1; 		//�ܿ��
    LTDC_Handler.Init.TotalHeigh = lcdltdc.vsw + lcdltdc.vbp + lcdltdc.pheight + lcdltdc.vfp - 1; 		//�ܸ߶�
    LTDC_Handler.Init.Backcolor.Red = 0;         								//��Ļ�������ɫ����
    LTDC_Handler.Init.Backcolor.Green = 0;       								//��Ļ��������ɫ����
    LTDC_Handler.Init.Backcolor.Blue = 0;        								//��Ļ����ɫ��ɫ����
    HAL_LTDC_Init(&LTDC_Handler);

    //������
    LTDC_Layer_Parameter_Config(0, (uint32_t)ltdc_framebuf[0], LCD_PIXFORMAT, 255, 0, 6, 7, 0X000000); 	//���������
    LTDC_Layer_Window_Config(0, 0, 0, lcdltdc.pwidth, lcdltdc.pheight); 								//�㴰������,��LCD�������ϵΪ��׼,��Ҫ����޸�!

    LTDC_Display_Dir(0);            											//Ĭ������ 0
    LTDC_Select_Layer(0);           											//ѡ���1�� 0
    LCD_LED(1);                     											//��������
    LTDC_Clear(0XFFFFFFFF);         											//����
}

//LTDC�ײ�IO��ʼ����ʱ��ʹ��
//�˺����ᱻHAL_LTDC_Init()����
//hltdc:LTDC���
void HAL_LTDC_MspInit(LTDC_HandleTypeDef *hltdc)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_LTDC_CLK_ENABLE();                								//ʹ��LTDCʱ��
    __HAL_RCC_DMA2D_CLK_ENABLE();               								//ʹ��DMA2Dʱ��
    __HAL_RCC_GPIOB_CLK_ENABLE();               								//ʹ��GPIOBʱ��
    __HAL_RCC_GPIOF_CLK_ENABLE();               								//ʹ��GPIOFʱ��
    __HAL_RCC_GPIOG_CLK_ENABLE();               								//ʹ��GPIOGʱ��
    __HAL_RCC_GPIOH_CLK_ENABLE();               								//ʹ��GPIOHʱ��
    __HAL_RCC_GPIOI_CLK_ENABLE();               								//ʹ��GPIOIʱ��

																				//��ʼ��PB5����������
    GPIO_Initure.Pin = GPIO_PIN_5;              								//PB5������������Ʊ���
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;    								//�������
    GPIO_Initure.Pull = GPIO_PULLUP;           	 								//����
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;       								//����
    HAL_GPIO_Init(GPIOB, &GPIO_Initure);

																				//��ʼ��PF10
    GPIO_Initure.Pin = GPIO_PIN_10;
    GPIO_Initure.Mode = GPIO_MODE_AF_PP;        								//����
    GPIO_Initure.Pull = GPIO_NOPULL;
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;       								//����
    GPIO_Initure.Alternate = GPIO_AF14_LTDC;    								//����ΪLTDC
    HAL_GPIO_Init(GPIOF, &GPIO_Initure);

																				//��ʼ��PG6,7,11
    GPIO_Initure.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_11;
    HAL_GPIO_Init(GPIOG, &GPIO_Initure);

																				//��ʼ��PH9,10,11,12,13,14,15
    GPIO_Initure.Pin = GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | \
                       GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOH, &GPIO_Initure);

																				//��ʼ��PI0,1,2,4,5,6,7,9,10
    GPIO_Initure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5 | \
                       GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_9 | GPIO_PIN_10;
    HAL_GPIO_Init(GPIOI, &GPIO_Initure);
}
#endif
/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/
