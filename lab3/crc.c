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
		

}

uint32_t crc_alg(char* data, int data_length, uint32_t gen_poly){

	char message[40];
	sprintf(message, "%d bytes using polynomial %x", data_length, gen_poly);
	debug(message);

	// error check	
	int data_mem_size = sizeof(data) / sizeof(char);
	if(data_mem_size < data_length){
		sprintf(message, "size of data buffer (%d) is less than data_length (%d)", data_mem_size, data_length);
		error(message);
		return;
	}
	if(data_length < 2){
		error("can't compute crc16 with less than 16 bits!");
		return;
	}

	// load first 17 bits into dividend
	uint32_t dividend = // 17 bits
		(data[0] << 9) | // 8 bits from byte 0
		(data[1] << 1) | // 8 bits from byte 1
		((data[2] >> 7) & 0x01); //1 bit from byte 2
	uint32_t divisor = gen_poly; //17 bits

	sprintf(message, "dividend %x", dividend);
	debug(message);

	int byte_pointer = 2;
	int bit_pointer = 6;

	//17 bit buffer
	uint32_t ret;

	while(byte_pointer < data_length){
		
		//xor dividend divisor

		//dividend = result

		//sll and pop next bit until
		//MSB of dividend is one or byte_pointer passed maximum

		uint32_t result = dividend ^ divisor;
		uint32_t working_buffer = result;
		
		//do until 1 in MSB
		while(!(working_buffer >> 16)){
			// move state to next bit
			bit_pointer--;
			if(bit_pointer < 0){
				bit_pointer = 7;
				byte_pointer++;
			}

			debug("loading next bit into working buffer");
			sprintf(message, "byte %d bit %d",
				byte_pointer, bit_pointer);
			debug(message);

			// the last bit position move exceeds
			// data buffer range
			// this indicates that the current
			// result is the return value
			if(byte_pointer >= data_length){
				debug("EOF");
				ret = result;
				break;
			}

			// isolate the bit_pointer bit and OR it into
			// LSB of working buffer
			working_buffer <<= 1;
			uint32_t mask = 0x00000001 << bit_pointer;
			working_buffer |= (uint32_t) (data[byte_pointer] & mask) >> bit_pointer;
			
			sprintf(message, "working buffer %x", working_buffer);
			debug(message);			

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
	uint32_t polynomial = 0b10001000000100001; //X^16+X^12+X^5 +1
	data_buffer[0] = 'a';
	uint32_t crc_code = crc_alg(data_buffer, 3, polynomial);
	char message[40];
	sprintf(message, "crc code: %x", crc_code);
	info(message);	
}
