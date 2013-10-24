/**
 * Matt Mallett mmallett@iastate.edu
 * Adam Sunderman asu1351@iastate.edu
 * go_back_n.h
 * go back n ARQ definitions
 */
 
//probability of bit corruption
#define P 0.01

#include <pthread.h>
#include <stdint.h>

//count number of transmissions
int mattsbigcounter;

//function definitions for sender and receiver threads
void *sender(void *);
void *receiver(void *);

//packet data structure holds all information of a packet
typedef struct packet packet_t;

struct packet{
	packet_t* next;
	unsigned char packet_type;
	unsigned char packet_number;
	unsigned char data[2];
	uint16_t crc_code;
};

//linked list data structure chains together packets
typedef struct linked_list linked_list_t;

struct linked_list{
	packet_t* head;
	packet_t* tail;
	int size;
};

#define ACK_TYPE 1 

#define NAK_TYPE 2

#define DATA_TYPE 3

#define BUFFER_SIZE 500

#define N 3

#define NUMBER_OF_PACKETS 13

//generated linked list to store
//packets for send_to_receive_buffer and
//receive_to_send_buffer
//allocates the memory for the structure internally
linked_list_t* init_linked_list(linked_list_t*);

//adds a packet to end of linked list
void add_packet(linked_list_t*, packet_t*);

//removes a packet from front of linked list
//returns NULL if list is empty
packet_t* remove_packet(linked_list_t*);

//empties given list
void clear_list(linked_list_t*);

//transforms the packet into a 6 bytes array
unsigned char* serialize_packet(packet_t);

//transforms 6 byte array into packet
packet_t* deserialize_packet(unsigned char*, packet_t*);

//transmits packet with given id
void transmit_packet(int);

//transmits N packets, checks bounds of NUMBER OF PACKETS
void transmit_window(int);

//pointer to memory containing alphabet
unsigned char* alphabet;

//sender writes to it receiver reads from it
//sender writes data packets containing alphabet
linked_list_t send_to_receive_buffer;

//lock on send_to_receive_buffer
pthread_mutex_t send_to_receive_mut;

//receiver writes to it sender reads from it
//receiver writes ACK or NAK packets
linked_list_t receive_to_send_buffer;

//lock on receive_to_send_buffer
pthread_mutex_t receive_to_send_mut;

//cvs to signal that sender has read an ack/nak 
pthread_cond_t 	 ack_sent; 	
pthread_cond_t 	 ack_read; 	

uint32_t polynomial = 0b0010001000000100001; //X^16+X^12+X^5 +1

long M = 2147483647;

void IntroduceError(char*, double);
