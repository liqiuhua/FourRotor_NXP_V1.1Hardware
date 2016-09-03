

#include "include.h"
#include "control_L.h"
#include "landzo_fxos8700.h"
uint32 SYSCLKKH,BUSCLKkh ;    //系统时钟和busclock

uint8 TIME0flag_1ms   ;
uint8 TIME0flag_2ms   ;
uint8 TIME0flag_3ms   ;
uint8 TIME0flag_5ms   ;
uint8 TIME0flag_10ms  ;
uint8 TIME0flag_20ms  ;
uint8 TIME0flag_50ms  ;
uint8 TIME0flag_80ms  ;
uint8 TIME0flag_100ms ;
uint8 TIME1flag_20ms  ;


T_FLOAT_ANGEL IMU_Angle_offset;
unsigned char OffsetFlag=0; 
unsigned char RC_Time_out;
typeGYRO GYRO_FXAS;
typeGYRO GYRO_FXAS_Filter;
AccelSensor foxacc1;
AccelSensor foxaccl_Filter;
MagSensor magccc ;
int16 magsensorMAX[3] = {-32768,-32768,-32768},magsensorMIN[3] = {32760,32760,32760},magsensorOFF[3];
int16 magsensorfif[3] ,magsensorcal[3] ;
uint16 agrn1 ;

T_FLOAT_ANGEL IMU_Angle;
float kkk;
uint8 UartDmadata[20] ;

#define FILTER_N 10
float filter_buf[FILTER_N + 1];
int Filter(float x) {
  int i;
  float filter_sum = 0;
  filter_buf[FILTER_N] =x;
  for(i = 0; i < FILTER_N; i++) {
    filter_buf[i] = filter_buf[i + 1]; 
    filter_sum += filter_buf[i];
  }
  return (float)(filter_sum / (float)FILTER_N);
}

int main(void){
	

	DisableInterrupts ; 
	ARMED=0;
	kkk = 1.00 ;
	int acc_offset_count=1;
	LanSystemCoreClockUpdate(&SYSCLKKH,&BUSCLKkh) ;
	gpio_enable_port();
	pit_init_ms(PIT0,1);   //初始化端口
	
	NVIC_SetPriority(UART1_RX_TX_IRQn, 3);
	NVIC_SetPriority(FTM0_IRQn, 5);
	NVIC_SetPriority(FTM3_IRQn, 8);
	NVIC_SetPriority(PORTB_IRQn,10);
	NVIC_SetPriority(PORTC_IRQn,11);
//	NVIC_SetPriority(PORTE_IRQn, 12);
	
//	      Yshell.Kp = 1.0f;
//        Yshell.Ki = 0.5f;
//        Yshell.Kd =8.0f;
//        Xshell.Kp =0.8f;
//        Xshell.Ki = 0;
//        Xshell.Kd = 3.0f;
//        Zshell.Kp = 0;
//        Zshell.Ki = 0;
//        Zshell.Kd = 0;

//        Yinner.Kp  = 0.8f;
//        Yinner.Ki  = 0;
//        Yinner.Kd  = 10.0f;
//        Xinner.Kp  = 1.0f;
//        Xinner.Ki  = 0;
//        Xinner.Kd  = 8.0f;
//        Zinner.Kp  = 0.5;
//        Zinner.Ki  = 0;
//        Zinner.Kd  = 0;
//			
			
			
	
	gpio_init(PTC19,GPO,HIGH);
	gpio_init(PTD0,GPO,HIGH);
	gpio_init(PTD1,GPO,HIGH);
	gpio_init(PTE28,GPO,HIGH);
	
	uart_DMA_init(KUART1,115200);		//串口和DMA初始化
	uart_irq_EN(KUART1);
	uart_init(KUART1,115200);
  DMA_Source2BUFF_Init(DMA_CH0, UartDmadata,(void *)&UART1_D,DMA_UART1_Tx,DMA_BYTE1, 10,1,0, DMA_either); 	
	
  UVA_FTM_init();
	UVA_CH_FTM_init() ; //遥控器通过初始化
//	mpl_mcu_init();     //高度传感器初始化
	FXAS_muc_init();		//陀螺仪传感器初始化
	FXO_MUC_init(&foxacc1,&magccc);		//地磁和加速度传感器初始化
  EnableInterrupts ;
	while(1){

		if(foxmagflag)
		{
			foxmagflag = 0 ;
			MagSensorfilter(&magccc.iBs[0],&magsensorfif[0]);
			MagSensorMAXMIN(&magsensorMAX[0],&magsensorMIN[0],&magsensorOFF[0],&magsensorfif[0]);
			agrn1 = MagSensorangle(&magsensorOFF[0],&magsensorfif[0],&magsensorcal[0]);
		}
				
		if(OffsetFlag==1){
			//	PTD1_OUT =0;
				if(ARMED==0 && acc_offset_count<2 )
				{
					AHRSupdate(GYRO_FXAS_Filter.X* GYRO_RAW_TO_RAD_S , GYRO_FXAS_Filter.Y* GYRO_RAW_TO_RAD_S , GYRO_FXAS_Filter.Z* GYRO_RAW_TO_RAD_S ,     \
											-foxaccl_Filter.iGs[1],foxaccl_Filter.iGs[0],-foxaccl_Filter.iGs[2] , \
											-magsensorfif[0],magsensorfif[1],magsensorfif[2]);
					IMU_Angle_offset.PITCH = Pitch;//IMU_Angle_offset.PITCH;//+)/(float)acc_offset_count;
					IMU_Angle_offset.ROL = Roll;//IMU_Angle_offset.ROL; //+)/(float)acc_offset_count;
					IMU_Angle_offset.YAW = Yaw;//IMU_Angle_offset.YAW ;//+)/(float)acc_offset_count;
					acc_offset_count++;
					delay_ms(100);
				}
				else
				{
					acc_offset_count=1;
					OffsetFlag=0;
				//	PTD1_OUT = 1;	
				}
		}
		else{
			if(TIME0flag_1ms)
			{
				TIME0flag_1ms = 0 ;		
			}
			if(TIME0flag_3ms)
			{
				TIME0flag_3ms = 0 ;

				GYRO_FXAS_Filter.X=LowPassFilter2p(400.0f, 200.0f,GYRO_FXAS.X-70);
				GYRO_FXAS_Filter.Y=LowPassFilter2p_Y(400.0f, 200.0f,GYRO_FXAS.Y+100);
				GYRO_FXAS_Filter.Z=LowPassFilter2p_Z(400.0f, 200.0f,GYRO_FXAS.Z);
				
//			GYRO_FXAS_Filter.X=Filter(GYRO_FXAS_Filter.X);
					
				Prepare_Data(&foxacc1,&foxaccl_Filter);

				AHRSupdate(GYRO_FXAS_Filter.X* GYRO_RAW_TO_RAD_S , GYRO_FXAS_Filter.Y* GYRO_RAW_TO_RAD_S , GYRO_FXAS_Filter.Z* GYRO_RAW_TO_RAD_S ,\
								-foxaccl_Filter.iGs[1],foxaccl_Filter.iGs[0],-foxaccl_Filter.iGs[2] , \
								-magsensorfif[1],magsensorfif[0],magsensorfif[2]);
			}		
			if(TIME0flag_5ms)
			{
				TIME0flag_5ms=0;
				IMU_Angle.PITCH=Pitch-IMU_Angle_offset.PITCH;
				IMU_Angle.ROL=Roll-IMU_Angle_offset.ROL;
				IMU_Angle.YAW=Yaw-IMU_Angle_offset.YAW;
				
				control_L(IMU_Angle, GYRO_FXAS_Filter);			
			}
			if(TIME0flag_10ms)
			{
				TIME0flag_10ms=0;		
//				control_L(IMU_Angle, GYRO_FXAS_Filter);	
				Data_Send_Senser();
				
			}
			if(TIME0flag_20ms)
			{
				TIME0flag_20ms=0;
					
				Data_Send_Status();				
			}
			if(TIME0flag_100ms)
			{
				TIME0flag_100ms=0;
				PTC19_OUT = ~PTC19_OUT ;
				Data_Send_RCData();			
				if(ARMED==1)
				{
					PTD0_OUT = ~PTD0_OUT ;
					PTD1_OUT = ~PTD1_OUT ;
					PTE28_OUT=1;	
					Data_Send_MotoPWM();				
				}
				if(ARMED==0)
				{
					PTD0_OUT =1;
					PTD1_OUT =1;
					PTE28_OUT=0;
				}
			}
		}
	}
}
