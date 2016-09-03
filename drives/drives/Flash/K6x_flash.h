/***********************************************************
 * 文件名       ：K6x_flash.h
 * 说明         ：flash写入擦除
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 *库版本        ：V1.0
 *时间          ：15.5.26
************************************************************/

#ifndef _K6X_FLASH_H
#define _K6X_FLASH_H

//k60N512包含512K的程序Flash
//512K的程序Flash分为256个扇区，每个扇区2K大小
//    sector（2K）为擦除最小单位
//    长字（32b）为写的最小单位
#include "common.h"

__RAMFUN void hw_flash_init(void) ;
__RAMFUN uint8 flash_erase_sector(uint16 sectorNo) ;
__RAMFUN uint8 flash_write(uint16 sectNo,uint16 offset,uint16 cnt,uint8 buf[]) ;




#endif //_HW_FLASH_H__ 
