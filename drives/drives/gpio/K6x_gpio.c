/***********************************************************
 * �ļ���       ��K6x_gpio.c
 * ˵��         ��IO�˿�Ӧ�ú���
 * ����         ��landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://www.landzo.cn
 *��汾        ��V1.0
 *ʱ��          ��15.5.26
************************************************************/
/*
 * ����ͷ�ļ�
 */
#include "common.h"
#include "K6x_gpio.h"

volatile GPIO_Type * GPIOX[5] = {PTA_BASE_PTR, PTB_BASE_PTR, PTC_BASE_PTR, PTD_BASE_PTR, PTE_BASE_PTR}; 
volatile PORT_Type * PTX[5] = {PORTA_BASE_PTR, PORTB_BASE_PTR, PORTC_BASE_PTR, PORTD_BASE_PTR, PORTE_BASE_PTR};

/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�gpio_enable_port
*  ����˵������������GPIO�˿ڵ�ʱ�ӡ� ���߲��������á�
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2015-5-15   �Ѳ���
*  ��    ע�����ݴ�ѧ������
*************************************************************************/
void gpio_enable_port (void)
{
 	  SIM_SCGC5 |=   SIM_SCGC5_PORTA_MASK \
	               | SIM_SCGC5_PORTB_MASK \
	               | SIM_SCGC5_PORTC_MASK \
	               | SIM_SCGC5_PORTD_MASK \
	               | SIM_SCGC5_PORTE_MASK;
}
/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�gpio_Interrupt_init
*  ����˵������ʼ��GPIO�ж�Դ
*  ����˵����PTxn        �˿ں�
*            GPIO_CFG    �˿�״̬
*            mode        �ж�״̬ 
*  �������أ���
*  �޸�ʱ�䣺2015-5-15   �Ѳ���
*  ��    ע��
*************************************************************************/

void gpio_Interrupt_init(PTxn ptxn, GPIO_CFG cfg, uint32 mode) 
{
  uint8 altcfg ;
	vuint32 PCRcfg ;
  if((cfg == GPI)||(cfg == GPI_DOWN)||(cfg == GPI_UP)){
     altcfg = ptxn>> 5 ; 
		 PCRcfg = ALT1 + mode+ cfg;
     port_init(ptxn,PCRcfg);
     GPIO_PDDR_REG(GPIOX[altcfg]) &= ~(1 << (ptxn&0x1f));  //���ö˿ڷ���Ϊ���� 
     NVIC_EnableIRQ((IRQn_Type)(altcfg+PORTA_IRQn));
  }
}

/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�gpio_init
*  ����˵������ʼ��gpio
*  ����˵����PTxn        �˿ں�
*            GPIO_CFG    �˿�״̬
*            uint8       �������״̬ 1 ��0
*  �������أ���
*  �޸�ʱ�䣺2015-5-15   �Ѳ���
*  ��    ע��
*************************************************************************/
void gpio_init (PTxn ptxn, GPIO_CFG cfg, uint8 sata)
{ 
  vuint32 PCRcfg;
//�˿ڷ���������뻹�����
    if( (cfg  == GPI) || (cfg == GPI_UP) || (cfg == GPI_DOWN) )
    {
        PCRcfg =cfg+ALT1 ;
       // le = cfg ;
        port_alt(ptxn,PCRcfg);
        GPIO_PDDR_REG(GPIOX[ptxn>> 5]) &= ~(1 << (ptxn&0x1f));  //���ö˿ڷ���Ϊ���� 
    }
    else
    {  port_alt(ptxn,ALT1) ;  
      //���ö˿ڷ���Ϊ���
        GPIO_PDDR_REG(GPIOX[ptxn>> 5]) |= (1 << (ptxn&0x1f));        // GPIO PDDR �ܽź� ��1������Ӧ�ܽ�����Ϊ�˿ڷ������
        //�˿��������/        
        if(sata == 0){     
            GPIO_PDOR_REG(GPIOX[ptxn>> 5]) &= ~(1 << (ptxn&0x1f));   // GPIO PDOR �ܽź� ��0������Ӧ�ܽ�����Ϊ�˿�����͵�ƽ
         }else{
            GPIO_PDOR_REG(GPIOX[ptxn>> 5])  |= (1 << (ptxn&0x1f));   // GPIO PDOR �ܽź� ��1������Ӧ�ܽ�����Ϊ�˿�����ߵ�ƽ
        }
    }
}



/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�gpio_set
*  ����˵������������״̬
*  ����˵����PTxn        �˿ں�
*            uint8       �������״̬ 1 ��0
*  �������أ���
*  �޸�ʱ�䣺2015-5-15   �Ѳ���
*  ��    ע��
*************************************************************************/
void gpio_set (PTxn ptxn,uint8 sata){
  if(sata) GPIO_PDOR_REG(GPIOX[ptxn>> 5])  |= (1 << (ptxn&0x1f));   // GPIO PDOR �ܽź� ��1������Ӧ�ܽ�����Ϊ�˿�����ߵ�ƽ
   else    GPIO_PDOR_REG(GPIOX[ptxn>> 5]) &= ~(1 << (ptxn&0x1f));   // GPIO PDOR �ܽź� ��0������Ӧ�ܽ�����Ϊ�˿�����͵�ƽ 
}
/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�gpio_turn
*  ����˵�������ŷ�ת
*  ����˵����PTxn        �˿ں�
*  �������أ���
*  �޸�ʱ�䣺2015-5-15   �Ѳ���
*  ��    ע��
*************************************************************************/
void gpio_turn(PTxn ptxn){
  GPIO_PTOR_REG(GPIOX[ptxn>> 5]) |= (1 << (ptxn&0x1f));   // GPIO PTOR �ܽź�  
}

/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�gpio_get
*  ����˵�������ŷ�ת
*  ����˵����PTxn        �˿ں�
*  �������أ���
*  �޸�ʱ�䣺2015-5-15   �Ѳ���
*  ��    ע��
*************************************************************************/
uint8 gpio_get (PTxn ptxn){
  return ((GPIO_PDIR_REG(GPIOX[ptxn>> 5])>>(ptxn&0x1f)) &0x01);   // GPIO PDIR �ܽź�  
}
               
