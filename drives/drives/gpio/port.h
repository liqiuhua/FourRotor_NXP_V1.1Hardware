/***********************************************************
 * �ļ���       ��port.h
 * ˵��         ���˿������ļ�
 * ����         ��landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://www.landzo.cn
 * ��汾       ��V1.0
 *ʱ��          ��15.5.26
************************************************************/

#ifndef __PORT_H__
#define __PORT_H__
#include  "common.h"

extern volatile PORT_Type *PTX[5] ;
/* PORTx_PCRn ���� */
typedef enum
{
    //�жϷ�ʽѡ��
    IRQ_DISABLED = 0X00 << PORT_PCR_IRQC_SHIFT,   //�жϽ�ֹ
    DMA_RISING   = 0x01 << PORT_PCR_IRQC_SHIFT,   //DMA�����ش���
    DMA_FALLING  = 0x02 << PORT_PCR_IRQC_SHIFT,   //DMA�½��ش���
    DMA_EITHER   = 0x03 << PORT_PCR_IRQC_SHIFT,   //DMA�����ش���
    IRQ_ZERO     = 0x08 << PORT_PCR_IRQC_SHIFT,   //�͵�ƽ����
    IRQ_RISING   = 0x09 << PORT_PCR_IRQC_SHIFT,   //�����ش���
    IRQ_FALLING  = 0x0A << PORT_PCR_IRQC_SHIFT,   //�½��ش���
    IRQ_EITHER   = 0x0B << PORT_PCR_IRQC_SHIFT,   //�����ش���
    IRQ_ONE      = 0x0C << PORT_PCR_IRQC_SHIFT,   //�ߵ�ƽ����

    //MUX ����ѡ��
    ALT0         = 0x00 << PORT_PCR_MUX_SHIFT,
    ALT1         = 0x01 << PORT_PCR_MUX_SHIFT,    //GPIO
    ALT2         = 0x02 << PORT_PCR_MUX_SHIFT,
    ALT3         = 0x03 << PORT_PCR_MUX_SHIFT,
    ALT4         = 0x04 << PORT_PCR_MUX_SHIFT,
    ALT5         = 0x05 << PORT_PCR_MUX_SHIFT,
    ALT6         = 0x06 << PORT_PCR_MUX_SHIFT,
    ALT7         = 0x07 << PORT_PCR_MUX_SHIFT,
    
    HDS          = 0x01 << PORT_PCR_DSE_SHIFT,    //�������������
    ODO          = 0x01 << PORT_PCR_ODE_SHIFT,    //©�����
    PF           = 0x01 << PORT_PCR_PFE_SHIFT,    //����Դ�˲���
    SSR          = 0x01 << PORT_PCR_SRE_SHIFT,    //������仯��          Slow slew rate

    //��������ѡ��
    PULLDOWN     = 0x02 << PORT_PCR_PS_SHIFT,     //����
    PULLUP       = 0x03 << PORT_PCR_PS_SHIFT,     //����


} port_pcr;

#define port_alt(portx,alt)      (PORT_PCR_REG(PTX[portx>>5],(portx&0x1f)) = ((PORT_PCR_REG(PTX[portx>>5],(portx&0x1f))&(~PORT_PCR_MUX_MASK))| (alt&PORT_PCR_MUX_MASK))) //����PORT�˿��������

#define port_init(portx,cfg)      (PORT_PCR_REG(PTX[portx>>5],(portx&0x1f)) = cfg); //����PORT�˿��������




#endif
