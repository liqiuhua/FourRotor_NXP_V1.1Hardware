/***********************************************************
 * 文件名       ：port.h
 * 说明         ：端口配置文件
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 * 库版本       ：V1.0
 *时间          ：15.5.26
************************************************************/

#ifndef __PORT_H__
#define __PORT_H__
#include  "common.h"

extern volatile PORT_Type *PTX[5] ;
/* PORTx_PCRn 配置 */
typedef enum
{
    //中断方式选择
    IRQ_DISABLED = 0X00 << PORT_PCR_IRQC_SHIFT,   //中断禁止
    DMA_RISING   = 0x01 << PORT_PCR_IRQC_SHIFT,   //DMA上升沿触发
    DMA_FALLING  = 0x02 << PORT_PCR_IRQC_SHIFT,   //DMA下降沿触发
    DMA_EITHER   = 0x03 << PORT_PCR_IRQC_SHIFT,   //DMA跳变沿触发
    IRQ_ZERO     = 0x08 << PORT_PCR_IRQC_SHIFT,   //低电平触发
    IRQ_RISING   = 0x09 << PORT_PCR_IRQC_SHIFT,   //上升沿触发
    IRQ_FALLING  = 0x0A << PORT_PCR_IRQC_SHIFT,   //下降沿触发
    IRQ_EITHER   = 0x0B << PORT_PCR_IRQC_SHIFT,   //跳变沿触发
    IRQ_ONE      = 0x0C << PORT_PCR_IRQC_SHIFT,   //高电平触发

    //MUX 功能选择
    ALT0         = 0x00 << PORT_PCR_MUX_SHIFT,
    ALT1         = 0x01 << PORT_PCR_MUX_SHIFT,    //GPIO
    ALT2         = 0x02 << PORT_PCR_MUX_SHIFT,
    ALT3         = 0x03 << PORT_PCR_MUX_SHIFT,
    ALT4         = 0x04 << PORT_PCR_MUX_SHIFT,
    ALT5         = 0x05 << PORT_PCR_MUX_SHIFT,
    ALT6         = 0x06 << PORT_PCR_MUX_SHIFT,
    ALT7         = 0x07 << PORT_PCR_MUX_SHIFT,
    
    HDS          = 0x01 << PORT_PCR_DSE_SHIFT,    //输出高驱动能力
    ODO          = 0x01 << PORT_PCR_ODE_SHIFT,    //漏极输出
    PF           = 0x01 << PORT_PCR_PFE_SHIFT,    //带无源滤波器
    SSR          = 0x01 << PORT_PCR_SRE_SHIFT,    //输出慢变化率          Slow slew rate

    //下拉上拉选择
    PULLDOWN     = 0x02 << PORT_PCR_PS_SHIFT,     //下拉
    PULLUP       = 0x03 << PORT_PCR_PS_SHIFT,     //上拉


} port_pcr;

#define port_alt(portx,alt)      (PORT_PCR_REG(PTX[portx>>5],(portx&0x1f)) = ((PORT_PCR_REG(PTX[portx>>5],(portx&0x1f))&(~PORT_PCR_MUX_MASK))| (alt&PORT_PCR_MUX_MASK))) //配置PORT端口输出定义

#define port_init(portx,cfg)      (PORT_PCR_REG(PTX[portx>>5],(portx&0x1f)) = cfg); //配置PORT端口输出定义




#endif
