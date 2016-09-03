/***********************************************************
 * 文件名       ：isr.c
 * 说明         ：中断功能模块
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 * 库版本       ：V1.0
 *时间          ：15.5.26
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
//*                             蓝宙电子工作室
//*
//*  函数名称：setIRQadrr
//*  功能说明：设置中断地址函数
//*  参数说明：IRQInterruptIndex 中断向量表，在芯片的.H文件，比如K60 在"MK60DZ10.h"
//*            void IRQAdrr(void)  中断函数
//*  函数返回：无
//*  修改时间：2015-5-15    已测试
//*  备    注：
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
//  中断向量表在ram区域，可以进行直接擦除
//  **************/
//  if(Vector_TAB == (uint32)&__VECTOR_RAM){   
//    Vector_TABl[IRQnum] = (uint32)(IRQAdrr) ;
//   }
//  else{
//    /*****************************
//    向量表在flash中
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
*                             蓝宙电子工作室
*
*  函数名称：USART0_IRQHandler
*  功能说明：串口0 中断 接收 服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2014-8-28    已测试
*  备    注：
*************************************************************************/
#define USART_REC_LEN  40
uint8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
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
//*                             蓝宙电子工作室
//*
//*  函数名称：USART1_IRQHandler                                                                                                                          
//*  功能说明：串口1 中断 接收 服务函数
//*  参数说明：无
//*  函数返回：无
//*  修改时间：2012-2-14    已测试
//*  备    注：
//*************************************************************************/


//void USART1_IRQHandler(void)
//{
//    uint8 ch;

//    DisableInterrupts;		    //关总中断

//    //接收一个字节数据并回发
//    ch = uart_getchar (UART1);      //接收到一个数据
//    uart_sendStr  (UART1, "\n你发送的数据为：");
//    uart_putchar (UART1, ch);       //就发送出去

//    EnableInterrupts;		    //开总中断
//}
///*************************************************************************
//*                             蓝宙嵌入式开发工作室
//*
//*  函数名称：PIT1_IRQHandler
//*  功能说明：PIT1定时中断服务函数
//*  参数说明：无
//*  函数返回：无
//*  修改时间：2012-9-18    已测试
//*  备    注：
//*************************************************************************/

void PIT1_IRQHandler(void)
{
  PIT_Flag_Clear(PIT1);       //清中断标志位
  //PTE28_OUT=~PTE28_OUT;
  
}

/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：PIT0_IRQHandler
*  功能说明：PIT0 定时中断服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2012-2-18    已测试
*  备    注：
*************************************************************************/



void PIT0_IRQHandler(void)
{
  PIT_Flag_Clear(PIT0);       //清中断标志位
  
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
*                             蓝宙电子工作室
*
*  函数名称：HardFault_Handler
*  功能说明：硬件上访中断服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2012-2-4    已测试
*  备    注：可以用LED闪烁来指示发生了硬件上访
*************************************************************************/
void HardFault_Handler(void)
{
  while (1)
  {
    //printf("\n****硬件上访错误!!!*****\r\n\n");
  }
}

///*************************************************************************
//*                             蓝宙电子工作室
//*
//*  函数名称：PORTD_IRQHandler
//*  功能说明：PORTD端口中断服务函数
//*  参数说明：无
//*  函数返回：无
//*  修改时间：2012-1-25    已测试
//*  备    注：引脚号需要自己初始化来清除
//*************************************************************************/


//void PORTD_IRQHandler()
//{
//  if(PORTD_ISFR & 0x2000)            //PTD13触发中断，采集的行中断
//  { 
//    PORTD_ISFR  |= 0x2000;         //写1清中断标志位
//    

//  }
//  
// 
//  
//}

///*************************************************************************
//*                             蓝宙电子工作室
//*
//*  函数名称：PORTA_IRQHandler
//*  功能说明：PORTA端口中断服务函数
//*  参数说明：无
//*  函数返回：无
//*  修改时间：2012-1-25    已测试
//*  备    注：引脚号需要根据自己初始化来修改
//*************************************************************************/
//void PORTA_IRQHandler()
//{
//  uint8_t  n = 0;    //引脚号
//  n = 19;
//  if(PORTA_ISFR & (1 << n))         //PTB0触发中断
//  {
//    PORTA_ISFR  |= (1 << n);        //写1清中断标志位
//    
//  }
//  
//  
//}

/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：PORTC_IRQHandler
*  功能说明：PORTC端口中断服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2016-8-25    已测试
*  备    注：引脚号需要根据自己初始化来修改
*************************************************************************/
void PORTC_IRQHandler(void)
{
  uint8_t  n = 0;    //引脚号
	uint8  status[1] ;
  n = 1;
	  if(PORTC_ISFR & (1 << n))         //PTC1触发中断
  {
    PORTC_ISFR  |= (1 << n);        //写1清中断标志位
  }
	 n = 2;
	  if(PORTC_ISFR & (1 << n))         //PTC1触发中断
  {
		
    PORTC_ISFR  |= (1 << n);        //写1清中断标志位
		spi_read(INT_SOURCE_FLAG,&status[0],1);
		if(status[0]&&0x01)
			FXAS21002_ReadData(&GYRO_FXAS);
  }
}
/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：PORTE_IRQHandler
*  功能说明：PORTE端口中断服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2016-8-25    已测试
*  备    注：引脚号需要根据自己初始化来修改
*************************************************************************/
void PORTE_IRQHandler(void)
{
  uint8_t  n = 0;    //引脚号
	uint8  status ;
  n = 27;
	  if(PORTE_ISFR & (1 << n))         //PTE27触发中断
  {
    PORTE_ISFR  |= (1 << n);        //写1清中断标志位
		
    status =I2C_ReadAddr(KI2C0,MPL31152_ADD,MPL3115_INT_SOURCE);
		if(status&0x80){
			mpl_readdata();
//			Gyro_flag=1;
		}
		
  }
}
/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：PORTB_IRQHandler
*  功能说明：PORTB端口中断服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2012-1-25    已测试
*  备    注：引脚号需要根据自己初始化来修改
*************************************************************************/
extern AccelSensor foxacc1;
extern MagSensor magccc ;
extern uint8 foxaccflag ,foxmagflag;
void PORTB_IRQHandler()
{
  uint8  n = 0;    //引脚号
  uint8  state0 , state1;

		n = 8;
  if(PORTB_ISFR & (1 << n))         //PTB0触发中断
  {
    PORTB_ISFR  |= (1 << n);        //写1清中断标志位
	}
	n = 9;
  if(PORTB_ISFR & (1 << n))         //PTB0触发中断
  {
    PORTB_ISFR  |= (1 << n);        //写1清中断标志位
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
//*                             蓝宙电子工作室
//*
//*  函数名称：FTM0_IRQHandler
//*  功能说明：FTM0输入捕捉中断服务函数
//*  参数说明：无
//*  函数返回：无
//*  修改时间：2016-8-18
//*  备    注：引脚号需要根据自己初始化来修改，参考现有的代码添加自己的功能
//*************************************************************************/

void FTM0_IRQHandler(void)
{
	static CHndata chdata[8] = {0};
	uint8 n;
	
	
	
		n = 3 ;
	if(FTM0_STATUS&(1<<n)){
		FTM0_STATUS &= ~(1<<n );
	  chdata[4].chvalue[0] = FTM0_CNT ;  //跳变沿当前值
		if(PTA6_INT == 0){
			if(chdata[4].chvalue[0] > chdata[4].chvalue[1])
				 rc_data.ch4 = chdata[4].chvalue[0] - chdata[4].chvalue[1];    //计算捕获值
			else {
				 rc_data.ch4 = 0xffff +chdata[4].chvalue[0] - chdata[4].chvalue[1];   //计算捕获值
			}
	}
		chdata[4].chvalue[1] = chdata[4].chvalue[0] ;   //存储值
	}
	n = 4 ;
	if(FTM0_STATUS&(1<<n)){
		FTM0_STATUS &= ~(1<<n );
	  chdata[0].chvalue[0] = FTM0_CNT ;  //跳变沿当前值
		if(PTD4_INT == 0){
			if(chdata[0].chvalue[0] > chdata[0].chvalue[1])
				 rc_data.roll = chdata[0].chvalue[0] - chdata[0].chvalue[1];    //计算捕获值
			else {
				 rc_data.roll = 0xffff +chdata[0].chvalue[0] - chdata[0].chvalue[1];   //计算捕获值
			}
	}
		chdata[0].chvalue[1] = chdata[0].chvalue[0] ;   //存储值
	}
  
  n = 5 ;
	if(FTM0_STATUS&(1<<n)){
		FTM0_STATUS &= ~(1<<n );
	  chdata[1].chvalue[0] = FTM0_CNT ;  //跳变沿当前值
		if(PTD5_INT == 0){
			if(chdata[1].chvalue[0] > chdata[1].chvalue[1])
				 rc_data.pitch = chdata[1].chvalue[0] - chdata[1].chvalue[1];    //计算捕获值
			else {
				rc_data.pitch = 0xffff +chdata[1].chvalue[0] - chdata[1].chvalue[1];   //计算捕获值
			}
			
	}
		chdata[1].chvalue[1] = chdata[1].chvalue[0] ;   //存储值
	}
	n = 6 ;
	if(FTM0_STATUS&(1<<n)){
		FTM0_STATUS &= ~(1<<n );
	  chdata[2].chvalue[0] = FTM0_CNT ;  //跳变沿当前值
		if(PTD6_INT == 0){
			if(chdata[2].chvalue[0] > chdata[2].chvalue[1])
				rc_data.throttle = chdata[2].chvalue[0] - chdata[2].chvalue[1];    //计算捕获值
			else {
				rc_data.throttle = 0xffff +chdata[2].chvalue[0] - chdata[2].chvalue[1];   //计算捕获值
			}
	}
		chdata[2].chvalue[1] = chdata[2].chvalue[0] ;   //存储值
	}
	
	n = 7 ;
	if(FTM0_STATUS&(1<<n)){
		FTM0_STATUS &= ~(1<<n );
	  chdata[3].chvalue[0] = FTM0_CNT ;  //跳变沿当前值
		if(PTD7_INT == 0){
			if(chdata[3].chvalue[0] > chdata[3].chvalue[1])
				rc_data.yaw = chdata[3].chvalue[0] - chdata[3].chvalue[1];    //计算捕获值
			else {
				rc_data.yaw = 0xffff +chdata[3].chvalue[0] - chdata[3].chvalue[1];   //计算捕获值
			}
	}
		chdata[3].chvalue[1] = chdata[3].chvalue[0] ;   //存储值
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
//*                             蓝宙电子工作室
//*
//*  函数名称：FTM1_IRQHandler
//*  功能说明：FTM1输入捕捉中断服务函数
//*  参数说明：无
//*  函数返回：无
//*  修改时间：2012-2-25
//*  备    注：引脚号需要根据自己初始化来修改，参考现有的代码添加自己的功能
//*************************************************************************/
//void FTM1_IRQHandler()
//{
//  uint8_t s = FTM1_STATUS;             //读取捕捉和比较状态  All CHnF bits can be checked using only one read of STATUS.
//  uint8_t n;
//  FTM1_STATUS = 0x00;               //清中断标志位
// 
//  n = 1;
//  if( s & (1 << n) )
//  {
//    FTM_CnSC_REG(FTM1_BASE_PTR, n) &= ~FTM_CnSC_CHIE_MASK; //禁止输入捕捉中断
//    /*     用户任务       */
//    /*********************/
//    //不建议在这里开启输入捕捉中断
//    //FTM_CnSC_REG(FTM1_BASE_PTR,n) |= FTM_CnSC_CHIE_MASK;  //开启输入捕捉中断
//  }
//  
//}


///*************************************************************************
//*                             蓝宙电子工作室
//*
//*  函数名称：DMA_CH4_Handler
//*  功能说明：DMA通道4的中断服务函数
//*  参数说明：是采集摄像头数据，本数据位摄像头AD数据，可以采集到300个点。
//设置标志位能够及时搬移。
//*  函数返回：无
//*  修改时间：2012-3-18    已测试
//*  备    注：
//*************************************************************************/
//void DMA_CH4_Handler(void)
//{
//  DMA_IRQ_CLEAN(DMA_CH4) ;
//  DMA_IRQ_DIS(DMA_CH4);
//  DMA_DIS(DMA_CH4);
//}

/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：DMA0_IRQHandler
*  功能说明：DMA通道0的中断服务函数
*  参数说明：
*  函数返回：无
*  修改时间：2012-3-18    已测试
*  备    注：
*************************************************************************/
void DMA0_IRQHandler(void)
{
	DMAflag |= 0x01 ;                    //通道0发送完成
  DMA_IRQ_CLEAN(DMA_CH0);                             //清除通道传输中断标志位    (这样才能再次进入中断)
  DMA_IRQ_DIS(DMA_CH0);
  DMA_DIS(DMA_CH0);  

}

//volatile uint8_t LPT_INT_count = 0;
//void  LPT_Handler(void)
//{
//  LPTMR0_CSR |= LPTMR_CSR_TCF_MASK;   //清除LPTMR比较标志
//  LPT_INT_count++;                    //中断溢出加1
//}
