#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include "../etape2/read_header.c"

#define SHT_SYMTAB 2

void afficher_table_symb(char * filename){
    Elf32_Shdr* sectiontab;
    Elf32_Shdr symbtab,strsymbtab;
    Elf32_Ehdr elfHdr = createObjectEnteteELF(filename);
    createAllObjectSectionHeader(filename, sectiontab, elfHdr.e_shnum);
    int i;
    for(i = 0;i < elfHdr.e_shnum;i++){
        if(sectiontab[i].sh_type==SHT_SYMTAB){
            symbtab = sectiontab[i];
            strsymbtab = sectiontab[i+1];
        }
    }
}