/* 
 * File:   read_relocations.h
 * Author: bochatom
 *
 * Created on 6 janvier 2016, 09:34
 */

#ifndef READ_RELOCATIONS_H
#define	READ_RELOCATIONS_H


#include <stdio.h>
#include <stdlib.h>
#include <byteswap.h>
#include <elf.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <inttypes.h>


typedef struct
{
  Elf32_Rel	rel;		/* relocation entry */
  Elf32_Word	link;		/* link */
} RelAndLink;


Elf32_Rel createObjectRelocations(char* nameFile, Elf32_Shdr sect, int index);
RelAndLink* createAllRelocationBySection(char* nameFile, int nbent, Elf32_Shdr sect);
void readRelocations(char * nameFile);
RelAndLink** createAllRelocations(char * nameFile);
void affichageRelocations(RelAndLink** allRel, int* tab_ind_sect_rel, int nb_sect_rel, char* nameFile);

#endif	/* READ_RELOCATIONS_H */

