#ifndef __landzo_fxos8700_H_
#define __landzo_fxos8700_H_

#include "common.h"
#include "include.h"
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

extern uint8 foxaccflag;
extern uint8 foxmagflag;


#define SPIWRITE                      0X80
#define SPIREAD                       0X7F

// channel components
#define CHX 0
#define CHY 1
#define CHZ 2

// FXOS8700 registers and constants
#define FXOS8700_OUT_X_MSB       	  	0x01
#define FXOS8700_WHO_AM_I      			  0x0D
#define FXOS8700_XYZ_DATA_CFG       	0x0E
#define FXOS8700_HP_FILTER_CUTOFF  	 	0x0F
#define FXOS8700_CTRL_REG1        	 	0x2A
#define FXOS8700_CTRL_REG2        	 	0x2B
#define FXOS8700_CTRL_REG3        	 	0x2C
#define FXOS8700_CTRL_REG4        	 	0x2D
#define FXOS8700_CTRL_REG5        	 	0x2E
#define FXOS8700_M_OUT_X_MSB          0X33
#define FXOS8700_M_CTRL_REG1         	0x5B
#define FXOS8700_M_CTRL_REG2        	0x5C
#define FXOS8700_WHO_AM_I_VALUE     	0xC7

// magnetometer sensor structure definition
typedef struct  
{
	int16 iBs[3];			// uncalibrated sensor frame readings (counts)
	float fBs[3];			// uncalibrated sensor frame readings (uT)
	int16 iCountsPeruT;		// initialized to COUNTSPERUT
	float fuTPerCount;		// initialized to FUTPERCOUNT
	float fCountsPeruT;		// initialized to FCOUNTSPERUT
	uint8 iWhoAmI;			// sensor whoami
} MagSensor;

// quaternion structure definition
typedef struct 
{
	float q0;	// scalar component
	float q1;	// x vector component
	float q2;	// y vector component
	float q3;	// z vector component
} fquaternion;
// accelerometer sensor structure definition
typedef struct 
{
	int16_t iGs[3];			    // 25Hz sensor frame integer readings (counts)
	int16_t iCountsPerg;		// initialized to COUNTSPERg
	float_t fGs[3];			    // 25Hz sensor frame readings (g)
	float_t fgPerCount;		  // initialized to FGPERCOUNT
	uint8 iWhoAmI;			  // sensor whoami
} AccelSensor;

extern unsigned char ACC_offset_flag;
	
uint8 FXOS8700CQ_init(AccelSensor *pthisAccel,MagSensor *pthisMag ) ;   //
uint8 FXOS8700_ReadData( AccelSensor *pthisAccel,  MagSensor *pthisMag) ;   //
uint8 FXO_MUC_init(AccelSensor *pthisAccel,  MagSensor *pthisMag) ;
void FXOS_SPI_ReadDate(uint8 addr , uint8 *buff ,uint8 cont);
uint8 FXOS8700_ReadDataACC( AccelSensor *pthisAccel) ;
uint8 FXOS8700_ReadDataMAG(  MagSensor *pthisMag);
void MagSensorMAXMIN(int16 *magsensormax,int16 *magsensormin,int16 *magsensoroff,int16*magsensordata) ;
uint16 MagSensorangle(int16 *magsensoroff,int16*magsensordata,int16 * magsenorcal) ;
void MagSensorfilter(int16 *magsensordatain ,int16* magsensordataout);
#endif

