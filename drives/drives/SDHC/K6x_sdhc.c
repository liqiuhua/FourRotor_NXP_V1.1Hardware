/***********************************************************
 * �ļ���       ��K6x_sdhc.c
 * ˵��         ��SD���ܺ���
 * ����         ��landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://www.landzo.cn
 *��汾        ��V1.0
 *ʱ��          ��15.5.26
************************************************************/

#include "K6x_sdhc.h"

SDCARD_STRUCT 		SDHC_Card;
SDCARD_INIT_STRUCT  SDHC_Init;
ESDHC_INFO_STRUCT	SDHC_Info;
ESDHC_DEVICE_STRUCT SDHC_Device;

const ESDHC_INIT_STRUCT K60_SDHC0_init = {
    0,                          //�豸��
    25000000,                   // ESDHC baudrate 
    BSP_SYSTEM_CLOCK            //ʱ��Ƶ�� 
};

//����ִ�е��������ÿ������ִ��ʱ��XFERTYP�Ĵ��������������
static const unsigned long ESDHC_COMMAND_XFERTYP[] = 
{
	// CMD0
    SDHC_XFERTYP_CMDINX(ESDHC_CMD0) | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_NO),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD1) | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_NO),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD2) | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_136),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD3) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD4) | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_NO),
    // CMD5 
    SDHC_XFERTYP_CMDINX(ESDHC_CMD5) | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD6) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD7) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD8) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD9) | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_136),
    // CMD10 
    SDHC_XFERTYP_CMDINX(ESDHC_CMD10) | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_136),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD11) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD12) | SDHC_XFERTYP_CMDTYP(ESDHC_XFERTYP_CMDTYP_ABORT) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD13) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    0,
    // CMD15 
    SDHC_XFERTYP_CMDINX(ESDHC_CMD15) | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_NO),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD16) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD17) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD18) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    0,
    // CMD20 
    SDHC_XFERTYP_CMDINX(ESDHC_CMD20) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    0,
    SDHC_XFERTYP_CMDINX(ESDHC_ACMD22) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_ACMD23) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD24) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    // CMD25 
    SDHC_XFERTYP_CMDINX(ESDHC_CMD25) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD26) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD27) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD28) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD29) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY),
    // CMD30 
    SDHC_XFERTYP_CMDINX(ESDHC_CMD30) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    0,
    SDHC_XFERTYP_CMDINX(ESDHC_CMD32) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD33) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD34) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    // CMD35 
    SDHC_XFERTYP_CMDINX(ESDHC_CMD35) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD36) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD37) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD38) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD39) | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    // CMD40 
    SDHC_XFERTYP_CMDINX(ESDHC_CMD40) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_ACMD41) | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD42) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY),
    0,
    0,
    // CMD45 
    0,
    0,
    0,
    0,
    0,
    // CMD50 
    0,
    SDHC_XFERTYP_CMDINX(ESDHC_ACMD51) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD52) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD53) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    0,
    // CMD55 
    SDHC_XFERTYP_CMDINX(ESDHC_CMD55) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD56) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY),
    0,
    0,
    0,
    // CMD60 
    SDHC_XFERTYP_CMDINX(ESDHC_CMD60) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD61) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY),
    0,
    0
};
static void SDHC_set_baudrate(uint32 clock, uint32 baudrate);
static uint8 SDHC_is_running(void);
static uint32 SDHC_status_wait(uint32 mask);
static uint32 SDHC_send_command (ESDHC_COMMAND_STRUCT_PTR command);

/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�sdhc_init
*  ����˵������ʼ��SDHCģ�顣
*  ����˵����esdhc_info_ptr:SDHC                                                    
//         esdhc_init_ptr:SDHC��ʼ���ṹ��ָ��
*  �������أ�ESDHC_OK �ɹ�ʱ����;��������ֵΪ���� 
*  �޸�ʱ�䣺
*  ��    ע�����ݴ�ѧ������
*************************************************************************/
uint32 sdhc_init(ESDHC_INFO_STRUCT_PTR  esdhc_info_ptr,ESDHC_INIT_STRUCT_CPTR esdhc_init_ptr)					
{
	esdhc_info_ptr->CARD = ESDHC_CARD_NONE;
	
    //ʹ��SDHCģ���ʱ����
    SIM_SCGC3 |= SIM_SCGC3_ESDHC_MASK;
    
	//��λSDHC
    SDHC_SYSCTL = SDHC_SYSCTL_RSTA_MASK | SDHC_SYSCTL_SDCLKFS(0x80);    
    while (SDHC_SYSCTL & SDHC_SYSCTL_RSTA_MASK){};
    
    //��ʼ��SDHC��ؼĴ���
    SDHC_VENDOR = 0;
    SDHC_BLKATTR = SDHC_BLKATTR_BLKCNT(1) | SDHC_BLKATTR_BLKSIZE(512);
    SDHC_PROCTL = SDHC_PROCTL_EMODE(ESDHC_PROCTL_EMODE_INVARIANT) | SDHC_PROCTL_D3CD_MASK; 
    SDHC_WML = SDHC_WML_RDWML(1) | SDHC_WML_WRWML(1);
    
    //����SDHCģ���ͨ������
    SDHC_set_baudrate (esdhc_init_ptr->CLOCK_SPEED,esdhc_init_ptr->BAUD_RATE);
    
    //���CMDͨ����DATͨ���Ƿ�׼������
    while (SDHC_PRSSTAT & (SDHC_PRSSTAT_CIHB_MASK | SDHC_PRSSTAT_CDIHB_MASK)){};

    //���ø������Ź���ΪSDHC
    PORTE_PCR(0) = 0xFFFF & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    // ESDHC.D1  
    PORTE_PCR(1) = 0xFFFF & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    // ESDHC.D0  
    PORTE_PCR(2) = 0xFFFF & (PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK);                                          // ESDHC.CLK 
    PORTE_PCR(3) = 0xFFFF & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    // ESDHC.CMD 
    PORTE_PCR(4) = 0xFFFF & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    // ESDHC.D3  
    PORTE_PCR(5) = 0xFFFF & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    // ESDHC.D2  
    
    //���SDHCģ����жϱ�־
    SDHC_IRQSTAT = 0xFFFF;
    //ʹ���ж�λ
    SDHC_IRQSTATEN = 	  SDHC_IRQSTATEN_DEBESEN_MASK | SDHC_IRQSTATEN_DCESEN_MASK | SDHC_IRQSTATEN_DTOESEN_MASK 
                         | SDHC_IRQSTATEN_CIESEN_MASK | SDHC_IRQSTATEN_CEBESEN_MASK | SDHC_IRQSTATEN_CCESEN_MASK | SDHC_IRQSTATEN_CTOESEN_MASK 
                         | SDHC_IRQSTATEN_BRRSEN_MASK | SDHC_IRQSTATEN_BWRSEN_MASK | SDHC_IRQSTATEN_CRMSEN_MASK
                         | SDHC_IRQSTATEN_TCSEN_MASK | SDHC_IRQSTATEN_CCSEN_MASK;
    
    //80��ʱ�����ڵĳ�ʼ��
    SDHC_SYSCTL |= SDHC_SYSCTL_INITA_MASK;
    while (SDHC_SYSCTL & SDHC_SYSCTL_INITA_MASK){};

    //��鿨�Ƿ��Ѿ�����
    if (SDHC_PRSSTAT & SDHC_PRSSTAT_CINS_MASK)
    {
        esdhc_info_ptr->CARD = ESDHC_CARD_UNKNOWN;
    }
    SDHC_IRQSTAT |= SDHC_IRQSTAT_CRM_MASK;
    
    return ESDHC_OK;
}
/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�sdhc_receive_block
*  ����˵��������n���ֽ�  
*  ����˵����buff: ���ջ�����                                                 
//		   btr:���ճ���
*  �������أ�1:�ɹ�;0:ʧ��   
*  �޸�ʱ�䣺
*  ��    ע�����ݴ�ѧ������
*************************************************************************/
uint32 sdhc_receive_block (uint8 *buff,uint32 btr)
{
    uint32	bytes, i, j;
    uint32	*ptr = (uint32*)buff;
    ESDHC_INFO_STRUCT_PTR   esdhc_info_ptr;

    //��ȡ����SDHC��Ϣ�����ݽṹ���ָ��
    esdhc_info_ptr = (ESDHC_INFO_STRUCT_PTR)&SDHC_Info;
    
    //���ָ���Ƿ�Ϊ��
    if (NULL == esdhc_info_ptr)
    {
        return 0;
    }    
    
    //����ʶ��Ŀ�����󣬽�������ǰ�����DATͨ���Ƿ�æ������ʹ���У�
    if ((esdhc_info_ptr->CARD == ESDHC_CARD_SD) || (esdhc_info_ptr->CARD == ESDHC_CARD_SDHC) || (esdhc_info_ptr->CARD == ESDHC_CARD_MMC) || (esdhc_info_ptr->CARD == ESDHC_CARD_CEATA))
    {
        while (SDHC_PRSSTAT & SDHC_PRSSTAT_DLA_MASK){};
    }    
    
    //��ȡ����ʱ��ÿ�ζ�ȡ4���ֽ�
    bytes = btr;
    while (bytes)
    {
        i = bytes > 512 ? 512 : bytes;
        for (j = (i + 3) >> 2; j != 0; j--)
        {
            if (SDHC_IRQSTAT & (SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK))
            {
                SDHC_IRQSTAT |= SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK | SDHC_IRQSTAT_BRR_MASK;
                return 0;
            }
            
            while (0 == (SDHC_PRSSTAT & SDHC_PRSSTAT_BREN_MASK)){};

            *ptr++ = SDHC_DATPORT;
        }
        bytes -= i;
    }
    
	return 1;						// Return with success     
}
/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�sdhc_send_block
*  ����˵��������n���ֽ�  
*  ����˵����buff: ���ͻ�����                                                 
//		   btr:���ͳ��� 
*  �������أ�1:�ɹ�;0:ʧ��     
*  �޸�ʱ�䣺
*  ��    ע�����ݴ�ѧ������
*************************************************************************/
uint32 sdhc_send_block (const uint8 *buff,uint32 btr)
{
    uint32  bytes, i;
    uint32	*ptr = (uint32*)buff;

    //��ȡ����ʱ��ÿ�ζ�ȡ4���ֽ�
    bytes = btr;
    while (bytes)
    {
        i = bytes > 512 ? 512 : bytes;
        bytes -= i;
        for (i = (i + 3) >> 2; i != 0; i--)
        {
            if (SDHC_IRQSTAT & (SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK))
            {
            	SDHC_IRQSTAT |= SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK | SDHC_IRQSTAT_BWR_MASK;
                return 0;
            }
            while (0 == (SDHC_PRSSTAT & SDHC_PRSSTAT_BWEN_MASK)){};

            SDHC_DATPORT = *ptr++;

        }
    }
	return 1;
}
/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�sdhc_ioctl
*  ����˵��������SDHCģ��
*  ����˵����cmd: ��������                                                 
//		   param_ptr:����ָ�� 
*  �������أ�ESDHC_OK �ɹ�ʱ����;��������ֵΪ����     
*  �޸�ʱ�䣺
*  ��    ע�����ݴ�ѧ������
*************************************************************************/

uint32 sdhc_ioctl(uint32 cmd,void *param_ptr)
{
    ESDHC_INFO_STRUCT_PTR   esdhc_info_ptr;
    ESDHC_DEVICE_STRUCT_PTR esdhc_device_ptr;
    ESDHC_INIT_STRUCT_CPTR  esdhc_init_ptr;
    ESDHC_COMMAND_STRUCT    command;
    uint8                   mem, io, mmc, ceata, mp, hc;
    uint32                  i, val;
    uint32                  result = ESDHC_OK;
    uint32             	    *param32_ptr = param_ptr;

    //��ȡ����SDHC��Ϣ�����ݽṹ���ָ��
    esdhc_info_ptr = (ESDHC_INFO_STRUCT_PTR)&SDHC_Info;
    
    if (NULL == esdhc_info_ptr)
    {
        return IO_DEVICE_DOES_NOT_EXIST;
    }
    
    esdhc_device_ptr = &SDHC_Device;
    if (NULL == esdhc_device_ptr)
    {
        return IO_ERROR_DEVICE_INVALID;
    }
    
    esdhc_init_ptr = esdhc_device_ptr->INIT;
    if (NULL == esdhc_init_ptr)
    {
        return IO_ERROR_DEVICE_INVALID;
    }    
    
    switch (cmd) 
    {
        case IO_IOCTL_ESDHC_INIT:        	
            result = hw_sdhc_init (esdhc_info_ptr, &K60_SDHC0_init);
            if (ESDHC_OK != result)
            {
                break;
            }
            
            mem = FALSE;
            io = FALSE;
            mmc = FALSE;
            ceata = FALSE;
            hc = FALSE;
            mp = FALSE;

            //CMD0,ʹ��SD���������ģʽ����λSD��
            command.COMMAND = ESDHC_CMD0;
            command.TYPE = ESDHC_TYPE_NORMAL;
            command.ARGUMENT = 0;
            command.READ = FALSE;
            command.BLOCKS = 0;
            if (SDHC_send_command (&command))
            {
                result = ESDHC_ERROR_INIT_FAILED;
                break;
            }
            
            for(i = 0;i < 2000000;i++)
            {
            }

            //CMD8
            command.COMMAND = ESDHC_CMD8;
            command.TYPE = ESDHC_TYPE_NORMAL;
            command.ARGUMENT = 0x000001AA;
            command.READ = FALSE;
            command.BLOCKS = 0;
            val = SDHC_send_command (&command);
            
            if (val == 0)
            {
                // SDHC��
            	if (command.RESPONSE[0] != command.ARGUMENT)
                {
                    result = ESDHC_ERROR_INIT_FAILED;
                    break;
                }
                hc = TRUE;
            }

            mp = TRUE;
            
            if (mp)
            {
                //CMD55������Ƿ�ΪMMC��
                command.COMMAND = ESDHC_CMD55;
                command.TYPE = ESDHC_TYPE_NORMAL;
                command.ARGUMENT = 0;
                command.READ = FALSE;
                command.BLOCKS = 0;
                val = SDHC_send_command (&command);
                if (val > 0)
                {
                    result = ESDHC_ERROR_INIT_FAILED;
                    break;
                }
                if (val < 0)
                {
                    // MMC �� CE-ATA
                    io = FALSE;
                    mem = FALSE;
                    hc = FALSE;
                    
                    //CMD1
                    command.COMMAND = ESDHC_CMD1;
                    command.TYPE = ESDHC_TYPE_NORMAL;
                    command.ARGUMENT = 0x40300000;
                    command.READ = FALSE;
                    command.BLOCKS = 0;
                    if (SDHC_send_command (&command))
                    {
                        result = ESDHC_ERROR_INIT_FAILED;
                        break;
                    }
                    if (0x20000000 == (command.RESPONSE[0] & 0x60000000))
                    {
                        hc = TRUE;
                    }
                    mmc = TRUE;

                    //CMD39
                    command.COMMAND = ESDHC_CMD39;
                    command.TYPE = ESDHC_TYPE_NORMAL;
                    command.ARGUMENT = 0x0C00;
                    command.READ = FALSE;
                    command.BLOCKS = 0;
                    if (SDHC_send_command (&command))
                    {
                        result = ESDHC_ERROR_INIT_FAILED;
                        break;
                    }
                    if (0xCE == (command.RESPONSE[0] >> 8) & 0xFF)
                    {
                        //CMD39
                        command.COMMAND = ESDHC_CMD39;
                        command.TYPE = ESDHC_TYPE_NORMAL;
                        command.ARGUMENT = 0x0D00;
                        command.READ = FALSE;
                        command.BLOCKS = 0;
                        if (SDHC_send_command (&command))
                        {
                            result = ESDHC_ERROR_INIT_FAILED;
                            break;
                        }
                        if (0xAA == (command.RESPONSE[0] >> 8) & 0xFF)
                        {
                            mmc = FALSE;
                            ceata = TRUE;
                        }
                    }
                }
                else
                {
                    //��ΪSD��ʱ
                    // ACMD41
                    command.COMMAND = ESDHC_ACMD41;
                    command.TYPE = ESDHC_TYPE_NORMAL;
                    command.ARGUMENT = 0;
                    command.READ = FALSE;
                    command.BLOCKS = 0;
                    if (SDHC_send_command (&command))
                    {
                        result = ESDHC_ERROR_INIT_FAILED;
                        break;
                    }
                    if (command.RESPONSE[0] & 0x300000)
                    {
                        val = 0;
                        do 
                        {
                            for(i = 0;i < 500000;i++)
                            {
                            }
                            val++;
                            
                            // CMD55 + ACMD41 - Send OCR 
                            command.COMMAND = ESDHC_CMD55;
                            command.TYPE = ESDHC_TYPE_NORMAL;
                            command.ARGUMENT = 0;
                            command.READ = FALSE;
                            command.BLOCKS = 0;
                            if (SDHC_send_command (&command))
                            {
                                result = ESDHC_ERROR_INIT_FAILED;
                                break;
                            }

                            command.COMMAND = ESDHC_ACMD41;
                            command.TYPE = ESDHC_TYPE_NORMAL;
                            if (hc)
                            {
                                command.ARGUMENT = 0x40300000;
                            }
                            else
                            {
                                command.ARGUMENT = 0x00300000;
                            }
                            command.READ = FALSE;
                            command.BLOCKS = 0;
                            if (SDHC_send_command (&command))
                            {
                                result = ESDHC_ERROR_INIT_FAILED;
                                break;
                            }
                        } while ((0 == (command.RESPONSE[0] & 0x80000000)) && (val < 10));
                        if (ESDHC_OK != result)
                        {
                            break;
                        }
                        if (val >= 10)
                        {
                            hc = FALSE;
                        }
                        else
                        {
                            mem = TRUE;
                            if (hc)
                            {
                                hc = FALSE;
                                if (command.RESPONSE[0] & 0x40000000)
                                {
                                    hc = TRUE;
                                }
                            }
                        }
                    }
                }
            }
            
            
            if (mmc)
            {
                esdhc_info_ptr->CARD = ESDHC_CARD_MMC;
            }
            if (ceata)
            {
                esdhc_info_ptr->CARD = ESDHC_CARD_CEATA;
            }
            if (io)
            {
                esdhc_info_ptr->CARD = ESDHC_CARD_SDIO;
            }
            if (mem)
            {
                esdhc_info_ptr->CARD = ESDHC_CARD_SD;
                if (hc)
                {
                    esdhc_info_ptr->CARD = ESDHC_CARD_SDHC;
                }
            }
            if (io && mem)
            {
                esdhc_info_ptr->CARD = ESDHC_CARD_SDCOMBO;
                if (hc)
                {
                    esdhc_info_ptr->CARD = ESDHC_CARD_SDHCCOMBO;
                }
            }

            //������Ÿ��üĴ���
            PORTE_PCR(0) = 0 & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    // ESDHC.D1  
            PORTE_PCR(1) = 0 & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    // ESDHC.D0  
            PORTE_PCR(2) = 0 & (PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK);                                          // ESDHC.CLK 
            PORTE_PCR(3) = 0 & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    // ESDHC.CMD 
            PORTE_PCR(4) = 0 & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    // ESDHC.D3  
            PORTE_PCR(5) = 0 & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    // ESDHC.D2  

            //����SDHCģ��Ĳ�����
            SDHC_set_baudrate (esdhc_init_ptr->CLOCK_SPEED, esdhc_init_ptr->BAUD_RATE);

            //���ø������Ź���ΪSDHC
            PORTE_PCR(0) = 0xFFFF & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    // ESDHC.D1  
            PORTE_PCR(1) = 0xFFFF & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    // ESDHC.D0  
            PORTE_PCR(2) = 0xFFFF & (PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK);                                          // ESDHC.CLK 
            PORTE_PCR(3) = 0xFFFF & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    // ESDHC.CMD 
            PORTE_PCR(4) = 0xFFFF & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    // ESDHC.D3  
            PORTE_PCR(5) = 0xFFFF & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    // ESDHC.D2  

            //ʹ��SDHCģ���ʱ��
            SIM_SCGC3 |= SIM_SCGC3_ESDHC_MASK;
            
            break;
        case IO_IOCTL_ESDHC_SEND_COMMAND:
            val = SDHC_send_command ((ESDHC_COMMAND_STRUCT_PTR)param32_ptr);
            if (val > 0)
            {
                result = ESDHC_ERROR_COMMAND_FAILED;
            }
            if (val < 0)
            {
                result = ESDHC_ERROR_COMMAND_TIMEOUT;
            }
            break;
        case IO_IOCTL_ESDHC_GET_BAUDRATE:
            if (NULL == param32_ptr) 
            {
                result = BRTOS_INVALID_PARAMETER;
            } 
            else 
            {
                //��ȡ��ǰ���õĲ�����
                val = ((SDHC_SYSCTL & SDHC_SYSCTL_SDCLKFS_MASK) >> SDHC_SYSCTL_SDCLKFS_SHIFT) << 1;
                val *= ((SDHC_SYSCTL & SDHC_SYSCTL_DVS_MASK) >> SDHC_SYSCTL_DVS_SHIFT) + 1;
                *param32_ptr = (uint32)((esdhc_init_ptr->CLOCK_SPEED) / val);
            }
            break;
        case IO_IOCTL_ESDHC_SET_BAUDRATE:
            if (NULL == param32_ptr) 
            {
                result = BRTOS_INVALID_PARAMETER;
            } 
            else if (0 == (*param32_ptr)) 
            {
                result = BRTOS_INVALID_PARAMETER;
            } 
            else 
            {
                if (! SDHC_is_running())
                {
                	//������Ÿ��üĴ���
                    PORTE_PCR(0) = 0 & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    // ESDHC.D1  
                    PORTE_PCR(1) = 0 & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    // ESDHC.D0  
                    PORTE_PCR(2) = 0 & (PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK);                                          // ESDHC.CLK 
                    PORTE_PCR(3) = 0 & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    // ESDHC.CMD 
                    PORTE_PCR(4) = 0 & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    // ESDHC.D3  
                    PORTE_PCR(5) = 0 & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    // ESDHC.D2  

                    //����SDHCģ��Ĳ�����
                    SDHC_set_baudrate (esdhc_init_ptr->CLOCK_SPEED, *param32_ptr);

                    //���ø������Ź���ΪSDHC
                    PORTE_PCR(0) = 0xFFFF & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    // ESDHC.D1  
                    PORTE_PCR(1) = 0xFFFF & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    // ESDHC.D0  
                    PORTE_PCR(2) = 0xFFFF & (PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK);                                          // ESDHC.CLK 
                    PORTE_PCR(3) = 0xFFFF & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    // ESDHC.CMD 
                    PORTE_PCR(4) = 0xFFFF & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    // ESDHC.D3  
                    PORTE_PCR(5) = 0xFFFF & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    // ESDHC.D2  

                    //ʹ��SDHCģ���ʱ��
                    SIM_SCGC3 |= SIM_SCGC3_ESDHC_MASK;
                }
                else
                {
                    result = IO_ERROR_DEVICE_BUSY;
                }
            }
            break;
        case IO_IOCTL_ESDHC_GET_BLOCK_SIZE:
            if (NULL == param32_ptr) 
            {
                result = BRTOS_INVALID_PARAMETER;
            } 
            else 
            {
                //��ȡSDHCģ�����õĿ�Ĵ�С
                *param32_ptr = (SDHC_BLKATTR & SDHC_BLKATTR_BLKSIZE_MASK) >> SDHC_BLKATTR_BLKSIZE_SHIFT;
            }       
            break;
        case IO_IOCTL_ESDHC_SET_BLOCK_SIZE:
            if (NULL == param32_ptr) 
            {
                result = BRTOS_INVALID_PARAMETER;
            } 
            else 
            {
                //����SDHCģ�鴦��Ŀ�Ĵ�С
                if (! SDHC_is_running())
                {
                    if (*param32_ptr > 0x0FFF)
                    {
                        result = BRTOS_INVALID_PARAMETER;
                    }
                    else
                    {
                        SDHC_BLKATTR &= (~ SDHC_BLKATTR_BLKSIZE_MASK); 
                        SDHC_BLKATTR |= SDHC_BLKATTR_BLKSIZE(*param32_ptr);
                    }
                }
                else
                {
                    result = IO_ERROR_DEVICE_BUSY;
                }
            }       
            break;
        case IO_IOCTL_ESDHC_GET_BUS_WIDTH:
            if (NULL == param32_ptr) 
            {
                result = BRTOS_INVALID_PARAMETER;
            } 
            else 
            {
                //��ȡ��ǰ���õ�SDHCģ������߿��
                val = (SDHC_PROCTL & SDHC_PROCTL_DTW_MASK) >> SDHC_PROCTL_DTW_SHIFT;
                if (ESDHC_PROCTL_DTW_1BIT == val)
                {
                    *param32_ptr = ESDHC_BUS_WIDTH_1BIT;
                }
                else if (ESDHC_PROCTL_DTW_4BIT == val)
                {
                    *param32_ptr = ESDHC_BUS_WIDTH_4BIT;
                }
                else if (ESDHC_PROCTL_DTW_8BIT == val)
                {
                    *param32_ptr = ESDHC_BUS_WIDTH_8BIT;
                }
                else
                {
                    result = ESDHC_ERROR_INVALID_BUS_WIDTH; 
                }
            }       
            break;
        case IO_IOCTL_ESDHC_SET_BUS_WIDTH:
            if (NULL == param32_ptr) 
            {
                result = BRTOS_INVALID_PARAMETER;
            } 
            else 
            {
            	//����SDHCģ������߿��
                if (! SDHC_is_running())
                {
                    if (ESDHC_BUS_WIDTH_1BIT == *param32_ptr)
                    {
                        SDHC_PROCTL &= (~ SDHC_PROCTL_DTW_MASK);
                        SDHC_PROCTL |= SDHC_PROCTL_DTW(ESDHC_PROCTL_DTW_1BIT);
                    }
                    else if (ESDHC_BUS_WIDTH_4BIT == *param32_ptr)
                    {
                        SDHC_PROCTL &= (~ SDHC_PROCTL_DTW_MASK);
                        SDHC_PROCTL |= SDHC_PROCTL_DTW(ESDHC_PROCTL_DTW_4BIT);
                    }
                    else if (ESDHC_BUS_WIDTH_8BIT == *param32_ptr)
                    {
                        SDHC_PROCTL &= (~ SDHC_PROCTL_DTW_MASK);
                        SDHC_PROCTL |= SDHC_PROCTL_DTW(ESDHC_PROCTL_DTW_8BIT);
                    }
                    else
                    {
                        result = ESDHC_ERROR_INVALID_BUS_WIDTH; 
                    }
                }
                else
                {
                    result = IO_ERROR_DEVICE_BUSY;
                }
            }       
            break;
        case IO_IOCTL_ESDHC_GET_CARD:
            if (NULL == param32_ptr) 
            {
                result = BRTOS_INVALID_PARAMETER;
            } 
            else 
            {
                //�ȴ�80��ʱ��
                SDHC_SYSCTL |= SDHC_SYSCTL_INITA_MASK;
                while (SDHC_SYSCTL & SDHC_SYSCTL_INITA_MASK)
                    { };
                    
                //��ȡSD�����ص�״̬
                if (SDHC_IRQSTAT & SDHC_IRQSTAT_CRM_MASK)
                {
                    SDHC_IRQSTAT |= SDHC_IRQSTAT_CRM_MASK;
                    esdhc_info_ptr->CARD = ESDHC_CARD_NONE;
                }
                if (SDHC_PRSSTAT & SDHC_PRSSTAT_CINS_MASK)
                {
                    if (ESDHC_CARD_NONE == esdhc_info_ptr->CARD)
                    {
                        esdhc_info_ptr->CARD = ESDHC_CARD_UNKNOWN;
                    }
                }
                else
                {
                    esdhc_info_ptr->CARD = ESDHC_CARD_NONE;
                }
                *param32_ptr = esdhc_info_ptr->CARD;
            }
            break;
        case IO_IOCTL_DEVICE_IDENTIFY:
            param32_ptr[IO_IOCTL_ID_PHY_ELEMENT]  = IO_DEV_TYPE_PHYS_ESDHC;
            param32_ptr[IO_IOCTL_ID_LOG_ELEMENT]  = IO_DEV_TYPE_LOGICAL_MFS;
            param32_ptr[IO_IOCTL_ID_ATTR_ELEMENT] = IO_ESDHC_ATTRIBS;
            break;
        case IO_IOCTL_FLUSH_OUTPUT:
            //�ȴ��������
            SDHC_status_wait (SDHC_IRQSTAT_TC_MASK);
            if (SDHC_IRQSTAT & (SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK))
            {
                SDHC_IRQSTAT |= SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK;
                result = ESDHC_ERROR_DATA_TRANSFER;
            }
            SDHC_IRQSTAT |= SDHC_IRQSTAT_TC_MASK | SDHC_IRQSTAT_BRR_MASK | SDHC_IRQSTAT_BWR_MASK;
            break;
        default:
            result = IO_ERROR_INVALID_IOCTL_CMD;
            break;
    }
    return result;
}

/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�SDHC_set_baudrate 
*  ����˵��������SDHCģ���ʱ��
*  ����˵����clock:ϵͳʱ��                                               
//         baudrate��������
*  �������أ�     
*  �޸�ʱ�䣺
*  ��    ע�����ݴ�ѧ������
*************************************************************************/
static void SDHC_set_baudrate(uint32 clock,uint32 baudrate)
{
	uint32 i, pres, div, min, minpres = 0x80, mindiv = 0x0F;
	int32  val;

    //�ҵ�����ķ�Ƶ����
    min = (uint32)-1;
    for (pres = 2; pres <= 256; pres <<= 1) 
    {
        for (div = 1; div <= 16; div++) 
        {
            val = pres * div * baudrate - clock;
            if (val >= 0)
            {
                if (min > val) 
                {
                    min = val;
                    minpres = pres;
                    mindiv = div;
                }
            }
        }
    }

    //��ֹSDHCģ��ʱ��
    SDHC_SYSCTL &= (~ SDHC_SYSCTL_SDCLKEN_MASK);

    //�޸ķ�Ƶ����
    div = SDHC_SYSCTL & (~ (SDHC_SYSCTL_DTOCV_MASK | SDHC_SYSCTL_SDCLKFS_MASK | SDHC_SYSCTL_DVS_MASK));
    SDHC_SYSCTL = div | (SDHC_SYSCTL_DTOCV(0x0E) | SDHC_SYSCTL_SDCLKFS(minpres >> 1) | SDHC_SYSCTL_DVS(mindiv - 1));

    //����ʱ���ȶ�
    while (0 == (SDHC_PRSSTAT & SDHC_PRSSTAT_SDSTB_MASK))
    {
		for(i = 0;i < 200000;i++)
		{
		}
    };

    //ʹ��SDHCģ��ʱ��
    SDHC_SYSCTL |= SDHC_SYSCTL_SDCLKEN_MASK;
    SDHC_IRQSTAT |= SDHC_IRQSTAT_DTOE_MASK;
}
/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�SDHC_is_running
*  ����˵�������SDHCģ���Ƿ�æ
*  ����˵����
*  �������أ� 1����æ;0��������    
*  �޸�ʱ�䣺
*  ��    ע�����ݴ�ѧ������
*************************************************************************/
static uint8 SDHC_is_running(void)
{
    return (0 != (SDHC_PRSSTAT & (SDHC_PRSSTAT_RTA_MASK | SDHC_PRSSTAT_WTA_MASK | SDHC_PRSSTAT_DLA_MASK | SDHC_PRSSTAT_CDIHB_MASK | SDHC_PRSSTAT_CIHB_MASK)));
}   
/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�SDHC_status_wait 
*  ����˵�����ȴ��жϱ�־λ��λ 
*  ����˵����mask�������ϵı�־λ���� 
*  �������أ����ش������     
*  �޸�ʱ�䣺
*  ��    ע�����ݴ�ѧ������
*************************************************************************/
static uint32 SDHC_status_wait(uint32 mask)
{
    uint32	result;
    do
    {
        result = SDHC_IRQSTAT & mask;
    } 
    while (0 == result);
    return result;
}
/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�SDHC_send_command  
*  ����˵������������ 
*  ����˵����command������ṹ��ָ��  
*  �������أ�0���ɹ���1������-1����ʱ��     
*  �޸�ʱ�䣺
*  ��    ע�����ݴ�ѧ������
*************************************************************************/

static uint32 SDHC_send_command (ESDHC_COMMAND_STRUCT_PTR command)
{
    uint32	xfertyp;//Transfer Type Register (SDHC_XFERTYP)
    
    //������������
    xfertyp = ESDHC_COMMAND_XFERTYP[command->COMMAND & 0x3F];
    if ((0 == xfertyp) && (0 != command->COMMAND))
    {
        return 1;
    }

    //���Ƴ����
    SDHC_IRQSTAT |= SDHC_IRQSTAT_CRM_MASK;

    //�ȴ�CMDͨ������
    while (SDHC_PRSSTAT & SDHC_PRSSTAT_CIHB_MASK){};

    //���ô������ͼĴ�������
    //д��������Ĵ���(Command Argument Register��SDHC_CMDARG)
    SDHC_CMDARG = command->ARGUMENT;
    //���XFERTYP�Ĵ���������������
    xfertyp &= (~ SDHC_XFERTYP_CMDTYP_MASK);
    //����XFERTYP�Ĵ���������������
    xfertyp |= SDHC_XFERTYP_CMDTYP(command->TYPE);
    //���Ϊ�ָ�����
    if (ESDHC_TYPE_RESUME == command->TYPE)
    {
    	//�����������Ϊ�ָ�CMD52д����ѡ���������ݴ���ѡ��λ��
        xfertyp |= SDHC_XFERTYP_DPSEL_MASK;
    }
    //������л�æ����
    if (ESDHC_TYPE_SWITCH_BUSY == command->TYPE)
    {
    	
        if ((xfertyp & SDHC_XFERTYP_RSPTYP_MASK) == SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48))
        {
            xfertyp &= (~ SDHC_XFERTYP_RSPTYP_MASK);
            xfertyp |= SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY);
        }
        else
        {
            xfertyp &= (~ SDHC_XFERTYP_RSPTYP_MASK);
            xfertyp |= SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48);
        }
    }
    //�������
    SDHC_BLKATTR &= (~ SDHC_BLKATTR_BLKCNT_MASK);
    //�����ж�
    if (0 != command->BLOCKS)
    {
    	//�鲻Ϊ0
        if ((xfertyp & SDHC_XFERTYP_RSPTYP_MASK) != SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY))
        {
            xfertyp |= SDHC_XFERTYP_DPSEL_MASK;
        }
        if (command->READ)
        {
            xfertyp |= SDHC_XFERTYP_DTDSEL_MASK;    
        }
        if (command->BLOCKS > 1)
        {
            xfertyp |= SDHC_XFERTYP_MSBSEL_MASK;    
        }
        if ((uint32)-1 != command->BLOCKS)
        {
        	SDHC_BLKATTR |= SDHC_BLKATTR_BLKCNT(command->BLOCKS);
            xfertyp |= SDHC_XFERTYP_BCEN_MASK;
        }
    }

    //ִ������
    SDHC_DSADDR = 0;
    SDHC_XFERTYP = xfertyp;
    
    //�ȴ�״̬�Ĵ�����λ
    if (SDHC_status_wait (SDHC_IRQSTAT_CIE_MASK | SDHC_IRQSTAT_CEBE_MASK | SDHC_IRQSTAT_CCE_MASK | SDHC_IRQSTAT_CC_MASK) != SDHC_IRQSTAT_CC_MASK)
    {
    	SDHC_IRQSTAT |= SDHC_IRQSTAT_CTOE_MASK | SDHC_IRQSTAT_CIE_MASK | SDHC_IRQSTAT_CEBE_MASK | SDHC_IRQSTAT_CCE_MASK | SDHC_IRQSTAT_CC_MASK;
        return 1;
    }

    //��⿨�Ƿ��Ƴ�
    if (SDHC_IRQSTAT & SDHC_IRQSTAT_CRM_MASK)
    {
    	SDHC_IRQSTAT |= SDHC_IRQSTAT_CTOE_MASK | SDHC_IRQSTAT_CC_MASK;
        return 1;
    }

    //��������Ƿ�ʱ
    if (SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK)
    {
    	SDHC_IRQSTAT |= SDHC_IRQSTAT_CTOE_MASK | SDHC_IRQSTAT_CC_MASK;
        return -1;
    }
    if ((xfertyp & SDHC_XFERTYP_RSPTYP_MASK) != SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_NO))
    {
        command->RESPONSE[0] = SDHC_CMDRSP(0);
        if ((xfertyp & SDHC_XFERTYP_RSPTYP_MASK) == SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_136))
        {
            command->RESPONSE[1] = SDHC_CMDRSP(1);
            command->RESPONSE[2] = SDHC_CMDRSP(2);
            command->RESPONSE[3] = SDHC_CMDRSP(3);
        }
    }
    
    SDHC_IRQSTAT |= SDHC_IRQSTAT_CC_MASK;

    return 0;
}

