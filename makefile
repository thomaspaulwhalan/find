CC=gcc

CFLAGS=-I . -Wall --pedantic

all: findf

findf: main.c
	$(CC) $(CFLAGS) main.c -o findf

clean:
	rm findf
