CC=gcc
CFLAGS=-Wall -Werror

phase_un: readelf_perso.o test_phase1.o
	$(CC) -o phase_un readelf_perso.o test_phase1.o

readelf_perso.o: readelf_perso.c readelf_perso.h
	$(CC) $(CFLAGS) -c readelf_perso.c
	
test_phase1.o: test_phase1.c
	$(CC) $(CFLAGS) -c test_phase1.c