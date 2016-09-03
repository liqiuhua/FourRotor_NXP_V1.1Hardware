/***********************************************************
 * 文件名       ：adc.h
 * 说明         ：ADC通过初始化
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 * 库版本       ：V1.0
 *时间          ：15.5.26
************************************************************/


#ifndef __K6x_ADC_H__
#define __K6x_ADC_H__ 

typedef enum ADCn  //ADC端口
{
    KADC0,
    KADC1
} ADCn;

//ADC通道
typedef enum ADC_Ch
{
    DAD0 = 0, //	    ADC0_DP0				                    ADC1_DP0
    DAD1 = 1, //	    ADC0_DP1				                    ADC1_DP1
    DAD2 = 2, //	    PGA0_DP2					            PGA1_DP2
    DAD3 = 3, //	    ADC0_DP3				                    ADC1_DP3

    //ADCx_CFG2[MUXSEL] 位决定 ADCx_SEn 通道为 a 或 b.
    AD4a = 4, //	    保留					           ADC1_SE4a   -- PTE0
    AD5a = 5, //	    保留					           ADC1_SE5a   -- PTE1
    AD6a = 6, //	    保留					           ADC1_SE6a   -- PTE2
    AD7a = 7, //	    保留					           ADC1_SE7a   -- PTE3

    //也是 4、5、6、7
    AD4b = 0x24, //	    ADC0_SE4b	-- PTC2			                ADC1_SE4b   -- PTC8
    AD5b = 0x25, //	    ADC0_SE5b	-- PTD1			                ADC1_SE5b   -- PTC9
    AD6b = 0x26, //	    ADC0_SE6b	-- PTD5			                ADC1_SE6b   -- PTC10
    AD7b = 0x27, //	    ADC0_SE7b	-- PTD6			                ADC1_SE7b   -- PTC11

    AD8 = 8,  //	    ADC0_SE8  	-- PTB0			                ADC1_SE8    -- PTB0
    AD9 = 9,  //	    ADC0_SE9  	-- PTB1			                ADC1_SE9    -- PTB1
    AD10 = 10, //	    ADC0_SE10 	-- PTA7			                ADC1_SE10   -- PTB4
    AD11 = 11, //	    ADC0_SE11 	-- PTA8			                ADC1_SE11   -- PTB5
    AD12 = 12, //	    ADC0_SE12 	-- PTB2			                ADC1_SE12   -- PTB6
    AD13 = 13, //	    ADC0_SE13 	-- PTB3			                ADC1_SE13   -- PTB7
    AD14 = 14, //	    ADC0_SE14 	-- PTC0			                ADC1_SE14   -- PTB10
    AD15 = 15, //	    ADC0_SE15 	-- PTC1			                ADC1_SE15   -- PTB11
    AD16 = 16, //	    ADC0_SE16				                    ADC1_SE16
    AD17 = 17, //	    ADC0_SE17 	-- PTE24		                ADC1_SE17   -- PTA17
    AD18 = 18, //	    ADC0_SE18 	-- PTE25		                VREF Output
    AD19 = 19, //	    ADC0_DM0				                    ADC1_DM0
    AD20 = 20, //	    ADC0_DM1				                    ADC1_DM1
    AD21 = 21, //				        	                   
    AD22 = 22, //
    AD23 = 23, //	    DAC0_OUT(12-bit) -- DAC0_OUT		        DAC1_OUT(12-bit)
    AD24 = 24, //	    保留					保留
    AD25 = 25, //	    保留					保留
    AD26 = 26, //	    Temperature Sensor (S.E)		            Temperature Sensor (S.E)
    AD27 = 27, //	    Bandgap (S.E)				                Bandgap (S.E)
    AD28 = 28, //	    保留					                    保留
    AD29 = 29, //	    VREFH (S.E)				                    VREFH (S.E)
    AD30 = 30, //	    VREFL					                    VREFL
    AD31 = 31 //	    禁用ADC0				                    禁用ADC1
} ADC_Ch;

//精度位数
typedef enum ADC_nbit
{
    ADC_8bit   = 0x00,
    ADC_10bit  = 0x02,
    ADC_12bit  = 0x01,
    ADC_16bit  = 0x03
} ADC_nbit;


#define Module_disabled    AD31         //   禁用模块
// Conversion Complete (COCO) mask
#define COCO_COMPLETE     ADC_SC1_COCO_MASK
#define COCO_NOT          0x00

// ADC interrupts: enabled, or disabled.
// ADC 中断: 使能或者禁止
#define AIEN_ON           ADC_SC1_AIEN_MASK
#define AIEN_OFF          0x00

// Differential or Single ended ADC input
//查分或者单端ADC输入
#define DIFF_SINGLE       0x00
#define DIFF_DIFFERENTIAL ADC_SC1_DIFF_MASK

// Power setting of ADC
//ADC电源设置
#define ADLPC_LOW         ADC_CFG1_ADLPC_MASK
#define ADLPC_NORMAL      0x00

// Clock divisor
//时钟分频
#define ADIV_1            0x00
#define ADIV_2            0x01
#define ADIV_4            0x02
#define ADIV_8            0x03

// Long samle time, or Short sample time
// 长采样时间或者短采样时间
#define ADLSMP_LONG       ADC_CFG1_ADLSMP_MASK
#define ADLSMP_SHORT      0x00


// ADC Input Clock Source choice? Bus clock, Bus clock/2, "altclk", or the
//                                ADC's own asynchronous clock for less noise
//ADC输入时钟源选择 总线，总线/2，”altclk“或者ADC自身异步时钟以减少噪声
#define ADICLK_BUS        0x00
#define ADICLK_BUS_2      0x01
#define ADICLK_ALTCLK     0x02
#define ADICLK_ADACK      0x03

//// ADCCFG2

// 选择通道A，通道B
#define MUXSEL_ADCB       ADC_CFG2_MUXSEL_MASK
#define MUXSEL_ADCA       0x00

// Ansync clock output enable: enable, or disable the output of it
// 异步时钟输出使能：使能，或者禁止输出
#define ADACKEN_ENABLED   ADC_CFG2_ADACKEN_MASK
#define ADACKEN_DISABLED  0x00

// High speed or low speed conversion mode
// 高速低速转换时间
#define ADHSC_HISPEED     ADC_CFG2_ADHSC_MASK
#define ADHSC_NORMAL      0x00

// Long Sample Time selector: 20, 12, 6, or 2 extra clocks for a longer sample time
// 长采样时间选择：20,12,6或者2个额外的时钟对于长采样时间
#define ADLSTS_20          0x00
#define ADLSTS_12          0x01
#define ADLSTS_6           0x02
#define ADLSTS_2           0x03


// Read-only status bit indicating conversion status
// 只读状态位只是转换状态
#define ADACT_ACTIVE       ADC_SC2_ADACT_MASK
#define ADACT_INACTIVE     0x00

// Trigger for starting conversion: Hardware trigger, or software trigger.
// For using PDB, the Hardware trigger option is selected.
// 触发开始转换:硬件触发，软件触发
#define ADTRG_HW           ADC_SC2_ADTRG_MASK
#define ADTRG_SW           0x00

// ADC Compare Function Enable: Disabled, or Enabled.
//ADC比较功能使能：禁止或者使能
#define ACFE_DISABLED      0x00
#define ACFE_ENABLED       ADC_SC2_ACFE_MASK

// Compare Function Greater Than Enable: Greater, or Less.
// 比较功能大于比较使能：大于或者小于
#define ACFGT_GREATER      ADC_SC2_ACFGT_MASK
#define ACFGT_LESS         0x00

// Compare Function Range Enable: Enabled or Disabled.
// 比较功能范围使能：使能或者禁止
#define ACREN_ENABLED      ADC_SC2_ACREN_MASK
#define ACREN_DISABLED     0x00

// DMA enable: enabled or disabled.
// DMA使能：使能或者禁止
#define DMAEN_ENABLED      ADC_SC2_DMAEN_MASK
#define DMAEN_DISABLED     0x00


//ADC转换的电压参考选择
#define REFSEL_EXT         0x00
#define REFSEL_ALT         0x01
#define REFSEL_RES         0x02     /* reserved */
#define REFSEL_RES_EXT     0x03     /* reserved but defaults to Vref */

////ADCSC3

// 校准开始或者关闭
#define CAL_BEGIN          ADC_SC3_CAL_MASK
#define CAL_OFF            0x00


// 指示校准失败成功的状态
#define CALF_FAIL          ADC_SC3_CALF_MASK
#define CALF_NORMAL        0x00

// ADC连续转换或者一次转换
#define ADCO_CONTINUOUS    ADC_SC3_ADCO_MASK
#define ADCO_SINGLE        0x00

// 平均采样使能或者禁止
#define AVGE_ENABLED       ADC_SC3_AVGE_MASK
#define AVGE_DISABLED      0x00

// MCU产生中断前的平均次数4,8,16，或者32
#define AVGS_4             0x00
#define AVGS_8             0x01
#define AVGS_16            0x02
#define AVGS_32            0x03



//外部函数接口声明
void          adc_init  (ADCn, ADC_Ch);              //AD初始化
uint16_t      ad_once   (ADCn, ADC_Ch, ADC_nbit);    //采集一次一路模拟量的AD值
uint16_t      ad_ave(ADCn adcn, ADC_Ch ch, ADC_nbit bit, uint8_t N); //均值滤波
uint16_t      ad_sum(ADCn adcn, ADC_Ch ch, ADC_nbit bit, uint8_t N);
void          adc_start (ADCn, ADC_Ch, ADC_nbit);    //开始adc转换
void          adc_stop  (ADCn);                      //停止ADC转换

void adc_DMA_init(ADCn adcn, ADC_Ch ch) ;


#endif /* __ADC16_H__ */
