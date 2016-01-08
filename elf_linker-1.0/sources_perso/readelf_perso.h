/* 
 * File:   readelf_perso.h
 * Author: bochatom
 *
 * Created on 8 janvier 2016, 13:57
 */

#ifndef READELF_PERSO_H
#define	READELF_PERSO_H

#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <byteswap.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <inttypes.h>

/* ----- ENTETE ELF ----- */
Elf32_Ehdr createObjectEnteteELF(char* nameFile);
void afficheTableEnTete(Elf32_Ehdr enTeteHeader);

/* ----- READ HEADER ----- */
char* getSectionsStringTable(char* nameFile);
Elf32_Shdr createObjectSectionheader(char* nameFile, int index);
void displaySectionHeader(char* nameFile, Elf32_Shdr* allSectHdr);
Elf32_Shdr* createAllObjectSectionHeader(char* nameFile);

/* ----- DISPLAY CONTENT ----- */
int getIndexSectionByNameOrIndex(char* nameFile, char* indiceOrNameSection, int isInt);
unsigned char* createSectionContent(char* nameFile, int indiceSectionHeader);
void displaySectionContent(unsigned char* tableauOctetsSection, char* nameFile, int indiceSectionHeader);

/* ----- SYMBOL TABLE ----- */
Elf32_Sym createObjectSymbolHeader(char* nameFile, int index, Elf32_Shdr shdr);
void afficherTableSymbole(char * filename);

/* ----- RELOCATIONS TABLE ----- */
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


#endif	/* READELF_PERSO_H */

