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


////////////////////////////////////////////
// DATA MODULE
// Keyboard IO
////////////////////////////////////////////

// maximum number of characters program will accept
#define DATA_BUFFER_SIZE 1024

// buffer to hold input data
char * data_buffer;

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
//    char* crc return pointer for the generated crc
//    returns the char* crc argument
char* crc_gen(char* data, char* crc);

// check if the crc is valid for the given data
//    char* data the data to check crc against
//    char* crc crc code used to check with
//    returns 1 if valid 0 if invalid
int crc_check(char* data, char* crc);

//////////////////////////////////////////
// NICE TO HAVES
// general utilities
//////////////////////////////////////////

int DEBUG = 1;

void prompt(char*);

void info(char*);
