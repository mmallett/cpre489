/*
 * Matt Mallett mmallett@iastate.edu
 * Adam Sunderman asu1351@iastate.edu
 * CprE 489 Lab 3
 * 
 * crc.c
 * CRC generator and checker implementation
 */

#include "crc.h"

// construct the data buffer
void init_data_buffer(){
	//overcommit on memory to stop weird things from happening..
	data_buffer = (unsigned char*) calloc((DATA_BUFFER_SIZE + 20) , sizeof(char));
}

// read data from keyboard into data buffer
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

// generate a crc field for a given data buffer
//     char* data the data to generate the CRC for
//     int16_t crc return pointer for the generated crc
//     returns the calculated crc
//
uint32_t crc_gen(unsigned char* data, int data_length, uint32_t gen_poly){
	return crc_alg(data, data_length, gen_poly);
}

// check if the crc is valid for the given data
//     char* data the data to check crc against
//     int16_t crc crc code used to check with
//     returns 1 if valid 0 if invalid
//
int crc_check(unsigned char* data, int data_length, uint32_t gen_poly)
{	
	uint32_t code = crc_alg(data, data_length, gen_poly);
	char message[4000];
	sprintf(message, "check code %x", code);
	info(message);
	
	if(code == 0)
		return 1;
	else
		return 0;
}

// compute crc checksum
uint32_t crc_alg(unsigned char* data, int data_length, uint32_t gen_poly){

	char message[4000];
	sprintf(message, "%d bytes using polynomial %x", data_length, gen_poly);
	debug(message);

	sprintf(message, "first 3 bytes %x%x%x", data[0], data[1], data[2]);
	debug(message);

	// load first 17 bits into dividend
	uint32_t dividend =  // 17 bits
		(((uint32_t)data[0]) << 9) | // 8 bits from byte 0
		(((uint32_t)data[1]) << 1) | // 8 bits from byte 1
		((((uint32_t)data[2]) >> 7) & 0x01); //1 bit from byte 2
	uint32_t divisor = gen_poly; //17 bits

	sprintf(message, "dividend %05x", dividend);
	debug(message);

	int byte_pointer = 2;
	int bit_pointer = 6;

	//17 bit buffer
	uint32_t ret;
	
	//the best buffer ever
	uint32_t working_buffer = dividend;

	//guess what this is
	uint32_t result;

	while(byte_pointer < data_length){
		
		//get working buffer ready for 'divide'
		while(!(working_buffer >> 16)){

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
			//	ret = result;
			//	break;
				goto END;
			}

			// isolate the bit_pointer bit and OR it into
			// LSB of working buffer
			working_buffer <<= 1;
			uint32_t mask = 0x00000001 << bit_pointer;
			working_buffer |= (uint32_t) (data[byte_pointer] & mask) >> bit_pointer;
//			uint32_t the_byte = (uint32_t) (data[byte_pointer] & mask) >> bit_pointer;
//			working_buffer |= the_byte;

//			sprintf(message, "bit %d in byte %d is %x", bit_pointer, byte_pointer, the_byte);
//			debug(message);
			
			sprintf(message, "working buffer %05x", working_buffer);
			debug(message);			

			// move state to next bit
			bit_pointer--;
			if(bit_pointer < 0){
				bit_pointer = 7;
				byte_pointer++;
			}


		}//end inner while

		//do the 'divide'
		
		result = working_buffer ^ divisor;
		working_buffer = result;


		sprintf(message, "result of 'divide' %x", result);
//		debug(message);
//		printf("testing\n");
			
	}//end outer while
END:
	return working_buffer & 0x0000FFFF;
}

/*****************************************************************************/
/* */
/* This routine is called with a pointer to a null terminted string of */
/* characters, data, and a probability of bit error, p, and introduces */
/* errors in the bits of the string according to this probability. */
/* The routine does not return anything. */
/* */
/*****************************************************************************/

//formats input prompt printing
void prompt(char* message){
	//printf("crc>> %s\n", message);
}

//formats information printing
void info(char* message){
	//printf("crc [info] %s\n", message);
}

//formats debug printing, toggled off by setting DEBUG = 0
void debug(char* message){
	/*if(DEBUG){
		printf("crc [debug] %s\n", message);
	}*/
}

//formats error printing
void error(char* message){
	//printf("crc [error] %s\n", message);
}

/*void main(){
	// get transmission data
	init_data_buffer();
	read_data();	

	//char* data_buffer = (char*) calloc(3, sizeof(char));
	// the generator polynomial
	uint32_t polynomial = 0b10001000000100001; //X^16+X^12+X^5 +1
	//data_buffer[0] = 'a';

	//get the crc code for this transmission data buffer
	int data_length = input_length+2;
	uint32_t crc_code = crc_gen(data_buffer, data_length, polynomial);

	char message[4000];
	sprintf(message, "crc code: %x", crc_code);
	info(message);

	// place the crc code into the last 2 bytes of the data buffer
	data_buffer[data_length-2] = (crc_code >> 8) & 0x000000FF;
	data_buffer[data_length-1] = crc_code & 0x000000FF;
	//add null terminator at end of code
	data_buffer[data_length] = '\0';
	
	sprintf(message, "Original: %s", data_buffer);
	info(message);	

	//corruption by XOR

	unsigned char xor1 = 0x01; // break the polynomial into 3 bytes
	unsigned char xor2 = 0b00010000;
	unsigned char xor3 = 0b00100001;

	data_buffer[0] ^= xor1;
	data_buffer[1] ^= xor2;
	data_buffer[2] ^= xor3;

	// corruption by introduce error function
	//IntroduceError(data_buffer, 1); //Will introduce error into data with probability .0001
	
	sprintf(message, "After Introduce Error: %s", data_buffer);
	info(message);

	//run crc check
	//the result is somewhat counterintuitive
	//return 1 means no error detected
	//return 0 means error detected
	if(crc_check(data_buffer, data_length, polynomial) == 0)
	{
		printf("error found\n");
	}
	else
	{
		printf("no error found\n");
	}	
}*/
