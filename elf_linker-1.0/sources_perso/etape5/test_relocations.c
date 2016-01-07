/* 
 * File:   test_relocations.c
 * Author: bochatom
 *
 * Created on 6 janvier 2016, 10:51
 */

#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "read_relocations.h"

int main(int argc,char* argv[]){
    char *f=argv[1];
    printf("%s\n",f);
    read_relocations(f);
    return(EXIT_SUCCESS);
}

