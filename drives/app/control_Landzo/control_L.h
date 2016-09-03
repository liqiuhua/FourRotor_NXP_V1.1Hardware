#ifndef __CONTROL_L_H_
#define __CONTROL_L_H_

#include "common.h"
#include "include.h"

/*
*********************************************************************************************************
*                                   ��̬��غ궨��
*********************************************************************************************************
*/
typedef struct {
      float Kp;
      float Ki;
      float Kd;
      float Ex;
      float Eix;
      float Out;

}PID;

//#define RtA 	57.324841		//  180/3.1415  �Ƕ��� ת��Ϊ������		
//#define AtR    	0.0174533		//  1/RtA             RtA����		
//#define Acc_G 	0.0011963		//  1/32768/4/9.8     ���ٶ�����Ϊ4G		
//#define Gyro_G 	0.03051756	        //  1/32768/1000      ����������Ϊ +��1000			
//#define Gyro_Gr	0.0005327               //  1/32768/1000/57.3 

extern uint16 Moto_duty[4];
extern PID Xinner;
extern PID Xshell;
extern T_FLOAT_ANGEL IMU_Angle;

extern PID Yinner;
extern PID Yshell;

extern PID Zinner;
extern PID Zshell;


void control_inner(float x,float y,float z,float T_inner);
void control_shell(float x,float y,float z,float T_shell);
void control_L(T_FLOAT_ANGEL Q_ANGLE,typeGYRO FXA_gyro);

#endif
