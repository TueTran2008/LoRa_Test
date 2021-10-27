
#ifndef __USER_IP_UART__
#define __USER_IP_UART__

#include "Std_Types.h"
#include "User_Ip_Uart_Types.h"
#include "User_Ip_Uart_Register.h"

#define USER_IP_UART_CHANNELID_4      4U
#define USER_IP_UART_CHANNELID_5      5U
#define USER_IP_UART_CHANNELID_7      7U
#define USER_IP_UART_CHANNELID_8      8U

#define USER_IP_UART_PRIORITY         1U

extern void User_Ip_Uart_DeInit(User_Ip_Uart_Type *HwChannel);
extern void User_Ip_Uart_Init(User_Uart_HWConfigType *HwConfig);

extern Std_ReturnType User_Ip_Uart_FindByte(uint8_t u8ChannelId, uint8_t ByteF,uint16_t *local);
extern Std_ReturnType User_Ip_Uart_ReadByte(uint8_t u8ChannelId, uint8_t *u8Data);
extern Std_ReturnType User_Ip_Uart_DeleteRxData(uint8_t u8ChannelId, uint16_t dlSize);
extern Std_ReturnType User_Ip_Uart_WriteBuffer(uint8_t u8ChannelId, const uint8_t *u8DataPtr, int16_t u16DataLen);
extern Std_ReturnType User_Ip_Uart_ReadBuffer(uint8_t u8ChannelId, uint8_t *data, uint16_t length, int16_t *remain);
#endif /* __USER_IP_UART__ */
