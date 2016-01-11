## -*- Makefile -*-
##
## User: cgil
## Time: 5 janv. 2016 10:49:17
##
	
CC=gcc
CFLAGS=-Wall -Werror

programme: en_tete_elf.o test_en_tete_elf.o
	$(CC) -o en_tete_elf en_tete_elf.o test_en_tete_elf.o

en_tete_elf.o: en_tete_elf.c en_tete_elf.h
	$(CC) $(CFLAGS) -c en_tete_elf.c

test_en_tete_elf.o: test_en_tete_elf.c
	$(CC) $(CFLAGS) -c test_en_tete_elf.c	


