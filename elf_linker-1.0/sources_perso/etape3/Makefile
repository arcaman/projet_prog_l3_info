## -*- Makefile -*-
##
## User: cgil
## Time: 5 janv. 2016 10:49:17
##
	
CC=gcc
CFLAGS=-Wall -Werror

programme: display_content.o test_display_content.o
	$(CC) -o test_display_content display_content.o test_display_content.o

display_content.o: display_content.c display_content.h
	$(CC) $(CFLAGS) -c display_content.c
	
test_display_content.o: test_display_content.c
	$(CC) $(CFLAGS) -c test_display_content.c

