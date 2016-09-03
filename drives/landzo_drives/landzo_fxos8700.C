
#include "include.h"
#include "landzo_fxos8700.h"
uint8 foxaccflag = 0,foxmagflag = 0 ;




/*******
???????
********/
#define  FXO_SCN                   PTB10_OUT
#define  FXO_SCK                   PTB11_OUT
#define  FXO_MISO                  PTB17_INT
#define  FXO_MOSI                  PTB16_OUT


/*************************************************************************
*                             ??????????
*
*  ????:FXOS_SPI_init
*  ????: ??SPI???
*  ????:	
*  ????:?
*  ????:2015-5-15   ???
*  ?    ?:
*************************************************************************/
void FXOS_SPI_init(void)
{
  gpio_init(PTB10,GPO,0) ;
  gpio_init(PTB17,GPI_UP,0) ;
  gpio_init(PTB11,GPO,1) ;
	gpio_init(PTB16,GPO,1) ;
}
/*************************************************************************
*                             ??????????
*
*  ????:FXOS_SPI_RW
*  ????: SPI?????
*  ????:	
*  ????:?
*  ????:2015-5-15   ???
*  ?    ?:
*************************************************************************/

void FXO_SPI_delay(void)
{
   volatile uint16_t ii;
   for(ii=0;ii<4;ii++)  ;
    //  asm("nop");   
}

uint8 FXOS_FXO_SPI_RW(uint8 value)
{
    uint8 bit_ctr,REvalue = 0;
    
    FXO_SCK = 0; // ..then set SCK low again
    FXO_SPI_delay() ;
    for(bit_ctr=0;bit_ctr<8;bit_ctr++) // output 8-bit
    {
      if(value & 0x80)
      { FXO_MOSI=1;}
      else
      {FXO_MOSI=0;}
      value = (value << 1); // shift next bit into MSB.
      FXO_SCK = 1;
      
      // if(MISO){ value =value| 0X01; } else {
  //    REvalue =REvalue& 0XFE; 
      // Set SCK high..
      REvalue <<= 1 ;
      REvalue |= FXO_MISO; // capture current MISO bit
      FXO_SPI_delay() ;
      FXO_SCK = 0; // ..then set SCK low again
      FXO_SPI_delay() ;
    }
    return REvalue ; // return read uint8
}

void Fxo_delay(uint16 time){
	vuint16  tdelay ,ti ;
	if(time <1) time =1 ;
	for(ti = 0 ; ti < time ;ti ++ ){
		for(tdelay =0 ; tdelay < 20 ; tdelay++);
	}
}
void FXOS_SPI_WriteDate(uint8 addr,uint8 data){
	uint8 fxosaddr ;
	FXO_SCN = 0 ;
	Fxo_delay(1);
	fxosaddr = addr|SPIWRITE ; 
	FXOS_FXO_SPI_RW(fxosaddr);
	FXOS_FXO_SPI_RW(addr);
	FXOS_FXO_SPI_RW(data);
	Fxo_delay(1);
	FXO_SCN = 1 ; 	
}

void FXOS_SPI_ReadDate(uint8 addr , uint8 *buff ,uint8 cont){
	uint8 fxosaddr ,datacont;
	fxosaddr = addr&SPIREAD;
	FXO_SCN = 0 ;
	Fxo_delay(1);
	FXOS_FXO_SPI_RW(fxosaddr);
	FXOS_FXO_SPI_RW(addr);
	for(datacont = 0 ; datacont < cont ;datacont++){
		*buff = FXOS_FXO_SPI_RW(0);
		 buff ++ ;
	}
  Fxo_delay(1);
	FXO_SCN = 1 ;
}
uint8 FXOS8700CQ_init(AccelSensor *pthisAccel,MagSensor *pthisMag ){

	uint8 status ;
//	uint8 addr ;

		

   FXOS_SPI_ReadDate(FXOS8700_WHO_AM_I,&status,1);
	 FXOS_SPI_ReadDate(FXOS8700_WHO_AM_I,&status,1);
	 FXOS_SPI_ReadDate(FXOS8700_WHO_AM_I,&status,1);
	// return immediately with error if incorrect
	if (status != FXOS8700_WHO_AM_I_VALUE)
	{
		pthisAccel->iWhoAmI = pthisMag->iWhoAmI = 0;
		return status;
	}
	// write 0000 0000 = 0x00 to CTRL_REG1 to place FXOS8700 into standby
	// [7-1] = 0000 000
	// [0]: active=0
	
//	addr = FXOS8700_CTRL_REG1|SPIWRITE ;
//	spi_wr_byte(KSPI1,addr);
//	status &= spi_wr_byte(KSPI1,00);

	  FXOS_SPI_WriteDate(FXOS8700_CTRL_REG1,0x00);
	// write 0001 1111 = 0x1F to M_CTRL_REG1
	// [7]: m_acal=0: auto calibration disabled
	// [6]: m_rst=0: one-shot magnetic reset disabled
	// [5]: m_ost=0: one-shot magnetic measurement disabled
	// [4-2]: m_os=111=7: maximum oversampling to reduce magnetometer noise
	// [1-0]: m_hms=11=3: select hybrid mode with accel and magnetometer active


		FXOS_SPI_WriteDate(FXOS8700_M_CTRL_REG1,0x1f);
//	addr = FXOS8700_M_CTRL_REG1|SPIWRITE ;
//	spi_wr_byte(KSPI1,addr);
//	status &= spi_wr_byte(KSPI1,0x1f);
	
	// write 0010 0000 = 0x20 to magnetometer control register 2
	// [7]: reserved
	// [6]: reserved
	// [5]: hyb_autoinc_mode=1 to map the magnetometer registers to follow the accelerometer registers
	// [4]: m_maxmin_dis=0 to retain default min/max latching even though not used
	// [3]: m_maxmin_dis_ths=0
	// [2]: m_maxmin_rst=0
	// [1-0]: m_rst_cnt=00 to enable magnetic reset each cycle

		FXOS_SPI_WriteDate(FXOS8700_M_CTRL_REG2,0x20);
//	addr = FXOS8700_M_CTRL_REG2|SPIWRITE ;
//	spi_wr_byte(KSPI1,addr);
//	status &= spi_wr_byte(KSPI1,0x20);
	
	// write 0000 0001= 0x01 to XYZ_DATA_CFG register
	// [7]: reserved
	// [6]: reserved
	// [5]: reserved
	// [4]: hpf_out=0
	// [3]: reserved
	// [2]: reserved
	// [1-0]: fs=01 for 4g mode: 2048 counts / g = 8192 counts / g after 2 bit left shift


		FXOS_SPI_WriteDate(FXOS8700_XYZ_DATA_CFG,0x01);
//	addr = FXOS8700_XYZ_DATA_CFG|SPIWRITE ;
//	spi_wr_byte(KSPI1,addr);
//	status &= spi_wr_byte(KSPI1,0x01);
	
	// write 0000 0010 = 0x02 to CTRL_REG2 to set MODS bits
	// [7]: st=0: self test disabled
	// [6]: rst=0: reset disabled
	// [5]: unused
	// [4-3]: smods=00
	// [2]: slpe=0: auto sleep disabled
	// [1-0]: mods=10 for high resolution (maximum over sampling)


//	addr = FXOS8700_CTRL_REG2|SPIWRITE ;
//	spi_wr_byte(KSPI1,addr);
//	status &= spi_wr_byte(KSPI1,0x02);

		FXOS_SPI_WriteDate(FXOS8700_CTRL_REG2,0x02);
		FXOS_SPI_WriteDate(FXOS8700_CTRL_REG3,0x01);
		FXOS_SPI_WriteDate(FXOS8700_CTRL_REG4,0x01);
		FXOS_SPI_WriteDate(FXOS8700_HP_FILTER_CUTOFF,0x20);
	// write 0010 0101 = 0x25 to accelerometer control register 1
	// [7-6]: aslp_rate=00
	// [5-3]: dr=100 for 25Hz data rate (when in hybrid mode)
	// [2]: lnoise=1 for low noise mode (since we're in 4g mode)
	// [1]: f_read=0 for normal 16 bit reads
	// [0]: active=1 to take the part out of standby and enable sampling
//	addr = FXOS8700_CTRL_REG1|SPIWRITE ;
//	spi_wr_byte(KSPI1,addr);
//	status &= spi_wr_byte(KSPI1,0x25);

		FXOS_SPI_WriteDate(FXOS8700_CTRL_REG1,0x05);
	//	FXOS_SPI_ReadDate(FXOS8700_CTRL_REG1,&status,1);
	#define FXOS8700_COUNTSPERG 8192			
	pthisAccel->iCountsPerg = FXOS8700_COUNTSPERG;
	pthisAccel->fgPerCount = 1.0F / FXOS8700_COUNTSPERG;
  #define FXOS8700_COUNTSPERUT 10						
	pthisMag->iCountsPeruT = FXOS8700_COUNTSPERUT;
	pthisMag->fCountsPeruT = (float)FXOS8700_COUNTSPERUT;
	pthisMag->fuTPerCount = 1.0F / FXOS8700_COUNTSPERUT;

  
	return status;
	

}

uint8 FXO_MUC_init(AccelSensor *pthisAccel,  MagSensor *pthisMag){

	gpio_init(PTB7,GPO,1) ;
	PTB7_OUT = 0 ;
	Fxo_delay(500) ;
  gpio_Interrupt_init(PTB8,GPI,FALLING);
	gpio_Interrupt_init(PTB9,GPI,FALLING);
	FXOS_SPI_init();
	(void)FXOS8700CQ_init(pthisAccel,pthisMag) ;
	return 1;
	
}

uint8 FXOS8700_ReadData( AccelSensor *pthisAccel,  MagSensor *pthisMag){
  uint8 spi_Buffer[13];
	int8 status;
	int8 i;
	// spi_wr_stream(KSPI1,spisnetbuf,13,spi_Buffer);
	FXOS_SPI_ReadDate(FXOS8700_OUT_X_MSB,spi_Buffer,12);
		// process the six channels
	for (i = CHX; i <= CHZ; i++)
	{
		// place the 12 bytes read into the accelerometer and magnetometer structures
		pthisAccel->iGs[i] = (spi_Buffer[2 * i] << 8) | spi_Buffer[2 * i + 1];
		pthisMag->iBs[i] = (spi_Buffer[2 * i + 6] << 8) | spi_Buffer[2 * i + 7];

		// check for -32768  since this value cannot be negated in a later HAL operation
		if (pthisAccel->iGs[i] == -32768) pthisAccel->iGs[i]++;
		if (pthisMag->iBs[i] == -32768) pthisMag->iBs[i]++;
		
		// convert to physical units of g and uT
		pthisAccel->fGs[i] = (float) pthisAccel->iGs[i] * pthisAccel->fgPerCount;			
		pthisMag->fBs[i] = (float) pthisMag->iBs[i] * pthisMag->fuTPerCount;	
	}
	
	return status;
	
}
AccelSensor ACC_offset;
unsigned char ACC_offset_flag = 1;
unsigned char A_offset_count = 0;
	float ACC_sum_X,ACC_sum_Y,ACC_sum_Z;
uint8 FXOS8700_ReadDataACC( AccelSensor *pthisAccel){
  uint8 spi_Buffer[13];
	int8 status;
	int8 i;
//	if(1==ACC_offset_flag){
//			FXOS_SPI_ReadDate(FXOS8700_OUT_X_MSB,spi_Buffer,6);
//			ACC_sum_X+=(spi_Buffer[0]<<8 ) | spi_Buffer[1];
//			ACC_sum_Y+=(spi_Buffer[2]<<8 ) | spi_Buffer[3];
//			ACC_sum_Z+=(spi_Buffer[4]<<8 ) | spi_Buffer[5];
////			delay_ms(5);
//			A_offset_count++;
//		
//		if(!(A_offset_count<20)){
//				A_offset_count=0;
				ACC_offset_flag=0;
//				ACC_offset.iGs[0]=ACC_sum_X /20.00f;
//				ACC_offset.iGs[1]=ACC_sum_Y/20.00f;
//				ACC_offset.iGs[2]=ACC_sum_Z /20.00f;
//		}
//	return status;
//	}

	FXOS_SPI_ReadDate(FXOS8700_OUT_X_MSB,spi_Buffer,6);
		// process the six channels
	for (i = CHX; i <= CHZ; i++)
	{
		// place the 12 bytes read into the accelerometer and magnetometer structures
		pthisAccel->iGs[i] = (spi_Buffer[2 * i] << 8) | spi_Buffer[2 * i + 1]-ACC_offset.iGs[i];

		// check for -32768  since this value cannot be negated in a later HAL operation
		if (pthisAccel->iGs[i] == -32768) pthisAccel->iGs[i]++;
		
		// convert to physical units of g and uT
		pthisAccel->fGs[i] = (float) pthisAccel->iGs[i] * pthisAccel->fgPerCount;			
	}
	
	return status;
	

}


uint8 FXOS8700_ReadDataMAG(  MagSensor *pthisMag){
  uint8 spi_Buffer[13];
	int8 status;
	int8 i;


	FXOS_SPI_ReadDate(FXOS8700_M_OUT_X_MSB,spi_Buffer,6);
		// process the six channels
	for (i = CHX; i <= CHZ; i++)
	{
		// place the 12 bytes read into the accelerometer and magnetometer structures
		pthisMag->iBs[i] = (spi_Buffer[2 * i + 6] << 8) | spi_Buffer[2 * i + 7];

		// check for -32768  since this value cannot be negated in a later HAL operation
		if (pthisMag->iBs[i] == -32768) pthisMag->iBs[i]++;
		
		// convert to physical units of g and uT		
		pthisMag->fBs[i] = (float) pthisMag->iBs[i] * pthisMag->fuTPerCount;	
	}
	
	return status;
	
}
#define FILTERCOUNT      	  	10
void MagSensorfilter(int16 *magsensordatain ,int16* magsensordataout){
	static int16 magsendataAryy[3] ;
	uint8 count ;
	for(count = 0 ; count < 3 ; count ++){
		magsendataAryy[count]  = (magsendataAryy[count]*(FILTERCOUNT-1) + magsensordatain[count])/FILTERCOUNT ;
	  magsensordataout[count] = 	magsendataAryy[count];
	}
}
void MagSensorMAXMIN(int16 *magsensormax,int16 *magsensormin,int16 *magsensoroff,int16*magsensordata){
   uint8 cout;
	for(cout = 0 ; cout < 3 ; cout ++ )
	{
		if(magsensormax[cout] < magsensordata[cout]) {
			magsensormax[cout] = magsensordata[cout] ;
			magsensoroff[cout] = (magsensormax[cout] + magsensormin[cout])>>1 ;
		}
		else if(magsensormin[cout] > magsensordata[cout]) {
			magsensormin[cout] = magsensordata[cout] ;
			magsensoroff[cout] = (magsensormax[cout] + magsensormin[cout])>>1 ;
		}
	}

}
uint16 MagSensorangle(int16 *magsensoroff,int16*magsensordata,int16 * magsenorcal){
vuint16 count ;
float  datac ;
//	int16 magsenorda[3] ;
	for(count = 0 ; count < 3 ; count ++){
		magsenorcal[count] = magsensordata[count]  - magsensoroff[count] ;
	}
	if(magsenorcal[0]>0){
		if(magsenorcal[1] >= 0){
			datac = atan((float)magsenorcal[1]/(float)magsenorcal[0]) ;
		  count = datac*180.0f/3.14f ;
		}
		else {
			magsenorcal[1] = -magsenorcal[1] ;
			datac = atan((float)magsenorcal[1]/(float)magsenorcal[0]) ;
			count = 360.0f -datac*180.0f/3.14f ;
		}
	}
	else if(magsenorcal[0] < 0){
		if(magsenorcal[1] <= 0){
			datac = atan((float)magsenorcal[1]/(float)magsenorcal[0]) ;
		  count = datac*180.0f/3.14f + 180.0f;
		}
		else{
			magsenorcal[0] = - magsenorcal[0] ;
			datac = atan((float)magsenorcal[1]/(float)magsenorcal[0]) ;
		  count = 180.0f -datac*180.0f/3.14f ;
		}
	}
	return count ;
	
}
