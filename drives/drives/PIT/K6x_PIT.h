/***********************************************************
 * 文件名       ：PTI.c
 * 说明         ：PTI初始化
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 * 库版本       ：V1.0
 *时间          ：15.5.26
************************************************************/

#ifndef _K6X_PIT_H_
#define _K6X_PIT_H_



typedef enum PITn
{
    PIT0,
    PIT1,
    PIT2,
    PIT3
} PITn;


void    pit_init(PITn pitn, uint32_t cnt);                                        //初始化PITn，并设置定时时间(单位为bus时钟周期)
#define     pit_init_ms(PITn,ms)          pit_init(PITn,ms * BUSCLKkh);         //初始化PITn，并设置定时时间(单位为 ms)
#define     PIT_Flag_Clear(PITn)          PIT_TFLG(PITn)|=PIT_TFLG_TIF_MASK      //清中断标志


void        pit_dma_init(PITn pitn, uint32_t cnt);
#define     pit_dma_init_ms(PITn,ms)     pit_dma_init(PITn,ms * BUSCLKkh)

#endif  //_PIT_H_
