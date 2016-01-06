## -*- Makefile -*-
##
## User: ducruyy
## Time: 5 janv. 2016 10:49:17
##

CC=gcc
CFLAGS=-Wall -Werror

test : test.o read_header.o
	$(CC) -o $@ $^

%.o : %.c %.h
	$(CC) $(CFLAGS) -c $<


