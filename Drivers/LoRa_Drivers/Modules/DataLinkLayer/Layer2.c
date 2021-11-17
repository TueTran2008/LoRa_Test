/*
 * Layer2.c
 *
 *  Created on: Nov 12, 2021
 *      Author: Admin
 */


#include "Layer2.h"


////////////////////////////////////////////////////////////////

uint8_t node_name[16];
/*
 * 	@brief : Set Message to Transmit
 *  @param Message: Pointer to Transmit Data
 *  @return : Pointer to payload_message_struct
 *
 */
void* L2_SetPayloadMessage(uint8_t* Message)
{
	payload_message_struct *payload_message;
	payload_message = (payload_message_struct *)calloc(1,sizeof(payload_message_struct));
	uint16_t Message_size = (uint16_t)strlen(Message);
	payload_message->message_size = Message_size;
	payload_message->message_ptr = (uint8_t*)calloc(1 + Message_size, sizeof(uint8_t));
	strcpy(payload_message->message_ptr,Message);
	return payload_message;
}



/*
 *
 *
 *
 */
return_type L2_SendMessage(uint8_t receiver, uint8_t *Message)
{
	uint8_t Message_size = (uint8_t)strlen(Message);
	if( Message_size == 0 || Message_size > MAX_MESSAGE_SIZE)
	{
		return ret_send_size_error;
	}
	if(receiver == NODENUMBER || receiver == 0)
	{
		return ret_send_error;
	}
	packet_struct packet;
	packet.receiver = receiver;
	//packet.nextnode = get next node from Layer3;
	packet.sender= NODENUMBER;
	packet.type = payload_msg;
	packet.payload = L2_SetPayloadMessage(Message);
	return L1_QueuePacket(packet);
}




////////////////////////////////////////////////////////////////
/*
 *
 *
 *
 */
void *L2_SetPayloadAck(uint32_t packet_id)
{
	payload_acknowledgment_struct *payload_acknowledgment;
	payload_acknowledgment = (payload_acknowledgment_struct *)calloc(1,sizeof(payload_acknowledgment_struct ));
	payload_acknowledgment->packet_id = packet_id;
	return payload_acknowledgment;
}


return_type L2_SendAcknowledgment(uint8_t receiver,uint32_t packet_id)
{
	if(receiver == NODENUMBER || receiver == 0)
	{
		return ret_send_error;
	}
	packet_struct packet = {
			.lastnode = NODENUMBER,
			//.nextnode =
			.receiver = receiver,
			.sender = NODENUMBER,
			.type = payload_ack,
			.payload = L2_SetPayloadAck(packet_id)
	};
	return L1_QueuePacket(packet);
}



////////////////////////////////////////////////////////////////

void* L2_SetPayLoadAnnounce()
{
	uint8_t name_size = (uint8_t)strlen(node_name);
	payload_annouce_struct *payload_annouce;
	payload_annouce = (payload_annouce_struct *)calloc(1,sizeof(payload_annouce_struct ));
	payload_annouce->name_size = name_size;
	payload_annouce->name_node = (uint8_t*)calloc(payload_annouce->name_size + 1,sizeof(uint8_t));
	strcpy(payload_annouce->name_node,node_name);
	return payload_annouce;
}

return_type L2_SendAnnouce()
{
	uint8_t name_size = (uint8_t)strlen(node_name);
	if(name_size == 0 || name_size > 15)
	{
		return ret_send_error;
	}
	packet_struct packet = {
			.lastnode = NODENUMBER,
			//.nextnode
			.receiver = BROADCASTADRR,
			.sender = NODENUMBER,
			.type = payload_ann,
			.payload = L2_SetPayLoadAnnounce()
	};
	return L1_QueuePacket(packet);
}


///////////////////////////////////////////////////////

return_type L2_relayPacket(packet_struct original_packet)
{
	if(original_packet.receiver == 0 || original_packet.receiver == NODENUMBER ||(original_packet.receiver > 10 && original_packet.receiver != BROADCASTADRR))
	{
		return ret_send_error;
	}
	if(original_packet.ttl == 0)
	{
		return ret_ttl_error;
	}
	packet_struct packet = original_packet;
	packet.ttl--;
	packet.lastnode = NODENUMBER;
	packet.nextnode = L3_getNextnode(packet.receiver);
	return L1_QueuePacket(packet);
}






void L2_handleMessage(packet_struct packet)
{
	if(packet.sender != NODENUMBER && (packet.nextnode == NODENUMBER || packet.nextnode == BROADCASTADRR))
	{
		if(packet.receiver == NODENUMBER || packet.receiver == BROADCASTADRR)
		{
			//message_save
			L2_SendAcknowledgment(packet.sender, packet.id);
		}
		if(packet.receiver  != NODENUMBER && packet.ttl > 1)
		{
			L2_relayPacket(packet);
		}
	}
	return ret_ok;
}




return_type L2_handleacknowledgment(packet_struct packet)
{
  if (packet.sender != NODENUMBER && (packet.nextnode == NODENUMBER || packet.nextnode == BROADCASTADRR))
  {
    if (packet.receiver == NODENUMBER)
    {
      int acks = message_getAckNum(packet.sender, ((payload_acknowledgment_struct *)packet.payload)->packet_id);
      int node_number;

      for (int i = 0; i < acks; i++)
      {
        node_number = message_getAckNode(packet.sender, ((payload_acknowledgment_struct *)packet.payload)->packet_id, i);
      }
    }

    if (packet.receiver != NODENUMBER && packet.ttl > 1)
    {
      L2_relayPacket(packet);
    }
  }
  return ret_ok;
}



return_type L2_handleAnnounce(packet_struct packet)
{
  if (packet.receiver == BROADCASTADRR && packet.sender != NODENUMBER)
  {
	  L3_handleAnnouce(packet);

    if (packet.ttl > 1)
    {
      L2_relayPacket(packet);
    }
    return ret_ok;
  }
  return ret_error;
}







