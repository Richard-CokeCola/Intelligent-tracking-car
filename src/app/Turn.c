#include"Turn.h"
#include "include.h"
#include"Speed_Control.h"
#include"isr.h"
extern int a1,a2,a3;
void  douji_init()    //A9
{    
     // FTM_PWM_init(FTM1, CH0 , 180,30);                       
      FTM_PWM_init(FTM1, CH1 , 90,30);        //  PTA9        
      FTM_CnV_REG(FTMx[FTM1], CH1) = MIDSTRING ;          //舵机中值调整设置    MIDSTRING =2500
      
}
/*
s16 TurnPWM(s8 TurnPosition, s8 TurnMidPosition)
{
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  ///////////////////////////原来这一段被注释掉了///////////////////////////////////////////////////////////////
 s16 direction,direction0;
   // s16 TurnPWM ;
    static int TurnKP ,TurnKD ;
    //static s16 direction0 = TURNMIDPOSITION ;
    direction = TurnPosition - TurnMidPosition ;
    direction0=0;
    direction =TurnPosition;
   if((direction<=0.9)&&(direction >= -0.9))
   {
      TurnKP = 0 ;
      TurnKD = 0 ; 
     
   }
    
    else if((direction<=2)&&(direction >= -2))
    { 
      TurnKP = 10 ;
      TurnKD = 3 ;
 
    }
    else if((direction<=3)&&(direction >= -3))
    { 
      TurnKP = 20 ;
      TurnKD = 5;
 
    }
    else
    {
     TurnKP = 25 ;
     TurnKD = 6 ;
  
    }
    
    TurnPWM =  TurnKP * direction *10+ TurnKD * (direction - direction0);
   if(TurnPWM > 420)
    {
      TurnPWM = 420 ;
    }
    else if (TurnPWM < -420 ) 
    {
      TurnPWM = -420 ;
    }
    
   
    direction0 = direction ;

    TurnPWM = MIDSTRING -TurnPWM;
    
    return TurnPWM ;
}
    */