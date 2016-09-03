/***********************************************************
 * 文件名       ：can.c
 * 说明         ：K60 CAN底层驱动程序函数
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 *库版本        ：V1.0
 *时间          ：15.5.26
************************************************************/
#include "k6x_can.h"

//内部使用函数声明
uint8 SetCANBand(uint8 CANChannel,uint32 baudrateKHz);


/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：CANInit
*  功能说明：查询是否接受到一个字节
*  参数说明：CANChannel：模块号
//		   baudrateKHz: 波特率
//         selfLoop: 模式选择(1=回环模式；0=正常模式)
//         idMask: ID过滤(1=ID过滤；0=ID不过滤)
*  函数返回：0：成功；1：失败
*  修改时间：2012-1-20
*  备    注：参考苏州大学的例程
*************************************************************************/
uint8 CANInit(uint8 CANChannel,uint32 baudrateKHz,uint8 selfLoop,uint8 idMask)
{
    int8 i;
    CAN_MemMapPtr CANBaseAdd;
  
    //使能FlexCAN外部时钟
    OSC_CR |= OSC_CR_ERCLKEN_MASK | OSC_CR_EREFSTEN_MASK;
    
    //通过模块号选择模块基地址
    if(CANChannel == 0)
        CANBaseAdd = CAN0_BASE_PTR;
#ifndef MK64F12 
    else if(CANChannel == 1)
        CANBaseAdd = CAN1_BASE_PTR;
#endif   
    //使能CAN模块时钟
    if(CANBaseAdd == CAN0_BASE_PTR)
        SIM_SCGC6 |=  SIM_SCGC6_FLEXCAN0_MASK;//使能CAN0的时钟模块
#ifndef MK64F12
    else
        SIM_SCGC3 |= SIM_SCGC3_FLEXCAN1_MASK;//使能CAN1的时钟模块
#endif    
   

    //配置CAN_RX/TX复用引脚功能
    if(CANChannel == 0)
    {
		PORTA_PCR12 = PORT_PCR_MUX(2) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; //上拉
		PORTA_PCR13 = PORT_PCR_MUX(2) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; //上拉
    }
    else
    {
    	PORTE_PCR24 = PORT_PCR_MUX(2) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; //Tx上拉
    	PORTE_PCR25 = PORT_PCR_MUX(2) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; //Rx上拉
    } 
    
    //选择时钟源，外设时钟48MHz，内部时钟：12MHz
    CAN_CTRL1_REG(CANBaseAdd) |= CAN_CTRL1_CLKSRC_MASK;//选择内部时钟    
    
    CAN_MCR_REG(CANBaseAdd) |= CAN_MCR_FRZ_MASK;  //使能冻结模式   
    CAN_MCR_REG(CANBaseAdd) &= ~CAN_MCR_MDIS_MASK;//使能CAN模块
    //确认已退出冻结模式
    while((CAN_MCR_LPMACK_MASK & CAN_MCR_REG(CANBaseAdd)));

    //软件复位
    CAN_MCR_REG(CANBaseAdd) ^= CAN_MCR_SOFTRST_MASK; 
    //等待复位完成
    while(CAN_MCR_SOFTRST_MASK & CAN_MCR_REG(CANBaseAdd));   
    //等待进入冻结模式 
    while(!(CAN_MCR_FRZACK_MASK & CAN_MCR_REG(CANBaseAdd)));
    
    //将16个邮箱缓冲区内容清0
    for(i=0;i<16;i++)
    {
          CANBaseAdd->MB[i].CS = 0x00000000;
          CANBaseAdd->MB[i].ID = 0x00000000;
          CANBaseAdd->MB[i].WORD0 = 0x00000000;
          CANBaseAdd->MB[i].WORD1 = 0x00000000;
    }
    
    //接收邮箱过滤IDE比较，RTR不比较
    CAN_CTRL2_REG(CANBaseAdd) &= ~CAN_CTRL2_EACEN_MASK;
    //远程请求帧产生
    CAN_CTRL2_REG(CANBaseAdd) &= ~CAN_CTRL2_RRS_MASK;
    //邮箱首先从接收FIFO队列匹配然后再在邮箱中匹配
    CAN_CTRL2_REG(CANBaseAdd) &= ~CAN_CTRL2_MRP_MASK;
 
    //使用一个32位过滤器
    CAN_MCR_REG(CANBaseAdd) |= (CAN_MCR_REG(CANBaseAdd) & ~CAN_MCR_IDAM_MASK) | CAN_MCR_IDAM(0);
    //设置波特率
    if(SetCANBand(CANChannel,baudrateKHz) == 1)//若设置错误
        return 1;
    
    //模式选择：回环模式或正常模式
    if(1==selfLoop)
        CAN_CTRL1_REG(CANBaseAdd) |= CAN_CTRL1_LPB_MASK;//使用回环模式

    //初始化掩码寄存器
    if(1==idMask)//屏蔽ID
    {
    	CAN_RXMGMASK_REG(CANBaseAdd) = 0x1FFFFFFF;
		CAN_RX14MASK_REG(CANBaseAdd) = 0x1FFFFFFF;
		CAN_RX15MASK_REG(CANBaseAdd) = 0x1FFFFFFF;
    }
    else//不屏蔽ID
    {
    	CAN_RXMGMASK_REG(CANBaseAdd) = 0x0;
		CAN_RX14MASK_REG(CANBaseAdd) = 0x0;
		CAN_RX15MASK_REG(CANBaseAdd) = 0x0;
    }

    //如果单独掩码功能使能，为每个队列初始化单独的掩码寄存器
    if(CAN_MCR_REG(CANBaseAdd) & CAN_MCR_IRMQ_MASK)
    {
        for(i = 0; i < NUMBER_OF_MB ; i++)
        {        
            CANBaseAdd->RXIMR[i] = 0x1FFFFFFFL;
        }
    }
    
    //只有在冻结模式下才能配置，配置完推出冻结模式
	CAN_MCR_REG(CANBaseAdd) &= ~(CAN_MCR_HALT_MASK);
	//等待直到退出冻结模式
	while( CAN_MCR_REG(CANBaseAdd) & CAN_MCR_FRZACK_MASK);    
	//等到不在冻结模式，休眠模式或者停止模式
	while((CAN_MCR_REG(CANBaseAdd) & CAN_MCR_NOTRDY_MASK));
    
    return 0;
}


/*************************************************************************
*                             蓝宙电子科技有限公司
*
*  函数名称：CANSendData
*  功能说明：发送数据
*  参数说明：CANChannel：模块号         
//          iMB：缓冲区号
//	    id: ID号
//          length：数据长度
//	    Data[8]:发送数据缓冲区
*  函数返回：0：成功，1：失败
*  修改时间：2015-5-15   已测试
*  备    注：苏州大学的例程
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
    //缓冲区和数据长度设置错误
    if(iMB >= NUMBER_OF_MB || length >8)
        return 1; //超出范围
    
    //转换8个字节转换成32位的word存储
    wno = (length-1)>>2;//是否超过4字节
    bno = (length-1)%4; //
    if(wno > 0)         //长度大于4(即发送数据超过4字节)
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
    // ID 格式
    // B31 30 29 28 27 26 ... 11 10 9 8 7 6 5 4 3 2 1 0
    // |	|	 |									  |
    // |    |    |------------------------------------|
    // |	|					|--> 29 位 ID
    // |	|
    // |    |--> RTR  1: 远程帧, 0: 数据帧
    // |
    // |-------> IDE 1 : 扩展ID, 0: 标准ID
    ///////////////////////////////////////////////////////
    
    //通过id判断帧类型——扩展帧
    wno = (id &  CAN_MSG_IDE_MASK)>>CAN_MSG_IDE_BIT_NO;  //IDE
    //通过id判断帧类型——远程帧
    bno = (id &  CAN_MSG_TYPE_MASK)>>CAN_MSG_TYPE_BIT_NO;//RTR
    
    //获得ID位数
    i =  wno? 0: FLEXCAN_MB_ID_STD_BIT_NO;
    
    //以下四步骤为发送过程
    CANBaseAdd->MB[iMB].CS = FLEXCAN_MB_CS_CODE(FLEXCAN_MB_CODE_TX_INACTIVE)//缓冲区写激活代码
							| (wno<<FLEXCAN_MB_CS_IDE_BIT_NO)//缓冲区写IDE位
							| (bno<<FLEXCAN_MB_CS_RTR_BIT_NO)//缓冲区写RTR位
                            | FLEXCAN_MB_CS_LENGTH(length);  //缓冲区写数据长度
    
    //缓冲区写ID
    CANBaseAdd->MB[iMB].ID = (1 << FLEXCAN_MB_ID_PRIO_BIT_NO) 
							| ((id & ~(CAN_MSG_IDE_MASK|CAN_MSG_TYPE_MASK))<<i);  
    
    //缓冲区写内容
    CANBaseAdd->MB[iMB].WORD0 = word[0];
    CANBaseAdd->MB[iMB].WORD1 = word[1];  
    
    //延迟
    for(i = 0;i < 100;i++);
    
    //通过制定的发送代码开始发送
    CANBaseAdd->MB[iMB].CS = (CANBaseAdd->MB[iMB].CS & ~(FLEXCAN_MB_CS_CODE_MASK)) 
							| FLEXCAN_MB_CS_CODE(FLEXCAN_MB_CODE_TX_ONCE)//写激活代码
							| FLEXCAN_MB_CS_LENGTH(length);//缓冲区写数据长度 
    
    //限时等待发送完成（如果使用中断则限时等待语句可删除）
    i=0;
    while(!(CANBaseAdd->IFLAG1 & (1<<iMB)))
    {
    	if((i++)>0x1000)
    		return 1;
    }
    //清报文缓冲区中断标志
    CANBaseAdd->IFLAG1 = (1<<iMB);
    return 0;
}


/*************************************************************************
*                             蓝宙电子科技有限公司
*
*  函数名称：CANEnableRXBuff
*  功能说明：使能接收缓冲区
*  参数说明：CANChannel：模块号         
//          iMB：缓冲区号
//	    id: ID号
*  函数返回：
*  修改时间：2015-5-15   已测试
*  备    注：苏州大学的例程
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
    //将MB配置为非激活状态
    CANBaseAdd->MB[iMB].CS = FLEXCAN_MB_CS_CODE(FLEXCAN_MB_CODE_RX_INACTIVE); 	
    
    //取出29位单独的ID
    id2 = id & ~(CAN_MSG_IDE_MASK | CAN_MSG_TYPE_MASK);
    if(id & CAN_MSG_IDE_MASK)//扩展帧
    {
        CANBaseAdd->MB[iMB].ID = id2;
        cs |= FLEXCAN_MB_CS_IDE;//置位IDE位
    }
    else//标准帧
    {
        CANBaseAdd->MB[iMB].ID = id2<<FLEXCAN_MB_ID_STD_BIT_NO;          
    }
    
    //激活接收缓冲区，code写0100
    CANBaseAdd->MB[iMB].CS = cs;      
}

/*************************************************************************
*                             蓝宙电子科技有限公司
*
*  函数名称：接收数据
*  功能说明：使能接收缓冲区
*  参数说明：CANChannel：CAN模块号
//          iMB：缓冲区号
//	    id: ID号
//	    lenght：数据长度
//	    Data[8]: 和接收数据缓冲区
*  函数返回：0：成功，1：失败
*  修改时间：2015-5-15   已测试
*  备    注：苏州大学的例程
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
    // 锁定MB
    code = FLEXCAN_get_code(CANBaseAdd->MB[iMB].CS);
    
    if(code != 0x02)//缓冲区没有接收到数据，返回错误
    {
        *Datalenght = 0;
        return 1;
    }
    length = FLEXCAN_get_length(CANBaseAdd->MB[iMB].CS);
    
    if(length <1)//接收到的数据长度小于1，返回错误
    {
        *Datalenght = 0;
        return 1;
    }
   
    //判断是标准帧还是扩展帧
    format = (CANBaseAdd->MB[iMB].CS & FLEXCAN_MB_CS_IDE)? 1:0;
    *id = (CANBaseAdd->MB[iMB].ID & FLEXCAN_MB_ID_EXT_MASK);
 
    if(!format)
    {
        *id >>= FLEXCAN_MB_ID_STD_BIT_NO; // 获得标准帧号
    }
    else
    { 
        *id |= CAN_MSG_IDE_MASK; //标记扩展的ID        
    }
 
    format = (CANBaseAdd->MB[iMB].CS & FLEXCAN_MB_CS_RTR)? 1:0;  
    if(format)
    {
        *id |= CAN_MSG_TYPE_MASK; //标记为远程帧类型       
    }
    //读取报文数据
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
*                             蓝宙电子科技有限公司
*
*  函数名称：EnableCANInterrupt
*  功能说明：使能缓冲区接收和发送中断
*  参数说明：CANChannel:CAN模块号
//          iMB:缓冲区号
*  函数返回：
*  修改时间：2015-5-15   已测试
*  备    注：苏州大学的例程
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
*                             蓝宙电子科技有限公司
*
*  函数名称：DisableCANInterrupt
*  功能说明：禁止缓冲区接收和发送中断
*  参数说明：CANChannel:CAN模块号
//         iMB:缓冲区号
*  函数返回：
*  修改时间：2015-5-15   已测试
*  备    注：苏州大学的例程
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
*                             蓝宙电子科技有限公司
*
*  函数名称：CANClearFlag
*  功能说明：清缓冲区中断标志
*  参数说明：CANChannel:CAN模块号
//         iMB:缓冲区号
*  函数返回：
*  修改时间：2015-5-15   已测试
*  备    注：苏州大学的例程
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
*                             蓝宙电子科技有限公司
*
*  函数名称：CANGetFlag
*  功能说明：获得缓冲区中断标志
*  参数说明：CANChannel:CAN模块号
//         iMB:缓冲区号
*  函数返回：
*  修改时间：2015-5-15   已测试
*  备    注：苏州大学的例程
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
*                             蓝宙电子科技有限公司
*
*  函数名称：SetCANBand
*  功能说明：设置CAN的波特率
*  参数说明：CANChannel:CAN模块号
//          baudrateKHz:波特率
*  函数返回：0:配置成功 1：配置成功
*  修改时间：2015-5-15   已测试
*  备    注：苏州大学的例程
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
