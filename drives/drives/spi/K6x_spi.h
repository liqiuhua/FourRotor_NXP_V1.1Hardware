/***********************************************************
 * �ļ���       ��K6x_gpio.c
 * ˵��         ��IO�˿�Ӧ�ú���
 * ����         ��landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://bbs.landzo.com
 *��汾        ��V1.0
 *ʱ��          ��15.5.26
************************************************************/


#ifndef _K6X_SPI_H_
#define _K6X_SPI_H_
#include "common.h"

#ifndef NULL
  #define NULL 0u
#endif
extern volatile  SPI_Type *SPIN[3];

//�������ӻ�ģʽ
typedef enum SPI_CFG
{
  MASTER,    
  SLAVE      
}SPI_CFG;

//����SPIģ���
typedef enum SPIn
{        //   PCS0      SCK      SOUT      SIN    |   PCS1     PCS2    PCS3    PCS4    PCS5
    KSPI0,//   PTA14     PTA15    PTA16     PTA17  |   
         //   PTC4      PTC5     PTC6      PTC7   |   PTC3     PTC2    PTC1    PTC0    PTB23
         //   PTD0      PTD1     PTD2      PTD3   |   PTD4     PTD5    PTD6
         
    KSPI1,//   PTE4      PTE2     PTE1      PTE3   |   PTE0     PTE5    PTE6
         //   PTB10     PTB11    PTB16     PTB17  |   PTB9
         
    KSPI2 //   PTB20     PTB21    PTB22     PTB23  |
         //   PTD11     PTD12    PTD13     PTD14  |   PTD15
}SPIn;


typedef struct k60_spi_init_struct
{  
   /* SPI ����ģʽ���� */
   uint16 TRANSFER_MODE;
   /*SPI���������ã�SCK ������ = (fSYS/Prescaler) x [(1+DBR)/Scaler ]��fSYS Ϊ Bus clock*/
   uint8 PBR;
   uint8 BR;
   
} K60_SPI_INIT_STRUCT, *K60_SPI_INIT_STRUCT_PTR;

#define SPI_TX_WAIT(SPIn)     while(  ( SPI_SR_REG(SPIN[SPIn]) & SPI_SR_TXRXS_MASK ) == 1 )     //�ȴ����� ���
#define SPI_RX_WAIT(SPIn)     while(  ( SPI_SR_REG(SPIN[SPIn]) & SPI_SR_RFDF_MASK ) == 0 )      //�ȴ����� FIFOΪ�ǿ�
#define SPI_EOQF_WAIT(SPIn)   while(  (SPI_SR_REG(SPIN[SPIn]) & SPI_SR_EOQF_MASK ) == 0 )       //�ȴ��������

//void spi_init(SPIn spin,K60_SPI_INIT_STRUCT_PTR  initStruct);
void spi_init(SPIn spin,SPI_CFG mode, uint32  baudKH) ;
uint8 spi_wr_byte(SPIn spin,uint16 data);
void spi_wr_stream(SPIn spin,uint8 sent_buff[],uint32 len,uint8 rec_buff[]);
void spi_irq_EN(SPIn);
void spi_irq_DIS(SPIn);

#endif  // _SPI_H_
