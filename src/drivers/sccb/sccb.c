/******************** (C) COPYRIGHT 2012-2013 岱默科技 DEMOK*********
 * 文件名         ： sccb.c
 * 描述           ： OV摄像头SCCB驱动函数
 *
 * 实验平台       ： 岱默科技DEMOK Kinetis开发板
 * 作者           ： 岱默科技DEMOK Kinetis开发小组

 * 淘宝店铺       ： http://shop60443799.taobao.com/
 * 技术交流邮箱   ： 1030923155@qq.com 
 * 技术交流QQ群   ： 103360642

 * 最后修订时间    ：2012-11-12
 * 修订内容        ：无
**********************************************************************************/
#include "sccb.h"
#include "gpio.h"



/*************************************************************************
*                             岱默科技DEMOK Kinetis开发小组
*
*  函数名称：sccb_init
*  功能说明：初始化SCCB  其中SCL接PTA10 SDA接PTA11
*  参数说明：无
*  函数返回：无
*
*************************************************************************/
//-------初始化PA10  PA11为GPIO--------//
void sccb_init(void)
{
  PORT_PCR_REG(PORTE_BASE_PTR, 10) = (0 | PORT_PCR_MUX(1) );
  PORT_PCR_REG(PORTE_BASE_PTR, 12) = (0 | PORT_PCR_MUX(1) );
 // gpio_init(PORTE,10,GPI,0);
 // gpio_init(PORTE,12,GPI,0);
}

/************************************************************************
*                             岱默科技DEMOK Kinetis开发小组
*
*  函数名称：sccb_wait
*  功能说明：SCCB延时，不应太小
*  参数说明：无
*  函数返回：无
*
*************************************************************************/
void sccb_wait(void)
{
  u8 i;
  
  for( i=0; i<200; i++)
  {
    asm ("nop");
  }
}
/************************************************************************
*                             岱默科技DEMOK Kinetis开发小组
*
*  函数名称：sccb_start
*  功能说明：SCCB启动位
*  参数说明：无
*  函数返回：无
*
*************************************************************************/
void sccb_start(void)
{
  SCL_OUT;
  SDA_OUT;
 
  SDA_HIGH;
  //sccb_wait();
  SCL_HIGH;
  sccb_wait();
  SDA_LOW;
  sccb_wait();
  SCL_LOW;
}

/************************************************************************
*                             岱默科技DEMOK Kinetis开发小组
*
*  函数名称：sccb_stop
*  功能说明：SCCB停止位
*  参数说明：无
*  函数返回：无
*
*************************************************************************/
void sccb_stop(void)
{
  SCL_OUT;
  SDA_OUT;
  
  SDA_LOW;
  sccb_wait();
  SCL_HIGH;
  sccb_wait();
  SDA_HIGH;
  sccb_wait();
}

/************************************************************************
*                             岱默科技DEMOK Kinetis开发小组
*
*  函数名称：sccb_sendByte
*  功能说明：在SCCB总线上发送一个字节
*  参数说明：data 要发送的字节内容
ack =0收到应答，ack=1未收到应答
*  函数返回：无
*
*************************************************************************/
u8 sccb_sendByte(u8 data)
{
  u8 i;
   u8 ack;
  SDA_OUT;
  for( i=0; i<8; i++)
  {
    if(data & 0x80)
      SDA_HIGH;
    else 
      SDA_LOW;
    data <<= 1;
    sccb_wait();
    SCL_HIGH;
    sccb_wait();
    SCL_LOW;
    sccb_wait();
  }
  SDA_HIGH;
  SDA_IN;
  sccb_wait();
  SCL_HIGH;
  sccb_wait();
  ack = SDA_DATA;
  SCL_LOW;
  sccb_wait();
  return ack;
}


/************************************************************************
*                             岱默科技DEMOK Kinetis开发小组
*
*  函数名称：sccb_regWrite
*  功能说明：通过SCCB总线向指定设备的指定地址发送指定内容
*  参数说明：device---设备号  读写有区别
*            address---写数据的寄存器
*            data---写的内容
*  函数返回：ack=1未收到应答(失败)    ack=0收到应答(成功)
*
*************************************************************************/
u16 xx;
void sccb_regWrite(u8 device,u8 address,u8 data)
{
  u8 i;
  u8 ack;
  for( i=0; i<20; i++)
  {
    sccb_start();
    ack = sccb_sendByte(device);
    xx=ack;
    if( ack == 1 )
    {
     // sccb_stop();
      continue;
    }
    
    ack = sccb_sendByte(address);
    if( ack == 1 )
    {
     // sccb_stop();
      continue;
    }
    
    ack = sccb_sendByte(data);
    if( ack == 1 )
    {
     // sccb_stop();
      continue;
    }
    
    sccb_stop();
    if( ack == 0 ) break;
  }
}