/******************** (C) COPYRIGHT 2011 ������ӹ����� ********************
 * �ļ���       ��isr.h
 * ����         ������ģ��ʵ��
 *
 * ʵ��ƽ̨     ��landzo���ӿ�����
 * ��汾       ��
 * Ƕ��ϵͳ     ��
 *
 * ����         ��landzo ������
 * �Ա���       ��http://landzo.taobao.com/

**********************************************************************************/



#ifndef __ISR_H
#define __ISR_H 1
#include  "common.h"
extern uint8 DMAflag ;
extern uint8 uartcount;
void HardFault_Handler(void);      //����Ӳ���Ϸã������ܷɣ�
void USART0_IRQHandler(void);
void USART1_IRQHandler(void);
void PIT0_IRQHandler(void);            //PIT0 ��ʱ�жϷ�����
void PIT1_IRQHandler(void);            //PIT1 ��ʱ�жϷ�����
void PORTA_IRQHandler(void);           //PORTA�жϷ�����
void PORTB_IRQHandler(void);           //PORTB�жϷ�����
void PORTD_IRQHandler(void);           //PORTD�жϷ�����
void FTM0_IRQHandler(void);            //FTM0���벶׽�ж�
void FTM1_IRQHandler(void);            //FTM0���벶׽�ж�
void LPT_Handler(void);           //LPT�����ж�
void DMA_CH0_Handler(void);
void DMA_CH4_Handler(void);              //DMAͨ��4���жϷ�����
void DMA0_IRQHandler(void);
void UART3_RX_TX_IRQHandler(void);
//void FTM0_IRQHandler();

#endif  //__ISR_H

/* End of "isr.h" */
