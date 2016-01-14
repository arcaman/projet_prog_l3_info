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
#include <string.h>
#include <stdlib.h>
#include <byteswap.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <inttypes.h>



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
unsigned char** createAllObjectSectionContent(FILE* fichierAnalyse, Elf32_Ehdr elfHdr);
void displaySectionContent(unsigned char* tableauOctetsSection, FILE* fichierAnalyse, int indiceSectionHeader, Elf32_Ehdr elfHdr);

/* ----- SYMBOL TABLE ----- */
Elf32_Sym createObjectSymbol(FILE* fichierAnalyse, int index, Elf32_Ehdr elfHdr, Elf32_Shdr* sectiontab);
Elf32_Sym* createAllObjectSymbol(FILE* fichierAnalyse, Elf32_Ehdr elfHdr, Elf32_Shdr* sectiontab);
void afficherTableSymbole(FILE* fichierAnalyse, Elf32_Ehdr elfHdr, Elf32_Shdr* sectiontab, Elf32_Sym * allSymbole);

/* ----- RELOCATIONS TABLE ----- */

Elf32_Rel createObjectRelocations(FILE* fichierAnalyse, Elf32_Shdr sect, int index, Elf32_Ehdr elfHdr);
Elf32_Rel * createAllRelocationBySection(FILE* fichierAnalyse, int nbent, Elf32_Shdr sect, Elf32_Ehdr elfHdr);
void readRelocations(FILE* fichierAnalyse, Elf32_Ehdr elfHdr, Elf32_Shdr * allSectHdr);
Elf32_Rel** createAllRelocations(FILE* fichierAnalyse, Elf32_Ehdr elfHdr, Elf32_Shdr * allSectHdr);
void affichageRelocations(Elf32_Rel** allRel, int* tabIndSectRel, int nbSectRel, FILE* fichierAnalyse, Elf32_Ehdr elfHdr);

/* ----- EDITION OBJET SANS RELOCALISATION -----*/
Elf32_Shdr* createObjectSectionHeaderWithoutRelocalisations(Elf32_Shdr* shdr, Elf32_Ehdr elf, Elf32_Ehdr* elfApresReloc);

int countNbSectionsNonRelocalisesByAllSectionHeader(Elf32_Ehdr elfHdr, Elf32_Shdr* allSectHdr);

Elf32_Shdr* createObjectSectionHeaderRelocalisations(Elf32_Ehdr elfHdr, Elf32_Shdr* allSectHdr, Elf32_Ehdr* elfHdrRelocalisations);

int countNbSectionsRelocalisesByAllSectionHeader(Elf32_Ehdr elfHdr, Elf32_Shdr* allSectHdr);

int* createTableComparaisonSymbolesApresRelocation(Elf32_Ehdr elfHdr, Elf32_Shdr* allSectHdr);

Elf32_Sym* creationTableDesSymbolesCorrecte(FILE* fichierAnalyse, Elf32_Sym* allObjectSymbol, int* tabComparaisonSymboles, Elf32_Ehdr elfHdr, Elf32_Shdr* sectiontab, int argc, char** argv);

u_int32_t convertCharToHexadecimal(char* valueHexaString);

/* ----- RELOCALISATION SYMBOLES -----*/

unsigned char * replaceSectionContent(FILE* fichierAnalyse, Elf32_Shdr* shdr, Elf32_Ehdr elfHdr, int indiceSection, Elf32_Sym* SymbolesCorrects);
unsigned char** replaceAllSectionsContent(FILE* fichierAnalyse, Elf32_Shdr* shdr, Elf32_Ehdr elfHdr, Elf32_Sym* SymbolesCorrects);
#endif	/* READELF_PERSO_H */

Elf32_Phdr createObjectProgramHeader(FILE* fichierAnalyse, Elf32_Ehdr elfHdr);

void creationFichierExecutable(FILE* fichierExecutable, Elf32_Ehdr elfHdrSansRelocalisations, Elf32_Phdr programHdr, Elf32_Shdr* objSectHdrSansRelocalisations, Elf32_Sym* tabSymbolesRelocalise, unsigned char** tableauSectionRelocation, int nbSecSupprimees);