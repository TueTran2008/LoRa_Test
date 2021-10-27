#include <stdlib.h>
#include "../../cfg/User_Uart_Cfg.h"
#include "User_Uart_Types.h"
#include "User_Ip_Usart.h"

#define USER_IP_USART_MIN_RANGEBAUDRATE       50u
#define USER_IP_USART_MAX_PRESCINDEX          12u
#define USER_IP_USART_BRR_MAX                 0x0000FFFFu

#define USER_IP_USART_SIZE_RINGBUFF           USER_UART_MAX_BUFFER_SIZE * 3

static User_Ip_Usart_RingBuffer_t Buffer_Usart0Channel;
static User_Ip_Usart_RingBuffer_t Buffer_Usart1Channel;
static User_Ip_Usart_RingBuffer_t Buffer_Usart2Channel;
static User_Ip_Usart_RingBuffer_t Buffer_Usart3Channel;
static User_Ip_Usart_RingBuffer_t Buffer_Usart6Channel;

static User_Ip_Usart_Type *const User_Ip_Usart_BaseAddress[USER_IP_USART_INSTANCE_COUNT] = USER_IP_USART_BASE_PTRS;
static User_UsartIRQn_Type User_Usart_IRQn[USER_IP_USART_INSTANCE_COUNT] = USER_IP_USART_RXTX_IRQS;

static Std_ReturnType User_Ip_Usart_SetBaudrate(User_Ip_Usart_Type *HwChannel, uint32_t UsartSourceClock, uint32_t baudrate);
static void User_Ip_Usart_SetParityMode    (User_Ip_Usart_Type *HwChannel, User_Uart_ParityMode_t parityMode);
static void User_Ip_Usart_SetStopBitCount  (User_Ip_Usart_Type *HwChannel, User_Uart_StopBitCount_t stopBitCount);
static User_Ip_Usart_RingBuffer_t *User_Ip_Usart_GetBuffer(uint8_t u8ChannelId);

static inline uint8_t User_Ip_Usart_GetChar(User_Ip_Usart_Type * Usart_channel);
static void User_Ip_Usart_ReceiveBuffer(uint8_t u8ChannelId, uint8_t u8Data);

static inline void User_Ip_Usart_PutChar(User_Ip_Usart_Type * Usart_channel, uint8_t u8Data);
static inline void User_Ip_Usart_TransmitBuffer(uint8_t u8ChannelId);

static Std_ReturnType User_Ip_Usart_SetBaudrate(User_Ip_Usart_Type *HwChannel, uint32_t UsartSourceClock, uint32_t baudrate)
{
	/* Check Source Clock */
	uint32_t BaudrateDiv = 0;
	uint8_t  Value = 0;
	uint32_t  UartDiv_Mant = 0;
	uint32_t  UartDiv_Frac = 0;

	Value = (HwChannel->CR1 & USER_IP_USART_CR1_OVER8_MASK) >> USER_IP_USART_CR1_OVER8_SHIFT;
    Value = 2 - Value;

	BaudrateDiv = UsartSourceClock / (baudrate/100);
	BaudrateDiv = BaudrateDiv / (8 * Value);

    UartDiv_Mant  = BaudrateDiv /100;
	UartDiv_Frac  = BaudrateDiv - 100 * UartDiv_Mant;

	Value  = (UartDiv_Frac * 16)/100;

	if((UartDiv_Frac * 16)%100 >= 50)
	{
		Value = Value + 1;
	}
    UartDiv_Frac = Value;

	HwChannel->BRR = (UartDiv_Mant << USER_IP_USART_BRR_MANT_SHIFT) & USER_IP_USART_BRR_MANT_MASK;
	HwChannel->BRR |= (Value << USER_IP_USART_BRR_FRAC_SHIFT) & USER_IP_USART_BRR_FRAC_MASK;

	return E_OK;
}

static void User_Ip_Usart_SetParityMode(User_Ip_Usart_Type *HwChannel, User_Uart_ParityMode_t parityMode)
{
	uint8_t u8ParityMode = 0u;
	if(parityMode == USER_UART_PARITY_NONE)
	{
		HwChannel->CR1 &= ~USER_IP_USART_CR1_PCE_MASK;
	}
	else
	{
		u8ParityMode = parityMode - 1u;
		HwChannel->CR1 &= ~USER_IP_USART_CR1_PS_MASK;
		HwChannel->CR1 |= (u8ParityMode << USER_IP_USART_CR1_PS_SHIFT) & USER_IP_USART_CR1_PS_MASK;
		HwChannel->CR1 |= USER_IP_USART_CR1_PCE_MASK;
	}
}

static void User_Ip_Usart_SetStopBitCount(User_Ip_Usart_Type *HwChannel, User_Uart_StopBitCount_t stopBitCount)
{
	HwChannel->CR2 &= ~USER_IP_USART_CR2_STOP_MASK;
	HwChannel->CR2 |= (stopBitCount << USER_IP_USART_CR2_STOP_SHIFT) & USER_IP_USART_CR2_STOP_MASK;
}

void User_Ip_Usart_DeInit(User_Ip_Usart_Type *HwChannel,uint8_t channelID)
{
	User_Ip_Usart_RingBuffer_t *BufferChannel;
    BufferChannel = User_Ip_Usart_GetBuffer(channelID);

	HwChannel->CR1 = USER_IP_USART_RESET_VALUE;
	HwChannel->CR2 = USER_IP_USART_RESET_VALUE;
	HwChannel->CR3 = USER_IP_USART_RESET_VALUE;
	HwChannel->BRR = USER_IP_USART_RESET_VALUE;

	BufferChannel->TxHead = 0u;
	BufferChannel->TxTail = 0u;
	BufferChannel->Status = USART_BUFFER_UNLOCK;
}

void User_Ip_Usart_Init(User_Uart_HWConfigType *HwConfig)
{
	uint8_t u8ChannelId = HwConfig->u8HwChannelID;
	User_Ip_Usart_RingBuffer_t *RingBuff;
	User_Ip_Usart_Type *Usart = User_Ip_Usart_BaseAddress[u8ChannelId];

	User_Ip_Usart_DeInit(Usart,u8ChannelId);
	Usart->CR1 &= ~USER_IP_USART_CR1_TE_MASK;
	Usart->CR1 &= ~USER_IP_USART_CR1_RE_MASK;

    RingBuff = User_Ip_Usart_GetBuffer(u8ChannelId);
    RingBuff->RingRxBufferChannel = (uint8_t *)calloc(USER_IP_USART_SIZE_RINGBUFF,sizeof(uint8_t));
    RingBuff->RingTxBufferChannel = (uint8_t *)calloc(USER_IP_USART_SIZE_RINGBUFF,sizeof(uint8_t));

	User_Ip_Usart_SetBaudrate (
								   Usart,
								   HwConfig->u32SourceClock,
								   HwConfig->u32Baudrate
	                           );

	User_Ip_Usart_SetParityMode (
									Usart,
									HwConfig->ParityMode
			                     );

	User_Ip_Usart_SetStopBitCount (
									  Usart,
									  HwConfig->StopBitCount
			                       );


	Usart->CR3 |= (
						 USER_IP_USART_CR3_EIE_MASK
					);

	Usart->CR1 |= (
			             USER_IP_USART_CR1_RXNEIE_MASK |
						 USER_IP_USART_CR1_TE_MASK |
						 USER_IP_USART_CR1_RE_MASK |
						 USER_IP_USART_CR1_UE_MASK
					  );

	/* Enable Interrupt*/
	HAL_NVIC_EnableIRQ(User_Usart_IRQn[u8ChannelId]);
	HAL_NVIC_SetPriority(User_Usart_IRQn[u8ChannelId],USER_IP_USART_PRIORITY,0u);

}

static inline uint8_t User_Ip_Usart_GetChar(User_Ip_Usart_Type * Usart_channel)
{
	return (Usart_channel->DR);
}

static inline void User_Ip_Usart_PutChar(User_Ip_Usart_Type *Usart_channel, uint8_t u8Data)
{
	Usart_channel->DR = u8Data;
}

void User_Ip_Usart_StartAsyncTransmit(User_Ip_Usart_Type *Usart_channel)
{
	Usart_channel->CR1 |= USER_IP_USART_CR1_TXEIE_MASK;
}

static inline void User_Ip_Usart_StopAsyncTransmit(User_Ip_Usart_Type *Usart_channel)
{
	Usart_channel->CR1 &= ~USER_IP_USART_CR1_TXEIE_MASK;
}

static User_Ip_Usart_RingBuffer_t *User_Ip_Usart_GetBuffer(uint8_t u8ChannelId)
{
	User_Ip_Usart_RingBuffer_t *BufferChannel;

	if(u8ChannelId == USER_IP_USART_CHANNELID_0)
	{
		BufferChannel = &Buffer_Usart0Channel;
	}
	else if(u8ChannelId == USER_IP_USART_CHANNELID_1)
	{
		BufferChannel = &Buffer_Usart1Channel;
	}
	else if(u8ChannelId == USER_IP_USART_CHANNELID_2)
	{
		BufferChannel = &Buffer_Usart2Channel;
	}
	else if(u8ChannelId == USER_IP_USART_CHANNELID_3)
	{
		BufferChannel = &Buffer_Usart3Channel;
	}
	else if(u8ChannelId == USER_IP_USART_CHANNELID_6)
	{
		BufferChannel = &Buffer_Usart6Channel;
	}
	else
	{
		BufferChannel = NULL;
	}

	return BufferChannel;
}


Std_ReturnType User_Ip_Usart_WriteBuffer(uint8_t u8ChannelId, const uint8_t *u8DataPtr, int16_t u16DataLen)
{
    Std_ReturnType ret = E_NOT_OK;
    uint16_t u16CountLen = 0u;
    uint16_t u16AvailableLen = 0u;
    User_Ip_Usart_RingBuffer_t *BufferChannel;
    BufferChannel = User_Ip_Usart_GetBuffer(u8ChannelId);
    if(BufferChannel == NULL)
	{
		return E_NOT_OK;
	}

	if(u8DataPtr == NULL || u16DataLen == 0u)
	{
		return E_NOT_OK;
	}

	if(USART_BUFFER_UNLOCK == BufferChannel->Status)
	{
		if(BufferChannel->TxHead > BufferChannel->TxTail)
		{
			u16AvailableLen = BufferChannel->TxHead - BufferChannel->TxTail;
		}
		else
		{
			u16AvailableLen = (USER_IP_USART_SIZE_RINGBUFF - BufferChannel->TxTail) + BufferChannel->TxHead;

		}

		if(u16AvailableLen >= u16DataLen)
		{
			BufferChannel->Status = USART_BUFFER_LOCK;
			for(u16CountLen = 0u;u16CountLen < u16DataLen;u16CountLen ++)
			{
				BufferChannel->RingTxBufferChannel[BufferChannel->TxTail] = u8DataPtr[u16CountLen];
				BufferChannel->TxTail = (BufferChannel->TxTail + 1U) & (USER_IP_USART_SIZE_RINGBUFF - 1U);

			}
			BufferChannel->Status = USART_BUFFER_UNLOCK;
			User_Ip_Usart_StartAsyncTransmit(User_Ip_Usart_BaseAddress[u8ChannelId]);
			ret = E_OK;
		}
	}
    return ret;
}

Std_ReturnType User_Ip_Usart_DeleteRxData(uint8_t u8ChannelId, uint16_t dlSize)
{
	uint16_t u16count = 0u;
    User_Ip_Usart_RingBuffer_t *BufferChannel;

    BufferChannel = User_Ip_Usart_GetBuffer(u8ChannelId);

	if(BufferChannel->RxTail > BufferChannel->RxHead)
	{
		u16count = BufferChannel->RxTail - BufferChannel->RxHead;
	}
	else if(BufferChannel->RxTail < BufferChannel->RxHead)
	{
		u16count = (USER_IP_USART_SIZE_RINGBUFF - BufferChannel->RxHead) + BufferChannel->RxTail;
	}
	else
	{
		return E_NOT_OK;
	}

    if(u16count <= dlSize)
    {
    	BufferChannel->RxHead = BufferChannel->RxTail;
    }

    BufferChannel->RxHead = (BufferChannel->RxHead + dlSize ) & (USER_IP_USART_SIZE_RINGBUFF - 1U);
    return E_OK;
}

Std_ReturnType User_Ip_Usart_FindByte(uint8_t u8ChannelId, uint8_t ByteF,uint16_t *local)
{
	uint16_t u16count = 0u;
	uint16_t u16ProtectCount = 0U;
    User_Ip_Usart_RingBuffer_t *BufferChannel;

    BufferChannel = User_Ip_Usart_GetBuffer(u8ChannelId);
    if(BufferChannel == NULL)
	{
		return E_NOT_OK;
	}
	u16count = BufferChannel->RxHead;

	while(u16count != BufferChannel->RxTail)
	{
		if(++ u16count == USER_IP_USART_SIZE_RINGBUFF)
		{
			u16count = 0;
		}
		u16ProtectCount++;
		if(BufferChannel->RingRxBufferChannel[u16count] == ByteF)
		{
			*local = u16ProtectCount;
			return E_OK;
		}
	}

    return E_NOT_OK;
}

Std_ReturnType User_Ip_Usart_ReadByte(uint8_t u8ChannelId, uint8_t *u8Data)
{
    User_Ip_Usart_RingBuffer_t *BufferChannel;

    BufferChannel = User_Ip_Usart_GetBuffer(u8ChannelId);
    if(BufferChannel == NULL)
	{
		return E_NOT_OK;
	}

	if(BufferChannel->RxHead != BufferChannel->RxTail)
	{
		*u8Data = BufferChannel->RingRxBufferChannel[BufferChannel->RxHead];
		BufferChannel->RxHead = (BufferChannel->RxHead + 1U) & (USER_IP_USART_SIZE_RINGBUFF - 1U);
	}
	else
	{
		return E_NOT_OK;
	}


    return E_OK;
}

Std_ReturnType User_Ip_Usart_ReadBuffer(uint8_t u8ChannelId, uint8_t *data, uint16_t length, int16_t *remain)
{
	uint16_t u16count = 0u;
    User_Ip_Usart_RingBuffer_t *BufferChannel;

    BufferChannel = User_Ip_Usart_GetBuffer(u8ChannelId);
    if(BufferChannel == NULL)
	{
		return E_NOT_OK;
	}

	if(BufferChannel->RxHead == BufferChannel->RxTail)
	{
		return E_NOT_OK;
	}

	for(u16count = 0u;u16count < length;u16count++)
	{
		if(BufferChannel->RxHead != BufferChannel->RxTail)
		{
			data[u16count] = BufferChannel->RingRxBufferChannel[BufferChannel->RxHead];
			BufferChannel->RxHead = (BufferChannel->RxHead + 1U) & (USER_IP_USART_SIZE_RINGBUFF - 1U);
		}
		else
		{
			*remain = u16count - length;
			break;
		}
	}

	if(BufferChannel->RxTail > BufferChannel->RxHead)
	{
		*remain = BufferChannel->RxTail - BufferChannel->RxHead;
	}
	else if(BufferChannel->RxTail < BufferChannel->RxHead)
	{
		*remain = (USER_IP_USART_SIZE_RINGBUFF - BufferChannel->RxHead) + BufferChannel->RxTail;
	}

    return E_OK;
}

static inline void User_Ip_Usart_TransmitBuffer(uint8_t u8ChannelId)
{
	User_Ip_Usart_RingBuffer_t *BufferChannel;
	User_Ip_Usart_Type *HwChannel = User_Ip_Usart_BaseAddress[u8ChannelId];

    BufferChannel = User_Ip_Usart_GetBuffer(u8ChannelId);
    if(BufferChannel == NULL)
	{
		return;
	}

	if(BufferChannel->TxTail != BufferChannel->TxHead)
	{
		User_Ip_Usart_PutChar(HwChannel,BufferChannel->RingTxBufferChannel[BufferChannel->TxHead]);
		BufferChannel->TxHead = (BufferChannel->TxHead + 1U) & (USER_IP_USART_SIZE_RINGBUFF - 1U);
	}
	else
	{
		User_Ip_Usart_StopAsyncTransmit(HwChannel);
	}
}

static void User_Ip_Usart_ReceiveBuffer(uint8_t u8ChannelId, uint8_t u8Data)
{
    User_Ip_Usart_RingBuffer_t *BufferChannel;

    BufferChannel = User_Ip_Usart_GetBuffer(u8ChannelId);
    if(BufferChannel == NULL)
	{
		return;
	}

	BufferChannel->RingRxBufferChannel[BufferChannel->RxTail] = u8Data;
	BufferChannel->RxTail = (BufferChannel->RxTail + 1U) & (USER_IP_USART_SIZE_RINGBUFF - 1U);

	if(BufferChannel->RxTail == BufferChannel->RxHead)
	{
		if(BufferChannel->RxHead == 0U)
		{
			BufferChannel->RxTail = USER_IP_USART_SIZE_RINGBUFF - 1U;
		}
		else
		{
			BufferChannel->RxTail = (BufferChannel->RxTail - 1U) & (USER_IP_USART_SIZE_RINGBUFF - 1U);
		}
	}

}

static void User_Ip_Usart_IrqHandler(uint8_t u8ChannelId)
{
	uint8_t Byte_Receive = 0u;
	User_Ip_Usart_Type *HwChannel = User_Ip_Usart_BaseAddress[u8ChannelId];
	uint32_t status = HwChannel->SR;

	if((status & USER_IP_USART_SR_TXE_MASK) == USER_IP_USART_SR_TXE_MASK)
	{
		User_Ip_Usart_TransmitBuffer(u8ChannelId);
	}

	if((status & USER_IP_USART_SR_RXNE_MASK) == USER_IP_USART_SR_RXNE_MASK)
	{
		Byte_Receive = User_Ip_Usart_GetChar(HwChannel);
		User_Ip_Usart_ReceiveBuffer(u8ChannelId,Byte_Receive);
	}

	if((status & USER_IP_USART_SR_PE_MASK) == USER_IP_USART_SR_PE_MASK)
	{
		/*TODO*/
	}

	if((status & USER_IP_USART_SR_FE_MASK) == USER_IP_USART_SR_FE_MASK)
	{
		/*TODO*/
	}

	if((status & USER_IP_USART_SR_ORE_MASK) == USER_IP_USART_SR_ORE_MASK)
	{
		/*TODO*/
		User_Ip_Usart_GetChar(HwChannel);
	}

	if((status & USER_IP_USART_SR_NF_MASK) == USER_IP_USART_SR_NF_MASK)
	{
		/*TODO*/
	}
	status = HwChannel->SR;
}

void USART1_IRQHandler(void)
{
    User_Ip_Usart_IrqHandler(USER_IP_USART_CHANNELID_1);
}

void USART2_IRQHandler(void)
{
    User_Ip_Usart_IrqHandler(USER_IP_USART_CHANNELID_2);
}

void USART3_IRQHandler(void)
{
    User_Ip_Usart_IrqHandler(USER_IP_USART_CHANNELID_3);
}

void USART6_IRQHandler(void)
{
    User_Ip_Usart_IrqHandler(USER_IP_USART_CHANNELID_6);
}
