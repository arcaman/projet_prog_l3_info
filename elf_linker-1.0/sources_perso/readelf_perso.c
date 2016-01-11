#include "readelf_perso.h"
#define MODE_BIG_ENDIAN 2
#define SHT_SYMTAB 2

/* ----- ENTETE ELF ----- */

Elf32_Ehdr createObjectEnteteELF(FILE *fichierAnalyse) {
    Elf32_Ehdr enTeteHeader;

    fread(&enTeteHeader, sizeof (Elf32_Ehdr), 1, fichierAnalyse);

    if (enTeteHeader.e_ident[5] == MODE_BIG_ENDIAN) { // 5 correspondant � l'octet �tant le big ou little
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
    printf("Magique : ");
    for (k = 0; k < EI_NIDENT; k++) {
        printf("%02x ", enTeteHeader.e_ident[k]);
    }
    printf("\n");
    switch (enTeteHeader.e_ident[4]) {
        case 1: printf("\tClasse : 32-bit\n");
            break;
        case 2: printf("\tClasse : 64-bit\n");
            break;
        default: printf("\tClasse : unknown\n");
    }

    switch (enTeteHeader.e_ident[5]) {
        case 1: printf("\tDonnees : little endian\n");
            break;
        case 2: printf("\tDonnees : big endian\n");
            break;
        default: printf("\tDonnees : unknown\n");
    }

    printf("\tVersion : %u\n", enTeteHeader.e_ident[6]);

    switch (enTeteHeader.e_ident[7]) {
        case 0: printf("\tOS/ABI : System V\n");
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

    printf("Type : %u\n", enTeteHeader.e_type);

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

    printf("version : 0x%x\n", enTeteHeader.e_version);
    printf("Adresse point entree : 0x%x\n", enTeteHeader.e_entry);
    printf("Debut des en-tetes de programme : %x (octets dans le fichier)\n", enTeteHeader.e_phoff);
    printf("Debut des en-tetes de section : %u (octets dans le fichier)\n", enTeteHeader.e_shoff);
    printf("Flags : 0x%x\n", enTeteHeader.e_flags);
    printf("Taille de cet en-tete : %u (bytes)\n", enTeteHeader.e_ehsize);
    printf("Taille de l en-tete du programme : %u (bytes)\n", enTeteHeader.e_phentsize);
    printf("Nombre d en-tete du programme : %u\n", enTeteHeader.e_phnum);
    printf("Taille des en-tetes de section : %u (bytes)\n", enTeteHeader.e_shentsize);
    printf("Nombre des en-tetes de section : %u\n", enTeteHeader.e_shnum);
    printf("Table d indexes des chaines d'en-tete de section : %u\n", enTeteHeader.e_shstrndx);

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

    if (elfHdr.e_ident[5] == MODE_BIG_ENDIAN) { // 5 correspondant � l'octet �tant le big ou little
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
    printf("nb sections : %i\n", elfHdr.e_shnum);

    //get and store the string table
    char* str = getSectionsStringTable(fichierAnalyse, elfHdr);

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
        printf("type : %u\n", allSectHdr[idx].sh_type); //a remplacer par leur �quivalent
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
            return idx;
        }
    }
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
        // 5 correspondant � l'octet �tant le big ou little
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
    //car on ne r�cupereras pas la bonne table
    fseek(fichierAnalyse, strsymbtab.sh_offset, SEEK_SET);
    char * str = malloc(strsymbtab.sh_size);
    for (i = 0; i < strsymbtab.sh_size; i++) {
        str[i] = fgetc(fichierAnalyse);
    }

    int k = symbtab.sh_size / sizeof (Elf32_Sym);

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

/* ----- RELOCATIONS TABLE ----- */

Elf32_Rel createObjectRelocations(FILE* fichierAnalyse, Elf32_Shdr sect, int index, Elf32_Ehdr elfHdr) {
    Elf32_Rel rel;
    fseek(fichierAnalyse, sect.sh_offset + index * sizeof (Elf32_Rel), SEEK_SET);
    fread(&rel, sizeof (Elf32_Rel), 1, fichierAnalyse);
    if (elfHdr.e_ident[5] == MODE_BIG_ENDIAN) { // 5 correspondant � l'octet �tant le big ou little
        rel.r_info = __bswap_32(rel.r_info);
        rel.r_offset = __bswap_32(rel.r_offset);
    }
    return rel;
}

RelAndLink * createAllRelocationBySection(FILE* fichierAnalyse, int nbent, Elf32_Shdr sect, Elf32_Ehdr elfHdr) {
    int i;
    RelAndLink* tab = malloc(nbent * sizeof (RelAndLink));
    for (i = 0; i < nbent; i++) {
        tab[i].rel = createObjectRelocations(fichierAnalyse, sect, i, elfHdr);
        tab[i].link = sect.sh_link;
    }
    return tab;
}

RelAndLink** createAllRelocations(FILE* fichierAnalyse, Elf32_Ehdr elfHdr, Elf32_Shdr * allSectHdr) {
    int i;
    int nb_sect_rel = 0;
    for (i = 0; i < elfHdr.e_shnum; i++) {
        if (allSectHdr[i].sh_type == SHT_REL) {
            nb_sect_rel++;
        }
    }
    RelAndLink* allRelForSec;
    RelAndLink** allSectRel = malloc(nb_sect_rel * sizeof (RelAndLink*));
    int l = 0;
    for (i = 0; i < elfHdr.e_shnum; i++) {
        if (allSectHdr[i].sh_type == SHT_REL) {
            int nb_ent = allSectHdr[i].sh_size / sizeof (Elf32_Rel);
            allRelForSec = createAllRelocationBySection(fichierAnalyse, nb_ent, allSectHdr[i], elfHdr);
            allSectRel[l] = allRelForSec;
            l++;
        }
    }
    return allSectRel;
}

void readRelocations(FILE* fichierAnalyse, Elf32_Ehdr elfHdr, Elf32_Shdr * allSectHdr) {
    int i;
    int nb_sect_rel = 0;
    for (i = 0; i < elfHdr.e_shnum; i++) {
        if (allSectHdr[i].sh_type == SHT_REL) {
            nb_sect_rel++;
        }
    }
    int* tab_ind_sect_rel = malloc(nb_sect_rel * sizeof (int));
    RelAndLink** allSectRel = malloc(nb_sect_rel * sizeof (RelAndLink*));
    int l = 0;
    for (i = 0; i < elfHdr.e_shnum; i++) {
        if (allSectHdr[i].sh_type == SHT_REL) {
            int nb_ent = allSectHdr[i].sh_size / sizeof (Elf32_Rel);
            RelAndLink* allRelForSec = createAllRelocationBySection(fichierAnalyse, nb_ent, allSectHdr[i], elfHdr);
            allSectRel[l] = allRelForSec;
            tab_ind_sect_rel[l] = i;
            l++;
        }
    }
    affichageRelocations(allSectRel, tab_ind_sect_rel, nb_sect_rel, fichierAnalyse, elfHdr);
    free(allSectRel);
    free(allSectHdr);
    free(tab_ind_sect_rel);
}

void affichageRelocations(RelAndLink** allRel, int* tab_ind_sect_rel, int nb_sect_rel, FILE* fichierAnalyse, Elf32_Ehdr elfHdr) {
    Elf32_Shdr* allSect = createAllObjectSectionHeader(fichierAnalyse, elfHdr);

    char* str = getSectionsStringTable(fichierAnalyse, elfHdr);
    int k, n = 0;
    for (k = 0; k < nb_sect_rel; k++) {
        int nb_ent_current = allSect[tab_ind_sect_rel[k]].sh_size / sizeof (Elf32_Rel);
        printf("Section de relocalisation ");
        int i = allSect[tab_ind_sect_rel[k]].sh_name;
        while (str[i] != '\0') {
            printf("%c", str[i]);
            i++;
        }
        printf(" a l adresse de decalage 0x%x contient %d entrees:\n", allSect[tab_ind_sect_rel[k]].sh_offset, nb_ent_current);
        printf("Decalage\tInfo\t\tType\n");
        for (n = 0; n < nb_ent_current; n++) {
            printf("%08x\t%08x\t%08x\n", allRel[k][n].rel.r_offset, allRel[k][n].rel.r_info, ELF32_R_TYPE(allRel[k][n].rel.r_info));
        }
        printf("\n");
    }
}

Elf32_Shdr* createObjectSectionHeaderWithoutRelocalisations(Elf32_Ehdr elfHdr, Elf32_Shdr* allSectHdr) {

    int nbSections = elfHdr.e_shnum;
    int i;
    int j = 0; //indice pour le tableau des sections
    int nbSectionsNonRelocalises = countNbSectionsNonRelocalisesByAllSectionHeader(elfHdr, allSectHdr);
    //printf("NB SECTIONS NON RELOC : %d", nbSectionsNonRelocalises);
    Elf32_Shdr* objSectHdrWithoutRelocalisations = malloc(sizeof (Elf32_Shdr) * nbSectionsNonRelocalises);
    for (i = 0; i < nbSections; i++) {

        if (allSectHdr[i].sh_type == SHT_REL) {
            //printf("La section %d a une relocalisation\n", i);
            //On ne fait rien. On ne recupere pas la section
        } else {
            objSectHdrWithoutRelocalisations[j] = allSectHdr[i];
            j++;

        }

        //printf("section numero %d : %d \n", i, allSectHdr[i].sh_type);
    }
    return objSectHdrWithoutRelocalisations;
}

int countNbSectionsNonRelocalisesByAllSectionHeader(Elf32_Ehdr elfHdr, Elf32_Shdr* allSectHdr) {
    int indiceNbCasesTabSansRelocalisations = 0;
    int nbSections = elfHdr.e_shnum;
    int i;
    for (i = 0; i < nbSections; i++) {
        if (allSectHdr[i].sh_type == SHT_REL) {
            //printf("La section %d a une relocalisation\n", i);
            //On ne fait rien. On ne recupere pas la section
        } else {
            indiceNbCasesTabSansRelocalisations++;
        }
    }
    return indiceNbCasesTabSansRelocalisations;
}

Elf32_Shdr* createObjectSectionHeaderRelocalisations(Elf32_Ehdr elfHdr, Elf32_Shdr* allSectHdr) {

    int nbSections = elfHdr.e_shnum;
    int i;
    int j = 0; //indice pour le tableau des sections
    int nbSectionsRelocalises = countNbSectionsRelocalisesByAllSectionHeader(elfHdr, allSectHdr);
    //printf("NB SECTIONS NON RELOC : %d", nbSectionsNonRelocalises);
    Elf32_Shdr* objSectHdrRelocalisations = malloc(sizeof (Elf32_Shdr) * nbSectionsRelocalises);
    for (i = 0; i < nbSections; i++) {

        if (allSectHdr[i].sh_type == SHT_REL) {
            objSectHdrRelocalisations[j] = allSectHdr[i];
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