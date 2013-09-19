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
	prompt("begin typing data");
	// read DATA_BUFFER_SIZE characters from STD_IN
	// break on newline
	int i;
	for(i=0; i<DATA_BUFFER_SIZE; i++){
		char c = getchar();
		if(c == '\n'){
			info("input halted on newline");
			return;
		}
		else{
			data_buffer[i] = c;
		}
	}
	info("data buffer limit reached, halting input");
	return;
}

void prompt(char* message){
	printf("crc>> %s\n", message);
}

void info(char* message){
	if(DEBUG){
		printf("crc [info] %s\n", message);
	}
}

void main(){
	init_data_buffer();
	read_data();	
}
