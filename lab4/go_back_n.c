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
	send_to_receive_buffer = generate_linkedList(); //= (char*) malloc(BUFFER_SIZE * sizeof(char));
	receive_to_send_buffer = generate_linkedList(); //(char*) malloc(BUFFER_SIZE * sizeof(char));

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

	pthread_cond_init(&sender_cv, NULL);
	pthread_cond_init(&receiver_cv, NULL);

	pthread_create(&sender_tid, NULL, sender, NULL);
	pthread_create(&receiver_tid, NULL, receiver, NULL);

	//join the 2 threads
	pthread_join(sender_tid, NULL);
	pthread_join(receiver_tid, NULL);

	return 0;
}//end main

linkedList generate_linkedList(){
	linkedList newLinkedList;
	newLinkedList.head = NULL;
	newLinkedList.tail = NULL;
	newLinkedList.size = 0;
	return newLinkedList;
}

void addPacket(linkedList* list, packet* pak){
	if(list->size == 0)
	{
		list->head = pak;
		list->tail = pak;
		pak->next = NULL;
		list->size++;
	}
	else
	{
		list->tail->next = pak;
		list->tail = pak;
		pak->next = NULL;
		list->size++;
	}
}

//returns NULL if element was not able to be removed.
packet* removePacket(linkedList* list) {
	if(list->size == 0)
	{
		return NULL;
	}
	else
	{
		packet* returnPacket;
		returnPacket = list->head;
		list->head = list->head->next;
		list->size--;
		return returnPacket;
	}
}

unsigned char* serialize_packet(packet mypacket)
{
	unsigned char* returnBuffer = (unsigned char*) malloc(6 * sizeof(char));
	returnBuffer[0] = mypacket.packet_type;
	returnBuffer[1] = mypacket.packet_number;
	returnBuffer[2] = mypacket.data1;
	returnBuffer[3] = mypacket.data2;
	returnBuffer[4] = (mypacket.crc_code >> 8) & 0x00FF;
	returnBuffer[5] = mypacket.crc_code & 0x00FF;
	return returnBuffer;
}

void *sender(void* arg){
	
	unsigned char r_last = 0;

	int packet_received = 0;

	packet* receivedPacket;

	packet* currentPacket;

	unsigned char* working_buffer;

	int alphabet_index = 0;

	int i;

	//begin critical section
	//write some stuff
	//write initial three to sender to receiver buffer
	pthread_mutex_lock(&send_to_receive_mut);
	for(i=0; i<3; i++){
		currentPacket = (packet*)malloc(sizeof(packet));
		currentPacket->packet_type = DATA_TYPE;
		currentPacket->packet_number = r_last+i;
		currentPacket->data1 = alphabet[2 * i];
		currentPacket->data2 = alphabet[2 * i + 1];
		currentPacket->crc_code = 0;
		working_buffer = serialize_packet(*currentPacket);
		uint16_t crc_code = crc_gen(working_buffer, 6, polynomial);
		currentPacket->crc_code = crc_code;
		addPacket(&send_to_receive_buffer, currentPacket);
		free(working_buffer);
	}

	//end critical section
	//release lock, signal other threads
	pthread_mutex_unlock(&send_to_receive_mut);
	//pthread_cond_broadcast(&sender_cv);
	
	//memcpy(send_to_receive_buffer, working_buffer, 18);

	while(r_last < 13){

		//wait until something in receiver to sender buffer
		//pthread_cond_wait(&receiver_cv, &receive_to_send_mut);
		//pthread_mutex_lock(&receive_to_send_mut);
		//clear to read, have lock

		//read the buffer
		//memcpy(working_buffer, receive_to_send_buffer, 6);

		// MAKE THESE USE LINKED LIST

                //'pop' the packet from the queue
      //          memmove(receive_to_send_buffer, receive_to_send_buffer + 6, 12);
		
		//pthread_mutex_unlock(&receive_to_send_mut);

		packet_received = 0;

		//'pop' the packet from the queue
		//	memmove(send_to_receive_buffer, send_to_receive_buffer + 6, 12);

		packet* receivedPacket;

		while(packet_received == 0)
		{

			pthread_mutex_lock(&receive_to_send_mut);
		
			if(receive_to_send_buffer.size != 0)
			{
				receivedPacket = removePacket(&receive_to_send_buffer);
				packet_received = 1;
				printf("ACK received\n");
			}
			pthread_mutex_unlock(&receive_to_send_mut);
		}
		

		//ACK or NAK??????
		
		//if NAK
		//	resend window
	
		//if ACK
		//	if sequence number in window
		//		r_next = sequence number
		//		transmit values r_next through r_next +2
		//	else
		//		discard packet

		//build the packet above, transmission here
		pthread_mutex_lock(&send_to_receive_mut);
        	//begin critical section
	        //write some stuff        

        	//end critical section
	        //release lock, signal other threads
	       
		pthread_mutex_unlock(&send_to_receive_mut);
       	        pthread_cond_broadcast(&sender_cv);
	}

	return NULL;

}//end sender

void *receiver(void* arg){

	int packet_received = 0;

	unsigned char r_next = 0;

	unsigned char * working_buffer = (unsigned char*) malloc(20 * sizeof(char));

	while(r_next < 13){
		//wait for something in sender to receiver buffer
		//pthread_cond_wait(&sender_cv, &send_to_receive_mut);

//		memcpy(working_buffer, send_to_receive_buffer, 6);

		// MAKE THIS USE LINKED LIST

		packet_received = 0;

		//'pop' the packet from the queue
	//	memmove(send_to_receive_buffer, send_to_receive_buffer + 6, 12);

		packet* receivedPacket;

		while(packet_received == 0)
		{

			pthread_mutex_lock(&send_to_receive_mut);
		
			if(send_to_receive_buffer.size != 0)
			{
				receivedPacket = removePacket(&send_to_receive_buffer);
				packet_received = 1;
			}
			pthread_mutex_unlock(&send_to_receive_mut);
		}

		int packet_number = receivedPacket->packet_number;

		//crc check for error
		int good = crc_check(serialize_packet(*receivedPacket), 6, polynomial);

		packet* currentPacket = (packet*) malloc(sizeof(packet));
		
		//if error
		//	send NAK with r_next
		if(!good){
			currentPacket->packet_type = NAK_TYPE;
			currentPacket->packet_number = r_next;
			currentPacket->data1 = 0x00;
			currentPacket->data2 = 0x00;
		}
	
		//if !error
		//	sequence number == r_next?
		//	NO
		//		NAK r_next
		//	YES
		//		report data to program
		//		r_next ++
		//		ACK r_next
		else{
			if(packet_number == r_next){
				//report buffer[2] buffer[3]
				currentPacket->packet_type = ACK_TYPE;
	      		currentPacket->packet_number = ++r_next;
				currentPacket->data1 = 0x00;
				currentPacket->data2 = 0x00;
				printf("receive to send packet type %d packet # %d\n", currentPacket->packet_type, currentPacket->packet_number);
			}
			else{
				currentPacket->packet_type = NAK_TYPE;
				currentPacket->packet_number = r_next;
				currentPacket->data1 = 0x00;
				currentPacket->data2 = 0x00;
			}	
		}

		uint16_t crc_code = crc_gen(serialize_packet(*currentPacket), 6, polynomial);
		currentPacket->crc_code = crc_code;
		
		//build packet above send below
		pthread_mutex_lock(&receive_to_send_mut);
		
		//write packet
//		memcpy(receive_to_send_buffer, working_buffer, 6);

		// ADD TO LINK LIST
		addPacket(&receive_to_send_buffer, currentPacket);

		pthread_mutex_unlock(&receive_to_send_mut);
		//pthread_cond_broadcast(&sender_cv);
	}

	return NULL;

}//end receiver
