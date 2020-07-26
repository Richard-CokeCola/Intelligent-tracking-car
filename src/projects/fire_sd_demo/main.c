/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：main.c
 * 描述         ：SD卡带文件系统实验
 *
 * 实验平台     ：野火kinetis开发板
 * 库版本       ：
 * 嵌入系统     ：
 *
 * 作者         ：野火嵌入式开发工作室
 * 淘宝店       ：http://firestm32.taobao.com
 * 技术支持论坛 ：http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/



#include "common.h"
#include "include.h"
#include "ff.h"


#define BUFF_SIZE   100


void main(void)
{
    FIL fdst;  //文件
    FATFS fs;  //文件系统

    uint32 size, sizetmp;
    int res;
    int i,j;
    char ch2[7010];
    //for(j=0;j<15;j++)
    for(i=0;i<7000;i++)
    {
      
       ch2[i]=1+0x30;
      
    }
    char *str = "山东交通学院";
    u8 buff[BUFF_SIZE];

    for(size = 0; size < BUFF_SIZE; size++)
      buff[size] = 0;

    f_mount(0, &fs);//SD卡系统初始化

    res = f_open(&fdst, "0:/FireDemo.txt", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);  //打开文件，如果没有就创建，带读写打开

    if( res == FR_DISK_ERR)
    {
        printf( "\n没插入SD卡？？\n" );
        return;
    }
    else if ( res == FR_OK )
    {
        printf( "\n文件打开成功 \n" );
    }
    else
    {
        printf("\n返回值异常");
    }


    f_puts(ch2, &fdst);                     //往文件里写入字符串str

   // size = f_size(&fdst);                   //获取文件的大小

   // printf( "\n文件大小为：%d \n" , size);  //串口打印文件的大小

    if(size > BUFF_SIZE)size = BUFF_SIZE;   //防止溢出

  //  f_lseek(&fdst, 0);                      //把指针指向文件顶部
   // f_read (&fdst, buff, size, (UINT *)&sizetmp);   //读取


   // printf("文件内容为：%s", (char const *)buff);

    f_close(&fdst);                         //关闭文件
}


FATFS fs;            // Work area (file system object) for logical drive
DIR dirs;
FRESULT res;         // FatFs function common result code
UINT br, bw;        // File Write count
FIL faddata;  //
unsigned char filebuff[512] ={0}; //"1.山东交通学院"; //文件缓存
unsigned char fileflag[15] = "00,ff,01,00"; //{0x00,0xff,0x01,0x00};
uint16 filebuffoffset = 512;
void main2(void)
{
	
    uint32 i;//运行计数器
    res = f_mount(0, &fs);     // for logical drive 0
    for(i = 63; i < 512; i++) //要写入的数据，用户可以把数据放到该数组，然后写入SD卡即可。
    {
        filebuff[i] = 'x';  //更新缓冲区的数据内容
    }
    filebuff[509] = ' ';  //空格
    filebuff[510] = 0x0d; //回车
    filebuff[511] = 0x0a; //最后一个字符为换行
	
    res = f_open(&faddata, "M&Y_CCD_Data.txt", FA_CREATE_ALWAYS | FA_WRITE | FA_READ); //打开SD卡文件，如果没有找到，则创建一个文件
    if(res)  // 打开失败，则创建文件，然后重新打开,LED2闪烁两下
    {
        res = f_close(&faddata);//关闭M&Y_CCD_Data.txt
    }
    res = f_open(&faddata, "M&Y_CCD_Data.txt", FA_CREATE_ALWAYS | FA_WRITE | FA_READ); //打开SD卡文件，如果没有找到，则创建一个文件
    if(!res)  // 打开文件成功
    {
        for(i = 0; i < 10; i++)
        {
            //写入数据
            res = f_lseek(&faddata, f_size(&faddata));//写指针移到文件末尾
            res = f_write(&faddata, fileflag, 15, &bw);
            filebuff[0] = i + '0'; //修改块索引号
            res = f_lseek(&faddata, f_size(&faddata));//写指针移到文件末尾
            res = f_write(&faddata, filebuff, filebuffoffset, &bw); //将文件缓冲区里的数据写入M&Y_CCD_Data.txt
        }
    }
    res = f_close(&faddata);//关闭lq_ccd_data.txt
}

