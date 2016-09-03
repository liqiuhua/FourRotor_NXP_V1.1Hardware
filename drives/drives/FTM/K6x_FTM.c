/***********************************************************
 * 文件名       ：FTM.c
 * 说明         ：FTM定时器，目前有 PWM、输入捕捉功能
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 *库版本        ：V1.0
 *时间          ：15.5.26
************************************************************/

#include "common.h"
#include  "K6x_FTM.h"

 FTM_Type *FTMx[4] = {FTM0_BASE_PTR, FTM1_BASE_PTR, FTM2_BASE_PTR,FTM3_BASE_PTR}; //定义三个指针数组保存 FTMn 的地址




/*************************************************************************
*                            蓝宙电子工作室
*
*  函数名称：FTM_PWM_init
*  功能说明：初始化FTM的PWM功能并设置频率、占空比。设置通道输出占空比。
*  参数说明：FTMn        模块号（FTM0、  FTM1、  FTM2）
*            CHn         通道号（CH0~CH7）
*            freq        频率（单位为Hz）
*            duty        占空比
*  函数返回：无
*  修改时间：2012-2-14
*  备    注：同一个FTM，各通道的PWM频率是一样的，共3个FTM，即可以输出3个不同频率PWM
*************************************************************************/
void FTM_PWM_init(FTMn ftmn, CHn ch, uint32 freq, uint32 duty)
{
    uint32 clk_hz = (BUSCLKkh * 1000) >> 1;       //bus频率/2  Gbuscl_KH = 50000
    uint16 mod;
    uint8 sc_ps;
    uint16 cv;


    /*       计算分频因子       */
    mod = (clk_hz >> 16 ) / freq ;
    for(sc_ps = 0; (mod >> sc_ps) >= 1; sc_ps++);
    if(freq < 1000)sc_ps++;
    
    mod = (clk_hz >> sc_ps) / freq;                        //频率设置因子,clk_hz = 25000000
    /* 
    uart_putchar(UART0,mod>>24); 
    uart_putchar(UART0,mod>>16); 
    uart_putchar(UART0,mod>>8); 
    uart_putchar(UART0,mod>>0); */
   
    cv = (duty * (mod - 0 + 1)) / FTM_PRECISON;            //占空比设置因子

    switch(ftmn)
    {
    case SFTM0:
        SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;                           //使能FTM0时钟
        switch(ch)
        {
        case CH0:
            if(FTM0_CH0 == PTC1)
            {
                port_alt(FTM0_CH0,ALT4) ;
            }
            else if(FTM0_CH0 == PTA3)
            {
                port_alt(FTM0_CH0,ALT3) ;
            }
            break;

        case CH1:
            if(FTM0_CH1 == PTC2)
            {
                port_alt(FTM0_CH1,ALT4) ;
            }
            else if(FTM0_CH1 == PTA4)
            {
                port_alt(FTM0_CH1,ALT3) ;
            }
            break;

        case CH2:
            if(FTM0_CH2 == PTC3)
            {
                port_alt(FTM0_CH2,ALT4) ;
            }
            else if(FTM0_CH2 == PTA5)
            {
                port_alt(FTM0_CH2,ALT3) ;
            }
            break;

        case CH3:
            if(FTM0_CH3 == PTC4)
            {
                port_alt(FTM0_CH3,ALT4) ;
            }
            else if(FTM0_CH3 == PTA6)
            {
                port_alt(FTM0_CH3,ALT3) ;
            }
            break;

        case CH4:
            if(FTM0_CH4 == PTD4)
            {
                port_alt(FTM0_CH4,ALT4) ;
            }
            else if(FTM0_CH4 == PTA7)
            {
                port_alt(FTM0_CH4,ALT3) ;
            }
            break;

        case CH5:
            if(FTM0_CH5 == PTD5)
            {
                port_alt(FTM0_CH5,ALT4) ;
            }
            else if(FTM0_CH5 == PTA0)
            {
                port_alt(FTM0_CH5,ALT4) ;
            }
            break;

        case CH6:
            if(FTM0_CH6 == PTD6)
            {
               port_alt(FTM0_CH6,ALT4) ;
            }
            else if(FTM0_CH6 == PTA1)
            {
                port_alt(FTM0_CH6,ALT4) ;
            }
            break;

        case CH7:
            if(FTM0_CH7 == PTD7)
            {
                port_alt(FTM0_CH7,ALT4) ;
            }
            else if(FTM0_CH7 == PTA2)
            {
                port_alt(FTM0_CH7,ALT3) ;
            }
            break;
        default:
            return;
        }
        break;

    case SFTM1:
        SIM_SCGC6 |= SIM_SCGC6_FTM1_MASK;                           //使能FTM1时钟
        switch(ch)
        {
        case CH0:
          port_alt(FTM1_CH0,ALT3) ;
          break;


        case CH1:
          port_alt(FTM1_CH1,ALT3) ;           
          break;

        default:
            return;
        }
        break;
    case SFTM2:
			  SIM_SCGC6 |= SIM_SCGC6_FTM2_MASK; 
        SIM_SCGC3 |= SIM_SCGC3_FTM2_MASK;                           //使能FTM2时钟
        switch(ch)
        {
        case CH0:
          port_alt(FTM2_CH0,ALT3) ;
          break;

        case CH1:
          port_alt(FTM2_CH1,ALT3) ;
          break;

        default:
            return;
        }
        break;
		case SFTM3:
        SIM_SCGC3 |= SIM_SCGC3_FTM3_MASK;                           //使能FTM0时钟
        switch(ch)
        {
        case CH0:
                port_alt(FTM3_CH0,ALT6) ;
            break;

        case CH1:
								port_alt(FTM3_CH1,ALT6) ;
            break;

        case CH2:
            port_alt(FTM3_CH2,ALT6) ;
            break;

        case CH3:
            port_alt(FTM3_CH3,ALT6) ;
            break;

        case CH4:
            port_alt(FTM3_CH4,ALT6) ;
            break;

        case CH5:
            port_alt(FTM3_CH5,ALT6) ;
            break;

        case CH6:
            port_alt(FTM3_CH6,ALT6) ;
            break;

        case CH7:
            port_alt(FTM3_CH7,ALT6) ;
            break;
        default:
            return;
        }
        break;
    default:
        break;
    }
    /******************** 选择输出模式为 边沿对齐PWM *******************/
    //通道状态控制，根据模式来选择 边沿或电平
    FTM_CnSC_REG(FTMx[ftmn], ch) &= ~FTM_CnSC_ELSA_MASK;
    FTM_CnSC_REG(FTMx[ftmn], ch) = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;
    //MSnB:MSnA = 1x
    // ELSnB:ELSnA = 10   High-true pulses  (clear Output on match)
    // ELSnB:ELSnA = 11   Low-true pulses   (set Output on match)
    // Edge-aligned PWM  边沿对齐PWM波   《k16 reference manual.pdf》  P944  或者  《K60P144M100SF2RM.pdf》P1001


    /******************** 配置时钟和分频 ********************/
    FTM_SC_REG(FTMx[ftmn])    = FTM_SC_CPWMS_MASK | FTM_SC_PS(sc_ps) | FTM_SC_CLKS(1);  //没有使能溢出中断
    FTM_CNTIN_REG(FTMx[ftmn]) = 0;                                                      // Channel (n) Value  。设置脉冲宽度：(CnV - CNTIN).
    FTM_MOD_REG(FTMx[ftmn])   = mod;                                                    //Modulo value模数, EPWM的周期为 ：MOD - CNTIN + 0x0001
    FTM_CNTIN_REG(FTMx[ftmn]) = 0;                                                      //Counter Initial Value 计数器初始化值
    FTM_CnV_REG(FTMx[ftmn], ch) = cv;
    FTM_CNT_REG(FTMx[ftmn])   = 0;                                                      //计数器。只有低16位可用

}




/*************************************************************************
*                            蓝宙电子工作室
*
*  函数名称：FTM_PWM_Duty
*  功能说明：设置通道占空比
*  参数说明：FTMn        模块号（FTM0、  FTM1、  FTM2）
*            CHn         通道号（CH0~CH7）
*            duty        占空比
*  函数返回：无
*  修改时间：2012-2-14
*  备    注：
*************************************************************************/
void FTM_PWM_Duty(FTMn ftmn, CHn ch, uint32 duty)
{
    uint32 cv;
    uint32 mod;

    //占空比 = (CnV-CNTIN)/(MOD-CNTIN+1)
    mod = FTM_MOD_REG(FTMx[ftmn]);        //读取 MOD 的值

    cv = (duty * (mod - 0 + 1)) / FTM_PRECISON;

    // 配置FTM通道值
    FTM_CnV_REG(FTMx[ftmn], ch) = cv;

}


/*************************************************************************
*                            蓝宙电子工作室
*
*  函数名称：FTM_PWM_freq
*  功能说明：设置FTM的频率
*  参数说明：FTMn        模块号（FTM0、  FTM1、  FTM2）
*            freq        频率（单位为Hz）
*  函数返回：无
*  修改时间：2012-2-14
*  备    注：同一个FTM，各通道的PWM频率是一样的，共3个FTM，即可以输出3个不同频率PWM
*************************************************************************/
void FTM_PWM_freq(FTMn ftmn, uint32 freq)             //设置FTM的频率
{
    uint32 clk_hz = (BUSCLKkh * 1000) >> 1;        //bus频率/2
    uint32 mod;
    uint8 sc_ps;

    /*       计算频率设置        */
    mod = (clk_hz >> 16 ) / freq ;
    for(sc_ps = 0; (mod >> sc_ps) >= 1; sc_ps++);
    mod = (clk_hz >> sc_ps) / freq;


    /******************** 配置时钟和分频 ********************/
    FTM_SC_REG(FTMx[ftmn])    = FTM_SC_CPWMS_MASK | FTM_SC_PS(sc_ps) | FTM_SC_CLKS(1);  //没有使能溢出中断
    FTM_CNTIN_REG(FTMx[ftmn]) = 0;                                                      // Channel (n) Value  。设置脉冲宽度：(CnV - CNTIN).
    FTM_MOD_REG(FTMx[ftmn])   = mod;                                                    //Modulo value模数, EPWM的周期为 ：MOD - CNTIN + 0x0001
    FTM_CNTIN_REG(FTMx[ftmn]) = 0;                                                      //Counter Initial Value 计数器初始化值
    FTM_CNT_REG(FTMx[ftmn])   = 0;                                                      //计数器。只有低16位可用
}






/*************************************************************************
*                            蓝宙电子工作室
*
*  函数名称：FTM_Input_init
*  功能说明：输入捕捉初始化函数
*  参数说明：FTMn        模块号（FTM0、  FTM1、  FTM2）
*            CHn         通道号（CH0~CH7）
*            Input_cfg   输入捕捉配置（Rising、Falling、Rising_or_Falling）上升沿捕捉、下降沿捕捉、跳变沿捕捉
*  函数返回：无
*  修改时间：2012-1-26
*  备    注：CH0~CH3可以使用过滤器，未添加这功能
*************************************************************************/
void FTM_Input_init(FTMn ftmn, CHn ch, Input_cfg cfg)
{

    switch(ftmn)
    {
    case SFTM0:
        SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;                           //使能FTM0时钟
        switch(ch)
        {
        case CH0:
            if(FTM0_CH0 == PTC1)
            {
                port_alt(FTM0_CH0,ALT4) ;
            }
            else if(FTM0_CH0 == PTA3)
            {
                port_alt(FTM0_CH0,ALT3) ;
            }
            break;

        case CH1:
            if(FTM0_CH1 == PTC2)
            {
                port_alt(FTM0_CH1,ALT4) ;
            }
            else if(FTM0_CH1 == PTA4)
            {
                port_alt(FTM0_CH1,ALT3) ;
            }
            break;

        case CH2:
            if(FTM0_CH2 == PTC3)
            {
                port_alt(FTM0_CH2,ALT4) ;
            }
            else if(FTM0_CH2 == PTA5)
            {
                port_alt(FTM0_CH2,ALT3) ;
            }
            break;

        case CH3:
            if(FTM0_CH3 == PTC4)
            {
                port_alt(FTM0_CH3,ALT4) ;
            }
            else if(FTM0_CH3 == PTA6)
            {
                port_alt(FTM0_CH3,ALT3) ;
            }
            break;

        case CH4:
            if(FTM0_CH4 == PTD4)
            {
                port_alt(FTM0_CH4,ALT4) ;
            }
            else if(FTM0_CH4 == PTA7)
            {
                port_alt(FTM0_CH4,ALT3) ;
            }
            break;

        case CH5:
            if(FTM0_CH5 == PTD5)
            {
                port_alt(FTM0_CH5,ALT4) ;
            }
            else if(FTM0_CH5 == PTA0)
            {
                port_alt(FTM0_CH5,ALT4) ;
            }
            break;

        case CH6:
            if(FTM0_CH6 == PTD6)
            {
               port_alt(FTM0_CH6,ALT4) ;
            }
            else if(FTM0_CH6 == PTA1)
            {
                port_alt(FTM0_CH6,ALT4) ;
            }
            break;

        case CH7:
            if(FTM0_CH7 == PTD7)
            {
                port_alt(FTM0_CH7,ALT4) ;
            }
            else if(FTM0_CH7 == PTA2)
            {
                port_alt(FTM0_CH7,ALT3) ;
            }
            break;
        default:
            return;
        }
        break;

    case SFTM1:
        SIM_SCGC6 |= SIM_SCGC6_FTM1_MASK;                           //使能FTM1时钟
        switch(ch)
        {
        case CH0:
          port_alt(FTM1_CH0,ALT3) ;
          break;


        case CH1:
          port_alt(FTM1_CH1,ALT3) ;           
          break;

        default:
            return;
        }
        break;
    case SFTM2:
        SIM_SCGC3 |= SIM_SCGC3_FTM2_MASK;                           //使能FTM2时钟
        switch(ch)
        {
        case CH0:
          port_alt(FTM2_CH0,ALT3) ;
          break;

        case CH1:
          port_alt(FTM2_CH1,ALT3) ;
          break;

        default:
            return;
        }
        break;
		case SFTM3:
        SIM_SCGC3 |= SIM_SCGC3_FTM3_MASK;                           //使能FTM2时钟
        switch(ch)
        {
        case CH6:
          port_alt(FTM3_CH6,ALT6) ;
          break;
        case CH7:
          port_alt(FTM3_CH7,ALT6) ;
          break;

        default:
            return;
        }
        break;
    default:
        break;
    }

    switch(cfg)
    {
  case Rising:    //上升沿触发
        FTM_CnSC_REG(FTMx[ftmn], ch) |=  ( FTM_CnSC_ELSA_MASK  | FTM_CnSC_CHIE_MASK );                   //置1
        FTM_CnSC_REG(FTMx[ftmn], ch) &= ~( FTM_CnSC_ELSB_MASK  | FTM_CnSC_MSB_MASK | FTM_CnSC_MSA_MASK); //清0
        break;

    case Falling:   //下降沿触发
        FTM_CnSC_REG(FTMx[ftmn], ch) |= (FTM_CnSC_ELSB_MASK  | FTM_CnSC_CHIE_MASK );                    //置1
        FTM_CnSC_REG(FTMx[ftmn], ch) &= ~( FTM_CnSC_ELSA_MASK | FTM_CnSC_MSB_MASK | FTM_CnSC_MSA_MASK); //清0
        break;

    case Rising_or_Falling: //上升沿、下降沿都触发
        FTM_CnSC_REG(FTMx[ftmn], ch) |=  ( FTM_CnSC_ELSB_MASK | FTM_CnSC_ELSA_MASK  | FTM_CnSC_CHIE_MASK ); //置1
        FTM_CnSC_REG(FTMx[ftmn], ch) &= ~( FTM_CnSC_MSB_MASK  | FTM_CnSC_MSA_MASK); //清0
        break;
    }

    FTM_SC_REG(FTMx[ftmn]) = FTM_SC_CLKS(0x1)+ FTM_SC_PS(3);       //System clock

    FTM_MODE_REG(FTMx[ftmn]) |= FTM_MODE_WPDIS_MASK;
    FTM_COMBINE_REG(FTMx[ftmn]) = 0;
    FTM_MODE_REG(FTMx[ftmn]) &= ~FTM_MODE_FTMEN_MASK;
    FTM_CNTIN_REG(FTMx[ftmn]) = 0;

    FTM_STATUS_REG(FTMx[ftmn]) = 0x00;               //清中断标志位

    //开启输入捕捉中断
		if(ftmn<3)
			NVIC_EnableIRQ((IRQn_Type)(FTM0_IRQn+ ftmn));
		else
			NVIC_EnableIRQ((IRQn_Type)(FTM3_IRQn));
}

/*************************************************************************
*                            蓝宙电子工作室
*
*  函数名称：FTM_QUAD_init
*  功能说明：正交解码模式 初始化函数
*  参数说明：FTMn        模块号（ FTM1、  FTM2）
*            CHn         通道号（CH0~CH1）
*  函数返回：无
*  修改时间：2014-6-25
*  备    注：CH0~CH3可以使用过滤器，未添加这功能
*************************************************************************/
void FTM_QUAD_init(FTMn ftmn,CHn ch)
{
  switch(ftmn)
  {
  case SFTM1:
    SIM_SCGC6|=SIM_SCGC6_FTM1_MASK;                             //使能FTM1时钟
    switch(ch)
    {
    case CH0:
      port_alt(FTM1_CH0,ALT6) ;
      break;
    
    case CH1:
      port_alt(FTM1_CH1,ALT6) ;
      break;
    default:
      return;
    }
    break;
  case SFTM2:
    SIM_SCGC3 |= SIM_SCGC3_FTM2_MASK;                           //使能FTM2时钟
    switch(ch)
    {
    case CH0:
      port_alt(FTM2_CH0,ALT6) ;
      break;
      
    default:
      return;
    }break;
  default:
    break;
  }
 // FTM_QDCTRL_QUADIR_MASK
  FTM_MODE_REG(FTMx[ftmn]) |= FTM_MODE_WPDIS_MASK|FTM_MODE_FTMEN_MASK;
  FTM_QDCTRL_REG(FTMx[ftmn]) &= ~FTM_QDCTRL_QUADMODE_MASK  ;//AB相同时确定方向和计数值 
  FTM_CNTIN_REG(FTMx[ftmn])=0;//FTM0计数器初始值为0  
  FTM_MOD_REG(FTMx[ftmn])=65535;//结束值  
  FTM_QDCTRL_REG(FTMx[ftmn]) |=FTM_QDCTRL_QUADEN_MASK;//启用FTMn正交解码模式  
  FTM_MODE_REG(FTMx[ftmn]) |= FTM_MODE_FTMEN_MASK;   //FTMnEN=1    
  FTM_CNT_REG(FTMx[ftmn])=0; 
  
  FTM_SC_REG(FTMx[ftmn]) |= FTM_SC_CLKS(3);  //选择外部时钟
  
}
/*************************************************************************
*                            蓝宙电子工作室
*
*  函数名称：FTM_SpeedMeasure_init
*  功能说明：正交解码模式 初始化函数
*  参数说明：FTMn        模块号（ FTM1、 FTM2）
*            CHn         通道号（CH0~CH1）
*  函数返回：无
*  修改时间：2014-6-25
*  备    注：CH0~CH3可以使用过滤器，未添加这功能
*************************************************************************/
void FTM_SpeedMeasure_init(void)
{
  FTM_QUAD_init(SFTM1,CH0);
  FTM_QUAD_init(SFTM1,CH1);
  FTM_QUAD_init(SFTM2,CH0);
  FTM_QUAD_init(SFTM2,CH1);
}


