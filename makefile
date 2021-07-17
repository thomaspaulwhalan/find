CC=gcc

CFLAGS=-I . -Wall --pedantic

all: find

find: main.c
	$(CC) $(CFLAGS) main.c -o find

clean:
	rm find
