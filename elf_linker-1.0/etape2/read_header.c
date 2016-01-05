#include "read_header.h"

void read_header(char * filename, size_t size){
FILE* ElfFile = NULL;
char* SectNames = NULL;
Elf32_Ehdr elfHdr;
Elf32_Shdr sectHdr;
uint32_t idx;
ElfFile = fopen(filename, "r");
// read ELF header, first thing in the file
fread(&elfHdr, 1, sizeof(elfHdr), ElfFile);
// read section name string table
// first, read its header.
fseek(ElfFile, elfHdr.e_shoff + elfHdr.e_shstrndx * sizeof(sectHdr), SEEK_SET);
fread(&sectHdr, 1, sizeof(sectHdr), ElfFile);
// next, read the section, string data
// printf("sh_size = %llu\n", sectHdr.sh_size);
SectNames = malloc(sectHdr.sh_size);
fseek(ElfFile, sectHdr.sh_offset, SEEK_SET);
fread(SectNames, 1, sectHdr.sh_size, ElfFile);

// read all section headers
for (idx = 0; idx < elfHdr.e_shnum; idx++){
    fseek(ElfFile, elfHdr.e_shoff + idx * sizeof(sectHdr), SEEK_SET);
    fread(&sectHdr, 1, sizeof(sectHdr), ElfFile);
    printf("section numero %i : \n",idx);
}
}


