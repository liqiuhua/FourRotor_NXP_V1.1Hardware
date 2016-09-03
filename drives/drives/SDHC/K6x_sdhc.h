/***********************************************************
 * 文件名       ：K6x_sdhc.h
 * 说明         ：SD功能函数
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://bbs.landzo.com
 *库版本        ：V1.0
 *时间          ：15.5.26
************************************************************/
#ifndef __K6X_SDHC_H__
#define __K6X_SDHC_H__

#include "common.h"

#define BSP_SYSTEM_CLOCK	96000000
#define SOCKWP				0
#define MCU_ENDIAN			LITTLE_ENDIAN

enum
{
 FILE_FOUND_DUMMY,
 FILE_NOT_FOUND_DUMMY,
 FILE_CREATE_OK_DUMMY,
 NO_FILE_ENTRY_AVAILABLE_DUMMY,
 NO_FAT_ENTRY_AVAIlABLE_DUMMY,
 ERROR_IDLE_DUMMY,
 FILE_DELETED_DUMMY,
 SD_FAT_OK,
 SD_FAT_ERROR,
 SD_BUSY,
 SD_INACTIVE,
 SD_CARD_FORMATTING,
 SD_CARD_FORMATTED,
 SD_FILE_RENAMING,
 SD_FILE_RENAMED,
 SD_FILE_WRITING,   
 SD_FILE_DELETING, 
 SD_FILE_DELETED, 
 SD_DELETE_FILE_DENIED, 
 SD_FILE_LISTING,
 SD_FILE_READING, 
 SD_FILE_READ,
 SD_DIR_CREATING,
 SD_DIR_CHANGING,
 SD_FILE_COPYING,
 SD_FILE_COPIED,
 SD_COPY_FILE_FAILURE,
 SD_FILE_NOT_FOUND,
 SD_FILE_FOUND,
 SD_CREATE_FILE_FAILURE,
 SD_CREATE_FILE_OK,
 SD_CREATE_DIR_OK,
 SD_CREATE_DIR_FAILURE, 
 SD_OPEN_DIR_OK,
 SD_OPEN_DIR_FAILURE,  
 SD_FILE_WRITE_FAILURE,
 SD_FILE_WRITTEN,
 VERBOSE_ON,
 VERBOSE_OFF
};

// Error code returned by I/O functions
#define IO_ERROR        (-1)

#define _IOC_NRBITS     8
#define _IOC_TYPEBITS   8


#ifndef _IOC_SIZEBITS
# define _IOC_SIZEBITS  14
#endif

#ifndef _IOC_DIRBITS
# define _IOC_DIRBITS   2
#endif

#define _IOC_NRMASK     ((1 << _IOC_NRBITS)-1)
#define _IOC_TYPEMASK   ((1 << _IOC_TYPEBITS)-1)
#define _IOC_SIZEMASK   ((1 << _IOC_SIZEBITS)-1)
#define _IOC_DIRMASK    ((1 << _IOC_DIRBITS)-1)

#define _IOC_NRSHIFT    0
#define _IOC_TYPESHIFT  (_IOC_NRSHIFT+_IOC_NRBITS)
#define _IOC_SIZESHIFT  (_IOC_TYPESHIFT+_IOC_TYPEBITS)
#define _IOC_DIRSHIFT   (_IOC_SIZESHIFT+_IOC_SIZEBITS)


#ifndef _IOC_NONE
# define _IOC_NONE      0U
#endif

#ifndef _IOC_WRITE
# define _IOC_WRITE     1U
#endif

#ifndef _IOC_READ
# define _IOC_READ      2U
#endif

#define _IOC(dir,type,nr,size) \
        (((dir)  << _IOC_DIRSHIFT) | \
         ((type) << _IOC_TYPESHIFT) | \
         ((nr)   << _IOC_NRSHIFT) | \
         ((size) << _IOC_SIZESHIFT))

#define _IO(type,nr)            			 _IOC(_IOC_NONE,(type),(nr),0)
#define IO_TYPE_ESDHC              			 0x13

#define IO_IOCTL_ESDHC_INIT                  _IO(IO_TYPE_ESDHC,0x01)
#define IO_IOCTL_ESDHC_SEND_COMMAND          _IO(IO_TYPE_ESDHC,0x02)
#define IO_IOCTL_ESDHC_GET_CARD              _IO(IO_TYPE_ESDHC,0x03)
#define IO_IOCTL_ESDHC_GET_BAUDRATE          _IO(IO_TYPE_ESDHC,0x04)
#define IO_IOCTL_ESDHC_SET_BAUDRATE          _IO(IO_TYPE_ESDHC,0x05)
#define IO_IOCTL_ESDHC_GET_BUS_WIDTH         _IO(IO_TYPE_ESDHC,0x06)
#define IO_IOCTL_ESDHC_SET_BUS_WIDTH         _IO(IO_TYPE_ESDHC,0x07)
#define IO_IOCTL_ESDHC_GET_BLOCK_SIZE        _IO(IO_TYPE_ESDHC,0x08)
#define IO_IOCTL_ESDHC_SET_BLOCK_SIZE        _IO(IO_TYPE_ESDHC,0x09)

// Element defines for ID array
#define IO_IOCTL_ID_PHY_ELEMENT          	 (0)
#define IO_IOCTL_ID_LOG_ELEMENT          	 (1)
#define IO_IOCTL_ID_ATTR_ELEMENT         	 (2)

// Query a device to find out its properties
#define IO_IOCTL_DEVICE_IDENTIFY    		_IO(0x00,0x09)
#define IO_IOCTL_FLUSH_OUTPUT       		_IO(0x00,0x03)


#define IO_DEV_ATTR_ERASE                    (0x0001)
#define IO_DEV_ATTR_INTERRUPT                (0x0002)
#define IO_DEV_ATTR_POLL                     (0x0004)
#define IO_DEV_ATTR_READ                     (0x0008)
#define IO_DEV_ATTR_REMOVE                   (0x0010)
#define IO_DEV_ATTR_SEEK                     (0x0020)
#define IO_DEV_ATTR_WRITE                    (0x0040)
#define IO_DEV_ATTR_SW_FLOW_CONTROL          (0x0080)
#define IO_DEV_ATTR_HW_FLOW_CONTROL          (0x0100)

#define IO_DEV_ATTR_BLOCK_MODE               (0x0200)

#define BRTOS_INVALID_PARAMETER    (0x0C)
#define IO_ERROR_BASE          	   (0x0A00)
#define IO_OK                      (0)
#define IO_DEVICE_EXISTS           (IO_ERROR_BASE|0x00)
#define IO_DEVICE_DOES_NOT_EXIST   (IO_ERROR_BASE|0x01)
#define IO_ERROR_READ              (IO_ERROR_BASE|0x02)
#define IO_ERROR_WRITE             (IO_ERROR_BASE|0x03)
#define IO_ERROR_SEEK              (IO_ERROR_BASE|0x04)
#define IO_ERROR_WRITE_PROTECTED   (IO_ERROR_BASE|0x05)
#define IO_ERROR_READ_ACCESS       (IO_ERROR_BASE|0x06)
#define IO_ERROR_WRITE_ACCESS      (IO_ERROR_BASE|0x07)
#define IO_ERROR_SEEK_ACCESS       (IO_ERROR_BASE|0x08)
#define IO_ERROR_INVALID_IOCTL_CMD (IO_ERROR_BASE|0x09)
#define IO_ERROR_DEVICE_BUSY       (IO_ERROR_BASE|0x0A)
#define IO_ERROR_DEVICE_INVALID    (IO_ERROR_BASE|0x0B)

#define IO_ERROR_TIMEOUT           (IO_ERROR_BASE|0x10)
#define IO_ERROR_INQUIRE           (IO_ERROR_BASE|0x11)

#define IO_DEV_TYPE_LOGICAL_MFS    (0x0004)
#define IO_DEV_TYPE_PHYS_ESDHC     (0x001C)
#define IO_ESDHC_ATTRIBS 		   (IO_DEV_ATTR_READ | IO_DEV_ATTR_REMOVE | IO_DEV_ATTR_SEEK | IO_DEV_ATTR_WRITE | IO_DEV_ATTR_BLOCK_MODE)


typedef struct esdhc_command_struct
{
    uint8  COMMAND;
    uint8  TYPE;
    uint32 ARGUMENT;
    uint8  READ;
    uint32 BLOCKS;
    uint32 RESPONSE[4];
    
} ESDHC_COMMAND_STRUCT, *ESDHC_COMMAND_STRUCT_PTR;


typedef struct esdhc_init_struct
{
    // The device number
    uint32 CHANNEL;

    // The communication baud rate
    uint32 BAUD_RATE;

    // The module input clock
    uint32 CLOCK_SPEED;
   
} ESDHC_INIT_STRUCT, *ESDHC_INIT_STRUCT_PTR;

extern const ESDHC_INIT_STRUCT K60_SDHC0_init;

typedef struct sdcard_init_struct
{  
   // Signal specification
   uint32             SIGNALS;

} SDCARD_INIT_STRUCT, *SDCARD_INIT_STRUCT_PTR;


typedef struct io_sdcard_struct
{
   // Init struct pointer
   SDCARD_INIT_STRUCT_PTR INIT;

   // The low level response timeout >= 250 ms   
   uint32				 SD_TIMEOUT;
   

   // The number of blocks for the device
   uint32                NUM_BLOCKS;

   // High capacity = block addressing
   uint8                 SDHC;

   // Specification 2 or later card = different CSD register
   uint8                 VERSION2;

   // Card address
   uint32                ADDRESS;

} SDCARD_STRUCT, *SDCARD_STRUCT_PTR;

typedef const ESDHC_INIT_STRUCT *ESDHC_INIT_STRUCT_CPTR;


typedef struct esdhc_info_struct
{
   // The actual card status
   uint32                CARD;

   // The intermediate buffer
   uint32                BUFFER[128];
   
} ESDHC_INFO_STRUCT, *ESDHC_INFO_STRUCT_PTR;



typedef struct esdhc_device_struct
{
   // The current init values for this device
   ESDHC_INIT_STRUCT_CPTR INIT;
   
   // The number of opened file descriptors
   uint32                 COUNT;

} ESDHC_DEVICE_STRUCT, *ESDHC_DEVICE_STRUCT_PTR;

#define IO_SDCARD_BLOCK_SIZE_POWER   (9)
#define IO_SDCARD_BLOCK_SIZE         (1 << IO_SDCARD_BLOCK_SIZE_POWER)

// ESDHC error codes
#define ESDHC_OK                             (0x00)
#define ESDHC_ERROR_INIT_FAILED              (0x01)
#define ESDHC_ERROR_COMMAND_FAILED           (0x02)
#define ESDHC_ERROR_COMMAND_TIMEOUT          (0x03)
#define ESDHC_ERROR_DATA_TRANSFER            (0x04)
#define ESDHC_ERROR_INVALID_BUS_WIDTH        (0x05)
#define ESDHC_ERROR_IO        				 (0x06)

// ESDHC bus widths
#define ESDHC_BUS_WIDTH_1BIT                 (0x00)
#define ESDHC_BUS_WIDTH_4BIT                 (0x01)
#define ESDHC_BUS_WIDTH_8BIT                 (0x02)

// ESDHC card types
#define ESDHC_CARD_NONE                      (0x00)
#define ESDHC_CARD_UNKNOWN                   (0x01)
#define ESDHC_CARD_SD                        (0x02)
#define ESDHC_CARD_SDHC                      (0x03)
#define ESDHC_CARD_SDIO                      (0x04)
#define ESDHC_CARD_SDCOMBO                   (0x05)
#define ESDHC_CARD_SDHCCOMBO                 (0x06)
#define ESDHC_CARD_MMC                       (0x07)
#define ESDHC_CARD_CEATA                     (0x08)

// ESDHC command types
#define ESDHC_TYPE_NORMAL                    (0x00)
#define ESDHC_TYPE_SUSPEND                   (0x01)
#define ESDHC_TYPE_RESUME                    (0x02)
#define ESDHC_TYPE_ABORT                     (0x03)
#define ESDHC_TYPE_SWITCH_BUSY               (0x04)

// ESDHC commands
#define ESDHC_CMD0                           (0)
#define ESDHC_CMD1                           (1)
#define ESDHC_CMD2                           (2)
#define ESDHC_CMD3                           (3)
#define ESDHC_CMD4                           (4)
#define ESDHC_CMD5                           (5)
#define ESDHC_CMD6                           (6)
#define ESDHC_CMD7                           (7)
#define ESDHC_CMD8                           (8)
#define ESDHC_CMD9                           (9)
#define ESDHC_CMD10                          (10)
#define ESDHC_CMD11                          (11)
#define ESDHC_CMD12                          (12)
#define ESDHC_CMD13                          (13)
#define ESDHC_CMD15                          (15)
#define ESDHC_CMD16                          (16)
#define ESDHC_CMD17                          (17)
#define ESDHC_CMD18                          (18)
#define ESDHC_CMD20                          (20)
#define ESDHC_CMD24                          (24)
#define ESDHC_CMD25                          (25)
#define ESDHC_CMD26                          (26)
#define ESDHC_CMD27                          (27)
#define ESDHC_CMD28                          (28)
#define ESDHC_CMD29                          (29)
#define ESDHC_CMD30                          (30)
#define ESDHC_CMD32                          (32)
#define ESDHC_CMD33                          (33)
#define ESDHC_CMD34                          (34)
#define ESDHC_CMD35                          (35)
#define ESDHC_CMD36                          (36)
#define ESDHC_CMD37                          (37)
#define ESDHC_CMD38                          (38)
#define ESDHC_CMD39                          (39)
#define ESDHC_CMD40                          (40)
#define ESDHC_CMD42                          (42)
#define ESDHC_CMD52                          (52)
#define ESDHC_CMD53                          (53)
#define ESDHC_CMD55                          (55)
#define ESDHC_CMD56                          (56)
#define ESDHC_CMD60                          (60)
#define ESDHC_CMD61                          (61)
#define ESDHC_ACMD6                          (0x40 + 6)
#define ESDHC_ACMD13                         (0x40 + 13)
#define ESDHC_ACMD22                         (0x40 + 22)
#define ESDHC_ACMD23                         (0x40 + 23)
#define ESDHC_ACMD41                         (0x40 + 41)
#define ESDHC_ACMD42                         (0x40 + 42)
#define ESDHC_ACMD51                         (0x40 + 51)

#define GPIO_PIN_MASK            0x1Fu
#define GPIO_PIN(x)              (((1)<<(x & GPIO_PIN_MASK)))

#define SDCARD_GPIO_DETECT          		0//(GPIOE_PDIR & GPIO_PDIR_PDI(GPIO_PIN(28)))
#define SDCARD_GPIO_PROTECT					0//(GPIOE_PDIR & GPIO_PDIR_PDI(GPIO_PIN(27)))

#define IO_ESDHC_ATTRIBS 					(IO_DEV_ATTR_READ | IO_DEV_ATTR_REMOVE | IO_DEV_ATTR_SEEK | IO_DEV_ATTR_WRITE | IO_DEV_ATTR_BLOCK_MODE)

#define ESDHC_XFERTYP_RSPTYP_NO              (0x00)
#define ESDHC_XFERTYP_RSPTYP_136             (0x01)
#define ESDHC_XFERTYP_RSPTYP_48              (0x02)
#define ESDHC_XFERTYP_RSPTYP_48BUSY          (0x03)

#define ESDHC_XFERTYP_CMDTYP_ABORT           (0x03)

#define ESDHC_PROCTL_EMODE_INVARIANT         (0x02)

#define ESDHC_PROCTL_DTW_1BIT                (0x00)
#define ESDHC_PROCTL_DTW_4BIT                (0x01)
#define ESDHC_PROCTL_DTW_8BIT                (0x10)


//3 函数声明

uint32 hw_sdhc_init(ESDHC_INFO_STRUCT_PTR  esdhc_info_ptr,\
		            ESDHC_INIT_STRUCT_CPTR esdhc_init_ptr);
uint32 hw_sdhc_receive_block (uint8 *buff,uint32 btr);
uint32 hw_sdhc_send_block (const uint8 *buff,uint32 btr);
uint32 hw_sdhc_ioctl(uint32 cmd,void *param_ptr);


extern SDCARD_STRUCT 		SDHC_Card;
extern SDCARD_INIT_STRUCT  SDHC_Init;
extern ESDHC_INFO_STRUCT	SDHC_Info;
extern ESDHC_DEVICE_STRUCT SDHC_Device;

#endif // SDHC_H_
