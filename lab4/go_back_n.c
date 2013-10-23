/**
 * Matt Mallett mmallett@iastate.edu
 * Adam Sunderman asu1351@iastate.edu
 * go_back_n.c
 * go back n ARQ protocol implementation
 */

#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <stdint.h>
#include <string.h>

#include "crc.h"
#include "go_back_n.h"

int main(){

	//init the buffers send_to_rec rec_to_send
	init_linked_list(&send_to_receive_buffer); //= (char*) malloc(BUFFER_SIZE * sizeof(char));
	init_linked_list(&receive_to_send_buffer); //(char*) malloc(BUFFER_SIZE * sizeof(char));

	//init the alphabet
	alphabet = (unsigned char*) malloc(26 * sizeof(char));
	int i;
	for(i=0; i<26; i++){
		alphabet[i] = 'A' + i;
	}

	//init thread stuff
	pthread_t sender_tid;
	pthread_t receiver_tid;

	pthread_mutex_init(&send_to_receive_mut, NULL);
	pthread_mutex_init(&receive_to_send_mut, NULL);
	
	pthread_cond_init(&ack_sent, NULL);
	pthread_cond_init(&ack_read, NULL);

	pthread_create(&sender_tid, NULL, sender, NULL);
	pthread_create(&receiver_tid, NULL, receiver, NULL);

	//join the 2 threads
	pthread_join(sender_tid, NULL);
	pthread_join(receiver_tid, NULL);

	return 0;
}//end main

linked_list_t* init_linked_list(linked_list_t* the_list){
	the_list = (linked_list_t*) malloc(sizeof(linked_list_t));
	the_list->head = NULL;
	the_list->tail = NULL;
	the_list->size = 0;
	return the_list;
	
	/*
	newlinked_list.head = NULL;
	newlinked_list.tail = NULL;
	newlinked_list.size = 0;
	return newlinked_list;*/
}

void add_packet(linked_list_t* list, packet_t* pkt){
	if(list->size == 0)
	{
		list->head = pkt;
		list->tail = pkt;
		pkt->next = NULL;
		list->size++;
	}
	else
	{
		list->tail->next = pkt;
		list->tail = pkt;
		pkt->next = NULL;
		list->size++;
	}
}

//returns NULL if element was not able to be removed.
packet_t* remove_packet(linked_list_t* list) {
	if(list->size == 0)
	{
		return NULL;
	}
	else
	{
		packet_t* return_packet;
		return_packet = list->head;
		list->head = list->head->next;
		list->size--;
		return return_packet;
	}
}

void clear_list(linked_list_t* list){
	
	while(list->size > 0){
		free(remove_packet(list));
	}
}

unsigned char* serialize_packet(packet_t mypacket)
{
	unsigned char* return_buffer = (unsigned char*) malloc(6 * sizeof(char));
	return_buffer[0] = mypacket.packet_type;
	return_buffer[1] = mypacket.packet_number;
	return_buffer[2] = mypacket.data[0];
	return_buffer[3] = mypacket.data[1];
	return_buffer[4] = (mypacket.crc_code >> 8) & 0x00FF;
	return_buffer[5] = mypacket.crc_code & 0x00FF;
	return return_buffer;
}

packet_t* deserialize_packet(unsigned char* pkt_buffer, packet_t* ret){
	ret->packet_type = pkt_buffer[0];
	ret->packet_number = pkt_buffer[1];
	ret->data[0] = pkt_buffer[2];
	ret->data[1] = pkt_buffer[3];
	ret->crc_code = (pkt_buffer[4] << 8) | pkt_buffer[5];
	return ret;
}

void transmit_packet(int number){
	
	printf("Sending packet %d\n", number);
	
	//make the packet
	packet_t * pkt = (packet_t*) malloc(sizeof(packet_t));
	pkt->packet_type = DATA_TYPE;
	pkt->packet_number = number;
	pkt->data[0] = alphabet[number * 2];
	pkt->data[1] = alphabet[number * 2 + 1];
	pkt->crc_code = 0;
	unsigned char* pkt_buffer = serialize_packet(*pkt);
	uint16_t crc_code = crc_gen(pkt_buffer, 6, polynomial);
	pkt->crc_code = crc_code;
	
	free(pkt_buffer);
	
	//get in form to corrupt
	pkt_buffer = serialize_packet(*pkt);
	
	//insert error
	
	deserialize_packet(pkt_buffer, pkt);
	free(pkt_buffer);
	
	//add to list
	add_packet(&send_to_receive_buffer, pkt);
	
}

void transmit_window(int start){
	//assume lock is had before this function is called
	int i;
	for(i=0; i<N; i++){
	
		if(start + i >= NUMBER_OF_PACKETS){
			break;
		}
	
		transmit_packet(start + i);
	}
	
}

void *sender(void* arg){
	
	int s_last = 0;
	int s_recent = 0;

	//begin critical section
	//write some stuff
	//write initial three to sender to receiver buffer
	pthread_mutex_lock(&send_to_receive_mut);
	
		transmit_window(s_recent);
		s_recent+=3;
		
		//end critical section
		//release lock, signal other threads
	pthread_mutex_unlock(&send_to_receive_mut);
	//pthread_cond_broadcast(&sender_cv);

	while(s_last < NUMBER_OF_PACKETS){
	
		int type = 0;
		
		pthread_mutex_lock(&receive_to_send_mut);
		
			if(receive_to_send_buffer.size > 0){
			
				packet_t* received_packet = remove_packet(&receive_to_send_buffer);
				
				s_last = received_packet->packet_number;
				type = received_packet->packet_type;
				
				free(received_packet);
			}
		
		pthread_mutex_unlock(&receive_to_send_mut);
	
	
		pthread_mutex_lock(&send_to_receive_mut);
		
			//drop packet from window
			//slide forward
			if(type == ACK_TYPE){
				
				packet_t* pkt = remove_packet(&send_to_receive_buffer);
				//TODO print packet num
				free(pkt);
				
				printf("Packet %d acknowledged\n", s_last -1);
			}
			
			//retransmit window
			else if(type == NAK_TYPE){
			
				clear_list(&send_to_receive_buffer);
				//NAK from s_last
				//TODO print it
				transmit_window(s_last);
				s_recent = s_last + 3;
				
				printf("Packet %d not acknowledged\n", s_last);
			}
			
			//keep window full
			if(send_to_receive_buffer.size < N && s_recent < NUMBER_OF_PACKETS){
			
				transmit_packet(s_recent++);
			}
		
		pthread_mutex_unlock(&send_to_receive_mut);
		
		pthread_cond_broadcast(&ack_read);
		
	}

	return NULL;

}//end sender

void *receiver(void* arg){

	unsigned char r_next = 0;

	while(r_next < NUMBER_OF_PACKETS){
		//wait for something in sender to receiver buffer
		//pthread_cond_wait(&sender_cv, &send_to_receive_mut);

		packet_t* received_packet = NULL;
		int display = 0;
		int good = 0;
		unsigned char data[2];
		
		pthread_mutex_lock(&send_to_receive_mut);
			
			if(send_to_receive_buffer.size > 0){
				received_packet = send_to_receive_buffer.head;
			}
		
			if(received_packet != NULL){
				good = crc_check(serialize_packet(*received_packet), 6, polynomial);
			}
			
			if(good){
				display = received_packet->packet_number == r_next;
				data[0] = received_packet->data[0];
				data[1] = received_packet->data[1];
				
				r_next += display;
				
				if(display){
					printf("\t\tPacket %d:%c%c\n",
						received_packet->packet_number,
						received_packet->data[0],
						received_packet->data[1]);
				}
				else{
					printf("\t\tPacket %d out of sequence\n",
						received_packet->packet_number);
				}
			}
			else{
				printf("\t\tPacket %d in error\n", received_packet->packet_number);
			}
		
		pthread_mutex_unlock(&send_to_receive_mut);
		
		if(received_packet != NULL){
		pthread_mutex_lock(&receive_to_send_mut);
		
			packet_t* pkt = (packet_t*) malloc(sizeof(packet_t));
			
			pkt->packet_type = (good) ? ACK_TYPE : NAK_TYPE;
			pkt->packet_number = r_next;
			
			add_packet(&receive_to_send_buffer, pkt);
		
		pthread_mutex_unlock(&receive_to_send_mut);
		}
		
		//wait for sender to read ack and move state
		pthread_mutex_lock(&send_to_receive_mut);
		pthread_cond_wait(&ack_read, &send_to_receive_mut);
		pthread_mutex_unlock(&send_to_receive_mut);
	}

	return NULL;

}//end receiver
