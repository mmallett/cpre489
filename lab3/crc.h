/*
 * Matt Mallett mmallett@iastate.edu
 * Adam Sunderman asu1351@iastate.edu
 * CprE 489 Lab 3
 *
 * crc.h
 * CRC generator and checker definitions
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

////////////////////////////////////////////
// DATA MODULE
// Keyboard IO
////////////////////////////////////////////

// maximum number of characters program will accept
#define DATA_BUFFER_SIZE 1024

// buffer to hold input data
signed char * data_buffer;

// length of aforementioned data buffer
int input_length;

// construct the data buffer
void init_data_buffer();

// read data from keyboard into data buffer
void read_data();

// wipe all data presently in data buffer
void clear_data();

//////////////////////////////////////////
// CRC MODULE
// crc calculation and checking
//////////////////////////////////////////

// generate a crc field for a given data buffer
//    char* data the data to generate the CRC for
//    int16_t crc return pointer for the generated crc
//    returns the calculated crc
uint32_t crc_gen(char* data, int data_length, uint32_t gen_poly);

// check if the crc is valid for the given data
//    char* data the data to check crc against
//    int16_t crc crc code used to check with
//    returns 1 if valid 0 if invalid
int crc_check(char* data, int data_length,  uint32_t gen_poly);

// compute crc checksum
uint32_t crc_alg(char* data, int data_length, uint32_t gen_poly);

//introduces error bits into data with probability p
void IntroduceError(char *data, double p);

//////////////////////////////////////////
// NICE TO HAVES
// general utilities
//////////////////////////////////////////

int DEBUG = 1;

//formats input prompt printing
void prompt(char*);

//formats information printing
void info(char*);

//formats debug printing, toggled off by setting DEBUG = 0
void debug(char*);

//formats error printing
void error(char*);
