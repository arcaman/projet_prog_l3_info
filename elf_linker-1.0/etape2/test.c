/*
* File: test.c
* Author: ducruyy
*
* Created on 5 janvier 2016, 08:25
*/

#include<stdio.h>
#include<stdlib.h>
#include<elf.h>
#include <sys/mman.h>
#include <fcntl.h>
#include"read_header.h"

/*
*
*/
int main(int argc,char** argv){
    char *f=argv[1];
    size_t s=argv[2];
    read_header(f,s);
    return(EXIT_SUCCESS);
}
