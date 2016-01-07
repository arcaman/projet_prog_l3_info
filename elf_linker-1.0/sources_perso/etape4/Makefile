## -*- Makefile -*-
##
## User: ducruyy
## Time: 5 janv. 2016 10:49:17
##

CC=gcc
CFLAGS=-Wall -Werror

test_symbole_table : test_symbole_table.o symbole_table.o ../etape2/read_header.o
	$(CC) -o $@ $^

%.o : %.c %.h
	$(CC) $(CFLAGS) -c $<


