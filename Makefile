CC      = gcc
CFLAGS  = -std=c99 -Wall -g
RM      = rm -f
OUTDIR	= ./build

default: all

run: all
	$(OUTDIR)/main

all: main

main:
	$(CC) $(CFLAGS) -o $(OUTDIR)/main src/*.c
