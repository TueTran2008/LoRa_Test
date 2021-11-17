/*
 * Layer3.h
 *
 *  Created on: Nov 12, 2021
 *      Author: Admin
 */

#ifndef LORA_DRIVERS_MODULES_NETWORKLAYER_LAYER3_H_
#define LORA_DRIVERS_MODULES_NETWORKLAYER_LAYER3_H_

#include "Layer1.h"
#include "Layer2.h"

#include <stdlib.h>
#include <string.h>

#define MAXNODE			10
#define INACTIVEMINS	10
#define TTL				100



void L3_Init();
void L3_UpdateNode();
int elapsedSeconds(uint32_t timestamp);
int L3_removeInactiveNodes();
int L3_getActive(uint8_t destination);
int L3_getNextnode(uint8_t destination);
int L3_getHops(uint8_t destination);
int L3_getRssi(uint8_t destination);



return_type L3_handleAnnouce(packet_struct packet);
return_type L3_HandlePacket(packet_struct packet);


#endif /* LORA_DRIVERS_MODULES_NETWORKLAYER_LAYER3_H_ */
