#ifndef __KEY_H
#define __KEY_H

#include "include.h"
#include "common.h"



//KEY状态宏定义
typedef enum KEY_status
{
  KEY_ON  =   0,         //按键按下(对应低电平)
  KEY_OFF =   1          //按键松开(对应高电平)
}KEY_status;




extern uint8_t KEY_State;


void KEY_Init(void);
uint8_t KEY_Scan(uint8_t mode);

#endif
