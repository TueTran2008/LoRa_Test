#include "User_Uart.h"
#include "./protocol/slip/User_Slip.h"

static User_Uart_ConfigType *UartConfigPtr = NULL;

void User_Uart_Init(User_Uart_ConfigType *UartConfig)
{
    uint8_t u8ChannelCount;
    User_Uart_HWConfigType  HWConfig;


    __HAL_RCC_USART1_CLK_ENABLE();
    //__HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_USART3_CLK_ENABLE();
    //__HAL_RCC_USART6_CLK_ENABLE();
    __HAL_RCC_UART4_CLK_ENABLE();
    __HAL_RCC_UART5_CLK_ENABLE();
    //__HAL_RCC_UART7_CLK_ENABLE();
    //__HAL_RCC_UART8_CLK_ENABLE();

    UartConfigPtr = UartConfig;

    for(u8ChannelCount = 0u; u8ChannelCount < UartConfigPtr->numberChannel; u8ChannelCount++)
    {
    	HWConfig.ParityMode = UartConfigPtr->ChannelConfig[u8ChannelCount].ParityMode;
    	HWConfig.StopBitCount = UartConfigPtr->ChannelConfig[u8ChannelCount].StopBitCount;
    	HWConfig.u32SourceClock = UartConfigPtr->ChannelConfig[u8ChannelCount].u32SourceClock;
    	HWConfig.u32Baudrate = UartConfigPtr->ChannelConfig[u8ChannelCount].u32Baudrate;
    	HWConfig.SlipStatus = UartConfigPtr->ChannelConfig[u8ChannelCount].SlipStatus;
    	HWConfig.u8HwChannelID = UartConfigPtr->ChannelConfig[u8ChannelCount].u8HwChannelID;
		if(HWConfig.SlipStatus == USER_UART_SLIP_ENABLE)
		{
        	User_Slip_Init(HWConfig.u8HwChannelID);
		}
        User_Uart_Ipw_Init(&HWConfig);
    }
}

Std_ReturnType User_Uart_AsyncTransmit(uint8_t u8ChannelId, uint8_t *DataPtr, uint16_t DataLen)
{
	uint8_t u8count;
	uint8_t ChannelID = 0xFFu;
	Std_ReturnType ret = E_NOT_OK;

	if(UartConfigPtr == NULL)
	{
		return E_NOT_OK;
	}

	for(u8count = 0;u8count < UartConfigPtr->numberChannel;u8count++)
	{
		if(UartConfigPtr->ChannelConfig[u8count].u8HwChannelID == u8ChannelId)
		{
			ChannelID = u8count;
			break;
		}
	}

	if(ChannelID == 0xFF)
	{
		return E_NOT_OK;
	}

	if(UartConfigPtr->ChannelConfig[ChannelID].SlipStatus == USER_UART_SLIP_ENABLE)
	{
		ret = User_Slip_SendFrame(u8ChannelId,DataPtr,DataLen);
	}
	else
	{
		ret = User_Uart_Ipw_AsyncTransmit(u8ChannelId,DataPtr,DataLen);
	}
    return ret;
}

Std_ReturnType User_Uart_Received(uint8_t u8ChannelId, uint8_t* DataPtr, uint16_t DataLen,int16_t *remain)
{
	uint8_t u8count = 0x00u;
	uint8_t ChannelID = 0xFFu;
	uint16_t DataLength = 0x00u;
	Std_ReturnType ret = E_NOT_OK;

	if(UartConfigPtr == NULL)
	{
		return E_NOT_OK;
	}

	for(u8count = 0;u8count < UartConfigPtr->numberChannel;u8count++)
	{
		if(UartConfigPtr->ChannelConfig[u8count].u8HwChannelID == u8ChannelId)
		{
			ChannelID = u8count;
			break;
		}
	}

	if(ChannelID == 0xFF)
	{
		return E_NOT_OK;
	}

	if(UartConfigPtr->ChannelConfig[ChannelID].SlipStatus == USER_UART_SLIP_ENABLE)
	{
		ret = User_Slip_ReceivedFrame(u8ChannelId,DataPtr,&DataLength);
		if(DataLength > DataLen || E_NOT_OK == ret)
		{
			*remain = -DataLen;
			return E_NOT_OK;
		}

		if(DataLength <= DataLen)
		{
			*remain = DataLength - DataLen;
		}
	}
	else
	{
		ret = User_Uart_Ipw_ReceiveData(u8ChannelId,DataPtr,DataLen,remain);
	}
	return ret;
}
