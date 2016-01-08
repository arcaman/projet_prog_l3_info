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
#include "../etape2/read_header.c"

Elf32_Rel createObjectRelocations(char* nameFile, Elf32_Shdr sect, int index);
void createAllRelocationBySection(char* nameFile, Elf32_Rel* tab, int shnum, Elf32_Shdr sect, int ind_tab);
void read_relocations(char * nameFile);
int size_needed_relocations_table(Elf32_Ehdr elfHdr, Elf32_Shdr* allSectHdr);
void affichage_relocations(Elf32_Rel* allRel, Elf32_Shdr* allSect, int* tab_ind_sect_rel, int nb_ent_tot, int nb_sect_rel);
int count_reloc_sect(Elf32_Ehdr elfHdr, Elf32_Shdr* allSectHdr);

#endif	/* READ_RELOCATIONS_H */

