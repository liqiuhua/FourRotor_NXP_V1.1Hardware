/*
** ###################################################################
**     Processor:           MK64FN1M0VMD12
**     Compilers:           ARM Compiler
**                          Freescale C/C++ for Embedded ARM
**                          GNU C Compiler
**                          GNU C Compiler - CodeSourcery Sourcery G++
**                          IAR ANSI C/C++ Compiler for ARM
**
**     Reference manual:    K64P144M120SF5RM, Rev.2, January 2014
**     Version:             rev. 2.3, 2014-01-24
**
**     Abstract:
**         Provides a system configuration function and a global variable that
**         contains the system frequency. It configures the device and initializes
**         the oscillator (PLL) that is part of the microcontroller device.
**
**     Copyright: 2014 Freescale, Inc. All Rights Reserved.
**
**     http:                 www.freescale.com
**     mail:                 support@freescale.com
**
**     Revisions:
**     - rev. 1.0 (2013-08-12)
**         Initial version.
**     - rev. 2.0 (2013-10-29)
**         Register accessor macros added to the memory map.
**         Symbols for Processor Expert memory map compatibility added to the memory map.
**         Startup file for gcc has been updated according to CMSIS 3.2.
**         System initialization updated.
**         MCG - registers updated.
**         PORTA, PORTB, PORTC, PORTE - registers for digital filter removed.
**     - rev. 2.1 (2013-10-29)
**         Definition of BITBAND macros updated to support peripherals with 32-bit acces disabled.
**     - rev. 2.2 (2013-12-09)
**         DMA - EARS register removed.
**         AIPS0, AIPS1 - MPRA register updated.
**     - rev. 2.3 (2014-01-24)
**         Update according to reference manual rev. 2
**         ENET, MCG, MCM, SIM, USB - registers updated
**
** ###################################################################
*/

/*!
 * @file MK64F12
 * @version 2.3
 * @date 2014-01-24
 * @brief Device specific configuration file for MK64F12 (implementation file)
 *
 * Provides a system configuration function and a global variable that contains
 * the system frequency. It configures the device and initializes the oscillator
 * (PLL) that is part of the microcontroller device.
 */

#include <stdint.h>
#include "MK64F12.h"
#include "system_MK64F125.h"


#define DISABLE_WDOG    1

#define CLOCK_SETUP     4

MCGPLL  Gmcgpll ;
SIMDIV  Gsimdiv ;
/* Predefined clock setups
   0 ... Multipurpose Clock Generator (MCG) in FLL Engaged Internal (FEI) mode
         Default  part configuration.
         Reference clock source for MCG module is the slow internal clock source 32.768kHz
         Core clock = 20.97MHz, BusClock = 20.97MHz
   1 ... Multipurpose Clock Generator (MCG) in PLL Engaged External (PEE) mode
         Maximum achievable clock frequency configuration.
         Reference clock source for MCG module is an external clock source 50MHz
         Core clock = 120MHz, BusClock = 60MHz
   2 ... Multipurpose Clock Generator (MCG) in Bypassed Low Power Internal (BLPI) mode
         Core clock/Bus clock derived directly from an fast internal clock 4MHz with no multiplication
         The clock settings is ready for Very Low Power Run mode.
         Core clock = 4MHz, BusClock = 4MHz
   3 ... Multipurpose Clock Generator (MCG) in Bypassed Low Power External (BLPE) mode
         Core clock/Bus clock derived directly from the RTC oscillator clock source 32.768kHz
         The clock settings is ready for Very Low Power Run mode.
         Core clock = 32.768kHz, BusClock = 32.768kHz
   4 ... Multipurpose Clock Generator (MCG) in PLL Engaged External (PEE) mode
         USB clock setup
         USB clock divider is set for USB to receive 48MHz input clock.
         Reference clock source for MCG module is an external clock source 50MHz
         USB clock divider is set for USB to receive 48MHz input clock.
         Core clock = 120MHz, BusClock = 60MHz
*/
/*-------------------------------------------------------------------------*
//函数名: syscloption                                                   *
//功  能:  根据选择系统时钟选择乘除因子                                                   *
//参  数: SYSPLL  选择时钟							  	   *
//返  回: 无                                                               *
//说  明: K60DZ  K64F12 的计算公式 SYSPLL =   OSCCLK *(pllVDIV+24)  /(pllPRDIV+1) 
*         K60FX        的计算公式 SYSPLL =   OSCCLK *(pllVDIV+16)  /(pllPRDIV+1)
//-------------------------------------------------------------------------*/
 void syscloption(system_option SYSPLL){
       long int systemclock ;
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

             #elif( OSCCLK == 16)
                if(SYSPLL == 50 ){
                 Gmcgpll.pllPRDIV = 7 ;
                 Gmcgpll.pllVDIV  = 1 ;
               }
               else if(SYSPLL <= 100){
                 Gmcgpll.pllPRDIV = 7 ;
                 Gmcgpll.pllVDIV = (uint8)(SYSPLL*(Gmcgpll.pllPRDIV+1) /OSCCLK - 24) ;
               }else if(SYSPLL <= 220){
                 Gmcgpll.pllPRDIV = 3 ;
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
 void system_init(system_option SYSPLL){
      
       
       syscloption(SYSPLL) ;
       
       //默认开始状态为FEI模式，切换到FBE模式
    //   OSC_CR = OSC_CR_ERCLKEN_MASK ;
          // Enable external oscillator, RANGE=2, HGO=1, EREFS=1, LP=0, IRCS=0
         MCG_C2 = MCG_C2_RANGE0(1)  | MCG_C2_EREFS0_MASK;   
      //     MCG_C2 = MCG_C2_RANGE0(2)  | MCG_C2_HGO0_MASK| MCG_C2_EREFS0_MASK; 	 
        //选择外部晶振（50MHz）作为时钟源，设置FLL分频器，清IREFS启动外部晶振
        MCG_C1 = MCG_C1_CLKS(2) | MCG_C1_FRDIV(4)|MCG_C1_IRCLKEN_MASK;
	    //   MCG_C1 = MCG_C1_CLKS(2) | MCG_C1_FRDIV(7);
	        /* MCG_C4: DMX32=0,DRST_DRS=0 */
    //    MCG_C4 &= (uint8_t)~(uint8_t)((MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(0x03))); 
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




/* ----------------------------------------------------------------------------
   -- Core clock
   ---------------------------------------------------------------------------- */



/* ----------------------------------------------------------------------------
   -- SystemInit()
   ---------------------------------------------------------------------------- */

void SystemInit (void) {
#if ((__FPU_PRESENT == 1) && (__FPU_USED == 1))
  SCB->CPACR |= ((3UL << 10*2) | (3UL << 11*2));    /* set CP10, CP11 Full Access */
#endif /* ((__FPU_PRESENT == 1) && (__FPU_USED == 1)) */
#if (DISABLE_WDOG)
  /* Disable the WDOG module */
  /* WDOG->UNLOCK: WDOGUNLOCK=0xC520 */
  WDOG->UNLOCK = WDOG_UNLOCK_WDOGUNLOCK(0xC520); /* Key 1 */
  /* WDOG->UNLOCK: WDOGUNLOCK=0xD928 */
  WDOG->UNLOCK = WDOG_UNLOCK_WDOGUNLOCK(0xD928); /* Key 2 */
  /* WDOG->STCTRLH: ??=0,DISTESTWDOG=0,BYTESEL=0,TESTSEL=0,TESTWDOG=0,??=0,??=1,WAITEN=1,STOPEN=1,DBGEN=0,ALLOWUPDATE=1,WINEN=0,IRQRSTEN=0,CLKSRC=1,WDOGEN=0 */
  WDOG->STCTRLH = WDOG_STCTRLH_BYTESEL(0x00) |
                 WDOG_STCTRLH_WAITEN_MASK |
                 WDOG_STCTRLH_STOPEN_MASK |
                 WDOG_STCTRLH_ALLOWUPDATE_MASK |
                 WDOG_STCTRLH_CLKSRC_MASK |
                 0x0100U;
#endif /* (DISABLE_WDOG) */
	
   system_init(SYSTEMCLOCK);
}


/* ----------------------------------------------------------------------------
   -- SystemCoreClockUpdate()
   ---------------------------------------------------------------------------- */

void LanSystemCoreClockUpdate (uint32 * sysclkKH,uint32 *busclkKH) {
  uint32 systemclock;                                                        /* Variable to store output clock frequency of the MCG module */
  uint32 busclock ;

#if (defined(MK60DZ10)||defined(MK64F12)) 
        systemclock = OSCCLK*((MCG_C6 & MCG_C6_VDIV0_MASK) + 24)/((MCG_C5&MCG_C5_PRDIV0_MASK) + 1) ;
#elif defined(MK60F15)
         systemclock = OSCCLK*((MCG_C6 & MCG_C6_VDIV0_MASK) + 16)/((MCG_C5&MCG_C5_PRDIV0_MASK) + 1) ;      
#endif
	systemclock *= 1000 ;
	busclock = systemclock /(((SIM_CLKDIV1&SIM_CLKDIV1_OUTDIV2_MASK)>>SIM_CLKDIV1_OUTDIV2_SHIFT)+1) ;
	*sysclkKH = systemclock ;
	*busclkKH = busclock ;

}
