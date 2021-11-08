/*
 * App_Test.h
 *
 *  Created on: Oct 25, 2021
 *      Author: Admin
 */

#ifndef APP_APP_TEST_H_
#define APP_APP_TEST_H_

#include "main.h"
#include "radio.h"
#include "User_Uart.h"
#include <string.h>
#include "rng.h"
//#include "sx1276-board.h"








#include "user_timer.h"
#define		LORA_BUFFER_LENGHT			24

typedef enum
{
    LOWPOWER,
    RX,
    RX_TIMEOUT,
    RX_ERROR,
    TX,
    TX_TIMEOUT,
}States_t;


typedef struct
{
	uint8_t TxBuffer[LORA_BUFFER_LENGHT];
	uint8_t	RxBuffer[LORA_BUFFER_LENGHT];
	uint16_t TxLen;
	uint16_t RxLen;
}LoRa_Buffer_t;




void LoRa_IRQHandle();
void Message_Handle();
void Session_Cad();
void Session_Default();
void Session_Response();




extern void App_TestMain();

#endif /* APP_APP_TEST_H_ */
