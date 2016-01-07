/*
 * File: test.c
 * Author: ducruyy
 *
 * Created on 5 janvier 2016, 08:25
 */

#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "read_header.h"
#include <sys/stat.h>

int main(int argc, char* argv[]) {

    char *nameFile = argv[1];
    Elf32_Shdr* allSectHdr = createAllObjectSectionHeader(nameFile);
    printf("%s\n", nameFile);
    displaySectionHeader(nameFile, allSectHdr);
    return (EXIT_SUCCESS);
}
