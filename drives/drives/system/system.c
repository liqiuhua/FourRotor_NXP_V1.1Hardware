
/***********************************************************
 * 文件名       ：system.c
 * 说明         ：系统频率初始化
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://bbs.landzo.com
 * 库版本       ：V1.0
 *时间          ：15.5.26
************************************************************/

#include "common.h"
#include "system.h"

MCGPLL  Gmcgpll ;
SIMDIV  Gsimdiv ;


/* ----------------------------------------------------------------------------
   -- Core clock
   ---------------------------------------------------------------------------- */

//uint32_t SystemCoreClock = DEFAULT_SYSTEM_CLOCK;

     
/*-------------------------------------------------------------------------*
//函数名: syscloption                                                   *
//功  能:  根据选择系统时钟选择乘除因子                                                   *
//参  数: SYSPLL  选择时钟							  	   *
//返  回: 无                                                               *
//说  明: K60DZ  K64F12 的计算公式 SYSPLL =   OSCCLK *(pllVDIV+24)  /(pllPRDIV+1) 
*         K60FX        的计算公式 SYSPLL =   OSCCLK *(pllVDIV+16)  /(pllPRDIV+1)
//-------------------------------------------------------------------------*/
     void syscloption(system_option SYSPLL){
       uint32 systemclock ;
       /**************
       计算pll分频因子,
       因为K60FX与另外芯片不一样，
       **************/
       #if (defined(MK60DZ10)||defined(MK64F12))
             #if( OSCCLK == 50) 
               if(SYSPLL == 50 ){
                 Gmcgpll.pllPRDIV = 24 ;
                 Gmcgpll.pllVDIV  = 1 ;
               }
               else if(SYSPLL <= 130){
                 Gmcgpll.pllPRDIV = 19 ;
                 Gmcgpll.pllVDIV = (uint8)(SYSPLL*(Gmcgpll.pllPRDIV+1) /OSCCLK- 24) ;
               }
               else if(SYSPLL <= 210){
                 Gmcgpll.pllPRDIV = 13 ;
                 Gmcgpll.pllVDIV = (uint8)(SYSPLL*(Gmcgpll.pllPRDIV+1) /OSCCLK - 24) ;
               }
               else {
                 Gmcgpll.pllPRDIV = 13 ;
                 Gmcgpll.pllVDIV = 16 ;
               }

             #elif( OSCCLK == 12)
                if(SYSPLL == 50 ){
                 Gmcgpll.pllPRDIV = 5 ;
                 Gmcgpll.pllVDIV  = 1 ;
               }
               else if(SYSPLL <= 130){
                 Gmcgpll.pllPRDIV = 3 ;
                 Gmcgpll.pllVDIV = (uint8)(SYSPLL*(Gmcgpll.pllPRDIV+1) /OSCCLK - 24) ;
               }else if(SYSPLL <= 220){
                 Gmcgpll.pllPRDIV = 2 ;
                 Gmcgpll.pllVDIV = (uint8)(SYSPLL*(Gmcgpll.pllPRDIV+1) /OSCCLK - 24) ;
               }else {
                 Gmcgpll.pllPRDIV = 2 ;
                 Gmcgpll.pllVDIV =  24 ;
               }
             #endif
           if(Gmcgpll.pllVDIV > 0x1F) Gmcgpll.pllVDIV = 0x1F ;       
            systemclock = OSCCLK *(Gmcgpll.pllVDIV+24)/(Gmcgpll.pllPRDIV+1) - 2; 
      #elif defined(MK60F15)
         #if( OSCCLK <= 50)  
           if(SYSPLL <= 320 ){
             Gmcgpll.pllPRDIV = 4 ; 
             Gmcgpll.pllVDIV = (uint8)(SYSPLL*(Gmcgpll.pllPRDIV+1) /OSCCLK- 16) ;
           }
            else {
              Gmcgpll.pllPRDIV = 4 ;
              Gmcgpll.pllVDIV = 1;

            }
          #elif( OSCCLK == 12)
             if(SYSPLL <= 320 ){
               Gmcgpll.pllPRDIV = 1 ; 
               Gmcgpll.pllVDIV  = (uint8)(SYSPLL*(Gmcgpll.pllPRDIV+1) /OSCCLK- 16) ;
             }
              else {
                Gmcgpll.pllPRDIV = 1 ;
                Gmcgpll.pllVDIV  = 20 ;
                #pragma warning
            } 
            #endif
         if(Gmcgpll.pllVDIV > 0x1F) Gmcgpll.pllVDIV = 0x1F ;       
         systemclock = OSCCLK *(Gmcgpll.pllVDIV+16)/(Gmcgpll.pllPRDIV+1) - 2;     
      #endif
        /**************
       计算系统时钟
       **************/
            
       Gsimdiv.coreDIV = 0 ;
       Gsimdiv.busDIV = systemclock / BUSCLOCKMAX ;
       Gsimdiv.FlexBusDIV = systemclock / FLEXCLOCKMAX ;
       Gsimdiv.FlashDIV = systemclock/FLASHCLOCKMAX ;
       if(Gsimdiv.busDIV > 0xF) Gsimdiv.busDIV = 0X0F ;
       if(Gsimdiv.FlexBusDIV > 0xF) Gsimdiv.FlexBusDIV = 0X0F ;
       if(Gsimdiv.FlashDIV > 0xF) Gsimdiv.FlashDIV = 0X0F ;
       
       
       
     }
/*-------------------------------------------------------------------------*
//函数名: system_init                                                  *
//功  能: 系统初始化                                                   *
//参  数: SYSPLL  系统选择时钟							  	   *
//返  回: sysclolck   设置的系统时钟                                                              *
//说  明: 芯片使用PLL来产生芯片所需的各种时钟，芯片复位后，MCG默认进入FEI模式，
//         需按照FEI->FEB->PBE->PEE的顺序进行配置。                                                     *
//-------------------------------------------------------------------------*/
     uint32 system_init(system_option SYSPLL){
       uint32 systemclock ;
       
       syscloption(SYSPLL) ;
       
       //默认开始状态为FEI模式，切换到FBE模式
       
          // Enable external oscillator, RANGE=2, HGO=1, EREFS=1, LP=0, IRCS=0
         MCG_C2 = MCG_C2_RANGE0(2) | MCG_C2_HGO0_MASK| MCG_C2_EREFS0_MASK;          
        //选择外部晶振（50MHz）作为时钟源，设置FLL分频器，清IREFS启动外部晶振
        MCG_C1 = MCG_C1_CLKS(2) | MCG_C1_FRDIV(7);
        while (!(MCG_S & MCG_S_OSCINIT0_MASK)) {};  //等待晶振稳定
        //等待参考时钟状态位清零
        while (MCG_S & MCG_S_IREFST_MASK){}; 
        //等待时钟状态位显示时钟源来自外部参考时钟
        while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2){}; 

        //已进入FBE模式，切换到PBE模式
        //设置PLL分频，为外部时钟源的分频，得到分频时钟
        MCG_C5 = MCG_C5_PRDIV0(Gmcgpll.pllPRDIV);
        //确保MCG_C6处于复位状态，禁止LOLIE、PLL、和时钟控制器，清PLL VCO分频器
        MCG_C6 = 0x0;
        //选择PLL VCO分频器，系统时钟分频器取决于时钟选项
        set_sys_dividers(Gsimdiv.coreDIV,Gsimdiv.busDIV,Gsimdiv.FlexBusDIV,Gsimdiv.FlashDIV) ;  
        MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV0(Gmcgpll.pllVDIV) ;
        while (!(MCG_S & MCG_S_PLLST_MASK)){}; // 等待PLL稳定
        while (!(MCG_S & MCG_S_LOCK0_MASK)){};  // 等待PLL锁定
        //已进入PBE模式，切换到PEE模式
        //通过清零CLKS位来进入PEE模式
        // CLKS=0, FRDIV=3, IREFS=0, IRCLKEN=0, IREFSTEN=0
        MCG_C1 &= ~MCG_C1_CLKS_MASK; 
            // Wait for clock status bits to update
        while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3) {};
       
#if (defined(MK60DZ10)||defined(MK64F12)) 
        systemclock = OSCCLK*((MCG_C6 & MCG_C6_VDIV0_MASK) + 24)/((MCG_C5&MCG_C5_PRDIV0_MASK) + 1) ;
#elif defined(MK60F15)
         systemclock = OSCCLK*((MCG_C6 & MCG_C6_VDIV0_MASK) + 16)/((MCG_C5&MCG_C5_PRDIV0_MASK) + 1) ;      
#endif
        systemclock *=1000 ;
       return systemclock ;
     }
     
     
/*-------------------------------------------------------------------------*
//函数名称：set_sys_dividers
//函数返回：无
//参数说明：outdiv1-4：预分频值
//功能概要：系统分频器设定。、
//补充说明：此函数必须放在RAM里执行，否则会产生错误e2448。当FLASH时钟分频改变
//         时，必须禁止FLASH的预取功能。在时钟分频改变之后，必须延时一段时间
//         后才可以重新使能预取功能。					
//-------------------------------------------------------------------------*/
__RAMFUN void set_sys_dividers(uint32 outdiv1, uint32 outdiv2, uint32 outdiv3, uint32 outdiv4)
{
    uint32 temp_reg;
    uint8 i;
    
    //保存FMC_PFAPR当前的值
    temp_reg = FMC_PFAPR;

    //通过M&PFD置位M0PFD来禁止预取功能
    FMC_PFAPR |= FMC_PFAPR_M7PFD_MASK 
               | FMC_PFAPR_M6PFD_MASK 
               | FMC_PFAPR_M5PFD_MASK
               | FMC_PFAPR_M4PFD_MASK 
               | FMC_PFAPR_M3PFD_MASK 
               | FMC_PFAPR_M2PFD_MASK
               | FMC_PFAPR_M1PFD_MASK 
               | FMC_PFAPR_M0PFD_MASK;

    //给时钟分频器设置期望值  
    SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(outdiv1) 
                | SIM_CLKDIV1_OUTDIV2(outdiv2) 
                | SIM_CLKDIV1_OUTDIV3(outdiv3) 
                | SIM_CLKDIV1_OUTDIV4(outdiv4);

    //等待分频器改变
    for (i = 0 ; i < outdiv4 ; i++){}

    //重新存FMC_PFAPR的原始值
    FMC_PFAPR = temp_reg;
}

     ////void SystemInit (void) {
//#if ((__FPU_PRESENT == 1) && (__FPU_USED == 1))
//  SCB->CPACR |= ((3UL << 10*2) | (3UL << 11*2));    /* set CP10, CP11 Full Access */
//#endif /* ((__FPU_PRESENT == 1) && (__FPU_USED == 1)) */
//#if (DISABLE_WDOG)
//  /* WDOG->UNLOCK: WDOGUNLOCK=0xC520 */
//  WDOG->UNLOCK = WDOG_UNLOCK_WDOGUNLOCK(0xC520); /* Key 1 */
//  /* WDOG->UNLOCK: WDOGUNLOCK=0xD928 */
//  WDOG->UNLOCK = WDOG_UNLOCK_WDOGUNLOCK(0xD928); /* Key 2 */
//  /* WDOG->STCTRLH: ?=0,DISTESTWDOG=0,BYTESEL=0,TESTSEL=0,TESTWDOG=0,?=0,?=1,WAITEN=1,STOPEN=1,DBGEN=0,ALLOWUPDATE=1,WINEN=0,IRQRSTEN=0,CLKSRC=1,WDOGEN=0 */
//  WDOG->STCTRLH = WDOG_STCTRLH_BYTESEL(0x00) |
//                 WDOG_STCTRLH_WAITEN_MASK |
//                 WDOG_STCTRLH_STOPEN_MASK |
//                 WDOG_STCTRLH_ALLOWUPDATE_MASK |
//                 WDOG_STCTRLH_CLKSRC_MASK |
//                 0x0100U;
//#endif /* (DISABLE_WDOG) */
//  if((RCM->SRS0 & RCM_SRS0_WAKEUP_MASK) != 0x00U)
//  {
//    if((PMC->REGSC & PMC_REGSC_ACKISO_MASK) != 0x00U)
//    {
//       PMC->REGSC |= PMC_REGSC_ACKISO_MASK; /* Release hold with ACKISO:  Only has an effect if recovering from VLLSx.*/
//    }
//  } else {
//#ifdef SYSTEM_RTC_CR_VALUE
//    SIM_SCGC6 |= SIM_SCGC6_RTC_MASK;
//    if ((RTC_CR & RTC_CR_OSCE_MASK) == 0x00U) { /* Only if the OSCILLATOR is not already enabled */
//      RTC_CR = (uint32_t)((RTC_CR & (uint32_t)~(uint32_t)(RTC_CR_SC2P_MASK | RTC_CR_SC4P_MASK | RTC_CR_SC8P_MASK | RTC_CR_SC16P_MASK)) | (uint32_t)SYSTEM_RTC_CR_VALUE);
//      RTC_CR |= (uint32_t)RTC_CR_OSCE_MASK;
//      RTC_CR &= (uint32_t)~(uint32_t)RTC_CR_CLKO_MASK;
//    }
//#endif
//  }
//
//  /* Power mode protection initialization */
//#ifdef SYSTEM_SMC_PMPROT_VALUE
//  SMC->PMPROT = SYSTEM_SMC_PMPROT_VALUE;
//#endif
//
//  /* System clock initialization */
//  /* Internal reference clock trim initialization */
//#if defined(SLOW_TRIM_ADDRESS)
//  if ( *((uint8_t*)SLOW_TRIM_ADDRESS) != 0xFFU) {                              /* Skip if non-volatile flash memory is erased */
//    MCG->C3 = *((uint8_t*)SLOW_TRIM_ADDRESS);
//  #endif /* defined(SLOW_TRIM_ADDRESS) */
//  #if defined(SLOW_FINE_TRIM_ADDRESS)
//    MCG->C4 = (MCG->C4 & ~(MCG_C4_SCFTRIM_MASK)) | ((*((uint8_t*) SLOW_FINE_TRIM_ADDRESS)) & MCG_C4_SCFTRIM_MASK);
//  #endif
//  #if defined(FAST_TRIM_ADDRESS)
//    MCG->C4 = (MCG->C4 & ~(MCG_C4_FCTRIM_MASK)) |((*((uint8_t*) FAST_TRIM_ADDRESS)) & MCG_C4_FCTRIM_MASK);
//  #endif
//  #if defined(FAST_FINE_TRIM_ADDRESS)
//    MCG->C2 = (MCG->C2 & ~(MCG_C2_FCFTRIM_MASK)) | ((*((uint8_t*)FAST_TRIM_ADDRESS)) & MCG_C2_FCFTRIM_MASK);
//  #endif /* defined(FAST_FINE_TRIM_ADDRESS) */
//#if defined(SLOW_TRIM_ADDRESS)
//  }
//  #endif /* defined(SLOW_TRIM_ADDRESS) */
//
//  /* Set system prescalers and clock sources */
//  SIM->CLKDIV1 = SYSTEM_SIM_CLKDIV1_VALUE; /* Set system prescalers */
//  SIM->SOPT1 = ((SIM->SOPT1) & (uint32_t)(~(SIM_SOPT1_OSC32KSEL_MASK))) | ((SYSTEM_SIM_SOPT1_VALUE) & (SIM_SOPT1_OSC32KSEL_MASK)); /* Set 32 kHz clock source (ERCLK32K) */
//  SIM->SOPT2 = ((SIM->SOPT2) & (uint32_t)(~(SIM_SOPT2_PLLFLLSEL_MASK))) | ((SYSTEM_SIM_SOPT2_VALUE) & (SIM_SOPT2_PLLFLLSEL_MASK)); /* Selects the high frequency clock for various peripheral clocking options. */
//#if ((MCG_MODE == MCG_MODE_FEI) || (MCG_MODE == MCG_MODE_FBI) || (MCG_MODE == MCG_MODE_BLPI))
//  /* Set MCG and OSC */
//#if  ((((SYSTEM_OSC_CR_VALUE) & OSC_CR_ERCLKEN_MASK) != 0x00U) || ((((SYSTEM_MCG_C5_VALUE) & MCG_C5_PLLCLKEN0_MASK) != 0x00U) && (((SYSTEM_MCG_C7_VALUE) & MCG_C7_OSCSEL_MASK) == 0x00U)))
//  /* SIM_SCGC5: PORTA=1 */
//  SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;
//  /* PORTA_PCR18: ISF=0,MUX=0 */
//  PORTA_PCR18 &= (uint32_t)~(uint32_t)((PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x07)));
//  if (((SYSTEM_MCG_C2_VALUE) & MCG_C2_EREFS_MASK) != 0x00U) {
//  /* PORTA_PCR19: ISF=0,MUX=0 */
//  PORTA_PCR19 &= (uint32_t)~(uint32_t)((PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x07)));
//  }
//#endif
//  MCG->SC = SYSTEM_MCG_SC_VALUE;       /* Set SC (fast clock internal reference divider) */
//  MCG->C1 = SYSTEM_MCG_C1_VALUE;       /* Set C1 (clock source selection, FLL ext. reference divider, int. reference enable etc.) */
//  /* Check that the source of the FLL reference clock is the requested one. */
//  if (((SYSTEM_MCG_C1_VALUE) & MCG_C1_IREFS_MASK) != 0x00U) {
//    while((MCG->S & MCG_S_IREFST_MASK) == 0x00U) {
//    }
//  } else {
//    while((MCG->S & MCG_S_IREFST_MASK) != 0x00U) {
//    }
//  }
//  MCG->C2 = (MCG->C2 & (uint8_t)(~(MCG_C2_FCFTRIM_MASK))) | (SYSTEM_MCG_C2_VALUE & (uint8_t)(~(MCG_C2_LP_MASK))); /* Set C2 (freq. range, ext. and int. reference selection etc. excluding trim bits; low power bit is set later) */
//  MCG->C4 = ((SYSTEM_MCG_C4_VALUE) & (uint8_t)(~(MCG_C4_FCTRIM_MASK | MCG_C4_SCFTRIM_MASK))) | (MCG->C4 & (MCG_C4_FCTRIM_MASK | MCG_C4_SCFTRIM_MASK)); /* Set C4 (FLL output; trim values not changed) */
//  OSC->CR = SYSTEM_OSC_CR_VALUE;       /* Set OSC_CR (OSCERCLK enable, oscillator capacitor load) */
//  MCG->C7 = SYSTEM_MCG_C7_VALUE;       /* Set C7 (OSC Clock Select) */
//  #if (MCG_MODE == MCG_MODE_BLPI)
//  /* BLPI specific */
//  MCG->C2 |= (MCG_C2_LP_MASK);         /* Disable FLL and PLL in bypass mode */
//  #endif
//
//#else /* MCG_MODE */
//  /* Set MCG and OSC */
//#if  (((SYSTEM_OSC_CR_VALUE) & OSC_CR_ERCLKEN_MASK) != 0x00U) || (((SYSTEM_MCG_C7_VALUE) & MCG_C7_OSCSEL_MASK) == 0x00U)
//  /* SIM_SCGC5: PORTA=1 */
//  SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;
//  /* PORTA_PCR18: ISF=0,MUX=0 */
//  PORTA_PCR18 &= (uint32_t)~(uint32_t)((PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x07)));
//  if (((SYSTEM_MCG_C2_VALUE) & MCG_C2_EREFS_MASK) != 0x00U) {
//  /* PORTA_PCR19: ISF=0,MUX=0 */
//  PORTA_PCR19 &= (uint32_t)~(uint32_t)((PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x07)));
//  }
//#endif
//  MCG->SC = SYSTEM_MCG_SC_VALUE;       /* Set SC (fast clock internal reference divider) */
//  MCG->C2 = (MCG->C2 & (uint8_t)(~(MCG_C2_FCFTRIM_MASK))) | (SYSTEM_MCG_C2_VALUE & (uint8_t)(~(MCG_C2_LP_MASK))); /* Set C2 (freq. range, ext. and int. reference selection etc. excluding trim bits; low power bit is set later) */
//  OSC->CR = SYSTEM_OSC_CR_VALUE;       /* Set OSC_CR (OSCERCLK enable, oscillator capacitor load) */
//  MCG->C7 = SYSTEM_MCG_C7_VALUE;       /* Set C7 (OSC Clock Select) */
//  #if (MCG_MODE == MCG_MODE_PEE)
//  MCG->C1 = (SYSTEM_MCG_C1_VALUE) | MCG_C1_CLKS(0x02); /* Set C1 (clock source selection, FLL ext. reference divider, int. reference enable etc.) - PBE mode*/
//  #else
//  MCG->C1 = SYSTEM_MCG_C1_VALUE;       /* Set C1 (clock source selection, FLL ext. reference divider, int. reference enable etc.) */
//  #endif
//  if ((((SYSTEM_MCG_C2_VALUE) & MCG_C2_EREFS_MASK) != 0x00U) && (((SYSTEM_MCG_C7_VALUE) & MCG_C7_OSCSEL_MASK) == 0x00U)) {
//    while((MCG->S & MCG_S_OSCINIT0_MASK) == 0x00U) { /* Check that the oscillator is running */
//    }
//  }
//  /* Check that the source of the FLL reference clock is the requested one. */
//  if (((SYSTEM_MCG_C1_VALUE) & MCG_C1_IREFS_MASK) != 0x00U) {
//    while((MCG->S & MCG_S_IREFST_MASK) == 0x00U) {
//    }
//  } else {
//    while((MCG->S & MCG_S_IREFST_MASK) != 0x00U) {
//    }
//  }
//  MCG->C4 = ((SYSTEM_MCG_C4_VALUE)  & (uint8_t)(~(MCG_C4_FCTRIM_MASK | MCG_C4_SCFTRIM_MASK))) | (MCG->C4 & (MCG_C4_FCTRIM_MASK | MCG_C4_SCFTRIM_MASK)); /* Set C4 (FLL output; trim values not changed) */
//#endif /* MCG_MODE */
//
//  /* Common for all MCG modes */
//
//  /* PLL clock can be used to generate clock for some devices regardless of clock generator (MCGOUTCLK) mode. */
//  MCG->C5 = (SYSTEM_MCG_C5_VALUE) & (uint8_t)(~(MCG_C5_PLLCLKEN0_MASK)); /* Set C5 (PLL settings, PLL reference divider etc.) */
//  MCG->C6 = (SYSTEM_MCG_C6_VALUE) & (uint8_t)~(MCG_C6_PLLS_MASK); /* Set C6 (PLL select, VCO divider etc.) */
//  if ((SYSTEM_MCG_C5_VALUE) & MCG_C5_PLLCLKEN0_MASK) {
//    MCG->C5 |= MCG_C5_PLLCLKEN0_MASK;  /* PLL clock enable in mode other than PEE or PBE */
//  }
//  /* BLPE, PEE and PBE MCG mode specific */
//
//#if (MCG_MODE == MCG_MODE_BLPE)
//  MCG->C2 |= (MCG_C2_LP_MASK);         /* Disable FLL and PLL in bypass mode */
//#elif ((MCG_MODE == MCG_MODE_PBE) || (MCG_MODE == MCG_MODE_PEE))
//  MCG->C6 |= (MCG_C6_PLLS_MASK);       /* Set C6 (PLL select, VCO divider etc.) */
//  while((MCG->S & MCG_S_LOCK0_MASK) == 0x00U) { /* Wait until PLL is locked*/
//  }
//  #if (MCG_MODE == MCG_MODE_PEE)
//  MCG->C1 &= (uint8_t)~(MCG_C1_CLKS_MASK);
//  #endif
//#endif
//#if ((MCG_MODE == MCG_MODE_FEI) || (MCG_MODE == MCG_MODE_FEE))
//  while((MCG->S & MCG_S_CLKST_MASK) != 0x00U) { /* Wait until output of the FLL is selected */
//  }
//#elif ((MCG_MODE == MCG_MODE_FBI) || (MCG_MODE == MCG_MODE_BLPI))
//  while((MCG->S & MCG_S_CLKST_MASK) != 0x04U) { /* Wait until internal reference clock is selected as MCG output */
//  }
//#elif ((MCG_MODE == MCG_MODE_FBE) || (MCG_MODE == MCG_MODE_PBE) || (MCG_MODE == MCG_MODE_BLPE))
//  while((MCG->S & MCG_S_CLKST_MASK) != 0x08U) { /* Wait until external reference clock is selected as MCG output */
//  }
//#elif (MCG_MODE == MCG_MODE_PEE)
//  while((MCG->S & MCG_S_CLKST_MASK) != 0x0CU) { /* Wait until output of the PLL is selected */
//  }
//#endif
//#if (((SYSTEM_SMC_PMCTRL_VALUE) & SMC_PMCTRL_RUNM_MASK) == (0x02U << SMC_PMCTRL_RUNM_SHIFT))
//  SMC->PMCTRL = (uint8_t)((SYSTEM_SMC_PMCTRL_VALUE) & (SMC_PMCTRL_RUNM_MASK)); /* Enable VLPR mode */
//  while(SMC->PMSTAT != 0x04U) {        /* Wait until the system is in VLPR mode */
//  }
//#endif
//
//#if defined(SYSTEM_SIM_CLKDIV2_VALUE)
//  SIM->CLKDIV2 = ((SIM->CLKDIV2) & (uint32_t)(~(SIM_CLKDIV2_USBFRAC_MASK | SIM_CLKDIV2_USBDIV_MASK))) | ((SYSTEM_SIM_CLKDIV2_VALUE) & (SIM_CLKDIV2_USBFRAC_MASK | SIM_CLKDIV2_USBDIV_MASK)); /* Selects the USB clock divider. */
//#endif
//
//  /* PLL loss of lock interrupt request initialization */
//  if (((SYSTEM_MCG_C6_VALUE) & MCG_C6_LOLIE0_MASK) != 0U) {
//    NVIC_EnableIRQ(MCG_IRQn);          /* Enable PLL loss of lock interrupt request */
//  }
//}

/* ----------------------------------------------------------------------------
   -- SystemCoreClockUpdate()
   ---------------------------------------------------------------------------- */
//
//void SystemCoreClockUpdate (void) {
//  uint32_t MCGOUTClock;                /* Variable to store output clock frequency of the MCG module */
//  uint16_t Divider;
//
//  if ((MCG->C1 & MCG_C1_CLKS_MASK) == 0x00U) {
//    /* Output of FLL or PLL is selected */
//    if ((MCG->C6 & MCG_C6_PLLS_MASK) == 0x00U) {
//      /* FLL is selected */
//      if ((MCG->C1 & MCG_C1_IREFS_MASK) == 0x00U) {
//        /* External reference clock is selected */
//        switch (MCG->C7 & MCG_C7_OSCSEL_MASK) {
//        case 0x00U:
//          MCGOUTClock = CPU_XTAL_CLK_HZ; /* System oscillator drives MCG clock */
//          break;
//        case 0x01U:
//          MCGOUTClock = CPU_XTAL32k_CLK_HZ; /* RTC 32 kHz oscillator drives MCG clock */
//          break;
//        case 0x02U:
//        default:
//          MCGOUTClock = CPU_INT_IRC_CLK_HZ; /* IRC 48MHz oscillator drives MCG clock */
//          break;
//        }
//        if (((MCG->C2 & MCG_C2_RANGE_MASK) != 0x00U) && ((MCG->C7 & MCG_C7_OSCSEL_MASK) != 0x01U)) {
//          switch (MCG->C1 & MCG_C1_FRDIV_MASK) {
//          case 0x38U:
//            Divider = 1536U;
//            break;
//          case 0x30U:
//            Divider = 1280U;
//            break;
//          default:
//            Divider = (uint16_t)(32LU << ((MCG->C1 & MCG_C1_FRDIV_MASK) >> MCG_C1_FRDIV_SHIFT));
//            break;
//          }
//        } else {/* ((MCG->C2 & MCG_C2_RANGE_MASK) != 0x00U) */
//          Divider = (uint16_t)(1LU << ((MCG->C1 & MCG_C1_FRDIV_MASK) >> MCG_C1_FRDIV_SHIFT));
//        }
//        MCGOUTClock = (MCGOUTClock / Divider); /* Calculate the divided FLL reference clock */
//      } else { /* (!((MCG->C1 & MCG_C1_IREFS_MASK) == 0x00U)) */
//        MCGOUTClock = CPU_INT_SLOW_CLK_HZ; /* The slow internal reference clock is selected */
//      } /* (!((MCG->C1 & MCG_C1_IREFS_MASK) == 0x00U)) */
//      /* Select correct multiplier to calculate the MCG output clock  */
//      switch (MCG->C4 & (MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS_MASK)) {
//        case 0x00U:
//          MCGOUTClock *= 640U;
//          break;
//        case 0x20U:
//          MCGOUTClock *= 1280U;
//          break;
//        case 0x40U:
//          MCGOUTClock *= 1920U;
//          break;
//        case 0x60U:
//          MCGOUTClock *= 2560U;
//          break;
//        case 0x80U:
//          MCGOUTClock *= 732U;
//          break;
//        case 0xA0U:
//          MCGOUTClock *= 1464U;
//          break;
//        case 0xC0U:
//          MCGOUTClock *= 2197U;
//          break;
//        case 0xE0U:
//          MCGOUTClock *= 2929U;
//          break;
//        default:
//          break;
//      }
//    } else { /* (!((MCG->C6 & MCG_C6_PLLS_MASK) == 0x00U)) */
//      /* PLL is selected */
//      Divider = (((uint16_t)MCG->C5 & MCG_C5_PRDIV0_MASK) + 0x01U);
//      MCGOUTClock = (uint32_t)(CPU_XTAL_CLK_HZ / Divider); /* Calculate the PLL reference clock */
//      Divider = (((uint16_t)MCG->C6 & MCG_C6_VDIV0_MASK) + 24U);
//      MCGOUTClock *= Divider;          /* Calculate the MCG output clock */
//    } /* (!((MCG->C6 & MCG_C6_PLLS_MASK) == 0x00U)) */
//  } else if ((MCG->C1 & MCG_C1_CLKS_MASK) == 0x40U) {
//    /* Internal reference clock is selected */
//    if ((MCG->C2 & MCG_C2_IRCS_MASK) == 0x00U) {
//      MCGOUTClock = CPU_INT_SLOW_CLK_HZ; /* Slow internal reference clock selected */
//    } else { /* (!((MCG->C2 & MCG_C2_IRCS_MASK) == 0x00U)) */
//      Divider = (uint16_t)(0x01LU << ((MCG->SC & MCG_SC_FCRDIV_MASK) >> MCG_SC_FCRDIV_SHIFT));
//      MCGOUTClock = (uint32_t) (CPU_INT_FAST_CLK_HZ / Divider); /* Fast internal reference clock selected */
//    } /* (!((MCG->C2 & MCG_C2_IRCS_MASK) == 0x00U)) */
//  } else if ((MCG->C1 & MCG_C1_CLKS_MASK) == 0x80U) {
//    /* External reference clock is selected */
//    switch (MCG->C7 & MCG_C7_OSCSEL_MASK) {
//    case 0x00U:
//      MCGOUTClock = CPU_XTAL_CLK_HZ;   /* System oscillator drives MCG clock */
//      break;
//    case 0x01U:
//      MCGOUTClock = CPU_XTAL32k_CLK_HZ; /* RTC 32 kHz oscillator drives MCG clock */
//      break;
//    case 0x02U:
//    default:
//      MCGOUTClock = CPU_INT_IRC_CLK_HZ; /* IRC 48MHz oscillator drives MCG clock */
//      break;
//    }
//  } else { /* (!((MCG->C1 & MCG_C1_CLKS_MASK) == 0x80U)) */
//    /* Reserved value */
//    return;
//  } /* (!((MCG->C1 & MCG_C1_CLKS_MASK) == 0x80U)) */
//  SystemCoreClock = (MCGOUTClock / (0x01U + ((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV1_MASK) >> SIM_CLKDIV1_OUTDIV1_SHIFT)));
//}
