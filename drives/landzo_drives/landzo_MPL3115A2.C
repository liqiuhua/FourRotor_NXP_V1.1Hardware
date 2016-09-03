
#include "include.h"
#include "landzo_MPL3115A2.h"

uint8 mpl3115_init(void){
	uint8 status ;
	I2C_init(KI2C0,IIC100KB);
	status = I2C_ReadAddr(KI2C0,MPL31152_ADD,MPL3115_WHO_AM_I);
	if(status != 0xc4)
		return 0 ;
	delay_ms(1);
	/* Set to Altimeter with an OSR = 128 */
	I2C_WriteAddr(KI2C0,MPL31152_ADD,MPL3115_CTRL_REG1,0x04);//b8
	delay_ms(1);
	/* Enable Data Flags in PT_DATA_CFG */
	I2C_WriteAddr(KI2C0,MPL31152_ADD,MPL3115_PT_DATA_CFG,0x07);
	delay_ms(1);
  /* Set INT to Active Low Open Drain */	
	I2C_WriteAddr(KI2C0,MPL31152_ADD,MPL3115_CTRL_REG3,0x11);
	delay_ms(1);
	/* Enable DRDY Interrupt */
	I2C_WriteAddr(KI2C0,MPL31152_ADD,MPL3115_CTRL_REG4,0x80);
	delay_ms(1);
	
	I2C_WriteAddr(KI2C0,MPL31152_ADD,MPL3115_CTRL_REG5,0x00);
	delay_ms(1);
	/* Set Active */
	I2C_WriteAddr(KI2C0,MPL31152_ADD,MPL3115_CTRL_REG1,0xB9);

	return 1 ;
	
}

uint8 mpl_mcu_init(void){
	
	if(!mpl3115_init()) return 0 ;
	gpio_Interrupt_init(PTE27,GPI,RING);
	
	return 1 ;
}

uint16  Gpressure ,Gtemp ;
double Att;
void mpl_readdata(void){
	uint8 pr[3] ;//temp[2];
	pr[0] =I2C_ReadAddr(KI2C0,MPL31152_ADD,MPL3115_OUT_P_MSB);
	delay_ms(1);
	pr[1] =I2C_ReadAddr(KI2C0,MPL31152_ADD,MPL3115_OUT_P_CSB);
	delay_ms(1);
	pr[2] =I2C_ReadAddr(KI2C0,MPL31152_ADD,MPL3115_OUT_P_LSB);
	delay_ms(1);
//	temp[0] =I2C_ReadAddr(KI2C0,MPL31152_ADD,MPL3115_OUT_T_MSB);
//	delay_ms(1);
//	temp[1] =I2C_ReadAddr(KI2C0,MPL31152_ADD,MPL3115_OUT_T_LSB);
	
	Att=(((double)((pr[0]<<8) | pr[1])) + ((double)(pr[2]>>4))*0.0625)*1000;
	
}
