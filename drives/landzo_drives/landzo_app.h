#ifndef __LANDZO_APP_H
#define __LANDZO_APP_H

#include "include.h"

typedef struct{
	  uint8  direct[2] ;
	  uint16 chvalue[2] ;
}CHndata;

void UVA_CH_FTM_init(void) ;    //遥控器通道初始化
void UVA_FTM(uint16* pwmduty) ; //PWM通道赋值
void UVA_FTM_init(void) ;     //PWM 初始化
uint8 Uart_DMA(uint8 damch ,void *souradd,                           //
	             void *daadd,uint32 trancount,DMA_sources DMSour ) ;   //初始化DMA发送函数

#endif
