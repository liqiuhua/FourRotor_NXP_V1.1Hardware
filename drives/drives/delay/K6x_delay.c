/***********************************************************
 * 文件名       ：adc.c
 * 说明         ：adc初始化
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 * 库版本       ：V1.0
 *时间          ：15.5.26
************************************************************/
#include "K6x_delay.h"




/**********************************************************
*                    蓝宙电子工作室
*
* 函数名称：delay_ms
* 功能说明：延时函数（不准确），内核频率为100M时较为准确
* 参数说明：无
* 函数返回：无
* 备注    ：
***********************************************************/
void delay_ms(uint32 time)
{
  uint32 i,j;
  for(i=0;i<time;i++)
  {
    for(j=BUSCLKkh;j>0;j--)
    {
      __ASM volatile ("nop");  
    }
  }
}
