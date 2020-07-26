/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：lptmr.c
 * 描述         ：lptmr定功耗定时器，目前只有一个定时器延时函数
 * 备注         ：官方例程
 *
 * 实验平台     ：野火kinetis开发板
 * 库版本       ：
 * 嵌入系统     ：
 *
 * 作者         ：飞思卡尔公司
 * 淘宝店       ：http://firestm32.taobao.com
 * 技术支持论坛 ：http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/

#include "common.h"
#include "lptmr.h"
#include "isr.h"
#define LPTMR_irq     85u

//============================================================================
//函数名称：lptmr_counter_init
//函数返回：无
//参数说明：pin_select:选择的引脚（仅1,2）
//功能概要：脉冲累加计数中断
//
//============================================================================
void lptmr_counter_init(LPT0_ALTn altn,u16 count,u8 PrescaleValue,LPT_CFG cfg)
{

	if(PrescaleValue > 0x0f)PrescaleValue=0x0f;

	//设置输入管脚
	if(altn==LPT0_ALT1)
	{
		SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;  //打开 PORTA 时钟
		PORTA_PCR19=PORT_PCR_MUX(0x6);      //在PTA19上使用 ALT6
	}
	else if(altn==LPT0_ALT2)
	{
		SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;  //使能 PORTC 时钟
		PORTC_PCR5=PORT_PCR_MUX(0x4);       //在PTC5上使用 ALT4
	}
	else                                    //不可能发生事件
	{
		assert_failed(__FILE__, __LINE__);  //设置管脚有误？
	}

    /* 开启模块时钟 */
    SIM_SCGC5|=SIM_SCGC5_LPTIMER_MASK;      //使能LPT模块时钟

    /* 清状态寄存器 */
    LPTMR0_CSR=0x00;                        //先关了LPT，这样才能设置时钟分频等

    /* 设置累加计数值  */
    LPTMR_CMR_REG(LPTMR0_BASE_PTR)  =   LPTMR_CMR_COMPARE(count);                       //设置比较值

    /* 时钟选择 */
	LPTMR_PSR_REG(LPTMR0_BASE_PTR)  =   LPTMR_PSR_PCS(0x1) | LPTMR_PSR_PBYP_MASK  |  LPTMR_PSR_PRESCALE(PrescaleValue);        //使用 LPO clock 且 bypass glitch filter
	//                                                        开启和配置脉冲滤波器：2^n个时钟上升沿才识别

    /* 管脚设置、使能中断  */
	LPTMR_CSR_REG(LPTMR0_BASE_PTR)  =   LPTMR_CSR_TPS(altn)| LPTMR_CSR_TMS_MASK  | ( cfg ==LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  | LPTMR_CSR_TEN_MASK  | LPTMR_CSR_TIE_MASK  ;
    //                                       选择输入管脚        选择脉冲计数                                   下降沿       上升沿           使能LPT
    // TFC = 0，即计数值等于比较值时，计数值复位

	enable_irq(LPTMR_irq);	                //开引脚的IRQ中断	
}

/*
void lptmr_counter_init(LPT0_ALTn altn, u16 count, u8 PrescaleValue, LPT_CFG cfg)
{

    if(PrescaleValue > 0x0f)PrescaleValue = 0x0f;

    //设置输入管脚
    if(altn == LPT0_ALT1)
    {
        SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;  //打开 PORTA 时钟
        PORTA_PCR19 = PORT_PCR_MUX(0x6);    //在PTA19上使用 ALT6
    }
    else if(altn == LPT0_ALT2)
    {
        SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;  //使能 PORTC 时钟
        PORTC_PCR5 = PORT_PCR_MUX(0x4);     //在PTC5上使用 ALT4
    }
    else                                    //不可能发生事件
    {
        assert_failed(__FILE__, __LINE__);  //设置管脚有误？
    }

    // 开启模块时钟 /
    SIM_SCGC5 |= SIM_SCGC5_LPTIMER_MASK;    //使能LPT模块时钟

    // 清状态寄存器 
    LPTMR0_CSR = 0x00;                      //先关了LPT，这样才能设置时钟分频等

    // 设置累加计数值  
   // LPTMR_CMR_REG(LPTMR0_BASE_PTR)  =   LPTMR_CMR_COMPARE(count);                       //设置比较值

    // 时钟选择 
  //  LPTMR_PSR_REG(LPTMR0_BASE_PTR)  =   LPTMR_PSR_PCS(0x1) | LPTMR_PSR_PBYP_MASK  |  LPTMR_PSR_PRESCALE(PrescaleValue);        //使用 LPO clock 且 bypass glitch filter
    //                                                        开启和配置脉冲滤波器：2^n个时钟上升沿才识别

    // 管脚设置、使能中断 
  //  LPTMR_CSR_REG(LPTMR0_BASE_PTR)  =   LPTMR_CSR_TPS(altn) | LPTMR_CSR_TMS_MASK  | ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  | LPTMR_CSR_TEN_MASK  | LPTMR_CSR_TIE_MASK  ;
    //                                       选择输入管脚        选择脉冲计数                                   下降沿       上升沿           使能LPT
    // TFC = 0，即计数值等于比较值时，计数值复位
//与所用的内部时钟无关
  //外部输入的信号作为计数时钟
  LPTMR0_PSR =LPTMR_PSR_PCS(0x1)|LPTMR_PSR_PBYP_MASK; 
  LPTMR0_CSR =LPTMR_CSR_TPS(1);
  LPTMR0_CSR|=LPTMR_CSR_TMS_MASK; //捕获模式
  LPTMR0_CSR|=LPTMR_CSR_TFC_MASK; //自由计数模式

 // LPTMR_ISR=void  LPT_Handler(void);
  enable_irq(85);
  
  LPTMR0_CSR|=LPTMR_CSR_TIE_MASK;
  LPTMR0_CSR|=LPTMR_CSR_TEN_MASK; //开启LPTMR
  
   // enable_irq(LPTMR_irq);	                //开引脚的IRQ中断
   // enable_irq(85);
}*/
//LPTMR_ISR_CALLBACK LPTMR_ISR;
void LPLD_LPTMR_Init()
{
  SIM_SCGC5|=SIM_SCGC5_LPTIMER_MASK;//初始化LPTMR时钟
   
  LPTMR0_CSR=0x00;  //清空寄存器
  LPTMR0_PSR=0x00;
  LPTMR0_CMR=0x00;
  
  SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK; 
  PORTA_PCR19=PORT_PCR_MUX(0x6);      //设置PTA19为脉冲累加
 
//  SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;  //设置PTC5为脉冲累加
//  PORTC_PCR5=PORT_PCR_MUX(0x4);      

  //与所用的内部时钟无关
  //外部输入的信号作为计数时钟
  LPTMR0_PSR =LPTMR_PSR_PCS(0x1)|LPTMR_PSR_PBYP_MASK; 
  LPTMR0_CSR =LPTMR_CSR_TPS(1);
  LPTMR0_CSR|=LPTMR_CSR_TMS_MASK; //捕获模式
  LPTMR0_CSR|=LPTMR_CSR_TFC_MASK; //自由计数模式

 // LPTMR_ISR=void  LPT_Handler(void);
  enable_irq(85);
  
  LPTMR0_CSR|=LPTMR_CSR_TIE_MASK;
  LPTMR0_CSR|=LPTMR_CSR_TEN_MASK; //开启LPTMR
}

/*void lptmr_pulse_counter(char pin_select) 
{ 
  unsigned int compare_value=0xffff;  //在编码器计数模式，只需定时
//读取计数值就可以，不需要比较 
//  char input; 
 
//  lptmr_clear_registers();          //复位LPTMR模块 
  SIM_SCGC5|=SIM_SCGC5_LPTIMER_MASK;//使能LPT模块时钟 
 
  //Set up GPIO 
  if(pin_select==1) 
  { 
    SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK; //打开 PORTA 时钟 
    PORTA_PCR19=PORT_PCR_MUX(0x6);     //在PTA19上使用 ALT6 
  } 
  else if(pin_select==2) 
  { 
    SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK; //打开 PORTC 时钟  
    PORTC_PCR5=PORT_PCR_MUX(0x4);      //在PTC5上使用 ALT4 
  } 
  else 
  { 
    return; 
      } 
 
  LPTMR0_PSR=LPTMR_PSR_PCS(0x1)|LPTMR_PSR_PBYP_MASK; //使用 LPO 
//clock 且 bypass glitch filter 
  LPTMR0_CMR=LPTMR_CMR_COMPARE(compare_value);        //设置比较值 
  LPTMR0_CSR=LPTMR_CSR_TPS(pin_select)|LPTMR_CSR_TMS_MASK; //设置LPT
//使用选择的引脚,且进入脉冲累加模式,上升沿捕捉 
   
  LPTMR0_CSR|=LPTMR_CSR_TEN_MASK; //开启 LPT模块 
}  */
/*
//清空计数
void lptmr_counter_clean(void)
{
    LPTMR_CSR_REG(LPTMR0_BASE_PTR)  &= ~LPTMR_CSR_TEN_MASK;     //禁用LPT的时候就会自动清计数器的值
    LPTMR_CSR_REG(LPTMR0_BASE_PTR)  |=LPTMR_CSR_TEN_MASK;
}*/



/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：time_delay_ms
*  功能说明：延时函数，使用定功耗定时器延时，准确
*  参数说明：ms   延时时间，单位为ms
*  函数返回：无
*  修改时间：2012-1-20
*  备    注：官方例程自带
*************************************************************************/
void time_delay_ms(uint32 ms)
{
    /* Make sure the clock to the LPTMR is enabled */
    SIM_SCGC5 |= SIM_SCGC5_LPTIMER_MASK;

    /* Set the compare value to the number of ms to delay */
    LPTMR0_CMR = ms;

    /* Set up LPTMR to use 1kHz LPO with no prescaler as its clock source */
    LPTMR0_PSR = LPTMR_PSR_PCS(1) | LPTMR_PSR_PBYP_MASK;

    /* Start the timer */
    LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;

    /* Wait for counter to reach compare value */
    while (!(LPTMR0_CSR & LPTMR_CSR_TCF_MASK));

    /* Clear Timer Compare Flag */
    LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK;

    return;
}


//#include "lptmr.h"

//==========================================================================
//函数名称: lptmr_internal_ref_input                                                       
//函数返回: 无                                          
//参数说明: 无 
//注    意: 内部参考时钟4分频作为模块时钟，使用ptc5号引脚，alt2通道        
//功能概要: 清上次寄存器的值便于本次设置      
//==========================================================================
void lptmr_internal_ref_input()
{
  unsigned int compare_value=15625; //4 seconds with prescale=8 and 2Mhz fast clock

  //Reset LPTMR module
  lptmr_clear_registers();
  // Enable LPT Module 
  SIM_SCGC5|=SIM_SCGC5_LPTIMER_MASK;
    
  //打开c口5号引脚的相应功能
  SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK; //Turn on PORTA clock
  PORTC_PCR5=PORT_PCR_MUX(0x4); //Use ALT2 on PTC 5
  
  //  Ensure Internal Reference Clock is Enabled 
  MCG_C1|=MCG_C1_IRCLKEN_MASK;  
  //Enable fast internal ref clock by setting MCG_C2[IRCS]=1
  //If wanted to use 32Khz slow mode, set MCG_C2[IRCS]=0 instead
  MCG_C2|=MCG_C2_IRCS_MASK;

  //Configure LPTMR 
  LPTMR0_CMR=LPTMR_CMR_COMPARE(compare_value);  //Set compare value
  LPTMR0_PSR=LPTMR_PSR_PCS(0x0)|LPTMR_PSR_PRESCALE(0x1);  //Use internal clock prescaled by8
  LPTMR0_CSR=LPTMR_CSR_TPS(0x2)|LPTMR_CSR_TMS_MASK;    //use 2模块(LPT0_ALT2)(c口5引脚) 计数模式  

  LPTMR0_CSR|=LPTMR_CSR_TEN_MASK; //开始计数 //Turn on LPT with default settings

}


//==========================================================================
//函数名称: lptmr_pulse_counter                                                        
//函数返回: 无                                          
//参数说明: 输入值为要使用的引脚号 
//注    意: 使用1khz的LPO时钟进行计数，使用与低速状态           
//功能概要: 清上次寄存器的值便于本次设置      
//==========================================================================
void lptmr_pulse_counter()
{
  unsigned int compare_value=1000;

  //Reset LPTMR module
  lptmr_clear_registers();

  //Set up GPIO
 // if(pin_select==LPT0_ALT1)
  {
    SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK; //Turn on PORTA clock
    PORTA_PCR19=PORT_PCR_MUX(0x6); //Use ALT6 on PTA19

    //printf("Testing ALT1 pin on PORTA19\n");
    //printf("\tTWR-K70F120M: ALT1 is conected to pin 18 on J15\n");
  }
 /* else if(pin_select==LPT0_ALT2)
  {
    SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK; //Turn on PORTC clock
    PORTC_PCR5=PORT_PCR_MUX(0x4); //Use ALT2 on PTC5

  }*/



  LPTMR0_PSR=LPTMR_PSR_PCS(0x1)|LPTMR_PSR_PBYP_MASK; //Use LPO clock but bypass glitch filter
  LPTMR0_CMR=LPTMR_CMR_COMPARE(compare_value);  //Set compare value
  LPTMR0_CSR=LPTMR_CSR_TPS(1)|LPTMR_CSR_TMS_MASK; //Set LPT to use the pin selected, and put in pulse count mode, on rising edge (default)

  //printf("Press any key to start pulse counter\n");
  //in_char(); //wait for keyboard press

  LPTMR0_CSR|=LPTMR_CSR_TEN_MASK; //Turn on LPT

  
}


//=================================================================================
//函数名： get_counter_value
//功能  ： 读编码器的计数值
//注意  ： 电压不稳，频率过低可能丢脉冲
//输入  ： 无
//输出  ： 编码器的计数值

// On each read of the LPTMR counter register, software must first write to the  *
//LPTMR counter register with any value. This will synchronize and register the  *
// current value of the LPTMR counter register into a temporary register. The    *
//contents of the temporary register are returned on each read of the LPTMR      *
// counter register.
//===============================================================================
uint16 get_counter_value()
{
  LPTMR0_CNR=0x1;
  return LPTMR0_CNR;
}

//==========================================================================
//函数名称: lptmr_clear_registers                                                        
//函数返回: 无                                          
//参数说明:             
//功能概要: 清上次寄存器的值便于本次设置      
//==========================================================================
void lptmr_clear_registers()
{
  LPTMR0_CSR=0x00;
  LPTMR0_PSR=0x00;
  LPTMR0_CMR=0x00;
}

