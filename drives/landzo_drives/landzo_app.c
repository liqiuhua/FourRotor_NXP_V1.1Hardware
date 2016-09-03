#include "landzo_app.h"

/*************************************************************************
*                             蓝宙电子科技有限公司
*
*  函数名称：Uart_DMA
*  功能说明：进行DMA数据发送数据装载
*  参数说明：damch   触发通道
             souradd 源地址
						 daadd   目标地址
						 trancount 传送次数
						 DMSour     触发端口
*  函数返回：无
*  修改时间：2016-7-10   已测试
*  备    注：
*************************************************************************/
uint8 DMAflag = 0xff;
uint8 Uart_DMA(uint8 damch ,void *souradd,void *daadd,uint32 trancount,DMA_sources DMSour ){

	   uint8 returnda ;
	   
	   if(DMAflag&(1<<damch)){  //如果通道为空才可以装载数据
			 returnda = OK  ;
			 DMAflag &= ~(1<<damch);
			 DMA_SADDR(damch) =  (uint32_t)souradd;                      // 设置  源地址
			 DMA_DADDR(damch) =  (uint32_t)daadd;                         // 设置目的地址
			 DMA_CITER_ELINKNO(damch)  = DMA_CITER_ELINKNO_CITER(trancount); //当前主循环次数
			 DMA_BITER_ELINKNO(damch)  = DMA_BITER_ELINKNO_BITER(trancount);//起始主循环次数 
			//配置 DMA 触发源 
			 DMAMUX_CHCFG_REG(DMAMUX_BASE_PTR, damch) |= DMAMUX_CHCFG_SOURCE(DMSour) ;                  // 通道触发传输源: 触发通道需要自己修改                                        
			 DMA_IRQ_CLEAN(damch);                             //清除通道传输中断标志位
			 DMA_EN(damch);                                    //使能通道CHn 硬件请求 
			 DMA_IRQ_EN(damch) ;   
		 }
		 else{
			 returnda = ERROR  ;
		 }
		 return returnda ;

}

/*************************************************************************
*                             蓝宙电子科技有限公司
*
*  函数名称：UVA_FTM_init
*  功能说明：PWM通道初始化
*  参数说明：    
*  函数返回：无
*  修改时间：2016-7-10   已测试
*  备    注：
*************************************************************************/
void UVA_FTM_init(void){
	FTM_PWM_init(SFTM3,CH2,400,50);
	FTM_PWM_init(SFTM3,CH3,400,50);
	FTM_PWM_init(SFTM3,CH4,400,50);
	FTM_PWM_init(SFTM3,CH5,400,50);
}

/*************************************************************************
*                             蓝宙电子科技有限公司
*
*  函数名称：UVA_FTM
*  功能说明：PWM通道参数
*  参数说明：    
*  函数返回：无
*  修改时间：2016-7-10   已测试
*  备    注：
*************************************************************************/
void UVA_FTM(uint16* pwmduty){
	uint32 mod,cv[4] ;
	uint8 temp0 ;
	    //占空比 = (CnV-CNTIN)/(MOD-CNTIN+1)
    mod = FTM_MOD_REG(FTMx[SFTM3]);        //读取 MOD 的值
	  for(temp0 = 0 ; temp0 < 4 ;temp0++){
     cv[temp0] = ((pwmduty[temp0]) * (mod - 0 + 1)) / FTM_PRECISON;
		}

    // 配置FTM通道值
		FTM_CnV_REG(FTMx[SFTM3], CH2) = cv[0];
		FTM_CnV_REG(FTMx[SFTM3], CH3) = cv[1];
		FTM_CnV_REG(FTMx[SFTM3], CH4) = cv[2];
    FTM_CnV_REG(FTMx[SFTM3], CH5) = cv[3];


}
/*************************************************************************
*                             蓝宙电子科技有限公司
*
*  函数名称：UVA_CH_FTM_init
*  功能说明：PWM通道初始化
*  参数说明：    
*  函数返回：无
*  修改时间：2016-7-10   已测试
*  备    注：
*************************************************************************/
void UVA_CH_FTM_init(void){
	FTM_Input_init(SFTM0,CH3,Rising_or_Falling);
	FTM_Input_init(SFTM0,CH4,Rising_or_Falling);
	FTM_Input_init(SFTM0,CH5,Rising_or_Falling);
	FTM_Input_init(SFTM0,CH6,Rising_or_Falling);
	FTM_Input_init(SFTM0,CH7,Rising_or_Falling);

}
