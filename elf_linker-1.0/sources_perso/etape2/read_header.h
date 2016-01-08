/* 
 * File:   read_header.h
 * Author: ducruyy
 *
 * Created on 5 janvier 2016, 09:03
 */

#ifndef READ_HEADER_H
#define	READ_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <byteswap.h>
#include <elf.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <inttypes.h>

char* getSectionsStringTable(char* nameFile);
Elf32_Shdr createObjectSectionheader(char* nameFile, int index);
void displaySectionHeader(char* nameFile, Elf32_Shdr* allSectHdr);
Elf32_Shdr* createAllObjectSectionHeader(char* nameFile);
#endif	/* READ_HEADER_H */

