CC=gcc
CFLAGS=-c -Wall

all: go_back_n

go_back_n: go_back_n.o crc.o
	$(CC) crc.o -o go_back_n go_back_n.o -lpthread

go_back_n.o: go_back_n.c crc.o
	$(CC) $(CFLAGS) go_back_n.c -lpthread

crc.o: crc.c
	$(CC) $(CFLAGS) crc.c

clean:
	rm -rf *o go_back_n
