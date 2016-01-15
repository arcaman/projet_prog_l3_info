#include "readelf_perso.h"
#define MODE_BIG_ENDIAN 2
#define SHT_SYMTAB 2

/* ----- ENTETE ELF ----- */

Elf32_Ehdr createObjectEnteteELF(FILE *fichierAnalyse) {
    Elf32_Ehdr enTeteHeader;

    fread(&enTeteHeader, sizeof (Elf32_Ehdr), 1, fichierAnalyse);

    if (enTeteHeader.e_ident[5] == MODE_BIG_ENDIAN) { // 5 correspondant à l'octet étant le big ou little
        enTeteHeader.e_type = __bswap_16(enTeteHeader.e_type);
        enTeteHeader.e_machine = __bswap_16(enTeteHeader.e_machine);
        enTeteHeader.e_version = __bswap_32(enTeteHeader.e_version);
        enTeteHeader.e_entry = __bswap_32(enTeteHeader.e_entry);
        enTeteHeader.e_phoff = __bswap_32(enTeteHeader.e_phoff);
        enTeteHeader.e_shoff = __bswap_32(enTeteHeader.e_shoff);
        enTeteHeader.e_flags = __bswap_32(enTeteHeader.e_flags);
        enTeteHeader.e_ehsize = __bswap_16(enTeteHeader.e_ehsize);
        enTeteHeader.e_phentsize = __bswap_16(enTeteHeader.e_phentsize);
        enTeteHeader.e_phnum = __bswap_16(enTeteHeader.e_phnum);
        enTeteHeader.e_shentsize = __bswap_16(enTeteHeader.e_shentsize);
        enTeteHeader.e_shnum = __bswap_16(enTeteHeader.e_shnum);
        enTeteHeader.e_shstrndx = __bswap_16(enTeteHeader.e_shstrndx);
    }


    return enTeteHeader;
}

void afficheTableEnTete(Elf32_Ehdr enTeteHeader) {

    int k;
    printf("En-tete ELF:\n");
    printf("Magique : ");
    for (k = 0; k < EI_NIDENT; k++) {
        printf("%02x ", enTeteHeader.e_ident[k]);
    }
    printf("\n");
    const char* classe[] = {"", "ELF32", "ELF64"};
    if (enTeteHeader.e_ident[4] == 1 || enTeteHeader.e_ident[4] == 2) {
        printf("\tClasse : %s\n", classe[enTeteHeader.e_ident[4]]);
    } else {
        printf("\tClasse : unknown\n");
    }
    const char* endian[] = {"", "complement a 2, systeme a octets de poids faible d'abord (little endian)", "complement a 2, systeme a octets de poids fort d'abord (big endian)"};
    if (enTeteHeader.e_ident[5] == 1 || enTeteHeader.e_ident[5] == 2) {
        printf("\tDonnees : %s\n", endian[enTeteHeader.e_ident[5]]);
    } else {
        printf("\tDonnees : unknown\n");
    }

    printf("\tVersion : %u (current)\n", enTeteHeader.e_ident[6]);

    switch (enTeteHeader.e_ident[7]) {
        case 0: printf("\tOS/ABI : UNIX - System V\n");
            break;
        case 1: printf("\tOS/ABI : HP-UX\n");
            break;
        case 2: printf("\tOS/ABI : NetBSD\n");
            break;
        case 3: printf("\tOS/ABI : Linux\n");
            break;
        case 6: printf("\tOS/ABI : Solaris\n");
            break;
        case 7: printf("\tOS/ABI : AIX\n");
            break;
        case 8: printf("\tOS/ABI : Irix\n");
            break;
        case 9: printf("\tOS/ABI : FreeBSD\n");
            break;
        case 12: printf("\tOS/ABI : OpenBSD\n");
            break;
        case 13: printf("\tOS/ABI : OpenVMS\n");
            break;
        default: printf("\tOS/ABI : unknown\n");
    }

    printf("\tVersion ABI : %u\n", enTeteHeader.e_ident[8]);

    if (enTeteHeader.e_type == 1) {
        printf("Type : REL (Fichier de relocalisation)\n");
    } else {
        printf("Type : %u\n", enTeteHeader.e_type);
    }
    switch (enTeteHeader.e_machine) {
        case 0: printf("Machine : No specific instruction set\n");
            break;
        case 2: printf("Machine : SPARC\n");
            break;
        case 3: printf("Machine : x86\n");
            break;
        case 8: printf("Machine : MIPS\n");
            break;
        case 20: printf("Machine : PowerPC\n");
            break;
        case 40: printf("Machine : ARM\n");
            break;
        case 42: printf("Machine : SuperH\n");
            break;
        case 50: printf("Machine : IA-64\n");
            break;
        case 62: printf("Machine : x86-64\n");
            break;
        case 183: printf("Machine : AArch64\n");
            break;
        default: printf("Machine : unknown\n");
    }

    printf("Version : 0x%x\n", enTeteHeader.e_version);
    printf("Adresse du point d'entree : 0x%x\n", enTeteHeader.e_entry);
    printf("Debut des en-tetes de programme : %x (octets dans le fichier)\n", enTeteHeader.e_phoff);
    printf("Debut des en-tetes de section : %u (octets dans le fichier)\n", enTeteHeader.e_shoff);
    printf("Fanions : 0x%x, Version5 EABI\n", enTeteHeader.e_flags);
    printf("Taille de cet en-tete : %u (bytes)\n", enTeteHeader.e_ehsize);
    printf("Taille de l'en-tete du programme : %u (bytes)\n", enTeteHeader.e_phentsize);
    printf("Nombre d'en-tete du programme : %u\n", enTeteHeader.e_phnum);
    printf("Taille des en-tetes de section : %u (bytes)\n", enTeteHeader.e_shentsize);
    printf("Nombre d'en-tetes de section : %u\n", enTeteHeader.e_shnum);
    printf("Table d'indexes des chaines d'en-tete de section : %u\n", enTeteHeader.e_shstrndx);

}

/* ----- READ HEADER ----- */

Elf32_Shdr createObjectSectionheader(FILE* fichierAnalyse, int index, Elf32_Ehdr elfHdr) {
    Elf32_Shdr shdr;
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
    return shdr;
}

Elf32_Shdr* createAllObjectSectionHeader(FILE* fichierAnalyse, Elf32_Ehdr elfHdr) {

    int nbSections = elfHdr.e_shnum;
    int i;
    Elf32_Shdr* allSectHdr = malloc(sizeof (Elf32_Shdr) * nbSections);
    for (i = 0; i < nbSections; i++) {
        allSectHdr[i] = createObjectSectionheader(fichierAnalyse, i, elfHdr);
    }
    return allSectHdr;
}

char* getSectionsStringTable(FILE* fichierAnalyse, Elf32_Ehdr elfHdr) {
    Elf32_Shdr stringTable = createObjectSectionheader(fichierAnalyse, elfHdr.e_shstrndx, elfHdr);
    fseek(fichierAnalyse, stringTable.sh_offset, SEEK_SET);
    int idx;
    char* str = malloc(stringTable.sh_size);
    for (idx = 0; idx < stringTable.sh_size; idx++) {
        str[idx] = fgetc(fichierAnalyse);
    }
    return str;
}

void displaySectionHeader(FILE* fichierAnalyse, Elf32_Ehdr elfHdr, Elf32_Shdr* allSectHdr) {

    uint32_t idx;
    // read ELF header, first thing in the file
    printf("Il y a %i en-tetes de section :\n\n", elfHdr.e_shnum);

    //get and store the string table
    char* str = getSectionsStringTable(fichierAnalyse, elfHdr);

    // read all section headers
    printf("[Nr]\tNom\t\t\tType\t\tAdr\tDecal.\tTaille\tEs\tLn\tNf\tFan\n");
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
        switch (size_tab / 8) {
            case 0: printf("\t\t\t");
                break;
            case 1: printf("\t\t");
                break;
            default: printf("\t");
                break;
        }

        //type
        switch (allSectHdr[idx].sh_type) {
            case 0: printf("NULL\t\t");
                break;
            case 1: printf("PROGBITS\t");
                break;
            case 2: printf("SYMTAB\t\t");
                break;
            case 3: printf("STRTAB\t\t");
                break;
            case 4: printf("RELA\t\t");
                break;
            case 5: printf("HASH\t\t");
                break;
            case 6: printf("DYNAMIC\t\t");
                break;
            case 7: printf("NOTE\t\t");
                break;
            case 8: printf("NOBITS\t\t");
                break;
            case 9: printf("REL\t\t");
                break;
            case 10: printf("SHLIB\t\t");
                break;
            case 11: printf("DYNSYM\t\t");
                break;
            case 0x70000000: printf("LOPROC\t\t");
                break;
            case 0x7fffffff: printf("HIPROC\t\t");
                break;
            case 0x80000000: printf("LOUSER\t\t");
                break;
            case 0xffffffff: printf("HIUSER\t\t");
                break;
                //ARM
            case 0x70000001: printf("ARM_EXIDX\t");
                break;
            case 0x70000002: printf("ARM_PREEMPTMAP\t");
                break;
            case 0x70000003: printf("ARM_ATTRIBUTES\t");
                break;
            case 0x70000004: printf("ARM_DEBUGOVERLAY\t");
                break;
            case 0x70000005: printf("ARM_OVERLAYSECTION\t");
                break;
            default: printf("unknown type\t");
                break;
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

        //ln
        printf("%u\t", allSectHdr[idx].sh_link);

        //info
        printf("%u\t", allSectHdr[idx].sh_info);


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
    free(str);
}

/* ----- DISPLAY CONTENT ----- */

int getIndexSectionByNameOrIndex(FILE* fichierAnalyse, Elf32_Ehdr elfHdr, char *indiceOrNameSection, int isInt, Elf32_Shdr* allSectHdr) {

    int idx;

    int nbSections = elfHdr.e_shnum;

    if (isInt == 1) {
        if (atoi(indiceOrNameSection) < 0 || atoi(indiceOrNameSection) > nbSections - 1) {
            return -1;
        }
        return atoi(indiceOrNameSection);
    }


    char* str = getSectionsStringTable(fichierAnalyse, elfHdr);
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
            free(str);
            return idx;
        }
    }
    free(str);
    return -1;
}

unsigned char* createSectionContent(FILE* fichierAnalyse, Elf32_Ehdr elfHdr, int indiceSectionHeader) {
    Elf32_Shdr shdr = createObjectSectionheader(fichierAnalyse, indiceSectionHeader, elfHdr);

    fseek(fichierAnalyse, shdr.sh_offset, SEEK_SET);

    unsigned char* tabOctets = malloc(sizeof (unsigned char)*shdr.sh_size);

    int i;
    unsigned char valeurOctet;
    for (i = 0; i < shdr.sh_size; i++) {
        fread(&valeurOctet, 1, 1, fichierAnalyse);
        tabOctets[i] = valeurOctet;
    }
    return tabOctets;
}

unsigned char** createAllObjectSectionContent(FILE* fichierAnalyse, Elf32_Ehdr elfHdr) {

    int nbSections = elfHdr.e_shnum;
    int i;
    unsigned char** tableauAllSectionContent = malloc(sizeof (unsigned char*) * nbSections);
    for (i = 0; i < nbSections; i++) {
        tableauAllSectionContent[i] = createSectionContent(fichierAnalyse, elfHdr, i);
    }
    return tableauAllSectionContent;
}

void displaySectionContent(unsigned char* tableauOctetsSection, FILE* fichierAnalyse, int indiceSectionHeader, Elf32_Ehdr elfHdr) {

    Elf32_Shdr shdr = createObjectSectionheader(fichierAnalyse, indiceSectionHeader, elfHdr);
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

/* ----- SYMBOL TABLE ----- */

Elf32_Sym createObjectSymbol(FILE* fichierAnalyse, int index, Elf32_Ehdr elfHdr, Elf32_Shdr* sectiontab) {
    Elf32_Sym sym;
    Elf32_Shdr symbtab;
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

    return sym;
}

Elf32_Sym* createAllObjectSymbol(FILE* fichierAnalyse, Elf32_Ehdr elfHdr, Elf32_Shdr* sectiontab) {
    int i;

    Elf32_Shdr symbTable;
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
        tab[i] = createObjectSymbol(fichierAnalyse, i, elfHdr, sectiontab);
    }
    return tab;
}

void afficherTableSymbole(FILE* fichierAnalyse, Elf32_Ehdr elfHdr, Elf32_Shdr* sectiontab, Elf32_Sym * allSymbole) {
    Elf32_Shdr symbtab, strsymbtab;
    int i;
    for (i = 0; i < elfHdr.e_shnum; i++) {
        if (sectiontab[i].sh_type == SHT_SYMTAB) {
            symbtab = sectiontab[i];
            strsymbtab = sectiontab[i + 1];
        }
    }
    //NE PAS REMPLACER PAR char* str = getSectionsStringTable(nameFile);
    //car on ne récupereras pas la bonne table

    fseek(fichierAnalyse, strsymbtab.sh_offset, SEEK_SET);
    char * str = malloc(strsymbtab.sh_size);
    for (i = 0; i < strsymbtab.sh_size; i++) {
        str[i] = fgetc(fichierAnalyse);
    }



    int k = symbtab.sh_size / sizeof (Elf32_Sym);

    printf("Num\tNom\tVal\tTail\t\tNdx\tLien\tType\t\tVis\n\n");

    for (i = 0; i < k; i++) {
        printf("%i\t", i);
        //printf("nom : ");
        int j = allSymbole[i].st_name;
        while (str[j] != '\0') {
            printf("%c", str[j]);
            j++;
        }
        printf("\t");
        printf("%08x\t", allSymbole[i].st_value);
        printf("%i\t", allSymbole[i].st_size);
        printf("%i\t", allSymbole[i].st_shndx);
        //printf("attachement : ");
        switch (ELF32_ST_BIND(allSymbole[i].st_info)) {
            case 0:
                printf("local\t");
                break;
            case 1:
                printf("global\t");
                break;
            case 2:
                printf("faible\t");
                break;
            case 13:
                printf("reserve au processeur\t");
                break;
            case 15:
                printf("reserve au processeur\t");
                break;
            default:
                printf("inconnu\t");

        }
        //printf("type : ");
        switch (ELF32_ST_TYPE(allSymbole[i].st_info)) {
            case 0:
                printf("non precise\t");
                break;
            case 1:
                printf("objet\t\t");
                break;
            case 2:
                printf("fonction\t\t");
                break;
            case 3:
                printf("section\t\t");
                break;
            case 4:
                printf("fichier\t\t");
                break;
            case 13:
                printf("reserve au processeur\t");
                break;
            case 15:
                printf("reserve au processeur\t");
                break;
            default:
                printf("autre type de symbole\t");
        }

        printf("%u", ELF32_ST_VISIBILITY(allSymbole[i].st_info));

        printf("\n");
    }
    free(str);
}

/* ----- RELOCATIONS TABLE ----- */

Elf32_Rel createObjectRelocations(FILE* fichierAnalyse, Elf32_Shdr sect, int index, Elf32_Ehdr elfHdr) {
    Elf32_Rel rel;
    fseek(fichierAnalyse, sect.sh_offset + index * sizeof (Elf32_Rel), SEEK_SET);
    fread(&rel, sizeof (Elf32_Rel), 1, fichierAnalyse);
    if (elfHdr.e_ident[5] == MODE_BIG_ENDIAN) { // 5 correspondant à l'octet étant le big ou little
        rel.r_info = __bswap_32(rel.r_info);
        rel.r_offset = __bswap_32(rel.r_offset);
    }
    return rel;
}

Elf32_Rel * createAllRelocationBySection(FILE* fichierAnalyse, int nbent, Elf32_Shdr sect, Elf32_Ehdr elfHdr) {
    int i;
    Elf32_Rel* tab = malloc(nbent * sizeof (Elf32_Rel));
    for (i = 0; i < nbent; i++) {
        tab[i] = createObjectRelocations(fichierAnalyse, sect, i, elfHdr);
        //tab[i].info = sect.sh_info;
    }
    return tab;
}

Elf32_Rel** createAllRelocations(FILE* fichierAnalyse, Elf32_Ehdr elfHdr, Elf32_Shdr * allSectHdr) {
    int i;
    int nbSectRel = 0;
    for (i = 0; i < elfHdr.e_shnum; i++) {
        if (allSectHdr[i].sh_type == SHT_REL) {
            nbSectRel++;
        }
    }
    Elf32_Rel* allRelForSec;
    Elf32_Rel** allSectRel = malloc(nbSectRel * sizeof (Elf32_Rel*));
    int l = 0;
    for (i = 0; i < elfHdr.e_shnum; i++) {
        if (allSectHdr[i].sh_type == SHT_REL) {
            int nb_ent = allSectHdr[i].sh_size / sizeof (Elf32_Rel);
            allRelForSec = createAllRelocationBySection(fichierAnalyse, nb_ent, allSectHdr[i], elfHdr);
            allSectRel[l] = allRelForSec;
            free(allRelForSec);
            l++;
        }
    }
    return allSectRel;
}

void readRelocations(FILE* fichierAnalyse, Elf32_Ehdr elfHdr, Elf32_Shdr * allSectHdr) {
    int i;
    int nbSectRel = 0;
    for (i = 0; i < elfHdr.e_shnum; i++) {
        if (allSectHdr[i].sh_type == SHT_REL) {
            nbSectRel++;
        }
    }
    int* tabIndSectRel = malloc(nbSectRel * sizeof (int));
    Elf32_Rel** allSectRel = malloc(nbSectRel * sizeof (Elf32_Rel*));
    int l = 0;
    for (i = 0; i < elfHdr.e_shnum; i++) {
        if (allSectHdr[i].sh_type == SHT_REL) {
            int nb_ent = allSectHdr[i].sh_size / sizeof (Elf32_Rel);
            Elf32_Rel* allRelForSec = createAllRelocationBySection(fichierAnalyse, nb_ent, allSectHdr[i], elfHdr);
            allSectRel[l] = allRelForSec;
            tabIndSectRel[l] = i;
            l++;
            free(allRelForSec);
        }
    }
    affichageRelocations(allSectRel, tabIndSectRel, nbSectRel, fichierAnalyse, elfHdr);
    free(tabIndSectRel);
    free(allSectRel);
}

void affichageRelocations(Elf32_Rel** allRel, int* tabIndSectRel, int nbSectRel, FILE* fichierAnalyse, Elf32_Ehdr elfHdr) {
    Elf32_Shdr* allSect = createAllObjectSectionHeader(fichierAnalyse, elfHdr);

    char* str = getSectionsStringTable(fichierAnalyse, elfHdr);
    int k, n = 0;
    for (k = 0; k < nbSectRel; k++) {
        int nb_ent_current = allSect[tabIndSectRel[k]].sh_size / sizeof (Elf32_Rel);
        printf("Section de relocalisation ");
        int i = allSect[tabIndSectRel[k]].sh_name;
        while (str[i] != '\0') {
            printf("%c", str[i]);
            i++;
        }
        printf(" a l adresse de decalage 0x%x contient %d entrees:\n", allSect[tabIndSectRel[k]].sh_offset, nb_ent_current);
        printf("Decalage\tInfo\t\tType\n");
        for (n = 0; n < nb_ent_current; n++) {
            printf("%08x\t%08x\t%08x\n", allRel[k][n].r_offset, allRel[k][n].r_info, ELF32_R_TYPE(allRel[k][n].r_info));
        }
        printf("\n");
    }
    free(str);
    free(allSect);
}

/* ----- EDITION OBJET SANS RELOCALISATION -----*/


Elf32_Shdr* createObjectSectionHeaderWithoutRelocalisations(Elf32_Shdr* shdr, Elf32_Ehdr elf, Elf32_Ehdr* elfApresReloc) {
    int nbSections = elf.e_shnum;
    int nbSectionsRelocalises = countNbSectionsRelocalisesByAllSectionHeader(elf, shdr);
    int nbSectionsApresReloc = countNbSectionsNonRelocalisesByAllSectionHeader(elf, shdr);
    Elf32_Shdr* shdrApresReloc = malloc(sizeof (Elf32_Shdr) * nbSectionsApresReloc);
    *elfApresReloc = elf;
    Elf32_Shdr* shdrcpy = malloc(nbSections * sizeof (Elf32_Shdr));
    memcpy(shdrcpy, shdr, nbSections * sizeof (Elf32_Shdr));
    int i;
    int k = 0;
    for (i = 0; i < nbSections; i++) {
        if (shdrcpy[i].sh_type == SHT_REL) {
            elfApresReloc->e_shnum--;
            elfApresReloc->e_shstrndx--; //note : ca fait planter l'affichage dans la 6, mais ca fera marcher normalement une fois le fichier réécrit
        } else {
            shdrApresReloc[k] = shdrcpy[i];
            if (shdrApresReloc[k].sh_link != 0) {
                shdrApresReloc[k].sh_link -= nbSectionsRelocalises;
            }
            k++;
        }
    }
    free(shdrcpy);
    return shdrApresReloc;
}

int countNbSectionsNonRelocalisesByAllSectionHeader(Elf32_Ehdr elfHdr, Elf32_Shdr* allSectHdr) {
    int indiceNbCasesTabSansRelocalisations = 0;
    int nbSections = elfHdr.e_shnum;
    int i;
    for (i = 0; i < nbSections; i++) {
        if (allSectHdr[i].sh_type != SHT_REL) {
            indiceNbCasesTabSansRelocalisations++;
        }
    }
    return indiceNbCasesTabSansRelocalisations;
}

Elf32_Shdr* createObjectSectionHeaderRelocalisations(Elf32_Ehdr elfHdr, Elf32_Shdr* allSectHdr, Elf32_Ehdr* elfHdrRelocalisations) {

    int nbSections = elfHdr.e_shnum;
    int i, j = 0; // j : indice pour le tableau des sections
    int nbSectionsRelocalises = countNbSectionsRelocalisesByAllSectionHeader(elfHdr, allSectHdr);
    //printf("NB SECTIONS NON RELOC : %d", nbSectionsNonRelocalises);
    elfHdrRelocalisations->e_shnum = nbSectionsRelocalises;

    Elf32_Shdr* objSectHdrRelocalisations = malloc(sizeof (Elf32_Shdr) * nbSectionsRelocalises);
    for (i = 0; i < nbSections; i++) {

        if (allSectHdr[i].sh_type == SHT_REL) {
            objSectHdrRelocalisations[j] = allSectHdr[i];
            objSectHdrRelocalisations[j].sh_info -= j;
            objSectHdrRelocalisations[j].sh_link -= nbSectionsRelocalises;
            j++;
        }
        //printf("section numero %d : %d \n", i, allSectHdr[i].sh_type);
    }
    return objSectHdrRelocalisations;
}

int countNbSectionsRelocalisesByAllSectionHeader(Elf32_Ehdr elfHdr, Elf32_Shdr* allSectHdr) {
    int indiceNbCasesTabRelocalisations = 0;
    int nbSections = elfHdr.e_shnum;
    int i;
    for (i = 0; i < nbSections; i++) {
        if (allSectHdr[i].sh_type == SHT_REL) {
            indiceNbCasesTabRelocalisations++;
        }
    }
    return indiceNbCasesTabRelocalisations;
}

/* ----- RENOMAGE SYMBOLES -----*/

int* createTableComparaisonSymbolesApresRelocation(Elf32_Ehdr elfHdr, Elf32_Shdr* allSectHdr) {
    int nbSections = elfHdr.e_shnum;
    int indiceSuppressionSection = 0;
    int i;
    int* tabComparaisonSymboles = malloc(sizeof (int)*nbSections);
    for (i = 0; i < nbSections; i++) {
        //printf("tableau indices : %i\n", i);
        if (allSectHdr[i].sh_type == SHT_REL) {
            indiceSuppressionSection++;
        }
        tabComparaisonSymboles[i] = i - indiceSuppressionSection;
    }
    return tabComparaisonSymboles;
}

Elf32_Sym* creationTableDesSymbolesCorrecte(FILE* fichierAnalyse, Elf32_Sym* allObjectSymbol, int* tabComparaisonSymboles, Elf32_Ehdr elfHdr, Elf32_Shdr* sectiontab, int argc, char** argv) {

    int i, j;

    Elf32_Shdr symbTable;
    for (i = 0; i < elfHdr.e_shnum; i++) {
        if (sectiontab[i].sh_type == SHT_SYMTAB) {
            symbTable = sectiontab[i];
        }
    }

    Elf32_Sym* tabSymbolesRelocalise = malloc(symbTable.sh_size);
    memcpy(tabSymbolesRelocalise, allObjectSymbol, symbTable.sh_size);
    int k = symbTable.sh_size / sizeof (Elf32_Sym);
    for (i = 0; i < k; i++) {
        tabSymbolesRelocalise[i].st_shndx = tabComparaisonSymboles[(allObjectSymbol[i].st_shndx)];
        //parcours de tous les symboles ou on a une adresse
        for (j = 2; j < argc; j++) { //debut des parametres a partir de argv[2] (le premier etant le script et le deuxieme le fichier)
            int count = 1;
            while (argv[j][count] != '\0') {
                count++;
            }
            char* tableauCaracteres = malloc(sizeof (char*)*count);
            strcpy(tableauCaracteres, argv[j]);

            char* chaineSplit = strtok(tableauCaracteres, "="); //1er argument qui correspond au nom de la section

            int indiceSection = getIndexSectionByNameOrIndex(fichierAnalyse, elfHdr, chaineSplit, 0, sectiontab);
            if (indiceSection != -1 && indiceSection == tabSymbolesRelocalise[i].st_shndx) {
                char* adresseSection = strtok(NULL, "="); //on recupere l adresse apres le =
                u_int32_t adresseSectionHexa = convertCharToHexadecimal(adresseSection);
                tabSymbolesRelocalise[i].st_value = tabSymbolesRelocalise[i].st_value + adresseSectionHexa;
            }
            free(tableauCaracteres);
        }
    }
    free(tabComparaisonSymboles);
    return tabSymbolesRelocalise;
}

u_int32_t convertCharToHexadecimal(char* valueHexaString) {
    int i = 2;
    u_int32_t valeurHexadecimal = 0;
    while (valueHexaString[i] != '\0') {
        valeurHexadecimal = 16 * valeurHexadecimal + (valueHexaString[i] - '0');
        i++;
    }
    return valeurHexadecimal;
}



/* ----- RELOCALISATION SYMBOLES -----*/

//note : indice section AVANT suppression des reltables

unsigned char * replaceSectionContent(FILE* fichierAnalyse, Elf32_Shdr* shdr, Elf32_Ehdr elfHdr, int indiceSection, Elf32_Sym* SymbolesCorrects) {
    int i, j;
    Elf32_Ehdr elfHdrRelocalisations;
    Elf32_Shdr* shdrRel = createObjectSectionHeaderRelocalisations(elfHdr, shdr, &elfHdrRelocalisations);
    unsigned char* sectionContent = createSectionContent(fichierAnalyse, elfHdr, indiceSection);
    for (i = 0; i < elfHdrRelocalisations.e_shnum; i++) {
        if (shdrRel[i].sh_info == indiceSection) { //la table Rel influe bien sur la section que nous allons modifier
            Elf32_Rel* reltable = createAllRelocationBySection(fichierAnalyse, shdrRel[i].sh_size / sizeof (Elf32_Rel), shdrRel[i], elfHdr);
            int nbIter = (shdrRel[i].sh_size / sizeof (Elf32_Rel));
            for (j = 0; j < nbIter; j++) {

                //printf("\nindex symb : %i\n", ELF32_R_SYM(reltable[j].r_info));
                Elf32_Sym symbole = SymbolesCorrects[ELF32_R_SYM(reltable[j].r_info)];

                Elf32_Addr S = symbole.st_value;

                unsigned char A[4];
                A[0] = sectionContent[reltable[j].r_offset];
                A[1] = sectionContent[(reltable[j].r_offset) + 1];
                A[2] = sectionContent[(reltable[j].r_offset) + 2];
                A[3] = sectionContent[(reltable[j].r_offset) + 3];
                //printf("a :       %02x %02x %02x %02x\n",A[0],A[1],A[2],A[3]);
                unsigned int AInt = A[0] | ((int) A[1] << 8) | ((int) A[2] << 16) | ((int) A[3] << 24);
                //printf("\n\n%d - %x\n\n", AInt, AInt);

                Elf32_Word res = 0;
                switch (ELF32_R_TYPE(reltable[j].r_info)) {
                    case 2:
                        //R_ARM_ABS32
                        // note, T = 0 dans notre cas
                        //(S + A) | T
                        res = (S + AInt) | 0;
                        break;
                    case 5:
                        //R_ARM_ABS16
                        //S + A
                        res = S + AInt;
                        break;
                    case 6:
                        //R_ARM_ABS12
                        //S + A
                        res = S + AInt;
                        break;
                    case 8:
                        //R_ARM_ABS8
                        //S + A
                        res = S + AInt;
                        break;
                    case 28:
                        //R_ARM_CALL
                        //( (S + A) | T) – P
                        res = ((S + AInt) | 0) - reltable[j].r_offset;
                        break;
                    case 29:
                        //R_ARM_JUMP24
                        //( (S + A) | T) – P
                        res = ((S + AInt) | 0) - reltable[j].r_offset;
                        break;
                    default:
                        //printf("unknow type\t");
                        break;
                }
                //printf("\n\nres : %i\n\n", res);

                sectionContent[reltable[j].r_offset] = (res >> 24) & 0xFF;
                sectionContent[reltable[j].r_offset + 1] = (res >> 16) & 0xFF;
                sectionContent[reltable[j].r_offset + 2] = (res >> 8) & 0xFF;
                sectionContent[reltable[j].r_offset + 3] = res & 0xFF;
            }
        }
    }
    return sectionContent;
}

unsigned char** replaceAllSectionsContent(FILE* fichierAnalyse, Elf32_Shdr* shdr, Elf32_Ehdr elfHdr, Elf32_Sym* SymbolesCorrects) {
    unsigned char** tab = malloc(elfHdr.e_shnum * sizeof (unsigned char *));
    int i;
    for (i = 0; i < elfHdr.e_shnum; i++) {
        tab[i] = replaceSectionContent(fichierAnalyse, shdr, elfHdr, i, SymbolesCorrects);
    }
    return tab;
}

Elf32_Phdr createObjectProgramHeader(FILE* fichierAnalyse, Elf32_Ehdr elfHdr) {
    Elf32_Phdr programHeader;
    fseek(fichierAnalyse, elfHdr.e_phoff, SEEK_SET);
    fread(&programHeader, sizeof (Elf32_Phdr) * elfHdr.e_phnum, 1, fichierAnalyse);
    return programHeader;
}

void creationFichierExecutable(FILE* fichierExecutable, Elf32_Ehdr elfHdrSansRelocalisations, Elf32_Phdr programHdr, Elf32_Shdr* objSectHdrSansRelocalisations, Elf32_Sym* tabSymbolesRelocalise, unsigned char** tableauSectionRelocation, int nbSecSupprimees) {
    unsigned char empty = 0;
    Elf32_Ehdr elfHdrCopy = elfHdrSansRelocalisations;
    if (elfHdrCopy.e_ident[5] == MODE_BIG_ENDIAN) { // 5 correspondant à l'octet étant le big ou little
        elfHdrCopy.e_type = __bswap_16(elfHdrCopy.e_type);
        elfHdrCopy.e_machine = __bswap_16(elfHdrCopy.e_machine);
        elfHdrCopy.e_version = __bswap_32(elfHdrCopy.e_version);
        elfHdrCopy.e_entry = __bswap_32(elfHdrCopy.e_entry);
        elfHdrCopy.e_phoff = __bswap_32(elfHdrCopy.e_phoff);
        elfHdrCopy.e_shoff = __bswap_32(elfHdrCopy.e_shoff);
        elfHdrCopy.e_flags = __bswap_32(elfHdrCopy.e_flags);
        elfHdrCopy.e_ehsize = __bswap_16(elfHdrCopy.e_ehsize);
        elfHdrCopy.e_phentsize = __bswap_16(elfHdrCopy.e_phentsize);
        elfHdrCopy.e_phnum = __bswap_16(elfHdrCopy.e_phnum);
        elfHdrCopy.e_shentsize = __bswap_16(elfHdrCopy.e_shentsize);
        elfHdrCopy.e_shnum = __bswap_16(elfHdrCopy.e_shnum);
        elfHdrCopy.e_shstrndx = __bswap_16(elfHdrCopy.e_shstrndx);
    }

    fwrite(&elfHdrCopy, sizeof (Elf32_Ehdr), 1, fichierExecutable); //ELF header verifié fonctionnel
    //printf("elfheader reecrit\n");
    if (elfHdrSansRelocalisations.e_phnum != 0) {
        fwrite(&programHdr, sizeof (Elf32_Phdr), 1, fichierExecutable); // marche pas s'il n'y a pas de prog header par défaut
        //printf("progheaders reecrits\n");
    }

    int i;
    int j = 0;
    for (i = 0; i < elfHdrSansRelocalisations.e_shnum; i++) {
        if (objSectHdrSansRelocalisations[i].sh_addralign > 1) {
            j = 0;
            while (j == 0) {
                if (ftell(fichierExecutable) % objSectHdrSansRelocalisations[i].sh_addralign != 0) {
                    fwrite(&empty, sizeof (unsigned char), 1, fichierExecutable);
                } else {
                    j = 1;
                }
            }
            //printf("%li\n", ftell(fichierExecutable));
        }
        if (ftell(fichierExecutable) == objSectHdrSansRelocalisations[i].sh_offset) {
            //printf("reecriture a l'adresse %li \n", ftell(fichierExecutable));
            for (j = 0; j < objSectHdrSansRelocalisations[i].sh_size; j++) {
                fwrite(&tableauSectionRelocation[i][j], sizeof (unsigned char), 1, fichierExecutable);
            }
            //fwrite(&tableauSectionRelocation[i], sizeof (unsigned char)*objSectHdrSansRelocalisations[i].sh_size, 1, fichierExecutable);
            //printf("section %i reecrite\n", i);
        } else {
            //si on arrive ici c'est qu'il s'agit des section APRES les section headers
            //on doit donc penser a supprimmer l'offset des section rel qui ont disparues
            objSectHdrSansRelocalisations[i].sh_offset -= nbSecSupprimees * sizeof (Elf32_Shdr);
            //on les réécriera plus loin, ne pas s'inquieter
            //printf("section %i apres les header, adresse modifiee\n", i);
        }
    }
    //printf("ecriture des headers de section\n");
    for (i = 0; i < elfHdrSansRelocalisations.e_shnum; i++) {
        if (elfHdrSansRelocalisations.e_ident[5] == MODE_BIG_ENDIAN) { // 5 correspondant à l'octet étant le big ou little
            objSectHdrSansRelocalisations[i].sh_name = __bswap_32(objSectHdrSansRelocalisations[i].sh_name);
            objSectHdrSansRelocalisations[i].sh_type = __bswap_32(objSectHdrSansRelocalisations[i].sh_type);
            objSectHdrSansRelocalisations[i].sh_flags = __bswap_32(objSectHdrSansRelocalisations[i].sh_flags);
            objSectHdrSansRelocalisations[i].sh_addr = __bswap_32(objSectHdrSansRelocalisations[i].sh_addr);
            objSectHdrSansRelocalisations[i].sh_offset = __bswap_32(objSectHdrSansRelocalisations[i].sh_offset);
            objSectHdrSansRelocalisations[i].sh_size = __bswap_32(objSectHdrSansRelocalisations[i].sh_size);
            objSectHdrSansRelocalisations[i].sh_link = __bswap_32(objSectHdrSansRelocalisations[i].sh_link);
            objSectHdrSansRelocalisations[i].sh_info = __bswap_32(objSectHdrSansRelocalisations[i].sh_info);
            objSectHdrSansRelocalisations[i].sh_addralign = __bswap_32(objSectHdrSansRelocalisations[i].sh_addralign);
            objSectHdrSansRelocalisations[i].sh_entsize = __bswap_32(objSectHdrSansRelocalisations[i].sh_entsize);
        }
        fwrite(&objSectHdrSansRelocalisations[i], sizeof (Elf32_Shdr), 1, fichierExecutable);
    }
    for (i = 0; i < elfHdrSansRelocalisations.e_shnum; i++) {
        //reinverser pour la suite
        if (elfHdrSansRelocalisations.e_ident[5] == MODE_BIG_ENDIAN) { // 5 correspondant à l'octet étant le big ou little
            objSectHdrSansRelocalisations[i].sh_name = __bswap_32(objSectHdrSansRelocalisations[i].sh_name);
            objSectHdrSansRelocalisations[i].sh_type = __bswap_32(objSectHdrSansRelocalisations[i].sh_type);
            objSectHdrSansRelocalisations[i].sh_flags = __bswap_32(objSectHdrSansRelocalisations[i].sh_flags);
            objSectHdrSansRelocalisations[i].sh_addr = __bswap_32(objSectHdrSansRelocalisations[i].sh_addr);
            objSectHdrSansRelocalisations[i].sh_offset = __bswap_32(objSectHdrSansRelocalisations[i].sh_offset);
            objSectHdrSansRelocalisations[i].sh_size = __bswap_32(objSectHdrSansRelocalisations[i].sh_size);
            objSectHdrSansRelocalisations[i].sh_link = __bswap_32(objSectHdrSansRelocalisations[i].sh_link);
            objSectHdrSansRelocalisations[i].sh_info = __bswap_32(objSectHdrSansRelocalisations[i].sh_info);
            objSectHdrSansRelocalisations[i].sh_addralign = __bswap_32(objSectHdrSansRelocalisations[i].sh_addralign);
            objSectHdrSansRelocalisations[i].sh_entsize = __bswap_32(objSectHdrSansRelocalisations[i].sh_entsize);
        }
    }
    for (i = 0; i < elfHdrSansRelocalisations.e_shnum; i++) {
        if (objSectHdrSansRelocalisations[i].sh_addralign > 1) {
            int j = 0;
            while (j == 0) {
                if (ftell(fichierExecutable) % objSectHdrSansRelocalisations[i].sh_addralign != 0) {
                    fwrite(&empty, sizeof (unsigned char), 1, fichierExecutable);
                } else {
                    j = 1;
                }
            }
            //printf("%li\n", ftell(fichierExecutable));
        }
        if (ftell(fichierExecutable) == objSectHdrSansRelocalisations[i].sh_offset) {
            if (objSectHdrSansRelocalisations[i].sh_type == SHT_SYMTAB) {
                //printf("reecriture a l'adresse %li \n", ftell(fichierExecutable));
                for (j = 0; j < objSectHdrSansRelocalisations[i].sh_size / sizeof (Elf32_Sym); j++) {
                    if (elfHdrSansRelocalisations.e_ident[5] == MODE_BIG_ENDIAN) {// 5 correspondant à l'octet étant le big ou little
                        tabSymbolesRelocalise[j].st_name = __bswap_32(tabSymbolesRelocalise[j].st_name);
                        tabSymbolesRelocalise[j].st_value = __bswap_32(tabSymbolesRelocalise[j].st_value);
                        tabSymbolesRelocalise[j].st_size = __bswap_32(tabSymbolesRelocalise[j].st_size);
                        // on ne swap pas les unsigned char parce qu'ils ne sont pas en big endian
                        tabSymbolesRelocalise[j].st_shndx = __bswap_16(tabSymbolesRelocalise[j].st_shndx);
                    }
                    fwrite(&tabSymbolesRelocalise[j], sizeof (Elf32_Sym), 1, fichierExecutable);
                }
                //printf("section %i reecrite\n", i);
            } else {
                //printf("reecriture a l'adresse %li \n", ftell(fichierExecutable));
                for (j = 0; j < objSectHdrSansRelocalisations[i].sh_size; j++) {
                    fwrite(&tableauSectionRelocation[i][j], sizeof (unsigned char), 1, fichierExecutable);
                }
                //printf("section %i reecrite\n", i);

            }
        }
    }
}