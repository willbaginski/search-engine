# Makefile for crawler

L = ../libcs50
C = ../common

# object files and the target library
OBJS = crawler.o
PROG = crawler
LLIBS = $C/common.a $L/libcs50.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I$L -I$C
FLAGS = # -DMEMTEST # -DNOSLEEP
TESTFLAG = -DMYTEST
CC = gcc
MAKE = make

# dependencies: object files depend on header files
$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) -DMYTEST $^ -o $@

.PHONY: all clean

# clean up after our compilation
clean:
	rm -f core
	rm -f $(LIB) *~ *.o
	rm -rf */
