/***********************************************************
 * �ļ���       ��adc.c
 * ˵��         ��adc��ʼ��
 * ����         ��landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://www.landzo.cn
 * ��汾       ��V1.0
 *ʱ��          ��15.5.26
************************************************************/
#include "common.h"
#include "K6x_adc.h"
#include "gpio_cfg.h"
#include  "port.h"


ADC_MemMapPtr ADCx[2] = {ADC0_BASE_PTR, ADC1_BASE_PTR}; //��������ָ�����鱣�� ADCx �ĵ�ַ
/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�adc_DMA_init
*  ����˵����AD��ʼ����ʹ��ʱ��
*  ����˵����ADCn        ģ��ţ� ADC0�� ADC1��
*  �������أ���ao
*  ��    ע���ο����ݴ�ѧ������
*************************************************************************/
void adc_DMA_init(ADCn adcn, ADC_Ch ch)
{
  
    adc_init(adcn, ch)  ;
    //��ʼ��ADCĬ������
    ADC_CFG1_REG(ADCx[adcn])  = ADLPC_LOW
                                 | ADC_CFG1_ADIV(ADIV_4)
                                 | ADLSMP_SHORT
                                 | ADC_CFG1_MODE(0)                   //8λ
                                 | ADC_CFG1_ADICLK(ADICLK_BUS);
    ADC_CFG2_REG(ADCx[adcn])  = (ch>>5) // MUXSEL_ADCB    //MUXSEL_ADCA
                                 | ADACKEN_ENABLED
                                 | ADHSC_HISPEED
                                 | ADC_CFG2_ADLSTS(ADLSTS_2) ;
    
    
   
    ADC_SC2_REG(ADCx[adcn]) = ADTRG_SW                       //Ӳ������
                                | ACFE_DISABLED 
                                | ACFGT_GREATER 
                                | ACREN_DISABLED 
                                | DMAEN_DISABLED               //ʹ��DMA DMAEN_ENABLED
                                | ADC_SC2_REFSEL(REFSEL_EXT);
    
    ADC_SC3_REG(ADCx[adcn])  = CAL_OFF 
                                 | ADCO_CONTINUOUS 
                                 | AVGE_DISABLED 
                                 | ADC_SC3_AVGS(AVGS_4);
  
    ADC_SC1_REG(ADCx[adcn],ch>>5) = AIEN_ON | DIFF_SINGLE |      //
                                 ADC_SC1_ADCH( ch );
 
}

/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�adc_init
*  ����˵������ʼ��PINΪADģʽ
*  ����˵����ADCn      ADģ�飨AD0��AD1)
*            ADC_Ch    ADͨ����1 2 3 ......)
*  �������أ���
*  �޸�ʱ�䣺2012-5-15   �Ѳ���
*  ��    ע��
*************************************************************************/
void adc_init(ADCn adcn, ADC_Ch ch)
{

    switch(adcn)
    {
    case KADC0:       /*   ADC0  */
        SIM_SCGC6 |= (SIM_SCGC6_ADC0_MASK );        //����ADC0ʱ��
        SIM_SOPT7 &= ~(SIM_SOPT7_ADC0ALTTRGEN_MASK  | SIM_SOPT7_ADC0PRETRGSEL_MASK);
        SIM_SOPT7 = SIM_SOPT7_ADC0TRGSEL(0);

        switch(ch)
        {
          case AD4b:  //ADC0_SE4b-- PTC2
            port_alt(PTC2,ALT0);
            break;
          case AD5b:  //ADC0_SE5b -- PTD1
            port_alt(PTD1,ALT0);
            break;
          case AD6b:  //ADC0_SE6b -- PTD5
            port_alt(PTD5,ALT0);
            break;
          case AD7b:  //ADC0_SE7b -- PTD6
            port_alt(PTD6,ALT0);
            break;
          case AD8:   //ADC0_SE8 -- PTB0
            port_alt(PTB0,ALT0);
            break;
          case AD9:   //ADC0_SE9 -- PTB1
            port_alt(PTB1,ALT0);
            break;
          case AD10:  //ADC0_SE10 -- PTA7
            port_alt(PTA7,ALT0);
            break;
          case AD11:  //ADC0_SE11 -- PTA8
            port_alt(PTA8,ALT0);
            break;
          case AD12:  //ADC0_SE12 -- PTB2
            port_alt(PTB2,ALT0);
            break;
          case AD13:  //ADC0_SE13 -- PTB3
            port_alt(PTB3,ALT0);
            break;
          case AD14:  //ADC0_SE14 -- PTC0
            port_alt(PTC0,ALT0);
            break;
          case AD15:  //ADC0_SE15 -- PTC1
            port_alt(PTC1,ALT0);
            break;
          case AD17:   //ADC0_SE17 -- PTE24
            port_alt(PTE24,ALT0);
            break;
          case AD18:   //ADC0_SE17 -- PTE25
            port_alt(PTE25,ALT0);
            break;
          
          default:
              return;
        }
        return;

    case KADC1:       /*   ADC1    */
        SIM_SCGC3 |= (SIM_SCGC3_ADC1_MASK );
        SIM_SOPT7 &= ~(SIM_SOPT7_ADC1ALTTRGEN_MASK  | SIM_SOPT7_ADC1PRETRGSEL_MASK) ;
        SIM_SOPT7 = SIM_SOPT7_ADC1TRGSEL(0);

        switch(ch)
        {
          case AD4a:   //ADC1_SE4a   -- PTE0
            port_alt(PTE0,ALT0);
            break;
          case AD5a:   //ADC1_SE5a   -- PTE1
            port_alt(PTE1,ALT0);
            break;
          case AD6a:   //ADC1_SE6a   -- PTE2
            port_alt(PTE2,ALT0);
            break;
          case AD7a:   //ADC1_SE7a   -- PTE3
            port_alt(PTE3,ALT0);
            break;
          case AD4b:   //ADC1_SE4b   -- PTC8
            port_alt(PTC8,ALT0);
            break;
          case AD5b:   //ADC1_SE5b   -- PTC9
            port_alt(PTC9,ALT0);
            break;
          case AD6b:   //ADC1_SE6b   -- PTC10
            port_alt(PTC10,ALT0);
            break;
          case AD7b:   //ADC1_SE7b   -- PTC11
            port_alt(PTC11,ALT0);
            break;
          case AD8:  //ADC1_SE8 -- PTB0
            port_alt(PTB0,ALT0);
            break;
          case AD9:  //ADC1_SE9 -- PTB1
            port_alt(PTB1,ALT0);
            break;
          case AD10:  //ADC1_SE10 -- PTB4
            port_alt(PTB4,ALT0);
            break;
          case AD11:  //ADC1_SE11 -- PTB5
            port_alt(PTB5,ALT0);
            break;
          case AD12:  //ADC1_SE12 -- PTB6
            port_alt(PTB6,ALT0);
            break;
          case AD13:  //ADC1_SE13 -- PTB7
            port_alt(PTB7,ALT0);
            break;
          case AD14:  //ADC1_SE14 -- PTB10
            port_alt(PTC10,ALT0);
            break;
          case AD15:  //ADC1_SE15 -- PTB11
            port_alt(PTB11,ALT0);
            break;
          case AD17:  //ADC1_SE17 -- PTA17
            port_alt(PTB17,ALT0);
            break;
          default:
            break;
        }
        break;
    default:
        break;
    }
}


/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�ad_once
*  ����˵�����ɼ�һ��һ·ģ������ADֵ
*  ����˵����ADCn        ģ��ţ� ADC0�� ADC1��
*            ADC_Channel ͨ����
*            ADC_nbit    ���ȣ� ADC_8bit,ADC_12bit, ADC_10bit, ADC_16bit ��
*  �������أ��޷��Ž��ֵ
*  �޸�ʱ�䣺2012-2-10
*  ��    ע���ο����ݴ�ѧ������
*************************************************************************/
uint16_t ad_once(ADCn adcn, ADC_Ch ch, ADC_nbit bit) //�ɼ�ĳ·ģ������ADֵ
{
    uint16_t result = 0;

    adc_start(adcn, ch, bit);	  //����ADCת��

    while (( ADC_SC1_REG(ADCx[adcn], 0) & ADC_SC1_COCO_MASK ) != ADC_SC1_COCO_MASK);
    result = ADC_R_REG(ADCx[adcn], 0);
    ADC_SC1_REG(ADCx[adcn], 0) &= ~ADC_SC1_COCO_MASK;
    return result;
}

/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�ad_ave
*  ����˵������β�����ȡƽ��ֵ
*  ����˵����ADCx        ģ��ţ� ADC0�� ADC1��
*            ADC_Channel ͨ����
*            ADC_nbit    ���ȣ� ADC_8bit,ADC_12bit, ADC_10bit, ADC_16bit ��
*            N           ��ֵ�˲�����(��Χ:0~255)
*  �������أ�16λ�޷��Ž��ֵ
*  �޸�ʱ�䣺2012-2-10
*  ��    ע���޸����ݴ�ѧ������
*************************************************************************/
uint16_t ad_ave(ADCn adcn, ADC_Ch ch, ADC_nbit bit, uint8_t N) //��ֵ�˲�
{
    uint32_t tmp = 0;
    uint8_t  i;

    for(i = 0; i < N; i++)
        tmp += ad_once(adcn, ch, bit);
    tmp = tmp / N;
    return (uint16_t)tmp;
}

/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�ad_SUM
*  ����˵������β�����ȡƽ��ֵ
*  ����˵����ADCx        ģ��ţ� ADC0�� ADC1��
*            ADC_Channel ͨ����
*            ADC_nbit    ���ȣ� ADC_8bit,ADC_12bit, ADC_10bit, ADC_16bit ��
*            N           ��ֵ�˲�����(��Χ:0~255)
*  �������أ�16λ�޷��Ž��ֵ
*  �޸�ʱ�䣺2012-2-10
*  ��    ע���޸����ݴ�ѧ������
*************************************************************************/
uint16_t ad_sum(ADCn adcn, ADC_Ch ch, ADC_nbit bit, uint8_t N) //��������
{
    uint32_t tmp = 0;
    uint8_t  i;
    adc_start(adcn, ch, bit);	  //����ADCת��
    
    for(i = 0; i < N; i++)
    {
      while (( ADC_SC1_REG(ADCx[adcn], 0 ) & ADC_SC1_COCO_MASK ) != ADC_SC1_COCO_MASK);
      tmp += ADC_R_REG(ADCx[adcn], 0);
      ADC_SC1_REG(ADCx[adcn], 0) &= ~ADC_SC1_COCO_MASK;
    }
    return (uint16_t)tmp;
}

/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�adc_start
*  ����˵��������adc���������Bͨ�������������������������
*  ����˵����ADCx        ģ��ţ� ADC0�� ADC1��
*            ADC_Channel ͨ����
*            ADC_nbit    ���ȣ� ADC_8bit,ADC_12bit, ADC_10bit, ADC_16bit ��
*  �������أ���
*  �޸�ʱ�䣺2012-2-10
*  ��    ע���޸����ݴ�ѧ������
*************************************************************************/
void adc_start(ADCn adcn, ADC_Ch ch, ADC_nbit bit)
{


    //��ʼ��ADCĬ������
    ADC_CFG1_REG(ADCx[adcn])  = ADLPC_NORMAL
                                 | ADC_CFG1_ADIV(ADIV_4)
                                 | ADLSMP_LONG
                                 | ADC_CFG1_MODE(bit)
                                 | ADC_CFG1_ADICLK(ADICLK_BUS);
    ADC_CFG2_REG(ADCx[adcn])  =  ((ch>>5)  << ADC_CFG2_MUXSEL_SHIFT)   //MUXSEL_ADCB    //MUXSEL_ADCA
                                 | ADACKEN_DISABLED
                                 | ADHSC_HISPEED
                                 | ADC_CFG2_ADLSTS(ADLSTS_20) ;
    ADC_SC1_REG(ADCx[adcn],0) = AIEN_ON | DIFF_SINGLE | ADC_SC1_ADCH( ch );
   
}

/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�adc_stop
*  ����˵����ֹͣADCת��
*  ����˵����ADCx        ģ��ţ� ADC0�� ADC1��
*            ADC_Channel ͨ����
*  �������أ���
*  �޸�ʱ�䣺2012-2-10
*  ��    ע���޸����ݴ�ѧ������
*************************************************************************/
void adc_stop(ADCn adcn)
{
    ADC_SC1_REG(ADCx[adcn],0) = AIEN_ON | DIFF_SINGLE | ADC_SC1_ADCH(Module_disabled);
    ADC_SC1_REG(ADCx[adcn],1) = AIEN_ON | DIFF_SINGLE | ADC_SC1_ADCH(Module_disabled);
}





