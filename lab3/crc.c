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
	//overcommit on memory to stop weird things from happening..
	data_buffer = (char*) malloc((DATA_BUFFER_SIZE + 20) * sizeof(char));
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
			input_length = i;
			return;
		}
		else{
			data_buffer[i] = c;
		}
	}
	info("data buffer limit reached, halting input");
	input_length = i;
	return;
}

int16_t crc_gen(char* data, int16_t gen_poly){
	int i;
	for(i=0; i<input_length+2; i++){
		// steps to interpret array as a bitstream
		// shift and mask to create a single 16 bit continuous 
		// chunk out of 3 bytes
		int j;
		for(j=0; j<8; j++){
			// upper bits of 8 MSB
			unsigned char seg_1 = (data_buffer[i] << j) << 8;
			// lower part of 8 MSB
			unsigned char seg_2 = (((unsigned) data_buffer[i+1]) >> (8 - j)) << 8;
			// upper part of 8 LSB
			unsigned char seg_3 = data_buffer[i+1] << j;
			// lower part of 8 LSB
			unsigned char seg_4 = data_buffer[i+3] & (0xFF ^ (0xFF << j));

			uint16_t chunk = seg_1 | seg_2 | seg_3 | seg_4;

			uint16_t result = chunk ^ gen_poly;

			// deconstruct result into the correct offsets across 3 bytes
		}
	}
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
