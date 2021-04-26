#!/bin/bash

# Makefile make me fast and accurate

CC = gcc
EXE = program
EXE_ARGS = ./input/test_input_01.in

CFLAGS = -Wall -O3 -Wno-error -std=c99
LFLAGS = 

SRC = program.c
OBJ = $(SRC:.c=.o)

.PHONY: clean

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) $(LFLAGS) -o $(EXE)

$(OBJ): $(SRC)
	$(CC) -c $(SRC) $(CFLAGS)

clean: 
	rm -rf $(OBJ) $(EXE)

run_test: $(EXE)
	./$(EXE) $(EXE_ARGS)

# End of Makefile



