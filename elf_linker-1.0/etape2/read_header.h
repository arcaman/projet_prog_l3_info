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
#define MODE_BIG_ENDIAN 2

void read_section_header(char * filename, size_t size);

Elf32_Ehdr read_elf_header(char *filename);

#endif	/* READ_HEADER_H */

