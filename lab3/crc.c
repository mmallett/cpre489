/*
 * Matt Mallett mmallett@iastate.edu
 * Adam Sunderman asu1351@iastate.edu
 * CprE 489 Lab 3
 * 
 * crc.c
 * CRC generator and checker implementation
 */

#include "crc.h"

void init_data_buffer(){
	data_buffer = (char*) malloc((DATA_BUFFER_SIZE + 4) * sizeof(char));
}

void read_data(){
	// read DATA_BUFFER_SIZE characters from STD_IN
	// break on newline
	int i;
	for(i=0; i<DATA_BUFFER_SIZE; i++){
		char c = getchar();
		if(c == '\n'){
			log_info("input halted on newline");
			return;
		}
		else{
			data_buffer[i] = c;
		}
	}
	log_info("data buffer limit reached, halting input");
	return;
}

void log_info(char* message){
	if(DEBUG){
		printf("[INFO] %s\n", message);
	}
}

int main(){
	
}
