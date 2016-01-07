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


void read_section_header(char * filename);
Elf32_Ehdr read_elf_header(char *filename);
//void read_section_content(FILE f, int offset, int size);
void createAllObjectSectionHeader(char* nameFile, Elf32_Shdr* tab, int shnum);
#endif	/* READ_HEADER_H */

