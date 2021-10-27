#ifndef __USER_IP_UART_TYPES__
#define __USER_IP_UART_TYPES__

#include "Compiler.h"

#define USER_IP_UART_SIZE_RINGBUFF       1024U

typedef void (* Uart_isr)(void);

typedef enum __User_Ip_Uart_BufferStatus_t
{
    UART_BUFFER_UNLOCK  = 0x0U,
	UART_BUFFER_LOCK    = 0x1U,
	UART_BUFFER_FULL    = 0x2U
} User_Ip_Uart_BufferStatus_t;

typedef struct
{
    volatile uint8_t *RingTxBufferChannel;
    volatile uint8_t *RingRxBufferChannel;
    volatile uint16_t TxTail;
    volatile uint16_t TxHead;
    volatile uint16_t RxTail;
    volatile uint16_t RxHead;
    User_Ip_Uart_BufferStatus_t Status;
} User_Ip_Uart_RingBuffer_t;

#endif /* __USER_IP_UART_TYPES__ */
