/******************** (C) COPYRIGHT 2011 Ұ��Ƕ��ʽ���������� ********************
 * �ļ���       ��lptmr.c
 * ����         ��lptmr�����Ķ�ʱ����Ŀǰֻ��һ����ʱ����ʱ����
 * ��ע         ���ٷ�����
 *
 * ʵ��ƽ̨     ��Ұ��kinetis������
 * ��汾       ��
 * Ƕ��ϵͳ     ��
 *
 * ����         ����˼������˾
 * �Ա���       ��http://firestm32.taobao.com
 * ����֧����̳ ��http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/

#include "common.h"
#include "lptmr.h"
#include "isr.h"
#define LPTMR_irq     85u

//============================================================================
//�������ƣ�lptmr_counter_init
//�������أ���
//����˵����pin_select:ѡ������ţ���1,2��
//���ܸ�Ҫ�������ۼӼ����ж�
//
//============================================================================
void lptmr_counter_init(LPT0_ALTn altn,u16 count,u8 PrescaleValue,LPT_CFG cfg)
{

	if(PrescaleValue > 0x0f)PrescaleValue=0x0f;

	//��������ܽ�
	if(altn==LPT0_ALT1)
	{
		SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;  //�� PORTA ʱ��
		PORTA_PCR19=PORT_PCR_MUX(0x6);      //��PTA19��ʹ�� ALT6
	}
	else if(altn==LPT0_ALT2)
	{
		SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;  //ʹ�� PORTC ʱ��
		PORTC_PCR5=PORT_PCR_MUX(0x4);       //��PTC5��ʹ�� ALT4
	}
	else                                    //�����ܷ����¼�
	{
		assert_failed(__FILE__, __LINE__);  //���ùܽ�����
	}

    /* ����ģ��ʱ�� */
    SIM_SCGC5|=SIM_SCGC5_LPTIMER_MASK;      //ʹ��LPTģ��ʱ��

    /* ��״̬�Ĵ��� */
    LPTMR0_CSR=0x00;                        //�ȹ���LPT��������������ʱ�ӷ�Ƶ��

    /* �����ۼӼ���ֵ  */
    LPTMR_CMR_REG(LPTMR0_BASE_PTR)  =   LPTMR_CMR_COMPARE(count);                       //���ñȽ�ֵ

    /* ʱ��ѡ�� */
	LPTMR_PSR_REG(LPTMR0_BASE_PTR)  =   LPTMR_PSR_PCS(0x1) | LPTMR_PSR_PBYP_MASK  |  LPTMR_PSR_PRESCALE(PrescaleValue);        //ʹ�� LPO clock �� bypass glitch filter
	//                                                        ���������������˲�����2^n��ʱ�������ز�ʶ��

    /* �ܽ����á�ʹ���ж�  */
	LPTMR_CSR_REG(LPTMR0_BASE_PTR)  =   LPTMR_CSR_TPS(altn)| LPTMR_CSR_TMS_MASK  | ( cfg ==LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  | LPTMR_CSR_TEN_MASK  | LPTMR_CSR_TIE_MASK  ;
    //                                       ѡ������ܽ�        ѡ���������                                   �½���       ������           ʹ��LPT
    // TFC = 0��������ֵ���ڱȽ�ֵʱ������ֵ��λ

	enable_irq(LPTMR_irq);	                //�����ŵ�IRQ�ж�	
}

/*
void lptmr_counter_init(LPT0_ALTn altn, u16 count, u8 PrescaleValue, LPT_CFG cfg)
{

    if(PrescaleValue > 0x0f)PrescaleValue = 0x0f;

    //��������ܽ�
    if(altn == LPT0_ALT1)
    {
        SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;  //�� PORTA ʱ��
        PORTA_PCR19 = PORT_PCR_MUX(0x6);    //��PTA19��ʹ�� ALT6
    }
    else if(altn == LPT0_ALT2)
    {
        SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;  //ʹ�� PORTC ʱ��
        PORTC_PCR5 = PORT_PCR_MUX(0x4);     //��PTC5��ʹ�� ALT4
    }
    else                                    //�����ܷ����¼�
    {
        assert_failed(__FILE__, __LINE__);  //���ùܽ�����
    }

    // ����ģ��ʱ�� /
    SIM_SCGC5 |= SIM_SCGC5_LPTIMER_MASK;    //ʹ��LPTģ��ʱ��

    // ��״̬�Ĵ��� 
    LPTMR0_CSR = 0x00;                      //�ȹ���LPT��������������ʱ�ӷ�Ƶ��

    // �����ۼӼ���ֵ  
   // LPTMR_CMR_REG(LPTMR0_BASE_PTR)  =   LPTMR_CMR_COMPARE(count);                       //���ñȽ�ֵ

    // ʱ��ѡ�� 
  //  LPTMR_PSR_REG(LPTMR0_BASE_PTR)  =   LPTMR_PSR_PCS(0x1) | LPTMR_PSR_PBYP_MASK  |  LPTMR_PSR_PRESCALE(PrescaleValue);        //ʹ�� LPO clock �� bypass glitch filter
    //                                                        ���������������˲�����2^n��ʱ�������ز�ʶ��

    // �ܽ����á�ʹ���ж� 
  //  LPTMR_CSR_REG(LPTMR0_BASE_PTR)  =   LPTMR_CSR_TPS(altn) | LPTMR_CSR_TMS_MASK  | ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  | LPTMR_CSR_TEN_MASK  | LPTMR_CSR_TIE_MASK  ;
    //                                       ѡ������ܽ�        ѡ���������                                   �½���       ������           ʹ��LPT
    // TFC = 0��������ֵ���ڱȽ�ֵʱ������ֵ��λ
//�����õ��ڲ�ʱ���޹�
  //�ⲿ������ź���Ϊ����ʱ��
  LPTMR0_PSR =LPTMR_PSR_PCS(0x1)|LPTMR_PSR_PBYP_MASK; 
  LPTMR0_CSR =LPTMR_CSR_TPS(1);
  LPTMR0_CSR|=LPTMR_CSR_TMS_MASK; //����ģʽ
  LPTMR0_CSR|=LPTMR_CSR_TFC_MASK; //���ɼ���ģʽ

 // LPTMR_ISR=void  LPT_Handler(void);
  enable_irq(85);
  
  LPTMR0_CSR|=LPTMR_CSR_TIE_MASK;
  LPTMR0_CSR|=LPTMR_CSR_TEN_MASK; //����LPTMR
  
   // enable_irq(LPTMR_irq);	                //�����ŵ�IRQ�ж�
   // enable_irq(85);
}*/
//LPTMR_ISR_CALLBACK LPTMR_ISR;
void LPLD_LPTMR_Init()
{
  SIM_SCGC5|=SIM_SCGC5_LPTIMER_MASK;//��ʼ��LPTMRʱ��
   
  LPTMR0_CSR=0x00;  //��ռĴ���
  LPTMR0_PSR=0x00;
  LPTMR0_CMR=0x00;
  
  SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK; 
  PORTA_PCR19=PORT_PCR_MUX(0x6);      //����PTA19Ϊ�����ۼ�
 
//  SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;  //����PTC5Ϊ�����ۼ�
//  PORTC_PCR5=PORT_PCR_MUX(0x4);      

  //�����õ��ڲ�ʱ���޹�
  //�ⲿ������ź���Ϊ����ʱ��
  LPTMR0_PSR =LPTMR_PSR_PCS(0x1)|LPTMR_PSR_PBYP_MASK; 
  LPTMR0_CSR =LPTMR_CSR_TPS(1);
  LPTMR0_CSR|=LPTMR_CSR_TMS_MASK; //����ģʽ
  LPTMR0_CSR|=LPTMR_CSR_TFC_MASK; //���ɼ���ģʽ

 // LPTMR_ISR=void  LPT_Handler(void);
  enable_irq(85);
  
  LPTMR0_CSR|=LPTMR_CSR_TIE_MASK;
  LPTMR0_CSR|=LPTMR_CSR_TEN_MASK; //����LPTMR
}

/*void lptmr_pulse_counter(char pin_select) 
{ 
  unsigned int compare_value=0xffff;  //�ڱ���������ģʽ��ֻ�趨ʱ
//��ȡ����ֵ�Ϳ��ԣ�����Ҫ�Ƚ� 
//  char input; 
 
//  lptmr_clear_registers();          //��λLPTMRģ�� 
  SIM_SCGC5|=SIM_SCGC5_LPTIMER_MASK;//ʹ��LPTģ��ʱ�� 
 
  //Set up GPIO 
  if(pin_select==1) 
  { 
    SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK; //�� PORTA ʱ�� 
    PORTA_PCR19=PORT_PCR_MUX(0x6);     //��PTA19��ʹ�� ALT6 
  } 
  else if(pin_select==2) 
  { 
    SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK; //�� PORTC ʱ��  
    PORTC_PCR5=PORT_PCR_MUX(0x4);      //��PTC5��ʹ�� ALT4 
  } 
  else 
  { 
    return; 
      } 
 
  LPTMR0_PSR=LPTMR_PSR_PCS(0x1)|LPTMR_PSR_PBYP_MASK; //ʹ�� LPO 
//clock �� bypass glitch filter 
  LPTMR0_CMR=LPTMR_CMR_COMPARE(compare_value);        //���ñȽ�ֵ 
  LPTMR0_CSR=LPTMR_CSR_TPS(pin_select)|LPTMR_CSR_TMS_MASK; //����LPT
//ʹ��ѡ�������,�ҽ��������ۼ�ģʽ,�����ز�׽ 
   
  LPTMR0_CSR|=LPTMR_CSR_TEN_MASK; //���� LPTģ�� 
}  */
/*
//��ռ���
void lptmr_counter_clean(void)
{
    LPTMR_CSR_REG(LPTMR0_BASE_PTR)  &= ~LPTMR_CSR_TEN_MASK;     //����LPT��ʱ��ͻ��Զ����������ֵ
    LPTMR_CSR_REG(LPTMR0_BASE_PTR)  |=LPTMR_CSR_TEN_MASK;
}*/



/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�time_delay_ms
*  ����˵������ʱ������ʹ�ö����Ķ�ʱ����ʱ��׼ȷ
*  ����˵����ms   ��ʱʱ�䣬��λΪms
*  �������أ���
*  �޸�ʱ�䣺2012-1-20
*  ��    ע���ٷ������Դ�
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
//��������: lptmr_internal_ref_input                                                       
//��������: ��                                          
//����˵��: �� 
//ע    ��: �ڲ��ο�ʱ��4��Ƶ��Ϊģ��ʱ�ӣ�ʹ��ptc5�����ţ�alt2ͨ��        
//���ܸ�Ҫ: ���ϴμĴ�����ֵ���ڱ�������      
//==========================================================================
void lptmr_internal_ref_input()
{
  unsigned int compare_value=15625; //4 seconds with prescale=8 and 2Mhz fast clock

  //Reset LPTMR module
  lptmr_clear_registers();
  // Enable LPT Module 
  SIM_SCGC5|=SIM_SCGC5_LPTIMER_MASK;
    
  //��c��5�����ŵ���Ӧ����
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
  LPTMR0_CSR=LPTMR_CSR_TPS(0x2)|LPTMR_CSR_TMS_MASK;    //use 2ģ��(LPT0_ALT2)(c��5����) ����ģʽ  

  LPTMR0_CSR|=LPTMR_CSR_TEN_MASK; //��ʼ���� //Turn on LPT with default settings

}


//==========================================================================
//��������: lptmr_pulse_counter                                                        
//��������: ��                                          
//����˵��: ����ֵΪҪʹ�õ����ź� 
//ע    ��: ʹ��1khz��LPOʱ�ӽ��м�����ʹ�������״̬           
//���ܸ�Ҫ: ���ϴμĴ�����ֵ���ڱ�������      
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
//�������� get_counter_value
//����  �� ���������ļ���ֵ
//ע��  �� ��ѹ���ȣ�Ƶ�ʹ��Ϳ��ܶ�����
//����  �� ��
//���  �� �������ļ���ֵ

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
//��������: lptmr_clear_registers                                                        
//��������: ��                                          
//����˵��:             
//���ܸ�Ҫ: ���ϴμĴ�����ֵ���ڱ�������      
//==========================================================================
void lptmr_clear_registers()
{
  LPTMR0_CSR=0x00;
  LPTMR0_PSR=0x00;
  LPTMR0_CMR=0x00;
}

