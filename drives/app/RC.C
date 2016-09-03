#include "RC.H"

//void RC_Init(void)
//{
//    FTM_Input_init(FTM0,CH0,Rising_or_Falling);           //遥控器第8通道  暂用超声波
//    FTM_Input_init(FTM0,CH1,Rising_or_Falling);           //遥控器第7通道  暂用超声波
//    FTM_Input_init(FTM0,CH2,Rising_or_Falling);
//    FTM_Input_init(FTM0,CH3,Rising_or_Falling);
//    FTM_Input_init(FTM0,CH4,Rising_or_Falling);
//    FTM_Input_init(FTM0,CH5,Rising_or_Falling);
//    FTM_Input_init(FTM0,CH6,Rising_or_Falling);           //遥控器第2通道  
//    FTM_Input_init(FTM0,CH7,Rising_or_Falling);           //遥控器第1通道 
//}
/**************************实现函数********************************************
*函数原型:    rcInt_IRQHandler(u8 flag)
*功　　能:    输入捕获 结算
输入参数：    flag
输出参数：    没有
*******************************************************************************/
struct _rc_data rc_data;
char ARMED=0;					//遥控解锁标志，0是上锁，1是解锁

