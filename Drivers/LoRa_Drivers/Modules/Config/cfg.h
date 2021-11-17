/*
 * cfg.h
 *
 *  Created on: Nov 15, 2021
 *      Author: Admin
 */

#ifndef LORA_DRIVERS_MODULES_CONFIG_CFG_H_
#define LORA_DRIVERS_MODULES_CONFIG_CFG_H_

#include "Radio.h"
#include "Layer1.h"
#include "Layer2.h"
#include "Layer3.h"
#define BUFFER_LENGHT		20

#define KEEPNMESSAGES 20


typedef struct
{
	RadioModems_t 	Modem;
	int8_t		 	TxOutPutPower;
	uint32_t 		fdev;
	uint32_t 		bandwidth;
	uint32_t 		datarate;
	uint8_t 		coderate;
	uint16_t 		preambleLen;
	bool 			fixLen;
	bool 			crcOn;
	bool 			freqHopOn;
	uint8_t 		hopPeriod;
	bool 			iqInverted;
	uint32_t 		timeout;
}TxConfig_t;

typedef struct
{
	RadioModems_t 	modem;
	uint32_t 		bandwidth;
	uint32_t 		datarate;
	uint8_t 		coderate;
	uint32_t 		bandwidthAfc;
	uint16_t 		preambleLen;
	uint16_t 		symbTimeout;
	bool 			fixLen;
	uint8_t 		payloadLen;
	bool 			crcOn;
	bool 			freqHopOn;
	uint8_t 		hopPeriod;
	bool 			iqInverted;
	bool 			rxContinuous;
}RxConfig_T;



typedef struct
{
	uint8_t MaxNode;
	uint8_t	NodeNumber;
	uint8_t BroadCast;
};











#endif /* LORA_DRIVERS_MODULES_CONFIG_CFG_H_ */
