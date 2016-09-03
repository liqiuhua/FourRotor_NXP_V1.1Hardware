/***********************************************************
 * 文件名       ：adc.c
 * 说明         ：adc初始化
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 * 库版本       ：V1.0
 *时间          ：15.5.26
************************************************************/
#include "common.h"
#include "K6x_adc.h"
#include "gpio_cfg.h"
#include  "port.h"


ADC_MemMapPtr ADCx[2] = {ADC0_BASE_PTR, ADC1_BASE_PTR}; //定义两个指针数组保存 ADCx 的地址
/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：adc_DMA_init
*  功能说明：AD初始化，使能时钟
*  参数说明：ADCn        模块号（ ADC0、 ADC1）
*  函数返回：无ao
*  备    注：参考苏州大学的例程
*************************************************************************/
void adc_DMA_init(ADCn adcn, ADC_Ch ch)
{
  
    adc_init(adcn, ch)  ;
    //初始化ADC默认配置
    ADC_CFG1_REG(ADCx[adcn])  = ADLPC_LOW
                                 | ADC_CFG1_ADIV(ADIV_4)
                                 | ADLSMP_SHORT
                                 | ADC_CFG1_MODE(0)                   //8位
                                 | ADC_CFG1_ADICLK(ADICLK_BUS);
    ADC_CFG2_REG(ADCx[adcn])  = (ch>>5) // MUXSEL_ADCB    //MUXSEL_ADCA
                                 | ADACKEN_ENABLED
                                 | ADHSC_HISPEED
                                 | ADC_CFG2_ADLSTS(ADLSTS_2) ;
    
    
   
    ADC_SC2_REG(ADCx[adcn]) = ADTRG_SW                       //硬件触发
                                | ACFE_DISABLED 
                                | ACFGT_GREATER 
                                | ACREN_DISABLED 
                                | DMAEN_DISABLED               //使能DMA DMAEN_ENABLED
                                | ADC_SC2_REFSEL(REFSEL_EXT);
    
    ADC_SC3_REG(ADCx[adcn])  = CAL_OFF 
                                 | ADCO_CONTINUOUS 
                                 | AVGE_DISABLED 
                                 | ADC_SC3_AVGS(AVGS_4);
  
    ADC_SC1_REG(ADCx[adcn],ch>>5) = AIEN_ON | DIFF_SINGLE |      //
                                 ADC_SC1_ADCH( ch );
 
}

/*************************************************************************
*                             蓝宙电子科技有限公司
*
*  函数名称：adc_init
*  功能说明：初始化PIN为AD模式
*  参数说明：ADCn      AD模块（AD0，AD1)
*            ADC_Ch    AD通道（1 2 3 ......)
*  函数返回：无
*  修改时间：2012-5-15   已测试
*  备    注：
*************************************************************************/
void adc_init(ADCn adcn, ADC_Ch ch)
{

    switch(adcn)
    {
    case KADC0:       /*   ADC0  */
        SIM_SCGC6 |= (SIM_SCGC6_ADC0_MASK );        //开启ADC0时钟
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
*                             蓝宙电子工作室
*
*  函数名称：ad_once
*  功能说明：采集一次一路模拟量的AD值
*  参数说明：ADCn        模块号（ ADC0、 ADC1）
*            ADC_Channel 通道号
*            ADC_nbit    精度（ ADC_8bit,ADC_12bit, ADC_10bit, ADC_16bit ）
*  函数返回：无符号结果值
*  修改时间：2012-2-10
*  备    注：参考苏州大学的例程
*************************************************************************/
uint16_t ad_once(ADCn adcn, ADC_Ch ch, ADC_nbit bit) //采集某路模拟量的AD值
{
    uint16_t result = 0;

    adc_start(adcn, ch, bit);	  //启动ADC转换

    while (( ADC_SC1_REG(ADCx[adcn], 0) & ADC_SC1_COCO_MASK ) != ADC_SC1_COCO_MASK);
    result = ADC_R_REG(ADCx[adcn], 0);
    ADC_SC1_REG(ADCx[adcn], 0) &= ~ADC_SC1_COCO_MASK;
    return result;
}

/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：ad_ave
*  功能说明：多次采样，取平均值
*  参数说明：ADCx        模块号（ ADC0、 ADC1）
*            ADC_Channel 通道号
*            ADC_nbit    精度（ ADC_8bit,ADC_12bit, ADC_10bit, ADC_16bit ）
*            N           均值滤波次数(范围:0~255)
*  函数返回：16位无符号结果值
*  修改时间：2012-2-10
*  备    注：修改苏州大学的例程
*************************************************************************/
uint16_t ad_ave(ADCn adcn, ADC_Ch ch, ADC_nbit bit, uint8_t N) //均值滤波
{
    uint32_t tmp = 0;
    uint8_t  i;

    for(i = 0; i < N; i++)
        tmp += ad_once(adcn, ch, bit);
    tmp = tmp / N;
    return (uint16_t)tmp;
}

/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：ad_SUM
*  功能说明：多次采样，取平均值
*  参数说明：ADCx        模块号（ ADC0、 ADC1）
*            ADC_Channel 通道号
*            ADC_nbit    精度（ ADC_8bit,ADC_12bit, ADC_10bit, ADC_16bit ）
*            N           均值滤波次数(范围:0~255)
*  函数返回：16位无符号结果值
*  修改时间：2012-2-10
*  备    注：修改苏州大学的例程
*************************************************************************/
uint16_t ad_sum(ADCn adcn, ADC_Ch ch, ADC_nbit bit, uint8_t N) //采样集合
{
    uint32_t tmp = 0;
    uint8_t  i;
    adc_start(adcn, ch, bit);	  //启动ADC转换
    
    for(i = 0; i < N; i++)
    {
      while (( ADC_SC1_REG(ADCx[adcn], 0 ) & ADC_SC1_COCO_MASK ) != ADC_SC1_COCO_MASK);
      tmp += ADC_R_REG(ADCx[adcn], 0);
      ADC_SC1_REG(ADCx[adcn], 0) &= ~ADC_SC1_COCO_MASK;
    }
    return (uint16_t)tmp;
}

/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：adc_start
*  功能说明：启动adc软件采样，B通道不能用于软件触发！！！！
*  参数说明：ADCx        模块号（ ADC0、 ADC1）
*            ADC_Channel 通道号
*            ADC_nbit    精度（ ADC_8bit,ADC_12bit, ADC_10bit, ADC_16bit ）
*  函数返回：无
*  修改时间：2012-2-10
*  备    注：修改苏州大学的例程
*************************************************************************/
void adc_start(ADCn adcn, ADC_Ch ch, ADC_nbit bit)
{


    //初始化ADC默认配置
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
*                             蓝宙电子工作室
*
*  函数名称：adc_stop
*  功能说明：停止ADC转换
*  参数说明：ADCx        模块号（ ADC0、 ADC1）
*            ADC_Channel 通道号
*  函数返回：无
*  修改时间：2012-2-10
*  备    注：修改苏州大学的例程
*************************************************************************/
void adc_stop(ADCn adcn)
{
    ADC_SC1_REG(ADCx[adcn],0) = AIEN_ON | DIFF_SINGLE | ADC_SC1_ADCH(Module_disabled);
    ADC_SC1_REG(ADCx[adcn],1) = AIEN_ON | DIFF_SINGLE | ADC_SC1_ADCH(Module_disabled);
}





