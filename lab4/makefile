CC=gcc
CFLAGS=-c -Wall

all: go_back_n

go_back_n: go_back_n.c go_back_n.h crc.o crc.h
	$(CC) -o go_back_n go_back_n.c go_back_n.h crc.o crc.h -lpthread

crc.o: crc.c crc.h
	$(CC) $(CFLAGS) crc.c crc.h

clean:
	rm -rf *o go_back_n
