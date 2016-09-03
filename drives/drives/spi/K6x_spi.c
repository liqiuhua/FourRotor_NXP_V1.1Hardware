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
#include "common.h"
#include "K6x_spi.h"

volatile  SPI_Type *SPIN[3]={SPI0_BASE_PTR,SPI1_BASE_PTR,SPI2_BASE_PTR}; //��������ָ�����鱣�� SPIx �ĵ�ַ


/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�spi_init
*  ����˵����sSPI��ʼ��
*  ����˵����spin��     SPIͨ���š�
*            SPI_CFG  ģʽѡ��
*            baudKH     ������
*  �������أ���
*  �޸�ʱ�䣺2015-5-15   �Ѳ���
*  ��    ע�����ݴ�ѧ������
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
  //ʹ��SPIģ��ʱ�ӣ�����SPI���Ź���
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
    //����ģʽ
    SPI_MCR_REG(SPIN[spin]) |= SPI_MCR_MSTR_MASK;                    
                                 
    SPI_CTAR_REG(SPIN[spin],0) = 0
            //| SPI_CTAR_DBR_MASK                                     //˫������ �������� DBR=1��CPHA=1��PBR=00����SCK Duty Cycle Ϊ 50/50
            //| SPI_CTAR_CPOL_MASK                                    //ʱ�Ӽ��ԣ�1��ʾ SCK ����Ծ״̬Ϊ�ߵ�ƽ,   NRF24L01 ����ԾΪ�͵�ƽ
            //| SPI_CTAR_CPHA_MASK                                    //������SCK�����ظı䣨����������½��ر���׽�������ȡ���������0����֮��  w25x16�������ض�ȡ���ݣ�NRF24L01�������ض�ȡ����
              | SPI_CTAR_PDT(3)                                       //��ʱ����Ϊ PDT*2+1 ������PDTΪ3������ʱ����Ϊ7��PDTΪ2bit
              | SPI_CTAR_PBR(spiPRS)                         //�����ʷ�Ƶ�� ��0~3 ��Ӧ�ķ�ƵֵPrescalerΪ 2��3��5��7
              | SPI_CTAR_BR(spiBRS)                           //�����ʼ�����ֵ ,��BR<=3,��ƵScaler Ϊ 2*��BR+1�� ����BR>=3����ƵScaler Ϊ 2^BR  ��BRΪ4bit
                                                                      //SCK ������ = (fSYS/Prescaler) x [(1+DBR)/Scaler ]          fSYS Ϊ Bus clock
              | SPI_CTAR_FMSZ(0xF)                                   //ÿ֡���� 7bit+1 ����8bit   
            //| SPI_CTAR_LSBFE_MASK                                   //1Ϊ��λ��ǰ��
              | SPI_CTAR_CSSCK(2)                                     //
              |SPI_CTAR_PCSSCK(1)                                     //����Ƭѡ�ź���Ч��ʱ�ӵ�һ�����س��ֵ���ʱ��Ԥ��Ƶֵ��tcsc��ʱԤ��Ƶ 2*x+1��
              |SPI_CTAR_PASC(1)
		          |SPI_CTAR_ASC(2)
		          ;      
  }
  else
  {
    //�ӻ�ģʽ
    SPI_CTAR_SLAVE_REG(SPIN[spin],0) = 0 
              | SPI_CTAR_SLAVE_FMSZ(0x07)
              | SPI_CTAR_SLAVE_CPOL_MASK 
              | SPI_CTAR_SLAVE_CPHA_MASK; 
  }
 
  //���־λ
  SPI_SR_REG(SPIN[spin]) = (SPI_SR_EOQF_MASK                        //End of Queue Flag�����Ͷ��п��ˣ��������
             | SPI_SR_TFUF_MASK                                     //Transmit FIFO Underflow Flag������FIFO�����־λ��SPIΪ�ӻ�ģʽ��Tx FIFOΪ�գ����ⲿSPI����ģʽ�������䣬��־λ�ͻ���1��д1��0
             | SPI_SR_TFFF_MASK                                     //Transmit FIFO Fill Flag������FIFO����־λ�� д1����DMA���������ִ���FIFO���˾ͻ���0�� 0��ʾTx FIFO����
             | SPI_SR_RFOF_MASK                                     //Receive FIFO Overflow Flag������FIFO�����־λ��
             | SPI_SR_RFDF_MASK);                                   //Receive FIFO Drain Flag������FIFO��ı�־λ��д1����DMA���������ִ���FIFO���˾ͻ���0��0��ʾRx FIFO��

  SPI_MCR_REG(SPIN[spin]) &= ~SPI_MCR_HALT_MASK;                    //����SPI���䡣1Ϊ��ͣ��0Ϊ����
}


/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�spi_wr_byte
*  ����˵����SPI���ͽ���һ�ֽ����ݡ�
*  ����˵����spin��SPIͨ���š�
*            data����Ҫ���͵����ݡ�
*  �������أ����յ�������
*  �޸�ʱ�䣺2015-5-15   �Ѳ���
*  ��    ע�����ݴ�ѧ������
*************************************************************************/
uint8 spi_wr_byte(SPIn spin,uint16 data)
{
  uint8 temp=0xff;  
  SPI_TX_WAIT(spin);
  do
  {
    //���־λ
    SPI_SR_REG(SPIN[spin]) = (SPI_SR_EOQF_MASK
                            | SPI_SR_TFUF_MASK
                            | SPI_SR_TFFF_MASK
                            | SPI_SR_RFOF_MASK
                            | SPI_SR_RFDF_MASK
                            );

    //��FIFO������
    SPI_MCR_REG(SPIN[spin])    |=  (SPI_MCR_CLR_TXF_MASK        //Clear TX FIFO.д1�� Tx FIFO counter
                               |SPI_MCR_CLR_RXF_MASK            //Clear RX FIFO. д1�� the Rx FIFO counter.
                               );
  }while( (SPI_SR_REG(SPIN[spin]) & SPI_SR_RFDF_MASK));         //��� Rx FIFO �ǿգ�����FIFO.
 
  SPI_PUSHR_REG(SPIN[spin]) = 0 
                 | SPI_PUSHR_CTAS(0)
                 | SPI_PUSHR_EOQ_MASK                           //End Of Queue��1Ϊ ����SPI��������
                 | SPI_PUSHR_TXDATA(data)
	               |SPI_PUSHR_PCS(1)
	           //    |SPI_PUSHR_CONT_MASK 
	                  ; 
  
  SPI_EOQF_WAIT(spin);                                          //Ҫ��ʱ��RX FIFO�Ķ����������Ȼ��������޵ȴ�
  
  while( !(SPI_SR_REG(SPIN[spin]) & SPI_SR_RFDF_MASK));         //RFDFΪ1��Rx FIFO is not empty.
  temp = (uint8)SPI_POPR_REG(SPIN[spin]);                       //��ȡһ�ν��յ�����          
  SPI_SR_REG(SPIN[spin]) |= SPI_SR_RFDF_MASK;
  return temp;
}
/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�spi_wr_stream
*  ����˵����SPI���ͽ��ն��ֽ����ݡ�
*  ����˵����spin��SPIͨ���š�
*            sent_buff[]����Ҫ���͵����ݡ�
*            len�����ݳ��ȡ�
*  �������أ����յ�������
*  �޸�ʱ�䣺2015-5-15   �Ѳ���
*  ��    ע�����ݴ�ѧ������
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
//�������ƣ�spi_irq_EN
//����������spin��SPIͨ���š�
//�������أ���
//���ܸ�Ҫ��ʹ��SPI�ж�
//=========================================================================
void spi_irq_EN(SPIn spin)
{
  SPI_RSER_REG(SPIN[spin]) |=SPI_RSER_RFDF_RE_MASK;     //����SPI�����ж� 
  NVIC_EnableIRQ((IRQn_Type)(spin + SPI0_IRQn));				                //���������ŵ�IRQ�ж�
}

//=========================================================================
//�������ƣ�spi_irq_DIS
//����������spin��SPIͨ���š�
//�������أ���
//���ܸ�Ҫ���ر�SPI�ж�
//=========================================================================
void spi_irq_DIS(SPIn spin)
{
  SPI_RSER_REG(SPIN[spin]) &=~SPI_RSER_RFDF_RE_MASK;    //�ر�SPI�����ж�  
  NVIC_DisableIRQ((IRQn_Type)(spin + SPI0_IRQn));			                //���������ŵ�IRQ�ж�
}
