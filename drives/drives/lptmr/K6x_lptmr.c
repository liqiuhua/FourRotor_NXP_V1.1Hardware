/***********************************************************
 * 文件名       ：lptmr.c
 * 说明         ：低功耗定时模式
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 * 库版本       ：V1.0
 *时间          ：15.5.26
************************************************************/

#include "common.h"
#include "K6x_lptmr.h"



/*************************************************************************
*                             蓝宙电子科技有限公司
*
*  函数名称：lptmr_counter_init
*  功能说明：获得脉冲计数
*  参数说明：LPT0_ALTn  PIN脚选择
*            count      脉冲个数
*            LPT_CFG    触发方式
*  函数返回：脉冲数
*  修改时间：2015-5-15   已测试
*  备    注：
*************************************************************************/
void lptmr_counter_init(LPT0_ALTn altn, uint16_t count, LPT_CFG cfg)
{


    /* 开启模块时钟 */
    SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;    //使能LPT模块时钟

    //设置输入管脚
    if(altn == LPT0_ALT1)
    {
        PORTA_PCR19 = PORT_PCR_MUX(0x6);    //在PTA19上使用 ALT5
    }
    else if(altn == LPT0_ALT2)
    {
        PORTC_PCR5 = PORT_PCR_MUX(0x3);     //在PTC5上使用 ALT3
    }

    // 清状态寄存器
    LPTMR0_CSR = 0x00 ;                                          //先关了LPT，这样才能设置时钟分频,清空计数值等
    LPTMR0_PSR = 0x00 ;
    LPTMR0_CSR = 0x00 ;
    LPTMR0_CMR = 0X00 ; 
    
    // 状态寄存器设置
    
    LPTMR0_PSR  =   (0
                     |LPTMR_PSR_PCS(0x00)
                     |LPTMR_PSR_PBYP_MASK
                     |LPTMR_PSR_PRESCALE(1)); 
    // 管脚设置、使能中断
    LPTMR0_CMR = count ;
    LPTMR0_CSR  =  (0
                    | LPTMR_CSR_TPS(altn)       // 选择输入管脚 选择
                    | LPTMR_CSR_TMS_MASK        // 选择脉冲计数 (注释了表示时间计数模式)
                    | ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //脉冲计数器触发方式选择：0为高电平有效，上升沿加1
               //     | LPTMR_CSR_TEN_MASK        //使能LPT(注释了表示禁用)
                    | LPTMR_CSR_TIE_MASK        //中断使能
                    //| LPTMR_CSR_TFC_MASK      //0:计数值等于比较值就复位；1：溢出复位（注释表示0）
                   );
     
    LPTMR0_CSR  |= LPTMR_CSR_TEN_MASK ; 
    NVIC_EnableIRQ(LPTimer_IRQn);	                //开引脚的IRQ中断
}


/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：time_delay_ms
*  功能说明：延时函数，使用定功耗定时器延时，准确
*  参数说明：ms   延时时间，单位为ms
*  函数返回：无
*  修改时间：2012-1-20
*  备    注：官方例程自带
*************************************************************************/
void time_delay_ms(uint32 ms)
{
    /* Make sure the clock to the LPTMR is enabled */
    SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;

    /* Set the compare value to the number of ms to delay */
    LPTMR0_CMR = ms;

    /* Set up LPTMR to use 1kHz LPO with no prescaler as its clock source */
    LPTMR0_PSR = LPTMR_PSR_PCS(1) | LPTMR_PSR_PBYP_MASK;

    /* Start the timer */
    LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;

    /* Wait for counter to reach compare value */
    while (!(LPTMR0_CSR & LPTMR_CSR_TCF_MASK));

    /* Clear Timer Compare Flag */
    LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK;

    return;
}


/*************************************************************************
*                             蓝宙电子科技有限公司
*
*  函数名称：lptmr_pulse_get
*  功能说明：获得脉冲计数
*  参数说明：
*  函数返回：脉冲数
*  修改时间：2015-5-15   已测试
*  备    注：
*************************************************************************/

uint16 lptmr_pulse_get(void)
{
    uint16  LPTCOUNT = 0  ;
    LPTMR0_CNR = 12 ;
    LPTCOUNT = LPTMR0_CNR ;
    lptmr_counter_clean();
    return (uint16)LPTCOUNT;
}



