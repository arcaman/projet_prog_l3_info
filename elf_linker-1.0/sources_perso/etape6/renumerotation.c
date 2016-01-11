#include "./renumerotation.h"
#include "../etape5/read_relocations.c"




void renumerotation(char* nameFile){
    char* newNameFile = "newFile.o";
//    strcat(newNameFile, nameFile);
    FILE* newFile = fopen(newNameFile,"w");
    Elf32_Ehdr ehdr = createObjectEnteteELF(nameFile);
    Elf32_Shdr* shdr = createAllObjectSectionHeader(nameFile);
    //Elf32_Rel** rel = createAllRelocations(nameFile);
    int nbSect = ehdr.e_shnum;
    int i;
    fwrite(&ehdr,sizeof(Elf32_Ehdr),1,newFile);
    for(i=0;i<nbSect;i++){
        if (shdr[i].sh_type==SHT_REL){
            
        }
    }
}
