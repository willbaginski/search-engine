# Makefile for querier

L = ../libcs50
C = ../common

# object files and target library
OBJS = querier.o
PROG = querier
LLIBS = $C/common.a $L/libcs50.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$C
CC = gcc
MAKE = make

# dependencies: object files depend on header files
all: querier fuzzquery

querier: querier.o $(LLIBS)
	$(CC) $(CFLAGS) $^ -o $@

fuzzquery: fuzzquery.o ../libcs50/libcs50.a
	$(CC) -Wall -pedantic -std=gnu11 -ggdb -I$L $^ -o $@

.PHONY = all clean

test:
	./testing.sh &> testing.out

# clean up
clean:
	rm -f core
	rm -f *~ *.o
