/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：main.c
 * 描述         ：GPIO操作实验
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


#if 1       //这里选择 0 或 1 来切换编译各个个不同的main函数 ，这里共五个main函数，靠宏定义决定编译哪个
//0表示屏蔽这段代码


/*************************************************************************
*                             野火嵌入式开发工作室
*                         51编程风格的GPIO实验输出测试
*
*  实验说明：野火GPIO实验，利用LED来显示电平高低
*
*  实验操作：无
*
*  实验效果：LED0每隔500ms
*
*  实验目的：明白如何用51编程风格来设置IO口电平
*
*  修改时间：2012-2-28     已测试
*
*  备    注：野火Kinetis开发板的 LED0~3 ，分别接PTD15~PTD12 ，低电平点亮
*************************************************************************/
void  main(void)
{
    gpio_init  (PORTD, 15, GPO, HIGH);      //初始化 PTD15 ：输出高电平 ,即 初始化 LED0，灭

    while(1)
    {
        PTD15_OUT = 0;                      //低电平点亮LED0

        time_delay_ms(500);                //延时500ms

        PTD15_OUT = 1;                      //高电平熄灭LED0

        time_delay_ms(500);                //延时500ms
    }
}


#elif   0

/*************************************************************************
*                             野火嵌入式开发工作室
*                       51编程风格的GPIO实验输入输出测试
*
*  实验说明：野火51编程风格的GPIO实验，利用LED来显示电平高低
*
*  实验操作：短接PTD15和PTD14
*
*  实验效果：LED0、LED1 同时闪烁，间隔500ms
*
*  实验目的：明白如何使用51编程风格的GPIO操作
*
*  修改时间：2012-2-28     已测试
*
*  备    注：野火Kinetis开发板的 LED0~3 ，分别接PTD15~PTD12 ，低电平点亮
*************************************************************************/
void  main(void)
{
    gpio_init  (PORTD, 15, GPO, HIGH);      //初始化 PTD15 ：输出高电平 ,即 初始化 LED0，灭
    gpio_init  (PORTD, 14, GPI, HIGH);      //初始化 PTD14 ：输入

    while(1)
    {
        //DDRD14 = 0;                         //PTD设为输入（初始化已经设置了，这里就不设置了）
        if( PTD14_IN == 1 )
        {
            PTD15_OUT   =   0;                  //如果 PTD14输入为1，即PTD15输出为1 ，那就设置PTD15输出为0
        }
        else
        {
            PTD15_OUT   =   1;                  //如果 PTD14输入为0，即PTD15输出为0 ，那就设置PTD15输出为1
        }
        time_delay_ms(500);                   //延时500ms
    }
}


#elif   0

/*************************************************************************
*                             野火嵌入式开发工作室
*                               GPIO实验简单测试
*  实验说明：野火GPIO实验，利用LED来显示电平高低
*
*  实验操作：无
*
*  实验效果：LED0每隔500ms
*
*  实验目的：明白如何设置IO口电平
*
*  修改时间：2012-2-28     已测试
*
*  备    注：野火Kinetis开发板的 LED0~3 ，分别接PTD15~PTD12 ，低电平点亮
*************************************************************************/
void  main(void)
{
    gpio_init  (PORTD, 15, GPO, HIGH);      //初始化 PTD15 ：输出高电平 ,即 初始化 LED0，灭

    while(1)
    {

        gpio_set(PORTD, 15, LOW);          //设置 PTD15 输出 低电平 , 即 LED0 亮

        time_delay_ms(500);                //延时500ms

        gpio_set(PORTD, 15, HIGH);         //设置 PTD15 输出 高电平 , 即 LED0 灭

        time_delay_ms(500);                //延时500ms
    }
}

#elif   0

/*************************************************************************
*                             野火嵌入式开发工作室
*                             GPIO实验并行读写测试
*  实验说明：野火GPIO实验，利用LED来显示电平高低
*
*  实验操作：PTD15和PTD13短接，PTD14和PTD12短接
*
*  实验效果：LED0、LED2和LED1、LED3 轮流间隔 500ms 闪烁
*
*  实验目的：明白如何并行读写数据
*
*  修改时间：2012-3-6     已测试
*
*  备    注：野火Kinetis开发板的 LED0~3 ，分别接PTD15~PTD12 ，低电平点亮
*************************************************************************/
void  main(void)
{
    u32  value;                                 //保存读取 PTD15~PTD12 的值

    gpio_init  (PORTD, 15, GPO, HIGH);           //初始化 PTD15 ：输出高电平 ,即 初始化 LED0，灭
    gpio_init  (PORTD, 14, GPO, LOW);            //初始化 PTD14 ：输出高电平 ,即 初始化 LED1，灭
    gpio_init  (PORTD, 13, GPI, HIGH);           //初始化 PTD13 ：输入
    gpio_init  (PORTD, 12, GPI, HIGH);           //初始化 PTD12 ：输入

    while(1)
    {
        value = GPIO_GET_2bit(PORTD, 12);       //读取 PTD13~PTD12的值

        if(value & (~0x0f))while(1);            //如果高位非0，则死循环。证明GPIO_GET_4bit返回的是4bit的数据
        else
        {
            GPIO_SET_2bit(PORTD, 14, (~value) & 0x03 ); //把读取回来的那两位值取反，清掉高位
        }
        time_delay_ms(500);                      //延时500ms
    }
}


#else

/*************************************************************************
*                             野火嵌入式开发工作室
*                              GPIO实验综合测试
*  实验说明：野火GPIO实验，利用LED来显示电平高低
*
*  实验操作：无
*
*  实验效果：LED0和LED1同时亮灭，因为LED1的亮灭都是读取LED0来控制的。
*            每次LED0亮，LED2都会翻转亮灭。
*
*            可以看到 LED 的亮灭顺序为：
*             LED0   LED1   LED2
*              √     √     √
*              ×     ×     √
*              √     √     ×
*              ×     ×     ×
*              ...                     循环下去
*
*  实验目的：测试 gpio_init、gpio_get、gpio_turn 函数
*
*  修改时间：2012-2-28     已测试
*
*  备    注：野火Kinetis开发板的 LED0~3 ，分别接PTD15~PTD12 ，低电平点亮
*************************************************************************/
void  main(void)
{
    u8  level;        //用来保存读取PTD15的电平
    u8  n;            //用来循环计数

    gpio_init  (PORTD, 15, GPO, HIGH);      //初始化 PTD15 ：输出高电平 ,即 初始化 LED0，灭
    gpio_init  (PORTD, 14, GPO, HIGH);      //初始化 PTD14 ：输出高电平 ,即 初始化 LED1，灭
    gpio_init  (PORTD, 13, GPO, HIGH);      //初始化 PTD13 ：输出高电平 ,即 初始化 LED2，灭

    while(1)
    {
        for(n = LOW; n <= HIGH; n++)
        {
            gpio_set(PORTD, 15, n);             //设置 PTD15 输出 电平 n ，即 n 为控制 LED0 亮灭变量

            level = gpio_get(PORTD, 15);        //读取 PTD15 的输出电平

            gpio_set(PORTD, 14, level);         //设置 PTD14 的电平 设为  读取到 PTD15 的电平

            if(level == LOW)
            {
                gpio_turn(PORTD, 13);            //每次读取到 PTD15 为输出低电平，即LED0亮，就翻转 PTD13电平，即亮灭 LED2
            }

            time_delay_ms(1000);                //延时1s
        }
    }
}


#endif




