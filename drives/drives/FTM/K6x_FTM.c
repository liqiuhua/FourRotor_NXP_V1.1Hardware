/***********************************************************
 * �ļ���       ��FTM.c
 * ˵��         ��FTM��ʱ����Ŀǰ�� PWM�����벶׽����
 * ����         ��landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://www.landzo.cn
 *��汾        ��V1.0
 *ʱ��          ��15.5.26
************************************************************/

#include "common.h"
#include  "K6x_FTM.h"

 FTM_Type *FTMx[4] = {FTM0_BASE_PTR, FTM1_BASE_PTR, FTM2_BASE_PTR,FTM3_BASE_PTR}; //��������ָ�����鱣�� FTMn �ĵ�ַ




/*************************************************************************
*                            ������ӹ�����
*
*  �������ƣ�FTM_PWM_init
*  ����˵������ʼ��FTM��PWM���ܲ�����Ƶ�ʡ�ռ�ձȡ�����ͨ�����ռ�ձȡ�
*  ����˵����FTMn        ģ��ţ�FTM0��  FTM1��  FTM2��
*            CHn         ͨ���ţ�CH0~CH7��
*            freq        Ƶ�ʣ���λΪHz��
*            duty        ռ�ձ�
*  �������أ���
*  �޸�ʱ�䣺2012-2-14
*  ��    ע��ͬһ��FTM����ͨ����PWMƵ����һ���ģ���3��FTM�����������3����ͬƵ��PWM
*************************************************************************/
void FTM_PWM_init(FTMn ftmn, CHn ch, uint32 freq, uint32 duty)
{
    uint32 clk_hz = (BUSCLKkh * 1000) >> 1;       //busƵ��/2  Gbuscl_KH = 50000
    uint16 mod;
    uint8 sc_ps;
    uint16 cv;


    /*       �����Ƶ����       */
    mod = (clk_hz >> 16 ) / freq ;
    for(sc_ps = 0; (mod >> sc_ps) >= 1; sc_ps++);
    if(freq < 1000)sc_ps++;
    
    mod = (clk_hz >> sc_ps) / freq;                        //Ƶ����������,clk_hz = 25000000
    /* 
    uart_putchar(UART0,mod>>24); 
    uart_putchar(UART0,mod>>16); 
    uart_putchar(UART0,mod>>8); 
    uart_putchar(UART0,mod>>0); */
   
    cv = (duty * (mod - 0 + 1)) / FTM_PRECISON;            //ռ�ձ���������

    switch(ftmn)
    {
    case SFTM0:
        SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;                           //ʹ��FTM0ʱ��
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
        SIM_SCGC6 |= SIM_SCGC6_FTM1_MASK;                           //ʹ��FTM1ʱ��
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
        SIM_SCGC3 |= SIM_SCGC3_FTM2_MASK;                           //ʹ��FTM2ʱ��
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
        SIM_SCGC3 |= SIM_SCGC3_FTM3_MASK;                           //ʹ��FTM0ʱ��
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
    /******************** ѡ�����ģʽΪ ���ض���PWM *******************/
    //ͨ��״̬���ƣ�����ģʽ��ѡ�� ���ػ��ƽ
    FTM_CnSC_REG(FTMx[ftmn], ch) &= ~FTM_CnSC_ELSA_MASK;
    FTM_CnSC_REG(FTMx[ftmn], ch) = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;
    //MSnB:MSnA = 1x
    // ELSnB:ELSnA = 10   High-true pulses  (clear Output on match)
    // ELSnB:ELSnA = 11   Low-true pulses   (set Output on match)
    // Edge-aligned PWM  ���ض���PWM��   ��k16 reference manual.pdf��  P944  ����  ��K60P144M100SF2RM.pdf��P1001


    /******************** ����ʱ�Ӻͷ�Ƶ ********************/
    FTM_SC_REG(FTMx[ftmn])    = FTM_SC_CPWMS_MASK | FTM_SC_PS(sc_ps) | FTM_SC_CLKS(1);  //û��ʹ������ж�
    FTM_CNTIN_REG(FTMx[ftmn]) = 0;                                                      // Channel (n) Value  �����������ȣ�(CnV - CNTIN).
    FTM_MOD_REG(FTMx[ftmn])   = mod;                                                    //Modulo valueģ��, EPWM������Ϊ ��MOD - CNTIN + 0x0001
    FTM_CNTIN_REG(FTMx[ftmn]) = 0;                                                      //Counter Initial Value ��������ʼ��ֵ
    FTM_CnV_REG(FTMx[ftmn], ch) = cv;
    FTM_CNT_REG(FTMx[ftmn])   = 0;                                                      //��������ֻ�е�16λ����

}




/*************************************************************************
*                            ������ӹ�����
*
*  �������ƣ�FTM_PWM_Duty
*  ����˵��������ͨ��ռ�ձ�
*  ����˵����FTMn        ģ��ţ�FTM0��  FTM1��  FTM2��
*            CHn         ͨ���ţ�CH0~CH7��
*            duty        ռ�ձ�
*  �������أ���
*  �޸�ʱ�䣺2012-2-14
*  ��    ע��
*************************************************************************/
void FTM_PWM_Duty(FTMn ftmn, CHn ch, uint32 duty)
{
    uint32 cv;
    uint32 mod;

    //ռ�ձ� = (CnV-CNTIN)/(MOD-CNTIN+1)
    mod = FTM_MOD_REG(FTMx[ftmn]);        //��ȡ MOD ��ֵ

    cv = (duty * (mod - 0 + 1)) / FTM_PRECISON;

    // ����FTMͨ��ֵ
    FTM_CnV_REG(FTMx[ftmn], ch) = cv;

}


/*************************************************************************
*                            ������ӹ�����
*
*  �������ƣ�FTM_PWM_freq
*  ����˵��������FTM��Ƶ��
*  ����˵����FTMn        ģ��ţ�FTM0��  FTM1��  FTM2��
*            freq        Ƶ�ʣ���λΪHz��
*  �������أ���
*  �޸�ʱ�䣺2012-2-14
*  ��    ע��ͬһ��FTM����ͨ����PWMƵ����һ���ģ���3��FTM�����������3����ͬƵ��PWM
*************************************************************************/
void FTM_PWM_freq(FTMn ftmn, uint32 freq)             //����FTM��Ƶ��
{
    uint32 clk_hz = (BUSCLKkh * 1000) >> 1;        //busƵ��/2
    uint32 mod;
    uint8 sc_ps;

    /*       ����Ƶ������        */
    mod = (clk_hz >> 16 ) / freq ;
    for(sc_ps = 0; (mod >> sc_ps) >= 1; sc_ps++);
    mod = (clk_hz >> sc_ps) / freq;


    /******************** ����ʱ�Ӻͷ�Ƶ ********************/
    FTM_SC_REG(FTMx[ftmn])    = FTM_SC_CPWMS_MASK | FTM_SC_PS(sc_ps) | FTM_SC_CLKS(1);  //û��ʹ������ж�
    FTM_CNTIN_REG(FTMx[ftmn]) = 0;                                                      // Channel (n) Value  �����������ȣ�(CnV - CNTIN).
    FTM_MOD_REG(FTMx[ftmn])   = mod;                                                    //Modulo valueģ��, EPWM������Ϊ ��MOD - CNTIN + 0x0001
    FTM_CNTIN_REG(FTMx[ftmn]) = 0;                                                      //Counter Initial Value ��������ʼ��ֵ
    FTM_CNT_REG(FTMx[ftmn])   = 0;                                                      //��������ֻ�е�16λ����
}






/*************************************************************************
*                            ������ӹ�����
*
*  �������ƣ�FTM_Input_init
*  ����˵�������벶׽��ʼ������
*  ����˵����FTMn        ģ��ţ�FTM0��  FTM1��  FTM2��
*            CHn         ͨ���ţ�CH0~CH7��
*            Input_cfg   ���벶׽���ã�Rising��Falling��Rising_or_Falling�������ز�׽���½��ز�׽�������ز�׽
*  �������أ���
*  �޸�ʱ�䣺2012-1-26
*  ��    ע��CH0~CH3����ʹ�ù�������δ����⹦��
*************************************************************************/
void FTM_Input_init(FTMn ftmn, CHn ch, Input_cfg cfg)
{

    switch(ftmn)
    {
    case SFTM0:
        SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;                           //ʹ��FTM0ʱ��
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
        SIM_SCGC6 |= SIM_SCGC6_FTM1_MASK;                           //ʹ��FTM1ʱ��
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
        SIM_SCGC3 |= SIM_SCGC3_FTM2_MASK;                           //ʹ��FTM2ʱ��
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
        SIM_SCGC3 |= SIM_SCGC3_FTM3_MASK;                           //ʹ��FTM2ʱ��
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
  case Rising:    //�����ش���
        FTM_CnSC_REG(FTMx[ftmn], ch) |=  ( FTM_CnSC_ELSA_MASK  | FTM_CnSC_CHIE_MASK );                   //��1
        FTM_CnSC_REG(FTMx[ftmn], ch) &= ~( FTM_CnSC_ELSB_MASK  | FTM_CnSC_MSB_MASK | FTM_CnSC_MSA_MASK); //��0
        break;

    case Falling:   //�½��ش���
        FTM_CnSC_REG(FTMx[ftmn], ch) |= (FTM_CnSC_ELSB_MASK  | FTM_CnSC_CHIE_MASK );                    //��1
        FTM_CnSC_REG(FTMx[ftmn], ch) &= ~( FTM_CnSC_ELSA_MASK | FTM_CnSC_MSB_MASK | FTM_CnSC_MSA_MASK); //��0
        break;

    case Rising_or_Falling: //�����ء��½��ض�����
        FTM_CnSC_REG(FTMx[ftmn], ch) |=  ( FTM_CnSC_ELSB_MASK | FTM_CnSC_ELSA_MASK  | FTM_CnSC_CHIE_MASK ); //��1
        FTM_CnSC_REG(FTMx[ftmn], ch) &= ~( FTM_CnSC_MSB_MASK  | FTM_CnSC_MSA_MASK); //��0
        break;
    }

    FTM_SC_REG(FTMx[ftmn]) = FTM_SC_CLKS(0x1)+ FTM_SC_PS(3);       //System clock

    FTM_MODE_REG(FTMx[ftmn]) |= FTM_MODE_WPDIS_MASK;
    FTM_COMBINE_REG(FTMx[ftmn]) = 0;
    FTM_MODE_REG(FTMx[ftmn]) &= ~FTM_MODE_FTMEN_MASK;
    FTM_CNTIN_REG(FTMx[ftmn]) = 0;

    FTM_STATUS_REG(FTMx[ftmn]) = 0x00;               //���жϱ�־λ

    //�������벶׽�ж�
		if(ftmn<3)
			NVIC_EnableIRQ((IRQn_Type)(FTM0_IRQn+ ftmn));
		else
			NVIC_EnableIRQ((IRQn_Type)(FTM3_IRQn));
}

/*************************************************************************
*                            ������ӹ�����
*
*  �������ƣ�FTM_QUAD_init
*  ����˵������������ģʽ ��ʼ������
*  ����˵����FTMn        ģ��ţ� FTM1��  FTM2��
*            CHn         ͨ���ţ�CH0~CH1��
*  �������أ���
*  �޸�ʱ�䣺2014-6-25
*  ��    ע��CH0~CH3����ʹ�ù�������δ����⹦��
*************************************************************************/
void FTM_QUAD_init(FTMn ftmn,CHn ch)
{
  switch(ftmn)
  {
  case SFTM1:
    SIM_SCGC6|=SIM_SCGC6_FTM1_MASK;                             //ʹ��FTM1ʱ��
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
    SIM_SCGC3 |= SIM_SCGC3_FTM2_MASK;                           //ʹ��FTM2ʱ��
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
  FTM_QDCTRL_REG(FTMx[ftmn]) &= ~FTM_QDCTRL_QUADMODE_MASK  ;//AB��ͬʱȷ������ͼ���ֵ 
  FTM_CNTIN_REG(FTMx[ftmn])=0;//FTM0��������ʼֵΪ0  
  FTM_MOD_REG(FTMx[ftmn])=65535;//����ֵ  
  FTM_QDCTRL_REG(FTMx[ftmn]) |=FTM_QDCTRL_QUADEN_MASK;//����FTMn��������ģʽ  
  FTM_MODE_REG(FTMx[ftmn]) |= FTM_MODE_FTMEN_MASK;   //FTMnEN=1    
  FTM_CNT_REG(FTMx[ftmn])=0; 
  
  FTM_SC_REG(FTMx[ftmn]) |= FTM_SC_CLKS(3);  //ѡ���ⲿʱ��
  
}
/*************************************************************************
*                            ������ӹ�����
*
*  �������ƣ�FTM_SpeedMeasure_init
*  ����˵������������ģʽ ��ʼ������
*  ����˵����FTMn        ģ��ţ� FTM1�� FTM2��
*            CHn         ͨ���ţ�CH0~CH1��
*  �������أ���
*  �޸�ʱ�䣺2014-6-25
*  ��    ע��CH0~CH3����ʹ�ù�������δ����⹦��
*************************************************************************/
void FTM_SpeedMeasure_init(void)
{
  FTM_QUAD_init(SFTM1,CH0);
  FTM_QUAD_init(SFTM1,CH1);
  FTM_QUAD_init(SFTM2,CH0);
  FTM_QUAD_init(SFTM2,CH1);
}


