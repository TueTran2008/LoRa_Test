#ifndef __USER_IP_USART_TYPES__
#define __USER_IP_USART_TYPES__

#include "Compiler.h"

typedef void (* usart_isr)(void);

typedef enum __User_Ip_Usart_BufferStatus_t
{
    USART_BUFFER_UNLOCK  = 0x0U,
	USART_BUFFER_LOCK    = 0x1U,
	USART_BUFFER_FULL    = 0x2U
} User_Ip_Usart_BufferStatus_t;

typedef struct
{
    volatile uint8_t *RingTxBufferChannel;
    volatile uint8_t *RingRxBufferChannel;
    volatile uint16_t TxTail;
    volatile uint16_t TxHead;
    volatile uint16_t RxTail;
    volatile uint16_t RxHead;
    User_Ip_Usart_BufferStatus_t Status;
} User_Ip_Usart_RingBuffer_t;


#endif /* __USER_IP_USART_TYPES__ */
