
/***********************************************************
 * �ļ���       ��K6x_uart.h
 * ˵��         �����ڳ�ʼ��
 * ����         ��landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://www.landzo.cn
 * ��汾       ��V1.0
 *ʱ��          ��15.5.26
************************************************************/

#ifndef __K6X_UART_H__
#define __K6X_UART_H__

#include "common.h"

typedef enum  UARTn
{
    //��ʼ��Ĭ������       --TXD--      --RXD--     ���Ը�������ͨ��������<<pin_cfg.h>>�����޸� uart_init
    KUART0,    //           PTD7         PTD6
    KUART1,    //           PTC4         PTC3
    KUART2,    //           PTD3         PTD2
    KUART3,    //           PTC17        PTC16
    KUART4,    //           PTE24        PTE25
    KUART5     //           PTE8         PTE9
} UARTn;

extern volatile UART_Type *UARTx[6];
/********************************************************************/



void uart_init (UARTn uratn, uint32 baud);                 //��ʼ��uartxģ��

char uart_getchar   (UARTn);                          //���޵ȴ�����1���ֽ�

char uart_query (UARTn uratn);                        //��ѯ�Ƿ���ܵ�һ���ֽ�
void uart_putchar   (UARTn, char ch);                 //����1���ֽ�
void uart_sendN     (UARTn , uint8 *buff, uint16 len); //����n���ֽ�
void uart_sendStr   (UARTn , const uint8 *str);          //�����ַ���



void uart_DMA_init (UARTn uratn, uint32 baud);      //��ʼ��DMAUartxģ��
void uart_irq_EN    (UARTn);                          //ʹ���ж�
void uart_irq_DIS   (UARTn);                          //��ֹ�ж�




/********************************************************************/

#endif /* __UART_H__ */
