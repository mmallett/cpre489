/**
 * Matt Mallett mmallett@iastate.edu
 * Adam Sunderman asu1351@iastate.edu
 * go_back_n.c
 * go back n ARQ protocol implementation
 */

#include "go_back_n.h"

int main(){

	//init the buffers send_to_rec rec_to_send
	send_to_receive_buffer = (char*) malloc(BUFFER_SIZE * sizeof(char));
	receive_to_send_buffer = (char*) malloc(BUFFER_SIZE * sizeof(char));


	//init the alphabet
	alphabet = (char*) malloc(26 * sizeof(char));
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
}

void *sender(void* arg){
	
	unsigned char r_next;

	//write initial three to sender to receiver buffer

	while(r_next < 13){

		//wait until something in receiver to sender buffer

		//ACK or NAK??????
		
		//if NAK
		//	resend window
	
		//if ACK
		//	if sequence number in window
		//		r_next = sequence number
		//		transmit values r_next through r_next +2
		//	else
		//		discard packet


	}

}

void *receiver(void* arg){

	unsigned char r_next;

	while(r_next < 13){
		
		//wait for something in sender to receiver buffer

		//crc check for error
		
		//if error
		//	send NAK with r_next
	
		//if !error
		//	sequence number == r_next?
		//	NO
		//		NAK r_next
		//	YES
		//		report data to program
		//		r_next ++
		//		ACK r_next



	}

}
