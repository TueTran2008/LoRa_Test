#include "User_Uart_Ipw.h"

void User_Uart_Ipw_Init(User_Uart_HWConfigType *HwConfig)
{
	if(HwConfig->u8HwChannelID < 3U)
	{
		//User_Ip_LPuart_Init(HwConfig);
	}
	else if(HwConfig->u8HwChannelID < 9U)
	{
		HwConfig->u8HwChannelID = HwConfig->u8HwChannelID - 3U;
		User_Ip_Usart_Init(HwConfig);
	}
	else
	{
		HwConfig->u8HwChannelID = HwConfig->u8HwChannelID - 10U;
		User_Ip_Uart_Init(HwConfig);
	}
}

Std_ReturnType User_Uart_Ipw_AsyncTransmit(uint8_t u8ChannelId, const uint8_t *u8DataPtr, int16_t u16DataLen)
{
	Std_ReturnType ret = E_NOT_OK;
	uint8_t channelID = u8ChannelId;

	if(channelID < 3U)
	{
		//ret = User_Ip_LPuart_WriteBuffer(channelID,u8DataPtr,u16DataLen);
	}
	else if(channelID < 9U)
	{
		channelID = channelID - 3u;
		ret = User_Ip_Usart_WriteBuffer(channelID,u8DataPtr,u16DataLen);
	}
	else
	{
		channelID = channelID - 10u;
		ret = User_Ip_Uart_WriteBuffer(channelID,u8DataPtr,u16DataLen);
	}
	return ret;
}

Std_ReturnType User_Uart_Ipw_ReceiveData(uint8_t u8ChannelId, uint8_t *data, uint16_t length, int16_t *remain)
{
	Std_ReturnType ret = E_NOT_OK;
	uint8_t channelID = u8ChannelId;

	if(channelID < 3U)
	{
		//ret = User_Ip_LPuart_ReadBuffer(channelID,data,length,remain);
	}
	else if(channelID < 9U)
	{
		channelID = channelID - 3u;
		ret = User_Ip_Usart_ReadBuffer(channelID,data,length,remain);
	}
	else
	{
		channelID = channelID - 10u;
		ret = User_Ip_Uart_ReadBuffer(channelID,data,length,remain);
	}

	return ret;
}

Std_ReturnType User_Ip_Ipw_FindByte(uint8_t u8ChannelId, uint8_t ByteF,uint16_t *local)
{
	Std_ReturnType ret = E_NOT_OK;
	uint8_t channelID = u8ChannelId;

	if(channelID < 3U)
	{
		//ret = User_Ip_LPuart_FindByte(channelID,ByteF,local);
	}
	else if(channelID < 9U)
	{
		channelID = channelID - 3u;
		ret = User_Ip_Usart_FindByte(channelID,ByteF,local);
	}
	else
	{
		channelID = channelID - 10u;
		ret = User_Ip_Uart_FindByte(channelID,ByteF,local);
	}

	return ret;
}

Std_ReturnType User_Ip_Ipw_ReadByte(uint8_t u8ChannelId, uint8_t *u8Data)
{
	Std_ReturnType ret = E_NOT_OK;
	uint8_t channelID = u8ChannelId;

	if(channelID < 3U)
	{
		//ret = User_Ip_LPuart_ReadByte(channelID,u8Data);
	}
	else if(channelID < 9U)
	{
		channelID = channelID - 3u;
		ret = User_Ip_Usart_ReadByte(channelID,u8Data);
	}
	else
	{
		channelID = channelID - 10u;
		ret = User_Ip_Uart_ReadByte(channelID,u8Data);
	}

	return ret;
}

Std_ReturnType User_Ip_Ipw_DeleteRxData(uint8_t u8ChannelId, uint16_t dlSize)
{
	Std_ReturnType ret = E_NOT_OK;
	uint8_t channelID = u8ChannelId;

	if(channelID < 3U)
	{
		//ret = User_Ip_LPuart_DeleteRxData(channelID,dlSize);
	}
	else if(channelID < 9U)
	{
		channelID = channelID - 3u;
		ret = User_Ip_Usart_DeleteRxData(channelID,dlSize);
	}
	else
	{
		channelID = channelID - 10u;
		ret = User_Ip_Uart_DeleteRxData(channelID,dlSize);
	}

	return ret;
}



