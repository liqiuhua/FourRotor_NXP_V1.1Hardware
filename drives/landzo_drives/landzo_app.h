#ifndef __LANDZO_APP_H
#define __LANDZO_APP_H

#include "include.h"

typedef struct{
	  uint8  direct[2] ;
	  uint16 chvalue[2] ;
}CHndata;

void UVA_CH_FTM_init(void) ;    //ң����ͨ����ʼ��
void UVA_FTM(uint16* pwmduty) ; //PWMͨ����ֵ
void UVA_FTM_init(void) ;     //PWM ��ʼ��
uint8 Uart_DMA(uint8 damch ,void *souradd,                           //
	             void *daadd,uint32 trancount,DMA_sources DMSour ) ;   //��ʼ��DMA���ͺ���

#endif
