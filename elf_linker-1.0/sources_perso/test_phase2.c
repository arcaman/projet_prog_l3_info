/* 
 * File:   test_phase1.c
 * Author: bochatom
 *
 * Created on 8 janvier 2016, 14:15
 */

#include <stdio.h>
#include <stdlib.h>
#include "readelf_perso.h"

/*
 * 
 */
int main(int argc, char** argv) {
    char *nameFile = argv[1];
    
    FILE* fichierAnalyse = fopen(nameFile, "r");

    Elf32_Ehdr elfHdr = createObjectEnteteELF(fichierAnalyse);
    Elf32_Shdr* allSectHdr = createAllObjectSectionHeader(fichierAnalyse, elfHdr);
    Elf32_Sym* allObjectSymbol = createAllObjectSymbol(fichierAnalyse, elfHdr, allSectHdr);
    Elf32_Phdr programHdr = createObjectProgramHeader(fichierAnalyse, elfHdr);


    Elf32_Ehdr elfHdrSansRelocalisations;
    Elf32_Shdr* objSectHdrSansRelocalisations = createObjectSectionHeaderWithoutRelocalisations(allSectHdr, elfHdr, &elfHdrSansRelocalisations);
    //printf("nb sections non relocalises : %d\n\n\n", elfHdrSansRelocalisations.e_shnum);
    displaySectionHeader(fichierAnalyse, elfHdrSansRelocalisations, objSectHdrSansRelocalisations);
    Elf32_Ehdr elfHdrRelocalisations = elfHdr;
    Elf32_Shdr* objSectHdrAvecRelocalisations = createObjectSectionHeaderRelocalisations(elfHdr, allSectHdr, &elfHdrRelocalisations);
    displaySectionHeader(fichierAnalyse, elfHdrRelocalisations, objSectHdrAvecRelocalisations);
    int* tabComparaisonSymboles = createTableComparaisonSymbolesApresRelocation(elfHdr, allSectHdr);
    Elf32_Sym* tabSymbolesRelocalise = creationTableDesSymbolesCorrecte(fichierAnalyse, allObjectSymbol, tabComparaisonSymboles, elfHdr, allSectHdr, argc, argv);
    afficherTableSymbole(fichierAnalyse, elfHdr, allSectHdr, tabSymbolesRelocalise);
    unsigned char** tableauAllSectionContent = createAllObjectSectionContent(fichierAnalyse, elfHdrSansRelocalisations);
    printf("original:\n");
    unsigned char** tableauSectionRelocation = replaceAllSectionsContent(fichierAnalyse, allSectHdr, elfHdrSansRelocalisations, tabSymbolesRelocalise);

    int j;
    for (j = 0; j < elfHdrSansRelocalisations.e_shnum; j++) {
        printf("\ncontenu de la section %d :\n", j);
        printf("original:\n");
        displaySectionContent(tableauAllSectionContent[j], fichierAnalyse, j, elfHdrSansRelocalisations);
        printf("modifie:\n");
        displaySectionContent(tableauSectionRelocation[j], fichierAnalyse, j, elfHdrSansRelocalisations);
    }


    printf("creation du fichier executable a partir de maintenant :\n");
    FILE* fichierExecutable = fopen("sortie_executable", "w");
    creationFichierExecutable(fichierExecutable, elfHdrSansRelocalisations, programHdr, objSectHdrSansRelocalisations, tabSymbolesRelocalise, tableauSectionRelocation);
    fclose(fichierExecutable);
    printf("la creation doit etre effectue");

    fclose(fichierAnalyse);

    return (EXIT_SUCCESS);
}

