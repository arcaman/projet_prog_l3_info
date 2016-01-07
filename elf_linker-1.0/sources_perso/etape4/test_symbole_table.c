/* 
 * File:   test.c
 * Author: rocheroh
 *
 * Created on 6 janvier 2016, 13:36
 */

#include <stdio.h>
#include <stdlib.h>
#include "symbole_table.h"

/*
 * 
 */
int main(int argc, char** argv) {
    afficherTableSymbole(argv[1]);
    return (EXIT_SUCCESS);
}

