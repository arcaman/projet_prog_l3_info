
/* 
 * File:   test_relocations.c
 * Author: bochatom
 *
 * Created on 6 janvier 2016, 10:51
 */

#include "renumerotation.h"

int main(int argc,char* argv[]){
    char *f=argv[1];
    printf("%s\n",f);
    renumerotation(f);
    return(EXIT_SUCCESS);
}

