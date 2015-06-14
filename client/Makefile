# PROJET client : Fichier Makefile
include ../../../Makefile.inc

PSE_DIR=../../..

CC = gcc

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
EXE=client

all: $(SRC) $(EXE)

$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDLIBS)
%.o: %.c %.h defs.h
	$(CC) -c $(CFLAGS) $< -o $@

main.o : servIO.h auction.h fileIO.h

clean :
	rm -f *.o $(EXE)
