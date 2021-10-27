
#ifndef __USER_UART_SLIP__
#define __USER_UART_SLIP__


#include "Std_Types.h"
#include "User_Uart_Ipw.h"
#include "../../cfg/User_Uart_Cfg.h"

#define USER_SLIP_END                0xC0U
#define USER_SLIP_ESC                0xDBU
#define USER_SLIP_ESC_END            0xDCU
#define USER_SLIP_ESC_ESC            0xDDU

#define USER_SLIP_END_FRAME          0x00U
#define USER_SLIP_ERROR_FRAME        0x01U
#define USER_SLIP_NOT_END_FRAME      0x02U

typedef struct
{
    uint8_t *DataSlipPtr;
    uint16_t DataLen;
}User_Slip_Buffer;

extern void User_Slip_Init(uint8_t channelID);
extern Std_ReturnType User_Slip_SendFrame(uint8_t u8ChannelId, uint8_t *u8DataPtr, uint16_t length);
extern Std_ReturnType User_Slip_ReceivedFrame (uint8_t u8ChannelId, uint8_t* DataPtr, uint16_t* DataLenPtr);

#endif /* __USER_SLIP__ */
