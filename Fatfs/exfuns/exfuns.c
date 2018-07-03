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
 * @file      exfun.c
 * @author    GuanXianDe
 * @version   V1.0.0
 * @date      22-June-2018
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
#if(USE_Fatfs_File== 1)

#include "string.h"
#include "exfuns.h"
#include "malloc.h"
//#include "fattester.h"
#include "bsp_rtc.h"
/*
********************************************************************************
*                             MACRO DEFINITIONS
********************************************************************************
*/
#define FILE_MAX_TYPE_NUM		7														//���FILE_MAX_TYPE_NUM������
#define FILE_MAX_SUBT_NUM		4														//���FILE_MAX_SUBT_NUM��С��
/*
********************************************************************************
*                          OS-RELATED    VARIABLES
********************************************************************************
*/
																						//�ļ������б�
uint8_t*const FILE_TYPE_TBL[FILE_MAX_TYPE_NUM][FILE_MAX_SUBT_NUM]=
{
    {"BIN"},																			//BIN�ļ�
    {"LRC"},																			//LRC�ļ�
    {"NES","SMS"},																		//NES/SMS�ļ�
    {"TXT","C","H"},																	//�ı��ļ�
    {"WAV","MP3","APE","FLAC"},															//֧�ֵ������ļ�
    {"BMP","JPG","JPEG","GIF"},															//ͼƬ�ļ�
    {"AVI"},																			//��Ƶ�ļ�
};
///////////////////////////////�����ļ���,ʹ��malloc��ʱ��////////////////////////////////////////////
FATFS *fs[_VOLUMES];																	//�߼����̹�����.
FIL *file;	  																			//�ļ�1
FIL *ftemp;	  																			//�ļ�2.
UINT br,bw;																				//��д����
FILINFO fileinfo;																		//�ļ���Ϣ
DIR dir;  																				//Ŀ¼

uint8_t *fatbuf;																		//SD�����ݻ�����
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
 * @brief :	Ϊexfuns�����ڴ�
 * @param :	none.
 * @retval:
			����ֵ:0,�ɹ�
			1,ʧ��
 */
uint8_t exfuns_init(void)
{
    uint8_t i;
    for(i=0; i<_VOLUMES; i++)
    {
        fs[i]=(FATFS*)mymalloc(SRAMIN,sizeof(FATFS));									//Ϊ����i�����������ڴ�
        if(!fs[i])break;
    }
    file=(FIL*)mymalloc(SRAMIN,sizeof(FIL));											//Ϊfile�����ڴ�
    ftemp=(FIL*)mymalloc(SRAMIN,sizeof(FIL));											//Ϊftemp�����ڴ�
    fatbuf=(uint8_t*)mymalloc(SRAMIN,512);												//Ϊfatbuf�����ڴ�
    if(i==_VOLUMES&&file&&ftemp&&fatbuf)return 0;  										//������һ��ʧ��,��ʧ��.
    else return 1;
}

/**
 * @brief :	��Сд��ĸתΪ��д��ĸ,���������,�򱣳ֲ���.
 * @param :	c:ת��������
 * @retval:	����ת���������
 */
uint8_t char_upper(uint8_t c)
{
    if(c<'A')return c;																	//����,���ֲ���.
    if(c>='a')return c-0x20;															//��Ϊ��д.
    else return c;																		//��д,���ֲ���
}

/**
 * @brief :	�����ļ�������
 * @param :	fname:�ļ���
 * @retval:
			0XFF,��ʾ�޷�ʶ����ļ����ͱ��.
			����,����λ��ʾ��������,����λ��ʾ����С��.
 */
uint8_t f_typetell(uint8_t *fname)
{
    uint8_t tbuf[5];
    uint8_t *attr='\0';																	//��׺��
    uint8_t i=0,j;
    while(i<250)
    {
        i++;
        if(*fname=='\0')break;															//ƫ�Ƶ��������.
        fname++;
    }
    if(i==250)return 0XFF;																//������ַ���.
    for(i=0; i<5; i++) 																	//�õ���׺��
    {
        fname--;
        if(*fname=='.')
        {
            fname++;
            attr=fname;
            break;
        }
    }
    strcpy((char *)tbuf,(const char*)attr);												//copy
    for(i=0; i<4; i++)tbuf[i]=char_upper(tbuf[i]); 										//ȫ����Ϊ��д
    for(i=0; i<FILE_MAX_TYPE_NUM; i++)													//����Ա�
    {
        for(j=0; j<FILE_MAX_SUBT_NUM; j++) 												//����Ա�
        {
            if(*FILE_TYPE_TBL[i][j]==0)break;											//�����Ѿ�û�пɶԱȵĳ�Ա��.
            if(strcmp((const char *)FILE_TYPE_TBL[i][j],(const char *)tbuf)==0)			//�ҵ���
            {
                return (i<<4)|j;
            }
        }
    }
    return 0XFF;																		//û�ҵ�
}

/**
 * @brief :	�õ�����ʣ������
 * @param :
			drv:���̱��("0:"/"1:")
			total:������	 ����λKB��
			free:ʣ������	 ����λKB��
 * @retval:	0,����.����,�������
 */
uint8_t exf_getfree(uint8_t *drv,uint32_t *total,uint32_t *free)
{
    FATFS *fs1;
    uint8_t res;
    uint32_t fre_clust=0, fre_sect=0, tot_sect=0;
																						//�õ�������Ϣ�����д�����
    res =(uint32_t)f_getfree((const TCHAR*)drv, (DWORD*)&fre_clust, &fs1);
    if(res==0)
    {
        tot_sect=(fs1->n_fatent-2)*fs1->csize;											//�õ���������
        fre_sect=fre_clust*fs1->csize;													//�õ�����������
#if _MAX_SS!=512				  														//������С����512�ֽ�,��ת��Ϊ512�ֽ�
        tot_sect*=fs1->ssize/512;
        fre_sect*=fs1->ssize/512;
#endif
        *total=tot_sect>>1;																//��λΪKB
        *free=fre_sect>>1;																//��λΪKB
    }
    return res;
}

/**
 * @brief :	�ļ����ƣ�ע���ļ���С��Ҫ����4GB.��
 * @param :
			��psrc�ļ�,copy��pdst.
			fcpymsg,����ָ��,����ʵ�ֿ���ʱ����Ϣ��ʾ
			pname:�ļ�/�ļ�����
			pct:�ٷֱ�
			mode:
				[0]:�����ļ���
				[1]:���°ٷֱ�pct
				[2]:�����ļ���
				[3~7]:����
			psrc,pdst:Դ�ļ���Ŀ���ļ�
			totsize:�ܴ�С(��totsizeΪ0��ʱ��,��ʾ����Ϊ�����ļ�����)
			cpdsize:�Ѹ����˵Ĵ�С.
			fwmode:�ļ�д��ģʽ
			0:������ԭ�е��ļ�
			1:����ԭ�е��ļ�
 * @retval:
			0,����
			����,����,0XFF,ǿ���˳�
 */
uint8_t exf_copy(uint8_t(*fcpymsg)(uint8_t*pname,uint8_t pct,uint8_t mode),uint8_t *psrc,uint8_t *pdst,uint32_t totsize,uint32_t cpdsize,uint8_t fwmode)
{
    uint8_t res;
    uint16_t br=0;
    uint16_t bw=0;
    FIL *fsrc=0;
    FIL *fdst=0;
    uint8_t *fbuf=0;
    uint8_t curpct=0;
    unsigned long long lcpdsize=cpdsize;
    fsrc=(FIL*)mymalloc(SRAMIN,sizeof(FIL));											//�����ڴ�
    fdst=(FIL*)mymalloc(SRAMIN,sizeof(FIL));
    fbuf=(uint8_t*)mymalloc(SRAMIN,8192);
    if(fsrc==NULL||fdst==NULL||fbuf==NULL)res=100;										//ǰ���ֵ����fatfs
    else
    {
        if(fwmode==0)fwmode=FA_CREATE_NEW;												//������
        else fwmode=FA_CREATE_ALWAYS;	  												//���Ǵ��ڵ��ļ�

        res=f_open(fsrc,(const TCHAR*)psrc,FA_READ|FA_OPEN_EXISTING);					//��ֻ���ļ�
        if(res==0)res=f_open(fdst,(const TCHAR*)pdst,FA_WRITE|fwmode); 					//��һ���򿪳ɹ�,�ſ�ʼ�򿪵ڶ���
        if(res==0)																		//�������򿪳ɹ���
        {
            if(totsize==0)																//�����ǵ����ļ�����
            {
                totsize=fsrc->obj.objsize;
                lcpdsize=0;
                curpct=0;
            } else curpct=(lcpdsize*100)/totsize;										//�õ��°ٷֱ�
            fcpymsg(psrc,curpct,0X02);													//���°ٷֱ�
            while(res==0)																//��ʼ����
            {
                res=f_read(fsrc,fbuf,8192,(UINT*)&br);									//Դͷ����512�ֽ�
                if(res||br==0)break;
                res=f_write(fdst,fbuf,(UINT)br,(UINT*)&bw);								//д��Ŀ���ļ�
                lcpdsize+=bw;
                if(curpct!=(lcpdsize*100)/totsize)										//�Ƿ���Ҫ���°ٷֱ�
                {
                    curpct=(lcpdsize*100)/totsize;
                    if(fcpymsg(psrc,curpct,0X02))										//���°ٷֱ�
                    {
                        res=0XFF;														//ǿ���˳�
                        break;
                    }
                }
                if(res||bw<br)break;
            }
            f_close(fsrc);
            f_close(fdst);
        }
    }
    myfree(SRAMIN,fsrc);																//�ͷ��ڴ�
    myfree(SRAMIN,fdst);
    myfree(SRAMIN,fbuf);
    return res;
}

/**
 * @brief :	�õ�·���µ��ļ���
 * @param :	dpfn���ļ�·��
 * @retval:
			0,·�����Ǹ������.
			����,�ļ��������׵�ַ
 */
uint8_t* exf_get_src_dname(uint8_t* dpfn)
{
    uint16_t temp=0;
    while(*dpfn!=0)
    {
        dpfn++;
        temp++;
    }
    if(temp<4)return 0;
    while((*dpfn!=0x5c)&&(*dpfn!=0x2f))dpfn--;											//׷����������һ��"\"����"/"��
    return ++dpfn;
}

/**
 * @brief :	�õ��ļ��д�С��ע���ļ��д�С��Ҫ����4GB.��
 * @param :	none.
 * @retval:
			0,�ļ��д�СΪ0,���߶�ȡ�����з����˴���.
			����,�ļ��д�С.
 */
uint32_t exf_fdsize(uint8_t *fdname)
{
#define MAX_PATHNAME_DEPTH	512+1														//���Ŀ���ļ�·��+�ļ������
    uint8_t res=0;
    DIR *fddir=0;																		//Ŀ¼
    FILINFO *finfo=0;																	//�ļ���Ϣ
    uint8_t * pathname=0;																//Ŀ���ļ���·��+�ļ���
    uint16_t pathlen=0;																	//Ŀ��·������
    uint32_t fdsize=0;

    fddir=(DIR*)mymalloc(SRAMIN,sizeof(DIR));											//�����ڴ�
    finfo=(FILINFO*)mymalloc(SRAMIN,sizeof(FILINFO));
    if(fddir==NULL||finfo==NULL)res=100;
    if(res==0)
    {
        pathname=mymalloc(SRAMIN,MAX_PATHNAME_DEPTH);
        if(pathname==NULL)res=101;
        if(res==0)
        {
            pathname[0]=0;
            strcat((char*)pathname,(const char*)fdname); 								//����·��
            res=f_opendir(fddir,(const TCHAR*)fdname); 									//��ԴĿ¼
            if(res==0)																	//��Ŀ¼�ɹ�
            {
                while(res==0)															//��ʼ�����ļ�������Ķ���
                {
                    res=f_readdir(fddir,finfo);											//��ȡĿ¼�µ�һ���ļ�
                    if(res!=FR_OK||finfo->fname[0]==0)break;							//������/��ĩβ��,�˳�
                    if(finfo->fname[0]=='.')continue;     								//�����ϼ�Ŀ¼
                    if(finfo->fattrib&0X10)												//����Ŀ¼(�ļ�����,0X20,�鵵�ļ�;0X10,��Ŀ¼;)
                    {
                        pathlen=strlen((const char*)pathname);							//�õ���ǰ·���ĳ���
                        strcat((char*)pathname,(const char*)"/");						//��б��
                        strcat((char*)pathname,(const char*)finfo->fname);				//Դ·��������Ŀ¼����
                        //printf("\r\nsub folder:%s\r\n",pathname);						//��ӡ��Ŀ¼��
                        fdsize+=exf_fdsize(pathname);									//�õ���Ŀ¼��С,�ݹ����
                        pathname[pathlen]=0;											//���������
                    } else fdsize+=finfo->fsize;										//��Ŀ¼,ֱ�Ӽ����ļ��Ĵ�С

                }
            }
            myfree(SRAMIN,pathname);
        }
    }
    myfree(SRAMIN,fddir);
    myfree(SRAMIN,finfo);
    if(res)return 0;
    else return fdsize;
}

/**
 * @brief :	�ļ��и��ƣ�ע���ļ��д�С��Ҫ����4GB.��
 * @param :
			��psrc�ļ���,copy��pdst�ļ���.
			pdst:��������"X:"/"X:XX"/"X:XX/XX"֮���.����Ҫʵ��ȷ����һ���ļ��д���
			fcpymsg,����ָ��,����ʵ�ֿ���ʱ����Ϣ��ʾ
			pname:�ļ�/�ļ�����
			pct:�ٷֱ�
			mode:
				[0]:�����ļ���
				[1]:���°ٷֱ�pct
				[2]:�����ļ���
				[3~7]:����
			psrc,pdst:Դ�ļ��к�Ŀ���ļ���
			totsize:�ܴ�С(��totsizeΪ0��ʱ��,��ʾ����Ϊ�����ļ�����)
			cpdsize:�Ѹ����˵Ĵ�С.
			fwmode:�ļ�д��ģʽ
			0:������ԭ�е��ļ�
			1:����ԭ�е��ļ�
 * @retval:
			0,�ɹ�
			����,�������;0XFF,ǿ���˳�
 */
uint8_t exf_fdcopy(uint8_t(*fcpymsg)(uint8_t*pname,uint8_t pct,uint8_t mode),uint8_t *psrc,uint8_t *pdst,uint32_t *totsize,uint32_t *cpdsize,uint8_t fwmode)
{
#define MAX_PATHNAME_DEPTH	512+1														//���Ŀ���ļ�·��+�ļ������
    uint8_t res=0;
    DIR *srcdir=0;																		//ԴĿ¼
    DIR *dstdir=0;																		//ԴĿ¼
    FILINFO *finfo=0;																	//�ļ���Ϣ
    uint8_t *fn=0;   																	//���ļ���

    uint8_t * dstpathname=0;															//Ŀ���ļ���·��+�ļ���
    uint8_t * srcpathname=0;															//Դ�ļ���·��+�ļ���

    uint16_t dstpathlen=0;																//Ŀ��·������
    uint16_t srcpathlen=0;																//Դ·������


    srcdir=(DIR*)mymalloc(SRAMIN,sizeof(DIR));											//�����ڴ�
    dstdir=(DIR*)mymalloc(SRAMIN,sizeof(DIR));
    finfo=(FILINFO*)mymalloc(SRAMIN,sizeof(FILINFO));

    if(srcdir==NULL||dstdir==NULL||finfo==NULL)res=100;
    if(res==0)
    {
        dstpathname=mymalloc(SRAMIN,MAX_PATHNAME_DEPTH);
        srcpathname=mymalloc(SRAMIN,MAX_PATHNAME_DEPTH);
        if(dstpathname==NULL||srcpathname==NULL)res=101;
        if(res==0)
        {
            dstpathname[0]=0;
            srcpathname[0]=0;
            strcat((char*)srcpathname,(const char*)psrc); 								//����ԭʼԴ�ļ�·��
            strcat((char*)dstpathname,(const char*)pdst); 								//����ԭʼĿ���ļ�·��
            res=f_opendir(srcdir,(const TCHAR*)psrc); 									//��ԴĿ¼
            if(res==0)																	//��Ŀ¼�ɹ�
            {
                strcat((char*)dstpathname,(const char*)"/");							//����б��
                fn=exf_get_src_dname(psrc);
                if(fn==0)																//���꿽��
                {
                    dstpathlen=strlen((const char*)dstpathname);
                    dstpathname[dstpathlen]=psrc[0];									//��¼����
                    dstpathname[dstpathlen+1]=0;										//������
                } else strcat((char*)dstpathname,(const char*)fn);						//���ļ���
                fcpymsg(fn,0,0X04);														//�����ļ�����
                res=f_mkdir((const TCHAR*)dstpathname);									//����ļ����Ѿ�����,�Ͳ�����.��������ھʹ����µ��ļ���.
                if(res==FR_EXIST)res=0;
                while(res==0)															//��ʼ�����ļ�������Ķ���
                {
                    res=f_readdir(srcdir,finfo);										//��ȡĿ¼�µ�һ���ļ�
                    if(res!=FR_OK||finfo->fname[0]==0)break;							//������/��ĩβ��,�˳�
                    if(finfo->fname[0]=='.')continue;     								//�����ϼ�Ŀ¼
                    fn=(uint8_t*)finfo->fname; 											//�õ��ļ���
                    dstpathlen=strlen((const char*)dstpathname);						//�õ���ǰĿ��·���ĳ���
                    srcpathlen=strlen((const char*)srcpathname);						//�õ�Դ·������

                    strcat((char*)srcpathname,(const char*)"/");						//Դ·����б��
                    if(finfo->fattrib&0X10)												//����Ŀ¼(�ļ�����,0X20,�鵵�ļ�;0X10,��Ŀ¼;)
                    {
                        strcat((char*)srcpathname,(const char*)fn);						//Դ·��������Ŀ¼����
                        res=exf_fdcopy(fcpymsg,srcpathname,dstpathname,totsize,cpdsize,fwmode);		//�����ļ���
                    } else 																//��Ŀ¼
                    {
                        strcat((char*)dstpathname,(const char*)"/");					//Ŀ��·����б��
                        strcat((char*)dstpathname,(const char*)fn);						//Ŀ��·�����ļ���
                        strcat((char*)srcpathname,(const char*)fn);						//Դ·�����ļ���
                        fcpymsg(fn,0,0X01);												//�����ļ���
                        res=exf_copy(fcpymsg,srcpathname,dstpathname,*totsize,*cpdsize,fwmode);		//�����ļ�
                        *cpdsize+=finfo->fsize;											//����һ���ļ���С
                    }
                    srcpathname[srcpathlen]=0;											//���������
                    dstpathname[dstpathlen]=0;											//���������
                }
            }
            myfree(SRAMIN,dstpathname);
            myfree(SRAMIN,srcpathname);
        }
    }
    myfree(SRAMIN,srcdir);
    myfree(SRAMIN,dstdir);
    myfree(SRAMIN,finfo);
    return res;
}


void test_fatfa_cvs()
{
    FIL fileobj;
    FRESULT fr;
    UINT br;

    uint32_t total,free;
    uint32_t size=0,offset=0;
    uint8_t BUF[25],p[25];																//BUF��д���ݣ�pΪ�ļ�·��
    uint8_t len=sizeof(BUF);
    if(exf_getfree((uint8_t *)"0:",&total,&free) == 0)									//�õ�SD������������ʣ������
    {
        sprintf((char *)BUF,"ENECO-%04d-%02d-%02d.csv",RTC_GetTimeYear(),RTC_GetTimeMonth(),RTC_GetTimeDate());
        sprintf((char *)p,"0:%s",BUF);

        if(f_open (&fileobj,(const char *)p, FA_OPEN_ALWAYS | FA_READ | FA_WRITE) == FR_OK)
        {
            size = f_size(&fileobj);
            fr= f_lseek(&fileobj,size);
            if(size == 0)																//��һ�δ����ļ�����Ҫ���ӱ�����
            {
                fr= f_lseek(&fileobj,size);
                {
                    memset(BUF,0,sizeof(BUF));
                    sprintf((char *)BUF,"%s,%s,%s,%s\r\n","ʱ��","����","����","����");
                    fr= f_write(&fileobj, BUF, sizeof(BUF),(void *)&br);
                    f_sync(&fileobj);													//ˢ��д�ļ��Ļ�����Ϣ
                }
            }																			//ע��д���˱���������Ҫƫ��ָ���ˣ���Ϊǰ��д��������ʱ����˻��У��Ѿ��ǵڶ��е��׵�ַ��

            if(fr == FR_OK)
            {
                memset(BUF,0,sizeof(BUF));
                sprintf((char *)BUF,"%02d:%02d:%02d,%02d,%02d,%02d\r\n",RTC_GetTimeHours(),RTC_GetTimeMinutes(),RTC_GetTimeSeconds(),RTC_GetTimeHours(),RTC_GetTimeMinutes(),RTC_GetTimeSeconds());
                fr= f_write(&fileobj, BUF, sizeof(BUF),(void *)&br);
                f_sync(&fileobj);														//ˢ��д�ļ��Ļ�����Ϣ
            }
        }
        f_close(&fileobj);

        if(f_open (&fileobj,(const char *)p, FA_OPEN_ALWAYS | FA_READ | FA_WRITE) == FR_OK)
        {
            memset(BUF,0,sizeof(BUF));
            size=f_size(&fileobj);
																						//��ӡ������
////////////////////						{
////////////////////							uint32_t i=0;
////////////////////							for(i=size/len;i>0;i--)
////////////////////							{
////////////////////								f_lseek(&fileobj,offset);
////////////////////								f_read(&fileobj,BUF,len,&br);
////////////////////								printf("%s",BUF);
////////////////////								offset += len;
////////////////////							}
////////////////////							f_lseek(&fileobj,offset);
////////////////////							f_read(&fileobj,BUF,size%len,&br);
////////////////////							printf("%s\r\n",BUF);
////////////////////						}
																						//��ӡ���µ�һ��
            offset=size-len;
            f_lseek(&fileobj,offset);
            f_read(&fileobj,BUF,size,&br);
            printf("%s",BUF);

            printf("size  = %dByte, %dKb\r\n",size,size>>10);
            printf("Times = %d\r\n",size/len);
            printf("total = %dKb %dMB\r\n",total,total>>10);
            printf("free  = %dKb %dMB\r\n\r\n",free,free>>10);
        }
        f_close(&fileobj);
    }
}

#endif
/******************* (C) COPYRIGHT 2015-2018 ENECO POWER********END OF FILE****/











