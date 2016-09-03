/***********************************************************
 * 文件名       ：K6x_gpio.c
 * 说明         ：IO端口应用函数
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 *库版本        ：V1.0
 *时间          ：15.5.26
************************************************************/
/*
 * 包含头文件
 */
#include "common.h"
#include "K6x_gpio.h"

volatile GPIO_Type * GPIOX[5] = {PTA_BASE_PTR, PTB_BASE_PTR, PTC_BASE_PTR, PTD_BASE_PTR, PTE_BASE_PTR}; 
volatile PORT_Type * PTX[5] = {PORTA_BASE_PTR, PORTB_BASE_PTR, PORTC_BASE_PTR, PORTD_BASE_PTR, PORTE_BASE_PTR};

/*************************************************************************
*                             蓝宙电子科技有限公司
*
*  函数名称：gpio_enable_port
*  功能说明：加载所有GPIO端口的时钟。 供高层驱动调用。
*  参数说明：
*  函数返回：无
*  修改时间：2015-5-15   已测试
*  备    注：苏州大学的例程
*************************************************************************/
void gpio_enable_port (void)
{
 	  SIM_SCGC5 |=   SIM_SCGC5_PORTA_MASK \
	               | SIM_SCGC5_PORTB_MASK \
	               | SIM_SCGC5_PORTC_MASK \
	               | SIM_SCGC5_PORTD_MASK \
	               | SIM_SCGC5_PORTE_MASK;
}
/*************************************************************************
*                             蓝宙电子科技有限公司
*
*  函数名称：gpio_Interrupt_init
*  功能说明：初始化GPIO中断源
*  参数说明：PTxn        端口号
*            GPIO_CFG    端口状态
*            mode        中断状态 
*  函数返回：无
*  修改时间：2015-5-15   已测试
*  备    注：
*************************************************************************/

void gpio_Interrupt_init(PTxn ptxn, GPIO_CFG cfg, uint32 mode) 
{
  uint8 altcfg ;
	vuint32 PCRcfg ;
  if((cfg == GPI)||(cfg == GPI_DOWN)||(cfg == GPI_UP)){
     altcfg = ptxn>> 5 ; 
		 PCRcfg = ALT1 + mode+ cfg;
     port_init(ptxn,PCRcfg);
     GPIO_PDDR_REG(GPIOX[altcfg]) &= ~(1 << (ptxn&0x1f));  //设置端口方向为输入 
     NVIC_EnableIRQ((IRQn_Type)(altcfg+PORTA_IRQn));
  }
}

/*************************************************************************
*                             蓝宙电子科技有限公司
*
*  函数名称：gpio_init
*  功能说明：初始化gpio
*  参数说明：PTxn        端口号
*            GPIO_CFG    端口状态
*            uint8       引脚输出状态 1 ，0
*  函数返回：无
*  修改时间：2015-5-15   已测试
*  备    注：
*************************************************************************/
void gpio_init (PTxn ptxn, GPIO_CFG cfg, uint8 sata)
{ 
  vuint32 PCRcfg;
//端口方向控制输入还是输出
    if( (cfg  == GPI) || (cfg == GPI_UP) || (cfg == GPI_DOWN) )
    {
        PCRcfg =cfg+ALT1 ;
       // le = cfg ;
        port_alt(ptxn,PCRcfg);
        GPIO_PDDR_REG(GPIOX[ptxn>> 5]) &= ~(1 << (ptxn&0x1f));  //设置端口方向为输入 
    }
    else
    {  port_alt(ptxn,ALT1) ;  
      //设置端口方向为输出
        GPIO_PDDR_REG(GPIOX[ptxn>> 5]) |= (1 << (ptxn&0x1f));        // GPIO PDDR 管脚号 置1，即对应管脚配置为端口方向输出
        //端口输出数据/        
        if(sata == 0){     
            GPIO_PDOR_REG(GPIOX[ptxn>> 5]) &= ~(1 << (ptxn&0x1f));   // GPIO PDOR 管脚号 清0，即对应管脚配置为端口输出低电平
         }else{
            GPIO_PDOR_REG(GPIOX[ptxn>> 5])  |= (1 << (ptxn&0x1f));   // GPIO PDOR 管脚号 置1，即对应管脚配置为端口输出高电平
        }
    }
}



/*************************************************************************
*                             蓝宙电子科技有限公司
*
*  函数名称：gpio_set
*  功能说明：设置引脚状态
*  参数说明：PTxn        端口号
*            uint8       引脚输出状态 1 ，0
*  函数返回：无
*  修改时间：2015-5-15   已测试
*  备    注：
*************************************************************************/
void gpio_set (PTxn ptxn,uint8 sata){
  if(sata) GPIO_PDOR_REG(GPIOX[ptxn>> 5])  |= (1 << (ptxn&0x1f));   // GPIO PDOR 管脚号 置1，即对应管脚配置为端口输出高电平
   else    GPIO_PDOR_REG(GPIOX[ptxn>> 5]) &= ~(1 << (ptxn&0x1f));   // GPIO PDOR 管脚号 清0，即对应管脚配置为端口输出低电平 
}
/*************************************************************************
*                             蓝宙电子科技有限公司
*
*  函数名称：gpio_turn
*  功能说明：引脚反转
*  参数说明：PTxn        端口号
*  函数返回：无
*  修改时间：2015-5-15   已测试
*  备    注：
*************************************************************************/
void gpio_turn(PTxn ptxn){
  GPIO_PTOR_REG(GPIOX[ptxn>> 5]) |= (1 << (ptxn&0x1f));   // GPIO PTOR 管脚号  
}

/*************************************************************************
*                             蓝宙电子科技有限公司
*
*  函数名称：gpio_get
*  功能说明：引脚反转
*  参数说明：PTxn        端口号
*  函数返回：无
*  修改时间：2015-5-15   已测试
*  备    注：
*************************************************************************/
uint8 gpio_get (PTxn ptxn){
  return ((GPIO_PDIR_REG(GPIOX[ptxn>> 5])>>(ptxn&0x1f)) &0x01);   // GPIO PDIR 管脚号  
}
               
