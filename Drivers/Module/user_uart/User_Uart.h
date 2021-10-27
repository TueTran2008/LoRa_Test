
#ifndef __USER_UART__
#define __USER_UART__

#include "./cfg/User_Uart_Cfg.h"
#include "Std_Types.h"
#include "User_Uart_Types.h"
#include "User_Uart_Ipw.h"

extern void User_Uart_Init(User_Uart_ConfigType *UartConfig);
extern Std_ReturnType User_Uart_AsyncTransmit(uint8_t u8ChannelId, uint8_t *DataPtr, uint16_t DataLen);
extern Std_ReturnType User_Uart_Received (uint8_t u8ChannelId, uint8_t* DataPtr, uint16_t DataLen,int16_t *remain);

#endif /* __USER_UART__ */
