#include "read_relocations.h"
#include "../etape2/read_header.c"
#define MODE_BIG_ENDIAN 2

Elf32_Rel createObjectRelocations(char* nameFile, Elf32_Shdr sect, int index) {
    Elf32_Rel rel;
    FILE* fichierAnalyse = fopen(nameFile, "r");
    Elf32_Ehdr elfHdr = createObjectEnteteELF(nameFile);
    fseek(fichierAnalyse, sect.sh_offset + index * sizeof (Elf32_Rel), SEEK_SET);

    fread(&rel, sizeof (Elf32_Rel), 1, fichierAnalyse);

    if (elfHdr.e_ident[5] == MODE_BIG_ENDIAN) { // 5 correspondant à l'octet étant le big ou little
        rel.r_info = __bswap_32(rel.r_info);
        rel.r_offset = __bswap_32(rel.r_offset);
    }

    fclose(fichierAnalyse);
    return rel;
}

Elf32_Rel* createAllRelocationBySection(char* nameFile, int nbent, Elf32_Shdr sect) {
    int i;
    Elf32_Ehdr elfHdr = createObjectEnteteELF(nameFile);
    Elf32_Shdr* allSectHdr = createAllObjectSectionHeader(nameFile);

    int nb_ent_tot = 0;
    for (i = 0; i < elfHdr.e_shnum; i++) {
        if (allSectHdr[i].sh_type == SHT_REL) {
            nb_ent_tot += allSectHdr[i].sh_size / allSectHdr[i].sh_entsize;
        }
    }
    Elf32_Rel* tab = malloc(nb_ent_tot * sizeof (Elf32_Rel));
    
    for (i = 0; i < nbent; i++) {
        tab[i] = createObjectRelocations(nameFile, sect, i);
    }
    return tab;
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
    int nb_ent_tot = 0;
    for (i = 0; i < elfHdr.e_shnum; i++) {
        if (allSectHdr[i].sh_type == SHT_REL) {
            nb_ent_tot += allSectHdr[i].sh_size / allSectHdr[i].sh_entsize;
        }
    }
    int* tab_ind_sect_rel = malloc(nb_sect_rel * sizeof (int));
    Elf32_Rel* allRelForSec = malloc(nb_ent_tot * sizeof (Elf32_Rel));
    Elf32_Rel** allSectRel = malloc(nb_sect_rel * sizeof (Elf32_Rel*));
    int l = 0;
    for (i = 0; i < elfHdr.e_shnum; i++) {
        if (allSectHdr[i].sh_type == SHT_REL) {
            
            int nb_ent = allSectHdr[i].sh_size / allSectHdr[i].sh_entsize;
            allRelForSec = createAllRelocationBySection(nameFile, nb_ent, allSectHdr[i]);
            allSectRel[l] = allRelForSec;
            tab_ind_sect_rel[l] = i;
            l++;
        }
    }
    affichage_relocations(allSectRel, tab_ind_sect_rel, nb_ent_tot, nb_sect_rel, nameFile);
    free(allSectRel);
    free(allSectHdr);
    free(tab_ind_sect_rel);
}

//revoir arg1
void affichage_relocations(Elf32_Rel** allRel, int* tab_ind_sect_rel, int nb_ent_tot, int nb_sect_rel, char* nameFile) {
    Elf32_Shdr* allSect = createAllObjectSectionHeader(nameFile);
    int k, n = 0;
    int currentRel = 0;
    for (k = 0; k < nb_sect_rel; k++) {
        int nb_ent_current = allSect[tab_ind_sect_rel[k]].sh_size / allSect[tab_ind_sect_rel[k]].sh_entsize;
        int nextRelSect = currentRel + nb_ent_current;
        printf("Section de relocalisation %u a l adresse de decalage 0x%x contient %d entrees:\n", allSect[tab_ind_sect_rel[k]].sh_name, allSect[tab_ind_sect_rel[k]].sh_offset, nb_ent_current);
        printf("Info\tDecalage\n");
        for (n = currentRel; n < nextRelSect; n++) {
            printf("%08x\t%08x\n", allRel[k][n].r_info, allRel[k][n].r_offset);
        }
        printf("\n");
        currentRel = nextRelSect;
    }
}