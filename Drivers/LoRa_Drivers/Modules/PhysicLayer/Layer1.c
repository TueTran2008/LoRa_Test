/*
 * Layer1.c
 *
 *  Created on: Nov 11, 2021
 *      Author: Admin
 */


#include "Layer1.h"



//uint8_t TxBuffer[100] = {0};

#define BUFFER_LENGHT 20
packet_struct packetbuffer[BUFFER_LENGHT];

packet_buffer packet = {
		.packet_head = 0,
		.packet_tail = 0,
		.packet_ptr = packetbuffer
};

/*
 *  @brief : Write Data to Packet Buffer
 *
 * 	@return: Return type status
 */

return_type L1_QueuePacket(packet_struct data)
{
	if(packet.packet_head  < packet.packet_tail)
	{
		if(packet.packet_head + 1  == packet.packet_tail)
		{
			return ret_buffer_full;
		}
	}
	else
	{
		packet.packet_ptr[packet.packet_head].ttl = data.ttl;
		packet.packet_ptr[packet.packet_head].receiver = data.receiver;
		packet.packet_ptr[packet.packet_head].sender = data.sender;
		packet.packet_ptr[packet.packet_head].nextnode = data.nextnode;
		packet.packet_ptr[packet.packet_head].lastnode = data.lastnode;
		packet.packet_ptr[packet.packet_head].id = data.id;
		packet.packet_ptr[packet.packet_head].type = data.type;
		packet.packet_ptr[packet.packet_head].payload = data.payload;
		packet.packet_head++;
		return ret_ok;
	}
}

return_type L1_SendPacket(packet_struct packet, uint8_t TxBuffer[])
{

	TxBuffer[0] = NETID;
	TxBuffer[1] = packet.ttl;
	TxBuffer[2] = packet.receiver;
	TxBuffer[3] = packet.sender;
	TxBuffer[4] = packet.lastnode;
	TxBuffer[5] = packet.nextnode;
	TxBuffer[6] = (uint8_t)(packet.id >> 24);
	TxBuffer[7] = (uint8_t)(packet.id >> 16);
	TxBuffer[8] = (uint8_t)(packet.id >> 8);
	TxBuffer[9] = (uint8_t)(packet.id);
	TxBuffer[10] = packet.type;
	if(packet.type == payload_msg)
	{
		payload_message_struct *payload_message;
		payload_message = (payload_message_struct*)packet.payload;
		TxBuffer[11] = payload_message->message_size;
		//uint8_t message_temp[payload_message->message_size+1];
		//strcpy(message_temp,payload_message->message_ptr);
		memcpy(&TxBuffer[12],payload_message->message_ptr,payload_message->message_size);
	}
	else if(packet.type == payload_ann)
	{
		payload_annouce_struct *payload_annouce = (payload_annouce_struct*)packet.payload;
		TxBuffer[12] = payload_annouce->name_size;
		//uint8_t message_temp[payload_annouce->name_size + 1];
		//strcyp(message_temp,payload_annouce->name_node);
		memcpy(&TxBuffer[12],packet.payload,payload_annouce->name_size);
	}
	else if(packet.type == payload_ack)
	{
		payload_acknowledgment_struct *payload_ack = (payload_acknowledgment_struct*)packet.payload;
		TxBuffer[11] = payload_ack->packet_id;
	}
}

/////////////////////////////////////////////////////////////////
return_type L1_Receive(uint8_t *data)
{
	packet_struct  packet;

	uint8_t netid = data[0];
	if(netid != NETID)
	{
		return ret_receive_netid_error;
	}
	packet.ttl = data[1];
	packet.receiver = data[2];
	packet.sender = data[3];
	packet.lastnode = data[4];
	packet.nextnode = data[5];
	if(packet.nextnode != NODENUMBER || packet.nextnode != BROADCASTADRR )
	{
		return ret_receive_wrong_node;
	}
	packet.id =	data[6];
	packet.id = (packet.id << 8 ) | data[7] ;
	packet.id = (packet.id << 8 ) | data[8] ;
	packet.id = (packet.id << 8 ) | data[9] ;
	packet.type = data[10];
	L3_HandlePacket(packet);

	if(packet.type == payload_msg)
	{
		uint8_t message_size = data[11];
		uint8_t *Message = (uint8_t*)calloc(message_size + 1 , sizeof(uint8_t));
		for(uint8_t CountMessage = 0 ; CountMessage < message_size; CountMessage++)
		{
			*(Message + CountMessage) = data[12+CountMessage];
		}
		packet.payload = L2_SetPayloadMessage(Message);
		free(Message);
		L2_handleMessage(packet);
	}
	else if( packet.type == payload_ack)
	{
	    uint32_t message_crc;
	    message_crc = data[11];
	    message_crc= (message_crc << 8 ) | data[12] ;
	    message_crc =(message_crc << 8 )  | data[13] ;
	    message_crc = (message_crc << 8 ) | data[14] ;
	    packet.payload = L2_SetPayloadAck(message_crc);

	    L2_SetPayloadAck(message_crc);
	}
	else if(packet.type == payload_ann)
	{

		uint8_t message_size = data[11];
		uint8_t *Message = (uint8_t*)calloc(message_size + 1 , sizeof(uint8_t));
		for(uint8_t CountMessage = 0 ; CountMessage < message_size; CountMessage++)
		{
			*(Message + CountMessage) = data[12+CountMessage];
		}
		packet.payload = L2_SetPayLoadAnnounce(Message);
		free(Message);
		L2_handleMessage(packet);
	}
	return ret_ok;
}












