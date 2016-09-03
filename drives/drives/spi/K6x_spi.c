/***********************************************************
 * 文件名       ：K6x_gpio.c
 * 说明         ：IO端口应用函数
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 *库版本        ：V1.0
 *时间          ：15.5.26
************************************************************/
#include "common.h"
#include "K6x_spi.h"

volatile  SPI_Type *SPIN[3]={SPI0_BASE_PTR,SPI1_BASE_PTR,SPI2_BASE_PTR}; //定义三个指针数组保存 SPIx 的地址


/*************************************************************************
*                             蓝宙电子科技有限公司
*
*  函数名称：spi_init
*  功能说明：sSPI初始化
*  参数说明：spin：     SPI通道号。
*            SPI_CFG  模式选择
*            baudKH     波特率
*  函数返回：无
*  修改时间：2015-5-15   已测试
*  备    注：苏州大学的例程
*************************************************************************/
//void spi_init(SPIn spin, K60_SPI_INIT_STRUCT_PTR  initStruct)

void spi_init(SPIn spin,SPI_CFG mode, uint32  baudKH)
{
  uint16 spiBRS,spiPRS;
  uint32 busclcok_kH ;
  uint32 spicout ;
  int32 temp0,temp1,temp2,tempmin;
  uint32 BRS[] = {2,4,6,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768};
  uint8  PRS[] = {2,3,5,7};
  busclcok_kH = BUSCLKkh/baudKH  ;
  tempmin = 0x7fff ;
  for(temp0 = 0 ; temp0 < 4 ;temp0 ++){
    
    spicout=  busclcok_kH/PRS[temp0] ;   
    for(temp1 = 0 ;temp1 < 0x10 ;temp1 ++){
      temp2 = (uint16)spicout - BRS[temp1] ;
      if(temp2 < 0) temp2 = -temp2 ;
      if(tempmin > temp2){
        tempmin = temp2 ;
        spiPRS = temp0 ;
        spiBRS = temp1 ;
      }
    }
  }
  //使能SPI模块时钟，配置SPI引脚功能
  if(spin == 0)
  {
    SIM_SCGC6 |= SIM_SCGC6_SPI0_MASK;
    port_init(SPI0_SIN,ALT2);                              //SIN
    port_init(SPI0_SOUT,PORT_PCR_DSE_MASK|ALT2);           //SOUT
    port_init(SPI0_SCK,PORT_PCR_DSE_MASK|ALT2);            //SCK
    port_init(SPI0_PCS0,PORT_PCR_DSE_MASK|ALT2);           //PCS0
    
//    PORTD_PCR0 = 0 | PORT_PCR_MUX(0x2) | PORT_PCR_DSE_MASK;//PCS0
//    PORTD_PCR1 = 0 | PORT_PCR_MUX(0x2) | PORT_PCR_DSE_MASK;//SCK
//    PORTD_PCR2 = 0 | PORT_PCR_MUX(0x2) | PORT_PCR_DSE_MASK;//SOUT
//    PORTD_PCR3 = 0 | PORT_PCR_MUX(0x2);                    //SIN
  }
  else if(spin == 1)
  {
    SIM_SCGC6 |= SIM_SCGC6_SPI1_MASK;
    port_init(SPI1_SIN,ALT2);                              //SIN
    port_init(SPI1_SOUT,PORT_PCR_DSE_MASK|ALT2);           //SOUT
    port_init(SPI1_SCK,PORT_PCR_DSE_MASK|ALT2);            //SCK
    port_init(SPI1_PCS0,PORT_PCR_DSE_MASK|ALT2);           //PCS0
    
//    PORTE_PCR1 = 0 | PORT_PCR_MUX(0x2) | PORT_PCR_DSE_MASK; //SOUT
//    PORTE_PCR2 = 0 | PORT_PCR_MUX(0x2) | PORT_PCR_DSE_MASK; //SCK
//    PORTE_PCR3 = 0 | PORT_PCR_MUX(0x2);                     //SIN
//    PORTE_PCR4 = 0 | PORT_PCR_MUX(0x2) | PORT_PCR_DSE_MASK; //PCS0
  }
  else if(spin == 2)
  {   
    SIM_SCGC3 |= SIM_SCGC3_SPI2_MASK;
    port_init(SPI2_SIN,ALT2);                              //SIN
    port_init(SPI2_SOUT,PORT_PCR_DSE_MASK|ALT2);           //SOUT
    port_init(SPI2_SCK,PORT_PCR_DSE_MASK|ALT2);            //SCK
    port_init(SPI2_PCS0,PORT_PCR_DSE_MASK|ALT2);           //PCS0
    
//    PORTD_PCR13 = 0 | PORT_PCR_MUX(0x2) | PORT_PCR_DSE_MASK; //SOUT
//    PORTD_PCR12 = 0 | PORT_PCR_MUX(0x2) | PORT_PCR_DSE_MASK; //SCK
//    PORTD_PCR14 = 0 | PORT_PCR_MUX(0x2);                     //SIN 
  }
    
  SPI_MCR_REG(SPIN[spin]) = 0 
             | SPI_MCR_CLR_TXF_MASK                                   //Clear the Tx FIFO counter.
             | SPI_MCR_CLR_RXF_MASK                                   //Clear the Rx FIFO counter.
             | SPI_MCR_PCSIS_MASK
             | SPI_MCR_HALT_MASK;
  if(mode == MASTER)
  {
    //主机模式
    SPI_MCR_REG(SPIN[spin]) |= SPI_MCR_MSTR_MASK;                    
                                 
    SPI_CTAR_REG(SPIN[spin],0) = 0
            //| SPI_CTAR_DBR_MASK                                     //双波特率 ，这里设 DBR=1，CPHA=1，PBR=00，得SCK Duty Cycle 为 50/50
            //| SPI_CTAR_CPOL_MASK                                    //时钟极性，1表示 SCK 不活跃状态为高电平,   NRF24L01 不活跃为低电平
            //| SPI_CTAR_CPHA_MASK                                    //数据在SCK上升沿改变（输出），在下降沿被捕捉（输入读取）。如果是0，则反之。  w25x16在上升沿读取数据；NRF24L01在上升沿读取数据
              | SPI_CTAR_PDT(3)                                       //延时因子为 PDT*2+1 ，这里PDT为3，即延时因子为7。PDT为2bit
              | SPI_CTAR_PBR(spiPRS)                         //波特率分频器 ，0~3 对应的分频值Prescaler为 2、3、5、7
              | SPI_CTAR_BR(spiBRS)                           //波特率计数器值 ,当BR<=3,分频Scaler 为 2*（BR+1） ，当BR>=3，分频Scaler 为 2^BR  。BR为4bit
                                                                      //SCK 波特率 = (fSYS/Prescaler) x [(1+DBR)/Scaler ]          fSYS 为 Bus clock
              | SPI_CTAR_FMSZ(0xF)                                   //每帧传输 7bit+1 ，即8bit   
            //| SPI_CTAR_LSBFE_MASK                                   //1为低位在前。
              | SPI_CTAR_CSSCK(2)                                     //
              |SPI_CTAR_PCSSCK(1)                                     //设置片选信号有效到时钟第一个边沿出现的延时的预分频值。tcsc延时预分频 2*x+1；
              |SPI_CTAR_PASC(1)
		          |SPI_CTAR_ASC(2)
		          ;      
  }
  else
  {
    //从机模式
    SPI_CTAR_SLAVE_REG(SPIN[spin],0) = 0 
              | SPI_CTAR_SLAVE_FMSZ(0x07)
              | SPI_CTAR_SLAVE_CPOL_MASK 
              | SPI_CTAR_SLAVE_CPHA_MASK; 
  }
 
  //清标志位
  SPI_SR_REG(SPIN[spin]) = (SPI_SR_EOQF_MASK                        //End of Queue Flag，发送队列空了，发送完毕
             | SPI_SR_TFUF_MASK                                     //Transmit FIFO Underflow Flag，传输FIFO下溢标志位，SPI为从机模式，Tx FIFO为空，而外部SPI主机模式启动传输，标志位就会置1，写1清0
             | SPI_SR_TFFF_MASK                                     //Transmit FIFO Fill Flag，传输FIFO满标志位。 写1或者DMA控制器发现传输FIFO满了就会清0。 0表示Tx FIFO满了
             | SPI_SR_RFOF_MASK                                     //Receive FIFO Overflow Flag，接收FIFO溢出标志位。
             | SPI_SR_RFDF_MASK);                                   //Receive FIFO Drain Flag，接收FIFO损耗标志位，写1或者DMA控制器发现传输FIFO空了就会清0。0表示Rx FIFO空

  SPI_MCR_REG(SPIN[spin]) &= ~SPI_MCR_HALT_MASK;                    //启动SPI传输。1为暂停，0为启动
}


/*************************************************************************
*                             蓝宙电子科技有限公司
*
*  函数名称：spi_wr_byte
*  功能说明：SPI发送接收一字节数据。
*  参数说明：spin：SPI通道号。
*            data：需要发送的数据。
*  函数返回：接收到的数据
*  修改时间：2015-5-15   已测试
*  备    注：苏州大学的例程
*************************************************************************/
uint8 spi_wr_byte(SPIn spin,uint16 data)
{
  uint8 temp=0xff;  
  SPI_TX_WAIT(spin);
  do
  {
    //清标志位
    SPI_SR_REG(SPIN[spin]) = (SPI_SR_EOQF_MASK
                            | SPI_SR_TFUF_MASK
                            | SPI_SR_TFFF_MASK
                            | SPI_SR_RFOF_MASK
                            | SPI_SR_RFDF_MASK
                            );

    //清FIFO计数器
    SPI_MCR_REG(SPIN[spin])    |=  (SPI_MCR_CLR_TXF_MASK        //Clear TX FIFO.写1清 Tx FIFO counter
                               |SPI_MCR_CLR_RXF_MASK            //Clear RX FIFO. 写1清 the Rx FIFO counter.
                               );
  }while( (SPI_SR_REG(SPIN[spin]) & SPI_SR_RFDF_MASK));         //如果 Rx FIFO 非空，则清FIFO.
 
  SPI_PUSHR_REG(SPIN[spin]) = 0 
                 | SPI_PUSHR_CTAS(0)
                 | SPI_PUSHR_EOQ_MASK                           //End Of Queue，1为 传输SPI最后的数据
                 | SPI_PUSHR_TXDATA(data)
	               |SPI_PUSHR_PCS(1)
	           //    |SPI_PUSHR_CONT_MASK 
	                  ; 
  
  SPI_EOQF_WAIT(spin);                                          //要及时把RX FIFO的东西清掉，不然这里就无限等待
  
  while( !(SPI_SR_REG(SPIN[spin]) & SPI_SR_RFDF_MASK));         //RFDF为1，Rx FIFO is not empty.
  temp = (uint8)SPI_POPR_REG(SPIN[spin]);                       //读取一次接收的数据          
  SPI_SR_REG(SPIN[spin]) |= SPI_SR_RFDF_MASK;
  return temp;
}
/*************************************************************************
*                             蓝宙电子科技有限公司
*
*  函数名称：spi_wr_stream
*  功能说明：SPI发送接收多字节数据。
*  参数说明：spin：SPI通道号。
*            sent_buff[]：需要发送的数据。
*            len：数据长度。
*  函数返回：接收到的数据
*  修改时间：2015-5-15   已测试
*  备    注：苏州大学的例程
*************************************************************************/
void spi_wr_stream(SPIn spin,uint8 sent_buff[],uint32 len,uint8 rec_buff[])
{
  uint32 i;
  if (NULL == rec_buff) 
  {
    for(i=0;i<len;i++)
      spi_wr_byte(spin, sent_buff[i]);
  }
  else
  {
    for(i=0;i<len;i++)
      rec_buff[i] = spi_wr_byte(spin, sent_buff[i]);
  }
}

//=========================================================================
//函数名称：spi_irq_EN
//函数参数：spin：SPI通道号。
//函数返回：无
//功能概要：使能SPI中断
//=========================================================================
void spi_irq_EN(SPIn spin)
{
  SPI_RSER_REG(SPIN[spin]) |=SPI_RSER_RFDF_RE_MASK;     //开启SPI接收中断 
  NVIC_EnableIRQ((IRQn_Type)(spin + SPI0_IRQn));				                //开接收引脚的IRQ中断
}

//=========================================================================
//函数名称：spi_irq_DIS
//函数参数：spin：SPI通道号。
//函数返回：无
//功能概要：关闭SPI中断
//=========================================================================
void spi_irq_DIS(SPIn spin)
{
  SPI_RSER_REG(SPIN[spin]) &=~SPI_RSER_RFDF_RE_MASK;    //关闭SPI接收中断  
  NVIC_DisableIRQ((IRQn_Type)(spin + SPI0_IRQn));			                //开接收引脚的IRQ中断
}
