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

typedef struct {
    Elf32_Rel rel; /* relocation entry */
    Elf32_Word link; /* link */
} RelAndLink;

/* ----- ENTETE ELF ----- */
Elf32_Ehdr createObjectEnteteELF(FILE* fichierAnalyse);
void afficheTableEnTete(Elf32_Ehdr enTeteHeader);

/* ----- READ HEADER ----- */
char* getSectionsStringTable(FILE* fichierAnalyse, Elf32_Ehdr elfHdr);
Elf32_Shdr createObjectSectionheader(FILE* fichierAnalyse, int index, Elf32_Ehdr elfHdr);
void displaySectionHeader(FILE* fichierAnalyse, Elf32_Ehdr elfHdr, Elf32_Shdr* allSectHdr);
Elf32_Shdr* createAllObjectSectionHeader(FILE* fichierAnalyse, Elf32_Ehdr elfHdr);

/* ----- DISPLAY CONTENT ----- */
int getIndexSectionByNameOrIndex(FILE* fichierAnalyse, Elf32_Ehdr elfHdr, char *indiceOrNameSection, int isInt, Elf32_Shdr* allSectHdr);
unsigned char* createSectionContent(FILE* fichierAnalyse, Elf32_Ehdr elfHdr, int indiceSectionHeader);
void displaySectionContent(unsigned char* tableauOctetsSection, FILE* fichierAnalyse, int indiceSectionHeader, Elf32_Ehdr elfHdr);

/* ----- SYMBOL TABLE ----- */
Elf32_Sym createObjectSymbol(FILE* fichierAnalyse, int index, Elf32_Ehdr elfHdr, Elf32_Shdr* sectiontab);
Elf32_Sym* createAllObjectSymbol(FILE* fichierAnalyse, Elf32_Ehdr elfHdr, Elf32_Shdr* sectiontab);
void afficherTableSymbole(FILE* fichierAnalyse, Elf32_Ehdr elfHdr, Elf32_Shdr* sectiontab, Elf32_Sym * allSymbole);

/* ----- RELOCATIONS TABLE ----- */

Elf32_Rel createObjectRelocations(FILE* fichierAnalyse, Elf32_Shdr sect, int index, Elf32_Ehdr elfHdr);
RelAndLink * createAllRelocationBySection(FILE* fichierAnalyse, int nbent, Elf32_Shdr sect, Elf32_Ehdr elfHdr);
void readRelocations(FILE* fichierAnalyse, Elf32_Ehdr elfHdr, Elf32_Shdr * allSectHdr);
RelAndLink** createAllRelocations(FILE* fichierAnalyse, Elf32_Ehdr elfHdr, Elf32_Shdr * allSectHdr);
void affichageRelocations(RelAndLink** allRel, int* tab_ind_sect_rel, int nb_sect_rel, FILE* fichierAnalyse, Elf32_Ehdr elfHdr);


#endif	/* READELF_PERSO_H */
