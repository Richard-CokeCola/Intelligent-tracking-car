#include "common.h"
#include "include.h"
#include "Speed_Control.h"

#include "OLED.h"
s16 MotorPulse, MotorPulseSigma;
s32  RingtMotorPWM ;
s32  RinghtMotorOut ;
extern int a1,a2,a3;

void FTM_init(void)
{
    FTM_PWM_init(FTM0 , CH1, 80000,0);                          //电机占空比设置初始化    占空比 = duty /(MOD +1 ) ;FTM_CnV_REG(FTMx[ftmn], ch) = cv;
    FTM_PWM_init(FTM0 , CH2, 80000,0);        
}


s32 MotorSpeedOut(s32 speedPWM)//融合后送到电机
{ 
  RinghtMotorOut =  speedPWM ;//如果电机反转，可以改一下正负号，

  if(RinghtMotorOut > MOTOR_OUT_MAX)
    RinghtMotorOut = MOTOR_OUT_MAX ;
  else if(RinghtMotorOut < MOTOR_OUT_MIN)
    RinghtMotorOut = MOTOR_OUT_MIN ;
   
  if(RinghtMotorOut >= 0)
  {  
     RinghtMotorOut = RinghtMotorOut ;
     RinghtMotorOut += 0 ;  
     FTM_CnV_REG(FTMx[FTM0], CH0) = 0 ;
     FTM_CnV_REG(FTMx[FTM0], CH1) = RinghtMotorOut;
  }
  else 
  {
     RinghtMotorOut = 0-RinghtMotorOut ;
     RinghtMotorOut += 0;
     FTM_CnV_REG(FTMx[FTM0], CH0) = RinghtMotorOut ;
     FTM_CnV_REG(FTMx[FTM0], CH1) = 0;
  }  
   return (RinghtMotorOut);
}

void GetMotorPulse() //正交解码，可以读出正负
{      
   MotorPulse =FTM2_CNT  ; //可直接读取正负值，变量类型要为s16类型 
   FTM2_CNT = 0; 
  
   if(MotorPulse>0)//让两个轮子向前一致，否则就会有一正一负的情况	
     MotorPulse = -MotorPulse; 
    
   MotorPulseSigma+=MotorPulse;
}  

#define SPEEDPERIODFAV  100 //速度控制周期
s32 SpeedPWMOut(s32 NewspeedPWM ,s32 LastspeedPWM,s32 PeriodCount)
{
    s32  speedPWMfav,SpeedOUT ;
    speedPWMfav = NewspeedPWM - LastspeedPWM ;
    SpeedOUT = speedPWMfav *(PeriodCount+1)/SPEEDPERIODFAV + LastspeedPWM ;   
    return SpeedOUT ; 
}
s32  DSpeed ,LastSpeedCut0, g_SpeedControlIntegral=0,SpeedDifference0=0;      
float SpeedPWMKP ,SpeedPWMKI ;
void SpeedPID()//都正 前冲  都负 后冲  结论 
{    
   LastSpeedCut0 = MotorPulseSigma ; //将左右轮的速度取平均
   MotorPulseSigma=0;//然后清零
    DSpeed =g_Speedgoal;//内部速度
    SpeedDifference0 =LastSpeedCut0-DSpeed  ;//与实际速度做差
    SpeedPWMKP = SpeedKP*SpeedDifference0;
    SpeedPWMKI = SpeedKI*SpeedDifference0;
    g_SpeedControlIntegral+=SpeedPWMKI; 
    if(g_SpeedControlIntegral  >=Pre_SPEED_OUT_MAX_I)  
    {
      g_SpeedControlIntegral=Pre_SPEED_OUT_MAX_I; 
    } 
    if(g_SpeedControlIntegral <= Pre_SPEED_OUT_MIN_I) 
    { 
      g_SpeedControlIntegral=Pre_SPEED_OUT_MIN_I;
    } 
    g_SpeedControlOutOld=g_SpeedControlOutNew; 
    g_SpeedControlOutNew=SpeedPWMKP+g_SpeedControlIntegral; 
    if(SpeedPWMKP>KPPLUSMAX)//30000
    {
      SpeedPWMKP = KPPLUSMAX;
    }else if (SpeedPWMKP <KPNEGATIVEMAX)
    {
      SpeedPWMKP = KPNEGATIVEMAX;                       
    } 
    if(SpeedPWMKI > KIPLUSMAX)//30000
    {
      SpeedPWMKI = KIPLUSMAX;
    } else if(SpeedPWMKI < KINEGATIVEMAX)
    {
      SpeedPWMKI = KINEGATIVEMAX;
    } 
}