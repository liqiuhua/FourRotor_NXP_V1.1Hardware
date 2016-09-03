/******************** (C) COPYRIGHT 2011 蓝宙电子工作室 ********************
 * 文件名       ：isr.h
 * 描述         ：工程模版实验
 *
 * 实验平台     ：landzo电子开发版
 * 库版本       ：
 * 嵌入系统     ：
 *
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/

**********************************************************************************/



#ifndef __ISR_H
#define __ISR_H 1
#include  "common.h"
extern uint8 DMAflag ;
extern uint8 uartcount;
void HardFault_Handler(void);      //发生硬件上访（程序跑飞）
void USART0_IRQHandler(void);
void USART1_IRQHandler(void);
void PIT0_IRQHandler(void);            //PIT0 定时中断服务函数
void PIT1_IRQHandler(void);            //PIT1 定时中断服务函数
void PORTA_IRQHandler(void);           //PORTA中断服务函数
void PORTB_IRQHandler(void);           //PORTB中断服务函数
void PORTD_IRQHandler(void);           //PORTD中断服务函数
void FTM0_IRQHandler(void);            //FTM0输入捕捉中断
void FTM1_IRQHandler(void);            //FTM0输入捕捉中断
void LPT_Handler(void);           //LPT脉冲中断
void DMA_CH0_Handler(void);
void DMA_CH4_Handler(void);              //DMA通道4的中断服务函数
void DMA0_IRQHandler(void);
void UART3_RX_TX_IRQHandler(void);
//void FTM0_IRQHandler();

#endif  //__ISR_H

/* End of "isr.h" */
