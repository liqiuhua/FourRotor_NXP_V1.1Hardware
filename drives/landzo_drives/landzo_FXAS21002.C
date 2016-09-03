
#include "include.h"
#include "landzo_FXAS21002.h"
//#include "math.h"
#define FXASWRITE                      0X7F
#define FXASREAD                       0X80

uint8 Gyro_flag=0;

#define IICEorr    (0)
#define IICOK      (1)

/*******
???????
********/
#define  FXA_SCN                   PTB20_OUT
#define  FXA_SCK                   PTB21_OUT
#define  FXA_MISO                  PTB23_INT
#define  FXA_MOSI                  PTB22_OUT

typeGYRO gyro_rad_s;


/*************************************************************************
*                             ??????????
*
*  ????:FXAS_SPI_init
*  ????: ??SPI???
*  ????:	
*  ????:?
*  ????:2015-5-15   ???
*  ?    ?:
*************************************************************************/
void FXAS_SPI_init(void)
{
  gpio_init(PTB20,GPO,0) ;
  gpio_init(PTB23,GPI_UP,0) ;
  gpio_init(PTB21,GPO,1) ;
	gpio_init(PTB22,GPO,1) ;
}
/*************************************************************************
*                             ??????????
*
*  ????:FXAS_SPI_RW
*  ????: SPI?????
*  ????:	
*  ????:?
*  ????:2015-5-15   ???
*  ?    ?:
*************************************************************************/

void SPI_delay(void)
{
   volatile uint16_t ii;
   for(ii=0;ii<19;ii++)  ;
    //  asm("nop");   
}

uint8 FXAS_SPI_RW(uint8 value)
{
    uint8 bit_ctr,REvalue = 0;
    
    FXA_SCK = 0; // ..then set SCK low again
    SPI_delay() ;
    for(bit_ctr=0;bit_ctr<8;bit_ctr++) // output 8-bit
    {
      if(value & 0x80)
      { FXA_MOSI=1;}
      else
      {FXA_MOSI=0;}
      value = (value << 1); // shift next bit into MSB.
      FXA_SCK = 1;
      
      // Set SCK high..
      REvalue <<= 1 ;
      REvalue |= FXA_MISO; // capture current MISO bit
      SPI_delay() ;
      FXA_SCK = 0; // ..then set SCK low again
      SPI_delay() ;
    }
    return REvalue ; // return read uint8
}

void FXASdelay(void){
	vuint16 i ;
	for(i =0 ;i< 500;i++) ;
}
uint8 spi_write(uint8 addr,uint8 data){
	uint8 status;
	FXA_SCN = 0 ;	
	FXASdelay();
	status = FXAS_SPI_RW(addr & FXASWRITE);
	FXASdelay();
	status = FXAS_SPI_RW(data);
	FXA_SCN = 1 ;
	return status;
}

uint8 spi_read(uint8 addr,uint8* data,uint8 nByte){
	uint8 status;
	uint8 i;	
	FXA_SCN = 0 ;
	FXASdelay();
	status = FXAS_SPI_RW((addr) | FXASREAD);
	
	for(i=0;i< nByte;i++){
		data[i] = FXAS_SPI_RW(0x0);
	}
	FXASdelay();
	FXA_SCN = 1 ;
	return status;
}
uint8 FXASs21002_init(void ){

	vuint8 status ,j;
	uint8 who[1];
	FXAS_SPI_init();	
	FXASdelay();
//		addr = ((0x0c)|0x80 );
	spi_read(0x0c,&who[0],1);
	spi_read(0x0c,&who[0],1);
	if((who[0] != FXAS21002_WHO_AM_I_VALUE))
		 return status=0;  
	
	spi_write(FXAS21002_CTRL_REG1,0x00);
	spi_write(FXAS21002_CTRL_REG0,0x80);
	spi_write(FXAS21002_CTRL_REG2,0x06);
	spi_write(FXAS21002_CTRL_REG1,0x06);
	
	return status=1;	
}

uint8 FXAS_muc_init(void){
	gpio_init(PTC0,GPO,0);
	PTC0_OUT=1;
	delay_ms(300);
	FXASs21002_init();
//	gpio_Interrupt_init(PTC1,GPI,FALLING);
	gpio_Interrupt_init(PTC2,GPI,RING);
	return 1;
}
float  LowPassFilter2p(float sample_freq, float cutoff_freq, float sample)
{
	static float _cutoff_freq, _b0,_b1,_b2,_a1,_a2,_delay_element_1,_delay_element_2;
    _cutoff_freq = cutoff_freq;
		if (_cutoff_freq <= 0.0f) {
        // no filtering
        return sample;
    }
    float fr = sample_freq/_cutoff_freq;
    float ohm =tanf(0.05f);
    float c = 1.0f+2.0f*cosf(M_PI_F/4.0f)*ohm + ohm*ohm;
    _b0 = ohm*ohm/c;
    _b1 = 2.0f*_b0;
    _b2 = _b0;
    _a1 = 2.0f*(ohm*ohm-1.0f)/c;
    _a2 = (1.0f-2.0f*cosf(M_PI_F/4.0f)*ohm+ohm*ohm)/c;
		
    float delay_element_0 = sample - _delay_element_1 * _a1 - _delay_element_2 * _a2;

    float output = delay_element_0 * _b0 + _delay_element_1 * _b1 + _delay_element_2 * _b2;
    
    _delay_element_2 = _delay_element_1;
    _delay_element_1 = delay_element_0;

    // return the value.  Should be no need to check limits
    return output;
}
float  LowPassFilter2p_Y(float sample_freq, float cutoff_freq, float sample)
{
	static float _cutoff_freq, _b0,_b1,_b2,_a1,_a2,_delay_element_1,_delay_element_2;
    _cutoff_freq = cutoff_freq;
		if (_cutoff_freq <= 0.0f) {
        // no filtering
        return sample;
    }
    float fr = sample_freq/_cutoff_freq;
    float ohm =tanf(0.05f);
    float c = 1.0f+2.0f*cosf(M_PI_F/4.0f)*ohm + ohm*ohm;
    _b0 = ohm*ohm/c;
    _b1 = 2.0f*_b0;
    _b2 = _b0;
    _a1 = 2.0f*(ohm*ohm-1.0f)/c;
    _a2 = (1.0f-2.0f*cosf(M_PI_F/4.0f)*ohm+ohm*ohm)/c;
		
    float delay_element_0 = sample - _delay_element_1 * _a1 - _delay_element_2 * _a2;

    float output = delay_element_0 * _b0 + _delay_element_1 * _b1 + _delay_element_2 * _b2;
    
    _delay_element_2 = _delay_element_1;
    _delay_element_1 = delay_element_0;

    // return the value.  Should be no need to check limits
    return output;
}
float  LowPassFilter2p_Z(float sample_freq, float cutoff_freq, float sample)
{
	static float _cutoff_freq, _b0,_b1,_b2,_a1,_a2,_delay_element_1,_delay_element_2;
    _cutoff_freq = cutoff_freq;
		if (_cutoff_freq <= 0.0f) {
        // no filtering
        return sample;
    }
    float fr = sample_freq/_cutoff_freq;
    float ohm =tanf(0.05f);
    float c = 1.0f+2.0f*cosf(M_PI_F/4.0f)*ohm + ohm*ohm;
    _b0 = ohm*ohm/c;
    _b1 = 2.0f*_b0;
    _b2 = _b0;
    _a1 = 2.0f*(ohm*ohm-1.0f)/c;
    _a2 = (1.0f-2.0f*cosf(M_PI_F/4.0f)*ohm+ohm*ohm)/c;
		
    float delay_element_0 = sample - _delay_element_1 * _a1 - _delay_element_2 * _a2;

    float output = delay_element_0 * _b0 + _delay_element_1 * _b1 + _delay_element_2 * _b2;
    
    _delay_element_2 = _delay_element_1;
    _delay_element_1 = delay_element_0;

    // return the value.  Should be no need to check limits
    return output;
}

typeGYRO GYRO_offset;
	float  GYRO_sum_X,GYRO_sum_Y,GYRO_sum_Z;
unsigned char GYRO_offset_flag = 1;
unsigned char G_offset_count = 0;
uint8 FXAS21002_ReadData(typeGYRO* GYRO ){
  uint8 spi_Buffer[6];
	uint8 status;

//	if(1==GYRO_offset_flag){
//			spi_read(0x01,&spi_Buffer[0],6);
//			GYRO_sum_X += ((spi_Buffer[0]<<8 ) | spi_Buffer[1]);
//			GYRO_sum_Y += ((spi_Buffer[2]<<8 ) | spi_Buffer[3]);
//			GYRO_sum_Z += ((spi_Buffer[4]<<8 ) | spi_Buffer[5]);
////			delay_ms(5);
//			G_offset_count++;
//		
//		if(!(G_offset_count<20)){
//				G_offset_count=0;
				GYRO_offset_flag=0;
//				GYRO_offset.X = GYRO_sum_X /20.00f;
//				GYRO_offset.Y = GYRO_sum_Y /20.00f;
//				GYRO_offset.Z = GYRO_sum_Z /20.00f;
//		}
//	return status;
//	}

	spi_read(0x01,&spi_Buffer[0],6);
	
	GYRO->X = ((spi_Buffer[0]<<8 ) | spi_Buffer[1]) - GYRO_offset.X;
	GYRO->Y = ((spi_Buffer[2]<<8 ) | spi_Buffer[3]) - GYRO_offset.Y;
	GYRO->Z = ((spi_Buffer[4]<<8 ) | spi_Buffer[5]) - GYRO_offset.Z;
//LowPassFilter2p(400.0f, 1000.0f,GYRO_FXAS.X);
	gyro_rad_s.X = (float)(GYRO->X) * GYRO_RAW_TO_RAD_S;
	gyro_rad_s.Y = (float)(GYRO->Y) * GYRO_RAW_TO_RAD_S;
	gyro_rad_s.Z = (float)(GYRO->Z) * GYRO_RAW_TO_RAD_S;
	return status;
	
}
