
#ifndef __USER_IP_USART__
#define __USER_IP_USART__

#include "Std_Types.h"
#include "User_Ip_Usart_Types.h"
#include "User_Ip_Usart_Register.h"


#define USER_IP_USART_CHANNELID_0      0U
#define USER_IP_USART_CHANNELID_1      1U
#define USER_IP_USART_CHANNELID_2      2U
#define USER_IP_USART_CHANNELID_3      3U
#define USER_IP_USART_CHANNELID_6      6U

#define USER_IP_USART_NUMBER_CHANNEL   1U

#define USER_IP_USART_PRIORITY         5U

extern void User_Ip_Usart_DeInit(User_Ip_Usart_Type *HwChannel,uint8_t channelID);
extern void User_Ip_Usart_Init(User_Uart_HWConfigType *HwConfig);

extern Std_ReturnType User_Ip_Usart_FindByte(uint8_t u8ChannelId, uint8_t ByteF,uint16_t *local);
extern Std_ReturnType User_Ip_Usart_ReadByte(uint8_t u8ChannelId, uint8_t *u8Data);
extern Std_ReturnType User_Ip_Usart_DeleteRxData(uint8_t u8ChannelId, uint16_t dlSize);
extern Std_ReturnType User_Ip_Usart_WriteBuffer(uint8_t u8ChannelId, const uint8_t *u8DataPtr, int16_t u16DataLen);
extern Std_ReturnType User_Ip_Usart_ReadBuffer(uint8_t u8ChannelId, uint8_t *data, uint16_t length, int16_t *remain);
#endif /* __USER_IP_USART__ */
