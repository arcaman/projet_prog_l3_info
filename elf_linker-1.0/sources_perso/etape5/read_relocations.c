#include "read_relocations.h"
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

void createAllRelocationBySection(char* nameFile, Elf32_Rel* tab, int nbent, Elf32_Shdr sect, int ind_tab) {
    int i;
    for (i = ind_tab; i < ind_tab + nbent; i++) {
        tab[i] = createObjectRelocations(nameFile, sect, i - ind_tab);
    }
}

void read_relocations(char * nameFile) {
    Elf32_Ehdr elfHdr = createObjectEnteteELF(nameFile);
    Elf32_Shdr* allSectHdr = malloc(elfHdr.e_shnum * sizeof (Elf32_Shdr));
    createAllObjectSectionHeader(nameFile, allSectHdr, elfHdr.e_shnum);
    int nb_sect_rel = count_reloc_sect(elfHdr, allSectHdr);
    int nb_ent_tot = size_needed_relocations_table(elfHdr, allSectHdr);
    int* tab_ind_sect_rel = malloc(nb_sect_rel * sizeof (int));
    Elf32_Rel* allSectRel = malloc(nb_ent_tot * sizeof (Elf32_Rel));
    int i;
    int l = 0;
    int ind_tab = 0;
    for (i = 0; i < elfHdr.e_shnum; i++) {
        if (allSectHdr[i].sh_type == SHT_REL) {
            tab_ind_sect_rel[l] = i;
            l++;
            int nb_ent = allSectHdr[i].sh_size / allSectHdr[i].sh_entsize;
            createAllRelocationBySection(nameFile, allSectRel, nb_ent, allSectHdr[i], ind_tab);
            ind_tab += nb_ent;
        }
    }
    affichage_relocations(allSectRel, allSectHdr, tab_ind_sect_rel, nb_ent_tot, nb_sect_rel);
    free(allSectRel);
    free(allSectHdr);
    free(tab_ind_sect_rel);
}

int size_needed_relocations_table(Elf32_Ehdr elfHdr, Elf32_Shdr* allSectHdr) {
    int i, nb_ent_tot = 0;
    for (i = 0; i < elfHdr.e_shnum; i++) {
        if (allSectHdr[i].sh_type == SHT_REL) {
            nb_ent_tot += allSectHdr[i].sh_size / allSectHdr[i].sh_entsize;
        }
    }
    return nb_ent_tot;
}

int count_reloc_sect(Elf32_Ehdr elfHdr, Elf32_Shdr* allSectHdr) {
    int i;
    int nb = 0;
    for (i = 0; i < elfHdr.e_shnum; i++) {
        if (allSectHdr[i].sh_type == SHT_REL) {
            nb++;
        }
    }
    return nb;
}

void affichage_relocations(Elf32_Rel* allRel, Elf32_Shdr* allSect, int* tab_ind_sect_rel, int nb_ent_tot, int nb_sect_rel) {
    int k, n = 0;
    int currentRel = 0;
    for (k = 0; k < nb_sect_rel; k++) {
        int nb_ent_current = allSect[tab_ind_sect_rel[k]].sh_size / allSect[tab_ind_sect_rel[k]].sh_entsize;
        int nextRelSect = currentRel + nb_ent_current;
        printf("Section de relocalisation %u a l adresse de decalage 0x%x contient %d entrees:\n", allSect[tab_ind_sect_rel[k]].sh_name, allSect[tab_ind_sect_rel[k]].sh_offset, nb_ent_current);
        printf("Decalage\tInfo\n");
        for (n = currentRel; n < nextRelSect; n++) {
            printf("%08x\t", allRel[n].r_info);
            printf("%08x\n", allRel[n].r_offset);
        }
        printf("\n");
        currentRel = nextRelSect;
    }
}