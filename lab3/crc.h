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
// NICE TO HAVES
// general utilities
//////////////////////////////////////////

int DEBUG = 1;

void log_info(char*);
