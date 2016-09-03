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
 * @brief Device specific configuration file for MK64F12 (header file)
 *
 * Provides a system configuration function and a global variable that contains
 * the system frequency. It configures the device and initializes the oscillator
 * (PLL) that is part of the microcontroller device.
 */

#ifndef SYSTEM_MK64F125_H_
#define SYSTEM_MK64F125_H_                        /**< Symbol preventing repeated inclusion */

#include  "common.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

	
/* Define clock source values */

#define CPU_XTAL_CLK_HZ                50000000u           /* Value of the external crystal or oscillator clock frequency in Hz */
#define CPU_XTAL32k_CLK_HZ             32768u              /* Value of the external 32k crystal or oscillator clock frequency in Hz */
#define CPU_INT_SLOW_CLK_HZ            32768u              /* Value of the slow internal oscillator clock frequency in Hz  */
#define CPU_INT_FAST_CLK_HZ            4000000u            /* Value of the fast internal oscillator clock frequency in Hz  */
#define CPU_INT_IRC_CLK_HZ             48000000u           /* Value of the 48M internal oscillator clock frequency in Hz  */

/* RTC oscillator setting */
/* RTC_CR: SC2P=0,SC4P=0,SC8P=0,SC16P=0,CLKO=1,OSCE=1,WPS=0,UM=0,SUP=0,WPE=0,SWR=0 */
#define SYSTEM_RTC_CR_VALUE            0x0300U             /* RTC_CR */

/* Low power mode enable */
/* SMC_PMPROT: AVLP=1,ALLS=1,AVLLS=1 */
#define SYSTEM_SMC_PMPROT_VALUE        0x2AU               /* SMC_PMPROT */

  
#define CPU_XTAL_CLK_HZ                50000000u           /* Value of the external crystal or oscillator clock frequency in Hz */
#define CPU_XTAL32k_CLK_HZ             32768u              /* Value of the external 32k crystal or oscillator clock frequency in Hz */
#define CPU_INT_SLOW_CLK_HZ            32768u              /* Value of the slow internal oscillator clock frequency in Hz  */
#define CPU_INT_FAST_CLK_HZ            4000000u            /* Value of the fast internal oscillator clock frequency in Hz  */
#define CPU_INT_IRC_CLK_HZ             48000000u           /* Value of the 48M internal oscillator clock frequency in Hz  */

/* RTC oscillator setting */
/* RTC_CR: SC2P=0,SC4P=0,SC8P=0,SC16P=0,CLKO=1,OSCE=1,WPS=0,UM=0,SUP=0,WPE=0,SWR=0 */
#define SYSTEM_RTC_CR_VALUE            0x0300U             /* RTC_CR */

/* Low power mode enable */
/* SMC_PMPROT: AVLP=1,ALLS=1,AVLLS=1 */
#define SYSTEM_SMC_PMPROT_VALUE        0x2AU               /* SMC_PMPROT */

  
typedef struct{
    uint8   pllPRDIV;            //外部晶振除数因子
    uint8   pllVDIV;             //外部晶振乘积因子
  }MCGPLL ;

typedef struct{
    uint8   coreDIV;            //Core除数因子
    uint8   busDIV;             //bus除数因子
    uint8   FlexBusDIV;         //FlexBus除数因子
    uint8   FlashDIV;           //Flash除数因子
  }SIMDIV ;
  
#define BUSCLOCKMAX          60     //bus clock最大频率
#define FLEXCLOCKMAX         50    //FLEX clock最大频率
#define FLASHCLOCKMAX        25    //FLASH clock最大频率


	
typedef enum system_option
{
    PLL50    = 50,
    PLL60    = 60,
    PLL70    = 70,
    PLL80    = 80,
    PLL90    = 90,
    PLL100   = 100,
    PLL110   = 110,
    PLL120   = 120,
    PLL130   = 130,
    PLL140   = 140,
    PLL150   = 150,
    PLL160   = 160,
    PLL170   = 170,
    PLL180   = 180,
    PLL190   = 190,
    PLL200   = 200, 
    PLL210   = 210,
    PLL220   = 220,
    PLL230   = 230,
    PLL240   = 240,
    PLL250   = 250,
    PLL260   = 260,
    PLL270   = 270,
    PLL280   = 280,
    PLL290   = 290,
    PLL300   = 300,
} system_option;
	
/***************
外部晶振 单位 M
***************/

#define OSCCLK    16

/**************
system频率定义
***************/

#define  SYSTEMCLOCK   PLL120



void system_init(system_option SYSPLL) ;
__RAMFUN void set_sys_dividers(uint32 outdiv1, uint32 outdiv2, uint32 outdiv3, uint32 outdiv4) ;



/**
 * @brief Setup the microcontroller system.
 *
 * Typically this function configures the oscillator (PLL) that is part of the
 * microcontroller device. For systems with variable clock speed it also updates
 * the variable SystemCoreClock. SystemInit is called from startup_device file.
 */
void SystemInit (void);

/**
 * @brief Updates the SystemCoreClock variable.
 *
 * It must be called whenever the core clock is changed during program
 * execution. SystemCoreClockUpdate() evaluates the clock register settings and calculates
 * the current core clock.
 */
void LanSystemCoreClockUpdate (uint32 * sysclkKH,uint32 *busclkKH) ;

#ifdef __cplusplus
}
#endif

#endif  /* #if !defined(SYSTEM_MK64F12_H_) */
