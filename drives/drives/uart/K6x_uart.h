
/***********************************************************
 * 文件名       ：K6x_uart.h
 * 说明         ：串口初始化
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 * 库版本       ：V1.0
 *时间          ：15.5.26
************************************************************/

#ifndef __K6X_UART_H__
#define __K6X_UART_H__

#include "common.h"

typedef enum  UARTn
{
    //初始化默认配置       --TXD--      --RXD--     可以复用其他通道，请在<<pin_cfg.h>>自行修改 uart_init
    KUART0,    //           PTD7         PTD6
    KUART1,    //           PTC4         PTC3
    KUART2,    //           PTD3         PTD2
    KUART3,    //           PTC17        PTC16
    KUART4,    //           PTE24        PTE25
    KUART5     //           PTE8         PTE9
} UARTn;

extern volatile UART_Type *UARTx[6];
/********************************************************************/



void uart_init (UARTn uratn, uint32 baud);                 //初始化uartx模块

char uart_getchar   (UARTn);                          //无限等待接受1个字节

char uart_query (UARTn uratn);                        //查询是否接受到一个字节
void uart_putchar   (UARTn, char ch);                 //发送1个字节
void uart_sendN     (UARTn , uint8 *buff, uint16 len); //发送n个字节
void uart_sendStr   (UARTn , const uint8 *str);          //发送字符串



void uart_DMA_init (UARTn uratn, uint32 baud);      //初始化DMAUartx模块
void uart_irq_EN    (UARTn);                          //使用中断
void uart_irq_DIS   (UARTn);                          //禁止中断




/********************************************************************/

#endif /* __UART_H__ */
