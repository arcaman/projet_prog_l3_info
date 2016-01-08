#include "read_header.h"
#include "../en_tete_elf.c"
#define MODE_BIG_ENDIAN 2

Elf32_Shdr createObjectSectionheader(char* nameFile, int index) {
    Elf32_Shdr shdr;
    FILE* fichierAnalyse = fopen(nameFile, "r");
    Elf32_Ehdr elfHdr = createObjectEnteteELF(nameFile);
    fseek(fichierAnalyse, elfHdr.e_shoff + index * sizeof (Elf32_Shdr), SEEK_SET);

    fread(&shdr.sh_name, sizeof (Elf32_Word), 1, fichierAnalyse);
    fread(&shdr.sh_type, sizeof (Elf32_Word), 1, fichierAnalyse);
    fread(&shdr.sh_flags, sizeof (Elf32_Word), 1, fichierAnalyse);
    fread(&shdr.sh_addr, sizeof (Elf32_Addr), 1, fichierAnalyse);
    fread(&shdr.sh_offset, sizeof (Elf32_Off), 1, fichierAnalyse);
    fread(&shdr.sh_size, sizeof (Elf32_Word), 1, fichierAnalyse);
    fread(&shdr.sh_link, sizeof (Elf32_Word), 1, fichierAnalyse);
    fread(&shdr.sh_info, sizeof (Elf32_Word), 1, fichierAnalyse);
    fread(&shdr.sh_addralign, sizeof (Elf32_Word), 1, fichierAnalyse);
    fread(&shdr.sh_entsize, sizeof (Elf32_Word), 1, fichierAnalyse);

    if (elfHdr.e_ident[5] == MODE_BIG_ENDIAN) { // 5 correspondant à l'octet étant le big ou little
        shdr.sh_name = __bswap_32(shdr.sh_name);
        shdr.sh_type = __bswap_32(shdr.sh_type);
        shdr.sh_flags = __bswap_32(shdr.sh_flags);
        shdr.sh_addr = __bswap_32(shdr.sh_addr);
        shdr.sh_offset = __bswap_32(shdr.sh_offset);
        shdr.sh_size = __bswap_32(shdr.sh_size);
        shdr.sh_link = __bswap_32(shdr.sh_link);
        shdr.sh_info = __bswap_32(shdr.sh_info);
        shdr.sh_addralign = __bswap_32(shdr.sh_addralign);
        shdr.sh_entsize = __bswap_32(shdr.sh_entsize);
    }

    fclose(fichierAnalyse);
    return shdr;
}

Elf32_Shdr* createAllObjectSectionHeader(char* nameFile) {

    Elf32_Ehdr elfHdr = createObjectEnteteELF(nameFile);
    int nbSections = elfHdr.e_shnum;
    int i;
    Elf32_Shdr* allSectHdr = malloc(sizeof (Elf32_Shdr) * nbSections);
    for (i = 0; i < nbSections; i++) {
        allSectHdr[i] = createObjectSectionheader(nameFile, i);
    }
    return allSectHdr;
}

char* getSectionsStringTable(char* nameFile) {
    Elf32_Ehdr elfHdr = createObjectEnteteELF(nameFile);
    Elf32_Shdr stringTable = createObjectSectionheader(nameFile, elfHdr.e_shstrndx);
    FILE* fichier = fopen(nameFile, "r");
    fseek(fichier, stringTable.sh_offset, SEEK_SET);
    int idx;
    char* str = malloc(stringTable.sh_size);
    for (idx = 0; idx < stringTable.sh_size; idx++) {
        str[idx] = fgetc(fichier);
    }
    fclose(fichier);
    return str;
}

void displaySectionHeader(char* nameFile, Elf32_Shdr* allSectHdr) {

    uint32_t idx;
    Elf32_Ehdr elfHdr = createObjectEnteteELF(nameFile);
    // read ELF header, first thing in the file
    printf("nb sections : %i\n", elfHdr.e_shnum);

    //get and store the string table
    char* str = getSectionsStringTable(nameFile);

    // read all section headers

    for (idx = 0; idx < elfHdr.e_shnum; idx++) {
        printf("SECTION numero %i : \n", idx);
        printf("name : ");
        int i = allSectHdr[idx].sh_name;
        while (str[i] != '\0') {
            printf("%c", str[i]);
            i++;
        }
        printf("\n");
        printf("type : %u\n", allSectHdr[idx].sh_type); //a remplacer par leur équivalent
        printf("size : %u offset : %u\n", allSectHdr[idx].sh_size, allSectHdr[idx].sh_offset);
        printf("flags : ");
        char *flags = "WAXMSILO";
        int save = allSectHdr[idx].sh_flags;
        for (i = 0; i < 8; i++) {
            if (((allSectHdr[idx].sh_flags >> i) & 1) == 1) {
                printf("%c", flags[i]);
            }
            allSectHdr[idx].sh_flags = save;
        }
        printf("\n");
        if (allSectHdr[idx].sh_addr != 0) {
            printf("address : %u\n", allSectHdr[idx].sh_addr);


        } else {
            printf("pas d'adresse memoire predefinie pour le stockage de cette section\n");
        }
        if (allSectHdr[idx].sh_entsize != 0) {
            printf("taille des entrees prefixee a %u bits \n", allSectHdr[idx].sh_entsize);
        }

        printf("\n");

    }
}


