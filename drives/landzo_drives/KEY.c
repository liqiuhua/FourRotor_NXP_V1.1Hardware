#include "KEY.h"

void KEY_Init(void)
{
    gpio_Interrupt_init(PTB2, GPI_UP,IRQ_FALLING);   //K1
    gpio_Interrupt_init(PTB3, GPI_UP,IRQ_FALLING);   //K2
	  gpio_Interrupt_init(PTB9, GPI_UP,IRQ_FALLING);   //K3
	  gpio_Interrupt_init(PTB10, GPI_UP,IRQ_FALLING);   //K4
	  gpio_Interrupt_init(PTB11, GPI_UP,IRQ_FALLING);   //K5
}

/**************************实现函数********************************************
*函数原型:    KEY_Scan(uint8_t mode)
*功　　能:    按键扫描
输入参数：    按键模式  1:连按  0:不支持连按     
输出参数：    
*******************************************************************************/
uint8 KEY;
uint8_t KEY_Scan(uint8_t mode)
{	 
    
 	return 0;// 无按键按下
}
