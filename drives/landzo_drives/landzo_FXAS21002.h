#ifndef __landzo_FXAS21002_H_
#define __landzo_FXAS21002_H_

#include "common.h"
#include "include.h"
#define ONE_G	9.80665f
#define M_PI_F 3.14159265358979323846f
// -2000 to 2000 degrees/s, 16 bit signed register, deg to rad conversion
#define GYRO_RAW_TO_RAD_S 	 (2000.0f / 32768.0f * M_PI_F / 180.0f)

// FXAS21002 registers and constants
#define FXAS21002_OUT_X_MSB       	  	0x01
#define FXAS21002_WHO_AM_I      			  0x0C
#define FXAS21002_XYZ_DATA_CFG       	  0x0E
#define FXAS21002_CTRL_REG1        	 	  0x13
#define FXAS21002_CTRL_REG0        	 	  0x0D
#define FXAS21002_CTRL_REG2        	 	  0x14
#define INT_SOURCE_FLAG                 0x0B
#define FXAS21002_M_CTRL_REG1         	0x5B
#define FXAS21002_M_CTRL_REG2        	  0x5C
#define FXAS21002_WHO_AM_I_VALUE     	  0xD7

// quaternion structure definition
typedef struct 
{
	int16_t X;
	int16_t Y;
	int16_t Z;
} typeGYRO ;
extern typeGYRO gyro_rad_s;
extern uint8 Gyro_flag;
extern unsigned char GYRO_offset_flag;

uint8 FXAS_muc_init(void) ;    //INIT
uint8 FXAS21002_ReadData(typeGYRO* GYRO) ;
uint8 spi_read(uint8 addr,uint8* data,uint8 nByte);
float  LowPassFilter2p(float sample_freq, float cutoff_freq, float sample);
float  LowPassFilter2p_Y(float sample_freq, float cutoff_freq, float sample);
float  LowPassFilter2p_Z(float sample_freq, float cutoff_freq, float sample);
#endif

