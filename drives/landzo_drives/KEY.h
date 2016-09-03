#ifndef __KEY_H
#define __KEY_H

#include "include.h"
#include "common.h"



//KEY״̬�궨��
typedef enum KEY_status
{
  KEY_ON  =   0,         //��������(��Ӧ�͵�ƽ)
  KEY_OFF =   1          //�����ɿ�(��Ӧ�ߵ�ƽ)
}KEY_status;




extern uint8_t KEY_State;


void KEY_Init(void);
uint8_t KEY_Scan(uint8_t mode);

#endif
