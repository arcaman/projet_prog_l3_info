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

Elf32_Rel createObjectRelocations(char* nameFile, Elf32_Shdr sect, int index);
Elf32_Rel* createAllRelocationBySection(char* nameFile, int nbent, Elf32_Shdr sect);
void readRelocations(char * nameFile);
void affichage_relocations(Elf32_Rel** allRel, int* tab_ind_sect_rel, int nb_ent_tot, int nb_sect_rel, char* nameFile);

#endif	/* READ_RELOCATIONS_H */

