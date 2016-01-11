#include "read_relocations.h"
#include "../etape2/read_header.c"
#define MODE_BIG_ENDIAN 2

Elf32_Rel createObjectRelocations(char* nameFile, Elf32_Shdr sect, int index) {
    Elf32_Rel rel;
    FILE* fichierAnalyse = fopen(nameFile, "r");
    Elf32_Ehdr elfHdr = createObjectEnteteELF(nameFile);
    fseek(fichierAnalyse, sect.sh_offset + index * sizeof (Elf32_Rel), SEEK_SET);
    fread(&rel, sizeof (Elf32_Rel), 1, fichierAnalyse);
    if (elfHdr.e_ident[5] == MODE_BIG_ENDIAN) { // 5 correspondant � l'octet �tant le big ou little
        rel.r_info = __bswap_32(rel.r_info);
        rel.r_offset = __bswap_32(rel.r_offset);
    }
    fclose(fichierAnalyse);
    return rel;
}

RelAndLink* createAllRelocationBySection(char* nameFile, int nbent, Elf32_Shdr sect) {
    int i;
    RelAndLink* tab = malloc(nbent * sizeof (RelAndLink));
    for (i = 0; i < nbent; i++) {
        tab[i].rel = createObjectRelocations(nameFile, sect, i);
        tab[i].link = sect.sh_link;
    }
    return tab;
}

RelAndLink** createAllRelocations(char * nameFile) {
    Elf32_Ehdr elfHdr = createObjectEnteteELF(nameFile);
    Elf32_Shdr* allSectHdr = createAllObjectSectionHeader(nameFile);
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
            allRelForSec = createAllRelocationBySection(nameFile, nb_ent, allSectHdr[i]);
            allSectRel[l] = allRelForSec;
            l++;
        }
    }
    return allSectRel;
}

void readRelocations(char * nameFile) {
    Elf32_Ehdr elfHdr = createObjectEnteteELF(nameFile);
    Elf32_Shdr* allSectHdr = createAllObjectSectionHeader(nameFile);
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
            RelAndLink* allRelForSec = createAllRelocationBySection(nameFile, nb_ent, allSectHdr[i]);
            allSectRel[l] = allRelForSec;
            tab_ind_sect_rel[l] = i;
            l++;
        }
    }
    affichageRelocations(allSectRel, tab_ind_sect_rel, nb_sect_rel, nameFile);
    free(allSectRel);
    free(allSectHdr);
    free(tab_ind_sect_rel);
}

void affichageRelocations(RelAndLink** allRel, int* tab_ind_sect_rel, int nb_sect_rel, char* nameFile) {
    Elf32_Shdr* allSect = createAllObjectSectionHeader(nameFile);

    char* str = getSectionsStringTable(nameFile);
    int k, n = 0;
    for (k = 0; k < nb_sect_rel; k++) {
        int nb_ent_current = allSect[tab_ind_sect_rel[k]].sh_size / sizeof(Elf32_Rel);
        printf("Section de relocalisation ");
        int i = allSect[tab_ind_sect_rel[k]].sh_name;
        while (str[i] != '\0') {
            printf("%c", str[i]);
            i++;
        }
        printf(" a l adresse de decalage 0x%x contient %d entrees:\n", allSect[tab_ind_sect_rel[k]].sh_offset, nb_ent_current);
        printf("Decalage\tInfo\t\tType\n");
        for (n = 0; n < nb_ent_current; n++) {
            printf("%08x\t%08x\t%08x\n", allRel[k][n].rel.r_offset, allRel[k][n].rel.r_info,ELF32_R_TYPE(allRel[k][n].rel.r_info));
        }
        printf("\n");
    }
}