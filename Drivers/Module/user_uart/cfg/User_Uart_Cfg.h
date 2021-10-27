
#ifndef __USER_UART_CFG__
#define __USER_UART_CFG__

#include "Std_Types.h"
#include "../User_Uart_Types.h"
#include "User_Uart_Ipw.h"
#include "User_Uart_Types.h"

#include "stm32f4xx_hal.h"

/* Define channel Id */
#define USER_UART_CHANNEL_LPUART0            0U
#define USER_UART_CHANNEL_LPUART1            1U
#define USER_UART_CHANNEL_LPUART2            2U

#define USER_UART_CHANNEL_USART0             3U
#define USER_UART_CHANNEL_USART1             4U
#define USER_UART_CHANNEL_USART2             5U
#define USER_UART_CHANNEL_USART3             6U
#define USER_UART_CHANNEL_USART4             7U
#define USER_UART_CHANNEL_USART5             8U
#define USER_UART_CHANNEL_USART6             9U

#define USER_UART_CHANNEL_UART0             10U
#define USER_UART_CHANNEL_UART1             11U
#define USER_UART_CHANNEL_UART2             12U
#define USER_UART_CHANNEL_UART3             13U
#define USER_UART_CHANNEL_UART4             14U
#define USER_UART_CHANNEL_UART5             15U
#define USER_UART_CHANNEL_UART6             16U
#define USER_UART_CHANNEL_UART7             17U
#define USER_UART_CHANNEL_UART8             18U

/* Define number of hardware use */
#define USER_UART_NUMBEROFHARDWARE_USE       1U
/* Define the number of channel used */
#define USER_UART_NUMBEROFCHANNEL_USED       18U

/* Define value here as 2^n for use & operator to prevent wrap value */
#define USER_UART_MAX_BUFFER_SIZE           256
#define USER_UART_MAX_MESSAGE_LENGTH        100

/* Define BuadRate of channel used */
#define USER_UART_SOURCECLOCK_CHANNEL_APB2_USART    84000000u
#define USER_UART_BAUDRATE_CHANNEL_USART1           115200

extern User_Uart_ConfigType User_Uart_ConfigPtr;

#endif /* __USER_UART_CFG__ */