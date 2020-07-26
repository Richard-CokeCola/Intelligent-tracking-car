#include "common.h"
#include "include.h"
#include "Speed_Control.h"
#include "sensor.h"
#include "OLED.h"
int bee1,bee2,bee3,bee4;
extern int   TurnPWM_d;
int TurnPWM_d=0;
static int TurnPWM_dlast=0;
  
void Sensor_init(void)
{
  adc_init(ADC0, AD9) ;  //B0     
  adc_init(ADC0, AD8) ;  //B1 
  adc_init(ADC0, AD12) ;  //B2 
  adc_init(ADC0, AD13) ;  //B3 
  //adc_init(ADC1, AD6a) ;  //E2
  //adc_init(ADC1, AD5a) ;  //E1
  //adc_init(ADC1, AD10) ;  //B4
  //adc_init(ADC1, AD11) ;  //B5
}


int process()     //数据采集处理函数
{
      
        TurnPWM_dlast=TurnPWM_d;
        int c=3.5,d=2;
        int lastoutput=0;
     bee1=ad_ave(ADC0, AD8, ADC_10bit,5);
     bee2=ad_ave(ADC0, AD13, ADC_10bit,5);
     bee3=ad_ave(ADC0, AD9, ADC_10bit,5);
     bee4=ad_ave(ADC0, AD12, ADC_10bit,5);
  
     int max;
     if ((bee1>bee2)&&(bee1>bee3)&&(bee1>bee4))
  {
    max = bee1;
  }
  else if ((bee2>bee1)&&(bee2>bee3)&&(bee2>bee4))
  {
    max = bee2;
  }
  else if ((bee3>bee1)&&(bee3>bee2)&&(bee3>bee4))
  {
    max = bee3;
  }
  else if ((bee4>bee1)&&(bee4>bee2)&&(bee4>bee3))
  {
    max = bee4;
  }

  //拉回
  if (bee1+bee2+bee3+bee4<800)//丢线或进入圆环轨道
  {
    if (bee1+bee2+bee3+bee4>200)//60
    {
      if (max==bee1) //丢线
       {  
         //TurnPWM_d=TurnPWM_d-(TurnPWM_d - TurnPWM_dlast)*0.5;
         TurnPWM_d=-750 ; //-(TurnPWM_d - TurnPWM_dlast)*0.3  ;
         TurnPWM_dlast= TurnPWM_d ;
       }
      else if (max==bee2)//圆环
       {
         TurnPWM_d=-(bee1+300);
         if ((bee1-bee4)>-150&&(bee1-bee4)<150&&(bee1+bee4)>20&&bee3>10&&bee4>10)
         {
           TurnPWM_d=370 ;
           TurnPWM_dlast= TurnPWM_d;      
         }
         TurnPWM_dlast= TurnPWM_d;
       }
      else if (max==bee3)//圆环
       {   
         TurnPWM_d=bee4+100;
          if ((bee1-bee4)>-150&&(bee1-bee4)<150&&(bee1+bee4)>20&&bee3>10&&bee4>10)
         {
           TurnPWM_d=350 ;
           TurnPWM_dlast= TurnPWM_d;
                  
         }
         TurnPWM_dlast= TurnPWM_d;
       }
    
      else if (max==bee4)//丢线
       { 
         //TurnPWM_d=TurnPWM_d-(TurnPWM_d - TurnPWM_dlast)*0.5;
         TurnPWM_d=750;// -(TurnPWM_d - TurnPWM_dlast)*0.3 ;
         TurnPWM_dlast=TurnPWM_d;//-TurnPWM_dlast;
       }
    }
   
    else //bee1+bee2+bee3+bee4<200 保持原状
    {
      TurnPWM_d=TurnPWM_dlast;
    }
    lastoutput=TurnPWM_d;
  }

  //普通情况 无极变速
else //bee1+bee2+bee3+bee4>800
{
  if (max==bee1)
   {
     TurnPWM_d =((bee3 + bee4)/d-(bee1 + bee2)/d)*1.9 ;
     //TurnPWM_d=TurnPWM_d-(TurnPWM_d - TurnPWM_dlast)*0.6;
   }
   else if (max==bee2)
    {
      if(bee2>100)
      {
        TurnPWM_d = ((-(bee1-100)/1.4))*1.9;  
        //原来：TurnPWM_d = -(bee1-85)/2 ;  
      }   
    }
   else if (max==bee3)
    {
         if (bee3>100)       
            {
               TurnPWM_d =  ((bee4-100)/1.4)*1.9 ;
            }
     }
   else if (max==bee4)
            { 
               TurnPWM_d = ((bee3+bee4)/d - (bee1+bee2)/d)*1.9;
               //TurnPWM_d=TurnPWM_d -(TurnPWM_d-TurnPWM_dlast)*0.6;
            }
  lastoutput+=(TurnPWM_d-lastoutput)/3;
}
 
//TurnPWM_d=TurnPWM_d-(TurnPWM_d-TurnPWM_dlast)*0.05;
TurnPWM_dlast=TurnPWM_d;
return lastoutput;// + (TurnPWM_d-TurnPWM_dlast)0.05 ;
     
     
}    
     
     
     
 