/***********************************************************
 * 文件名       ：K6x_gpio.h
 * 说明         ：IO端口应用
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 *库版本        ：V1.0
 *时间          ：15.5.26
************************************************************/
#ifndef __K6x_GPIO_H__
#define __K6x_GPIO_H__

#include "gpio_cfg.h"
#include  "port.h"
/*
 * 定义管脚方向
 */
typedef enum GPIO_CFG
{
    GPI         = 0,                                //定义管脚输入方向      GPIOx_PDDRn里，0表示输入，1表示输出
    GPO         = 1,                                //定义管脚输出方向
    
    GPI_DOWN    = PULLDOWN,                       //输入下拉              
    GPI_UP      = PULLUP,                       //输入上拉    
       
} GPIO_CFG;

#define DISABLED      IRQ_DISABLED    //中断禁止
#define DMARING       DMA_RISING     //DMA上升沿触发
#define DMAFALLING    DMA_FALLING     //DMA下降沿触发
#define DMAEITHER     DMA_EITHER     //DMA跳变沿触发
#define ZERO          IRQ_ZERO        //低电平触发
#define RING          IRQ_RISING      //上升沿触发
#define FALLING       IRQ_FALLING    //下降沿触发
#define EITHER        IRQ_EITHER    //跳变沿触发
#define ONE           IRQ_ONE         //高电平触发

void gpio_init (PTxn ptxn, GPIO_CFG cfg, uint8 data) ;
void gpio_set (PTxn ptxn,uint8 sata) ;
void gpio_turn(PTxn ptxn) ;
uint8 gpio_get (PTxn ptxn) ;
void gpio_enable_port (void) ;
void gpio_Interrupt_init(PTxn ptxn, GPIO_CFG cfg, uint32 mode)   ;

#endif      
