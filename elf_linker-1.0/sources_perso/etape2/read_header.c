#include "read_header.h"
#include "../etape1/en_tete_elf.c"
#include <math.h>
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
    printf("Il y a %i en-tetes de section :\n\n", elfHdr.e_shnum);

    //get and store the string table
    char* str = getSectionsStringTable(nameFile);

    // read all section headers
    printf("[Nr]\tNom\t\t\tType\t\tAdr\tDecal.\tTaille\tEs\tFan\n");
    for (idx = 0; idx < elfHdr.e_shnum; idx++) {
        //[Nr]
        printf("[%i]\t", idx);
        //Nom
        int i = allSectHdr[idx].sh_name;
        int size_tab = 0;
        while (str[i] != '\0') {
            printf("%c", str[i]);
            i++;
            size_tab++;
        }
        switch(size_tab/8) {
            case 0 : printf("\t\t\t");break;
            case 1 : printf("\t\t");break;
            default : printf("\t");break;
        }
        
        //type
        switch(allSectHdr[idx].sh_type) {
            case 0 : printf("NULL\t\t");break;
            case 1 : printf("PROGBITS\t");break;
            case 2 : printf("SYMTAB\t\t");break;
            case 3 : printf("STRTAB\t\t");break;
            case 4 : printf("RELA\t\t");break;
            case 5 : printf("HASH\t\t");break;
            case 6 : printf("DYNAMIC\t\t");break;
            case 7 : printf("NOTE\t\t");break;
            case 8 : printf("NOBITS\t\t");break;
            case 9 : printf("REL\t\t");break;
            case 10 : printf("SHLIB\t\t");break;
            case 11 : printf("DYNSYM\t\t");break;
            case 0x70000000 : printf("LOPROC\t\t");break;
            case 0x7fffffff : printf("HIPROC\t\t");break;
            case 0x80000000 : printf("LOUSER\t\t");break;
            case 0xffffffff : printf("HIUSER\t\t");break;
            //ARM
            case 0x70000001 : printf("ARM_EXIDX\t");break;
            case 0x70000002 : printf("ARM_PREEMPTMAP\t");break;
            case 0x70000003 : printf("ARM_ATTRIBUTES\t");break;
            case 0x70000004 : printf("ARM_DEBUGOVERLAY\t");break;
            case 0x70000005 : printf("ARM_OVERLAYSECTION\t");break;
            default : printf("unknown type\t");break;
        }

        //adr
        if (allSectHdr[idx].sh_addr != 0) {
            printf("%u\t", allSectHdr[idx].sh_addr);
        } else {
            printf("undef\t");
        }
        
        //decalage
        printf("%u\t", allSectHdr[idx].sh_offset); 

        //size
        printf("%u\t", allSectHdr[idx].sh_size);

        //Es
        if (allSectHdr[idx].sh_entsize != 0) {
            printf("%u bits\t", allSectHdr[idx].sh_entsize);
        } else {
            printf("\t");
        }

        //fan
        char *flags = "WAXMSILO";
        int save = allSectHdr[idx].sh_flags;
        for (i = 0; i < 8; i++) {
            if (((allSectHdr[idx].sh_flags >> i) & 1) == 1) {
                printf("%c", flags[i]);
            }
            allSectHdr[idx].sh_flags = save;
        }
        printf("\t");


        printf("\n");

    }
}


