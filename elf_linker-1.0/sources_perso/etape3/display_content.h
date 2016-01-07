/* 
 * File:   read_header.h
 * Author: ducruyy
 *
 * Created on 5 janvier 2016, 09:03
 */

#ifndef DISPLAY_CONTENT_H
#define	DISPLAY_CONTENT_H

#include <stdio.h>
#include <stdlib.h>
#include <byteswap.h>
#include <elf.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <inttypes.h>

int getIndexSectionByNameOrIndex(char* nameFile, char* indiceOrNameSection, int isInt);

void createSectionContent(char* nameFile, int indiceSectionHeader, char* tab);


#endif	/* READ_HEADER_H */

