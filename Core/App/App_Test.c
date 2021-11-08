/*
 * App_Test.c
 *
 *  Created on: Oct 25, 2021
 *      Author: Admin
 */

#include "App_Test.h"
#include <string.h>
#include "user_timer.h"

#include "sx126x.h"


bool EnableMaster=true;//Ö÷´ÓÑ¡Ôñ

#define RF_FREQUENCY                                434000000 // Hz
#define TX_OUTPUT_POWER                             22        // 22 dBm
#define LORA_BANDWIDTH                              1         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                      	11         // [SF7..SF12]
#define LORA_CODINGRATE                             2         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         0         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false

uint16_t  crc_value;
/*!
 * Radio events function pointer
 */
static RadioEvents_t RadioEvents;

#define RX_TIMEOUT_VALUE                            10000
#define BUFFER_SIZE                                 64 // Define the payload size here




/////////////////////////////////////////

#define SESSION_RESPONSE 			0x01
#define SESSION_DEFAULT				0x00
#define SESSION_CAD					0x02

//////////////////////////////////////////

typedef void (*Session_Ptr)(void);
Session_Ptr Session_Current[3] = {Session_Default,Session_Response,Session_Cad};



LoRa_Buffer_t DataBuffer;


const uint8_t MSG[] = "hello";

uint16_t BufferSize = BUFFER_SIZE;
uint8_t TX_Buffer[BUFFER_SIZE];
uint8_t RX_Buffer[BUFFER_SIZE];

int8_t RssiValue = 0;
int8_t SnrValue = 0;



uint8_t SessionID =  0;



uint8_t ReceiveMessageID = 0;
///////////////////////////////////////////
#define PING_MESSAGE_R			0x01
#define PONG_MESSAGE_R			0x02


///////////////////////////////////////////

uint8_t TransmitID = 0;

#define	PONG_MESSAGE_T			0x02
#define	PING_MESSAGE_T			0x01
#define	DONT_SEND_T				0x00
#define ACK_MESSAGE_T			0x03
///////////////////////////////////////////
#define POS_USER_ID				5
#define POS_RANDOMKEY			4

#define		LORA_BUFFER_LENGHT			24
//////////////////////////////////////////


#define FIX_BYTE_NUMBER				5
#define DATA_PACKET_BEGIN			6



typedef enum
{
	User_Gateway	=		0xFF,
	User_Node_0		= 		0,
	User_Node_1		= 		1,
	User_Node_2		= 		2,
	User_Node_3		= 		3,
	User_Node_4		= 		4,
	User_Node_5		= 		5,
	User_Node_6		= 		6,
	User_Node_7		= 		7,
	User_Node_8		= 		8,
	User_Node_9		= 		9,
	User_Node_10	= 		10,
	User_Node_11	= 		11,
	User_Node_12	= 		12,
	User_Node_13	= 		13,
	User_Node_14	= 		14,
	User_Node_15	= 		15
}User_ID;

#define DATAMESSAGE_LENGHT		1
typedef enum
{
	MESSAGE_PING	=		0x01
}DataMessage_t;


static Std_ReturnType DataEncode(uint8_t *srdata,uint8_t *desdata,uint16_t u16srclen,User_ID user_id)
{
	Std_ReturnType ret = E_NOT_OK;
	uint32_t rndkey = 0;
	uint16_t u16deslen = 0;
	uint32_t ChipUniqueID[3] = {0};
	if(u16srclen > 0 && srdata !=NULL && desdata != NULL && u16srclen < LORA_BUFFER_LENGHT - 5 )
	{
		ret =  E_NOT_OK;
	}
	else
	{
		HAL_RNG_GenerateRandomNumber(&hrng, &rndkey);
		ChipUniqueID[1] = *(uint32_t *)(0x1FFF7A10); // high address
		ChipUniqueID[2] = *(uint32_t  *)(0x1FFF7A14); //
		ChipUniqueID[3] = *(uint32_t *)(0x1FFF7A18); //  Low address
		//////////////////////////////////////////////////////////////
		desdata[0] = (uint8_t)ChipUniqueID[0];
		desdata[1] = (uint8_t)ChipUniqueID[1];
		desdata[2] = (uint8_t)ChipUniqueID[2];
		desdata[POS_RANDOMKEY] = (uint8_t)rndkey;
		desdata[POS_USER_ID] = user_id ^ rndkey;
		for(uint8_t ByteCount = 0; ByteCount < u16srclen; ByteCount++)
		{
			desdata[ByteCount+6] = srdata[ByteCount] ^ rndkey;
		}
		u16deslen = 4 + u16srclen;
		desdata[0] = (uint8_t)rndkey ^ u16deslen;
		u16deslen++;
		ret = E_OK;
	}
	return ret;
}
static Std_ReturnType DataDecode(uint8_t *srdata,uint8_t *desdata,User_ID user_id)
{
	Std_ReturnType ret = E_NOT_OK;
	uint8_t id = 0;
	uint16_t u16len = 0;
	id = srdata[POS_USER_ID]^srdata[POS_RANDOMKEY];
	u16len = srdata[0] ^srdata[POS_RANDOMKEY];

	if(u16len > 0 && srdata !=NULL && desdata != NULL && u16len > LORA_BUFFER_LENGHT - 5  )
	{
		return E_NOT_OK;
	}
	else
	{
		if( id == user_id)
		{
			for(uint8_t ByteCount = 0; ByteCount < u16len - FIX_BYTE_NUMBER ; ByteCount++)
			{
				desdata[ByteCount]  = (uint8_t)(srdata[ByteCount+DATA_PACKET_BEGIN] ^ srdata[POS_RANDOMKEY]);
			}
			ret = E_OK;
		}
	}
	return ret;
}
static Std_ReturnType PollingNode(User_ID user_nodeid)
{
	if ( E_OK == DataEncode(MESSAGE_PING,DataBuffer.TxBuffer,DATAMESSAGE_LENGHT,user_nodeid))
	{
		SessionID = SESSION_RESPONSE;
	}
}



void Session_Default()
{
	//uint64_t count = 0;
	while(1)
	{
		if(SessionID != SESSION_DEFAULT)
		{
			break;
		}





		Radio.Rx(0xFFFFFF);
		HAL_Delay(1);
	}
}
void Session_Response()
{
	while(1)
	{
		if(SessionID != SESSION_RESPONSE)
		{
			break;
		}
		switch(ReceiveMessageID)
		{
			case PING_MESSAGE_R:
				TransmitID = PONG_MESSAGE_T;
				break;
			case PONG_MESSAGE_R:
				TransmitID = ACK_MESSAGE_T;
				break;
			default:
				SessionID = SESSION_DEFAULT;
				break;
		}
		switch(TransmitID)
		{
			case PING_MESSAGE_T:
				//HAL_Delay(60);
				//User_Uart_AsyncTransmit(USER_UART_CHANNEL_USART1,(uint8_t*)"PING\n",6);
				TransmitID = DONT_SEND_T;
				Radio.Send(DataBuffer.TxBuffer,4);
				SessionID = SESSION_DEFAULT;
				//HAL_Delay(2000);
				break;
			case ACK_MESSAGE_T:
				HAL_Delay(100);
				User_Uart_AsyncTransmit(USER_UART_CHANNEL_USART1,(uint8_t*)"ACK\n",5);
				TransmitID = DONT_SEND_T;
				Radio.Send((uint8_t*)"ACK",4);
				SessionID = SESSION_DEFAULT;
				break;
			default:
				SessionID = SESSION_DEFAULT;
				break;
		}
		HAL_Delay(1);
	}
}

void OnTxDone( void )
{
	User_Uart_AsyncTransmit(USER_UART_CHANNEL_USART1, (uint8_t*)"TxDone\n", strlen("TxDone\n"));
	Radio.Rx( RX_TIMEOUT_VALUE );
}
void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
	uint16_t size1 = size;
	int16_t rssi1 = rssi;
	int8_t snr1 = snr;
	Radio.Standby();
	DataDecode(payload, DataBuffer.RxBuffer, User_Gateway);
}
void OnRxTimeout( void )
{
	//static uint16_t RxCount =  0;
	Radio.Rx( RX_TIMEOUT_VALUE );
}

void OnCaDDetectted(bool channelActivityDetected )
{
	if(channelActivityDetected)
	{
		Radio.Rx( RX_TIMEOUT_VALUE );
	}
}
void App_TestMain()
{
	uint32_t count =  0;
	User_Uart_Init(&User_Uart_ConfigPtr);
	RadioEvents_t RadioEvents = {
		.TxDone = OnTxDone,
		.RxDone = OnRxDone,
		.TxTimeout = NULL,
		.RxTimeout = OnRxTimeout,
		.RxError = NULL,
		.CadDone = OnCaDDetectted
	};
	// Config LoRa Module

	Radio.Init( &RadioEvents );
	Radio.SetChannel( RF_FREQUENCY );
	Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                     LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                     LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                     true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );
	Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                     LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                     LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                     0, true, 0, 0, LORA_IQ_INVERSION_ON, false );
	SessionID = SESSION_DEFAULT;
	Init_Timer(&Timer2_Cfg);
	Tim_AddEvent(TIMER2_HW,LoRa_IRQHandle);
	Tim_AddEvent(TIMER2_HW,Message_Handle);
	SX126xSetCadParams(0x02, 24, 11, LORA_CAD_ONLY, 00);
	while(1)
	{
		//Session_Current[SessionID]();
		if(count++ > 300)
		{
			count = 0;
			Radio.StartCad();

		}
		HAL_Delay(1);
	}
}
void LoRa_IRQHandle()
{
	Radio.IrqProcess();
}
void Message_Handle()
{
	if(strncmp(DataBuffer.RxBuffer,"PONG",DATAMESSAGE_LENGHT) == 0)
	{
		DataBuffer.RxBuffer[0] = 0;
		User_Uart_AsyncTransmit(USER_UART_CHANNEL_USART1,RX_Buffer,4);
		User_Uart_AsyncTransmit(USER_UART_CHANNEL_USART1,(uint8_t*)"\n",2);
		ReceiveMessageID = PONG_MESSAGE_R;
		SessionID = SESSION_RESPONSE;
	}
}



