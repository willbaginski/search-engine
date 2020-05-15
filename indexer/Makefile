# Makefile for indexer

L = ../libcs50
C = ../common

# the target library
LLIBS = $C/common.a $L/libcs50.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I$C -I$L
FLAGS = # -DMEMTEST # -DNOSLEEP
TESTFLAG = -DMYTEST
CC = gcc
MAKE = make

# dependencies: object files depend on header files
all: indexer indextest

test:
	bash -v ./testing.sh

indexer: indexer.o $(LLIBS)
	$(CC) $(CFLAGS) $^ -o $@

indextest: indextest.o $(LLIBS)
	$(CC) $(CFLAGS) $^ -o $@ 

.PHONY: all clean

# clean up after our compilation
clean:
	rm -f core
	rm -f *~ *.o
	rm -f *.sorted
	rm -f testIndex testCopy
