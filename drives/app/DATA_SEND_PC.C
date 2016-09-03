#include "DATA_SEND_PC.H"
#include "landzo_fxos8700.h"
#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))
dt_flag_t f;					//需要发送数据的标志
uint8 data_to_send[50]={0};
extern typeGYRO GYRO_FXAS_Filter;
extern typeGYRO GYRO_FXAS;
extern AccelSensor foxaccl_Filter;
extern MagSensor magccc ;

void Data_Send_Status()
{
	uint8 _cnt=0;
	uint8 i;
	int16 _temp0,_temp1,_temp2;
		int _temp3 = (int)(100*100);
	uint8 sum = 0;
	_temp0 = (int)(IMU_Angle.PITCH*100);
	_temp1 = (int)(IMU_Angle.ROL*100); 
	_temp2 = (int)(IMU_Angle.YAW*100);

	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x01;
	data_to_send[_cnt++]=0;

//        _temp = (int)(30*100);
	data_to_send[_cnt++]=BYTE1(_temp1);
	data_to_send[_cnt++]=BYTE0(_temp1);
	
//        _temp = (int)(30*100);
	data_to_send[_cnt++]=BYTE1(_temp0);
	data_to_send[_cnt++]=BYTE0(_temp0);
	
//        _temp = (int)(30*100);
	data_to_send[_cnt++]=BYTE1(_temp2);
	data_to_send[_cnt++]=BYTE0(_temp2);
//	vs32 _temp2 = (int)(BaroAlt*100);
        
	data_to_send[_cnt++]=BYTE3(_temp3);
	data_to_send[_cnt++]=BYTE2(_temp3);
	data_to_send[_cnt++]=BYTE1(_temp3);
	data_to_send[_cnt++]=BYTE0(_temp3);
		
//	if(ARMED==0)				data_to_send[_cnt++]=0xA0;	//锁定
//	else if(ARMED==1)	
	data_to_send[_cnt++]=0x01;
	
	data_to_send[3] = _cnt-4;
	
	
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;
	Uart_DMA(DMA_CH0,(void *)data_to_send,(void *)&UART1_D,_cnt,DMA_UART1_Tx);

}
void Data_Send_Senser(void)
{
	uint8 _cnt=0;
	uint8 i;
	uint8 sum = 0;

	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x02;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=BYTE1(foxaccl_Filter.iGs[0]);
	data_to_send[_cnt++]=BYTE0(foxaccl_Filter.iGs[0]);
	data_to_send[_cnt++]=BYTE1(foxaccl_Filter.iGs[1]);
	data_to_send[_cnt++]=BYTE0(foxaccl_Filter.iGs[1]);
	data_to_send[_cnt++]=BYTE1(foxaccl_Filter.iGs[2]);
	data_to_send[_cnt++]=BYTE0(foxaccl_Filter.iGs[2]);
	data_to_send[_cnt++]=BYTE1(GYRO_FXAS_Filter.X);
	data_to_send[_cnt++]=BYTE0(GYRO_FXAS_Filter.X);
	data_to_send[_cnt++]=BYTE1(GYRO_FXAS_Filter.Y);
	data_to_send[_cnt++]=BYTE0(GYRO_FXAS_Filter.Y);
	data_to_send[_cnt++]=BYTE1(GYRO_FXAS_Filter.Z);
	data_to_send[_cnt++]=BYTE0(GYRO_FXAS_Filter.Z);
	data_to_send[_cnt++]=BYTE1(magsensorfif[0]);
	data_to_send[_cnt++]=BYTE0(magsensorfif[0]);
	data_to_send[_cnt++]=BYTE1(magsensorfif[1]);
	data_to_send[_cnt++]=BYTE0(magsensorfif[1]);
	data_to_send[_cnt++]=BYTE1(magsensorfif[2]);
	data_to_send[_cnt++]=BYTE0(magsensorfif[2]);
	
	data_to_send[3] = _cnt-4;
	

	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++] = sum;
	
	Uart_DMA(DMA_CH0,(void *)data_to_send,(void *)&UART1_D,_cnt,DMA_UART1_Tx);

}
void Data_Send_RCData(void)
{
	uint8 _cnt=0;
	uint8 i;
	uint8 sum = 0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x03;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=BYTE1(rc_data.throttle);
	data_to_send[_cnt++]=BYTE0(rc_data.throttle);
	data_to_send[_cnt++]=BYTE1(rc_data.yaw);
	data_to_send[_cnt++]=BYTE0(rc_data.yaw);
	data_to_send[_cnt++]=BYTE1(rc_data.roll);
	data_to_send[_cnt++]=BYTE0(rc_data.roll);
	data_to_send[_cnt++]=BYTE1(rc_data.pitch);
	data_to_send[_cnt++]=BYTE0(rc_data.pitch);
	data_to_send[_cnt++]=BYTE1(rc_data.ch4);
	data_to_send[_cnt++]=BYTE0(rc_data.ch4);
	data_to_send[_cnt++]=BYTE1(rc_data.ch5);
	data_to_send[_cnt++]=BYTE0(rc_data.ch5);
	data_to_send[_cnt++]=BYTE1(rc_data.ch6);
	data_to_send[_cnt++]=BYTE0(rc_data.ch6);
	data_to_send[_cnt++]=BYTE1(rc_data.ch7);
	data_to_send[_cnt++]=BYTE0(rc_data.ch7);
	data_to_send[_cnt++]=BYTE1(rc_data.ch7);
	data_to_send[_cnt++]=BYTE0(rc_data.ch7);
	data_to_send[_cnt++]=BYTE1(rc_data.ch7);
	data_to_send[_cnt++]=BYTE0(rc_data.ch7);
	
	data_to_send[3] = _cnt-4;
	
	
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	Uart_DMA(DMA_CH0,(void *)data_to_send,(void *)&UART1_D,_cnt,DMA_UART1_Tx);
}

static void Send_Check(uint8 head, uint8 check_sum)
{
	uint8 i;
	uint8 sum = 0;
	data_to_send[0]=0xAA;
	data_to_send[1]=0xAA;
	data_to_send[2]=0xEF;
	data_to_send[3]=2;
	data_to_send[4]=head;
	data_to_send[5]=check_sum;
	
	
	
	for(i=0;i<6;i++)
		sum += data_to_send[i];
	data_to_send[6]=sum;
//uart_sendN(KUART1 ,data_to_send, 7);
	
	  DMA_IRQ_CLEAN(DMA_CH0);                             //清除通道传输中断标志位    (这样才能再次进入中断)
		DMA_IRQ_DIS(DMA_CH0);
		DMA_DIS(DMA_CH0);  
		DMA_EN(DMA_CH0);                                    //使能通道CHn 硬件请求 
		DMA_IRQ_EN(DMA_CH0) ;  
		DMA_IRQ_CLEAN(DMA_CH0);
	Uart_DMA(DMA_CH0,(void *)data_to_send,(void *)&UART1_D,7,DMA_UART1_Tx);
}
/////////////////////////////////////////////////////////////////////////////////////
//Data_Receive_Prepare函数是协议预解析，根据协议的格式，将收到的数据进行一次格式性解析，格式正确的话再进行数据解析
//移植时，此函数应由用户根据自身使用的通信方式自行调用，比如串口每收到一字节数据，则调用此函数一次
//此函数解析出符合格式的数据帧后，会自行调用数据解析函数
void Data_Receive_Prepare(uint8 data)
{
	static uint8 RxBuffer[50];
	static uint8 _data_len = 0,_data_cnt = 0;
	static uint8 state = 0;
	
	if(state==0&&data==0xAA)
	{
		state=1;
		RxBuffer[0]=data;
	}
	else if(state==1&&data==0xAF)
	{
		state=2;
		RxBuffer[1]=data;
	}
	else if(state==2&&data<0XF1)
	{
		state=3;
		RxBuffer[2]=data;
	}
	else if(state==3&&data<50)
	{
		state = 4;
		RxBuffer[3]=data;
		_data_len = data;
		_data_cnt = 0;
	}
	else if(state==4&&_data_len>0)
	{
		_data_len--;
		RxBuffer[4+_data_cnt++]=data;
		if(_data_len==0)
			state = 5;
	}
	else if(state==5)
	{
		state = 0;
		RxBuffer[4+_data_cnt]=data;
		Data_Receive_Anl(RxBuffer,_data_cnt+5);
	}
	else
		state = 0;
}
/////////////////////////////////////////////////////////////////////////////////////
//Data_Receive_Anl函数是协议数据解析函数，函数参数是符合协议格式的一个数据帧，该函数会首先对协议数据进行校验
//校验通过后对数据进行解析，实现相应功能
//此函数可以不用用户自行调用，由函数Data_Receive_Prepare自动调用
//uint16 flash_save_en_cnt = 0;
//uint16 RX_CH[8];

void Data_Receive_Anl(uint8 *data_buf,uint8 num)
{
	uint8 sum = 0;
	uint8 i;
	for(i=0;i<(num-1);i++)
		sum += *(data_buf+i);
	if(!(sum==*(data_buf+num-1)))		return;		//判断sum
	if(!(*(data_buf)==0xAA && *(data_buf+1)==0xAF))		return;		//判断帧头
	
	if(*(data_buf+2)==0X01)
	{
		if(*(data_buf+4)==0X01)
		{
//			mpu6050.Acc_CALIBRATE = 1;
			//mpu6050.Cali_3d = 1;
		}
		else if(*(data_buf+4)==0X02){}
//			mpu6050.Gyro_CALIBRATE = 1;
		else if(*(data_buf+4)==0X03)
		{
//			mpu6050.Acc_CALIBRATE = 1;		
//			mpu6050.Gyro_CALIBRATE = 1;			
		}
		else if(*(data_buf+4)==0X04)
		{
//			Mag_CALIBRATED = 1;
		}
		else if((*(data_buf+4)>=0X021)&&(*(data_buf+4)<=0X26))
		{
			//acc_3d_calibrate_f = 1;
		}
		else if(*(data_buf+4)==0X20)
		{
			//acc_3d_step = 0; //退出，6面校准步清0
		}
	}
	
	if(*(data_buf+2)==0X02)
	{
		if(*(data_buf+4)==0X01)
		{
//			f.send_pid1 = 1;
//			f.send_pid2 = 1;
//			f.send_pid3 = 1;
//			f.send_pid4 = 1;
//			f.send_pid5 = 1;
//			f.send_pid6 = 1;
		}
		if(*(data_buf+4)==0X02)
		{
			
		}
		if(*(data_buf+4)==0XA0)		//读取版本信息
		{
	//		f.send_version = 1;
		}
		if(*(data_buf+4)==0XA1)		//恢复默认参数
		{
//			Para_ResetToFactorySetup();
		}
	}

	if(*(data_buf+2)==0X03)
	{
//		if( NS != 1 )
//		{
////			Feed_Rc_Dog(2);
//		}

//		RX_CH[THR] = (int16_t)(*(data_buf+4)<<8)|*(data_buf+5) ;
//		RX_CH[YAW] = (int16_t)(*(data_buf+6)<<8)|*(data_buf+7) ;
//		RX_CH[ROL] = (int16_t)(*(data_buf+8)<<8)|*(data_buf+9) ;
//		RX_CH[PIT] = (int16_t)(*(data_buf+10)<<8)|*(data_buf+11) ;
//		RX_CH[AUX1] = (int16_t)(*(data_buf+12)<<8)|*(data_buf+13) ;
//		RX_CH[AUX2] = (int16_t)(*(data_buf+14)<<8)|*(data_buf+15) ;
//		RX_CH[AUX3] = (int16_t)(*(data_buf+16)<<8)|*(data_buf+17) ;
//		RX_CH[AUX4] = (int16_t)(*(data_buf+18)<<8)|*(data_buf+19) ;
	}

	if(*(data_buf+2)==0X10)								//PID1
    {
        Xshell.Kp = 0.1*( (int16_t)(*(data_buf+4)<<8)|*(data_buf+5) );
        Xshell.Ki = 0.001*( (int16_t)(*(data_buf+6)<<8)|*(data_buf+7) );
        Xshell.Kd = 0.1*( (int16_t)(*(data_buf+8)<<8)|*(data_buf+9) );
        Yshell.Kp = 0.1*( (int16_t)(*(data_buf+10)<<8)|*(data_buf+11) );
        Yshell.Ki = 0.001*( (int16_t)(*(data_buf+12)<<8)|*(data_buf+13) );
        Yshell.Kd = 0.1*( (int16_t)(*(data_buf+14)<<8)|*(data_buf+15) );
        Zshell.Kp = 0.1*( (int16_t)(*(data_buf+16)<<8)|*(data_buf+17) );
        Zshell.Ki = 0.001*( (int16_t)(*(data_buf+18)<<8)|*(data_buf+19) );
        Zshell.Kd = 0.1*( (int16_t)(*(data_buf+20)<<8)|*(data_buf+21) );
        Send_Check(*(data_buf+2),sum);
				//Param_SavePID();
		//	  PID_Para_Init();
	//			flash_save_en_cnt = 1;
    }
    if(*(data_buf+2)==0X11)								//PID2
    {
        Xinner.Kp  = 0.1*( (int16_t)(*(data_buf+4)<<8)|*(data_buf+5) );
        Xinner.Ki  = 0.1*( (int16_t)(*(data_buf+6)<<8)|*(data_buf+7) );
        Xinner.Kd  = 0.1*( (int16_t)(*(data_buf+8)<<8)|*(data_buf+9) );
        Yinner.Kp  = 0.1*( (int16_t)(*(data_buf+10)<<8)|*(data_buf+11) );
        Yinner.Ki  = 0.1*( (int16_t)(*(data_buf+12)<<8)|*(data_buf+13) );
        Yinner.Kd  = 0.1*( (int16_t)(*(data_buf+14)<<8)|*(data_buf+15) );
        Zinner.Kp  = 0.1*( (int16_t)(*(data_buf+16)<<8)|*(data_buf+17) );
        Zinner.Ki  = 0.1*( (int16_t)(*(data_buf+18)<<8)|*(data_buf+19) );
        Zinner.Kd  = 0.1*( (int16_t)(*(data_buf+20)<<8)|*(data_buf+21) );
        Send_Check(*(data_buf+2),sum);
	//			PID_Para_Init();
//				flash_save_en_cnt = 1;
    }
    if(*(data_buf+2)==0X12)								//PID3
    {	
//        pid_setup.groups.hc_sp.kp  = 0.001*( (int16_t)(*(data_buf+4)<<8)|*(data_buf+5) );
//        pid_setup.groups.hc_sp.ki  = 0.001*( (int16_t)(*(data_buf+6)<<8)|*(data_buf+7) );
//        pid_setup.groups.hc_sp.kd  = 0.001*( (int16_t)(*(data_buf+8)<<8)|*(data_buf+9) );
//			
//        pid_setup.groups.hc_height.kp = 0.001*( (int16_t)(*(data_buf+10)<<8)|*(data_buf+11) );
//        pid_setup.groups.hc_height.ki = 0.001*( (int16_t)(*(data_buf+12)<<8)|*(data_buf+13) );
//        pid_setup.groups.hc_height.kd = 0.001*( (int16_t)(*(data_buf+14)<<8)|*(data_buf+15) );
//			
//        pid_setup.groups.ctrl3.kp 	= 0.001*( (int16_t)(*(data_buf+16)<<8)|*(data_buf+17) );
//        pid_setup.groups.ctrl3.ki 	= 0.001*( (int16_t)(*(data_buf+18)<<8)|*(data_buf+19) );
//        pid_setup.groups.ctrl3.kd 	= 0.001*( (int16_t)(*(data_buf+20)<<8)|*(data_buf+21) );
        Send_Check(*(data_buf+2),sum);
//				PID_Para_Init();
//				flash_save_en_cnt = 1;
    }
	if(*(data_buf+2)==0X13)								//PID4
	{
//		    pid_setup.groups.ctrl4.kp  = 0.001*( (int16_t)(*(data_buf+4)<<8)|*(data_buf+5) );
//        pid_setup.groups.ctrl4.ki  = 0.001*( (int16_t)(*(data_buf+6)<<8)|*(data_buf+7) );
//        pid_setup.groups.ctrl4.kd  = 0.001*( (int16_t)(*(data_buf+8)<<8)|*(data_buf+9) );
			
//         pid_setup.groups.hc_height.kp = 0.001*( (int16_t)(*(data_buf+10)<<8)|*(data_buf+11) );
//         pid_setup.groups.hc_height.ki = 0.001*( (int16_t)(*(data_buf+12)<<8)|*(data_buf+13) );
//         pid_setup.groups.hc_height.kd = 0.001*( (int16_t)(*(data_buf+14)<<8)|*(data_buf+15) );
// 			
//         pid_setup.groups.ctrl3.kp 	= 0.001*( (int16_t)(*(data_buf+16)<<8)|*(data_buf+17) );
//         pid_setup.groups.ctrl3.ki 	= 0.001*( (int16_t)(*(data_buf+18)<<8)|*(data_buf+19) );
//         pid_setup.groups.ctrl3.kd 	= 0.001*( (int16_t)(*(data_buf+20)<<8)|*(data_buf+21) );
		Send_Check(*(data_buf+2),sum);
//		PID_Para_Init();
//	flash_save_en_cnt = 1;
	}
	if(*(data_buf+2)==0X14)								//PID5
	{
		Send_Check(*(data_buf+2),sum);
	}
	if(*(data_buf+2)==0X15)								//PID6
	{
		Send_Check(*(data_buf+2),sum);FXAS_muc_init();
	}
	uartcount = 0 ;
	
}

void Data_Send_MotoPWM(void)
{
	uint8 _cnt=0;
	uint8 sum = 0;
	
	uint8 i;
	
	uint16 Moto_duty_send[4];
	for(i=0;i<4;i++)
		Moto_duty_send[i]=Moto_duty[i]-8000;
	uint16 shell_out=Xshell.Out*100;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x06;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=BYTE1(Moto_duty_send[0]);
	data_to_send[_cnt++]=BYTE0(Moto_duty_send[0]);
	data_to_send[_cnt++]=BYTE1(Moto_duty_send[1]);
	data_to_send[_cnt++]=BYTE0(Moto_duty_send[1]);
	data_to_send[_cnt++]=BYTE1(Moto_duty_send[2]);
	data_to_send[_cnt++]=BYTE0(Moto_duty_send[2]);
	data_to_send[_cnt++]=BYTE1(Moto_duty_send[3]);
	data_to_send[_cnt++]=BYTE0(Moto_duty_send[3]);
//	data_to_send[_cnt++]=BYTE1(Xinner.Out);
//	data_to_send[_cnt++]=BYTE0(Xinner.Out);
//	data_to_send[_cnt++]=BYTE1(Yinner.Out);
//	data_to_send[_cnt++]=BYTE0(Yinner.Out);
//	data_to_send[_cnt++]=BYTE1(Zinner.Out);
//	data_to_send[_cnt++]=BYTE0(Zinner.Out);
//	data_to_send[_cnt++]=BYTE1(Moto_PWM_8);
//	data_to_send[_cnt++]=BYTE0(Moto_PWM_8);
	
	data_to_send[3] = _cnt-4;
	
	
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	Uart_DMA(DMA_CH0,(void *)data_to_send,(void *)&UART1_D,_cnt,DMA_UART1_Tx);
}
