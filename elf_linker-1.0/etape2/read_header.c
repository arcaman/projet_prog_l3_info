#include "read_header.h"
#include "../en_tete_elf.c"
#define MODE_BIG_ENDIAN 2

Elf32_Shdr createObjectSectionheader(char* nameFile, int index){
    Elf32_Shdr shdr;
    FILE* fichierAnalyse = fopen(nameFile, "r");
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
    
    fclose(fichierAnalyse);
    return shdr;
}

void read_section_header(char * filename, size_t size){
    FILE* ElfFile = NULL;
    char* SectNames = NULL;
    Elf32_Ehdr elfHdr;
    Elf32_Shdr sectHdr;
    uint32_t idx;
    ElfFile = fopen(filename, "r");
    // read ELF header, first thing in the file
    elfHdr = createObjectEnteteELF(filename);
    printf("nb sections : %i\n",elfHdr.e_shnum); 
    
    
    
    // read section name string table
    // first, read its header.
    fseek(ElfFile, elfHdr.e_shoff + elfHdr.e_shstrndx * sizeof(Elf32_Shdr), SEEK_SET);
    fread(&sectHdr, 1, sizeof(Elf32_Shdr), ElfFile);
    // next, read the section, string data
    //printf("sh_size = %llu\n", (long long unsigned int)sectHdr.sh_size);
    SectNames = malloc(sectHdr.sh_size);
    fseek(ElfFile, sectHdr.sh_offset, SEEK_SET);
    fread(SectNames, 1, sectHdr.sh_size, ElfFile);

    
    // read all section headers
    for (idx = 0; idx < elfHdr.e_shnum; idx++){
        fseek(ElfFile, elfHdr.e_shoff + idx * sizeof(Elf32_Shdr), SEEK_SET);
        fread(&sectHdr, 1, sizeof(Elf32_Shdr), ElfFile);
        //printf("section numero %i : \n",idx);
    }
}

