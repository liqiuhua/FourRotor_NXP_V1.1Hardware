#ifndef __ATT_H_
#define __ATT_H_

#include "include.h"
extern float Pitch, Roll, Yaw;
extern T_FLOAT_ANGEL Q_ANGLE;	


#define RtA 		57.324841				
#define AtR    	0.0174533				
#define Acc_G 	0.0011963				
#define Gyro_G 	0.0610351				
#define Gyro_Gr	0.0010653	

void Prepare_Data(AccelSensor *_acc ,AccelSensor *ACC_KF);
uint8 AHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz) ;
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az);
#endif
