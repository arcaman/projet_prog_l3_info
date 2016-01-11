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
    int retry = 1;

    FILE* fichierAnalyse = fopen(nameFile, "r");

    Elf32_Ehdr elfHdr = createObjectEnteteELF(fichierAnalyse);
    Elf32_Shdr* allSectHdr = createAllObjectSectionHeader(fichierAnalyse, elfHdr);

    while (retry) {
        printf("Entrez le numero correspondant aux informations a afficher pour %s\n", nameFile);
        printf("1 - Entete\n2 - Section header\n3 - Display Content\n4 - Symbole table\n5 - Relocations table\n6 - Generation sections\n\n");
        int sel = 0;
        scanf("%d", &sel);
        retry = 0;
        switch (sel) {

            case 1: //entete
            {

                afficheTableEnTete(elfHdr);
            }
                break;

            case 2: //read header
            {
                //Elf32_Shdr* allSectHdr = createAllObjectSectionHeader(fichierAnalyse, elfHdr);
                displaySectionHeader(fichierAnalyse, elfHdr, allSectHdr);
            }
                break;

            case 3: //display content
            {
                if (argc != 4) {
                    printf("Il n y a pas suffisamment d arguments pour afficher le contenu\n");
                } else {
                    char* indiceOrNameSection = argv[2];
                    int isInt = atoi(argv[3]);
                    int idSection = getIndexSectionByNameOrIndex(fichierAnalyse, elfHdr, indiceOrNameSection, isInt, allSectHdr);
                    unsigned char* tableauOctetsSection = createSectionContent(fichierAnalyse, elfHdr, idSection);
                    displaySectionContent(tableauOctetsSection, fichierAnalyse, idSection, elfHdr);
                }
            }
                break;

            case 4: //symbol table
                ; //on ne peut pas declarer une variable directement après un statement, d'ou la ligne vide
                Elf32_Sym* allObjectSymbol;
                allObjectSymbol = createAllObjectSymbol(fichierAnalyse, elfHdr, allSectHdr);
                afficherTableSymbole(fichierAnalyse, elfHdr, allSectHdr, allObjectSymbol);
                //afficherTableSymbole(fichierAnalyse);
                break;

            case 5: //relocations table
                ;

                readRelocations(fichierAnalyse, elfHdr, allSectHdr);


                break;

            case 6:
                ;
                Elf32_Shdr* objSectHdrSansRelocalisations = createObjectSectionHeaderWithoutRelocalisations(elfHdr, allSectHdr);
                int nbSectionsNonRelocaliseByAllSectionHeader = countNbSectionsNonRelocalisesByAllSectionHeader(elfHdr, allSectHdr);
                Elf32_Ehdr elfHdrSansRelocalisations = elfHdr;
                elfHdrSansRelocalisations.e_shnum = nbSectionsNonRelocaliseByAllSectionHeader;
                //printf("nb sections non relocalises : %d", nbSectionsNonRelocaliseByAllSectionHeader);
                displaySectionHeader(fichierAnalyse, elfHdrSansRelocalisations, objSectHdrSansRelocalisations);
                
                Elf32_Shdr* objSectHdrAvecRelocalisations = createObjectSectionHeaderRelocalisations(elfHdr, allSectHdr);
                int nbSectionsRelocaliseByAllSectionHeader = countNbSectionsRelocalisesByAllSectionHeader(elfHdr, allSectHdr);
                Elf32_Ehdr elfHdrRelocalisations = elfHdr;
                elfHdrRelocalisations.e_shnum = nbSectionsRelocaliseByAllSectionHeader;
                displaySectionHeader(fichierAnalyse, elfHdrRelocalisations, objSectHdrAvecRelocalisations);
                
                
                
                break;


            default: //redemande ce qu'il faut afficher si sel a une aurte valeur
            {
                printf("Veuillez réessayer avec un entier compris entre 1 et 6.\n\n");
                retry = 1;
            }
        }
    }

    fclose(fichierAnalyse);

    return (EXIT_SUCCESS);
}

