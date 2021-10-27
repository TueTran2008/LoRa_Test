/*
 * App_Test.c
 *
 *  Created on: Oct 25, 2021
 *      Author: Admin
 */

#include "App_Test.h"


#define USE_BAND_433	//Ñ¡ÔñÒ»¸öÆµÂÊ
#define USE_MODEM_LORA	//Ñ¡ÔñloraÄ£Ê½

#if defined( USE_BAND_433 )
#endif



#define TX_OUTPUT_POWER                             20        // dBm

#if defined( USE_MODEM_LORA )

#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                       7         // [SF7..SF12]
#define LORA_CODINGRATE                             1         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         5         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false
#endif


#define RX_TIMEOUT_VALUE                            1000
#define BUFFER_SIZE                                 64 // Define the payload size here


#define USE_BAND_433
#define USE_MODEM_LORA
/////////////////////////////////////////////////////////////////////////////
#if defined( USE_BAND_433 )

#define RF_FREQUENCY                                434000000 // Hz
#endif
const uint8_t PingMsg[] = "PING";
const uint8_t PongMsg[] = "PONG";
uint16_t BufferSize = BUFFER_SIZE;
uint8_t Buffer[BUFFER_SIZE];
States_t State = LOWPOWER;
int8_t RssiValue = 0;
int8_t SnrValue = 0;


void App_TestMain()
{
	  Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
	                                   LORA_SPREADING_FACTOR, LORA_CODINGRATE,
	                                   LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
	                                   true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );

	  Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
	                                   LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
	                                   LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
	                                   0, true, 0, 0, LORA_IQ_INVERSION_ON, true );

	while(1)
	{
		/*Sx1276SetNSS(0);
		Sx1276SpiInOut(0x12);
		Sx1276SetNSS(1);
		HAL_Delay(500);*/
	}
}



