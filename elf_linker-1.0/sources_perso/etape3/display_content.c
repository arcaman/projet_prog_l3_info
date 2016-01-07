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
    Elf32_Shdr* allSectHdr = createAllObjectSectionHeader(nameFile);
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

unsigned char* createSectionContent(char* nameFile, int indiceSectionHeader) {
    Elf32_Shdr shdr = createObjectSectionheader(nameFile, indiceSectionHeader);
    FILE* fichierAnalyse = fopen(nameFile, "r");

    fseek(fichierAnalyse, shdr.sh_offset, SEEK_SET);

    unsigned char* tabOctets = malloc(sizeof (unsigned char)*shdr.sh_size);

    int i;
    unsigned char valeurOctet;
    for (i = 0; i < shdr.sh_size; i++) {
        fread(&valeurOctet, 1, 1, fichierAnalyse);
        tabOctets[i] = valeurOctet;
    }
    fclose(fichierAnalyse);
    return tabOctets;
}

void displaySectionContent(unsigned char* tableauOctetsSection, char* nameFile, int indiceSectionHeader) {

    Elf32_Shdr shdr = createObjectSectionheader(nameFile, indiceSectionHeader);
    int i, j = 0;
    for (i = 0; i < shdr.sh_size; i++) {
        printf("%02x", tableauOctetsSection[i]);
        j++;
        if (j == 4 || j == 8 || j == 12) {
            printf(" ");
        } else if (j == 16) {
            printf("\n");
            j = 0;
        }
    }
    printf("\n");
}