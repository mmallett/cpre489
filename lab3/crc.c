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
		// maintain 16 bit result buffer
		// start by xor generator polynomial
		// pull bits from the data such that MSB has a 1
		// xor result and data buff
		
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

int16_t crc_alg(char* data, int data_length, int16_t gen_poly){

	// error check	
	int data_mem_size = sizeof(data) / sizeof(char);
	if(data_mem_size < data_length){
		char message[40];
		sprintf(message, "size of data buffer (%d) is less than data_length (%d)", data_mem_size, data_length);
		error(message);
		return;
	}
	if(data_length < 2){
		error("can't compute crc16 with less than 16 bits!");
		return;
	}

	// load first 16 bits into dividend
	int16_t dividend = (data[0] << 8) | data[1];
	int16_t divisor = gen_poly;

	int byte_pointer = 2;
	int bit_pointer = 7;

	int16_t ret;

	while(byte_pointer < data_length){
		
		//xor dividend divisor

		//dividend = result

		//sll and pop next bit until
		//MSB of dividend is one or byte_pointer passed maximum

		uint16_t result = dividend ^ divisor;
		uint16_t working_buffer = result;
		
		//do until 1 in MSB
		while(!(working_buffer >> 15)){
			// move state to next bit
			bit_pointer--;
			if(bit_pointer < 0){
				bit_pointer = 7;
				byte_pointer++;
			}

			// the last bit position move exceeds
			// data buffer range
			// this indicates that the current
			// result is the return value
			if(byte_pointer >= data_length){
				ret = result;
				break;
			}

			// isolate the bit_pointer bit and OR it into
			// LSB of working buffer
			working_buffer <<= 1;
			uint16_t mask = 0x0001 << bit_pointer;
			working_buffer |= (uint16_t) (data[byte_pointer] & mask) >> bit_pointer;
			
		}//end inner while

		dividend = working_buffer;
			
	}//end outer while

	return ret;
}

void prompt(char* message){
	printf("crc>> %s\n", message);
}

void info(char* message){
	printf("crc [info] %s\n", message);
}

void debug(char* message){
	if(DEBUG){
		printf("crc [debug] %s\n", message);
	}
}

void error(char* message){
	printf("crc [error] %s\n", message);
}

void main(){
	//init_data_buffer();
	//read_data();	
	char* data_buffer = (char*) calloc(3, sizeof(char));
	int16_t polynomial = 0b1000000000000001;
	data_buffer[0] = 'a';
	int16_t crc_code = crc_alg(data_buffer, 3, polynomial);
	printf("mats big code %x\n", crc_code);
}
