# Makefile for common.a

L = ../libcs50

# object files and the target library
OBJS = pagedir.o
LIB = common.a
LLIBS = $L/libcs50.a

MAKE = make
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I/$L
FLAGS = # -DMEMTEST # -DNOSLEEP
CC = gcc

# dependencies: object files depend on header files
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

pagedir.o: pagedir.h

.PHONY: all clean

# clean up after our compilation
clean:
	rm -f core
	rm -f $(LIB) *~ *.o
