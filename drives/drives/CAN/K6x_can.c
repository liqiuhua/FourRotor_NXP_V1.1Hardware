/***********************************************************
 * �ļ���       ��can.c
 * ˵��         ��K60 CAN�ײ�����������
 * ����         ��landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://www.landzo.cn
 *��汾        ��V1.0
 *ʱ��          ��15.5.26
************************************************************/
#include "k6x_can.h"

//�ڲ�ʹ�ú�������
uint8 SetCANBand(uint8 CANChannel,uint32 baudrateKHz);


/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�CANInit
*  ����˵������ѯ�Ƿ���ܵ�һ���ֽ�
*  ����˵����CANChannel��ģ���
//		   baudrateKHz: ������
//         selfLoop: ģʽѡ��(1=�ػ�ģʽ��0=����ģʽ)
//         idMask: ID����(1=ID���ˣ�0=ID������)
*  �������أ�0���ɹ���1��ʧ��
*  �޸�ʱ�䣺2012-1-20
*  ��    ע���ο����ݴ�ѧ������
*************************************************************************/
uint8 CANInit(uint8 CANChannel,uint32 baudrateKHz,uint8 selfLoop,uint8 idMask)
{
    int8 i;
    CAN_MemMapPtr CANBaseAdd;
  
    //ʹ��FlexCAN�ⲿʱ��
    OSC_CR |= OSC_CR_ERCLKEN_MASK | OSC_CR_EREFSTEN_MASK;
    
    //ͨ��ģ���ѡ��ģ�����ַ
    if(CANChannel == 0)
        CANBaseAdd = CAN0_BASE_PTR;
#ifndef MK64F12 
    else if(CANChannel == 1)
        CANBaseAdd = CAN1_BASE_PTR;
#endif   
    //ʹ��CANģ��ʱ��
    if(CANBaseAdd == CAN0_BASE_PTR)
        SIM_SCGC6 |=  SIM_SCGC6_FLEXCAN0_MASK;//ʹ��CAN0��ʱ��ģ��
#ifndef MK64F12
    else
        SIM_SCGC3 |= SIM_SCGC3_FLEXCAN1_MASK;//ʹ��CAN1��ʱ��ģ��
#endif    
   

    //����CAN_RX/TX�������Ź���
    if(CANChannel == 0)
    {
		PORTA_PCR12 = PORT_PCR_MUX(2) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; //����
		PORTA_PCR13 = PORT_PCR_MUX(2) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; //����
    }
    else
    {
    	PORTE_PCR24 = PORT_PCR_MUX(2) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; //Tx����
    	PORTE_PCR25 = PORT_PCR_MUX(2) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; //Rx����
    } 
    
    //ѡ��ʱ��Դ������ʱ��48MHz���ڲ�ʱ�ӣ�12MHz
    CAN_CTRL1_REG(CANBaseAdd) |= CAN_CTRL1_CLKSRC_MASK;//ѡ���ڲ�ʱ��    
    
    CAN_MCR_REG(CANBaseAdd) |= CAN_MCR_FRZ_MASK;  //ʹ�ܶ���ģʽ   
    CAN_MCR_REG(CANBaseAdd) &= ~CAN_MCR_MDIS_MASK;//ʹ��CANģ��
    //ȷ�����˳�����ģʽ
    while((CAN_MCR_LPMACK_MASK & CAN_MCR_REG(CANBaseAdd)));

    //�����λ
    CAN_MCR_REG(CANBaseAdd) ^= CAN_MCR_SOFTRST_MASK; 
    //�ȴ���λ���
    while(CAN_MCR_SOFTRST_MASK & CAN_MCR_REG(CANBaseAdd));   
    //�ȴ����붳��ģʽ 
    while(!(CAN_MCR_FRZACK_MASK & CAN_MCR_REG(CANBaseAdd)));
    
    //��16�����仺����������0
    for(i=0;i<16;i++)
    {
          CANBaseAdd->MB[i].CS = 0x00000000;
          CANBaseAdd->MB[i].ID = 0x00000000;
          CANBaseAdd->MB[i].WORD0 = 0x00000000;
          CANBaseAdd->MB[i].WORD1 = 0x00000000;
    }
    
    //�����������IDE�Ƚϣ�RTR���Ƚ�
    CAN_CTRL2_REG(CANBaseAdd) &= ~CAN_CTRL2_EACEN_MASK;
    //Զ������֡����
    CAN_CTRL2_REG(CANBaseAdd) &= ~CAN_CTRL2_RRS_MASK;
    //�������ȴӽ���FIFO����ƥ��Ȼ������������ƥ��
    CAN_CTRL2_REG(CANBaseAdd) &= ~CAN_CTRL2_MRP_MASK;
 
    //ʹ��һ��32λ������
    CAN_MCR_REG(CANBaseAdd) |= (CAN_MCR_REG(CANBaseAdd) & ~CAN_MCR_IDAM_MASK) | CAN_MCR_IDAM(0);
    //���ò�����
    if(SetCANBand(CANChannel,baudrateKHz) == 1)//�����ô���
        return 1;
    
    //ģʽѡ�񣺻ػ�ģʽ������ģʽ
    if(1==selfLoop)
        CAN_CTRL1_REG(CANBaseAdd) |= CAN_CTRL1_LPB_MASK;//ʹ�ûػ�ģʽ

    //��ʼ������Ĵ���
    if(1==idMask)//����ID
    {
    	CAN_RXMGMASK_REG(CANBaseAdd) = 0x1FFFFFFF;
		CAN_RX14MASK_REG(CANBaseAdd) = 0x1FFFFFFF;
		CAN_RX15MASK_REG(CANBaseAdd) = 0x1FFFFFFF;
    }
    else//������ID
    {
    	CAN_RXMGMASK_REG(CANBaseAdd) = 0x0;
		CAN_RX14MASK_REG(CANBaseAdd) = 0x0;
		CAN_RX15MASK_REG(CANBaseAdd) = 0x0;
    }

    //����������빦��ʹ�ܣ�Ϊÿ�����г�ʼ������������Ĵ���
    if(CAN_MCR_REG(CANBaseAdd) & CAN_MCR_IRMQ_MASK)
    {
        for(i = 0; i < NUMBER_OF_MB ; i++)
        {        
            CANBaseAdd->RXIMR[i] = 0x1FFFFFFFL;
        }
    }
    
    //ֻ���ڶ���ģʽ�²������ã��������Ƴ�����ģʽ
	CAN_MCR_REG(CANBaseAdd) &= ~(CAN_MCR_HALT_MASK);
	//�ȴ�ֱ���˳�����ģʽ
	while( CAN_MCR_REG(CANBaseAdd) & CAN_MCR_FRZACK_MASK);    
	//�ȵ����ڶ���ģʽ������ģʽ����ֹͣģʽ
	while((CAN_MCR_REG(CANBaseAdd) & CAN_MCR_NOTRDY_MASK));
    
    return 0;
}


/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�CANSendData
*  ����˵������������
*  ����˵����CANChannel��ģ���         
//          iMB����������
//	    id: ID��
//          length�����ݳ���
//	    Data[8]:�������ݻ�����
*  �������أ�0���ɹ���1��ʧ��
*  �޸�ʱ�䣺2015-5-15   �Ѳ���
*  ��    ע�����ݴ�ѧ������
*************************************************************************/
uint8 CANSendData(uint8 CANChannel,uint16 iMB, uint32 id,uint8 length,uint8 Data[])
{
    int16  i,wno,bno;
    uint32 word[2] = {0};
    CAN_MemMapPtr CANBaseAdd;
    
    if(CANChannel == 0)
        CANBaseAdd = CAN0_BASE_PTR;
 #ifndef MK64F12   
    else if(CANChannel == 1)
        CANBaseAdd = CAN1_BASE_PTR;
#endif  
    //�����������ݳ������ô���
    if(iMB >= NUMBER_OF_MB || length >8)
        return 1; //������Χ
    
    //ת��8���ֽ�ת����32λ��word�洢
    wno = (length-1)>>2;//�Ƿ񳬹�4�ֽ�
    bno = (length-1)%4; //
    if(wno > 0)         //���ȴ���4(���������ݳ���4�ֽ�)
    {
    	word[0] = (
    			     (Data[0]<<24) 
    			   | (Data[1]<<16) 
    			   | (Data[2]<< 8) 
    			   |  Data[3]
    			 );
    }
    for(i=0;i<=bno;i++)
       word[wno] |= Data[(wno<<2)+i] << (24-(i<<3)); 

    ///////////////////////////////////////////////////////  
    // ID ��ʽ
    // B31 30 29 28 27 26 ... 11 10 9 8 7 6 5 4 3 2 1 0
    // |	|	 |									  |
    // |    |    |------------------------------------|
    // |	|					|--> 29 λ ID
    // |	|
    // |    |--> RTR  1: Զ��֡, 0: ����֡
    // |
    // |-------> IDE 1 : ��չID, 0: ��׼ID
    ///////////////////////////////////////////////////////
    
    //ͨ��id�ж�֡���͡�����չ֡
    wno = (id &  CAN_MSG_IDE_MASK)>>CAN_MSG_IDE_BIT_NO;  //IDE
    //ͨ��id�ж�֡���͡���Զ��֡
    bno = (id &  CAN_MSG_TYPE_MASK)>>CAN_MSG_TYPE_BIT_NO;//RTR
    
    //���IDλ��
    i =  wno? 0: FLEXCAN_MB_ID_STD_BIT_NO;
    
    //�����Ĳ���Ϊ���͹���
    CANBaseAdd->MB[iMB].CS = FLEXCAN_MB_CS_CODE(FLEXCAN_MB_CODE_TX_INACTIVE)//������д�������
							| (wno<<FLEXCAN_MB_CS_IDE_BIT_NO)//������дIDEλ
							| (bno<<FLEXCAN_MB_CS_RTR_BIT_NO)//������дRTRλ
                            | FLEXCAN_MB_CS_LENGTH(length);  //������д���ݳ���
    
    //������дID
    CANBaseAdd->MB[iMB].ID = (1 << FLEXCAN_MB_ID_PRIO_BIT_NO) 
							| ((id & ~(CAN_MSG_IDE_MASK|CAN_MSG_TYPE_MASK))<<i);  
    
    //������д����
    CANBaseAdd->MB[iMB].WORD0 = word[0];
    CANBaseAdd->MB[iMB].WORD1 = word[1];  
    
    //�ӳ�
    for(i = 0;i < 100;i++);
    
    //ͨ���ƶ��ķ��ʹ��뿪ʼ����
    CANBaseAdd->MB[iMB].CS = (CANBaseAdd->MB[iMB].CS & ~(FLEXCAN_MB_CS_CODE_MASK)) 
							| FLEXCAN_MB_CS_CODE(FLEXCAN_MB_CODE_TX_ONCE)//д�������
							| FLEXCAN_MB_CS_LENGTH(length);//������д���ݳ��� 
    
    //��ʱ�ȴ�������ɣ����ʹ���ж�����ʱ�ȴ�����ɾ����
    i=0;
    while(!(CANBaseAdd->IFLAG1 & (1<<iMB)))
    {
    	if((i++)>0x1000)
    		return 1;
    }
    //�屨�Ļ������жϱ�־
    CANBaseAdd->IFLAG1 = (1<<iMB);
    return 0;
}


/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�CANEnableRXBuff
*  ����˵����ʹ�ܽ��ջ�����
*  ����˵����CANChannel��ģ���         
//          iMB����������
//	    id: ID��
*  �������أ�
*  �޸�ʱ�䣺2015-5-15   �Ѳ���
*  ��    ע�����ݴ�ѧ������
*************************************************************************/
void CANEnableRXBuff(uint8 CANChannel,uint16 iMB, uint32 id)
{
    uint32 id2;
    uint32 cs = FLEXCAN_MB_CS_CODE(FLEXCAN_MB_CODE_RX_EMPTY); 
    CAN_MemMapPtr CANBaseAdd;
    
    if(CANChannel == 0)
        CANBaseAdd = CAN0_BASE_PTR;
#ifndef MK64F12 
    else if(CANChannel == 1)
        CANBaseAdd = CAN1_BASE_PTR;
#endif
    //��MB����Ϊ�Ǽ���״̬
    CANBaseAdd->MB[iMB].CS = FLEXCAN_MB_CS_CODE(FLEXCAN_MB_CODE_RX_INACTIVE); 	
    
    //ȡ��29λ������ID
    id2 = id & ~(CAN_MSG_IDE_MASK | CAN_MSG_TYPE_MASK);
    if(id & CAN_MSG_IDE_MASK)//��չ֡
    {
        CANBaseAdd->MB[iMB].ID = id2;
        cs |= FLEXCAN_MB_CS_IDE;//��λIDEλ
    }
    else//��׼֡
    {
        CANBaseAdd->MB[iMB].ID = id2<<FLEXCAN_MB_ID_STD_BIT_NO;          
    }
    
    //������ջ�������codeд0100
    CANBaseAdd->MB[iMB].CS = cs;      
}

/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ���������
*  ����˵����ʹ�ܽ��ջ�����
*  ����˵����CANChannel��CANģ���
//          iMB����������
//	    id: ID��
//	    lenght�����ݳ���
//	    Data[8]: �ͽ������ݻ�����
*  �������أ�0���ɹ���1��ʧ��
*  �޸�ʱ�䣺2015-5-15   �Ѳ���
*  ��    ע�����ݴ�ѧ������
*************************************************************************/

uint8 CANRecData(uint8 CANChannel,uint16 iMB, uint32 *id,uint8 *Datalenght,uint8 Data[])
{
    int8   i,wno,bno;
    uint16 code;
    uint8  *pMBData;
    int16  length;
    int8   format;
    uint8 *pBytes = Data;
    CAN_MemMapPtr CANBaseAdd;
    
    if(CANChannel == 0)
        CANBaseAdd = CAN0_BASE_PTR;
#ifndef MK64F12 
    else if(CANChannel == 1)
        CANBaseAdd = CAN1_BASE_PTR;
#endif   
    // ����MB
    code = FLEXCAN_get_code(CANBaseAdd->MB[iMB].CS);
    
    if(code != 0x02)//������û�н��յ����ݣ����ش���
    {
        *Datalenght = 0;
        return 1;
    }
    length = FLEXCAN_get_length(CANBaseAdd->MB[iMB].CS);
    
    if(length <1)//���յ������ݳ���С��1�����ش���
    {
        *Datalenght = 0;
        return 1;
    }
   
    //�ж��Ǳ�׼֡������չ֡
    format = (CANBaseAdd->MB[iMB].CS & FLEXCAN_MB_CS_IDE)? 1:0;
    *id = (CANBaseAdd->MB[iMB].ID & FLEXCAN_MB_ID_EXT_MASK);
 
    if(!format)
    {
        *id >>= FLEXCAN_MB_ID_STD_BIT_NO; // ��ñ�׼֡��
    }
    else
    { 
        *id |= CAN_MSG_IDE_MASK; //�����չ��ID        
    }
 
    format = (CANBaseAdd->MB[iMB].CS & FLEXCAN_MB_CS_RTR)? 1:0;  
    if(format)
    {
        *id |= CAN_MSG_TYPE_MASK; //���ΪԶ��֡����       
    }
    //��ȡ��������
    wno = (length-1)>>2;
    bno = length-1;
    if(wno>0)
    {  
        (*(uint32*)pBytes) = CANBaseAdd->MB[iMB].WORD0;
        swap_4bytes(pBytes);
        bno -= 4;
        pMBData = (uint8*)&CANBaseAdd->MB[iMB].WORD1+3;
    }
    else
    {
        pMBData = (uint8*)&CANBaseAdd->MB[iMB].WORD0+3;
    }
    
    for(i=0; i <= bno; i++)
        pBytes[i+(wno<<2)] = *pMBData--;	

    *Datalenght = length;
     return 0;
}

/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�EnableCANInterrupt
*  ����˵����ʹ�ܻ��������պͷ����ж�
*  ����˵����CANChannel:CANģ���
//          iMB:��������
*  �������أ�
*  �޸�ʱ�䣺2015-5-15   �Ѳ���
*  ��    ע�����ݴ�ѧ������
*************************************************************************/

void EnableCANInterrupt(uint8 CANChannel,uint16 iMB)
{
    CAN_MemMapPtr CANBaseAdd;
    
    if(CANChannel == 0)
        CANBaseAdd = CAN0_BASE_PTR;
#ifndef MK64F12 
    else if(CANChannel == 1)
        CANBaseAdd = CAN1_BASE_PTR;
#endif    
    CAN_IMASK1_REG(CANBaseAdd) = (1<<iMB);
}
/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�DisableCANInterrupt
*  ����˵������ֹ���������պͷ����ж�
*  ����˵����CANChannel:CANģ���
//         iMB:��������
*  �������أ�
*  �޸�ʱ�䣺2015-5-15   �Ѳ���
*  ��    ע�����ݴ�ѧ������
*************************************************************************/

void DisableCANInterrupt(uint8 CANChannel,uint16 iMB)
{
    CAN_MemMapPtr CANBaseAdd;
    
    if(CANChannel == 0)
        CANBaseAdd = CAN0_BASE_PTR;
#ifndef MK64F12    
    else if(CANChannel == 1)
        CANBaseAdd = CAN1_BASE_PTR;
#endif    
    CAN_IMASK1_REG(CANBaseAdd) &= ~CAN_IMASK1_BUFLM(iMB);
}
/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�CANClearFlag
*  ����˵�����建�����жϱ�־
*  ����˵����CANChannel:CANģ���
//         iMB:��������
*  �������أ�
*  �޸�ʱ�䣺2015-5-15   �Ѳ���
*  ��    ע�����ݴ�ѧ������
*************************************************************************/

void CANClearFlag(uint8 channel,uint16 iMB)
{
    CAN_MemMapPtr CANBaseAdd;
    
    if(channel == 0)
        CANBaseAdd = CAN0_BASE_PTR;
#ifndef MK64F12     
    else if(channel == 1)
        CANBaseAdd = CAN1_BASE_PTR;
#endif
    CANBaseAdd->IFLAG1 = (1<<iMB);
}
/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�CANGetFlag
*  ����˵������û������жϱ�־
*  ����˵����CANChannel:CANģ���
//         iMB:��������
*  �������أ�
*  �޸�ʱ�䣺2015-5-15   �Ѳ���
*  ��    ע�����ݴ�ѧ������
*************************************************************************/

uint32 CANGetFlag(uint8 channel,uint16 iMB)
{
    CAN_MemMapPtr CANBaseAdd;
    
    if(channel == 0)
        CANBaseAdd = CAN0_BASE_PTR;
#ifndef MK64F12 
    else if(channel == 1)
        CANBaseAdd = CAN1_BASE_PTR;
#endif
    return (CANBaseAdd->IFLAG1 & (1<<iMB));
}

/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�SetCANBand
*  ����˵��������CAN�Ĳ�����
*  ����˵����CANChannel:CANģ���
//          baudrateKHz:������
*  �������أ�0:���óɹ� 1�����óɹ�
*  �޸�ʱ�䣺2015-5-15   �Ѳ���
*  ��    ע�����ݴ�ѧ������
*************************************************************************/

uint8 SetCANBand(uint8 CANChannel,uint32 baudrateKHz)
{    
    CAN_MemMapPtr CANBaseAdd;
    
    if(CANChannel == 0)
        CANBaseAdd = CAN0_BASE_PTR;
#ifndef MK64F12 
    else if(CANChannel == 1)
        CANBaseAdd = CAN1_BASE_PTR;
#endif    
    switch (baudrateKHz)
    {
          case (33):	// 33.33K
             if(CAN_CTRL1_REG(CANBaseAdd) & CAN_CTRL1_CLKSRC_MASK)
             { 
				 // 48M/120= 400k sclock, 12Tq
				 // PROPSEG = 3, LOM = 0x0, LBUF = 0x0, TSYNC = 0x0, SAMP = 1
				 // RJW = 3, PSEG1 = 4, PSEG2 = 4,PRESDIV = 120
				 CAN_CTRL1_REG(CANBaseAdd) = (0 | CAN_CTRL1_PROPSEG(2) 
												| CAN_CTRL1_RJW(2)
												| CAN_CTRL1_PSEG1(3) 
												| CAN_CTRL1_PSEG2(3)
												| CAN_CTRL1_PRESDIV(119));
             }
             else
             { 
				 // 12M/20= 600k sclock, 18Tq
				 // PROPSEG = 1, LOM = 0x0, LBUF = 0x0, TSYNC = 0x0, SAMP = 1
				 // RJW = 4, PSEG1 = 8, PSEG2 = 8,PRESDIV = 20
				CAN_CTRL1_REG(CANBaseAdd) = (0  | CAN_CTRL1_PROPSEG(0) 
												| CAN_CTRL1_PROPSEG(3)
												| CAN_CTRL1_PSEG1(7) 
												| CAN_CTRL1_PSEG2(7)
												| CAN_CTRL1_PRESDIV(19));
             }
             break;
          case (83):	// 83.33K
             if(CAN_CTRL1_REG(CANBaseAdd) & CAN_CTRL1_CLKSRC_MASK)
             {
				 // 48M/48= 1M sclock, 12Tq
				 // PROPSEG = 3, LOM = 0x0, LBUF = 0x0, TSYNC = 0x0, SAMP = 1
				 // RJW = 3, PSEG1 = 4, PSEG2 = 4,PRESDIV = 48
				 CAN_CTRL1_REG(CANBaseAdd) = (0 | CAN_CTRL1_PROPSEG(2) 
												| CAN_CTRL1_RJW(2)
												| CAN_CTRL1_PSEG1(3)
												| CAN_CTRL1_PSEG2(3)
												| CAN_CTRL1_PRESDIV(47));
             }
             else
             { 
				 // 12M/12= 1M sclock, 12Tq
				 // PROPSEG = 3, LOM = 0x0, LBUF = 0x0, TSYNC = 0x0, SAMP = 1
				 // RJW = 3, PSEG1 = 4, PSEG2 = 4,PRESDIV = 12
				 CAN_CTRL1_REG(CANBaseAdd) = (0 | CAN_CTRL1_PROPSEG(2) 
												| CAN_CTRL1_RJW(2)
												| CAN_CTRL1_PSEG1(3) 
												| CAN_CTRL1_PSEG2(3)
												| CAN_CTRL1_PRESDIV(11));
             }
             break;
          case (50):
             if(CAN_CTRL1_REG(CANBaseAdd) & CAN_CTRL1_CLKSRC_MASK)
             {                
				 // 48M/80= 0.6M sclock, 12Tq
				 // PROPSEG = 3, LOM = 0x0, LBUF = 0x0, TSYNC = 0x0, SAMP = 1
				 // RJW = 3, PSEG1 = 4, PSEG2 = 4, PRESDIV = 40
				 CAN_CTRL1_REG(CANBaseAdd) = (0 | CAN_CTRL1_PROPSEG(2) 
												| CAN_CTRL1_RJW(1)
												| CAN_CTRL1_PSEG1(3) 
												| CAN_CTRL1_PSEG2(3)
												| CAN_CTRL1_PRESDIV(79));
             }
             else
             {
				 // 12M/20= 0.6M sclock, 12Tq
				 // PROPSEG = 3, LOM = 0x0, LBUF = 0x0, TSYNC = 0x0, SAMP = 1
				 // RJW = 3, PSEG1 = 4, PSEG2 = 4, PRESDIV = 20                 
				 CAN_CTRL1_REG(CANBaseAdd) = (0 | CAN_CTRL1_PROPSEG(2)
												| CAN_CTRL1_RJW(2)
												| CAN_CTRL1_PSEG1(3) 
												| CAN_CTRL1_PSEG2(3)
												| CAN_CTRL1_PRESDIV(19));                   
             }
             break;
          case (100):
             if(CAN_CTRL1_REG(CANBaseAdd) & CAN_CTRL1_CLKSRC_MASK)
             { 
				 // 48M/40= 1.2M sclock, 12Tq
				 // PROPSEG = 3, LOM = 0x0, LBUF = 0x0, TSYNC = 0x0, SAMP = 1
				 // RJW = 3, PSEG1 = 4, PSEG2 = 4, PRESDIV = 40
				 CAN_CTRL1_REG(CANBaseAdd) = (0 | CAN_CTRL1_PROPSEG(2)
												| CAN_CTRL1_RJW(2)
												| CAN_CTRL1_PSEG1(3) 
												| CAN_CTRL1_PSEG2(3)
												| CAN_CTRL1_PRESDIV(39));
             }
             else
             {
				 // 12M/10= 1.2M sclock, 12Tq
				 // PROPSEG = 3, LOM = 0x0, LBUF = 0x0, TSYNC = 0x0, SAMP = 1
				 // RJW = 3, PSEG1 = 4, PSEG2 = 4, PRESDIV = 10
				 CAN_CTRL1_REG(CANBaseAdd) = (0 | CAN_CTRL1_PROPSEG(2) 
												| CAN_CTRL1_RJW(2)
												| CAN_CTRL1_PSEG1(3) 
												| CAN_CTRL1_PSEG2(3)
												| CAN_CTRL1_PRESDIV(9));                   
             }
             break;
          case (125):
             if(CAN_CTRL1_REG(CANBaseAdd) & CAN_CTRL1_CLKSRC_MASK)
             {                 
				 // 48M/32= 1.5M sclock, 12Tq
				 // PROPSEG = 3, LOM = 0x0, LBUF = 0x0, TSYNC = 0x0, SAMP = 1
				 // RJW = 3, PSEG1 = 4, PSEG2 = 4, PRESDIV = 32
				 CAN_CTRL1_REG(CANBaseAdd) = (0 | CAN_CTRL1_PROPSEG(2) 
											    | CAN_CTRL1_RJW(2)
											    | CAN_CTRL1_PSEG1(3) 
											    | CAN_CTRL1_PSEG2(3)
											    | CAN_CTRL1_PRESDIV(31));
             }
             else
             {
				 // 12M/8= 1.5M sclock, 12Tq
				 // PROPSEG = 3, LOM = 0x0, LBUF = 0x0, TSYNC = 0x0, SAMP = 1
				 // RJW = 3, PSEG1 = 4, PSEG2 = 4, PRESDIV = 8
				 CAN_CTRL1_REG(CANBaseAdd) = (0 | CAN_CTRL1_PROPSEG(2) 
												| CAN_CTRL1_RJW(2)
												| CAN_CTRL1_PSEG1(3) 
												| CAN_CTRL1_PSEG2(3)
												| CAN_CTRL1_PRESDIV(7));                  
             }
             break;
          case (250):
             if(CAN_CTRL1_REG(CANBaseAdd) & CAN_CTRL1_CLKSRC_MASK)
             {                
				 // 48M/16= 3M sclock, 12Tq
				 // PROPSEG = 3, LOM = 0x0, LBUF = 0x0, TSYNC = 0x0, SAMP = 1
				 // RJW = 2, PSEG1 = 4, PSEG2 = 4, PRESDIV = 16
				 CAN_CTRL1_REG(CANBaseAdd) = (0 | CAN_CTRL1_PROPSEG(2)
												| CAN_CTRL1_RJW(1)
												| CAN_CTRL1_PSEG1(3) 
												| CAN_CTRL1_PSEG2(3)
												| CAN_CTRL1_PRESDIV(15));
             }
             else
             {
				 // 12M/4= 3M sclock, 12Tq
				 // PROPSEG = 3, LOM = 0x0, LBUF = 0x0, TSYNC = 0x0, SAMP = 1
				 // RJW = 2, PSEG1 = 4, PSEG2 = 4, PRESDIV = 4
				 CAN_CTRL1_REG(CANBaseAdd) = (0 | CAN_CTRL1_PROPSEG(2) 
												| CAN_CTRL1_RJW(1)
												| CAN_CTRL1_PSEG1(3)
												| CAN_CTRL1_PSEG2(3)
												| CAN_CTRL1_PRESDIV(3));                   
             }
             break;
          case (500):
             if(CAN_CTRL1_REG(CANBaseAdd) & CAN_CTRL1_CLKSRC_MASK)
             {                
				 // 48M/8=6M sclock, 12Tq
				 // PROPSEG = 3, LOM = 0x0, LBUF = 0x0, TSYNC = 0x0, SAMP = 1
				 // RJW = 2, PSEG1 = 4, PSEG2 = 4, PRESDIV = 6
				 CAN_CTRL1_REG(CANBaseAdd) = (0 | CAN_CTRL1_PROPSEG(2) 
												| CAN_CTRL1_RJW(1)
												| CAN_CTRL1_PSEG1(3) 
												| CAN_CTRL1_PSEG2(3)
												| CAN_CTRL1_PRESDIV(7));
             }
             else
             {
				 // 12M/2=6M sclock, 12Tq
				 // PROPSEG = 3, LOM = 0x0, LBUF = 0x0, TSYNC = 0x0, SAMP = 1
				 // RJW = 2, PSEG1 = 4, PSEG2 = 4, PRESDIV = 2
				 CAN_CTRL1_REG(CANBaseAdd) = (0 | CAN_CTRL1_PROPSEG(2) 
												| CAN_CTRL1_RJW(1)
												| CAN_CTRL1_PSEG1(3) 
												| CAN_CTRL1_PSEG2(3)
												| CAN_CTRL1_PRESDIV(1));                   
             }
             break;
          case (1000): 
             if(CAN_CTRL1_REG(CANBaseAdd) & CAN_CTRL1_CLKSRC_MASK)
             {                  
				 // 48M/6=8M sclock
				 // PROPSEG = 4, LOM = 0x0, LBUF = 0x0, TSYNC = 0x0, SAMP = 1
				 // RJW = 1, PSEG1 = 1, PSEG2 = 2, PRESCALER = 6
				 CAN_CTRL1_REG(CANBaseAdd) = (0 | CAN_CTRL1_PROPSEG(3) 
												| CAN_CTRL1_RJW(0)
												| CAN_CTRL1_PSEG1(0)
												| CAN_CTRL1_PSEG2(1)
												| CAN_CTRL1_PRESDIV(5));
             }
             else
             {  
				 // 12M/1=12M sclock,12Tq
				 // PROPSEG = 3, LOM = 0x0, LBUF = 0x0, TSYNC = 0x0, SAMP = 1
				 // RJW = 4, PSEG1 = 4, PSEG2 = 4, PRESCALER = 1
				 CAN_CTRL1_REG(CANBaseAdd) = (0 | CAN_CTRL1_PROPSEG(2) 
												| CAN_CTRL1_RJW(3)
												| CAN_CTRL1_PSEG1(3) 
												| CAN_CTRL1_PSEG2(3)
												| CAN_CTRL1_PRESDIV(0));
             }
             break;
          default: 
             return 1;
       }
    return 0;
}
