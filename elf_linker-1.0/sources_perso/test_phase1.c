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
    if(argc==1) {printf("Erreur : saisissez un argument !\n");
        return 1;}
    char *nameFile = argv[1];
    int retry = 1;
    

    FILE* fichierAnalyse = fopen(nameFile, "r");

    Elf32_Ehdr elfHdr = createObjectEnteteELF(fichierAnalyse);
    Elf32_Shdr* allSectHdr = createAllObjectSectionHeader(fichierAnalyse, elfHdr);
    Elf32_Sym* allObjectSymbol = createAllObjectSymbol(fichierAnalyse, elfHdr, allSectHdr);

    while (retry) {
        printf("\nEntrez le numero correspondant aux informations a afficher pour %s\n", nameFile);
        printf("1 - Entete\n2 - Section header\n3 - Display Content\n4 - Symbole table\n5 - Relocations table\n6 - Exit\n\n");
        int sel = 0;
        scanf("%d", &sel);
        retry = 1;
        switch (sel) {

            case 1: //entete
            {

                afficheTableEnTete(elfHdr);
            }
                break;

            case 2: //read header
            {
                displaySectionHeader(fichierAnalyse, elfHdr, allSectHdr);
            }
                break;

            case 3: //display content
            {
                //                char* indiceOrNameSection = malloc(20*sizeof(char));
                //                int isInt;
                //                printf("Indiquez un indice ou un nom de section :\n");
                //                scanf("Indiquez un indice ou un nom de section %s",indiceOrNameSection);
                //                printf("Indiquez 1 si c'est un indice ou 0 si c'est un nom de section : \n");
                //                scanf("%d",&isInt);
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
                afficherTableSymbole(fichierAnalyse, elfHdr, allSectHdr, allObjectSymbol);
                break;

            case 5: //relocations table
                readRelocations(fichierAnalyse, elfHdr, allSectHdr);
                break;
                
            case 6: //Exit the program
                ;
                retry = 0;
                break;

            default: //redemande ce qu'il faut afficher si sel a une autre valeur
            {
                printf("Veuillez réessayer avec un entier compris entre 1 et 5.\n\n");
            }
        }
    }

    fclose(fichierAnalyse);

    return (EXIT_SUCCESS);
}

