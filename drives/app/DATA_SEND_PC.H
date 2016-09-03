/***********************************************************
 * �ļ���       ��DATA_SEND_PC.h
 * ˵��         ����λ�������ϴ���������λ��������������λ�����̽����޸�
 * ����         ��landzo �������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://www.landzo.cn
 * ��汾       ��
 *ʱ��          ��16.08.01
************************************************************/


#ifndef __DATA_SEND_PC_H
#define __DATA_SEND_PC_H

#include "include.h"
#include "common.h"
#include "landzo_MPL3115A2.h"
#include "landzo_FXAS21002.h"

typedef struct 
{
		uint8 send_version;
		uint8 send_status;
		uint8 send_senser;
		uint8 send_senser2;
		uint8 send_pid1;
		uint8 send_pid2;
		uint8 send_pid3;
		uint8 send_pid4;
		uint8 send_pid5;
		uint8 send_pid6;
		uint8 send_rcdata;
		uint8 send_offset;
		uint8 send_motopwm;
		uint8 send_power;
		uint8 send_user;
		uint8 send_speed;
		uint8 send_location;

}dt_flag_t;

extern dt_flag_t f;

extern T_INT16_XYZ mpu_acc_kf;

extern AccelSensor foxacc1;
extern MagSensor magccc ;
extern int16 magsensorfif[3] ,magsensorcal[3] ;

//void Data_Exchange(void);
void Data_Receive_Prepare(uint8 data);
void Data_Receive_Anl(uint8 *data_buf,uint8 num);

void Data_Send_RCData(void);
void Data_Send_MotoPWM(void);
void Data_Send_Status(void);
void Data_Send_Senser(void) ;


void Send_Power(uint16 votage, uint16 current);


#endif