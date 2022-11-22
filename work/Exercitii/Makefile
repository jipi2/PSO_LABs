CC=gcc
CPP=g++

CFLAGS=-Wall -O3

LDFLAGS=

.PHONY: all clean

all: mmx brighten  brighten_sse

brighten: brighten.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

brighten_sse: brighten_sse.c
	$(CC) $^ -o $@


clean:
	-rm -rf mmx brighten brighten_sse
