/***********************************************************
 * �ļ���       ��i2c.c
 * ˵��         ��I2C�����������޸��Թٷ�����
 * ����         ��landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://www.landzo.cn
 * ��汾       ��V1.0
 *ʱ��          ��15.5.26
************************************************************/


#include "common.h"
#include "K6X_i2c.h"


unsigned char MasterTransmission;
unsigned char SlaveID;

volatile  I2C_Type *I2Cx[2] = {I2C0_BASE_PTR, I2C1_BASE_PTR}; //��������ָ�����鱣�� I2Cx �ĵ�ַ


/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�I2C_init
*  ����˵����I2C��ʼ�������ò����ʺ�����I2C
*  ����˵����I2Cn        ģ��ţ�I2C0��I2C1��
*  �������أ���
*  �޸�ʱ�䣺2012-1-20
*  ��    ע��
*************************************************************************/

uint16 I2C_ICR_MAP[0x40]  =
    {
        20, 22, 24, 26, 28, 30, 34, 40,  28,  32, 36, 40,  44,  48,  56, 68,  48,  56,
        64, 72, 80, 88, 104, 128, 80, 96, 112, 128, 144, 160, 192, 240, 160, 192, 224,
        256, 288, 320, 384, 480, 320, 384, 448, 512, 576, 640, 768, 960,640, 768, 896,
        1024, 1152, 1280, 1536, 1920, 1280, 1536, 1792, 2048, 2304, 2560, 3072, 3840
    };
void I2C_init(I2Cn i2cn,IICBaudK baud)
{
    uint8 mult,ICR_clock ,tem8p0,tem8p1;
    uint32 baudkH,clocktemp,busclock  ;
    baudkH = baud ;   
    busclock = BUSCLKkh;
    clocktemp = busclock/baudkH ;
    clocktemp/= 3840 ;
    tem8p0= busclock%3840 ;
    if(tem8p0) clocktemp ++ ;
    if(clocktemp > 4) 
    {  mult = 3 ;
       tem8p0 =  0x3f ;
    }
    else{
      mult = clocktemp >> 1 ;
      clocktemp = busclock /(clocktemp*baudkH);
    }
    /*************
    ���ݼ���������
    ����
    *************/
    ICR_clock = 0x41 ;
    baudkH = 0xffff ;
    for(tem8p1 = 0 ; tem8p1 < 0x40 ; tem8p1++){
      
      if(I2C_ICR_MAP[tem8p1] > clocktemp )
      { busclock = I2C_ICR_MAP[tem8p1] - clocktemp ;  }
      else{
        busclock = clocktemp - I2C_ICR_MAP[tem8p1]  ;
      }
      if(baudkH > busclock){
        baudkH = busclock ;
        ICR_clock = tem8p1 ;
      }
      
    }


    if(i2cn == KI2C0)
    {
        /* ����ʱ�� */
        SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;         //���� I2C0ʱ��
			

        /* ���� I2C0���ܵ� GPIO �ӿ� */
			if((I2C0_SCL == PTB0)||(I2C0_SCL == PTB2)||(I2C0_SCL == PTD8)){
					port_alt(I2C0_SCL,ALT2) ;
					port_alt(I2C0_SDA,ALT2) ;
			}
			else if(I2C0_SCL == PTE24){
				  port_alt(I2C0_SCL,ALT5) ;
					port_alt(I2C0_SDA,ALT5) ;
			}
			else if(I2C0_SCL == PTD2){
				  port_alt(I2C0_SCL,ALT7) ;
					port_alt(I2C0_SDA,ALT7) ;
			}
    }
    else
    {
        /* ����ʱ�� */
        SIM_SCGC4 |= SIM_SCGC4_I2C1_MASK;         //���� I2C1ʱ��

        /* ���� I2C1���ܵ� GPIO �ӿ� */
        if(I2C1_SCL == PTE1)
           port_alt(I2C1_SCL,ALT6) ;
        else if(I2C1_SCL == PTC10)
            port_alt(I2C1_SCL,ALT2) ;

        if(I2C1_SDA == PTE0)
            port_alt(I2C1_SDA,ALT6) ;
        else if (I2C1_SDA == PTC11)
            port_alt(I2C1_SDA,ALT2) ;

    }

    /* ����Ƶ�� */
    I2C_F_REG(I2Cx[i2cn])  = I2C_F_MULT(mult) | I2C_F_ICR(ICR_clock) ;  // I2C Frequency Divider register (I2Cx_F)  I2C��Ƶ�Ĵ���   I2C�������Ϊ 400k
    // I2C baud rate = bus speed (Hz)/(mul �� SCL divider)  ������ 50MHz/(1 ��128)=390.625kHz
    // SDA hold time = bus period (s) �� mul �� SDA hold value
    // SCL start hold time = bus period (s) �� mul �� SCL start hold value
    // SCL stop hold time = bus period (s) �� mul �� SCL stop hold value

    /* ʹ�� IIC1 */
    I2C_C1_REG(I2Cx[i2cn]) = I2C_C1_IICEN_MASK;
}

/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�I2C_StartTransmission
*  ����˵�������� I2C ����
*  ����˵����I2Cn        ģ��ţ�I2C0��I2C1��
*            SlaveID     7λ�ӻ���ַ
*            MSmode      ��дģʽ�� MWSR �� MRSW ��
*  �������أ���
*  �޸�ʱ�䣺2012-1-20
*  ��    ע��
*************************************************************************/
void I2C_StartTransmission (I2Cn i2cn, uint8 SlaveID, MSmode Mode)
{

    SlaveID = ( SlaveID << 1 ) | Mode ;            //ȷ��д��ַ�Ͷ���ַ

    /* send start signal */
    i2c_Start(i2cn);

    /* send ID with W/R bit */
    i2c_write_byte(i2cn, SlaveID);
}

/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�Pause
*  ����˵������ʱ��I2C��д����Ҫ��ʱһ��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-1-20
*  ��    ע���ڹٷ�������������ʱ��
*************************************************************************/
void I2C_delay(void)
{
    uint16_t n;
    for(n = 1; n < 50000; n++)      
    {
        __ASM volatile ("nop");  
    }
}

/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�I2C_ReadAddr
*  ����˵������ȡI2C�豸ָ����ַ�Ĵ���������
*  ����˵����I2Cn        ģ��ţ�I2C0��I2C1��
*            SlaveID     7λ�ӻ���ַ
*            Addr        �ӻ��ļĴ�����ַ
*  �������أ��ӻ��Ĵ���������
*  �޸�ʱ�䣺2012-1-20
*  ��    ע��
*************************************************************************/
uint8 I2C_ReadAddr(I2Cn i2cn, uint8 SlaveID, uint8 Addr)
{
    uint8 result;

    /* Send Slave Address */
    I2C_StartTransmission (i2cn, SlaveID, MWSR);
    i2c_Wait(i2cn);

    /* Write Register Address */
    i2c_write_byte(i2cn, Addr);
    i2c_Wait(i2cn);

    /* Do a repeated start */
    i2c_RepeatedStart(i2cn);

    /* Send Slave Address */
    i2c_write_byte(i2cn, ( SlaveID << 1) | MRSW );
    i2c_Wait(i2cn);

    /* Put in Rx Mode */
    i2c_PutinRxMode(i2cn);

    /* Turn off ACK since this is second to last byte being read*/
    i2c_DisableAck(i2cn); //��Ӧ��

    /* Dummy read ��ٶ�ȡ*/
    result = I2C_D_REG(I2Cx[i2cn]);
    i2c_Wait(i2cn);

    /* Send stop since about to read last byte */
    i2c_Stop(i2cn);

    /* Read byte */
    result = I2C_D_REG(I2Cx[i2cn]);

    return result;
}


/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�I2C_WriteData1
*  ����˵����д��һ���ֽ����ݵ�I2C�豸ָ���Ĵ�����ַ
*  ����˵����I2Cn        ģ��ţ�I2C0��I2C1��
*            SlaveID     7λ�ӻ���ַ
*            Addr        �ӻ��ļĴ�����ַ
*            Data        ����
*  �������أ���
*  �޸�ʱ�䣺2012-1-20
*  ��    ע��
*************************************************************************/
void I2C_WriteAddr(I2Cn i2cn, uint8 SlaveID, uint8 Addr, uint8 Data)
{
    /* send data to slave */
    I2C_StartTransmission(i2cn, SlaveID, MWSR);    //��������
    i2c_Wait(i2cn);

    i2c_write_byte(i2cn, Addr);                    //д��ַ
    i2c_Wait(i2cn);

    i2c_write_byte(i2cn, Data);                    //д����
    i2c_Wait(i2cn);

    i2c_Stop(i2cn);

    I2C_delay();                                        //��ʱ̫�̵Ļ�������д����
}

/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�I2C_WriteDataN
*  ����˵����д��һ���ֽ����ݵ�I2C�豸ָ���Ĵ�����ַ
*  ����˵����I2Cn        ģ��ţ�I2C0��I2C1��
*            SlaveID     7λ�ӻ���ַ
*            Addr        �ӻ��ļĴ�����ַ
*            Data        ����
*           leng         ���ݳ���
*  �������أ���
*  �޸�ʱ�䣺2012-1-20
*  ��    ע��
*************************************************************************/
void I2C_WriteDataN(I2Cn i2cn, uint8 SlaveID, uint8 Addr, uint8 *Data,uint16 leng )
{
  uint16 dataleng ;  
  /* send data to slave */
    I2C_StartTransmission(i2cn, SlaveID, MWSR);    //��������
    i2c_Wait(i2cn);

    i2c_write_byte(i2cn, Addr);                    //д��ַ
    i2c_Wait(i2cn);
    for(dataleng = 0 ; dataleng < leng ;leng ++){
       i2c_write_byte(i2cn, *Data);                    //д����
        Data ++ ;
        i2c_Wait(i2cn);
      }

    i2c_Stop(i2cn);

    I2C_delay();                                        //��ʱ̫�̵Ļ�������д����
}
