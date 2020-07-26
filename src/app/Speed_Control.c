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
    FTM_PWM_init(FTM0 , CH1, 80000,0);                          //���ռ�ձ����ó�ʼ��    ռ�ձ� = duty /(MOD +1 ) ;FTM_CnV_REG(FTMx[ftmn], ch) = cv;
    FTM_PWM_init(FTM0 , CH2, 80000,0);        
}


s32 MotorSpeedOut(s32 speedPWM)//�ںϺ��͵����
{ 
  RinghtMotorOut =  speedPWM ;//��������ת�����Ը�һ�������ţ�

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

void GetMotorPulse() //�������룬���Զ�������
{      
   MotorPulse =FTM2_CNT  ; //��ֱ�Ӷ�ȡ����ֵ����������ҪΪs16���� 
   FTM2_CNT = 0; 
  
   if(MotorPulse>0)//������������ǰһ�£�����ͻ���һ��һ�������	
     MotorPulse = -MotorPulse; 
    
   MotorPulseSigma+=MotorPulse;
}  

#define SPEEDPERIODFAV  100 //�ٶȿ�������
s32 SpeedPWMOut(s32 NewspeedPWM ,s32 LastspeedPWM,s32 PeriodCount)
{
    s32  speedPWMfav,SpeedOUT ;
    speedPWMfav = NewspeedPWM - LastspeedPWM ;
    SpeedOUT = speedPWMfav *(PeriodCount+1)/SPEEDPERIODFAV + LastspeedPWM ;   
    return SpeedOUT ; 
}
s32  DSpeed ,LastSpeedCut0, g_SpeedControlIntegral=0,SpeedDifference0=0;      
float SpeedPWMKP ,SpeedPWMKI ;
void SpeedPID()//���� ǰ��  ���� ���  ���� 
{    
   LastSpeedCut0 = MotorPulseSigma ; //�������ֵ��ٶ�ȡƽ��
   MotorPulseSigma=0;//Ȼ������
    DSpeed =g_Speedgoal;//�ڲ��ٶ�
    SpeedDifference0 =LastSpeedCut0-DSpeed  ;//��ʵ���ٶ�����
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