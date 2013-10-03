CC=gcc
CFLAGS=-c -Wall

all: go_back_n

go_back_n: go_back_n.o go_back_n.h crc.o crc.h
	$(CC) -o go_back_n go_back_n.o crc.o crc.o -lpthread

go_back_n.o: crc.o go_back_n.c go_back_n.h crc.h
	$(CC) $(CFLAGS) go_back_n.c crc.o

crc.o: crc.c crc.h
	$(CC) $(CFLAGS) crc.c

clean:
	rm -rf *o go_back_n
