/***********************************************************
 * �ļ���       ��K6x_flash.h
 * ˵��         ��flashд�����
 * ����         ��landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://www.landzo.cn
 *��汾        ��V1.0
 *ʱ��          ��15.5.26
************************************************************/

#ifndef _K6X_FLASH_H
#define _K6X_FLASH_H

//k60N512����512K�ĳ���Flash
//512K�ĳ���Flash��Ϊ256��������ÿ������2K��С
//    sector��2K��Ϊ������С��λ
//    ���֣�32b��Ϊд����С��λ
#include "common.h"

__RAMFUN void hw_flash_init(void) ;
__RAMFUN uint8 flash_erase_sector(uint16 sectorNo) ;
__RAMFUN uint8 flash_write(uint16 sectNo,uint16 offset,uint16 cnt,uint8 buf[]) ;




#endif //_HW_FLASH_H__ 
