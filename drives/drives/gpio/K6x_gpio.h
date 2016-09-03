/***********************************************************
 * �ļ���       ��K6x_gpio.h
 * ˵��         ��IO�˿�Ӧ��
 * ����         ��landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://www.landzo.cn
 *��汾        ��V1.0
 *ʱ��          ��15.5.26
************************************************************/
#ifndef __K6x_GPIO_H__
#define __K6x_GPIO_H__

#include "gpio_cfg.h"
#include  "port.h"
/*
 * ����ܽŷ���
 */
typedef enum GPIO_CFG
{
    GPI         = 0,                                //����ܽ����뷽��      GPIOx_PDDRn�0��ʾ���룬1��ʾ���
    GPO         = 1,                                //����ܽ��������
    
    GPI_DOWN    = PULLDOWN,                       //��������              
    GPI_UP      = PULLUP,                       //��������    
       
} GPIO_CFG;

#define DISABLED      IRQ_DISABLED    //�жϽ�ֹ
#define DMARING       DMA_RISING     //DMA�����ش���
#define DMAFALLING    DMA_FALLING     //DMA�½��ش���
#define DMAEITHER     DMA_EITHER     //DMA�����ش���
#define ZERO          IRQ_ZERO        //�͵�ƽ����
#define RING          IRQ_RISING      //�����ش���
#define FALLING       IRQ_FALLING    //�½��ش���
#define EITHER        IRQ_EITHER    //�����ش���
#define ONE           IRQ_ONE         //�ߵ�ƽ����

void gpio_init (PTxn ptxn, GPIO_CFG cfg, uint8 data) ;
void gpio_set (PTxn ptxn,uint8 sata) ;
void gpio_turn(PTxn ptxn) ;
uint8 gpio_get (PTxn ptxn) ;
void gpio_enable_port (void) ;
void gpio_Interrupt_init(PTxn ptxn, GPIO_CFG cfg, uint32 mode)   ;

#endif      
