/*
 * Layer2.h
 *
 *  Created on: Nov 12, 2021
 *      Author: Admin
 */

#ifndef LORA_DRIVERS_MODULES_DATALINKLAYER_LAYER2_H_
#define LORA_DRIVERS_MODULES_DATALINKLAYER_LAYER2_H_

#include <stdlib.h>
#include <string.h>
#include "Layer1.h"

#define MAX_MESSAGE_SIZE			30
#define NODENUMBER					1

#define BROADCASTADRR				0xFF




return_type L2_handleAnnounce(packet_struct packet);
return_type L2_handleacknowledgment(packet_struct packet);
void L2_handleMessage(packet_struct packet);
return_type L2_relayPacket(packet_struct original_packet);
return_type L2_SendAnnouce();
void* L2_SetPayLoadAnnounce();
return_type L2_SendAcknowledgment(uint8_t receiver,uint32_t packet_id);
void *L2_SetPayloadAck(uint32_t packet_id);
return_type L2_SendMessage(uint8_t receiver, uint8_t *Message);
void* L2_SetPayloadMessage(uint8_t* Message);

#endif /* LORA_DRIVERS_MODULES_DATALINKLAYER_LAYER2_H_ */
