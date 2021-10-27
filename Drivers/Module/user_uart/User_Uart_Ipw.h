
#ifndef __USER_LPUART_IPW__
#define __USER_LPUART_IPW__

#include "./cfg/User_Uart_Cfg.h"
#include "./ip/usart/User_Ip_Usart.h"
#include "./ip/uart/User_Ip_Uart.h"

#define USER_UART_CHANNELID_0  USER_LPUART_CHANNELID_0
#define USER_UART_CHANNELID_1  USER_LPUART_CHANNELID_1
#define USER_UART_CHANNELID_2  USER_LPUART_CHANNELID_2

extern void User_Uart_Ipw_Init(User_Uart_HWConfigType *HwConfig);
extern Std_ReturnType User_Ip_Ipw_FindByte(uint8_t u8ChannelId, uint8_t ByteF,uint16_t *local);
extern Std_ReturnType User_Ip_Ipw_ReadByte(uint8_t u8ChannelId, uint8_t *u8Data);
extern Std_ReturnType User_Ip_Ipw_DeleteRxData(uint8_t u8ChannelId, uint16_t dlSize);
extern Std_ReturnType User_Uart_Ipw_AsyncTransmit(uint8_t u8ChannelId, const uint8_t *u8DataPtr, int16_t u16DataLen);
extern Std_ReturnType User_Uart_Ipw_ReceiveData(uint8_t u8ChannelId, uint8_t *data, uint16_t length, int16_t *remain);

#endif /* __USER_LPUART_IPW__ */

