/*
 * LoRa_PacketType.h
 *
 *  Created on: Nov 11, 2021
 *      Author: Admin
 */

#ifndef LORA_DRIVERS_MODULES_LORA_PACKETTYPE_H_
#define LORA_DRIVERS_MODULES_LORA_PACKETTYPE_H_


#include "Std_Types.h"
#define MAXNODE 10




typedef enum return_type
{
  ret_ok = 0,
  ret_error,
  ret_buffer_empty,
  ret_buffer_full,
  ret_send_duty_error,
  ret_send_anticollision_error,
  ret_send_error,
  ret_send_size_error,
  ret_receive_netid_error,
  ret_receive_wrong_node,
  ret_receive_duplicate,
  ret_ttl_error,
  ret_routing_worse,
  ret_routing_better,
  ret_routing_updated,
  ret_message_not_found,
  ret_message_found
} return_type;


typedef enum payload_type
{
  payload_msg = 0,
  payload_ack,
  payload_ann
} payload_type;







////////////////////////////////////

typedef struct
{
	uint8_t 	 ttl;				// Time to live. Counting Hop down
	uint8_t		 receiver;			// Receiver Node Number
	uint8_t 	 sender;				// Sender Node Number
	uint8_t 	 lastnode;			//
	uint8_t 	 nextnode;
	uint32_t 	 id;
	payload_type type;
	void*		 payload;
	int16_t 	 rssi;
}packet_struct;



typedef struct
{
	packet_struct*	packet_ptr;
	uint8_t		packet_head;
	uint8_t		packet_tail;
}packet_buffer;

//////////////////////////////////////////////////////////
typedef struct
{
	uint8_t 	message_size;			// message lenght
	uint8_t*	message_ptr;			// message text
}payload_message_struct;
//////////////////////////////////////////////////////////
typedef struct
{
  uint32_t packet_id;
} payload_acknowledgment_struct;
// ACK ID from receiver to sender

//////////////////////////////////////////////////////////
typedef struct
{
	uint8_t name_size;					//node name size
	uint8_t* name_node;					//node's name
}payload_annouce_struct;

typedef struct
{
	uint8_t active;
	uint8_t next_node;
	uint8_t hops;
	int8_t rssi;
	uint32_t last_id;
	char name[16];
	uint32_t timestamp;
}routing_table_struct;


typedef struct
{
  uint8_t receiver;
  uint8_t sender;
  uint32_t id;
  char *message;
  uint8_t acks;
  uint8_t acks_nodes[MAXNODE - 1];
} message_struct;



#endif /* LORA_DRIVERS_MODULES_LORA_PACKETTYPE_H_ */
