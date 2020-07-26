/******************** (C) COPYRIGHT 2012-2013 岱默科技 DEMOK*********
 * 文件名         ： sccb.h
 * 描述           ： OV摄像头SCCB驱动头文件
 *
 * 实验平台       ： 岱默科技DEMOK Kinetis开发板
 * 作者           ： 岱默科技DEMOK Kinetis开发小组

 * 淘宝店铺       ： http://shop60443799.taobao.com/
 * 技术交流邮箱   ： 1030923155@qq.com 
 * 技术交流QQ群   ： 103360642

 * 最后修订时间    ：2012-11-12
 * 修订内容        ：无
**********************************************************************************/
#ifndef __SCCB_H__
#define __SCCB_H__

#include "common.h"
#include "gpio.h"

//--- 默认SCL接PA10   SDA接PA11------//
void sccb_init(void);                           //初始化SCCB端口为GPIO
void sccb_wait(void);                           //SCCB时序延时
void sccb_start(void);                          //起始标志
void sccb_stop(void);                           //停止标志
u8 sccb_sendByte(u8 data);
void sccb_regWrite(u8 device,u8 address,u8 data);

//--GPIO_DDR_1bit(PORTx,n,ddr)--//
#define SCL_OUT   GPIO_DDR_1bit(PORTE,10,1)      //SCL-PA0设置为输出端口
#define SDA_OUT   GPIO_DDR_1bit(PORTE,12,1)      //SDA-PA1设置为输出端口
#define SDA_IN    GPIO_DDR_1bit(PORTE,12,0)      //SDA-PA1设置为输入端口

//--GPIO_SET_1bit(PORTx,n,data)--//
#define SCL_HIGH  GPIO_SET_1bit(PORTE,10,1)
#define SCL_LOW   GPIO_SET_1bit(PORTE,10,0)
#define SDA_HIGH  GPIO_SET_1bit(PORTE,12,1)
#define SDA_LOW   GPIO_SET_1bit(PORTE,12,0)
#define SDA_DATA  GPIO_GET_1bit(PORTE,12) 



#endif