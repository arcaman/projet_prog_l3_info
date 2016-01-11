/* 
 * File:   renumerotation.h
 * Author: jacqurap
 *
 * Created on 8 janvier 2016, 09:59
 */

#ifndef RENUMEROTATION_H
#define	RENUMEROTATION_H

#include <stdio.h>
#include <stdlib.h>
#include <byteswap.h>
#include <elf.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <inttypes.h>

void renumerotation(char* nameFile);

#endif	/* RENUMEROTATION_H */

