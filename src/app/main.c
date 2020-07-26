#include "include.h"
#include "Speed_Control.h"
#include "sensor.h"
#include "OLED.h"
#define  LIN_COUT    65535
#define array_length 10


float SpeedKP =2;             //�ٶ�P  25
float SpeedKI =2;            //�ٶ�I   2
float TurnP=50;              //����P  94
float TurnD=3;              //����D   8.4
float AmSpeed=-00;
float g_Speedgoal=0;
int a1,a2,a3;
int  Station6;
int PWMC;
int pwm_count=0;
extern uint8 LPT_INT_count;
extern uint8 TIME0flag_2ms;
extern uint8 TIME0flag_5ms;                    //PTI0 5ms��־λ
extern uint8 TIME0flag_10ms;                    //PTI0 10ms��־λ
extern uint8 TIME0flag_20ms;                    //PTI0 20ms��־λ 
extern uint8 TIME0flag_80ms;                     //PTI0 80ms��־λ   
extern uint8 TIME0flag_100ms;                     //PTI0 100ms��־λ 
extern uint8 TIME0flag_500ms;                     //PTI0 100ms��־λ 
extern int bee1,bee2,bee3,bee4;
int turn[array_length];
u32 motor_output=0;

/*
int QueueOP(int array[],short type,int value){
  //0 for read ,1 for write
  static int read_pointer=0,write_pointer=array_length-1;
  int result;
  if(type == 0){
      //read
      result=array[read_pointer];
      read_pointer++;
      if(read_pointer==array_length)
      {  
        read_pointer=0;
      }
      return result;
    }
    else{
      //write
      array[write_pointer]=value;
      write_pointer++;
      if(write_pointer==array_length)
      {  
        write_pointer=0;
      }
    }
    
}
*/
void main()
{

    int ii=0;
    PWMC=200;
    DisableInterrupts;   //��ֹ���ж� 
    Sensor_init();       //�������˿ڳ�ʼ��
    FTM_init();         //�������PWM��ʼ��    C1  &  C2
    douji_init();       //����˿ڳ�ʼ��       A9
    OLED_Init();                                     //��ʼ��OLED������     
    pit_init_ms(PIT0, 0.5); 
    pit_init_ms(PIT1, 100);//��ʼ��PIT1����ʱʱ��Ϊ�� 100ms 
    gpio_init (PORTA , 17, GPO,HIGH); //�������е�
    gpio_init (PORTD , 15, GPI,0); 
    gpio_init (PORTD , 14, GPI,0); 
    gpio_init (PORTE , 0, GPI,0); 
    gpio_init (PORTE , 1, GPI,0); 
    lptmr_counter_init(LPT0_ALT2, LIN_COUT, 2, LPT_Rising);
    
    EnableInterrupts;    //�����ж� 
    
//    for(ii=0;ii<array_length;ii++){
//    turn[ii]=2100;
//    } 
    
   
    
    while(1)
  {
    
    
    if(TIME0flag_20ms == 1)
    { 
      TIME0flag_20ms = 0 ; 
      
//    QueueOP(turn,1,process() ); 
//    pwm_count=QueueOP(turn,0,0); 

      count = LPTMR0_CNR; 
      lptmr_counter_clean();
      count = LPT_INT_count * LIN_COUT +  count; 
      
      motor_output= motor_pid(10,3,2,35)/100;//9.3.7.35
      
     /* FTM_PWM_Duty(FTM0, CH1, (bee1+bee2+bee3+bee4)/8);
      if (FTM_PWM_Duty<125)
      {
        FTM_PWM_Duty = 125 ;
      }
      */
      
      if(bee1+bee2+bee3+bee4<700)
      {         
        if ((bee1-bee4)>-150&&(bee1-bee4)<150&&(bee1+bee4)>20&&bee3>10&&bee4>10)//bee2��Բ������
          {
             FTM_PWM_Duty(FTM0, CH1, 0);
          }
         else if ((bee1-bee4)>-150&&(bee1-bee4)<150&&(bee1+bee4)>20&&bee3>10&&bee4>10)//bee3��Բ������
          {
             FTM_PWM_Duty(FTM0, CH1, 0);
          }
       else
        FTM_PWM_Duty(FTM0, CH1, 105);
      }
      else
      {
        FTM_PWM_Duty(FTM0, CH1, 220);
      }
      
      //FTM_PWM_Duty(FTM0 , CH1,motor_output);
      
      pwm_count=process();
      //FTM_CnV_REG(FTMx[FTM1], CH1) =2540 ;//����
      FTM_CnV_REG(FTMx[FTM1], CH1) =2540+pwm_count; 
      
    }

     /*********************
    500ms����ִ�д����
    *********************/
    if(TIME0flag_500ms == 1)
    { 
     
    //motor_output=200;
      
      TIME0flag_500ms = 0 ;
    OLED_Fill(0x00);//���� 
      
      OLED_Print(0,0,"1:");  
      OLED_Display_Num(15,0,bee1); 
    
      OLED_Print(0,2,"2:");  
      OLED_Display_Num(15,2,bee2); 
    
      OLED_Print(0,4,"3:");  
      OLED_Display_Num(15,4,bee3); 
      
      OLED_Print(0,6,"4:");  
      OLED_Display_Num(15,6,bee4);
      
      
    }
  }
}

    



