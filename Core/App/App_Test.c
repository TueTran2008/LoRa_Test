/*
 * App_Test.c
 *
 *  Created on: Oct 25, 2021
 *      Author: Admin
 */

#include "App_Test.h"
#include "Layer1.h"


bool EnableMaster=true;//Ö÷´ÓÑ¡Ôñ

#define RF_FREQUENCY                                434000000 // Hz
#define TX_OUTPUT_POWER                             22        // 22 dBm
#define LORA_BANDWIDTH                              1         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                       11         // [SF7..SF12]
#define LORA_CODINGRATE                             2         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         0         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false


uint8_t start_count = 0;
#define PING_MESSAGE			0x01
#define PONG_MESSAGE			0x02



#define SESSION_DEFAULT			0x00
#define SESSION_RESPONSE		0x01

typedef void(*Session_Ptr)(void);
void Session_Default();
void Session_Response();
void Session_Receive();
Session_Ptr Session_Current[2] = { Session_Default,Session_Response};
uint8_t SessionID = 0;



uint32_t rnd = 0;
uint16_t  crc_value;
uint16_t StateID = 0;


const uint8_t PingMsg[] = "PING";
const uint8_t PongMsg[] = "PONG";


uint32_t slotcount = 0;


 uint8_t RxBuffer[100];

uint16_t Buffersize = 0;




void Payload_Handle();
void LoRa_IRQHandle();



uint8_t enable_sendpong = 0;







#define ACK_MESSAGE_T			0x03





uint8_t TransmitID = 0;

//uint8_t* data[100];
/*!
 * Radio events function pointer
 */
static RadioEvents_t RadioEvents;
LoRa_Buffer_t DataBuffer;

#define RX_TIMEOUT_VALUE                            1000
#define BUFFER_SIZE                                 64 // Define the payload size here
uint16_t BufferSize = BUFFER_SIZE;
uint8_t TX_Buffer[128];
uint8_t RX_Buffer[128];

int8_t RssiValue = 0;
int8_t SnrValue = 0;

bool ChannelDetected = false;
uint8_t	enable_send 	=  0 ;

#define POS_USER_ID				5
#define POS_RANDOMKEY			4

#define		LORA_BUFFER_LENGHT			24
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


static Std_ReturnType DataEncode(uint8_t *srdata,uint8_t *desdata,uint16_t u16srclen,uint16_t *deslen,User_ID user_id);
static Std_ReturnType DataDecode(uint8_t *srdata,uint8_t *desdata,User_ID user_id);










void OnCaDDetectted(bool channelActivityDetected )
{
	//ChannelDetected = false;
	//HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
	if( channelActivityDetected )
	{
		enable_send = 0;
		HAL_RNG_GenerateRandomNumber(&hrng, &rnd);
	}
	else
	{
		enable_send = 1;
	}
}
void OnTxDone( void )
{
	SessionID = SESSION_DEFAULT;
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);
	Radio.Rx(5000);
}
void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
	memcpy(RxBuffer,payload,24);
	if(strncmp(RxBuffer,"START",5) == 0)
	{
		enable_send = 1;
		SessionID = SESSION_RESPONSE;
		TransmitID = ACK_MESSAGE_T;
		memset(RxBuffer,0,5);
	}
	else
	{
		SessionID = SESSION_DEFAULT;
		//TransmitID = ACK_MESSAGE_T;
		memset(RxBuffer,0,5);
	}
	Radio.Rx(1);
}




void OnRxTimeout( void )
{
	Radio.Standby();
	User_Uart_AsyncTransmit(USER_UART_CHANNEL_USART1,(uint8_t*)"RxE\n",5);

}
void OnRxError( void )
{
	User_Uart_AsyncTransmit(USER_UART_CHANNEL_USART1,(uint8_t*)"RxE\n",9);
}



void App_TestMain()
{
	RadioEvents_t RadioEvents = {
		.TxDone = OnTxDone,
		.RxDone = OnRxDone,
		.TxTimeout = NULL,
		.RxTimeout = OnRxTimeout,
		.RxError = OnRxError,
		.CadDone = OnCaDDetectted
	};

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
	User_Uart_Init(&User_Uart_ConfigPtr);
	Init_Timer(&Timer2_Cfg);

	Tim_AddEvent(TIMER2_HW, LoRa_IRQHandle);
	Tim_AddEvent(TIMER2_HW, Payload_Handle);
	SX126xSetCadParams(0x02, 24, 11, 0, 00);

	while(1)
	{
		Session_Current[SessionID]();
		//Radio.Send("Hello",5);
		//HAL_Delay(2000);
		//Radio.Rx(1000);
	}
}






void LoRa_IRQHandle()
{
	Radio.IrqProcess( );
}

void Session_Response()
{

	uint32_t time = HAL_GetTick();
	while(1)
	{
		if(SessionID != SESSION_RESPONSE)
		{
			break;
		}
		switch(TransmitID)
		{
			case ACK_MESSAGE_T:

				if(HAL_GetTick() - time > ((uint8_t)rnd + 300))
				{
					HAL_RNG_GenerateRandomNumber(&hrng, &rnd);
					time = HAL_GetTick();
					//Radio.StartCad();

				if(enable_send == 1)
				{
					enable_send = 0;
					Radio.Send("ACK1",4);
					HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
					TransmitID = 0x00;
				}
				}
				break;
			case  4:
				if(HAL_GetTick() - time > ((uint8_t)rnd + 300))
				{
					HAL_RNG_GenerateRandomNumber(&hrng, &rnd);
					time = HAL_GetTick();
					Radio.StartCad();
					HAL_Delay(50);
				}
				if(enable_send == 1)
				{
					enable_send = 0;
					Radio.Send("HelloFormNode1",14);
					HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_12);
					TransmitID = 0x00;
					SessionID = SESSION_DEFAULT;
				}
				break;
			default:
			break;
		}
	HAL_Delay(1);
	}
}


void Session_Default()
{
	Radio.Rx(1000);
	while(1)
	{
		if(SessionID != SESSION_DEFAULT)
		{
			break;
		}
		HAL_Delay(1);
	}
}
void Payload_Handle()
{

	//else
	//{
	//	SessionID = SESSION_DEFAULT;
		//TransmitID = ACK_MESSAGE_T;
		//memset(RxBuffer,0,5);
	//}
}



static Std_ReturnType DataEncode(uint8_t *srdata,uint8_t *desdata,uint16_t u16srclen,uint16_t *deslen,User_ID user_id)
{
	Std_ReturnType ret = E_NOT_OK;
	uint32_t rndkey = 0;
	uint16_t u16deslen = 0;
	uint32_t ChipUniqueID[3] = {0};
	//HAL_RNG_GenerateRandomNumber(&hrng, &rndkey);


	if(u16srclen > 0 && srdata !=NULL && desdata != NULL && u16srclen > LORA_BUFFER_LENGHT - 5 )
	{
		ret =  E_NOT_OK;
	}
	else
	{

		ChipUniqueID[0] = *(uint32_t *)(0x1FFF7A10); // high address
		ChipUniqueID[1] = *(uint32_t  *)(0x1FFF7A14); //
		ChipUniqueID[2] = *(uint32_t *)(0x1FFF7A18); //  Low address
		desdata[1] = (uint8_t)ChipUniqueID[0];
		desdata[2] = (uint8_t)ChipUniqueID[1];
		desdata[3] = (uint8_t)ChipUniqueID[2];
		desdata[POS_RANDOMKEY] = (uint8_t)rndkey;
		desdata[POS_USER_ID] = user_id ^ desdata[POS_RANDOMKEY];

		for(uint8_t ByteCount = 0; ByteCount < u16srclen; ByteCount++)
		{
			desdata[ByteCount+6] = srdata[ByteCount] ^ rndkey;
		}
		u16deslen = 6 + u16srclen;
		//u16deslen++;
		*deslen = u16deslen;
		desdata[0] = (uint8_t)rndkey ^ u16deslen;
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
			for(uint8_t ByteCount = 0; ByteCount < u16len - 6; ByteCount++)
			{
				desdata[ByteCount]  = (uint8_t)(srdata[ByteCount+6] ^ srdata[POS_RANDOMKEY]);
			}
			ret = E_OK;
		}
	}
	return ret;
}
