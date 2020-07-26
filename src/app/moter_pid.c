#include "include.h"

uint16 count;

uint32 motor_pid(uint8 kp,uint8 ki,uint8 kd,uint32 target_speed){

  static int32_t lastoutput=0;
  static int32_t error_v2=0;
  static int32_t error_v1=0;
  static int32_t error_v0=0;
  
  error_v2=error_v1;
  error_v1=error_v0;
  error_v0=target_speed-count;
  
  lastoutput=lastoutput+(kp*error_v0-ki*error_v1+kd*error_v2);
  
  if(lastoutput>28000) 
    lastoutput=28000;
    
  return lastoutput;  
}