/***********************************************************
 * �ļ���       ��include.h
 * ˵��         ������ģ������ļ�
 * ����         ��landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://www.landzo.cn
 * ��汾       ��V1.0
 *ʱ��          ��15.5.26
************************************************************/

#ifndef __INCLUDE_H
#define __INCLUDE_H

#include  "common.h"

/*
 * Include �û��Զ����ͷ�ļ�
 */

#include  "port.h"
#include  "K6x_gpio.h"
#include "isr.h"
#include <stdint.h>
#include "K6x_delay.h"

#include  "K6x_adc.h"       //ADCģ��
#include  "K6x_can.h" 
#include  "K6x_dma.h"       //DMAģ��
#include  "K6x_flash.h" 
#include  "K6x_FTM.h"       //FTMģ�飨FTM0��������� / ͨ�� /PWM     FTM1��2���������� / ͨ�� /PWM ��
#include  "K6x_i2c.h"
#include  "K6x_lptmr.h"     //�͹��Ķ�ʱ��(��ʱ)
#include  "K6x_PIT.h"       //�����жϼ�ʱ��
#include  "K6x_spi.h"
#include  "K6x_uart.h"      //���� 

#include "KEY.h"
#include "landzo_app.h"

#include "landzo_MPL3115A2.h"
#include "landzo_FXAS21002.h" 
#include "landzo_fxos8700.h"

#include "DATA_SEND_PC.h"
#include "RC.H"

#include "Att_Q.h"
#include "control_L.h"


#endif  //__INCLUDE_H__
