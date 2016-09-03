/***********************************************************
 * 文件名       ：K6x_uart.c
 * 说明         ：串口初始化
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 * 库版本       ：V1.0
 *时间          ：15.5.26
************************************************************/

//#include "common.h"
#include "K6x_uart.h"
#include "K6x_delay.h"

volatile UART_Type *UARTx[6] = {UART0_BASE_PTR, UART1_BASE_PTR, UART2_BASE_PTR, UART3_BASE_PTR, UART4_BASE_PTR, UART5_BASE_PTR}; //定义五个指针数组保存 UARTx 的地址

/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：uart_DMA_init
*  功能说明：初始化串口，设置波特率
*  参数说明：UARTn       模块号（UART0~UART5）
*            baud        波特率，如9600、19200、56000、115200等
*  函数返回：无
*  修改时间：2012-1-20
*  备    注：在官方例程上修改
*************************************************************************/
void uart_DMA_init (UARTn uratn, uint32 baud)
{
    register uint16 sbr, brfa;
    uint8 temp;
    uint32 sysclk;     //时钟

    
    switch(uratn)
    {
    case KUART0:
        if(UART0_RX == PTA1)
          port_alt(UART0_RX,ALT2);
        else 
          port_alt(UART0_RX,ALT3);

        if(UART0_TX == PTA2)
            port_alt(UART0_TX,ALT2);
        else 
            port_alt(UART0_TX,ALT3);

        SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;      //使能 UARTn 时钟
        break;

    case KUART1:
        port_alt(UART1_RX,ALT3);
        port_alt(UART1_TX,ALT3);
        SIM_SCGC4 |= SIM_SCGC4_UART1_MASK;
        break;

    case KUART2:
        port_alt(UART2_RX,ALT3);
        port_alt(UART2_TX,ALT3);
        SIM_SCGC4 |= SIM_SCGC4_UART2_MASK;
        break;

    case KUART3:
        port_alt(UART3_RX,ALT3);
        port_alt(UART3_TX,ALT3);
        SIM_SCGC4 |= SIM_SCGC4_UART3_MASK;
        break;

    case KUART4:
        port_alt(UART4_RX,ALT3);
        port_alt(UART4_TX,ALT3);
        SIM_SCGC1 |= SIM_SCGC1_UART4_MASK;
        break;

    case KUART5:
        port_alt(UART5_RX,ALT3);
        port_alt(UART5_TX,ALT3);

        SIM_SCGC1 |= SIM_SCGC1_UART5_MASK;
        break;
    default:
        break;
    }

    //设置的时候，应该禁止发送接受
    UART_C2_REG(UARTx[uratn]) &= ~(UART_C2_TE_MASK  | UART_C2_RE_MASK );

    //配置成8位无校验模式
    //设置 UART 数据格式、校验方式和停止位位数。通过设置 UART 模块控制寄存器 C1 实现；
    UART_C1_REG(UARTx[uratn]) = 0;	// 全部直接使用默认设置就行，所以直接清0

    //计算波特率，串口0、1使用内核时钟，其它串口使用外设时钟
    if ((uratn == KUART0) || (uratn == KUART1))
        sysclk = SYSCLKKH * 1000;            //内核时钟
    else
        sysclk = BUSCLKkh * 1000;  //外设时钟

    //设置 UART 数据通讯波特率。通过设置 UART 模块的波特率寄存器
    sbr = (uint16)(sysclk / (baud << 4));

    /* Save off the current value of the UARTx_BDH except for the SBR field */
    temp = UART_BDH_REG(UARTx[uratn]) & ~(UART_BDH_SBR(0x1F));

    UART_BDH_REG(UARTx[uratn]) = temp |  UART_BDH_SBR(((sbr & 0x1F00) >> 8));
    UART_BDL_REG(UARTx[uratn]) = (uint8)(sbr & UART_BDL_SBR_MASK);

    //brfa = (((sysclk*32)/(baud * 16)) - (sbr * 32));
    brfa = (((sysclk << 5) / (baud << 4)) - (sbr << 5));

    /* Save off the current value of the UARTx_C4 register except for the BRFA field */
    temp = UART_C4_REG(UARTx[uratn]) & ~(UART_C4_BRFA(0x1F));

    UART_C4_REG(UARTx[uratn]) = temp |  UART_C4_BRFA(brfa);
    
    /* 允许DMA发送 */
    UART_C5_REG(UARTx[uratn]) |= UART_C5_TDMAS_MASK;

    /* 允许发送和接收 */
    UART_C2_REG(UARTx[uratn]) |= (UART_C2_TE_MASK | UART_C2_RE_MASK |UART_C2_TIE_MASK);

}


/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：uart_init
*  功能说明：初始化串口，设置波特率
*  参数说明：UARTn       模块号（UART0~UART5）
*            baud        波特率，如9600、19200、56000、115200等
*  函数返回：无
*  修改时间：2012-1-20
*  备    注：在官方例程上修改
*************************************************************************/
void uart_init (UARTn uratn, uint32 baud)
{
    register uint16 sbr, brfa;
    uint8 temp;
    uint32 sysclk;     //时钟

    switch(uratn)
    {
    case KUART0:
        if(UART0_RX == PTA1)
          port_alt(UART0_RX,ALT2);
        else 
          port_alt(UART0_RX,ALT3);

        if(UART0_TX == PTA2)
            port_alt(UART0_TX,ALT2);
        else 
            port_alt(UART0_TX,ALT3);

        SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;      //使能 UARTn 时钟
        break;

    case KUART1:
        port_alt(UART1_RX,ALT3);
        port_alt(UART1_TX,ALT3);
        SIM_SCGC4 |= SIM_SCGC4_UART1_MASK;
        break;

    case KUART2:
        port_alt(UART2_RX,ALT3);
        port_alt(UART2_TX,ALT3);
        SIM_SCGC4 |= SIM_SCGC4_UART2_MASK;
        break;

    case KUART3:
        port_alt(UART3_RX,ALT3);
        port_alt(UART3_TX,ALT3);
        SIM_SCGC4 |= SIM_SCGC4_UART3_MASK;
        break;

    case KUART4:
        port_alt(UART4_RX,ALT3);
        port_alt(UART4_TX,ALT3);
        SIM_SCGC1 |= SIM_SCGC1_UART4_MASK;
        break;

    case KUART5:
        port_alt(UART5_RX,ALT3);
        port_alt(UART5_TX,ALT3);

        SIM_SCGC1 |= SIM_SCGC1_UART5_MASK;
        break;
    default:
        break;
    }

    //设置的时候，应该禁止发送接受
    UART_C2_REG(UARTx[uratn]) &= ~(UART_C2_TE_MASK  | UART_C2_RE_MASK );

    //配置成8位无校验模式
    //设置 UART 数据格式、校验方式和停止位位数。通过设置 UART 模块控制寄存器 C1 实现；
    UART_C1_REG(UARTx[uratn]) = 0;	// 全部直接使用默认设置就行，所以直接清0

    //计算波特率，串口0、1使用内核时钟，其它串口使用外设时钟
    if ((uratn == KUART0) | (uratn == KUART1))
        sysclk = SYSCLKKH * 1000;            //内核时钟
    else
        sysclk = BUSCLKkh * 1000;  //外设时钟

    //设置 UART 数据通讯波特率。通过设置 UART 模块的波特率寄存器
    sbr = (uint16)(sysclk / (baud << 4));

    /* Save off the current value of the UARTx_BDH except for the SBR field */
    temp = UART_BDH_REG(UARTx[uratn]) & ~(UART_BDH_SBR(0x1F));

    UART_BDH_REG(UARTx[uratn]) = temp |  UART_BDH_SBR(((sbr & 0x1F00) >> 8));
    UART_BDL_REG(UARTx[uratn]) = (uint8)(sbr & UART_BDL_SBR_MASK);

    brfa = (((sysclk << 5) / (baud << 4)) - (sbr << 5));

    /* Save off the current value of the UARTx_C4 register except for the BRFA field */
    temp = UART_C4_REG(UARTx[uratn]) & ~(UART_C4_BRFA(0x1F));

    UART_C4_REG(UARTx[uratn]) = temp |  UART_C4_BRFA(brfa);

    /* 允许发送和接收 */
    UART_C2_REG(UARTx[uratn]) |= (UART_C2_TE_MASK | UART_C2_RE_MASK );
//    enable_irq(uratn + (INT_UART0_RX_TX+INC_IRQ));
}

/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：uart_getchar
*  功能说明：无限时间等待串口接受一个字节
*  参数说明：UARTn       模块号（UART0~UART5）
*  函数返回：接收到的字节
*  修改时间：2012-1-20
*  备    注：官方例程
*************************************************************************/
char uart_getchar (UARTn uratn)
{
    /* Wait until character has been received */
    while (!(UART_S1_REG(UARTx[uratn]) & UART_S1_RDRF_MASK));

    /* Return the 8-bit data from the receiver */
    return UART_D_REG(UARTx[uratn]);
}

/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：uart_pendchar
*  功能说明：有限时间等待串口接受一个字节
*  参数说明：UARTn       模块号（UART0~UART5）
*  函数返回：接收到的字节
*  修改时间：2012-1-20
*  备    注：
*************************************************************************/
char uart_rechar (UARTn uratn, char *ch)
{
    uint32 i = 0;

    while(++i < 0xffffff)                                         //时间限制
    {
        if(UART_S1_REG(UARTx[uratn]) & UART_S1_RDRF_MASK)         //查询是否接受到数据
        {
            *ch  =   UART_D_REG(UARTx[uratn]);                    //接受到8位的数据
            return  1;                                            //返回 1 表示接收成功
        }
    }

    *ch = 0;                                                     //接收不到，应该清空了接收区
    return 0;                                                    //返回0表示接收失败
}


/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：uart_putchar
*  功能说明：串口发送一个字节
*  参数说明：UARTn       模块号（UART0~UART5）
*  函数返回：无
*  修改时间：2012-1-20
*  备    注：官方例程
*************************************************************************/
void uart_putchar (UARTn uratn, char ch)
{
    //等待发送缓冲区空
    while(!(UART_S1_REG(UARTx[uratn]) & UART_S1_TDRE_MASK));
    //发送数据
    UART_D_REG(UARTx[uratn]) = (uint8)ch;
}

/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：uart_query
*  功能说明：查询是否接受到一个字节
*  参数说明：UARTn       模块号（UART0~UART5）
*  函数返回：1           接收到一个字节了
*            0           没有接收到
*  修改时间：2012-1-20
*  备    注：官方例程
*************************************************************************/
char uart_query (UARTn uratn)
{
    return (UART_S1_REG(UARTx[uratn]) & UART_S1_RDRF_MASK);
}

/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：uart_sendN
*  功能说明：串行 发送指定len个字节长度字符串 （包括 NULL 也会发送）
*  参数说明：UARTn       模块号（UART0~UART5）
*            buff        发送缓冲区
*            len         发送长度
*  函数返回：无
*  修改时间：2012-1-20
*  备    注：
*************************************************************************/
void uart_sendN (UARTn uratn, uint8 *buff, uint16 len)
{
    int i;
    for(i = 0; i < len; i++)
    {
        uart_putchar(uratn, buff[i]);
			delay_ms(1);
    }
}
/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：uart_sendN
*  功能说明：串行 字符串 （包括 NULL 也会发送）
*  参数说明：UARTn       模块号（UART0~UART5）
*            str        发送字符串     
*  函数返回：无
*  修改时间：2012-1-20
*  备    注：
*************************************************************************/
void uart_sendStr(UARTn uratn, const uint8 *str)
{
  while(*str)
  {
    uart_putchar(uratn, *str++);
  }
}

/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：uart_irq_EN
*  功能说明：串口中断使能
*  参数说明：UARTn       模块号（UART0~UART5）
*            
*  函数返回：无
*  修改时间：2016-7-4
*  备    注：
*************************************************************************/
void uart_irq_EN (UARTn uratn){
	
	UART_C2_REG(UARTx[uratn])|=UART_C2_RIE_MASK;
	if(uratn <= 3) 
	  NVIC_EnableIRQ((IRQn_Type)((uratn<<1)+UART0_RX_TX_IRQn)) ;
	else
		NVIC_EnableIRQ((IRQn_Type)(((uratn-4)<<1)+UART4_RX_TX_IRQn)) ;
	
}
	
/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：uart_irq_DIS
*  功能说明：串口中断使能
*  参数说明：UARTn       模块号（UART0~UART5）
*            
*  函数返回：无
*  修改时间：2016-7-4
*  备    注：
*************************************************************************/
void uart_irq_DIS (UARTn uratn){
	
	UART_C2_REG(UARTx[uratn])&= ~UART_C2_RIE_MASK;
	if(uratn <= 3) 
	  NVIC_DisableIRQ((IRQn_Type)((uratn<<1)+UART0_RX_TX_IRQn)) ;
	else
		NVIC_DisableIRQ((IRQn_Type)((uratn<<1)+UART4_RX_TX_IRQn)) ;
	
}
