/***********************************************************
 * �ļ���       ��PTI.c
 * ˵��         ��PTI��ʼ��
 * ����         ��landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://www.landzo.cn
 * ��汾       ��V1.0
 *ʱ��          ��15.5.26
************************************************************/

#ifndef _K6X_PIT_H_
#define _K6X_PIT_H_



typedef enum PITn
{
    PIT0,
    PIT1,
    PIT2,
    PIT3
} PITn;


void    pit_init(PITn pitn, uint32_t cnt);                                        //��ʼ��PITn�������ö�ʱʱ��(��λΪbusʱ������)
#define     pit_init_ms(PITn,ms)          pit_init(PITn,ms * BUSCLKkh);         //��ʼ��PITn�������ö�ʱʱ��(��λΪ ms)
#define     PIT_Flag_Clear(PITn)          PIT_TFLG(PITn)|=PIT_TFLG_TIF_MASK      //���жϱ�־


void        pit_dma_init(PITn pitn, uint32_t cnt);
#define     pit_dma_init_ms(PITn,ms)     pit_dma_init(PITn,ms * BUSCLKkh)

#endif  //_PIT_H_
