#include "../etape2/read_header.c"

int getIndexSectionByNameOrIndex(char* nameFile, char *indiceOrNameSection, int isInt) {

    int idx;
    Elf32_Ehdr elfHdr = createObjectEnteteELF(nameFile);

    int nbSections = elfHdr.e_shnum;

    if (isInt == 1) {
        if (atoi(indiceOrNameSection) < 0 || atoi(indiceOrNameSection) > nbSections - 1) {
            return -1;
        }
        return atoi(indiceOrNameSection);
    }


    FILE* fichier = fopen(nameFile, "r");
    Elf32_Shdr strTab = createObjectSectionheader(nameFile, elfHdr.e_shstrndx);
    fseek(fichier, strTab.sh_offset, SEEK_SET);
    char * str = malloc(strTab.sh_size);
    for (idx = 0; idx < strTab.sh_size; idx++) {
        str[idx] = fgetc(fichier);
    }
    fclose(fichier);
    Elf32_Shdr* allSectHdr = malloc(elfHdr.e_shnum * sizeof (Elf32_Shdr));
    createAllObjectSectionHeader(nameFile, allSectHdr, elfHdr.e_shnum);
    for (idx = 0; idx < elfHdr.e_shnum; idx++) {
        int i = allSectHdr[idx].sh_name;
        int n = 0;
        int checkChaineEgale = 0;

        if (str[i] != '\0') {
            checkChaineEgale = 1;
        }
        while (str[i] != '\0' && checkChaineEgale == 1) {
            if (str[i] != indiceOrNameSection[n]) {
                checkChaineEgale = 0;
            }
            i++;
            n++;
        }
        if (checkChaineEgale == 1) {
            return idx;
        }
    }
    return -1;
}

void createSectionContent(char* nameFile, int indiceSectionHeader, char* tab) {
    Elf32_Shdr shdr = createObjectSectionheader(nameFile, indiceSectionHeader);
    FILE* fichierAnalyse = fopen(nameFile, "r");

    fseek(fichierAnalyse, shdr.sh_offset, SEEK_SET);

    int i, j = 0;
    uint16_t k;
    for (i = 0; i < shdr.sh_size; i++) {
        fread(&k, 1, 1, fichierAnalyse);
        printf("%x ", k);
        //decalage de bits pour etre sur de recuperer la bonne valeur
        j++;
        if (j == 8) {
            printf(" ");
        } else if (j == 16) {
            printf("\n");
            j = 0;
        }
    }


    
    //    fseek(f, offset, SEEK_SET);
    //    int i, j = 0;
    //    unsigned char k;
    //    for (i = size; i > 0; i--) {
    //        fread(&k, 1, 1, f);
    //        printf("%02x ", k);
    //        j++;
    //        if (j == 8) {
    //            printf(" ");
    //        } else if (j == 16) {
    //            printf("\n");
    //            j = 0;
    //        }
    //    }


    fclose(fichierAnalyse);

}

//void displaySectionContent(FILE *, int offset, int size) {
//
//
//}

//void displaySectionContent(FILE * f, int offset, int size) {
//    
//    
//    
//    fseek(f, offset, SEEK_SET);
//    int i, j = 0;
//    unsigned char k;
//    for (i = size; i > 0; i--) {
//        fread(&k, 1, 1, f);
//        printf("%02x ", k);
//        j++;
//        if (j == 8) {
//            printf(" ");
//        } else if (j == 16) {
//            printf("\n");
//            j = 0;
//        }
//    }
//}