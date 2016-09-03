/***********************************************************
 * 文件名       ：control_L.c
 * 说明         ：姿态控制文件，本文件使用的是目前开源留下的串级控制PID控制方法，使用角度外环和角速度内环。
 * 作者         ：landzo 蓝宙电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 * 论坛         ：http://www.landzo.cn
 * 库版本       ：V1.0
 * 时间         ：16.08.15
************************************************************/



#include "control_L.h"


PID Xinner;
PID Xshell;

PID Yinner;
PID Yshell;

PID Zinner;
PID Zshell;
uint16 Moto_duty[4];
#define Zero(x) (x<30&&x>-30)?0:x
#define Thousand(x) (x>1000)?1000:(x<0?0:x)
#define Speed_limit(x) ((x>200)?200:(x<(-200)?(-200):x))
//角速度环（内环）使用的是PD控制器

/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：control_inner
*  功能说明：角速度控制器，使用的是PD控制
*  参数说明：X Y Z分别是陀螺仪的测量值，T_inner是时间
*            
*  函数返回：无
*  修改时间：2016-08-15
*  备    注：T_inner是一个运行时间
*************************************************************************/
void control_inner(float x,float y,float z,float T_inner)
{
	static float X_inner_old=0;//上一次的角速度数据
	static float Y_inner_old=0;
	static float Z_inner_old=0;
	
	float Y_E=300 * (Yshell.Out/30.0f);//期望角度转换成角速度，可能不能这么转换，需要考量
//	float Y_E=300 * ((float)(Zero(RC_mid - rc_data.roll))/100.0f/30.0f);
	float X_E=300 * (Xshell.Out/30.0f);
//	float X_E=300 * ((float)(Zero(RC_mid - rc_data.roll))/100.0f/30.0f);	

	float Z_E=300 * ((float)(Zero(RC_mid - rc_data.yaw))/100.0f/30.0f);//201608301552从由角度控制的Z轴yaw输出控制更改为由遥控Yaw直接控制。

	if(X_E>300)//期望角速度限幅
		X_E=300;
	if(X_E<-300)
		X_E=-300;
	if(Y_E>300)
		Y_E=300;
	if(Y_E<-300)
		Y_E=-300;
	if(Z_E>300)
		Z_E=300;
	if(Z_E<-300)
		Z_E=-300;
	
	
	//pd控制器
  Xinner.Ex=X_E-(x/16.38f)/10.0F;
	
	if(Xinner.Ex<5&&Xinner.Ex>-5)
		Xinner.Eix =0;
	else
		Xinner.Eix += Xinner.Ex * T_inner;//积分
	if(Xinner.Eix>800)//期望角速度限幅
		Xinner.Eix=800;
	if(Xinner.Eix<-800)
		Xinner.Eix=-800;
  Xinner.Out = Xinner.Kp * Xinner.Ex + \
						   Xinner.Ki*Xinner.Eix - \
               Xinner.Kd * (x/16.38f-X_inner_old);
	X_inner_old=x/16.38f;
	
	
	
	Yinner.Ex=Y_E-(y/16.38f)/10.0F;
  Yinner.Out=Yinner.Kp *Yinner.Ex + \
             Yinner.Kd * (y/16.38f-Y_inner_old);
	Y_inner_old=y/16.38f;
	
	
	Zinner.Ex=Z_E-(z/16.38f)/10.0F;
  Zinner.Out=Zinner.Kp * Zinner.Ex - \
             Zinner.Kd * (z/16.38f-Z_inner_old);
	Z_inner_old=z/16.38f;  
}


//角度使用的是PID控制器
void control_shell(float y,float x,float z,float T_shell )
{
	static float X_shell_old=0;//上一次的角度
	static float Y_shell_old=0;
	static float Z_shell_old=0;
	

  Xshell.Ex=(float)(Zero(RC_mid - rc_data.roll))/100.0f-x;//期望角度	
  Xshell.Eix+=Xshell.Ex * T_shell;//积分
	if(Xshell.Eix>20)
		Xshell.Eix=20;
	if(Xshell.Eix<-20)
		Xshell.Eix=-20;
	//pid控制器
  Xshell.Out=Xshell.Kp * Xshell.Ex + \
            Xshell.Ki*Xshell.Eix + \
						Xshell.Kd	*(Xshell.Ex-X_shell_old)*(0.005f/T_shell);
  X_shell_old=Xshell.Ex;

	Yshell.Ex=(float)(Zero(RC_mid - rc_data.pitch))/100.0f-y;	
  Yshell.Eix+=Yshell.Ex * T_shell;
	if(Yshell.Eix>20)
		Yshell.Eix=20;
	if(Yshell.Eix<-20)
		Yshell.Eix=-20;
	
  Yshell.Out=Yshell.Kp * Yshell.Ex + \
             Yshell.Ki * Yshell.Eix + \
						 Yshell.Kd * (Yshell.Ex-Y_shell_old)*(0.005f/T_shell);
  Y_shell_old=Yshell.Ex;
	
	Zshell.Ex=(float)(Zero(RC_mid - rc_data.yaw))/100.0f-z;	//201608301555在其他模式中起作用但是在这暂时不使用。也就是yaw角的角度控制取消，利用Zshell.Kp ki kd 为零消除这个影响
  Zshell.Eix+=Zshell.Ex * T_shell;
	if(Zshell.Eix>20)
		Zshell.Eix=20;
	if(Zshell.Eix<-20)
		Zshell.Eix=-20;
	
  Zshell.Out=Zshell.Kp * Zshell.Ex + \
             Zshell.Ki * Zshell.Eix + \
						 Zshell.Kd	*(Zshell.Ex-Z_shell_old)*(0.005f/T_shell);
  Z_shell_old=Zshell.Ex;
  
}

#define PWM_Low(x) x<9000?9000:(x>16000?16000:x)
#define PWM_limit(x) (x>((rc_data.throttle-8255)*(0.5f)))?((rc_data.throttle-8255)*0.5f) \
											:((x<(-((rc_data.throttle-8255))*0.5f)?(-((rc_data.throttle-8255)*0.5f)):x))


void PWM_out(void)
{
	if((rc_data.throttle>8500)&&(ARMED==1))
	{
		if(rc_data.throttle>14000)
				rc_data.throttle=14000;
  	Moto_duty[0] =PWM_Low((rc_data.throttle-8255)*1.3f + (+ PWM_limit((Speed_limit(Xinner.Out) - Speed_limit(Yinner.Out) + Speed_limit(Zinner.Out) )*8.0f))+8000);
		Moto_duty[1] =PWM_Low((rc_data.throttle-8255)*1.3f + (+ PWM_limit((Speed_limit(Xinner.Out) + Speed_limit(Yinner.Out) - Speed_limit(Zinner.Out) )*8.0f))+8000);		
		Moto_duty[2] =PWM_Low((rc_data.throttle-8255)*1.3f + (- PWM_limit((Speed_limit(Xinner.Out) + Speed_limit(Yinner.Out) + Speed_limit(Zinner.Out) )*8.0f))+8000);		
		Moto_duty[3] =PWM_Low((rc_data.throttle-8255)*1.3f + (- PWM_limit((Speed_limit(Xinner.Out) - Speed_limit(Yinner.Out) - Speed_limit(Zinner.Out) )*8.0f))+8000);

		UVA_FTM(Moto_duty);
	}
	else 
	{
		Moto_duty[0]=8000;
		Moto_duty[1]=8000;
		Moto_duty[2]=8000;
		Moto_duty[3]=8000;
		UVA_FTM(Moto_duty);
	}
}

void control_L(T_FLOAT_ANGEL Q_ANGLE,typeGYRO FXA_gyro)
{
  static uint8 control_count=0;
  
	if(control_count>1)
	{
		control_count=0;
		control_shell(Q_ANGLE.PITCH,Q_ANGLE.ROL,Q_ANGLE.YAW,0.002);
	}
	control_inner(FXA_gyro.X,FXA_gyro.Y,FXA_gyro.Z,0.002);
	control_count++;
  PWM_out();
}
