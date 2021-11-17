/*
 * Layer3.c
 *
 *  Created on: Nov 12, 2021
 *      Author: Admin
 */


#include "Layer3.h"

routing_table_struct routing_table[MAXNODE];
uint8_t node_name[16];

void L3_Init()
{
	memset(routing_table,0,MAXNODE*sizeof(routing_table_struct));
	routing_table[NODENUMBER - 1].active = 1;
	L2_SendAnnouce();
}


void L3_UpdateNode()
{
	routing_table[NODENUMBER-1].active = 1;
	strcpy(routing_table[NODENUMBER - 1].name, node_name);
}


int elapsedSeconds(uint32_t timestamp)
{
  return (millis() - timestamp) / 1000;
}


int L3_removeInactiveNodes()
{
  int ret = 0;
  for (int NodeCount = 0; NodeCount < MAXNODE; NodeCount++)
  {
    if (NodeCount != (NODENUMBER - 1) && routing_table[NodeCount].active)
    {
      if ((elapsedSeconds(routing_table[NodeCount].timestamp) / 60) >= INACTIVEMINS)
      {
        routing_table[NodeCount].active = 0;
        ret++;
      }
    }
  }
}

int L3_getActive(uint8_t destination)
{
  return routing_table[destination - 1].active;
}

int L3_getNextnode(uint8_t destination)
{
	if(destination == BROADCASTADRR)
	{
		return BROADCASTADRR;
	}
	else if(routing_table[NODENUMBER-1].active == 1)
	{
		return routing_table[NODENUMBER-1].next_node;
	}
	else
	{
		return 0;
	}
}

int L3_getHops(uint8_t destination)
{
  return routing_table[destination - 1].hops;
}
int L3_getRssi(uint8_t destination)
{
  return routing_table[destination - 1].rssi;
}

return_type L3_HandlePacket(packet_struct packet)
{
	int16_t rssi = packet.rssi;
	int packet_hops = TTL - packet.ttl;
	routing_table[packet.lastnode - 1].timestamp = HAL_GetTick();
	routing_table[packet.lastnode - 1].rssi = rssi;
	if(packet.sender != NODENUMBER)
	{
		if(routing_table[packet.lastnode - 1].active == 0)
		{
			routing_table[packet.lastnode - 1].active = 1;
			routing_table[packet.lastnode - 1].hops = 0;
			routing_table[packet.lastnode - 1].last_id = 0;
			routing_table[packet.lastnode - 1].next_node = packet.lastnode;
		}
		if(packet_hops > 0 )
		{
			if (routing_table[packet.sender - 1].active == 0)
			{
				routing_table[packet.lastnode - 1].active = 1;
				routing_table[packet.lastnode - 1].hops = packet_hops;
				routing_table[packet.lastnode - 1].last_id = 0;
				routing_table[packet.lastnode - 1].next_node = packet.lastnode;
			}
		}
		return ret_ok;
	}
	return ret_error;
}



return_type L3_handleAnnouce(packet_struct packet)
{
	  int packet_rssi = packet.rssi;
	  int packet_hops = TTL - packet.ttl;

	  int current_hops = routing_table[packet.sender - 1].hops;
	  int current_rssi = routing_table[packet.sender - 1].rssi;

	  strncpy(routing_table[packet.sender - 1].name, ((payload_annouce_struct *)packet.payload)->name_node, ((payload_annouce_struct *)packet.payload)->name_size);
	  routing_table[packet.sender - 1].name[((payload_annouce_struct *)packet.payload)->name_size] = 0;

	  if (packet.id == routing_table[packet.sender - 1].last_id)
	  {
	    if (packet_hops < current_hops || (packet_hops == current_hops && packet_rssi > current_rssi))
	    {
	      routing_table[packet.sender - 1].rssi = packet_rssi;
	      routing_table[packet.sender - 1].next_node = packet.lastnode;
	      routing_table[packet.sender - 1].hops = packet_hops;
	      return ret_routing_better;
	    }
	    else
	      return ret_routing_worse;
	  }
	  else
	  {
	    routing_table[packet.sender - 1].rssi = packet_rssi;
	    routing_table[packet.sender - 1].last_id = packet.id;
	    routing_table[packet.sender - 1].next_node = packet.lastnode;
	    routing_table[packet.sender - 1].hops = packet_hops;
	    return ret_routing_updated;
	  }
}
