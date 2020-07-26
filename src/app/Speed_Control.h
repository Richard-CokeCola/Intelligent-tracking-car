#ifndef  calculation_H
#define  calculation_H  
void GetMotorPulse();

extern float g_SpeedControlOutNew,g_SpeedControlOutOld ;
extern s32  PWMout ;
extern float AmSpeed;//目标速度 用于外部
extern float g_Speedgoal;//用于内部
extern float  ANGLE_CONTROL_P;
extern float  ANGLE_CONTROL_D;
extern float  SpeedKP ;
extern float  SpeedKI ;
extern u8 TIME0flag_1ms;
#define MIDSTRING              2540
#define MOTOR_OUT_MIN         -9000
#define MOTOR_OUT_MAX          9000

#define  TURNTIMCOUNT          2       
 /**********************
 * PID
 *********************/
#define  KPPLUSMAX            (30000)
#define  KPNEGATIVEMAX        (-30000)
#define  KIPLUSMAX            (30000)
#define  KINEGATIVEMAX         (-30000)
#define  KDPLUSMAX             (30000)
#define  KDNEGATIVEMAX        (-30000)
#define  KWPLUSMAX            (30000)
#define  KWNEGATIVEMAX        (-30000)
#define  KOUPLUSMAX           (32000)
#define  KOUPLUSMIN           (-32000)
#define  Pre_SPEED_OUT_MAX_I   4000
#define  Pre_SPEED_OUT_MIN_I   -4000
/**********************
 * Start Stop
 *********************/
#define CAR_CONTROL_SET		g_nCarControlFlag = 1
#define CAR_CONTROL_CLEAR	g_nCarControlFlag = 0
#define IF_CAR_CONTROL		(g_nCarControlFlag)

#define SPEED_CONTROL_STOP	g_nSpeedControlFlag = 0
#define SPEED_CONTROL_START	g_nSpeedControlFlag = 1

#define DIRECTION_CONTROL_STOP	g_nDirectionControlFlag = 0
#define DIRECTION_CONTROL_START	g_nDirectionControlFlag = 1

#define CAR_FAILURE_ANGLE_MAX	25//1000
#define CAR_FAILURE_ANGLE_MIN	-25//-1000

#define CAR_STAND_ANGLE_MAX	260
#define CAR_STAND_ANGLE_MIN	-260
//------------------------------------------------------------------------------
#define ANGLE_CONTROL_STOP	g_nAngleControlFlag = 0
#define ANGLE_CONTROL_START	g_nAngleControlFlag = 1;
#define ANGLE_CONTROL_OUT_MAX	128000
#define ANGLE_CONTROL_OUT_MIN	-128000
 
#endif