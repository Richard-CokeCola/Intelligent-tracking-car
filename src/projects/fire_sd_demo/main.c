/******************** (C) COPYRIGHT 2011 Ұ��Ƕ��ʽ���������� ********************
 * �ļ���       ��main.c
 * ����         ��SD�����ļ�ϵͳʵ��
 *
 * ʵ��ƽ̨     ��Ұ��kinetis������
 * ��汾       ��
 * Ƕ��ϵͳ     ��
 *
 * ����         ��Ұ��Ƕ��ʽ����������
 * �Ա���       ��http://firestm32.taobao.com
 * ����֧����̳ ��http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/



#include "common.h"
#include "include.h"
#include "ff.h"


#define BUFF_SIZE   100


void main(void)
{
    FIL fdst;  //�ļ�
    FATFS fs;  //�ļ�ϵͳ

    uint32 size, sizetmp;
    int res;
    int i,j;
    char ch2[7010];
    //for(j=0;j<15;j++)
    for(i=0;i<7000;i++)
    {
      
       ch2[i]=1+0x30;
      
    }
    char *str = "ɽ����ͨѧԺ";
    u8 buff[BUFF_SIZE];

    for(size = 0; size < BUFF_SIZE; size++)
      buff[size] = 0;

    f_mount(0, &fs);//SD��ϵͳ��ʼ��

    res = f_open(&fdst, "0:/FireDemo.txt", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);  //���ļ������û�оʹ���������д��

    if( res == FR_DISK_ERR)
    {
        printf( "\nû����SD������\n" );
        return;
    }
    else if ( res == FR_OK )
    {
        printf( "\n�ļ��򿪳ɹ� \n" );
    }
    else
    {
        printf("\n����ֵ�쳣");
    }


    f_puts(ch2, &fdst);                     //���ļ���д���ַ���str

   // size = f_size(&fdst);                   //��ȡ�ļ��Ĵ�С

   // printf( "\n�ļ���СΪ��%d \n" , size);  //���ڴ�ӡ�ļ��Ĵ�С

    if(size > BUFF_SIZE)size = BUFF_SIZE;   //��ֹ���

  //  f_lseek(&fdst, 0);                      //��ָ��ָ���ļ�����
   // f_read (&fdst, buff, size, (UINT *)&sizetmp);   //��ȡ


   // printf("�ļ�����Ϊ��%s", (char const *)buff);

    f_close(&fdst);                         //�ر��ļ�
}


FATFS fs;            // Work area (file system object) for logical drive
DIR dirs;
FRESULT res;         // FatFs function common result code
UINT br, bw;        // File Write count
FIL faddata;  //
unsigned char filebuff[512] ={0}; //"1.ɽ����ͨѧԺ"; //�ļ�����
unsigned char fileflag[15] = "00,ff,01,00"; //{0x00,0xff,0x01,0x00};
uint16 filebuffoffset = 512;
void main2(void)
{
	
    uint32 i;//���м�����
    res = f_mount(0, &fs);     // for logical drive 0
    for(i = 63; i < 512; i++) //Ҫд������ݣ��û����԰����ݷŵ������飬Ȼ��д��SD�����ɡ�
    {
        filebuff[i] = 'x';  //���»���������������
    }
    filebuff[509] = ' ';  //�ո�
    filebuff[510] = 0x0d; //�س�
    filebuff[511] = 0x0a; //���һ���ַ�Ϊ����
	
    res = f_open(&faddata, "M&Y_CCD_Data.txt", FA_CREATE_ALWAYS | FA_WRITE | FA_READ); //��SD���ļ������û���ҵ����򴴽�һ���ļ�
    if(res)  // ��ʧ�ܣ��򴴽��ļ���Ȼ�����´�,LED2��˸����
    {
        res = f_close(&faddata);//�ر�M&Y_CCD_Data.txt
    }
    res = f_open(&faddata, "M&Y_CCD_Data.txt", FA_CREATE_ALWAYS | FA_WRITE | FA_READ); //��SD���ļ������û���ҵ����򴴽�һ���ļ�
    if(!res)  // ���ļ��ɹ�
    {
        for(i = 0; i < 10; i++)
        {
            //д������
            res = f_lseek(&faddata, f_size(&faddata));//дָ���Ƶ��ļ�ĩβ
            res = f_write(&faddata, fileflag, 15, &bw);
            filebuff[0] = i + '0'; //�޸Ŀ�������
            res = f_lseek(&faddata, f_size(&faddata));//дָ���Ƶ��ļ�ĩβ
            res = f_write(&faddata, filebuff, filebuffoffset, &bw); //���ļ��������������д��M&Y_CCD_Data.txt
        }
    }
    res = f_close(&faddata);//�ر�lq_ccd_data.txt
}
