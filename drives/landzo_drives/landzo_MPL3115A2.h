#ifndef __landzo_MPL3115A2_H_
#define __landzo_MPL3115A2_H_

#include "common.h"
//#include "include.h"


/***********************************************************************************************
 **
 **  MPL3115 Sensor Internal Registers
 */
 
#define MPL31152_ADD     0X60

 
enum
{
	MPL3115_STATUS_00 = 0,          // 0x00
	MPL3115_OUT_P_MSB,              // 0x01
	MPL3115_OUT_P_CSB,              // 0x02
	MPL3115_OUT_P_LSB,              // 0x03
	MPL3115_OUT_T_MSB,              // 0x04
	MPL3115_OUT_T_LSB,              // 0x05
	MPL3115_OUT_DR_STATUS,          // 0x06
	MPL3115_OUT_P_DELTA_MSB,        // 0x07
	MPL3115_OUT_P_DELTA_CSB,        // 0x08
	MPL3115_OUT_P_DELTA_LSB,        // 0x09
	MPL3115_OUT_T_DELTA_MSB,        // 0x0A
	MPL3115_OUT_T_DELTA_LSB,        // 0x0B
	MPL3115_WHO_AM_I,               // 0x0C
	MPL3115_F_STATUS,               // 0x0D
	MPL3115_F_DATA,                 // 0x0E
	MPL3115_F_SETUP,                // 0x0F
	MPL3115_TIME_DLY,               // 0x10
	MPL3115_SYSMOD,                 // 0x11
	MPL3115_INT_SOURCE,             // 0x12
	MPL3115_PT_DATA_CFG,            // 0x13
	MPL3115_BAR_IN_MSB,             // 0x14
	MPL3115_BAR_IN_LSB,             // 0x15
	MPL3115_P_ARM_MSB,              // 0x16
	MPL3115_P_ARM_LSB,              // 0x17
	MPL3115_T_ARM,                  // 0x18
	MPL3115_P_ARM_WND_MSB,          // 0x19
	MPL3115_P_ARM_WND_LSB,          // 0x1A
	MPL3115_T_ARM_WND,              // 0x1B
	MPL3115_P_MIN_MSB,              // 0x1C
	MPL3115_P_MIN_CSB,              // 0x1D
	MPL3115_P_MIN_LSB,              // 0x1E
	MPL3115_T_MIN_MSB,              // 0x1F
	MPL3115_T_MIN_LSB,              // 0x20
	MPL3115_P_MAX_MSB,              // 0x21
	MPL3115_P_MAX_CSB,              // 0x22
	MPL3115_P_MAX_LSB,              // 0x23
	MPL3115_T_MAX_MSB,              // 0x24
	MPL3115_T_MAX_LSB,              // 0x25
	MPL3115_CTRL_REG1,              // 0x26
	MPL3115_CTRL_REG2,              // 0x27
	MPL3115_CTRL_REG3,              // 0x28
	MPL3115_CTRL_REG4,              // 0x29
	MPL3115_CTRL_REG5,              // 0x2A
	MPL3115_OFF_P,                  // 0x2B
	MPL3115_OFF_T,                  // 0x2C
	MPL3115_OFF_H                   // 0x2D
};


void mpl_readdata(void) ;
uint8 mpl_mcu_init(void) ;

#endif

