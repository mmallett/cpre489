/**
 * Matt Mallett mmallett@iastate.edu
 * Adam Sunderman asu1351@iastate.edu
 * go_back_n.h
 * go back n ARQ definitions
 */

#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <stdint.h>
#include <string.h>

#include "crc.h"


//function definitions for sender and receiver threads
void *sender(void *);
void *receiver(void *);

typedef struct{
	packet* next;
	unsigned char packet_type;
	unsigned char packet_number;
	unsigned char data1;
	unsigned char data2;
	uint16_t crc_code;
} packet;

typedef stuct{
	packet* head;
	packet* tail;
	int size;
} linkedList;

#define ACK_TYPE 1 

#define NAK_TYPE 2

#define DATA_TYPE 3

#define BUFFER_SIZE 500

#define N 3

//generated linked list to store
//packets for send_to_receive_buffer and
//receive_to_send_buffer
linkedList generate_linkedList();

//adds a packet to end of linked list
void addPacket(linkedList*, packet*);

//removes a packet from front of linked list
packet* removePacket(linkedList*);

unsigned char* serialize_packet(packet);

unsigned char* alphabet;

//sender writes to it receiver reads from it
//sender writes data packets containing alphabet
linkedList send_to_receive_buffer;

//lock on send_to_receive_buffer
pthread_mutex_t send_to_receive_mut;

//condition variable that sender has completed its buffer work
pthread_cond_t sender_cv;

//receiver writes to it sender reads from it
//receiver writes ACK or NAK packets
linkedList receive_to_send_buffer;

//lock on receive_to_send_buffer
pthread_mutex_t receive_to_send_mut;

//condition variabel that receiver has completed its buffer work
pthread_cond_t receiver_cv;

uint32_t polynomial = 0b10001000000100001; //X^16+X^12+X^5 +1
