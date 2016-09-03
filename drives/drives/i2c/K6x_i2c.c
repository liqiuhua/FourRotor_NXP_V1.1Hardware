/***********************************************************
 * 文件名       ：i2c.c
 * 说明         ：I2C驱动函数，修改自官方例程
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 * 库版本       ：V1.0
 *时间          ：15.5.26
************************************************************/


#include "common.h"
#include "K6X_i2c.h"


unsigned char MasterTransmission;
unsigned char SlaveID;

volatile  I2C_Type *I2Cx[2] = {I2C0_BASE_PTR, I2C1_BASE_PTR}; //定义两个指针数组保存 I2Cx 的地址


/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：I2C_init
*  功能说明：I2C初始化，设置波特率和启动I2C
*  参数说明：I2Cn        模块号（I2C0、I2C1）
*  函数返回：无
*  修改时间：2012-1-20
*  备    注：
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
    根据计算查找最近
    因子
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
        /* 开启时钟 */
        SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;         //开启 I2C0时钟
			

        /* 配置 I2C0功能的 GPIO 接口 */
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
        /* 开启时钟 */
        SIM_SCGC4 |= SIM_SCGC4_I2C1_MASK;         //开启 I2C1时钟

        /* 配置 I2C1功能的 GPIO 接口 */
        if(I2C1_SCL == PTE1)
           port_alt(I2C1_SCL,ALT6) ;
        else if(I2C1_SCL == PTC10)
            port_alt(I2C1_SCL,ALT2) ;

        if(I2C1_SDA == PTE0)
            port_alt(I2C1_SDA,ALT6) ;
        else if (I2C1_SDA == PTC11)
            port_alt(I2C1_SDA,ALT2) ;

    }

    /* 设置频率 */
    I2C_F_REG(I2Cx[i2cn])  = I2C_F_MULT(mult) | I2C_F_ICR(ICR_clock) ;  // I2C Frequency Divider register (I2Cx_F)  I2C分频寄存器   I2C最大波特率为 400k
    // I2C baud rate = bus speed (Hz)/(mul × SCL divider)  即这里 50MHz/(1 ×128)=390.625kHz
    // SDA hold time = bus period (s) × mul × SDA hold value
    // SCL start hold time = bus period (s) × mul × SCL start hold value
    // SCL stop hold time = bus period (s) × mul × SCL stop hold value

    /* 使能 IIC1 */
    I2C_C1_REG(I2Cx[i2cn]) = I2C_C1_IICEN_MASK;
}

/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：I2C_StartTransmission
*  功能说明：启动 I2C 传输
*  参数说明：I2Cn        模块号（I2C0、I2C1）
*            SlaveID     7位从机地址
*            MSmode      读写模式（ MWSR 或 MRSW ）
*  函数返回：无
*  修改时间：2012-1-20
*  备    注：
*************************************************************************/
void I2C_StartTransmission (I2Cn i2cn, uint8 SlaveID, MSmode Mode)
{

    SlaveID = ( SlaveID << 1 ) | Mode ;            //确定写地址和读地址

    /* send start signal */
    i2c_Start(i2cn);

    /* send ID with W/R bit */
    i2c_write_byte(i2cn, SlaveID);
}

/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：Pause
*  功能说明：延时，I2C读写后，需要延时一下
*  参数说明：无
*  函数返回：无
*  修改时间：2012-1-20
*  备    注：在官方基础上增大延时数
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
*                             蓝宙电子工作室
*
*  函数名称：I2C_ReadAddr
*  功能说明：读取I2C设备指定地址寄存器的数据
*  参数说明：I2Cn        模块号（I2C0、I2C1）
*            SlaveID     7位从机地址
*            Addr        从机的寄存器地址
*  函数返回：从机寄存器的数据
*  修改时间：2012-1-20
*  备    注：
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
    i2c_DisableAck(i2cn); //不应答

    /* Dummy read 虚假读取*/
    result = I2C_D_REG(I2Cx[i2cn]);
    i2c_Wait(i2cn);

    /* Send stop since about to read last byte */
    i2c_Stop(i2cn);

    /* Read byte */
    result = I2C_D_REG(I2Cx[i2cn]);

    return result;
}


/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：I2C_WriteData1
*  功能说明：写入一个字节数据到I2C设备指定寄存器地址
*  参数说明：I2Cn        模块号（I2C0、I2C1）
*            SlaveID     7位从机地址
*            Addr        从机的寄存器地址
*            Data        数据
*  函数返回：无
*  修改时间：2012-1-20
*  备    注：
*************************************************************************/
void I2C_WriteAddr(I2Cn i2cn, uint8 SlaveID, uint8 Addr, uint8 Data)
{
    /* send data to slave */
    I2C_StartTransmission(i2cn, SlaveID, MWSR);    //启动传输
    i2c_Wait(i2cn);

    i2c_write_byte(i2cn, Addr);                    //写地址
    i2c_Wait(i2cn);

    i2c_write_byte(i2cn, Data);                    //写数据
    i2c_Wait(i2cn);

    i2c_Stop(i2cn);

    I2C_delay();                                        //延时太短的话，可能写出错
}

/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：I2C_WriteDataN
*  功能说明：写入一个字节数据到I2C设备指定寄存器地址
*  参数说明：I2Cn        模块号（I2C0、I2C1）
*            SlaveID     7位从机地址
*            Addr        从机的寄存器地址
*            Data        数据
*           leng         数据长度
*  函数返回：无
*  修改时间：2012-1-20
*  备    注：
*************************************************************************/
void I2C_WriteDataN(I2Cn i2cn, uint8 SlaveID, uint8 Addr, uint8 *Data,uint16 leng )
{
  uint16 dataleng ;  
  /* send data to slave */
    I2C_StartTransmission(i2cn, SlaveID, MWSR);    //启动传输
    i2c_Wait(i2cn);

    i2c_write_byte(i2cn, Addr);                    //写地址
    i2c_Wait(i2cn);
    for(dataleng = 0 ; dataleng < leng ;leng ++){
       i2c_write_byte(i2cn, *Data);                    //写数据
        Data ++ ;
        i2c_Wait(i2cn);
      }

    i2c_Stop(i2cn);

    I2C_delay();                                        //延时太短的话，可能写出错
}
