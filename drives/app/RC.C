#include "RC.H"

//void RC_Init(void)
//{
//    FTM_Input_init(FTM0,CH0,Rising_or_Falling);           //ң������8ͨ��  ���ó�����
//    FTM_Input_init(FTM0,CH1,Rising_or_Falling);           //ң������7ͨ��  ���ó�����
//    FTM_Input_init(FTM0,CH2,Rising_or_Falling);
//    FTM_Input_init(FTM0,CH3,Rising_or_Falling);
//    FTM_Input_init(FTM0,CH4,Rising_or_Falling);
//    FTM_Input_init(FTM0,CH5,Rising_or_Falling);
//    FTM_Input_init(FTM0,CH6,Rising_or_Falling);           //ң������2ͨ��  
//    FTM_Input_init(FTM0,CH7,Rising_or_Falling);           //ң������1ͨ�� 
//}
/**************************ʵ�ֺ���********************************************
*����ԭ��:    rcInt_IRQHandler(u8 flag)
*��������:    ���벶�� ����
���������    flag
���������    û��
*******************************************************************************/
struct _rc_data rc_data;
char ARMED=0;					//ң�ؽ�����־��0��������1�ǽ���
