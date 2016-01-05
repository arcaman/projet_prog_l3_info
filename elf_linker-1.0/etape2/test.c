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

off_t fsize(const char *filename) {
    struct stat st; 

    if (stat(filename, &st) == 0)
        return st.st_size;

    return -1; 
}
int main(int argc,char** argv){
    char *f=argv[1];
    size_t s=fsize(f);
    read_header(f,s);
    return(EXIT_SUCCESS);
}
