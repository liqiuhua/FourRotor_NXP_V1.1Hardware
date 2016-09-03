#include "landzo_app.h"

/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�Uart_DMA
*  ����˵��������DMA���ݷ�������װ��
*  ����˵����damch   ����ͨ��
             souradd Դ��ַ
						 daadd   Ŀ���ַ
						 trancount ���ʹ���
						 DMSour     �����˿�
*  �������أ���
*  �޸�ʱ�䣺2016-7-10   �Ѳ���
*  ��    ע��
*************************************************************************/
uint8 DMAflag = 0xff;
uint8 Uart_DMA(uint8 damch ,void *souradd,void *daadd,uint32 trancount,DMA_sources DMSour ){

	   uint8 returnda ;
	   
	   if(DMAflag&(1<<damch)){  //���ͨ��Ϊ�ղſ���װ������
			 returnda = OK  ;
			 DMAflag &= ~(1<<damch);
			 DMA_SADDR(damch) =  (uint32_t)souradd;                      // ����  Դ��ַ
			 DMA_DADDR(damch) =  (uint32_t)daadd;                         // ����Ŀ�ĵ�ַ
			 DMA_CITER_ELINKNO(damch)  = DMA_CITER_ELINKNO_CITER(trancount); //��ǰ��ѭ������
			 DMA_BITER_ELINKNO(damch)  = DMA_BITER_ELINKNO_BITER(trancount);//��ʼ��ѭ������ 
			//���� DMA ����Դ 
			 DMAMUX_CHCFG_REG(DMAMUX_BASE_PTR, damch) |= DMAMUX_CHCFG_SOURCE(DMSour) ;                  // ͨ����������Դ: ����ͨ����Ҫ�Լ��޸�                                        
			 DMA_IRQ_CLEAN(damch);                             //���ͨ�������жϱ�־λ
			 DMA_EN(damch);                                    //ʹ��ͨ��CHn Ӳ������ 
			 DMA_IRQ_EN(damch) ;   
		 }
		 else{
			 returnda = ERROR  ;
		 }
		 return returnda ;

}

/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�UVA_FTM_init
*  ����˵����PWMͨ����ʼ��
*  ����˵����    
*  �������أ���
*  �޸�ʱ�䣺2016-7-10   �Ѳ���
*  ��    ע��
*************************************************************************/
void UVA_FTM_init(void){
	FTM_PWM_init(SFTM3,CH2,400,50);
	FTM_PWM_init(SFTM3,CH3,400,50);
	FTM_PWM_init(SFTM3,CH4,400,50);
	FTM_PWM_init(SFTM3,CH5,400,50);
}

/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�UVA_FTM
*  ����˵����PWMͨ������
*  ����˵����    
*  �������أ���
*  �޸�ʱ�䣺2016-7-10   �Ѳ���
*  ��    ע��
*************************************************************************/
void UVA_FTM(uint16* pwmduty){
	uint32 mod,cv[4] ;
	uint8 temp0 ;
	    //ռ�ձ� = (CnV-CNTIN)/(MOD-CNTIN+1)
    mod = FTM_MOD_REG(FTMx[SFTM3]);        //��ȡ MOD ��ֵ
	  for(temp0 = 0 ; temp0 < 4 ;temp0++){
     cv[temp0] = ((pwmduty[temp0]) * (mod - 0 + 1)) / FTM_PRECISON;
		}

    // ����FTMͨ��ֵ
		FTM_CnV_REG(FTMx[SFTM3], CH2) = cv[0];
		FTM_CnV_REG(FTMx[SFTM3], CH3) = cv[1];
		FTM_CnV_REG(FTMx[SFTM3], CH4) = cv[2];
    FTM_CnV_REG(FTMx[SFTM3], CH5) = cv[3];


}
/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�UVA_CH_FTM_init
*  ����˵����PWMͨ����ʼ��
*  ����˵����    
*  �������أ���
*  �޸�ʱ�䣺2016-7-10   �Ѳ���
*  ��    ע��
*************************************************************************/
void UVA_CH_FTM_init(void){
	FTM_Input_init(SFTM0,CH3,Rising_or_Falling);
	FTM_Input_init(SFTM0,CH4,Rising_or_Falling);
	FTM_Input_init(SFTM0,CH5,Rising_or_Falling);
	FTM_Input_init(SFTM0,CH6,Rising_or_Falling);
	FTM_Input_init(SFTM0,CH7,Rising_or_Falling);

}
