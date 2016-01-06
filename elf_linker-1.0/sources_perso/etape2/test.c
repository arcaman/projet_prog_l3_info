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

int main(int argc,char* argv[]){
    char *f=argv[1];
    printf("%s\n",f);
    read_section_header(f);
    return(EXIT_SUCCESS);
}
