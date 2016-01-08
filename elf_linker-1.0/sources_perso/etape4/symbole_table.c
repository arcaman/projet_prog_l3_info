#include "symbole_table.h"

#define SHT_SYMTAB 2
#define MODE_BIG_ENDIAN 2

Elf32_Sym createObjectSymbol(char* nameFile, int index) {
    Elf32_Sym sym;
    Elf32_Shdr symbtab;
    FILE* fichierAnalyse = fopen(nameFile, "r");
    Elf32_Ehdr elfHdr = createObjectEnteteELF(nameFile);
    Elf32_Shdr* sectiontab = createAllObjectSectionHeader(nameFile);
    int i;
    for (i = 0; i < elfHdr.e_shnum; i++) {
        if (sectiontab[i].sh_type == SHT_SYMTAB) {
            symbtab = sectiontab[i];
        }
    }
    fseek(fichierAnalyse, symbtab.sh_offset + index * sizeof (Elf32_Sym), SEEK_SET);

    fread(&sym.st_name, sizeof (Elf32_Word), 1, fichierAnalyse);
    fread(&sym.st_value, sizeof (Elf32_Addr), 1, fichierAnalyse);
    fread(&sym.st_size, sizeof (Elf32_Word), 1, fichierAnalyse);
    fread(&sym.st_info, sizeof (unsigned char), 1, fichierAnalyse);
    fread(&sym.st_other, sizeof (unsigned char), 1, fichierAnalyse);
    fread(&sym.st_shndx, sizeof (Elf32_Half), 1, fichierAnalyse);

    if (elfHdr.e_ident[5] == MODE_BIG_ENDIAN) {
        // 5 correspondant à l'octet étant le big ou little
        sym.st_name = __bswap_32(sym.st_name);
        sym.st_value = __bswap_32(sym.st_value);
        sym.st_size = __bswap_32(sym.st_size);
        // on ne swap pas les unsigned char parce qu'ils ne sont pas en big endian
        sym.st_shndx = __bswap_16(sym.st_shndx);
    }

    fclose(fichierAnalyse);
    return sym;
}

Elf32_Sym* createAllObjectSymbol(char* nameFile) {
    int i;
    
    Elf32_Shdr symbTable;
    Elf32_Ehdr elfHdr = createObjectEnteteELF(nameFile);
    Elf32_Shdr* sectiontab = createAllObjectSectionHeader(nameFile);
    for (i = 0; i < elfHdr.e_shnum; i++) {
        if (sectiontab[i].sh_type == SHT_SYMTAB) {
            symbTable = sectiontab[i];
        }
    }
    //on alloue la taille du contenu de la section, car ona besoin 
    //d'autant de place pour stocker ces symboles
    Elf32_Sym* tab = malloc(symbTable.sh_size);
    int k = symbTable.sh_size / sizeof (Elf32_Sym);
    for (i = 0; i < k; i++) {
        tab[i] = createObjectSymbol(nameFile, i);
    }
    return tab;
}

void afficherTableSymbole(char * nameFile) {
    Elf32_Shdr symbtab, strsymbtab;
    Elf32_Ehdr elfHdr = createObjectEnteteELF(nameFile);
    Elf32_Shdr* sectiontab = createAllObjectSectionHeader(nameFile);
    int i;
    for (i = 0; i < elfHdr.e_shnum; i++) {
        if (sectiontab[i].sh_type == SHT_SYMTAB) {
            symbtab = sectiontab[i];
            strsymbtab = sectiontab[i + 1];
        }
    }
    //NE PAS REMPLACER PAR char* str = getSectionsStringTable(nameFile);
    //car on ne récupereras pas la bonne table
    FILE* fichier = fopen(nameFile, "r");
    fseek(fichier, strsymbtab.sh_offset, SEEK_SET);
    char * str = malloc(strsymbtab.sh_size);
    for (i = 0; i < strsymbtab.sh_size; i++) {
        str[i] = fgetc(fichier);
    }
    fclose(fichier);



    int k = symbtab.sh_size / sizeof (Elf32_Sym);
    Elf32_Sym* allSymbole = createAllObjectSymbol(nameFile);

    for (i = 0; i < k; i++) {
        printf("symbole numero %i :\n", i);
        printf("nom : ");
        int j = allSymbole[i].st_name;
        while (str[j] != '\0') {
            printf("%c", str[j]);
            j++;
        }
        printf("\n");
        printf("valeur : %i\n", allSymbole[i].st_value);
        printf("taille : %i\n", allSymbole[i].st_size);
        printf("symbole utilise dans la section %i\n", allSymbole[i].st_shndx);
        printf("attachement : ");
        switch (ELF32_ST_BIND(allSymbole[i].st_info)) {
            case 0:
                printf("local\n");
                break;
            case 1:
                printf("global\n");
                break;
            case 2:
                printf("faible\n");
                break;
            case 13:
                printf("reserve au processeur\n");
                break;
            case 15:
                printf("reserve au processeur\n");
                break;
            default:
                printf("inconnu\n");

        }
        printf("type : ");
        switch (ELF32_ST_TYPE(allSymbole[i].st_info)) {
            case 0:
                printf("non precise\n");
                break;
            case 1:
                printf("objet\n");
                break;
            case 2:
                printf("fonction\n");
                break;
            case 3:
                printf("section\n");
                break;
            case 4:
                printf("fichier\n");
                break;
            case 13:
                printf("reserve au processeur\n");
                break;
            case 15:
                printf("reserve au processeur\n");
                break;
            default:
                printf("autre type de symbole\n");
        }



        printf("\n");
    }
}