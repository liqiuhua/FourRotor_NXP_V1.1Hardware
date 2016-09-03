/***********************************************************
 * �ļ���       ��isr.c
 * ˵��         ���жϹ���ģ��
 * ����         ��landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://www.landzo.cn
 * ��汾       ��V1.0
 *ʱ��          ��15.5.26
************************************************************/

#include "isr.h"
#include  "include.h"
#include "landzo_fxos8700.h"
extern typeGYRO GYRO_FXAS;
extern uint8 TIME0flag_1ms   ;
extern uint8 TIME0flag_5ms   ;
extern uint8 TIME0flag_10ms  ;
extern uint8 TIME0flag_3ms  ;
extern uint8 TIME0flag_20ms  ;
extern uint8 TIME0flag_50ms  ;
extern uint8 TIME0flag_80ms  ;
extern uint8 TIME0flag_100ms ;
extern uint8 TIME1flag_20ms  ;
uint32  TimeCount = 0 ;
extern unsigned char OffsetFlag; 
extern unsigned char RC_Time_out;
///*************************************************************************
//*                             ������ӹ�����
//*
//*  �������ƣ�setIRQadrr
//*  ����˵���������жϵ�ַ����
//*  ����˵����IRQInterruptIndex �ж���������оƬ��.H�ļ�������K60 ��"MK60DZ10.h"
//*            void IRQAdrr(void)  �жϺ���
//*  �������أ���
//*  �޸�ʱ�䣺2015-5-15    �Ѳ���
//*  ��    ע��
//*************************************************************************/

//__RAMFUN void setIRQadrr(IRQInterruptIndex IRQnum,void IRQAdrr(void) ){
//  extern uint32 __VECTOR_TABLE[];
//  extern uint32 __VECTOR_RAM[];
//  vuint32  *Vector_TABl ,Vector_TAB;
//  vuint16 temp0 ,temp1;
//  vuint32 buf[512];
//  Vector_TAB = SCB_VTOR ;
//  Vector_TABl = (vuint32  *)Vector_TAB ;
//  /**************
//  �ж���������ram���򣬿��Խ���ֱ�Ӳ���
//  **************/
//  if(Vector_TAB == (uint32)&__VECTOR_RAM){   
//    Vector_TABl[IRQnum] = (uint32)(IRQAdrr) ;
//   }
//  else{
//    /*****************************
//    ��������flash��
//    *****************************/
//    for(temp0 = 0 ; temp0 < 512 ; temp0++){
//      buf[temp0] = Vector_TABl[temp0] ;
//    }
//    buf[IRQnum]  = (uint32)(IRQAdrr) ;  
//    hw_flash_init() ;
//    flash_erase_sector(0) ;
//    flash_write(0,0,2048,(uint8 *)buf) ;
//  }
//  }



/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�USART0_IRQHandler
*  ����˵��������0 �ж� ���� ������
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2014-8-28    �Ѳ���
*  ��    ע��
*************************************************************************/
#define USART_REC_LEN  40
uint8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
uint8 uartcount= 0 ;
uint8 TimeCut=0;
//static uint8 RxState = 0;
void UART1_RX_TX_IRQHandler(void)
{

 
  if ((UART_S1_REG(UARTx[1]) & UART_S1_RDRF_MASK)){

		Data_Receive_Prepare(UART_D_REG(UARTx[1]));
	}
		 if(uartcount > USART_REC_LEN ) uartcount = 0 ;



}
///*************************************************************************
//*                             ������ӹ�����
//*
//*  �������ƣ�USART1_IRQHandler                                                                                                                          
//*  ����˵��������1 �ж� ���� ������
//*  ����˵������
//*  �������أ���
//*  �޸�ʱ�䣺2012-2-14    �Ѳ���
//*  ��    ע��
//*************************************************************************/


//void USART1_IRQHandler(void)
//{
//    uint8 ch;

//    DisableInterrupts;		    //�����ж�

//    //����һ���ֽ����ݲ��ط�
//    ch = uart_getchar (UART1);      //���յ�һ������
//    uart_sendStr  (UART1, "\n�㷢�͵�����Ϊ��");
//    uart_putchar (UART1, ch);       //�ͷ��ͳ�ȥ

//    EnableInterrupts;		    //�����ж�
//}
///*************************************************************************
//*                             ����Ƕ��ʽ����������
//*
//*  �������ƣ�PIT1_IRQHandler
//*  ����˵����PIT1��ʱ�жϷ�����
//*  ����˵������
//*  �������أ���
//*  �޸�ʱ�䣺2012-9-18    �Ѳ���
//*  ��    ע��
//*************************************************************************/

void PIT1_IRQHandler(void)
{
  PIT_Flag_Clear(PIT1);       //���жϱ�־λ
  //PTE28_OUT=~PTE28_OUT;
  
}

/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�PIT0_IRQHandler
*  ����˵����PIT0 ��ʱ�жϷ�����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-2-18    �Ѳ���
*  ��    ע��
*************************************************************************/



void PIT0_IRQHandler(void)
{
  PIT_Flag_Clear(PIT0);       //���жϱ�־λ
  
  TimeCount ++ ;
	TimeCut++;
  TIME0flag_1ms = 1 ;
	if(TimeCut==2)
	{
		TimeCut=0;
		TIME0flag_3ms=1;
	}
  if(TimeCount%5 == 0 )
  {
    TIME0flag_5ms = 1;
  } 
  if(TimeCount%10 == 0 )
  {
    TIME0flag_10ms = 1;    
  }   
  if(TimeCount%20 == 0 )
  {
    TIME0flag_20ms = 1;
  }
  if(TimeCount%50 == 0 )
  {
    TIME0flag_50ms = 1;
  }
  if(TimeCount%80 == 0 )
  {
    TIME0flag_80ms = 1;
  }  
  if(TimeCount%100 == 0 )
  {
    TIME0flag_100ms = 1;
    TimeCount = 0 ;
  }  
}

/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�HardFault_Handler
*  ����˵����Ӳ���Ϸ��жϷ�����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-2-4    �Ѳ���
*  ��    ע��������LED��˸��ָʾ������Ӳ���Ϸ�
*************************************************************************/
void HardFault_Handler(void)
{
  while (1)
  {
    //printf("\n****Ӳ���Ϸô���!!!*****\r\n\n");
  }
}

///*************************************************************************
//*                             ������ӹ�����
//*
//*  �������ƣ�PORTD_IRQHandler
//*  ����˵����PORTD�˿��жϷ�����
//*  ����˵������
//*  �������أ���
//*  �޸�ʱ�䣺2012-1-25    �Ѳ���
//*  ��    ע�����ź���Ҫ�Լ���ʼ�������
//*************************************************************************/


//void PORTD_IRQHandler()
//{
//  if(PORTD_ISFR & 0x2000)            //PTD13�����жϣ��ɼ������ж�
//  { 
//    PORTD_ISFR  |= 0x2000;         //д1���жϱ�־λ
//    

//  }
//  
// 
//  
//}

///*************************************************************************
//*                             ������ӹ�����
//*
//*  �������ƣ�PORTA_IRQHandler
//*  ����˵����PORTA�˿��жϷ�����
//*  ����˵������
//*  �������أ���
//*  �޸�ʱ�䣺2012-1-25    �Ѳ���
//*  ��    ע�����ź���Ҫ�����Լ���ʼ�����޸�
//*************************************************************************/
//void PORTA_IRQHandler()
//{
//  uint8_t  n = 0;    //���ź�
//  n = 19;
//  if(PORTA_ISFR & (1 << n))         //PTB0�����ж�
//  {
//    PORTA_ISFR  |= (1 << n);        //д1���жϱ�־λ
//    
//  }
//  
//  
//}

/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�PORTC_IRQHandler
*  ����˵����PORTC�˿��жϷ�����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2016-8-25    �Ѳ���
*  ��    ע�����ź���Ҫ�����Լ���ʼ�����޸�
*************************************************************************/
void PORTC_IRQHandler(void)
{
  uint8_t  n = 0;    //���ź�
	uint8  status[1] ;
  n = 1;
	  if(PORTC_ISFR & (1 << n))         //PTC1�����ж�
  {
    PORTC_ISFR  |= (1 << n);        //д1���жϱ�־λ
  }
	 n = 2;
	  if(PORTC_ISFR & (1 << n))         //PTC1�����ж�
  {
		
    PORTC_ISFR  |= (1 << n);        //д1���жϱ�־λ
		spi_read(INT_SOURCE_FLAG,&status[0],1);
		if(status[0]&&0x01)
			FXAS21002_ReadData(&GYRO_FXAS);
  }
}
/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�PORTE_IRQHandler
*  ����˵����PORTE�˿��жϷ�����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2016-8-25    �Ѳ���
*  ��    ע�����ź���Ҫ�����Լ���ʼ�����޸�
*************************************************************************/
void PORTE_IRQHandler(void)
{
  uint8_t  n = 0;    //���ź�
	uint8  status ;
  n = 27;
	  if(PORTE_ISFR & (1 << n))         //PTE27�����ж�
  {
    PORTE_ISFR  |= (1 << n);        //д1���жϱ�־λ
		
    status =I2C_ReadAddr(KI2C0,MPL31152_ADD,MPL3115_INT_SOURCE);
		if(status&0x80){
			mpl_readdata();
//			Gyro_flag=1;
		}
		
  }
}
/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�PORTB_IRQHandler
*  ����˵����PORTB�˿��жϷ�����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-1-25    �Ѳ���
*  ��    ע�����ź���Ҫ�����Լ���ʼ�����޸�
*************************************************************************/
extern AccelSensor foxacc1;
extern MagSensor magccc ;
extern uint8 foxaccflag ,foxmagflag;
void PORTB_IRQHandler()
{
  uint8  n = 0;    //���ź�
  uint8  state0 , state1;

		n = 8;
  if(PORTB_ISFR & (1 << n))         //PTB0�����ж�
  {
    PORTB_ISFR  |= (1 << n);        //д1���жϱ�־λ
	}
	n = 9;
  if(PORTB_ISFR & (1 << n))         //PTB0�����ж�
  {
    PORTB_ISFR  |= (1 << n);        //д1���жϱ�־λ
		FXOS_SPI_ReadDate(0,&state0,1); 
		FXOS_SPI_ReadDate(0x32,&state1,1); 
		if((state0 &0x08)&&(state1 &0x08)){
      FXOS8700_ReadData(&foxacc1,&magccc);
		  foxaccflag = 1 ;
		  foxmagflag = 1 ;}
	 else if((state0 &0x08)){
			foxaccflag = 1 ;
		  FXOS8700_ReadDataACC(&foxacc1);
		}
		else if(state1 &0x08){
			foxmagflag = 1 ;
			FXOS8700_ReadDataMAG(&magccc);
		}
    
  }

}

///*************************************************************************
//*                             ������ӹ�����
//*
//*  �������ƣ�FTM0_IRQHandler
//*  ����˵����FTM0���벶׽�жϷ�����
//*  ����˵������
//*  �������أ���
//*  �޸�ʱ�䣺2016-8-18
//*  ��    ע�����ź���Ҫ�����Լ���ʼ�����޸ģ��ο����еĴ�������Լ��Ĺ���
//*************************************************************************/

void FTM0_IRQHandler(void)
{
	static CHndata chdata[8] = {0};
	uint8 n;
	
	
	
		n = 3 ;
	if(FTM0_STATUS&(1<<n)){
		FTM0_STATUS &= ~(1<<n );
	  chdata[4].chvalue[0] = FTM0_CNT ;  //�����ص�ǰֵ
		if(PTA6_INT == 0){
			if(chdata[4].chvalue[0] > chdata[4].chvalue[1])
				 rc_data.ch4 = chdata[4].chvalue[0] - chdata[4].chvalue[1];    //���㲶��ֵ
			else {
				 rc_data.ch4 = 0xffff +chdata[4].chvalue[0] - chdata[4].chvalue[1];   //���㲶��ֵ
			}
	}
		chdata[4].chvalue[1] = chdata[4].chvalue[0] ;   //�洢ֵ
	}
	n = 4 ;
	if(FTM0_STATUS&(1<<n)){
		FTM0_STATUS &= ~(1<<n );
	  chdata[0].chvalue[0] = FTM0_CNT ;  //�����ص�ǰֵ
		if(PTD4_INT == 0){
			if(chdata[0].chvalue[0] > chdata[0].chvalue[1])
				 rc_data.roll = chdata[0].chvalue[0] - chdata[0].chvalue[1];    //���㲶��ֵ
			else {
				 rc_data.roll = 0xffff +chdata[0].chvalue[0] - chdata[0].chvalue[1];   //���㲶��ֵ
			}
	}
		chdata[0].chvalue[1] = chdata[0].chvalue[0] ;   //�洢ֵ
	}
  
  n = 5 ;
	if(FTM0_STATUS&(1<<n)){
		FTM0_STATUS &= ~(1<<n );
	  chdata[1].chvalue[0] = FTM0_CNT ;  //�����ص�ǰֵ
		if(PTD5_INT == 0){
			if(chdata[1].chvalue[0] > chdata[1].chvalue[1])
				 rc_data.pitch = chdata[1].chvalue[0] - chdata[1].chvalue[1];    //���㲶��ֵ
			else {
				rc_data.pitch = 0xffff +chdata[1].chvalue[0] - chdata[1].chvalue[1];   //���㲶��ֵ
			}
			
	}
		chdata[1].chvalue[1] = chdata[1].chvalue[0] ;   //�洢ֵ
	}
	n = 6 ;
	if(FTM0_STATUS&(1<<n)){
		FTM0_STATUS &= ~(1<<n );
	  chdata[2].chvalue[0] = FTM0_CNT ;  //�����ص�ǰֵ
		if(PTD6_INT == 0){
			if(chdata[2].chvalue[0] > chdata[2].chvalue[1])
				rc_data.throttle = chdata[2].chvalue[0] - chdata[2].chvalue[1];    //���㲶��ֵ
			else {
				rc_data.throttle = 0xffff +chdata[2].chvalue[0] - chdata[2].chvalue[1];   //���㲶��ֵ
			}
	}
		chdata[2].chvalue[1] = chdata[2].chvalue[0] ;   //�洢ֵ
	}
	
	n = 7 ;
	if(FTM0_STATUS&(1<<n)){
		FTM0_STATUS &= ~(1<<n );
	  chdata[3].chvalue[0] = FTM0_CNT ;  //�����ص�ǰֵ
		if(PTD7_INT == 0){
			if(chdata[3].chvalue[0] > chdata[3].chvalue[1])
				rc_data.yaw = chdata[3].chvalue[0] - chdata[3].chvalue[1];    //���㲶��ֵ
			else {
				rc_data.yaw = 0xffff +chdata[3].chvalue[0] - chdata[3].chvalue[1];   //���㲶��ֵ
			}
	}
		chdata[3].chvalue[1] = chdata[3].chvalue[0] ;   //�洢ֵ
	}
	if(((8000<rc_data.throttle)&&(rc_data.throttle<8300))&&(rc_data.pitch<8300)&&(rc_data.roll<8300))
	{
		ARMED=1;
	}
	if(((8200<rc_data.throttle)&&(rc_data.throttle<8300))&&(rc_data.pitch>14600)&&(rc_data.roll>14600))
	{
		OffsetFlag=1;
	}
		if((rc_data.throttle<8300)&&(rc_data.pitch<8300)&&(rc_data.roll>14600))
	{
		ARMED=0;
	}


}

///*************************************************************************
//*                             ������ӹ�����
//*
//*  �������ƣ�FTM1_IRQHandler
//*  ����˵����FTM1���벶׽�жϷ�����
//*  ����˵������
//*  �������أ���
//*  �޸�ʱ�䣺2012-2-25
//*  ��    ע�����ź���Ҫ�����Լ���ʼ�����޸ģ��ο����еĴ�������Լ��Ĺ���
//*************************************************************************/
//void FTM1_IRQHandler()
//{
//  uint8_t s = FTM1_STATUS;             //��ȡ��׽�ͱȽ�״̬  All CHnF bits can be checked using only one read of STATUS.
//  uint8_t n;
//  FTM1_STATUS = 0x00;               //���жϱ�־λ
// 
//  n = 1;
//  if( s & (1 << n) )
//  {
//    FTM_CnSC_REG(FTM1_BASE_PTR, n) &= ~FTM_CnSC_CHIE_MASK; //��ֹ���벶׽�ж�
//    /*     �û�����       */
//    /*********************/
//    //�����������￪�����벶׽�ж�
//    //FTM_CnSC_REG(FTM1_BASE_PTR,n) |= FTM_CnSC_CHIE_MASK;  //�������벶׽�ж�
//  }
//  
//}


///*************************************************************************
//*                             ������ӹ�����
//*
//*  �������ƣ�DMA_CH4_Handler
//*  ����˵����DMAͨ��4���жϷ�����
//*  ����˵�����ǲɼ�����ͷ���ݣ�������λ����ͷAD���ݣ����Բɼ���300���㡣
//���ñ�־λ�ܹ���ʱ���ơ�
//*  �������أ���
//*  �޸�ʱ�䣺2012-3-18    �Ѳ���
//*  ��    ע��
//*************************************************************************/
//void DMA_CH4_Handler(void)
//{
//  DMA_IRQ_CLEAN(DMA_CH4) ;
//  DMA_IRQ_DIS(DMA_CH4);
//  DMA_DIS(DMA_CH4);
//}

/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�DMA0_IRQHandler
*  ����˵����DMAͨ��0���жϷ�����
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2012-3-18    �Ѳ���
*  ��    ע��
*************************************************************************/
void DMA0_IRQHandler(void)
{
	DMAflag |= 0x01 ;                    //ͨ��0�������
  DMA_IRQ_CLEAN(DMA_CH0);                             //���ͨ�������жϱ�־λ    (���������ٴν����ж�)
  DMA_IRQ_DIS(DMA_CH0);
  DMA_DIS(DMA_CH0);  

}

//volatile uint8_t LPT_INT_count = 0;
//void  LPT_Handler(void)
//{
//  LPTMR0_CSR |= LPTMR_CSR_TCF_MASK;   //���LPTMR�Ƚϱ�־
//  LPT_INT_count++;                    //�ж������1
//}
