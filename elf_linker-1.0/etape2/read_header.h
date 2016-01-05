/* 
 * File:   read_header.h
 * Author: ducruyy
 *
 * Created on 5 janvier 2016, 09:03
 */

#ifndef READ_HEADER_H
#define	READ_HEADER_H

#include<stdio.h>
#include<stdlib.h>
#include<elf.h>
#include <sys/mman.h>
#include <fcntl.h>
void read_header(Elf32_Shdr * section_headers);



#endif	/* READ_HEADER_H */

