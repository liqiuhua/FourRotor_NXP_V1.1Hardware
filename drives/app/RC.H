#ifndef __RC_H
#define __RC_H

#include "include.h"
#include "common.h"

#define RC_mid  0x2C90
struct _rc_data{
        uint16 pitch;
        uint16 roll;
        uint16 throttle;
        uint16 yaw;
        uint16 ch4;
        uint16 ch5;
        uint16 ch6;
        uint16 ch7;
};
extern char ARMED;
extern struct _rc_data rc_data;

void RC_Init(void);
void RC_Handle(uint8 flag);

#endif
