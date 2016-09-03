/***********************************************************
 * �ļ���       ��common.h
 * ˵��         �������ļ�
 * ����         ��landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://www.landzo.cn
 * ��汾       ��V1.0
 *ʱ��          ��15.5.26
************************************************************/
#ifndef _COMMON_H_
#define _COMMON_H_

/*
 * Include the generic CPU header file
 */
   
/**************
��Ƭ��ѡ��
**************/
#include "MK64F12.h"

#define __NVIC_PRIO_BITS          4

/*
 * Include the cpu specific header file
 */



  
#if defined(__ICCARM__)
#define     __RAMFUN   __ramfunc
#else
#define     __RAMFUN
#endif
	
	
typedef struct{
				float X;
				float Y;
				float Z;}T_FLOAT_XYZ;
typedef struct{
				float ROL;
				float PITCH;
				float YAW;}T_FLOAT_ANGEL;
typedef struct{
						int16_t X;
						int16_t Y;
						int16_t Z;
						}T_INT16_XYZ;
	
	
	
	//���ͱ����궨��
	typedef unsigned char	       uint8;  // 8 bits
	typedef unsigned short int	 uint16; // 16 bits
	typedef unsigned long int	   uint32; // 32 bits
	
	typedef char			           int8;   //  8 bits 
	typedef short int	       	   int16;  // 16 bits
	typedef int		       	       int32;  // 32 bits
	
	typedef volatile int8		     vint8;  //  8 bits
	typedef volatile int16	     vint16; // 16 bits
	typedef volatile int32	     vint32; // 32 bits
	
	typedef volatile uint8	     vuint8;  //  8 bits
	typedef volatile uint16	     vuint16; // 16 bits
	typedef volatile uint32      vuint32; // 32 bits
  
	typedef void(func)(void)            ;


typedef union 
{
    uint32  DU;
    uint16  W[2];
    uint8   B[4];
} Bittyp; 

 typedef struct 
    {
        uint32 b0: 1;
        uint32 b1: 1;
        uint32 b2: 1;
        uint32 b3: 1;
        uint32 b4: 1;
        uint32 b5: 1;
        uint32 b6: 1;
        uint32 b7: 1;
        uint32 b8: 1;
        uint32 b9: 1;
        uint32 b10: 1;
        uint32 b11: 1;
        uint32 b12: 1;
        uint32 b13: 1;
        uint32 b14: 1;
        uint32 b15: 1;
        uint32 b16: 1;
        uint32 b17: 1;
        uint32 b18: 1;
        uint32 b19: 1;
        uint32 b20: 1;
        uint32 b21: 1;
        uint32 b22: 1;
        uint32 b23: 1;
        uint32 b24: 1;
        uint32 b25: 1;
        uint32 b26: 1;
        uint32 b27: 1;
        uint32 b28: 1;
        uint32 b29: 1;
        uint32 b30: 1;
        uint32 b31: 1;
    }Bittyp1;

#define  SysTick_IRQn   -1

#include "core_cm4.h"

#include "stdlib.h"	
#include "math.h"
#include <assert.h>

/***********************
ϵͳ���õ�ʱ�Ӷ���
***********************/
#include "system_MK64F125.h"
#include "gpio_cfg.h"
#include  "port.h"
#include  "pin_cfg.h"

		/*!< Macro to enable all interrupts. */
#define EnableInterrupts __ASM volatile (" CPSIE i");

/*!< Macro to disable all interrupts. */
#define DisableInterrupts __ASM volatile (" CPSID i");

#define HIGH                             (1)
#define LOW                              (0)
#define OK                                (1)
#define ERROR                              (0)

extern uint32 SYSCLKKH,BUSCLKkh ;

#endif 
