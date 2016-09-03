/***********************************************************
 * 文件名       ：include.h
 * 说明         ：驱动模块包含文件
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 * 库版本       ：V1.0
 *时间          ：15.5.26
************************************************************/

#ifndef __INCLUDE_H
#define __INCLUDE_H

#include  "common.h"

/*
 * Include 用户自定义的头文件
 */

#include  "port.h"
#include  "K6x_gpio.h"
#include "isr.h"
#include <stdint.h>
#include "K6x_delay.h"

#include  "K6x_adc.h"       //ADC模块
#include  "K6x_can.h" 
#include  "K6x_dma.h"       //DMA模块
#include  "K6x_flash.h" 
#include  "K6x_FTM.h"       //FTM模块（FTM0：电机控制 / 通用 /PWM     FTM1、2：正交解码 / 通用 /PWM ）
#include  "K6x_i2c.h"
#include  "K6x_lptmr.h"     //低功耗定时器(延时)
#include  "K6x_PIT.h"       //周期中断计时器
#include  "K6x_spi.h"
#include  "K6x_uart.h"      //串口 

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
