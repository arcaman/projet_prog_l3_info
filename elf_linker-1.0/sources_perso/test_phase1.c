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
    while(retry) {
        printf("Entrez le numero correspondant aux informations a afficher pour %s\n",nameFile);
        printf("1 - Entete\n2 - Section header\n3 - Display Content\n4 - Symbole table\n5 - Relocations table\n\n");
        int sel = 0;
        scanf("%d", &sel);
        retry = 0;
        switch(sel) {

            case 1 : //entete
            {
                Elf32_Ehdr objet = createObjectEnteteELF(nameFile);   
                afficheTableEnTete(objet); 
            }
                break;

            case 2 : //read header
            {
                Elf32_Shdr* allSectHdr = createAllObjectSectionHeader(nameFile);
                displaySectionHeader(nameFile, allSectHdr);
            }
                break;

            case 3 : //display content
            {
                if (argc != 4) {
                    printf("Il n y a pas suffisamment d arguments pour afficher le contenu\n");
                } else {
                    char* indiceOrNameSection = argv[2];
                    int isInt = atoi(argv[3]);
                    int idSection = getIndexSectionByNameOrIndex(nameFile, indiceOrNameSection, isInt);
                    unsigned char* tableauOctetsSection = createSectionContent(nameFile, idSection);
                    displaySectionContent(tableauOctetsSection, nameFile, idSection);
                }
            }           
                break;

            case 4 : //symbol table
                afficherTableSymbole(nameFile);
                break;

            case 5 : //relocations table
                readRelocations(nameFile); 
                break;
                
            default : //redemande ce qu'il faut afficher si sel a une aurte valeur
            {
                printf("Veuillez réessayer avec un entier compris entre 1 et 5.\n\n");
                retry = 1;
            }
        }
    }
    return (EXIT_SUCCESS);
}

